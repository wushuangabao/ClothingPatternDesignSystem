#include "dialogpantsh.h"
#include "ui_dialogpantsh.h"
#include "../dialogsize.h"
#include "../../mainwindow.h"

DialogPantsH::DialogPantsH(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPantsH)
{
    ui->setupUi(this);

    setLabels(ui->page_1);
    setLabels(ui->page_3);
    setLabels(ui->page_4);
    setLabels(ui->page_5);
    setLabels(ui->page_6);
    setLabels(ui->page_7);
    setLabels(ui->page_8);
    setLabels(ui->tab_1);
    setLabels(ui->tab_2);
    setLabels(ui->tab_3);
    setLabels(ui->page_10);

    pushButtons.append(ui->pantsLength);
    pushButtons.append(ui->waistPostion);
    pushButtons.append(ui->waistHead);
    pushButtons.append(ui->pantsLoop);
    pushButtons.append(ui->door);
    pushButtons.append(ui->sang1);
    pushButtons.append(ui->pocket1);
    pushButtons.append(ui->sang2);
    pushButtons.append(ui->pocket2);
    pushButtons.append(ui->pantsFoot);
    changePage(0);

    QPixmap pm(":/pantsh/pants.png");
    pm=pm.scaled(400,400,Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
    scene=new QGraphicsScene(ui->graphicsView);
    scene->addPixmap(pm);
    ui->graphicsView->setScene(scene);

    stringList<<tr("长裤")<<tr("中腰")<<tr("挂钩1")<<tr("无裤袢")<<tr("直门襟")<<tr("前片双褶")<<tr("斜插袋")<<tr("后片双省")<<tr("无袋")<<tr("无翻边");
    updateString();
}

DialogPantsH::~DialogPantsH()
{
    delete ui;
    delete scene;
}

/**
 * @brief
 *
 * @param w
 */
void DialogPantsH::setLabels(QWidget *w)
{
    QList<QPushButton*> buttonList = w->findChildren<QPushButton*>();
    for(int i=0;i<buttonList.size();i++)
    {
        QPushButton* b=buttonList.at(i);
        QString text=b->text();
        QPoint pos_b=b->pos();
        qreal w_b=b->width();
        qreal h_b=b->height();

        QLabel* l=new QLabel(w);
        l->setGeometry(pos_b.x(),pos_b.y()+h_b,w_b,16);
        l->setText(text);
        l->setAlignment(Qt::AlignHCenter);
    }
}

/**
 * @brief
 *
 */
void DialogPantsH::updateString()
{
    QString str=tr("选择款式：")+stringList.at(0);
    int n=stringList.size(), i=1;
    for(;i<n;i++)
    {
        str=str+"，"+stringList.at(i);
    }
    ui->labelString->setText(str);
}

/**
 * @brief
 *
 * @param id
 */
void DialogPantsH::changePage(int id)
{
    int old_id=ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(id);
    pushButtons.at(old_id)->setStyleSheet("color:rgb(255,255,255);background-color:rgb(232,94,35);");
    pushButtons.at(old_id)->setStyleSheet("hover{background-color: qlineargradient(spread:pad,x1:0.299,y1:0,x2:1,y2:0.00568182,stop:0.19403 rgba(255,105,46,255),stop:1 rgba(255, 255, 255, 255));}");
    pushButtons.at(id)->setStyleSheet("color:rgb(0,0,0);background-color:rgb(255,255,255);");
}

/**
 * @brief
 *
 * @param buttonList
 * @param str
 */
void DialogPantsH::selectPushButton(QList<QPushButton*> buttonList,QString str)
{
    for(int i=0;i<buttonList.size();i++)
    {
        QPushButton* b = buttonList.at(i);
        if(b->text()==str)
            b->setStyleSheet("border:3px solid rgb(222,84,27);");
        else{
            b->setStyleSheet("");
        }
    }
}

/**
 * @brief
 *
 * @param str
 */
void DialogPantsH::changePantsL(QString str)
{
    QList<QPushButton*> buttonList = ui->page_1->findChildren<QPushButton*>();
    selectPushButton(buttonList,str);
    stringList.replace(0,str);
    updateString();
}

/**
 * @brief
 *
 * @param str
 */
void DialogPantsH::changeWaistPostion(QString str)
{
    QList<QPushButton*> buttonList = ui->page_2->findChildren<QPushButton*>();
    selectPushButton(buttonList,str);
    str.remove(tr("裤"));
    stringList.replace(1,str);
    updateString();
}

/**
 * @brief
 *
 * @param str
 */
void DialogPantsH::changeWaistHead(QString str)
{
    QList<QPushButton*> buttonList = ui->page_3->findChildren<QPushButton*>();
    selectPushButton(buttonList,str);
    stringList.replace(2,str);
    updateString();
}

/**
 * @brief
 *
 * @param str
 */
void DialogPantsH::changePantsLoop(QString str)
{
    QList<QPushButton*> buttonList = ui->page_4->findChildren<QPushButton*>();
    selectPushButton(buttonList,str);
    stringList.replace(3,str);
    updateString();
}

/**
 * @brief
 *
 * @param str
 */
void DialogPantsH::changeDoor(QString str)
{
    QList<QPushButton*> buttonList = ui->page_5->findChildren<QPushButton*>();
    selectPushButton(buttonList,str);
    stringList.replace(4,str);
    updateString();
}

/**
 * @brief
 *
 * @param str
 */
void DialogPantsH::changeSang1(QString str)
{
    QList<QPushButton*> buttonList = ui->page_6->findChildren<QPushButton*>();
    selectPushButton(buttonList,str);
    str="前片"+str;
    stringList.replace(5,str);
    updateString();
}

/**
 * @brief
 *
 * @param str
 */
void DialogPantsH::changePocket1(QString str)
{
    QList<QPushButton*> buttonList = ui->page_7->findChildren<QPushButton*>();
    selectPushButton(buttonList,str);
    stringList.replace(6,str);
    updateString();
}
/**
 * @brief
 *
 * @param str
 */
void DialogPantsH::changeSang2(QString str)
{
    QList<QPushButton*> buttonList = ui->page_8->findChildren<QPushButton*>();
    selectPushButton(buttonList,str);
    str="后片"+str;
    stringList.replace(7,str);
    updateString();
}
/**
 * @brief
 *
 * @param str
 */
void DialogPantsH::changePocket2(QString str)
{
    QList<QPushButton*> buttonList = ui->page_9->findChildren<QPushButton*>();
    selectPushButton(buttonList,str);
    stringList.replace(8,str);
    updateString();
}

/**
 * @brief
 *
 * @param str
 */
void DialogPantsH::changePantsFoot(QString str)
{
    QList<QPushButton*> buttonList = ui->page_10->findChildren<QPushButton*>();
    selectPushButton(buttonList,str);
    stringList.replace(9,str);
    updateString();
}


void DialogPantsH::on_pantsLength_clicked()
{    changePage(0);}
void DialogPantsH::on_waistPostion_clicked()
{    changePage(1);}
void DialogPantsH::on_waistHead_clicked()
{    changePage(2);}
void DialogPantsH::on_pantsLoop_clicked()
{    changePage(3);}
void DialogPantsH::on_door_clicked()
{    changePage(4);}
void DialogPantsH::on_sang1_clicked()
{    changePage(5);
}void DialogPantsH::on_pocket1_clicked()
{    changePage(6);}
void DialogPantsH::on_sang2_clicked()
{    changePage(7);}
void DialogPantsH::on_pocket2_clicked()
{    changePage(8);}
void DialogPantsH::on_pantsFoot_clicked()
{    changePage(9);}

/**
 * @brief
 *
 * @param k
 */
void DialogPantsH::keyPressEvent(QKeyEvent* k)
{
    if(k->key()==Qt::Key_W)
     {
         pageUp();
     }
     else if(k->key()==Qt::Key_S)
     {
         pageDown();
     }
     else if(k->key()==Qt::Key_A)
     {}
     else if(k->key()==Qt::Key_D)
     {}
}

/**
 * @brief
 *
 */
void DialogPantsH::pageDown()
{
    int i=ui->stackedWidget->currentIndex();
    if(i<9)
        changePage(++i);
}

/**
 * @brief
 *
 */
void DialogPantsH::pageUp()
{
    int i=ui->stackedWidget->currentIndex();
    if(i>0)
        changePage(--i);
}

void DialogPantsH::on_pushButton21_clicked()
{    changePantsL(ui->pushButton21->text());}
void DialogPantsH::on_pushButton22_clicked()
{    changePantsL(ui->pushButton22->text());}
void DialogPantsH::on_pushButton23_clicked()
{    changePantsL(ui->pushButton23->text());}
void DialogPantsH::on_pushButton24_clicked()
{    changePantsL(ui->pushButton24->text());}
void DialogPantsH::on_pushButton25_clicked()
{    changePantsL(ui->pushButton25->text());}
void DialogPantsH::on_pushButton26_clicked()
{    changePantsL(ui->pushButton26->text());}
void DialogPantsH::on_pushButton27_clicked()
{    changePantsL(ui->pushButton27->text());}
void DialogPantsH::on_pushButton28_clicked()
{    changePantsL(ui->pushButton28->text());}
void DialogPantsH::on_pushButton31_clicked()
{    changeWaistPostion(ui->pushButton31->text());}
void DialogPantsH::on_pushButton32_clicked()
{    changeWaistPostion(ui->pushButton32->text());}
void DialogPantsH::on_pushButton33_clicked()
{    changeWaistPostion(ui->pushButton33->text());}
void DialogPantsH::on_pushButton34_clicked()
{    changeWaistPostion(ui->pushButton34->text());}
void DialogPantsH::on_pushButton41_clicked()
{    changeWaistHead(ui->pushButton41->text());}
void DialogPantsH::on_pushButton42_clicked()
{    changeWaistHead(ui->pushButton42->text());}
void DialogPantsH::on_pushButton43_clicked()
{    changeWaistHead(ui->pushButton43->text());}
void DialogPantsH::on_pushButton44_clicked()
{    changeWaistHead(ui->pushButton44->text());}
void DialogPantsH::on_pushButton45_clicked()
{    changeWaistHead(ui->pushButton45->text());}
void DialogPantsH::on_pushButton46_clicked()
{    changeWaistHead(ui->pushButton46->text());}
void DialogPantsH::on_pushButton47_clicked()
{    changeWaistHead(ui->pushButton47->text());}
void DialogPantsH::on_pushButton48_clicked()
{    changeWaistHead(ui->pushButton48->text());}
void DialogPantsH::on_pushButton49_clicked()
{    changeWaistHead(ui->pushButton49->text());}
void DialogPantsH::on_pushButton51_clicked()
{    changePantsLoop(ui->pushButton51->text());}
void DialogPantsH::on_pushButton52_clicked()
{    changePantsLoop(ui->pushButton52->text());}
void DialogPantsH::on_pushButton65_clicked()
{    changeDoor(ui->pushButton65->text());}
void DialogPantsH::on_pushButton64_clicked()
{    changeDoor(ui->pushButton64->text());}
void DialogPantsH::on_pushButton63_clicked()
{    changeDoor(ui->pushButton63->text());}
void DialogPantsH::on_pushButton62_clicked()
{    changeDoor(ui->pushButton62->text());}
void DialogPantsH::on_pushButton61_clicked()
{    changeDoor(ui->pushButton61->text());}
void DialogPantsH::on_pushButton71_clicked()
{    changeSang1(ui->pushButton71->text());}
void DialogPantsH::on_pushButton72_clicked()
{    changeSang1(ui->pushButton72->text());}
void DialogPantsH::on_pushButton73_clicked()
{    changeSang1(ui->pushButton73->text());}
void DialogPantsH::on_pushButton74_clicked()
{    changeSang1(ui->pushButton74->text());}
void DialogPantsH::on_pushButton75_clicked()
{    changeSang1(ui->pushButton75->text());}
void DialogPantsH::on_pushButton76_clicked()
{    changeSang1(ui->pushButton76->text());}
void DialogPantsH::on_pushButton77_clicked()
{    changeSang1(ui->pushButton77->text());}
void DialogPantsH::on_pushButton81_clicked()
{    changePocket1(ui->pushButton81->text());}
void DialogPantsH::on_pushButton82_clicked()
{    changePocket1(ui->pushButton82->text());}
void DialogPantsH::on_pushButton83_clicked()
{    changePocket1(ui->pushButton83->text());}
void DialogPantsH::on_pushButton84_clicked()
{    changePocket1(ui->pushButton84->text());}
void DialogPantsH::on_pushButton85_clicked()
{    changePocket1(ui->pushButton85->text());}
void DialogPantsH::on_pushButton91_clicked()
{    changeSang2(ui->pushButton91->text());}
void DialogPantsH::on_pushButton92_clicked()
{    changeSang2(ui->pushButton92->text());}
void DialogPantsH::on_pushButton93_clicked()
{    changeSang2(ui->pushButton93->text());}
void DialogPantsH::on_pushButton94_clicked()
{    changeSang2(ui->pushButton94->text());}
void DialogPantsH::on_pushButton1001_clicked()
{    changePocket2(ui->pushButton1001->text());}
void DialogPantsH::on_pushButton1002_clicked()
{    changePocket2(ui->pushButton1002->text());}
void DialogPantsH::on_pushButton1003_clicked()
{    changePocket2(ui->pushButton1003->text());}
void DialogPantsH::on_pushButton1004_clicked()
{    changePocket2(ui->pushButton1004->text());}
void DialogPantsH::on_pushButton1005_clicked()
{    changePocket2(ui->pushButton1005->text());}
void DialogPantsH::on_pushButton1006_clicked()
{    changePocket2(ui->pushButton1006->text());}
void DialogPantsH::on_pushButton1007_clicked()
{    changePocket2(ui->pushButton1007->text());}
void DialogPantsH::on_pushButton1008_clicked()
{    changePocket2(ui->pushButton1008->text());}
void DialogPantsH::on_pushButton1009_clicked()
{    changePocket2(ui->pushButton1009->text());}
void DialogPantsH::on_pushButton1010_clicked()
{    changePocket2(ui->pushButton1010->text());}
void DialogPantsH::on_pushButton1011_clicked()
{    changePocket2(ui->pushButton1011->text());}
void DialogPantsH::on_pushButton1012_clicked()
{    changePocket2(ui->pushButton1012->text());}
void DialogPantsH::on_pushButton1013_clicked()
{    changePocket2(ui->pushButton1013->text());}
void DialogPantsH::on_pushButton1014_clicked()
{    changePocket2(ui->pushButton1014->text());}
void DialogPantsH::on_pushButton1015_clicked()
{    changePocket2(ui->pushButton1015->text());}
void DialogPantsH::on_pushButton111_clicked()
{    changePantsFoot(ui->pushButton111->text());}
void DialogPantsH::on_pushButton112_clicked()
{    changePantsFoot(ui->pushButton112->text());}
void DialogPantsH::on_pushButton113_clicked()
{    changePantsFoot(ui->pushButton113->text());}


/**
 * @brief 进入下一步，运行尺寸输入对话框
 *
 */
void DialogPantsH::on_buttonNext_clicked()
{
    DialogSize *dialogSize=new DialogSize((QWidget*)parent());
    dialogSize->exec();
    delete dialogSize;
    this->close();
}
