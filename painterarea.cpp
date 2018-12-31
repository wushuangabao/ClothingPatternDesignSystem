#include <QtWidgets>
#include <QPalette>
#include <QPainter>
#include <QDebug>
#include <QRect>
#include "painterarea.h"
#include "mypath.h"
#include "data/mypathdata.h"
#include "data/labelpoint.h"

//构造函数
PainterArea::PainterArea(QWidget *parent) : QWidget(parent)
{
    myPathData = new MyPathData("myPath");
    old_typePants = -1;
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

PainterArea::~PainterArea()
{
    delete myPathData;
}

void PainterArea::setMyPath()
{
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

    if(old_typePants!=typePants)
    {
        QString filePath = QDir::currentPath() + "/" + myPathData->name;
        myPathData->saveTo(filePath+".txt");
        old_typePants = typePants;
        emit resetModel();
    }

//    pen.setWidthF(1);
    pen.setColor(Qt::white);
    painter.setPen(pen);
//    if()
        painter.drawPath(*(myPath->myPath));
//    else
//    {
//        //如果myPathData有改动，则按照myPathData绘图
//        painter.drawPath(myPath->outLines_data());
//    }

    pen.setWidthF(1.5);
    pen.setColor(Qt::yellow);
    painter.setPen(pen);
    painter.drawPath(yellowPath);
}

int PainterArea::xLogical(int xPhysical)
{
    return qRound((xPhysical+intLeft)/scalingMulti);
}
int PainterArea::yLogical(int yPhysical)
{
    return qRound((yPhysical+intUp)/scalingMulti);
}
int PainterArea::xPhysical(int xLogical)
{
    return qRound(xLogical*scalingMulti-intLeft);
}
int PainterArea::yPhysical(int yLogical)
{
    return qRound(yLogical*scalingMulti-intUp);
}

//鼠标事件
void PainterArea::mousePressEvent(QMouseEvent *event)
{
    QPoint pos=event->pos();
    QWidget *w=this->childAt(pos);
    if(w!=nullptr && w->inherits("LabelPoint") && event->button()==Qt::LeftButton)
    {   //拖动labelPoint
        selectedLabelPoint=reinterpret_cast<LabelPoint *>(w);
        selectedLabelPoint->setHandlerPos(pos);
    }
    else if(event->button()==Qt::MidButton)
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
void PainterArea::wheelEvent(QWheelEvent *event)
{
    int x = event->x();
    int y = event->y();
    //当滚轮远离使用者时进行放大，当滚轮向使用者方向旋转时进行缩小
    if(event->delta() > 0)
    {
        if(scalingMulti<3)
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

void PainterArea::setLabelPoint(int id,CurvePoint *point)
{
    qreal px=point->x, py=point->y,
          lpx=xPhysical(px),lpy=yPhysical(py);
    LabelPoint *lp=new LabelPoint(this);
    lp->setText(myPathData->findName(px,py));
    lp->setStyleSheet("QLabel{background-color:green;}");
    lp->move(lpx,lpy);
    lp->show();
    lp->id=id;
    lp->point=point;
    labelPoints.append(lp);
}

void PainterArea::clearLabelPoints()
{
    int num=labelPoints.size(),i;
    for(i=0;i<num;i++)
    {
        delete labelPoints.at(i);
    }
    labelPoints.clear();
}

void PainterArea::updateLabelPoints()
{
    int num=labelPoints.size(),i;
    for(i=0;i<num;i++)
    {
        LabelPoint* lp=labelPoints.at(i);
        qreal px=lp->point->x, py=lp->point->y,
              lpx=xPhysical(px),lpy=yPhysical(py);
        lp->move(lpx,lpy);
    }
}

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

void PainterArea::changePath()
{
}
