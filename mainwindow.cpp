#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <math.h>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    digitBTNs = {{Qt::Key_0,ui->btnNum0},
                 {Qt::Key_1,ui->btnNum1},
                 {Qt::Key_2,ui->btnNum2},
                 {Qt::Key_3,ui->btnNum3},
                 {Qt::Key_4,ui->btnNum4},
                 {Qt::Key_5,ui->btnNum5},
                 {Qt::Key_6,ui->btnNum6},
                 {Qt::Key_7,ui->btnNum7},
                 {Qt::Key_8,ui->btnNum8},
                 {Qt::Key_9,ui->btnNum9},
                 };

    foreach(auto btn,digitBTNs)
         connect(btn,SIGNAL(clicked()),this,SLOT(btnNumClicked()));

//    connect(ui->btnNum0,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
//    connect(ui->btnNum1,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
//    connect(ui->btnNum2,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
//    connect(ui->btnNum3,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
//    connect(ui->btnNum4,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
//    connect(ui->btnNum5,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
//    connect(ui->btnNum6,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
//    connect(ui->btnNum7,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
//    connect(ui->btnNum8,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
//    connect(ui->btnNum9,SIGNAL(clicked()),this,SLOT(btnNumClicked()));

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
     }else if(op == "×"){
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

void MainWindow::on_btnsign_clicked()
{
    // 如果当前是 Error 状态，先清零
       if (operand == "Error") {
           on_btnClearAll_clicked();
           return;
       }

       // 如果当前没有任何输入，默认把 0 变成 -0（或者不理它，这里选择变为 -0）
       if (operand.isEmpty()) {
           operand = "-0";
       }
       // 如果当前字符串是以 "-" 开头，说明是负数，去掉 "-" 变正数
       else if (operand.startsWith("-")) {
           operand.remove(0, 1);
       }
       // 如果当前是正数（且不为 0），在前面加上 "-"
       else if (operand != "0") {
           operand.prepend("-");
       }

       // 更新界面显示
       ui->display->setText(operand);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // 1. 处理数字键 0-9 (复用你之前的 QMap 逻辑)
    if (digitBTNs.contains(event->key())) {
        digitBTNs[event->key()]->animateClick(100);
        return;
    }

    // 2. 处理四则运算和等号 (映射到对应的按钮，模拟点击动画)
    switch (event->key()) {
        case Qt::Key_Plus:
            ui->btnPlus->animateClick(100);
            break;
        case Qt::Key_Minus:
            ui->btnMinus->animateClick(100);
            break;
        case Qt::Key_Asterisk: // 键盘上的 * 号
        case Qt::Key_X:        // 防止有些键盘按 x 也是乘
            ui->btnMultiple->animateClick(100);
            break;
        case Qt::Key_Slash:    // 键盘上的 / 号
            ui->btnDivide->animateClick(100);
            break;
        case Qt::Key_Equal:    // 键盘上的 = 号
        case Qt::Key_Enter:    // 键盘上的回车键（小键盘的回车）
        case Qt::Key_Return:   // 键盘上的回车键（主键盘的回车）
            ui->btnEqual->animateClick(100);
            break;

        // 3. 处理功能键
        case Qt::Key_Period:   // 小数点
            ui->btnPeriod->animateClick(100);
            break;
        case Qt::Key_Backspace:// 退格键
            ui->btnDel->animateClick(100);
            break;
        case Qt::Key_Escape:   // ESC键 映射为 C (全部清除)
            ui->btnClear->animateClick(100);
            break;
        case Qt::Key_Delete:   // Delete键 映射为 CE (清除当前输入)
            ui->btnClearAll->animateClick(100);
            break;
        case Qt::Key_F9: // 按键盘 F9 切换正负号
               ui->btnsign->animateClick(100);
               break;

        default:
            // 如果没有匹配的按键，交给父类处理（保持良好的 Qt 习惯）
            QMainWindow::keyPressEvent(event);
            break;
    }
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


