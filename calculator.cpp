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
    EXP,
    POW,
    BROPEN,
    BRCLOSE
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
    connect(ui->BtnBracketStart, SIGNAL(released()), this, SLOT(MathBtnPressed()));
    connect(ui->BtnBracketEnd, SIGNAL(released()), this, SLOT(MathBtnPressed()));
    connect(ui->BtnEql, SIGNAL(released()), this, SLOT(EqualBtnPressed()));

    connect(ui->BtnAC, SIGNAL(released()), this, SLOT(ACBtnPressed()));
    connect(ui->BtnDel, SIGNAL(released()), this, SLOT(DelBtnPressed()));
    connect(ui->BtnExp, SIGNAL(released()), this, SLOT(ExpBtnPressed()));
    connect(ui->BtnPow, SIGNAL(released()), this, SLOT(PowBtnPressed()));
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
    QRegularExpression regex = QRegularExpression("[-+E^/*\\)\\(]");
    QStringList nums = displayVal.split(regex, QString::SkipEmptyParts);
    QStringList::const_iterator i;
    for(i = nums.constBegin(); i != nums.constEnd(); ++i)
    {
        numList.append(i->toDouble());
    }
    QRegularExpressionMatchIterator x =  regex.globalMatch(displayVal);
    while(x.hasNext())
    {
        QRegularExpressionMatch mtch = x.next();
        QString temp = mtch.captured();
        if(temp == "+")
        {
            oprList.append(ADD);
        }
        else if(temp == "-")
        {
            oprList.append(SUB);
        }
        else if(temp == "*")
        {
            oprList.append(MUL);
        }
        else if(temp == "/")
        {
            oprList.append(DIV);
        }
        else if(temp == "E")
        {
            oprList.append(EXP);
        }
        else if(temp == "^")
        {
            oprList.append(POW);
        }
        else if(temp == "(")
        {
            oprList.append(BROPEN);
        }
        else if(temp == ")")
        {
            oprList.append(BRCLOSE);
        }
    }

    if(displayVal.front() != '-')
    {
        oprList.prepend(ADD);
    }

    while(oprList.contains(BROPEN))
    {
        int indexStart = oprList.indexOf(BROPEN);
        int indexEnd = oprList.indexOf(BRCLOSE);
        QVector<double> tempNums = numList.mid(indexStart, indexEnd - indexStart);
        QVector<int> tempOpr = oprList.mid(indexStart + 1, indexEnd - indexStart - 1);
        if(tempNums.length() == tempOpr.length() && tempOpr[0] == SUB)
        {
            tempNums.prepend(0);
            numList.insert(indexStart + 1, 0.0);
        }
        double res = Calculate(tempNums, tempOpr);
        oprList.remove(indexStart, indexEnd - indexStart + 1);
        numList.remove(indexStart + 1, indexEnd - indexStart - 1);
        numList.replace(indexStart, res);
    }

    double res = Calculate(numList, oprList);
    QString sol = QString::number(res);
    ui->Display->setText(sol);
    numList.clear();
    oprList.clear();
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
    QString newValue = displayValue + btnValue;
    ui->Display->setText(newValue);
}

double Calculator::Calculate(QVector<double> nums, QVector<int> oprs)
{
    while(oprs.contains(EXP))
    {
        int index = oprs.indexOf(EXP);
        double res = nums[index]*qPow(10, nums[index+1]);
        oprs.remove(index);
        nums.remove(index+1);
        nums.replace(index, res);
    }

    while(oprs.contains(POW))
    {
        int index = oprs.indexOf(POW);
        double res = qPow(nums[index], nums[index+1]);
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
