#include <QDir>
#include <QDebug>
//#include <QGridLayout>
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
    // stringList<<tr("长裤")<<tr("低腰")<<tr("延伸宝剑头")<<tr("有裤袢")<<tr("暗门襟")<<tr("单省")<<tr("斜插袋")<<tr("双省")<<tr("双挖袋")<<tr("无翻边");
    updateString();

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
    // 不需要释放 stackedPages 中所有手动 new 的 按钮、页面等。因为父亲被删除的时候会自动销毁他的孩子。
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
        // 在 stackedPages 中新建一个 page
        QWidget* page = new QWidget(this);
        ui->stackedPages->insertWidget(id, page);
        int widthPage = ui->stackedPages->width();
        // 遍历 dirStyle 中的文件
        QString dirStyle = getDir(dir + stringList[id]);
        QDir dir(dirStyle);
        QFileInfoList list = dir.entryInfoList();
        QList<Component> components;
        QTabWidget* tabWidget = nullptr;
        int i = 0;
        bool isDir = false;
        while(i < list.size()){
            QFileInfo fileInfo = list.at(i);
            QString fileName = fileInfo.fileName();
            if(fileName == "." || fileName == ".."){
                ++i; continue;
            }
            // 新建一个 tab
            isDir = fileInfo.isDir();
            if(isDir){
                if(tabWidget == nullptr){
                    tabWidget = new QTabWidget(page);
                    tabWidget->resize(widthPage,ui->stackedPages->height());
                }
                createTabPage(tabWidget, fileInfo, &components);
                ++i; continue;
            }
            // 新建一个 Component
            if(fileInfo.suffix() == "png")
                createPushButton(page, fileInfo, &components);
            ++i;
        }
        btnInStackedPages << components;
        if(!isDir)
            setPosition(components, widthPage);
    }
}

/**
 * @brief 将列表中的 btn、label 的位置一一排列
 * @param components
 * @param widthMax
 */
void DialogPantsH::setPosition(QList<Component> components, int widthMax)
{
    int size = components.size();
    if(size == 0) return;
    int heightMax = 0, xOld = -3, yOld = 0;
    for(int i=0; i<size; ++i){
        QPushButton* btn = components[i].btn;
        int hBtn = btn->height(),  wBtn = btn->width();
        if(heightMax < hBtn)
            heightMax = hBtn;
        int x = xOld + 3, y = yOld;
        if(xOld + wBtn > widthMax){  // 换行
            x = 0; y = yOld + heightMax + 50;
            heightMax = 0;
        }
        xOld = x + wBtn; yOld = y;
        btn->move(x, y);
        components[i].label->setGeometry(x, y + hBtn + 2, wBtn, 16);
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
 * @brief 创建一个按钮，并创建对应的 Component
 * @param parent 按钮的父widget
 * @param info 文件信息
 * @param components 款式部件集合
 * @return
 */
void DialogPantsH::createPushButton(QWidget *parent, const QFileInfo &info, QList<Component>* components)
{
    QString imgPath = info.filePath();
    QString name = info.baseName();
    QImage img(imgPath);
    // 创建 push button
    QPushButton* pb = new QPushButton(name, parent);
    connect(pb,SIGNAL(clicked()),this,SLOT(selectPushButton()));  // 给 pb 加上槽函数
    pb->setStyleSheet("QPushButton{color: rgba(255, 255, 255, 0);background-image:url(" + imgPath + ");}\
                      QPushButton:hover{background-color: rgba(222, 222, 222, 55);}");
    pb->resize(img.width(),img.height());
    // 创建 label
    QLabel* l = new QLabel(parent);
    l->setText(name);
    l->setAlignment(Qt::AlignHCenter);
    // 创建 Component 加入 components
    Component component{
        imgPath,
        pb,
        l
    };
    components->append(component);
}

/**
 * @brief 创建一个 tab 页面，加入 tabWidget 中
 * @param parent tabWidget
 * @param info 文件信息
 */
void DialogPantsH::createTabPage(QTabWidget *parent, const QFileInfo &info, QList<Component>* components)
{
    QList<Component> componentsInOnePage;
    QWidget* page = new QWidget(this);
    QString tabName = info.fileName();
    parent->addTab(page, tabName);
    QString path = info.absoluteFilePath();
    // 遍历 path 中的文件
    QDir dir(path);
    QFileInfoList list = dir.entryInfoList();
    int i = 0;
    while(i < list.size()){
        QFileInfo fileInfo = list.at(i);
        QString fileName = fileInfo.fileName();
        if(fileName == "." || fileName == ".."){
            ++i; continue;
        }
        if(fileInfo.suffix() == "png")
            createPushButton(page, fileInfo, &componentsInOnePage);
        ++i;
    }
    int size = componentsInOnePage.size();
    if(size > 0){
        // componentsInOnePage 中的 button 名称前加上 tab 页的名字，并加入 components
        for(int i = 0; i < size; ++i){
            QString name = tabName.remove("袋") + componentsInOnePage[i].btn->text();
            componentsInOnePage[i].btn->setText(name);
            components->append(componentsInOnePage[i]);
        }
        // componentsInOnePage 中的 button、label 进行排列
        setPosition(componentsInOnePage, ui->stackedPages->width());
    }
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
        pushButtons.at(old_id)->setStyleSheet(nullptr);
//      pushButtons.at(old_id)->setStyleSheet("color:rgb(255,255,255);background-color:rgb(232,94,35);");
//      pushButtons.at(old_id)->setStyleSheet("hover{background-color: qlineargradient(spread:pad,x1:0.299,y1:0,x2:1,y2:0.00568182,stop:0.19403 rgba(255,105,46,255),stop:1 rgba(255, 255, 255, 255));}");
    }
    if(id < pushButtons.size() && id >= 0)
        pushButtons.at(id)->setStyleSheet("color:rgb(0,0,0);background-color:rgb(255,255,255);");
}

/**
 * @brief 选择stackedPages中的某个款式
 * @param buttonList 本页中所有款式
 * @param str 款式名称
 */
void DialogPantsH::selectPushButton(QList<Component> buttonList, QString str)
{
    for(int i=0;i<buttonList.size();i++)    {
        QPushButton* b = buttonList[i].btn;
        QString img = buttonList[i].imgPath;
        if(b->text() == str)
            b->setStyleSheet("border:3px solid rgb(222,84,27);color:rgba(255, 255, 255, 0);background-image:url(" + img + ");");
        else{
            b->setStyleSheet("QPushButton{color: rgba(255, 255, 255, 0);background-image:url(" + img + ");}\
                             QPushButton:hover{background-color: rgba(222, 222, 222, 55);}");
        }
    }
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
    if(k->key()==Qt::Key_W){
         pageUp();
     }
     else if(k->key()==Qt::Key_S){
         pageDown();
     }
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
 */
void DialogPantsH::on_buttonNext_clicked()
{
    DialogSize *dialogSize=new DialogSize(static_cast<QWidget*>(parent()));
    dialogSize->exec();
    delete dialogSize;
    this->close();
}

/**
 * @brief 选择某一款式（点击按钮）的槽函数
 */
void DialogPantsH::selectPushButton()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    qDebug() << btn->text() << " is clicked!";
    int id = ui->stackedPages->currentIndex();
    QList<Component> list = btnInStackedPages[id];
    QString str = btn->text();
    selectPushButton(list, str);
    switch (id) {
    case 1:
        str.remove("裤");
        break;
    case 6:
        str = "前片" + str;
        break;
    case 7:
        str = "后片" + str;
        break;
    }
    stringList.replace(id, str);
    updateString();
}
