#include <QtWidgets>
#include <QPalette>
#include <QPainter>
#include <QDebug>
#include <QRect>
#include "painterarea.h"
#include "mypath.h"
#include "data/mypathdata.h"
#include "data/labelpoint.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "dxflib/dl_dxf.h"
#include "dxflib/dl_creationadapter.h"
#include "dxflib/test_creationclass.h"

//构造函数
/**
 * @brief
 *
 * @param parent
 */
PainterArea::PainterArea(QWidget *parent) : QWidget(parent)
{
    myPathData = new MyPathData("myPath");
    //    old_typePants = -1;
    typePants = 0;

    pantsHeight=1650;
    pantsL=1020;
    pantsW=720;
    pantsH=940;
    pantsCrotchH=245;
    typeSang1=0;
    typeSang2=1;

    myPath = new MyPath(this); //必须等设置完上面的一系列尺寸再构造

    //设置背景为黑色
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, Qt::black);
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    setMouseTracking(true);//始终跟踪鼠标
    selectedLabelPoint=nullptr;

    //下面代码用于测试
    //setMyPath();

}

/**
 * @brief
 *
 */
PainterArea::~PainterArea()
{
    delete myPathData;
}

/**
 * @brief 重新绘制路径
 *
 */
void PainterArea::setMyPath()
{
    dataChanged=false;
    myPath->initializeSize();
    if(!myPath->myPath->isEmpty()){
        delete myPath->myPath;
        myPath->myPath=new QPainterPath;
    }
    QPainterPath path;
    auxiliaryLines=path;

    myPath->setStartPoint(500.0,100.0);
    auxiliaryLines.addPath(myPath->auxiliaryLinesH_1());
    myPath->drawOutline1(typeSang1);

    myPath->setStartPoint(100.0,100.0);
    auxiliaryLines.addPath(myPath->auxiliaryLinesH_2());
    myPath->drawOutline2(typeSang2);
}

//绘制事件
/**
 * @brief
 *
 * @param event
 */
void PainterArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    //    QBrush brush(Qt::white);
    //    painter.setBrush(brush);

    QSize painterAreaSize=this->size();
    painter.setWindow(qRound(intLeft/scalingMulti),
                      qRound(intUp/scalingMulti),
                      qRound(painterAreaSize.width()/scalingMulti),
                      qRound(painterAreaSize.height()/scalingMulti));
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen;
    pen.setWidthF(0);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.drawPath(auxiliaryLines);

    //    if(old_typePants!=typePants)
    //    {
    //        QString filePath = QDir::currentPath() + "/" + myPathData->name;
    //        myPathData->saveTo(filePath+".txt");
    //        old_typePants = typePants;
    //        emit resetModel();
    //    }

    //    pen.setWidthF(1);
    pen.setColor(Qt::white);
    painter.setPen(pen);
    if(!dataChanged)
        painter.drawPath(*(myPath->myPath));
    else
    {
        //如果myPathData有改动，则按照myPathData绘图
        painter.drawPath(myPath->outLines_data());
    }

    pen.setWidthF(1.5);
    pen.setColor(Qt::yellow);
    painter.setPen(pen);
    painter.drawPath(yellowPath);

    pen.setWidthF(1.5);
    pen.setColor(Qt::green);
    painter.setPen(pen);
    painter.drawPath(greenPath);
}

/**
 * @brief
 *
 * @param xPhysical
 * @return int
 */
int PainterArea::xLogical(int xPhysical)
{
    return qRound((xPhysical+intLeft)/scalingMulti);
}
/**
 * @brief
 *
 * @param yPhysical
 * @return int
 */
int PainterArea::yLogical(int yPhysical)
{
    return qRound((yPhysical+intUp)/scalingMulti);
}
/**
 * @brief
 *
 * @param xLogical
 * @return int
 */
int PainterArea::xPhysical(int xLogical)
{
    return qRound(xLogical*scalingMulti-intLeft);
}
/**
 * @brief
 *
 * @param yLogical
 * @return int
 */
int PainterArea::yPhysical(int yLogical)
{
    return qRound(yLogical*scalingMulti-intUp);
}

//鼠标事件
/**
 * @brief
 *
 * @param event
 */
void PainterArea::mousePressEvent(QMouseEvent *event)
{
    QPoint pos=event->pos();
    QWidget *w=this->childAt(pos);
    if(w!=nullptr && w->inherits("LabelPoint"))
    {
        if(event->button()==Qt::LeftButton) //左键：拖动labelPoint
        {
            selectedLabelPoint=reinterpret_cast<LabelPoint *>(w);
            selectedLabelPoint->setHandlerPos(pos);
        }
    }
    if(event->button()==Qt::MidButton)
    {   //拖动画布
        isMoving=true;
        xBeforeMoving=pos.x();
        yBeforeMoving=pos.y();
    }
    //状态栏显示坐标
    stringTempStatus = tr("物理(")+QString::number(event->x()) + "," + QString::number(event->y()) +
            tr(") 逻辑(")+QString::number(xLogical(event->x())) + "," + QString::number(yLogical(event->y())) + ")";
    emit mouseCoordinateChanged();
}
/**
 * @brief
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
 * @brief
 *
 * @param event
 */
