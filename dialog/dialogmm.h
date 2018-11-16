#ifndef DIALOGMM_H
#define DIALOGMM_H

#include <QDialog>

namespace Ui {
class DialogMM;
}

class DialogMM : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMM(QWidget *parent = nullptr);
    ~DialogMM();

    int Height=1650;
    int L=1020;
    int W=720;
    int H=940;
    int Cro=245;

private slots:
    void on_comboBox_W_currentTextChanged(const QString &arg1);

    void on_comboBox_H_currentTextChanged(const QString &arg1);

private:
    Ui::DialogMM *ui;
};

#endif // DIALOGMM_H
