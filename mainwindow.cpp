#include <QtWidgets>
#include <QMessageBox>
#include <QtDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "painterarea.h"
#include "dialog/dialogms.h"
#include "dialog/dialogmm.h"
#include "data/mypathdata.h"



MainWindow::MainWindow() :
    QMainWindow(),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialogMS = new DialogMS;
    dialogMM = new DialogMM;

    setWindowTitle(tr("BasicDrawing Test"));

    painterArea = new PainterArea(this);
    setCentralWidget(painterArea);

    setDockNestingEnabled(true); //允许嵌套dock
    //记录所有的dock指针
    m_docks.append(ui->dockPointData);
    m_docks.append(ui->dockPathData);
    numberDocks = 2;

    labelScaling = new QLabel(tr("放大倍数 %1 ").arg(painterArea->scalingMulti));
    statusBar()->addPermanentWidget(labelScaling);

    modelPoints = new QStandardItemModel(this);
    modelPaths = new QStandardItemModel(this);
//    model->setItem(0, 0, new QStandardItem("张三"));
//    model->setItem(0, 1, new QStandardItem("3"));
//    model->setItem(0, 2, new QStandardItem("men"));
//    ui->tablePoints->setModel(modelPoints);


    connect(painterArea,SIGNAL(mouseCoordinateChanged()),this,SLOT(setStatusMouseCoordinate()));
    connect(painterArea,SIGNAL(scalingMultiChanged()),this,SLOT(setStatusScalingMulti()));
    connect(dialogMS,SIGNAL(typeSangChanged(int,int)),painterArea,SLOT(setTypeSang(int,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete painterArea;
    delete dialogMS;
    delete dialogMM;

}

///
/// \brief 移除并隐藏所有的dock
///
void MainWindow::removeAllDock()
{
    for(int i=0;i<numberDocks;++i)
    {
        removeDockWidget(m_docks[i]);
    }
}
///
/// \brief 显示指定序号的dock
/// \param index 指定序号，如果不指定，则会显示所有
///
void MainWindow::showDock(const QList<int> &index)
{
    if (index.isEmpty())
    {
        for(int i=0;i<9;++i)
        {
            m_docks[i]->show();
        }
    }
    else
    {
        foreach (int i, index) {
            m_docks[i]->show();
        }
    }
}

void MainWindow::setStatusMouseCoordinate(){
    this->statusBar()->showMessage(painterArea->mouseCoordinate,0);
}

void MainWindow::setStatusScalingMulti(){
    labelScaling->setText(tr("放大倍数 %1 ").arg(painterArea->scalingMulti));
}

void MainWindow::on_action_M_S_triggered()
{
    dialogMS->exec();
}

void MainWindow::on_action_F_S_triggered()
{
    //截取painterArea
    int aLeft = painterArea->startPoint.x()-40, aTop = painterArea->startPoint.y()-40,
        aRight = aLeft+440+painterArea->pantsH/4, aBottom = aTop+painterArea->pantsL+50;
    qreal oldScalingMulti = painterArea->scalingMulti;
    int oldIntUp = painterArea->intUp, oldIntLeft = painterArea->intLeft;
    painterArea->scalingMulti = 10.0;
    painterArea->intUp = 0;
    painterArea->intLeft = 0;
    QRect saveArea(10*aLeft,10*aTop,10*aRight,10*aBottom);
    qDebug()<<"saveArea = "<<saveArea;
    QPixmap pixMap = painterArea->grab(saveArea);
    painterArea->scalingMulti = oldScalingMulti;
    painterArea->intUp = oldIntUp;
    painterArea->intLeft = oldIntLeft;

    //保获取路径，存成jpg文件
    QString filePath = QDir::currentPath() + "/" + painterArea->myPathData->name;
//    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
//                               filePath + ".jpg",
//                               tr("%1 Files (*.%2);;All Files (*)")
//                               .arg(QString::fromLatin1("JPG"))
//                               .arg(QString::fromLatin1("jpg")));
//    qDebug()<<"save to: "<<fileName;
//    bool boolPixMapSaved = pixMap.save(fileName,"JPG");
    bool boolPixMapSaved = pixMap.save(filePath+".jpg","JPG");

    //输出txt文件
    if(painterArea->myPathData->saveTo(filePath+".txt") && boolPixMapSaved)
        QMessageBox::information(nullptr,"Save","The jpg and txt files have saved successfully.");
    else
        QMessageBox::information(nullptr,"Save","The jpg and txt files failed to save!");
}

void MainWindow::on_action_M_M_triggered()
{
    if(dialogMM->exec()==QDialog::Accepted)
    {
        painterArea->pantsCrotchH=dialogMM->Cro;
        painterArea->pantsH=dialogMM->H;
        painterArea->pantsHeight=dialogMM->Height;
        painterArea->pantsL=dialogMM->L;
        painterArea->pantsW=dialogMM->W;
        painterArea->update();
    }
}