void PainterArea::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos=event->pos();
    if(selectedLabelPoint!=nullptr)
    {   //拖动labelPoint
        selectedLabelPoint->moveTo(pos);
        this->update();
    }
    else if(isMoving)
    {   //拖动画布
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
 * @brief
 *
 * @param event
 */
void PainterArea::wheelEvent(QWheelEvent *event)
{
    int x = event->x();
    int y = event->y();
    //当滚轮远离使用者时进行放大，当滚轮向使用者方向旋转时进行缩小
    if(event->delta() > 0)
    {
        if(scalingMulti<5)
        {
            scalingMulti = scalingMulti+0.1;
            intLeft+=qRound(0.1*x);
            intUp+=qRound(0.1*y);
        }
        //        else
        //        {
        //            scalingMulti = scalingMulti+0.5;
        //            intLeft+=qRound(0.5*x);
        //            intUp+=qRound(0.5*y);
        //        }
    }
    else
    {
        if(scalingMulti>0.101)
        {
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
 * @brief
 *
 * @param point
 */
void PainterArea::setLabelPoint(CurvePoint *point)
{
    QPointF p = myPathData->pointData[point->id];
    qreal lpx=xPhysical(p.x()),lpy=yPhysical(p.y());
    LabelPoint *lp=new LabelPoint(this);
    if(point->isCtrlPoint)
    {
        lp->setText("ctrlPoint");
        lp->setStyleSheet("QLabel{background-color:white;}");
        lp->move(lpx,lpy-lp->height());
    }
    else
    {
        lp->setText(myPathData->findName(point->id));
        lp->setStyleSheet("QLabel{background-color:green;}");
        lp->move(lpx,lpy);
    }
    lp->show();
    lp->point=point;
    labelPoints.append(lp);
}

/**
 * @brief
 *
 */
void PainterArea::clearLabelPoints()
{
    int num=labelPoints.size(),i;
    for(i=0;i<num;i++)
    {
        delete labelPoints.at(i);
    }
    labelPoints.clear();
}

/**
 * @brief
 *
 */
void PainterArea::updateLabelPoints()
{
    int num=labelPoints.size(),i;
    for(i=0;i<num;i++)
    {
        LabelPoint* lp=labelPoints.at(i);
        QPointF p=myPathData->pointData[lp->point->id];
        qreal lpx=xPhysical(p.x()),lpy=yPhysical(p.y());
        lp->move(lpx,lpy);
    }
}

/**
 * @brief
 *
 * @param frontOrBack
 * @param intCase
 */
void PainterArea::setTypeSang(int frontOrBack,int intCase)
{
    if(frontOrBack==1)
        typeSang1 = intCase;
    else if(frontOrBack==2)
        typeSang2 = intCase;
    qDebug()<<"typeSang changed"<<endl;
    setMyPath();
    update();
}

/**
 * @brief 输出dxf文件
 *
 * @return bool
 */
bool PainterArea::writeDXF() {
    DL_Dxf* dxf = new DL_Dxf();
    DL_Codes::version exportVersion = DL_Codes::AC1015;
    DL_WriterA* dw = dxf->out("myfile.dxf", exportVersion);
    if (dw==NULL) {
        printf("Cannot open file 'myfile.dxf' \
               for writing.");
               // abort function e.g. with return
    }

    dxf->writeHeader(*dw);

    // int variable:
    dw->dxfString(9, "$INSUNITS");
    dw->dxfInt(70, 4);
    // real (double, float) variable:
    dw->dxfString(9, "$DIMEXE");
    dw->dxfReal(40, 1.25);
    // string variable:
    dw->dxfString(9, "$TEXTSTYLE");
    dw->dxfString(7, "Standard");
    // vector variable:
    dw->dxfString(9, "$LIMMIN");
    dw->dxfReal(10, 0.0);
    dw->dxfReal(20, 0.0);

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

    MyPathData *data = myPathData;
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
                        DL_Attributes("mainlayer", 256, -1, "BYLAYER", 1.0));
        }
        else //曲线
        {
            QList<QPointF> points;
            MyPath path(this);
            qreal t=0.0;
            CurvePoint *p = pathData.startPoint->pre;
            QPointF firstCtrlPoint = data->pointData[p->id];
            while(p->next->isCtrlPoint!=true)
            {
                p=p->next; points<<data->pointData[p->id];
            }
            p=p->next;
            QPointF lastCtrlPoint = data->pointData[p->id];
            path.curveThrough_data(points,firstCtrlPoint,lastCtrlPoint);
            qreal dt=1.0/path.myPath->length();  //以每1mm为一根小线段
            while(t<1) /**< TODO:使曲线的经过点成为小线段的端点 */
            {
                QPointF sp=path.myPath->pointAtPercent(t);
                t=t+dt;
                if(t>1)
                    t=1;
                QPointF ep=path.myPath->pointAtPercent(t);
                dxf->writeLine(
                            *dw,
                            DL_LineData(sp.x()/10,
                                        -sp.y()/10,
                                        0.0,
                                        ep.x()/10,
                                        -ep.y()/10,
                                        0.0
                                        ),
                            DL_Attributes("mainlayer", 256, -1, "BYLAYER", 1.0));
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

// 设置画布中心点坐标为yellowPath的中心点
/**
 * @brief
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
    intUp=(centerRect.y()-yCenter)*scalingMulti;
    intLeft=(centerRect.x()-xCenter)*scalingMulti;

    emit scalingMultiChanged();
}

/**
 * @brief
 *
 */
void PainterArea::changePath()
{
}
