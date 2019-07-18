#include <QInputDialog>
#include "dialogaggignparameter.h"
#include "ui_dialogaggignparameter.h"
#include "../../rules/myrule.h"

DialogAggignParameter::DialogAggignParameter(QString arg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAggignParameter)
{
    name = arg;

    ui->setupUi(this);

    QString s = arg + " =";
    ui->label->setText(s);

    connect(this,SIGNAL(initializeWith(const QString)),parent,SLOT(assignValue(const QString)));
}

DialogAggignParameter::~DialogAggignParameter()
{
    delete ui;
}

/**
 * @brief Ok
 */
void DialogAggignParameter::on_buttonBox_accepted()
{
    QString v = ui->labelValue->text();
    v.replace(" cm","");
    emit initializeWith(v);
}

/**
 * @brief Cancel
 */
void DialogAggignParameter::on_buttonBox_rejected()
{

}

/**
 * @brief 设置关键参数
 */
void DialogAggignParameter::on_radioButton1_clicked()
{
    QStringList sizes;
    sizes<<"H"<<"W"<<"CR"<<"L";

    bool ok;
    QString s = QInputDialog::getItem(this,"关键参数","选择尺寸",sizes,0,false,&ok);
    if(ok && !sizes.isEmpty())
        ui->labelValue->setText(s);
}

/**
 * @brief 设置常量参数
 */
void DialogAggignParameter::on_radioButton2_clicked()
{
    bool ok;
    double d = QInputDialog::getDouble(this,"常量参数","设置数值(单位:cm)",0.0,0,200,2,&ok);
    if(ok)
        ui->labelValue->setText(QString("%1 cm").arg(d));
}

/**
 * @brief 设置过程参数
 */
void DialogAggignParameter::on_radioButton3_clicked()
{
    bool ok = true;
    QString v = QInputDialog::getText(nullptr,"过程参数","设置公式（可使用已定义的参数名）：",QLineEdit::Normal,"单位：cm（不要加上单位）",&ok);
    if(ok){
        // 测试表达式v是否可以求值：
        qreal value = MyRule::calculate(v, &ok);
        if(ok)
            ui->labelValue->setText(QString::number(value));
        else
            ui->labelValue->setText(v);
    }
}
