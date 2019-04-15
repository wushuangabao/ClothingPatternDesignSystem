#include <QtWidgets>
#include <QMessageBox>
#include <QtDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "painterarea.h"
#include "mypath.h"
#include "dialog/dialogms.h"
#include "dialog/dialogmm.h"
#include "dialog/dialogdesign.h"
#include "data/mypathdata.h"
//#include <QHeaderView>


/**
 * @brief 主窗口的构造
 *
 */
MainWindow::MainWindow() :
    QMainWindow(),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialogMS = new DialogMS(this);
    dialogMM = new DialogMM(this);
    dialogDesign = new DialogDesign(this);

    setWindowTitle(tr("Lancy PDS 2018"));

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

    ui->tablePoints->setModel(modelPoints);
    modelPoints->setHeaderData(0,Qt::Horizontal, tr("x坐标"));
    modelPoints->setHeaderData(1,Qt::Horizontal, tr("y坐标"));
    modelPoints->setHeaderData(2,Qt::Horizontal, tr("别名"));

    ui->tablePaths->setModel(modelPaths);
    ui->tablePaths->horizontalHeader()->hide();
    ui->tablePaths->setEditTriggers(QAbstractItemView::NoEditTriggers);//设为只读
    ui->tablePaths->setSelectionMode(QAbstractItemView::SingleSelection);//只能单选

    connect(painterArea,SIGNAL(mouseCoordinateChanged()),this,SLOT(setStatusMouseCoordinate()));
    connect(painterArea,SIGNAL(scalingMultiChanged()),this,SLOT(setStatusScalingMulti()));
    connect(painterArea,SIGNAL(resetModel()),this,SLOT(resetModel()));
    connect(dialogMS,SIGNAL(typeSangChanged(int,int)),painterArea,SLOT(setTypeSang(int,int)));
//    connect(ui->tablePaths->model(),SIGNAL(itemChanged(QStandardItem*)),painterArea,SLOT(changePath()));

    //下面代码用于测试
    //painterArea->setMyPath();
}

/**
 * @brief 主窗口的析构
 *
 */
MainWindow::~MainWindow()
{
    delete ui;
    delete painterArea;
    delete labelScaling;
    delete dialogMS;
    delete dialogMM;
    delete dialogDesign;
    delete modelPaths;
    delete modelPoints;
}

/**
 * @brief 移除并隐藏所有的dock
 *
 */
void MainWindow::removeAllDock()
{
    for(int i=0;i<numberDocks;++i)
    {
        removeDockWidget(m_docks[i]);
    }
}

/**
 * @brief 显示指定序号的dock
 *
 * @param index 指定序号，如果不指定，则会显示所有
 */
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

/**
 * @brief 关闭主窗口事件
 *
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    switch(QMessageBox::question(this,tr("提示"),tr("确定退出?")))
    {
    case QMessageBox::Yes:
        event->accept();
        break;
    default:
        event->ignore();
        break;
    }
}

/**
 * @brief 设置状态栏（显示鼠标位置
 *
 */
void MainWindow::setStatusMouseCoordinate(){
    this->statusBar()->showMessage(painterArea->stringTempStatus,0);
}

/**
 * @brief 设置状态栏（显示放大倍数
 *
 */
void MainWindow::setStatusScalingMulti(){
    labelScaling->setText(tr("放大倍数 %1 ").arg(painterArea->scalingMulti));
    this->statusBar()->showMessage("",0);
}

/**
 * @brief 根据myPathData修改tablePoints和tablePaths
 *
 */
