#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>

namespace Ui {
class Calculator;
}

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    explicit Calculator(QWidget *parent = nullptr);
    ~Calculator();

private:
    Ui::Calculator *ui;
    void Calculate();

private slots:
    void NumPressed();
    void MathBtnPressed();
    void EqualBtnPressed();
    void ACBtnPressed();
    void DelBtnPressed();
    void ExpBtnPressed();
    void PowBtnPressed();
};

#endif // CALCULATOR_H
