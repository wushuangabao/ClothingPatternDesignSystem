#include "dialogsize.h"
#include "ui_dialogsize.h"
#include <QDir>
#include <QDebug>
#include "../../mainwindow.h"
#include "../../painterarea.h"
#include "../dialogdesign/dialogpantsh.h"
#include "../../rules/mypainter.h"
#include "../../rules/myrule.h"
#include "../../data/mypathdata.h"

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
 */
void DialogSize::on_pushButton_clicked()
{
    QString L = ui->lineEdit_1->text(); // L 105
    QString W = ui->lineEdit_2->text(); // W 72
    QString H = ui->lineEdit_3->text(); // H 94
    QString CR= ui->lineEdit_4->text(); // CR 24.5
    QString wb = ui->lineEdit_5->text(); // widthBand
    QString params = H +","+ W +","+ L +","+ CR;
    drawByRule("女裤/女裤净样板.txt", params);
    this->close();
}

/**
 * @brief 根据规则文件（的路径）画样板
 * @param path 规则文件的路径
 * @param in 输入的参数字符串
 */
void DialogSize::drawByRule(QString path, QString in)
{
    DialogPantsH* dlgPantsH = static_cast<DialogPantsH*>(parent());
    QString dir = QDir::currentPath() + "/rules/" + path;
    int i1 = dir.lastIndexOf("/"),
        i2 = dir.lastIndexOf(".");
    MyRule rule(dir);
    MyPainter mp = rule.drawPathWith(in);
    MyPathData* data = new MyPathData(*mp.myData);
    MainWindow* mw = static_cast<MainWindow*>(dlgPantsH->parent());
    data->setName(dir.mid(i1+1).left(i2-i1-1));
    data->params = in;
    mw->painterArea->addPath(data);
}
