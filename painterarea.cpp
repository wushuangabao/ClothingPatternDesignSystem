#include <QtWidgets>
#include <QPalette>
#include <QPainter>
#include <QDebug>
#include <QRect>
#include "painterarea.h"
#include "rules/mypainter.h"
#include "data/mypathdata.h"
#include "data/labelpoint.h"
#include "data/pathbutton.h"
#include "mainwindow.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "dxflib/dl_dxf.h"

/**
 * @brief 构造函数
 *
 * @param parent
 */
PainterArea::PainterArea(QWidget *parent) : QWidget(parent)
{
    currentId=-1;

    // 设置背景颜色
    color = Qt::black;
    this->setAutoFillBackground(true);
    setColor();

    setMouseTracking(true);  // 始终跟踪鼠标
    selectedLabelPoint=nullptr;
}

/**
 * @brief 析构函数
 *
 */
PainterArea::~PainterArea()
{
    clearLabelPoints();
    int size = myPaths.size();
    if(size>0)
        for(int i=0;i<size;++i){
            delete myPaths[i];
            delete btnPaths[i];
        }
    size = pathNames.size();
    if(size > 0)
        for(int i = 0;i < size; ++i)
            delete pathNames[i];
}

/**
 * @brief 新增path到myPaths数组
 * @param path
 */
void PainterArea::addPath(MyPathData *path)
{
    int i = myPaths.size();
    myPaths << path;
    setCurrentPath(i);

    // 在状态栏添加按钮
    MainWindow* mw = static_cast<MainWindow*>(parent());
    PathButton* btn = new PathButton(myPaths[i]->name,this);
    btn->index = btnPaths.size();
    connect(btn,SIGNAL(clicked()),btn,SLOT(beClicked()));
    btnPaths << btn;
    mw->statusBar()->insertPermanentWidget(i+1, btn);
}

/**
 * @brief 获取当前的 MyPathData
 * @return
 */
MyPathData *PainterArea::currentPath()
{
    if(myPaths.isEmpty() || currentId < 0 || currentId >= myPaths.size())
        return nullptr;
    else return myPaths[currentId];
}

/**
 * @brief 设置当前选中的path
 * @param i -1表示不重新选择当前的currentId
 * @return bool
 */
bool PainterArea::setCurrentPath(int i)
{
    if(i >= myPaths.size())
        return false;
    else if(i != -1)
        currentId = i;
    MainWindow* mw = static_cast<MainWindow*>(parent());
    if(currentId >= 0 && currentId < myPaths.size()){
        mw->resetModel(currentId);
        this->update();
    }
    else return false;
    return true;
}

/**
 * @brief 绘制事件
 *
 * @param event
 */
void PainterArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QBrush brushGreen(Qt::green);
    QBrush brushEmpty;

    QSize painterAreaSize=this->size();
    painter.setWindow(qRound(intLeft/scalingMulti),
                      qRound(intUp/scalingMulti),
                      qRound(painterAreaSize.width()/scalingMulti),
                      qRound(painterAreaSize.height()/scalingMulti));
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen;
//    pen.setWidthF(0);
//    painter.setPen(pen);
//    painter.drawPath(auxiliaryLines);

    // 遍历 myPaths 进行 drawByPathData
    MyPainter myPainter;
    qreal k;
    if(!myPaths.isEmpty())
        for(int i=0; i<myPaths.size(); ++i){
            if(i == currentId){
                //pen.setStyle(Qt::SolidLine);
                k = 1.2;
            }
            else{
                //pen.setStyle(Qt::DotLine);
                k = 0.6;
            }
            // 辅助线：青色
            pen.setColor(Qt::cyan);
            pen.setWidthF(0.5 / scalingMulti * k);
            painter.setPen(pen);
            painter.drawPath(myPainter.drawByPathData(myPaths[i], 3));
            // 内部线：白色
            pen.setColor(Qt::white);
            pen.setWidthF(0.6 / scalingMulti * k);
            painter.setPen(pen);
            painter.drawPath(myPainter.drawByPathData(myPaths[i], 1));
            // 经向线：红色
            pen.setColor(Qt::red);
            pen.setWidthF(0);
            painter.setPen(pen);
            QList<QPointF> posNames;
            QList<QString> names;
            painter.drawPath(myPainter.drawByPathData(myPaths[i], 2, &posNames, &names));
            drawPathNames(posNames, names);
            // 轮廓线：棕色（dark orange）
            pen.setColor(QColor(205,102,0));
            pen.setWidthF(1.8 / scalingMulti * k);
            painter.setPen(pen);
            painter.drawPath(myPainter.drawByPathData(myPaths[i], 0));
            // 点：绿色
            pen.setColor(Qt::green);
            pen.setWidthF(0.2 / scalingMulti * k);
            painter.setBrush(brushGreen);
            painter.setPen(pen);
            painter.drawPath(myPainter.drawPointsByData(myPaths[i]));
            painter.setBrush(brushEmpty);
        }

    pen.setWidthF(0);
    pen.setColor(Qt::yellow);
    painter.setPen(pen);
    painter.drawPath(yellowPath);

    pen.setColor(Qt::green);
    painter.setPen(pen);
    painter.drawPath(greenPath);
}

