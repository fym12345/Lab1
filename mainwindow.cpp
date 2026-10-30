#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <math.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnNum0,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum1,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum2,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum3,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum4,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum5,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum6,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum7,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum8,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum9,SIGNAL(clicked()),this,SLOT(btnNumClicked()));

    connect(ui->btnMultiple,SIGNAL(clicked()),this,SLOT(btnbinaryOperatorClicked()));
    connect(ui->btnPlus,SIGNAL(clicked()),this,SLOT(btnbinaryOperatorClicked()));
    connect(ui->btnMinus,SIGNAL(clicked()),this,SLOT(btnbinaryOperatorClicked()));
    connect(ui->btnDivide,SIGNAL(clicked()),this,SLOT(btnbinaryOperatorClicked()));

    connect(ui->btnpercentage,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnInverse,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSquare,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSqrt,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::calculation(bool *ok)
{
    double result=0;
    if (operands.size() == 2 && opcodes.size() > 0){
     //取操作数
     double operand1 = operands.front().toDouble();
     operands.pop_front();
     double operand2= operands.front().toDouble();
     operands.pop_front();

     //取操作符
     QString op = opcodes.front();
     opcodes.pop_front();

     if(op == "+"){
       result = operand1+operand2;
     }else if(op == "-"){
       result = operand1-operand2;
     }else if(op == "*"){
       result = operand1*operand2;
     }else if(op == "/"){
         if(operand2 == 0) {
                 if(ok) *ok = false; // 利用传入的指针传递错误状态
                 return "Error";     // 显示错误提示
             }
       result = operand1/operand2;
     }


     ui->statusbar->showMessage(QString("calculation is in progress: operands is %1,opcode is %2").arg(operands.size()).arg(opcodes.size()));
    }else
     ui->statusbar->showMessage(QString("operands is %1,opcode is %2").arg(operands.size()).arg(opcodes.size()));
    return QString::number(result);

}

void MainWindow::btnNumClicked()
{
    if (isCalculated) {
           operand = "";
           isCalculated = false;
       }

    QString digit = qobject_cast<QPushButton *>(sender())->text();

    if(digit == "0" && operand == "0")
        digit = "";
    if(operand == "0" && digit != "0")
        operand = "";

    operand += digit;
    ui->display->setText(operand);

}




void MainWindow::on_btnPeriod_clicked()
{
    if(!operand.contains("."))
       operand += qobject_cast<QPushButton *>(sender())->text();
    ui->display->setText(operand);
}


void MainWindow::on_btnDel_clicked()
{
operand = operand.left(operand.length() - 1);
ui->display->setText(operand);
}


void MainWindow::on_btnClearAll_clicked()
{
    operand.clear();
        operands.clear();
        opcodes.clear();
        isCalculated = false;
        ui->display->setText("0");
}


void MainWindow::on_btnClear_clicked()
{
    operand.clear();
    isCalculated = false; // 清除当前输入时，也取消已计算状态
    ui->display->setText("0");
}

void MainWindow::btnbinaryOperatorClicked()
{
    if (operand == "Error") {
        on_btnClearAll_clicked();
        return;
    }

    QString opcode = qobject_cast<QPushButton *>(sender())->text();

    if (isCalculated) {
        operands.clear();
        opcodes.clear();
        isCalculated = false;
    }

    if(operand != ""){
        operands.push_back(operand);
        operand = "";
    }

    // 如果队列里已经有2个数（说明是按了连算，比如 2 + 3 +）
    if (operands.size() == 2) {
        QString result = calculation(); // 算出 5
        ui->display->setText(result);

        // 【关键修复】把算出的 5 重新塞回队列作为下一次运算的第一个数
        operands.push_back(result);
    }

    // 把刚按下的操作符（+）塞进操作符队列
    opcodes.push_back(opcode);
}

void MainWindow::on_btnEqual_clicked()
{
    if (operand == "Error") {
        on_btnClearAll_clicked();
        return;
    }

    // 把最后输入的数字（4）放进队列
    if(operand != ""){
        operands.push_back(operand);
        operand = "";
    }

    // 此时队列里有：操作数[5, 4]，操作符[+]
    QString result = calculation();
    ui->display->setText(result);

    operand = result;
    isCalculated = true;

    // 算完后清空队列，迎接下一次全新计算
    operands.clear();
    opcodes.clear();
}

void MainWindow::btnUnaryOperatorClicked()
{
    if(operand != ""){
        double result = operand.toDouble();
        operand = "";

        QString op =qobject_cast<QPushButton*>(sender())->text();

        if(op == "%")
            result /=100.0;
        else if(op == "1/x")
            result = 1/result;
        else if(op == "x^2")
            result *= result;
        else if(op == "√")
            result = sqrt(result);

        operand = QString::number(result); // 算完存回 operand
                ui->display->setText(operand);
                isCalculated = true; // 单目运算算完，同样标记为已计算状态
    }
}


