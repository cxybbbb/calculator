#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "OperationRegistry.h"

#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <cmath>
//45
MainWindow::MainWindow(OperationRegistry *registry, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_registry(registry)
{
    ui->setupUi(this);

    // 结果由程序写入，避免用户手动改出一个与运算无关的值。
    ui->result->setReadOnly(true);

    connect(ui->addButton, &QPushButton::clicked,
            this, &MainWindow::selectAddOperation);
    connect(ui->subtractButton, &QPushButton::clicked,
            this, &MainWindow::selectSubtractOperation);
    connect(ui->multiplyButton, &QPushButton::clicked,
            this, &MainWindow::selectMultiplyOperation);
    connect(ui->divideButton, &QPushButton::clicked,
            this, &MainWindow::selectDivideOperation);
    connect(ui->pushButton, &QPushButton::clicked,
            this, &MainWindow::calculate);

    // 输入完第二个操作数直接回车，等价于点击“=”。
    connect(ui->number2, &QLineEdit::returnPressed,
            this, &MainWindow::calculate);

    // 改动任一操作数后清掉旧结果，避免用户看到过期数字。
    connect(ui->number1, &QLineEdit::textChanged, this, [this]() {
        ui->result->clear();
    });
    connect(ui->number2, &QLineEdit::textChanged, this, [this]() {
        ui->result->clear();
    });

    // 界面默认就是加法，与 label_2 上的“+”保持一致。
    selectOperation(QStringLiteral("add"), QStringLiteral("+"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectAddOperation()
{
    selectOperation(QStringLiteral("add"), QStringLiteral("+"));
}

void MainWindow::selectSubtractOperation()
{
    selectOperation(QStringLiteral("subtract"), QStringLiteral("-"));
}

void MainWindow::selectMultiplyOperation()
{
    selectOperation(QStringLiteral("multiply"), QStringLiteral("*"));
}

void MainWindow::selectDivideOperation()
{
    selectOperation(QStringLiteral("divide"), QStringLiteral("/"));
}

void MainWindow::selectOperation(const QString &operationId, const QString &symbol)
{
    m_operationId = operationId;
    ui->label_2->setText(symbol);
    ui->result->clear();
}

void MainWindow::calculate()
{
    double leftOperand = 0.0;
    if (!readOperand(ui->number1, QStringLiteral("第一个操作数"), &leftOperand))
        return;

    double rightOperand = 0.0;
    if (!readOperand(ui->number2, QStringLiteral("第二个操作数"), &rightOperand))
        return;

    if (m_operationId.isEmpty()) {
        showWarning(QStringLiteral("未选择运算"),
                    QStringLiteral("请先点击 +、-、*、/ 中的任意一个。"));
        return;
    }

    const OperationResult result = m_registry->execute(
            m_operationId, QList<double>() << leftOperand << rightOperand);

    if (!result.succeeded()) {
        ui->result->clear();
        switch (result.error) {
        case OperationResult::DivisionByZero:
            showWarning(QStringLiteral("无法计算"), QStringLiteral("除数不能为 0。"));
            break;
        case OperationResult::NonFiniteResult:
            showWarning(QStringLiteral("无法计算"), QStringLiteral("结果超出可表示范围。"));
            break;
        case OperationResult::UnknownOperation:
            showWarning(QStringLiteral("无法计算"),
                        QStringLiteral("对应的运算插件不可用，请检查 plugins 目录。"));
            break;
        default:
            showWarning(QStringLiteral("无法计算"), QStringLiteral("运算失败。"));
            break;
        }
        return;
    }

    ui->result->setText(QString::number(result.value, 'g', 15));
}

bool MainWindow::readOperand(QLineEdit *editor,
                             const QString &operandName,
                             double *value)
{
    const QString text = editor->text().trimmed();
    if (text.isEmpty()) {
        showWarning(QStringLiteral("输入有误"), QStringLiteral("请填写%1。").arg(operandName));
        editor->setFocus();
        return false;
    }

    // QString::toDouble 同时承担“格式合法”和“能表示成 double”两项检查，
    // 小数点固定为 '.'，不跟随系统区域设置。
    bool ok = false;
    const double parsed = text.toDouble(&ok);
    if (!ok || !std::isfinite(parsed)) {
        showWarning(QStringLiteral("输入有误"),
                    QStringLiteral("「%1」不是有效数字，%2只能填写数字。")
                    .arg(text, operandName));
        editor->setFocus();
        editor->selectAll();
        return false;
    }

    *value = parsed;
    return true;
}

void MainWindow::showWarning(const QString &title, const QString &message)
{
    QMessageBox::warning(this, title, message);
}
