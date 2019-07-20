#include "dialogsize.h"
#include "ui_dialogsize.h"
#include "../../mainwindow.h"
#include "../../painterarea.h"

/**
 * @brief 构造函数
 *
 * @param parent
 */
DialogSize::DialogSize(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSize)
{
    ui->setupUi(this);
}

/**
 * @brief 析构函数
 *
 */
DialogSize::~DialogSize()
{
    delete ui;
}

/**
 * @brief 将输入的尺寸赋值给painterArea
 *
 */
void DialogSize::on_pushButton_clicked()
{
    int pantsL=qRound(ui->lineEdit_1->text().toFloat()*10);
    int pantsW=qRound(ui->lineEdit_2->text().toFloat()*10);
    int pantsH=qRound(ui->lineEdit_3->text().toFloat()*10);
    int pantsCroH=qRound(ui->lineEdit_4->text().toFloat()*10);
    int waistWidth=qRound(ui->lineEdit_5->text().toFloat()*10);

    MainWindow* mainWin=reinterpret_cast<MainWindow*>(parent());
    mainWin->painterArea->pantsL=pantsL;
    mainWin->painterArea->pantsW=pantsW;
    mainWin->painterArea->pantsH=pantsH;
    mainWin->painterArea->pantsCrotchH=pantsCroH;

    this->close();
}
