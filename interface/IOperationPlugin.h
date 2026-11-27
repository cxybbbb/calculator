#ifndef CALCULATOR_IOPERATIONPLUGIN_H
#define CALCULATOR_IOPERATIONPLUGIN_H

#include <QList>
#include <QString>
#include <QtPlugin>

struct OperationDescriptor
{
    OperationDescriptor()
        : operandCount(0)
    {
    }

    OperationDescriptor(const QString &operationId,
                        const QString &operationDisplayName,
                        int operationOperandCount)
        : id(operationId)
        , displayName(operationDisplayName)
        , operandCount(operationOperandCount)
    {
    }

    QString id;
    QString displayName;
    int operandCount;
};

struct OperationResult
{
    enum Error {
        NoError,
        UnknownOperation,
        InvalidOperandCount,
        DivisionByZero,
        DomainError,
        NonFiniteResult,
        InternalError
    };

    OperationResult()
        : value(0.0)
        , error(InternalError)
    {
    }

    static OperationResult success(double resultValue)
    {
        OperationResult result;
        result.value = resultValue;
        result.error = NoError;
        return result;
    }

    static OperationResult failure(Error resultError, const QString &details = QString())
    {
        OperationResult result;
        result.error = resultError == NoError ? InternalError : resultError;
        result.errorDetails = details;
        return result;
    }

    bool succeeded() const
    {
        return error == NoError;
    }

    double value;
    Error error;
    QString errorDetails;
};

class IOperationPlugin
{
public:
    virtual ~IOperationPlugin() {}

    virtual QString pluginId() const = 0;
    virtual QString pluginName() const = 0;
    virtual QString pluginVersion() const = 0;
    virtual QList<OperationDescriptor> operations() const = 0;
    virtual OperationResult calculate(const QString &operationId,
                                      const QList<double> &operands) const = 0;
};

#define CALCULATOR_OPERATION_PLUGIN_IID "org.qtcalculator.IOperationPlugin/1.0"
Q_DECLARE_INTERFACE(IOperationPlugin, CALCULATOR_OPERATION_PLUGIN_IID)

#endif