/**
 * @brief 逻辑横坐标
 *
 * @param xPhysical
 * @return int
 */
int PainterArea::xLogical(int xPhysical)
{
    return qRound((xPhysical+intLeft)/scalingMulti);
}
/**
 * @brief 逻辑纵坐标
 *
 * @param yPhysical
 * @return int
 */
int PainterArea::yLogical(int yPhysical)
{
    return qRound((yPhysical+intUp)/scalingMulti);
}
/**
 * @brief 物理横坐标
 *
 * @param xLogical
 * @return int
 */
int PainterArea::xPhysical(int xLogical)
{
    return qRound(xLogical*scalingMulti-intLeft);
}
/**
 * @brief 物理纵坐标
 *
 * @param yLogical
 * @return int
 */
int PainterArea::yPhysical(int yLogical)
{
    return qRound(yLogical*scalingMulti-intUp);
}

/**
 * @brief 逻辑横坐标
 *
 * @param xPhysical
 * @return int
 */
int PainterArea::xLogical(qreal xPhysical)
{
    return qRound((xPhysical+intLeft)/scalingMulti);
}
/**
 * @brief 逻辑纵坐标
 *
 * @param yPhysical
 * @return int
 */
int PainterArea::yLogical(qreal yPhysical)
{
    return qRound((yPhysical+intUp)/scalingMulti);
}
/**
 * @brief 物理横坐标
 *
 * @param xLogical
 * @return int
 */
int PainterArea::xPhysical(qreal xLogical)
{
    return qRound(xLogical*scalingMulti-intLeft);
}
/**
 * @brief 物理纵坐标
 *
 * @param yLogical
 * @return int
 */
int PainterArea::yPhysical(qreal yLogical)
{
    return qRound(yLogical*scalingMulti-intUp);
}

/**
 * @brief 鼠标按压事件
 *
 * @param event
 */
void PainterArea::mousePressEvent(QMouseEvent *event)
{
    QPoint pos=event->pos();
    QWidget *w=this->childAt(pos);
    if(w!=nullptr && w->inherits("LabelPoint"))
    {
        // 左键拖动labelPoint
        if(event->button()==Qt::LeftButton){
            selectedLabelPoint = reinterpret_cast<LabelPoint *>(w);
            selectedLabelPoint->setHandlerPos(pos);
        }
    }
    if(event->button()==Qt::MidButton)
    {
        // 拖动画布
        isMoving=true;
        xBeforeMoving=pos.x();
        yBeforeMoving=pos.y();
    }
    // 状态栏显示坐标
    stringTempStatus = tr("物理(")+QString::number(event->x()) + "," + QString::number(event->y()) +
            tr(") 逻辑(")+QString::number(xLogical(event->x())) + "," + QString::number(yLogical(event->y())) + ")";
    emit mouseCoordinateChanged();
}
/**
 * @brief 鼠标释放事件
 *
 * @param event
 */
