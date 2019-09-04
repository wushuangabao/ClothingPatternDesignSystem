#include <QDir>
#include <QDebug>
#include "dialogpantsh.h"
#include "ui_dialogpantsh.h"
#include "../dialogsize/dialogsize.h"
#include "../../mainwindow.h"

DialogPantsH::DialogPantsH(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPantsH)
{
    ui->setupUi(this);

    stringList<<"裤长"<<"腰位"<<"前腰头"<<"后腰头"<<"裤袢"<<"门襟"<<"前褶省"<<"前袋"<<"后褶省"<<"后袋"<<"裤脚";
//    stringList<<tr("长裤")<<tr("低腰")<<tr("延伸宝剑头")<<tr("有裤袢")<<tr("暗门襟")<<tr("单省")<<tr("斜插袋")<<tr("双省")<<tr("双挖袋")<<tr("无翻边");
    updateString();

//    setLabels(ui->page_1);
//    setLabels(ui->page_3);
//    setLabels(ui->page_4);
//    setLabels(ui->page_5);
//    setLabels(ui->page_6);
//    setLabels(ui->page_7);
//    setLabels(ui->page_8);
//    setLabels(ui->tab_1);
//    setLabels(ui->tab_2);
//    setLabels(ui->tab_3);
//    setLabels(ui->page_10);


    setStackedPages();


    pushButtons.append(ui->pantsLength);
    pushButtons.append(ui->waistPostion);
    pushButtons.append(ui->waistHead1);
    pushButtons.append(ui->waistHead2);
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


}

DialogPantsH::~DialogPantsH()
{
    delete ui;
    delete scene;

    // todo: 释放 stackedPages 中所有手动创建的 按钮、页面
}

/**
 * @brief
 *
 * @param w
 */
void DialogPantsH::setLabels(QWidget *w)
{
    QList<QPushButton*> buttonList = w->findChildren<QPushButton*>();
    for(int i=0; i<buttonList.size(); i++){
        QPushButton* b = buttonList.at(i);
        QString text = b->text();
        QPoint pos_b = b->pos();
        qreal w_b = b->width();
        qreal h_b = b->height();

        QLabel* l = new QLabel(w);
        l->setGeometry(pos_b.x(), pos_b.y()+h_b, w_b, 16);
        l->setText(text);
        l->setAlignment(Qt::AlignHCenter);
    }
}

/**
 * @brief 根据styles文件夹中的内容设置stackedPages
 */
void DialogPantsH::setStackedPages()
{
    // 遍历 styles 文件夹中的 11 个子文件夹
    QString dir = getDir("styles");
    dir = dir + "/";
    int len = stringList.size();
    for(int id = 0; id < len; ++id){
        // 在 stackedPages 中新建一个页面
        QWidget* page = new QWidget();
        ui->stackedPages->insertWidget(id, page);
        // 改变 page 的大小、位置、样式
//        page->setStyleSheet("QPushButton{color: rgba(255, 255, 255, 0);}QPushButton:hover{background-color: rgba(222, 222, 222, 55);}");
        // 遍历 dirStyle 中的文件
        QString dirStyle = getDir(dir + stringList[id]);
        QDir dir(dirStyle);
        QFileInfoList list = dir.entryInfoList();
        QList<QPushButton*> listButton;
        QTabWidget* tabWidget = new QTabWidget(page);
        int i = 0;
        while(i < list.size()){
            QFileInfo fileInfo = list.at(i);
            if(fileInfo.fileName() == "." | fileInfo.fileName() == ".."){
                ++i;
                continue;
            }
            // 新建一个 tab
            if(fileInfo.isDir()){
                createTabPage(tabWidget, fileInfo);
                ++i;
                continue;
            }
            // 新建一个 button 加入 listButton
            if(fileInfo.suffix() == "png"){
                listButton << createPushButton(page, fileInfo);
            }
            ++i;
        }
        if(listButton.size() == 0){
            // 改变 tabWidget 大小和位置
        }else{
            btnInStackedPages << listButton;
            // 排列按钮

        }

    }
}

/**
 * @brief 获取路径（如果没有，就创建一个）
 * @param dir
 * @return 完整的路径名（在dir前面加上currentPath）
 */
QString DialogPantsH::getDir(QString dir)
{
    QString preDir = QDir::currentPath();
    if(!dir.contains(preDir))
        dir = preDir + "/" + dir;
    QDir* folder = new QDir;
    if(!folder->exists(dir))
        folder->mkdir(dir);
    delete folder;
    return dir;
}

