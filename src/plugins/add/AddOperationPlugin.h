#ifndef CALCULATOR_ADDOPERATIONPLUGIN_H
#define CALCULATOR_ADDOPERATIONPLUGIN_H

#include "IOperationPlugin.h"

#include <QObject>

class AddOperationPlugin : public QObject, public IOperationPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID CALCULATOR_OPERATION_PLUGIN_IID)
    Q_INTERFACES(IOperationPlugin)

public:
    QString pluginId() const override;
    QString pluginName() const override;
    QString pluginVersion() const override;
    QList<OperationDescriptor> operations() const override;
    OperationResult calculate(const QString &operationId,
                              const QList<double> &operands) const override;
};

#endif
