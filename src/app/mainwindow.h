#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QLineEdit;
class OperationRegistry;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(OperationRegistry *registry, QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void selectAddOperation();
    void selectSubtractOperation();
    void selectMultiplyOperation();
    void selectDivideOperation();
    void calculate();

private:
    void selectOperation(const QString &operationId, const QString &symbol);
    bool readOperand(QLineEdit *editor,
                     const QString &operandName,
                     double *value);
    void showWarning(const QString &title, const QString &message);

    Ui::MainWindow *ui;
    OperationRegistry *m_registry;
    QString m_operationId;
};

#endif