void MainWindow::resetModel(){
    modelPoints->clear();
    modelPaths->clear();
    MyPathData *data = painterArea->myPathData;
    int nPaths = data->numberPath,
        nPoints = data->numberPoint;
    for(int i=0;i<nPoints;++i)
    {
        QPointF p = data->pointData[i];
        QString x = QString::number(p.x(),'f',2);
        QString y = QString::number(p.y(),'f',2);
        QString name = data->findName(i);
        modelPoints->setItem(i, 0, new QStandardItem(x));
        modelPoints->setItem(i, 1, new QStandardItem(y));
        modelPoints->setItem(i, 2, new QStandardItem(name));
    }
    for(int i=0;i<nPaths;i++)
    {
        PathData path = data->pathData[i];
        QString type = path.isLine?tr("直线"):tr("曲线");
        QString point = data->stringsOf(path.startPoint);
        modelPaths->setItem(i, 0, new QStandardItem(type));
        modelPaths->setItem(i, 1, new QStandardItem(point));
    }
    ui->tablePoints->resizeColumnsToContents();  //根据内容自动调整所有列的列宽
    ui->tablePaths->resizeColumnsToContents();
}

/**
 * @brief action触发 省褶类型修改
 *
 */
void MainWindow::on_action_M_S_triggered()
{
    dialogMS->exec();
}

/**
 * @brief action触发 文件保存
 *
 */
void MainWindow::on_action_F_S_triggered()
{
//    //截取painterArea
//    int aLeft = 60, aTop = 60,
//        aRight = aLeft+440+painterArea->pantsH/4,
//        aBottom = aTop+painterArea->pantsL+50;
//    qreal oldScalingMulti = painterArea->scalingMulti;
//    int oldIntUp = painterArea->intUp, oldIntLeft = painterArea->intLeft;
//    painterArea->scalingMulti = 5.0;
//    painterArea->intUp = 0;
//    painterArea->intLeft = 0;
//    painterArea->update();
//    QPixmap pixMap = painterArea->grab(QRect(5*aLeft,5*aTop,5*aRight,5*aBottom));
//    painterArea->scalingMulti = oldScalingMulti;
//    painterArea->intUp = oldIntUp;
//    painterArea->intLeft = oldIntLeft;
//    painterArea->update();

    //保获取路径
    QString filePath = QDir::currentPath() + "/" + painterArea->myPathData->name;
//    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
//                       filePath + ".dxf",
//                       tr("%1 Files (*.%2)")
//                       .arg(QString::fromLatin1("DXF"))
//                       .arg(QString::fromLatin1("dxf")));
//    qDebug()<<"save to: "<<fileName;

//    //保存为jpg文件
//    bool boolPixMapSaved = pixMap.save(fileName,"JPG");
//    if(boolPixMapSaved)
//        QMessageBox::information(nullptr,"Save","The jpg file has saved successfully.");

    //保存为dxf文件
    bool boolDXFSaved=painterArea->writeDXF();


    //输出txt文件
    if(painterArea->myPathData->saveTo(filePath+".txt") && boolDXFSaved)
        QMessageBox::information(nullptr,"Save","The dxf and txt files have saved successfully.");
    else
        QMessageBox::information(nullptr,"Save","The dxf and txt files failed to save!");
}

/**
 * @brief action触发 尺寸修改
 *
 */
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

/**
 * @brief 点击tablePath
 *
 * @param index
 */
void MainWindow::on_tablePaths_clicked(const QModelIndex &index)
{
    int id = index.row();
    showPath(id);
}

/**
 * @brief 在painterArea上显示tablePaths中row=id的path及其上的点
 *
 * @param id
 */