/**
 * @brief 创建一个按钮
 * @param parent 父widget
 * @param info 文件信息
 * @return
 */
QPushButton *DialogPantsH::createPushButton(QWidget *parent, const QFileInfo &info)
{
    QString imgPath = info.filePath();
    QImage img(imgPath);
    QPushButton* pb = new QPushButton(info.baseName(), parent);
    connect(pb,SIGNAL(clicked()),this,SLOT(selectPushButton()));  // 给 pb 加上槽函数
    pb->setStyleSheet("background-image:url(" + imgPath + ");");
    pb->resize(img.width(),img.height());
    return pb;
}

/**
 * @brief 创建一个 tab 页面，加入 tabWidget 中
 * @param parent tabWidget
 * @param info 文件信息
 */
void DialogPantsH::createTabPage(QTabWidget *parent, const QFileInfo &info)
{
    QWidget* page = new QWidget();
    parent->addTab(page, info.fileName());
    QString path = info.filePath();
    // 遍历 path 中的文件
}

void DialogPantsH::updateString()
{
    QString str = tr("选择款式：")+stringList.at(0);
    int n = stringList.size(), i=1;
    for(;i<n;i++)
        str=str+"，"+stringList.at(i);
    ui->labelString->setText(str);
}

/**
 * @brief stackedWidget翻页
 * @param id 0到10
 */
void DialogPantsH::changePage(int id)
{
//    int old_id = ui->stackedWidget->currentIndex();
//    ui->stackedWidget->setCurrentIndex(id);
    int old_id = ui->stackedPages->currentIndex();
    ui->stackedPages->setCurrentIndex(id);
    if(old_id < pushButtons.size() && old_id >= 0){
        pushButtons.at(old_id)->setStyleSheet("color:rgb(255,255,255);background-color:rgb(232,94,35);");
        pushButtons.at(old_id)->setStyleSheet("hover{background-color: qlineargradient(spread:pad,x1:0.299,y1:0,x2:1,y2:0.00568182,stop:0.19403 rgba(255,105,46,255),stop:1 rgba(255, 255, 255, 255));}");
    }
    if(id < pushButtons.size() && id >= 0)
        pushButtons.at(id)->setStyleSheet("color:rgb(0,0,0);background-color:rgb(255,255,255);");
}

/**
 * @brief 选择stackedPages中的某个款式
 * @param buttonList 所有款式按钮
 * @param str 款式名称
 */
void DialogPantsH::selectPushButton(QList<QPushButton*> buttonList,QString str)
{
    for(int i=0;i<buttonList.size();i++)    {
        QPushButton* b = buttonList.at(i);
        if(b->text()==str)
            b->setStyleSheet("border:3px solid rgb(222,84,27);color:rgba(255, 255, 255, 0);");
        else{
            b->setStyleSheet("QPushButton{color: rgba(255, 255, 255, 0);}\
                             QPushButton:hover{background-color: rgba(222, 222, 222, 55);}");
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
void DialogPantsH::on_waistHead1_clicked()
{    changePage(2);}
void DialogPantsH::on_waistHead2_clicked()
{    changePage(3);}
void DialogPantsH::on_pantsLoop_clicked()
{    changePage(4);}
void DialogPantsH::on_door_clicked()
{    changePage(5);}
void DialogPantsH::on_sang1_clicked()
{    changePage(6);
}void DialogPantsH::on_pocket1_clicked()
{    changePage(7);}
void DialogPantsH::on_sang2_clicked()
{    changePage(8);}
void DialogPantsH::on_pocket2_clicked()
{    changePage(9);}
void DialogPantsH::on_pantsFoot_clicked()
{    changePage(10);}


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

void DialogPantsH::pageDown()
{
    int i = ui->stackedPages->currentIndex();
    if(i < pushButtons.size() - 1)
        changePage(++i);
}

void DialogPantsH::pageUp()
{
    int i = ui->stackedPages->currentIndex();
    if(i > 0)
        changePage(--i);
}

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

/**
 * @brief 选择某一款式（点击按钮）的槽函数
 */
void DialogPantsH::selectPushButton()
{
    QPushButton* btn= qobject_cast<QPushButton*>(sender());
    qDebug() << btn->text() << " is clicked!";

}
