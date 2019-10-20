#include <QtWidgets>
#include <QMessageBox>
#include <QDir>
#include <QtDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "painterarea.h"
#include "rules/mypainter.h"
#include "rules/myrule.h"
#include "dialog/dialogmodify/dialogms.h"
#include "dialog/dialogmodify/dialogmm.h"
#include "dialog/dialogdesign/dialogdesign.h"
#include "dialog/dialogrule/dialogrule.h"
#include "dialog/dialogtest/dialogtestcurve.h"
#include "dialog/dialogstyle/dialogstyle.h"
#include "data/mypathdata.h"
#include "rules/mypattern.h"


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
    connect(painterArea,SIGNAL(resetModel(int)),this,SLOT(resetModel(int)));
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
 * @param i 绘图路径在myPaths中的索引
 */
void MainWindow::resetModel(int i){
    modelPoints->clear();
    modelPaths->clear();
    MyPathData *data = painterArea->myPaths[i];
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
 * @brief action触发 尺寸修改
 *
 */
void MainWindow::on_action_M_M_triggered()
{
    if(dialogMM->exec()==QDialog::Accepted)
    {
        //painterArea->pantsCrotchH=dialogMM->Cro;
        //painterArea->pantsH=dialogMM->H;
        //painterArea->pantsHeight=dialogMM->Height;
        //painterArea->pantsL=dialogMM->L;
        //painterArea->pantsW=dialogMM->W;
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
    PathData pathData = painterArea->currentPath()->pathData[id];
    QPainterPath yellowPath;
    if(pathData.isLine)
    {
        QPointF sp = painterArea->currentPath()->pointData[pathData.startPoint->id],
                ep = painterArea->currentPath()->pointData[pathData.endPoint->id];
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
        points.append(painterArea->currentPath()->pointData[p->id]);
        while(!p->next->isCtrlPoint)
        {
            p = p->next;
            cPoints.append(p);
            points.append(painterArea->currentPath()->pointData[p->id]);
        }
        c2 = p->next;
        cPoints.append(c2);
        showCtrlPoint(c1);
        showCtrlPoint(c2);
        painterArea->greenPath = QPainterPath();
        // 画yellowPath
        MyPainter painter;
        painter.curve(points,painterArea->currentPath()->pointData[c1->id],painterArea->currentPath()->pointData[c2->id]);
        yellowPath = *(painter.myPath);
        painterArea->yellowPath = yellowPath;
        painterArea->setCenterToYellowPath();
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
    CurvePoint *pointBeCtrled = nullptr;
    if(ctrlPoint->pre)
        pointBeCtrled = ctrlPoint->pre;
    else if(ctrlPoint->next)
        pointBeCtrled = ctrlPoint->next;
    else
        return;
}

/**
 * @brief action触发 款式设计
 */
void MainWindow::on_action_Design_triggered()
{
    dialogDesign->exec();
}

/**
 * @brief action触发 测试曲线
 */
void MainWindow::on_actiontest_curve_triggered()
{
    DialogTestCurve* dialogTest = new DialogTestCurve(this);
    dialogTest->exec();
    delete dialogTest;
}

/**
 * @brief action触发 当前路径保存为txt
 *
 */
void MainWindow::on_action_F_S_triggered()
{
    QString filePath = QDir::currentPath() + "/" + painterArea->currentPath()->name;
    //    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
    //                       filePath + ".dxf",
    //                       tr("%1 Files (*.%2)")
    //                       .arg(QString::fromLatin1("DXF"))
    //                       .arg(QString::fromLatin1("dxf")));
    //    qDebug()<<"save to: "<<fileName;

    if(painterArea->currentPath()->saveTxtTo(filePath+".txt"))
        QMessageBox::information(nullptr,"Save","The txt file have saved successfully.");
    else
        QMessageBox::information(nullptr,"Save","The txt file failed to save!");
}

/**
 * @brief action触发 当前路径存为ASTM标准DXF格式
 */
void MainWindow::on_action_F_A_triggered()
{
    // 输出所有样板
    MyPattern pattern(painterArea->myPaths);
    QString filePath = QDir::currentPath() + "/" + pattern.infoName + ".dxf";
    pattern.writeASTM(filePath);

    // 输出当前的样板
    //    QString filePath=QDir::currentPath()+"/"+painterArea->currentPath()->name+".dxf";
    //    if(painterArea->currentPath()->writeASTM(filePath))
    //        QMessageBox::information(nullptr,"Save","The dxf file have saved successfully.");
    //    else
    //        QMessageBox::information(nullptr,"Save","The dxf file failed to save!");
}

/**
 * @brief action触发 当前路径存为AutoCAD标准DXF格式
 */
void MainWindow::on_action_F_D_triggered()
{
    QString filePath=painterArea->currentPath()->name+".dxf";
    if(painterArea->writeDXF(filePath))
        QMessageBox::information(nullptr,"Save","The dxf file have saved successfully.");
    else
        QMessageBox::information(nullptr,"Save","The dxf file failed to save!");
}

/**
 * @brief action触发 编辑、管理制板规则
 */
void MainWindow::on_action_Rule_triggered()
{
    DialogRule* dialogRule = new DialogRule(this);
    dialogRule->exec();
    delete dialogRule;
}

/**
 * @brief action触发 设置界面风格样式
 */
void MainWindow::on_action_Style_triggered()
{
    DialogStyle* dlg = new DialogStyle(this);
    dlg->exec();
    delete dlg;
}

/**
 * @brief 设置界面颜色
 *
 * @param color
 * @param object 修改颜色的对象
 */
void MainWindow::setColor(QString color, QString object)
{
    if(object == "painterArea"){
        painterArea->setColor(color);
    }
}

/**
 * @brief 移动painterArea中当前选中的路径的位置
 */
void MainWindow::on_action_MovePath_triggered()
{
    MyPathData* path = painterArea->currentPath();
    if(path == nullptr) return;
    qreal x = path->basePoint.x(),
          y = path->basePoint.y();
    bool ok = false;
    QString v = QInputDialog::getText(nullptr,"初始化-输入实体","基准点 =",QLineEdit::Normal,
                                      QString::number(x)+","+QString::number(y),&ok);
    int i = v.indexOf(',');
    if(!ok) return;
    x = v.left(i).toInt(&ok);
    y = v.mid(i+1).toInt(&ok);
    if(!ok) return;
    path->moveBasePointTo(QPointF(x,y));
    painterArea->greenPath = QPainterPath();
    painterArea->yellowPath = QPainterPath();
    this->resetModel(painterArea->currentId);
}

/**
 * @brief 对painterArea中当前选中的净板进行分片
 */
void MainWindow::on_action_FenPian_triggered()
{
    MyPathData* path = painterArea->currentPath();
    if(path == nullptr) return;
    painterArea->myPaths.removeAt(painterArea->currentId);
    painterArea->btnPaths.removeAt(painterArea->currentId);
    drawByRule("女裤/女裤前片分片.txt", path->params);
    drawByRule("女裤/女裤后片分片.txt", path->params);
    delete path; // 删除净样板
}

/**
 * @brief 根据规则文件画样板
 * @param path 规则文件的路径
 * @param in 输入的参数（字符串）
 */
void MainWindow::drawByRule(QString path, QString in)
{
    QString dir = QDir::currentPath() + "/rules/" + path;
    int i1 = dir.lastIndexOf("/"),
        i2 = dir.lastIndexOf(".");
    MyRule rule(dir);
    MyPainter mp = rule.drawPathWith(in);
    MyPathData* data = new MyPathData(*mp.myData);
    data->setName(dir.mid(i1+1).left(i2-i1-1));
    data->params = in;
    painterArea->addPath(data);
}