void PainterArea::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
        selectedLabelPoint=nullptr;
    else if(event->button()==Qt::MidButton)
    {
        isMoving=false;
        intLeft=intLeft-event->pos().x()+xBeforeMoving;
        intUp=intUp-event->pos().y()+yBeforeMoving;
        this->update();
    }
}
/**
 * @brief 鼠标移动事件
 *
 * @param event
 */
void PainterArea::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos=event->pos();
    if(selectedLabelPoint!=nullptr)
    {
        // 拖动labelPoint
        selectedLabelPoint->moveTo(pos);
        // 改变曲线中间的控制点
        CurvePoint* cp = selectedLabelPoint->point;
        if(isCurvePath(&yellowPath) && !cp->isLast && !cp->isFirst){
            selectedLabelPoint->changePos();
            if(cp->isCtrlPoint){
                // 改变绿线
                greenPath = QPainterPath();
                greenPath.moveTo(currentPath()->pointData[cp->id]);
                if(cp->pre != nullptr)
                    greenPath.lineTo(currentPath()->pointData[cp->pre->id]);
                else if(cp->next != nullptr)
                    greenPath.lineTo(currentPath()->pointData[cp->next->id]);
            }
            this->update();
        }
    }
    else if(isMoving)
    {
        // 拖动画布
        intLeft=intLeft-pos.x()+xBeforeMoving;
        intUp=intUp-pos.y()+yBeforeMoving;
        xBeforeMoving=pos.x();
        yBeforeMoving=pos.y();
        this->update();
        updateLabelPoints();
    }
    stringTempStatus = tr("物理(")+QString::number(event->x()) + "," + QString::number(event->y()) +
            tr(") 逻辑(")+QString::number(xLogical(event->x())) + "," + QString::number(yLogical(event->y())) + ")";
    emit mouseCoordinateChanged();
}
/**
 * @brief 鼠标滚轮事件
 *
 * @param event
 */
void PainterArea::wheelEvent(QWheelEvent *event)
{
    int x = event->x();
    int y = event->y();
    // 当滚轮远离使用者时进行放大，当滚轮向使用者方向旋转时进行缩小
    if(event->delta() > 0){
        if(scalingMulti<5){
            scalingMulti = scalingMulti+0.1;
            intLeft+=qRound(0.1*x);
            intUp+=qRound(0.1*y);
        }
    }
    else{
        if(scalingMulti>0.101){
            scalingMulti = scalingMulti-0.1;
            intLeft-=0.1*x;
            intUp-=0.1*y;
        }
    }
    this->update();
    updateLabelPoints();
    emit scalingMultiChanged();
}

/**
 * @brief path是否是曲线
 * @param path
 * @return
 */
bool PainterArea::isCurvePath(QPainterPath *path)
{
    int sizePath = path->elementCount();
    for(int i=0;i<sizePath;++i){
        if(path->elementAt(i).type==QPainterPath::CurveToElement)
            return true;
    }
    return false;
}

/**
 * @brief 画样板的名称，其实就设置 pathNames 标签数组
 * @param posList
 * @param names
 */
void PainterArea::drawPathNames(QList<QPointF> posList, QList<QString> names)
{
    int size = posList.size();
    if(size <= 0)
        return;
    for(int i = 0; i < size; ++i){
        QLabel *label = new QLabel(names[i], this);
        label->move(posList[i].x(), posList[i].y());
        pathNames.append(label);
        label->show();
    }
}

/**
 * @brief 设置选中点的标签
 *
 * @param point
 */
