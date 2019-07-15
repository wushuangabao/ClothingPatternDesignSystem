#include "dialogdefparameter.h"
#include "ui_dialogdefparameter.h"
#include "dialogaggignparameter.h"

DialogDefParameter::DialogDefParameter(QString arg, QWidget *parent, bool input) :
    QDialog(parent),
    ui(new Ui::DialogDefParameter)
{
    type = arg;
    code = "";
    isInput = input;

    ui->setupUi(this);

    QString s = "输入" + type + "的名称：";
    ui->label->setText(s);

    if(type == "参数" || type == "点"){
        ui->pushButton->setEnabled(true);
    } else {
        ui->pushButton->setEnabled(false);
    }

    connect(this,SIGNAL(newCode(const QString)),parent,SLOT(insertCode(const QString)));
}

DialogDefParameter::~DialogDefParameter()
{
    delete ui;
}

/**
 * @brief Ok
 */
void DialogDefParameter::on_buttonBox_accepted()
{
    QString name = ui->lineEdit->text();

    // todo:检查赋值语句的合法性

    // todo:检查实体名合法性（以及是否与其他实体名重复）
    if(name == ""){
        name = "entity"; // 权宜之计
    }

    if(code != "")
        code = type + " " + name + " = " + code;
    else
        code = type + " " + name;

    if(isInput)
        code = "输入 " + code;

    emit newCode(code);
}

/**
 * @brief Cancel
 */
void DialogDefParameter::on_buttonBox_rejected()
{

}

/**
 * @brief 点击初始化赋值按钮
 */
void DialogDefParameter::on_pushButton_clicked()
{
    QString name = ui->lineEdit->text();
    if(name == "")
        name = "未命名" + type;

    if(type == "参数"){
        DialogAggignParameter* dlg = new DialogAggignParameter(name,this);
        dlg->exec();
        delete dlg;
    }else if(type == "点"){
        // todo
    }
}

/**
 * @brief 初始化赋值槽函数
 * @param value
 */
void DialogDefParameter::assignValue(const QString &value)
{
    code = value;
    ui->labelAssign->setText(QString("= %1").arg(code));
}
