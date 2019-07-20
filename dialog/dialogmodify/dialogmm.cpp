#include "dialogmm.h"
#include "ui_dialogmm.h"
#include <QDebug>

/**
 * @brief
 *
 * @param parent
 */
DialogMM::DialogMM(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMM)
{
    ui->setupUi(this);
}

/**
 * @brief
 *
 */
DialogMM::~DialogMM()
{
    delete ui;
}

/**
 * @brief
 *
 * @param arg1
 */
void DialogMM::on_comboBox_W_currentTextChanged(const QString &arg1)
{
    W = qRound(10*arg1.toFloat());
}

/**
 * @brief
 *
 * @param arg1
 */
void DialogMM::on_comboBox_H_currentTextChanged(const QString &arg1)
{
    H = qRound(10*arg1.toFloat());
}
