#include <QPalette>
#include <QColorDialog>
#include "dialogstyle.h"
#include "ui_dialogstyle.h"

DialogStyle::DialogStyle(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogStyle)
{
    ui->setupUi(this);

    connect(this,SIGNAL(colorChanged(QString, QString)),parent,SLOT(setColor(QString,QString)));
}

DialogStyle::~DialogStyle()
{
    delete ui;
}

/**
 * @brief 修改画布颜色
 */
void DialogStyle::on_pushButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "Select Color", QColorDialog::DontUseNativeDialog);
    if(color.isValid()){
        //ui->widget->setPalette(color);//'PlaceholderText' is not a member of 'QPalette'
        emit colorChanged(color.name(),"painterArea");
    }
}
