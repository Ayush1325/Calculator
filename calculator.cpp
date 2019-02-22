#include "calculator.h"
#include "ui_calculator.h"

enum operation
{
    ADD,
    SUB,
    MUL,
    DIV
};

double currentVal = 0.0;

double calcVal = 0.0;
bool addTrig;
bool subTrig;
bool mulTrig;
bool divTrig;

QVector<double> numList;
QVector<operation> oprList;

Calculator::Calculator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Calculator)
{
    ui->setupUi(this);
    ui->Display->setText(QString::number(calcVal));
    QPushButton *numBtns[10];
    for(int i = 0; i < 10; ++i)
    {
        QString btnName = "Btn" + QString::number(i);
        numBtns[i] = Calculator::findChild<QPushButton *>(btnName);
        connect(numBtns[i], SIGNAL(released()), this, SLOT(NumPressed()));
    }

    connect(ui->BtnAdd, SIGNAL(released()), this, SLOT(MathBtnPressed()));
    connect(ui->BtnSub, SIGNAL(released()), this, SLOT(MathBtnPressed()));
    connect(ui->BtnMul, SIGNAL(released()), this, SLOT(MathBtnPressed()));
    connect(ui->BtnDiv, SIGNAL(released()), this, SLOT(MathBtnPressed()));
    connect(ui->BtnEql, SIGNAL(released()), this, SLOT(EqualBtnPressed()));
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
    if((displayValue.toDouble() == 0 || displayValue.toDouble() == 0.0) && displayValue.length() == 1 )
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
    QString newVal = displayVal + butValue;
    ui->Display->setText(newVal);
}

void Calculator::EqualBtnPressed()
{
    double sol = 0.0;
    QString displayVal = ui->Display->text();
    double dsplVal = displayVal.toDouble();
    if(addTrig || subTrig || mulTrig || divTrig)
    {
        if(addTrig)
        {
            sol = calcVal + dsplVal;
        }
        else if(subTrig)
        {
            sol = calcVal - dsplVal;
        }
        else if(mulTrig)
        {
            sol = calcVal * dsplVal;
        }
        else if(divTrig)
        {
            sol = calcVal / dsplVal;
        }
        ui->Display->setText(QString::number(sol));
    }
    else
    {

    }
}
