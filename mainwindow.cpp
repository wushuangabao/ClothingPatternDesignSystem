#include <QtWidgets>
#include <QtDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "painterarea.h"
#include "dialog/dialogms.h"
#include "dialog/dialogmm.h"


MainWindow::MainWindow() :
    QMainWindow(),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialogMS = new DialogMS;
    dialogMM = new DialogMM;
    painterArea = new PainterArea(this);
    setCentralWidget(painterArea);
    setWindowTitle(tr("BasicDrawing Test"));
    labelScaling = new QLabel(tr("放大倍数 %1 ").arg(painterArea->scalingMulti));
    statusBar()->addPermanentWidget(labelScaling);

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
    QString initialPath = QDir::currentPath() + "/untitled.jpg";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString::fromLatin1("JPG"))
                               .arg(QString::fromLatin1("jpg")));
    qDebug()<<fileName;
    pixMap.save(fileName,"JPG");
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
