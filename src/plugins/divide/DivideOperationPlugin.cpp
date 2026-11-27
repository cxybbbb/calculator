#include "DivideOperationPlugin.h"

#include <cmath>

QString DivideOperationPlugin::pluginId() const
{
    return QStringLiteral("org.qtcalculator.divide");
}

QString DivideOperationPlugin::pluginName() const
{
    return QStringLiteral("Divide Operation");
}

QString DivideOperationPlugin::pluginVersion() const
{
    return QStringLiteral("1.0.0");
}

QList<OperationDescriptor> DivideOperationPlugin::operations() const
{
    return QList<OperationDescriptor>()
            << OperationDescriptor(QStringLiteral("divide"), QStringLiteral("除法"), 2);
}

OperationResult DivideOperationPlugin::calculate(const QString &operationId,
                                                 const QList<double> &operands) const
{
    if (operationId != QStringLiteral("divide"))
        return OperationResult::failure(OperationResult::UnknownOperation);
    if (operands.size() != 2)
        return OperationResult::failure(OperationResult::InvalidOperandCount);

    // -0.0 == 0.0 成立，因此 “除以 -0” 也会被这里拦住。
    if (operands.at(1) == 0.0)
        return OperationResult::failure(OperationResult::DivisionByZero);

    const double result = operands.at(0) / operands.at(1);
    if (!std::isfinite(result))
        return OperationResult::failure(OperationResult::NonFiniteResult);
    return OperationResult::success(result);
}
