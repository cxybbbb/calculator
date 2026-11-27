#include "SubtractOperationPlugin.h"

#include <cmath>

QString SubtractOperationPlugin::pluginId() const
{
    return QStringLiteral("org.qtcalculator.subtract");
}

QString SubtractOperationPlugin::pluginName() const
{
    return QStringLiteral("Subtract Operation");
}

QString SubtractOperationPlugin::pluginVersion() const
{
    return QStringLiteral("1.0.0");
}

QList<OperationDescriptor> SubtractOperationPlugin::operations() const
{
    return QList<OperationDescriptor>()
            << OperationDescriptor(QStringLiteral("subtract"), QStringLiteral("减法"), 2);
}

OperationResult SubtractOperationPlugin::calculate(const QString &operationId,
                                                   const QList<double> &operands) const
{
    if (operationId != QStringLiteral("subtract"))
        return OperationResult::failure(OperationResult::UnknownOperation);
    if (operands.size() != 2)
        return OperationResult::failure(OperationResult::InvalidOperandCount);

    // 顺序敏感：左操作数是 operands.at(0)。
    const double result = operands.at(0) - operands.at(1);
    if (!std::isfinite(result))
        return OperationResult::failure(OperationResult::NonFiniteResult);
    return OperationResult::success(result);
}
