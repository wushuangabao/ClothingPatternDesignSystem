#include <QtWidgets>
#include <QPalette>
#include <QPainter>
#include <QDebug>
#include <QRect>
#include "painterarea.h"
#include "mypath.h"
#include "data/mypathdata.h"

//构造函数
PainterArea::PainterArea(QWidget *parent) : QWidget(parent)
{
    myPathDatas = new QList<MyPathData>;

    pantsHeight=1650;
    pantsL=1020;
    pantsW=720;
    pantsH=940;
    pantsCrotchH=245;
    typeSang1=0;
    typeSang2=0;

    //设置背景为黑色
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, Qt::black);
    this->setAutoFillBackground(true);
    this->setPalette(pal);

//    startLabel=new QLabel(this);
//    startLabel->resize(4, 4);
//    startLabel->setStyleSheet("QLabel{background-color:red;}");
//    startLabel->move(40, 40);

//    ctrlLabel1=new QLabel(this);
//    ctrlLabel1->resize(4, 4);
//    ctrlLabel1->setStyleSheet("QLabel{background-color:yellow;}");
//    ctrlLabel1->move(0, 0);

//    ctrlLabel2=new QLabel(this);
//    ctrlLabel2->resize(4, 4);
//    ctrlLabel2->setStyleSheet("QLabel{background-color:yellow;}");
//    ctrlLabel2->move(0, 0);

//    ctrlLabel3=new QLabel(this);
//    ctrlLabel3->resize(4, 4);
//    ctrlLabel3->setStyleSheet("QLabel{background-color:yellow;}");
//    ctrlLabel3->move(0, 0);

//    ctrlLabel4=new QLabel(this);
//    ctrlLabel4->resize(4, 4);
//    ctrlLabel4->setStyleSheet("QLabel{background-color:yellow;}");
//    ctrlLabel4->move(0, 0);

    selectedWidget=nullptr;
    startPoint=QPoint(40,40);
}

PainterArea::~PainterArea()
{
    delete myPathDatas;
}

//绘制事件
void PainterArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QPen pen;
    MyPath path(this);

    QSize painterAreaSize=this->size();
    qDebug()<<"painterAreaSize = "<<painterAreaSize<<endl;
    painter.setWindow(intLeft,intUp,qRound(painterAreaSize.width()/scalingMulti),qRound(painterAreaSize.height()/scalingMulti));
    painter.setRenderHint(QPainter::Antialiasing);
    QPointF startPoint2=startPoint;
    QPointF startPoint1(startPoint2.x()+400,startPoint2.y());

    pen.setWidthF(0);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.drawPath(path.auxiliaryLinesH_1(startPoint1));
    painter.drawPath(path.auxiliaryLinesH_2(startPoint2));
    pen.setWidthF(0);
    pen.setColor(Qt::white);
    painter.setPen(pen);
    painter.drawPath(path.outlineH_1(startPoint1,typeSang1));
    painter.drawPath(path.outlineH_2(startPoint2,typeSang2));
}

//鼠标事件
void PainterArea::mousePressEvent(QMouseEvent *event)
{
    QWidget *w=this->childAt(event->pos());
    if(w!=nullptr && w->inherits("QLabel") && event->button()==Qt::LeftButton){
        selectedWidget=w;
    }else if(event->button()==Qt::MidButton){
        isMoving=true;
        xBeforeMoving=event->pos().x();
        yBeforeMoving=event->pos().y();
        qDebug()<<"(x,y)beforeMoving = "<<xBeforeMoving<<","<<yBeforeMoving<<endl;
    }
    //状态栏显示selectedWidget的坐标
    mouseCoordinate = QString::number(event->x()) + "," + QString::number(event->y());
    emit mouseCoordinateChanged();
}
void PainterArea::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
        selectedWidget=nullptr;
    else if(event->button()==Qt::MidButton){
        isMoving=false;
        intLeft=intLeft-event->pos().x()+xBeforeMoving;
        intUp=intUp-event->pos().y()+yBeforeMoving;
        this->update();
    }
}
void PainterArea::mouseMoveEvent(QMouseEvent *event)
{
    //移动selectedWidget
    if(selectedWidget!=nullptr){
        selectedWidget->move(event->pos());
        this->update();
    }
    //拖动画布
    else if(isMoving){
        intLeft=intLeft-event->pos().x()+xBeforeMoving;
        intUp=intUp-event->pos().y()+yBeforeMoving;
        xBeforeMoving=event->pos().x();
        yBeforeMoving=event->pos().y();
        this->update();
    }
    //状态栏显示selectedWidget的坐标
    mouseCoordinate = QString::number(event->x()) + "," + QString::number(event->y());
    emit mouseCoordinateChanged();
}
void PainterArea::wheelEvent(QWheelEvent *event)
{
    //int x = event->x();
    //int y = event->y();
    //当滚轮远离使用者时进行放大，当滚轮向使用者方向旋转时进行缩小
    if(event->delta() > 0)
        scalingMulti = scalingMulti+0.1;
    else
        scalingMulti = scalingMulti-0.1;
    this->update();
    emit scalingMultiChanged();
}

void PainterArea::setTypeSang(int frontOrBack,int intCase)
{
    if(frontOrBack==1)
        typeSang1 = intCase;
    else if(frontOrBack==2)
        typeSang2 = intCase;
    qDebug()<<"typeSang changed"<<endl;
    this->update();
}