void PainterArea::setLabelPoint(CurvePoint *point)
{
    QPointF p = currentPath()->pointData[point->id];
    int lpx=xPhysical(p.x()),lpy=yPhysical(p.y());
    LabelPoint *lp=new LabelPoint(this);
    if(point->isCtrlPoint)
    {
        lp->setText("ctrlPoint");
        lp->setStyleSheet("QLabel{background-color:white;}");
        lp->move(lpx,lpy);
    }
    else
    {
        lp->setText(currentPath()->findName(point->id));
        lp->setStyleSheet("QLabel{background-color:green;}");
        lp->move(lpx,lpy);
    }
    lp->show();
    lp->point = point;
    lp->setOldPoint(p);
    labelPoints.append(lp);
}

/**
 * @brief 清除所有点的标签
 *
 */
void PainterArea::clearLabelPoints()
{
    int num=labelPoints.size(), i;
    if(num == 0)
        return;
    for(i=0;i<num;i++)
        delete labelPoints.at(i);
    labelPoints.clear();
}

/**
 * @brief 更新点标签的位置
 *
 */
void PainterArea::updateLabelPoints()
{
    int num=labelPoints.size(),i;
    for(i=0;i<num;i++)
    {
        LabelPoint* lp=labelPoints.at(i);
        QPointF p=myPaths[currentId]->pointData[lp->point->id];
        int lpx=xPhysical(p.x()),lpy=yPhysical(p.y());
        lp->move(lpx,lpy);
    }
}

/**
 * @brief 重置省褶
 *
 * @param frontOrBack 1：前片 2：后片
 * @param intCase 类型
 */
void PainterArea::setTypeSang(int frontOrBack,int intCase)
{
    //if(frontOrBack==1)
        //typeSang1 = intCase;
    //else if(frontOrBack==2)
        //typeSang2 = intCase;
    emit resetModel(currentId);
    update();
}

/**
 * @brief 输出dxf文件（AutoCAD标准）
 *
 * @return bool
 */
