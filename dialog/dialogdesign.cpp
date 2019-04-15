#include "dialogdesign.h"
#include "ui_dialogdesign.h"
#include "dialogdesign/dialogpantsh.h"
#include "../mainwindow.h"
#include "../painterarea.h"
#include "../data/mypathdata.h"

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
        pantsH=new DialogPantsH((QWidget*)mainWin);
        pantsH->exec();
        delete pantsH;
        break;
    default:
        break;
    }
    this->close();//然而这个窗口会一直存在到退出pantH->exec()为止
    MainWindow *m=(MainWindow*)mainWin;
    m->painterArea->myPathData->clear();
    m->painterArea->setMyPath();
}

/**
 * @brief 选择H廓形
 *
 */
void DialogDesign::on_pushButton_1_clicked()
{
    emit selectKuoXing(1);
}

