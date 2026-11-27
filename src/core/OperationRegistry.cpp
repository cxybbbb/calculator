#include "OperationRegistry.h"

#include <QDir>
#include <QFileInfo>
#include <QLibrary>
#include <QPluginLoader>
#include <QSet>

OperationRegistry::OperationRegistry(QObject *parent)
    : QObject(parent)
{
}

OperationRegistry::~OperationRegistry()
{
    // 先卸载再删除加载器：unload() 会销毁插件对象，
    // 保证销毁顺序是“插件对象 → 动态库句柄”，不会留下悬空指针。
    for (QPluginLoader *loader : m_loaders) {
        loader->unload();
        delete loader;
    }
    m_loaders.clear();
    m_operations.clear();
}

bool OperationRegistry::loadFrom(const QString &directoryPath, QStringList *errors)
{
    // 调用方可以不关心错误详情；这里用一个本地列表兜底，避免空指针判断散落各处。
    QStringList localErrors;
    QStringList *target = errors ? errors : &localErrors;

    if (!m_loaders.isEmpty()) {
        target->append(QStringLiteral("插件已经加载过，不能重复调用 loadFrom()。"));
        return false;
    }

    const QDir directory(directoryPath);
    if (!directory.exists()) {
        target->append(QStringLiteral("插件目录不存在：%1").arg(directoryPath));
        return false;
    }

    QSet<QString> pluginIds;
    QSet<QString> operationIds;

    const QFileInfoList files = directory.entryInfoList(QDir::Files, QDir::Name);
    for (const QFileInfo &file : files) {
        // Linux 上是 .so，Windows 上是 .dll，交给 Qt 判断而不是硬编码后缀。
        if (!QLibrary::isLibrary(file.fileName()))
            continue;

        QPluginLoader *loader = new QPluginLoader(file.absoluteFilePath(), this);
        QObject *instance = loader->instance();
        if (!instance) {
            target->append(QStringLiteral("无法加载 %1：%2")
                           .arg(file.fileName(), loader->errorString()));
            delete loader;
            continue;
        }

        // 跨 DLL 边界的安全类型检查：按接口 IID 比对，不依赖 C++ RTTI。
        IOperationPlugin *plugin = qobject_cast<IOperationPlugin *>(instance);
        if (!plugin) {
            target->append(QStringLiteral("%1 不是可用的运算插件。").arg(file.fileName()));
            loader->unload();
            delete loader;
            continue;
        }

        if (plugin->pluginId().isEmpty() || pluginIds.contains(plugin->pluginId())) {
            target->append(QStringLiteral("%1 的插件标识为空或重复。").arg(file.fileName()));
            loader->unload();
            delete loader;
            continue;
        }

        bool valid = true;
        QSet<QString> pluginOperationIds;
        const QList<OperationDescriptor> descriptors = plugin->operations();
        for (const OperationDescriptor &descriptor : descriptors) {
            if (descriptor.id.isEmpty()
                    || descriptor.operandCount < 0
                    || descriptor.operandCount > 2
                    || operationIds.contains(descriptor.id)
                    || pluginOperationIds.contains(descriptor.id)) {
                valid = false;
                break;
            }
            pluginOperationIds.insert(descriptor.id);
        }
        if (!valid || descriptors.isEmpty()) {
            target->append(QStringLiteral("%1 的运算描述为空或非法。").arg(file.fileName()));
            loader->unload();
            delete loader;
            continue;
        }

        pluginIds.insert(plugin->pluginId());
        for (const OperationDescriptor &descriptor : descriptors) {
            operationIds.insert(descriptor.id);
            m_operations.insert(descriptor.id, plugin);
        }
        m_loaders.append(loader);
    }

    if (m_operations.isEmpty()) {
        target->append(QStringLiteral("目录 %1 中没有可用的运算插件。").arg(directoryPath));
        return false;
    }
    return true;
}

IOperationPlugin *OperationRegistry::find(const QString &operationId) const
{
    return m_operations.value(operationId, nullptr);
}

QStringList OperationRegistry::missingOperations(const QStringList &operationIds) const
{
    QStringList missing;
    for (const QString &operationId : operationIds) {
        if (!m_operations.contains(operationId))
            missing.append(operationId);
    }
    return missing;
}

OperationResult OperationRegistry::execute(const QString &operationId,
                                           const QList<double> &operands) const
{
    IOperationPlugin *plugin = find(operationId);
    if (!plugin)
        return OperationResult::failure(OperationResult::UnknownOperation);

    return plugin->calculate(operationId, operands);
}
