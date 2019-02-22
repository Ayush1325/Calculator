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

private slots:
    void NumPressed();
    void MathBtnPressed();
    void EqualBtnPressed();
    void AnsBtnPressed();
    void ACBtnPressed();
    void DelBtnPressed();
};

#endif // CALCULATOR_H
