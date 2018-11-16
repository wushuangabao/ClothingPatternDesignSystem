#include "dialogmm.h"
#include "ui_dialogmm.h"
#include <QDebug>

DialogMM::DialogMM(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMM)
{
    ui->setupUi(this);
}

DialogMM::~DialogMM()
{
    delete ui;
}

void DialogMM::on_comboBox_W_currentTextChanged(const QString &arg1)
{
    W = qRound(10*arg1.toFloat());
}

void DialogMM::on_comboBox_H_currentTextChanged(const QString &arg1)
{
    H = qRound(10*arg1.toFloat());
}
