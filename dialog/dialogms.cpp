#include "dialogms.h"
#include "ui_dialogms.h"
#include <QDebug>

DialogMS::DialogMS(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMS)
{
    ui->setupUi(this);
}

DialogMS::~DialogMS()
{
    delete ui;
}

/**
 * @brief
 *
 * @param checked
 */
void DialogMS::on_radioButton1_0_clicked(bool checked)
{
    if(checked)
        emit typeSangChanged(1,0);
}

/**
 * @brief
 *
 * @param checked
 */
void DialogMS::on_radioButton1_1_clicked(bool checked)
{
    if(checked)
        emit typeSangChanged(1,1);
}

/**
 * @brief
 *
 * @param checked
 */
void DialogMS::on_radioButton2_0_clicked(bool checked)
{
    if(checked)
        emit typeSangChanged(2,0);
}

/**
 * @brief
 *
 * @param checked
 */
void DialogMS::on_radioButton2_1_clicked(bool checked)
{
    if(checked)
        emit typeSangChanged(2,1);
}
