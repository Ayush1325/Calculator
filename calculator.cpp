#include "calculator.h"
#include "ui_calculator.h"
#include <QRegularExpression>

enum operation
{
    ADD,
    SUB,
    MUL,
    DIV
};

QVector<double> numList;
QVector<int> oprList;

Calculator::Calculator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Calculator)
{
    ui->setupUi(this);
    ui->Display->setText(QString::number(0));
    QPushButton *numBtns[10];
    for(int i = 0; i < 10; ++i)
    {
        QString btnName = "Btn" + QString::number(i);
        numBtns[i] = Calculator::findChild<QPushButton *>(btnName);
        connect(numBtns[i], SIGNAL(released()), this, SLOT(NumPressed()));
    }
    connect(ui->BtnDecimal, SIGNAL(released()), this, SLOT(NumPressed()));

    connect(ui->BtnAdd, SIGNAL(released()), this, SLOT(MathBtnPressed()));
    connect(ui->BtnSub, SIGNAL(released()), this, SLOT(MathBtnPressed()));
    connect(ui->BtnMul, SIGNAL(released()), this, SLOT(MathBtnPressed()));
    connect(ui->BtnDiv, SIGNAL(released()), this, SLOT(MathBtnPressed()));
    connect(ui->BtnEql, SIGNAL(released()), this, SLOT(EqualBtnPressed()));

    connect(ui->BtnAC, SIGNAL(released()), this, SLOT(ACBtnPressed()));
    connect(ui->BtnDel, SIGNAL(released()), this, SLOT(DelBtnPressed()));

    numList.append(0.0);
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::NumPressed()
{
    QPushButton *btn = (QPushButton *)sender();
    QString btnValue = btn->text();
    QString displayValue = ui->Display->text();
    if((displayValue.toDouble() == 0 || displayValue.toDouble() == 0.0) && displayValue.length() == 1 && displayValue != "-")
    {
        ui->Display->setText(btnValue);
    }
    else
    {
        QString newValue = displayValue + btnValue;
        ui->Display->setText(newValue);
    }
}

void Calculator::MathBtnPressed()
{
    QString displayVal = ui->Display->text();
    QPushButton *btn = (QPushButton *)sender();
    QString butValue = btn->text();
    if((displayVal.toDouble() == 0 || displayVal.toDouble() == 0.0) && displayVal.length() == 1 )
    {
        ui->Display->setText(butValue);
    }
    else
    {
        QString newValue = displayVal + butValue;
        ui->Display->setText(newValue);
    }
}

void Calculator::EqualBtnPressed()
{
    QString displayVal = ui->Display->text();
    QStringList nums = displayVal.split(QRegularExpression("[-+/*]"));
    QStringList::const_iterator i;
    for(i = nums.constBegin(); i != nums.constEnd(); ++i)
    {
        numList.append(i->toDouble());
    }

    QStringList opr = displayVal.split(QRegExp("[0-9]+[\\.]*[0-9]*"), QString::SkipEmptyParts);
    for(i = opr.constBegin(); i != opr.constEnd(); ++i)
    {
        if(*i == "+")
        {
            oprList.append(ADD);
        }
        else if(*i == "-")
        {
            oprList.append(SUB);
        }
        else if(*i == "*")
        {
            oprList.append(MUL);
        }
        else if(*i == "/")
        {
            oprList.append(DIV);
        }
    }

    if((numList.length() - opr.length()) == 2)
    {
        oprList.prepend(ADD);
    }

    while(oprList.contains(DIV))
    {
        int index = oprList.indexOf(DIV);
        double res = numList[index]/numList[index+1];
        oprList.remove(index);
        numList.remove(index+1);
        numList.replace(index, res);
    }
    while(oprList.contains(MUL))
    {
        int index = oprList.indexOf(MUL);
        double res = numList[index]*numList[index+1];
        oprList.remove(index);
        numList.remove(index+1);
        numList.replace(index, res);
    }
    while(oprList.contains(SUB))
    {
        int index = oprList.indexOf(SUB);
        double res = numList[index]-numList[index+1];
        oprList.remove(index);
        numList.remove(index+1);
        numList.replace(index, res);
    }
    while(oprList.contains(ADD))
    {
        int index = oprList.indexOf(ADD);
        double res = numList[index]+numList[index+1];
        oprList.remove(index);
        numList.remove(index+1);
        numList.replace(index, res);
    }

    QString sol = QString::number(numList[0]);
    ui->Display->setText(sol);
}

void Calculator::ACBtnPressed()
{
    numList.clear();
    oprList.clear();
    numList.append(0);
    ui->Display->setText("0");
}

void Calculator::DelBtnPressed()
{
    QString text = ui->Display->text();
    text.chop(1);
    ui->Display->setText(text);
}

void Calculator::AnsBtnPressed()
{

}
