#include "dialogruleedit.h"
#include "ui_dialogruleedit.h"

DialogRuleEdit::DialogRuleEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRuleEdit)
{
    ui->setupUi(this);
}

DialogRuleEdit::~DialogRuleEdit()
{
    delete ui;
}
