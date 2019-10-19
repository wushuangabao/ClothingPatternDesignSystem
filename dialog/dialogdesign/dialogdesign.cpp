#include "dialogdesign.h"
#include "ui_dialogdesign.h"
#include "dialogpantsh.h"
#include "../../mainwindow.h"
#include "../../painterarea.h"
#include "../../data/mypathdata.h"

DialogDesign::DialogDesign(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDesign)
{
    ui->setupUi(this);

    step=1;

    QFont font( "Microsoft YaHei", 15, 75);
    ui->label->setFont(font);

    connect(this,SIGNAL(selectKuoXing(int)),this,SLOT(enterStep2(int)));
}

DialogDesign::~DialogDesign()
{
    delete ui;
}

/**
 * @brief 进入步骤2
 *
 * @param intKuoXing 廓形类型
 */
void DialogDesign::enterStep2(int intKuoXing)
{
    QObject *mainWin=parent();
    switch (intKuoXing) {
    case 1:
        pantsH=new DialogPantsH(static_cast<QWidget*>(mainWin));
        pantsH->exec();
        delete pantsH;
        break;
    default:
        break;
    }
    this->close();
}

/**
 * @brief 选择H廓形
 *
 */
void DialogDesign::on_pushButton_1_clicked()
{
    emit selectKuoXing(1);
}

