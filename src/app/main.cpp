#include "mainwindow.h"

#include "OperationRegistry.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    // 插件固定放在可执行程序同级的 plugins/ 目录下，与 app.pro 中的 DESTDIR 对应。
    const QString pluginDirectory = QDir(QCoreApplication::applicationDirPath())
            .filePath(QStringLiteral("plugins"));

    OperationRegistry registry;
    QStringList errors;
    if (!registry.loadFrom(pluginDirectory, &errors)) {
        QMessageBox::critical(nullptr,
                              QStringLiteral("插件加载失败"),
                              errors.join(QLatin1Char('\n')));
        return 1;
    }

    // 缺少任何一个四则运算插件都不进入主界面，避免按钮点了才算不出来。
    const QStringList requiredOperations = QStringList()
            << QStringLiteral("add")
            << QStringLiteral("subtract")
            << QStringLiteral("multiply")
            << QStringLiteral("divide");
    const QStringList missing = registry.missingOperations(requiredOperations);
    if (!missing.isEmpty()) {
        QMessageBox::critical(nullptr,
                              QStringLiteral("插件加载失败"),
                              QStringLiteral("缺少运算插件：%1")
                              .arg(missing.join(QStringLiteral(", "))));
        return 1;
    }

    // registry 在 window 之前构造，因此后析构：窗口销毁时插件仍然有效。
    MainWindow window(&registry);
    window.show();
    return application.exec();
}