bool PainterArea::writeDXF(QString fileName) {
    DL_Dxf* dxf = new DL_Dxf();
    DL_Codes::version exportVersion = DL_Codes::AC1015;
    DL_WriterA* dw = dxf->out(fileName.toUtf8(), exportVersion);
    if (dw==nullptr)
        return false;

    dxf->writeHeader(*dw);

    dw->sectionEnd();
    dw->sectionTables();
    dxf->writeVPort(*dw);

    dw->tableLinetypes(3);
    dxf->writeLinetype(*dw, DL_LinetypeData("BYBLOCK", "BYBLOCK", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("BYLAYER", "BYLAYER", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("CONTINUOUS", "Continuous", 0, 0, 0.0));
    dw->tableEnd();

    int numberOfLayers = 3;
    dw->tableLayers(numberOfLayers);
    dxf->writeLayer(*dw,
                    DL_LayerData("0", 0),
                    DL_Attributes(
                        std::string(""),      // leave empty
                        DL_Codes::black,        // default color
                        100,                  // default width
                        "CONTINUOUS", 1.0));       // default line style
    dxf->writeLayer(*dw,
                    DL_LayerData("mainlayer", 0),
                    DL_Attributes(
                        std::string(""),
                        DL_Codes::red,
                        100,
                        "CONTINUOUS", 1.0));
    dxf->writeLayer(*dw,
                    DL_LayerData("anotherlayer", 0),
                    DL_Attributes(
                        std::string(""),
                        DL_Codes::black,
                        100,
                        "CONTINUOUS", 1.0));
    dw->tableEnd();

    dw->tableStyle(1);
    dxf->writeStyle(*dw, DL_StyleData("standard", 0, 2.5, 1.0, 0.0, 0, 2.5, "txt", ""));
    dw->tableEnd();

    dxf->writeView(*dw);
    dxf->writeUcs(*dw);
    dw->tableAppid(1);
    dxf->writeAppid(*dw, "ACAD");
    dw->tableEnd();

    dxf->writeDimStyle(*dw, 1, 1, 1, 1, 1);
    dxf->writeBlockRecord(*dw);
    dxf->writeBlockRecord(*dw, "myblock1");
    dxf->writeBlockRecord(*dw, "myblock2");
    dw->tableEnd();

    dw->sectionEnd();

    dw->sectionBlocks();
    dxf->writeBlock(*dw, DL_BlockData("*Model_Space", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Model_Space");
    dxf->writeBlock(*dw, DL_BlockData("*Paper_Space", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Paper_Space");
    dxf->writeBlock(*dw, DL_BlockData("*Paper_Space0", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Paper_Space0");

    dxf->writeBlock(*dw, DL_BlockData("myblock1", 0, 0.0, 0.0, 0.0));
    // ...
    // write block entities e.g. with dxf->writeLine(), ..
    // ...
    dxf->writeEndBlock(*dw, "myblock1");

    dxf->writeBlock(*dw, DL_BlockData("myblock2", 0, 0.0, 0.0, 0.0));
    // ...
    // write block entities e.g. with dxf->writeLine(), ..
    // ...
    dxf->writeEndBlock(*dw, "myblock2");

    dw->sectionEnd();
    dw->sectionEntities();
    // write all entities in model space:
    MyPathData *data = myPaths[currentId];
    int numPaths = data->numberPath, i;
    for(i=0;i<numPaths;++i)
    {
        PathData pathData = data->pathData[i];
        QPointF startPoint = data->pointData[pathData.startPoint->id];
        if(pathData.isLine)
        {
            QPointF endPoint = data->pointData[pathData.endPoint->id];
            dxf->writeLine(
                        *dw,
                        DL_LineData(startPoint.x()/10,
                                    -startPoint.y()/10,
                                    0.0,
                                    endPoint.x()/10,
                                    -endPoint.y()/10,
                                    0.0
                                    ),
                        DL_Attributes("0", 256, -1, "BYLAYER", 1.0));
        }
        else //曲线
        {
            QList<QPointF> points;
            MyPainter painter;
            qreal t=0.0;
            CurvePoint *p = pathData.startPoint->pre;
            QPointF firstCtrlPoint = data->pointData[p->id];
            while(p->next->isCtrlPoint!=true)
            {
                p=p->next; points<<data->pointData[p->id];
            }
            p=p->next;
            QPointF lastCtrlPoint = data->pointData[p->id];
            painter.curve(points,firstCtrlPoint,lastCtrlPoint);
            qreal dt=1.0/painter.myPath->length();  //以每1mm为一根小线段
            while(t<1) /**< TODO:使曲线的经过点成为小线段的端点 */
            {
                QPointF sp=painter.myPath->pointAtPercent(t);
                t=t+dt;
                if(t>1)
                    t=1;
                QPointF ep=painter.myPath->pointAtPercent(t);
                dxf->writeLine(
                            *dw,
                            DL_LineData(sp.x()/10,
                                        -sp.y()/10,
                                        0.0,
                                        ep.x()/10,
                                        -ep.y()/10,
                                        0.0
                                        ),
                            DL_Attributes("0", 256, -1, "BYLAYER", 1.0));
            }
        }
    }

    dw->sectionEnd();

    dxf->writeObjects(*dw);
    dxf->writeObjectsEnd(*dw);

    dw->dxfEOF();
    dw->close();
    delete dw;
    delete dxf;

    return true;
}

/**
 * @brief 设置画布中心点坐标为yellowPath的中心点
 *
 */
void PainterArea::setCenterToYellowPath()
{
    scalingMulti=1.0;
    QRectF rect=yellowPath.boundingRect();
    QPointF centerRect=rect.center();
    QSize sizePainterArea=this->size();
    qreal xCenter=sizePainterArea.width()/2.0,
            yCenter=sizePainterArea.height()/2.0;
    intUp=static_cast<int>((centerRect.y()-yCenter)*scalingMulti);
    intLeft=static_cast<int>((centerRect.x()-xCenter)*scalingMulti);

    emit scalingMultiChanged();
}

/**
 * @brief 设置背景颜色
 * @param color
 */
void PainterArea::setColor(QString color)
{
    if(color != "")
        this->color = QColor(color);
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, this->color);
    this->setPalette(pal);
}
