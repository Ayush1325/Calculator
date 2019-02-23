#include "calculator.h"
#include "ui_calculator.h"
#include "QRegularExpression"
#include "QtMath"

enum operation
{
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    COMBINATION,
    PERMUTATION
};

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
    connect(ui->BtnBracketStart, SIGNAL(released()), this, SLOT(MathBtnPressed()));
    connect(ui->BtnBracketEnd, SIGNAL(released()), this, SLOT(MathBtnPressed()));
    connect(ui->BtnEql, SIGNAL(released()), this, SLOT(EqualBtnPressed()));

    connect(ui->BtnAC, SIGNAL(released()), this, SLOT(ACBtnPressed()));
    connect(ui->BtnDel, SIGNAL(released()), this, SLOT(DelBtnPressed()));
    connect(ui->BtnExp, SIGNAL(released()), this, SLOT(ExpBtnPressed()));
    connect(ui->BtnPow, SIGNAL(released()), this, SLOT(PowBtnPressed()));

    connect(ui->BtnPermutation, SIGNAL(released()), this, SLOT(PermutationBtnPressed()));
    connect(ui->BtnCombination, SIGNAL(released()), this, SLOT(CombinationBtnPressed()));
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
    if((displayValue.toDouble() == 0 || displayValue.toDouble() == 0.0) && displayValue.length() == 1 && displayValue != "-" && displayValue != "(")
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
    if((displayVal.toDouble() == 0 || displayVal.toDouble() == 0.0) && displayVal.length() == 1 && displayVal != "(")
    {
        ui->Display->setText(butValue + " ");
    }
    else
    {
        QString newValue = displayVal + " " + butValue + " ";
        ui->Display->setText(newValue);
    }
}

void Calculator::EqualBtnPressed()
{

    QVector<double> numList;
    QVector<int> oprList;
    QString displayVal = ui->Display->text();
    QStringList expression = displayVal.split(" ", QString::SkipEmptyParts);
    expression.prepend("(");
    expression.append(")");
    while (expression.length() > 1) {
        int indexStart = expression.lastIndexOf("(");
        int indexEnd = expression.indexOf(")", indexStart);
        QStringList part =  expression.mid(indexStart + 1, indexEnd - indexStart - 1);
        double res = Bracket(part);
        for (int j = 0; j < indexEnd - indexStart; ++j)
        {
            expression.removeAt(indexStart + 1);
        }
        expression.replace(indexStart, QString::number(res));
    }
    QString sol = expression[0];
    ui->Display->setText(sol);
}

void Calculator::ACBtnPressed()
{
    ui->Display->setText("0");
}

void Calculator::DelBtnPressed()
{
    QString text = ui->Display->text();
    text.chop(1);
    ui->Display->setText(text);
}

void Calculator::ExpBtnPressed()
{
    QString displayVal = ui->Display->text();
    QString newValue = displayVal + "E";
    ui->Display->setText(newValue);
}

void Calculator::PowBtnPressed()
{
    QPushButton *btn = (QPushButton *)sender();
    QString btnValue = btn->text();
    QString displayValue = ui->Display->text();
    QString newValue = displayValue + " " + btnValue + " ";
    ui->Display->setText(newValue);
}

double Calculator::Calculate(QVector<double> nums, QVector<int> oprs)
{
    while(oprs.contains(POW))
    {
        int index = oprs.indexOf(POW);
        double res = qPow(nums[index], nums[index+1]);
        oprs.remove(index);
        nums.remove(index+1);
        nums.replace(index, res);
    }

    while(oprs.contains(COMBINATION))
    {
        int index = oprs.indexOf(COMBINATION);
        double res = nCr(nums[index], nums[index+1]);
        oprs.remove(index);
        nums.remove(index+1);
        nums.replace(index, res);
    }

    while(oprs.contains(PERMUTATION))
    {
        int index = oprs.indexOf(PERMUTATION);
        double res = nPr(nums[index], nums[index+1]);
        oprs.remove(index);
        nums.remove(index+1);
        nums.replace(index, res);
    }

    while(oprs.contains(DIV))
    {
        int index = oprs.indexOf(DIV);
        double res = nums[index]/nums[index+1];
        oprs.remove(index);
        nums.remove(index+1);
        nums.replace(index, res);
    }

    while(oprs.contains(MUL))
    {
        int index = oprs.indexOf(MUL);
        double res = nums[index]*nums[index+1];
        oprs.remove(index);
        nums.remove(index+1);
        nums.replace(index, res);
    }
    while(oprs.contains(SUB))
    {
        int index = oprs.indexOf(SUB);
        double res = nums[index]-nums[index+1];
        oprs.remove(index);
        nums.remove(index+1);
        nums.replace(index, res);
    }
    while(oprs.contains(ADD))
    {
        int index = oprs.indexOf(ADD);
        double res = nums[index]+nums[index+1];
        oprs.remove(index);
        nums.remove(index+1);
        nums.replace(index, res);
    }
    return nums[0];
}

double Calculator::Bracket(QStringList exp)
{
    QVector<double> numList;
    QVector<int> oprList;
    if(exp.first() == "-")
    {
        exp.prepend(0);
    }
    QStringList::const_iterator i;
    for(i = exp.constBegin(); i != exp.constEnd(); ++i)
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
        else if(*i == "^")
        {
            oprList.append(POW);
        }
        else if(*i == "P")
        {
            oprList.append(PERMUTATION);
        }
        else if(*i == "C")
        {
            oprList.append(COMBINATION);
        }
        else
        {
            numList.append(i->toDouble());
        }
    }
    return Calculate(numList, oprList);
}

double Calculator::nPr(double n, double r)
{
    return factorial(n)/factorial(n - r);
}

double Calculator::nCr(double n, double r)
{
    return factorial(n)/(factorial(n - r) * factorial(r));
}

double Calculator::factorial(double num)
{
    if(num == 1)
    {
        return num;
    }
    return num * factorial(num - 1);
}

void Calculator::PermutationBtnPressed()
{
    QString displayVal = ui->Display->text();
    QString newValue = displayVal + " P ";
    ui->Display->setText(newValue);
}

void Calculator::CombinationBtnPressed()
{
    QString displayVal = ui->Display->text();
    QString newValue = displayVal + " C ";
    ui->Display->setText(newValue);
}
