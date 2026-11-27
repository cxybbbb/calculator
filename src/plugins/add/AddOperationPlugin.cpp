#include "AddOperationPlugin.h"

#include <cmath>

QString AddOperationPlugin::pluginId() const
{
    return QStringLiteral("org.qtcalculator.add");
}

QString AddOperationPlugin::pluginName() const
{
    return QStringLiteral("Add Operation");
}

QString AddOperationPlugin::pluginVersion() const
{
    return QStringLiteral("1.0.0");
}

QList<OperationDescriptor> AddOperationPlugin::operations() const
{
    return QList<OperationDescriptor>()
            << OperationDescriptor(QStringLiteral("add"), QStringLiteral("加法"), 2);
}

OperationResult AddOperationPlugin::calculate(const QString &operationId,
                                              const QList<double> &operands) const
{
    if (operationId != QStringLiteral("add"))
        return OperationResult::failure(OperationResult::UnknownOperation);
    if (operands.size() != 2)
        return OperationResult::failure(OperationResult::InvalidOperandCount);

    const double result = operands.at(0) + operands.at(1);
    if (!std::isfinite(result))
        return OperationResult::failure(OperationResult::NonFiniteResult);
    return OperationResult::success(result);
}
