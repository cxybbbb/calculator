#include "MultiplyOperationPlugin.h"

#include <cmath>

QString MultiplyOperationPlugin::pluginId() const
{
    return QStringLiteral("org.qtcalculator.multiply");
}

QString MultiplyOperationPlugin::pluginName() const
{
    return QStringLiteral("Multiply Operation");
}

QString MultiplyOperationPlugin::pluginVersion() const
{
    return QStringLiteral("1.0.0");
}

QList<OperationDescriptor> MultiplyOperationPlugin::operations() const
{
    return QList<OperationDescriptor>()
            << OperationDescriptor(QStringLiteral("multiply"), QStringLiteral("乘法"), 2);
}

OperationResult MultiplyOperationPlugin::calculate(const QString &operationId,
                                                   const QList<double> &operands) const
{
    if (operationId != QStringLiteral("multiply"))
        return OperationResult::failure(OperationResult::UnknownOperation);
    if (operands.size() != 2)
        return OperationResult::failure(OperationResult::InvalidOperandCount);

    const double result = operands.at(0) * operands.at(1);
    if (!std::isfinite(result))
        return OperationResult::failure(OperationResult::NonFiniteResult);
    return OperationResult::success(result);
}
