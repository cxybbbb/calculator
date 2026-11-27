#ifndef CALCULATOR_OPERATIONREGISTRY_H
#define CALCULATOR_OPERATIONREGISTRY_H

#include "IOperationPlugin.h"

#include <QHash>
#include <QList>
#include <QObject>
#include <QStringList>

class QPluginLoader;

// 负责“找到插件并调用它”。Core 只认识接口和 operationId，
// 不认识任何一个具体插件类，因此新增运算不需要改动这里。
class OperationRegistry : public QObject
{
public:
    explicit OperationRegistry(QObject *parent = nullptr);
    ~OperationRegistry() override;

    bool loadFrom(const QString &directory, QStringList *errors);
    IOperationPlugin *find(const QString &operationId) const;
    QStringList missingOperations(const QStringList &operationIds) const;

    OperationResult execute(const QString &operationId,
                            const QList<double> &operands) const;

private:
    QHash<QString, IOperationPlugin *> m_operations;
    QList<QPluginLoader *> m_loaders;
};

#endif