void MainWindow::showPath(int id)
{
    PathData pathData = painterArea->myPathData->pathData[id];
    QPainterPath yellowPath;
    if(pathData.isLine)
    {
        QPointF sp = painterArea->myPathData->pointData[pathData.startPoint->id],
                ep = painterArea->myPathData->pointData[pathData.endPoint->id];
        yellowPath.moveTo(sp);
        yellowPath.lineTo(ep);
        painterArea->yellowPath = yellowPath;
        painterArea->setCenterToYellowPath();
        painterArea->greenPath = QPainterPath();
        // 显示labelPoint
        painterArea->clearLabelPoints();
        painterArea->setLabelPoint(pathData.startPoint);
        painterArea->setLabelPoint(pathData.endPoint);
        // 选中表格中对应的点
        ui->tablePoints->clearSelection();
        selectTablePoint(pathData.startPoint->id);
        selectTablePoint(pathData.endPoint->id);
    }
    else
    {
        CurvePoint *p = pathData.startPoint;
        CurvePoint *c1 = p->pre;
        CurvePoint *c2 = nullptr;
        QList<CurvePoint*> cPoints;
        cPoints<<c1<<p;
        QList<QPointF> points;
        points.append(painterArea->myPathData->pointData[p->id]);
        while(!p->next->isCtrlPoint)
        {
            p = p->next;
            cPoints.append(p);
            points.append(painterArea->myPathData->pointData[p->id]);
        }
        c2 = p->next;
        cPoints.append(c2);
        // 画yellowPath
        MyPath path(painterArea);
        path.curveThrough_data(points,painterArea->myPathData->pointData[c1->id],painterArea->myPathData->pointData[c2->id]);
        yellowPath = *(path.myPath);
        painterArea->yellowPath = yellowPath;
        painterArea->setCenterToYellowPath();
        // 画greenPath
        painterArea->greenPath = QPainterPath();
        showCtrlPoint(c1);
        showCtrlPoint(c2);
        // 显示labelPoint、选中表格中对应的点
        painterArea->clearLabelPoints();
        ui->tablePoints->clearSelection();
        int n = cPoints.size();
        for(int i=0;i<n;i++)
        {
            painterArea->setLabelPoint(cPoints.at(i));
            selectTablePoint(cPoints.at(i)->id);
        }
    }
    ui->labelPathLen->setText("Length:"+QString::number(yellowPath.length())+"mm");
    this->statusBar()->showMessage(modelPaths->item(id,1)->text(),0);
    painterArea->update();
}

/**
 * @brief 没有用
 *
 * @param index
 */
void MainWindow::on_tablePaths_activated(const QModelIndex &index)
{
    qDebug()<<"on_tablePaths_activated";
}

/**
 * @brief 从tablePoints中选择点
 *
 * @param point
 */
void MainWindow::selectTablePoint(QPointF point)
{
    int i, numRows=modelPoints->rowCount();
    for(i=0;i<numRows;i++)
    {
        qreal x=modelPoints->item(i,0)->text().toDouble();
        qreal y=modelPoints->item(i,1)->text().toDouble();
        qreal dx=x-point.x(), dy=y-point.y(), E=0.1;
        if(dx<E && dx>-E && dy>-E && dy<E)
        {
            selectTablePoint(i);
            break;
        }
    }
}
/**
 * @brief 从tablePoints中选择点
 *
 * @param id
 */
void MainWindow::selectTablePoint(int id)
{
    ui->tablePoints->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tablePoints->selectRow(id);
    ui->tablePoints->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

/**
 * @brief 在painterArea上用label显示锚点
 *
 * @param ctrlPoint
 */
void MainWindow::showCtrlPoint(CurvePoint *ctrlPoint)
{
    if(!ctrlPoint->isCtrlPoint)
        return;
    CurvePoint *pointBeCtrled=nullptr;
    if(ctrlPoint->pre)
        pointBeCtrled=ctrlPoint->pre;
    else if(ctrlPoint->next)
        pointBeCtrled=ctrlPoint->next;
    else
        return;
    QPainterPath greenPath;
    greenPath.moveTo(painterArea->myPathData->pointData[pointBeCtrled->id]);
    greenPath.lineTo(painterArea->myPathData->pointData[ctrlPoint->id]);
    painterArea->greenPath.addPath(greenPath);
}

/**
 * @brief action触发 款式设计
 *
 */
void MainWindow::on_action_Design_triggered()
{
    dialogDesign->exec();
}

