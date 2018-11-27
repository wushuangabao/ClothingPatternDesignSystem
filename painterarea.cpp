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
    myPathData = new MyPathData("myPath");
    old_typePants = -1;
    typePants = 0;

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

    myPath = new MyPath(this);
    myPath->setStartPoint(500.0,100.0);
    auxiliaryLines.addPath(myPath->auxiliaryLinesH_1());
    myPath->drawOutline1(typeSang1);
    myPath->setStartPoint(100.0,100.0);
    auxiliaryLines.addPath(myPath->auxiliaryLinesH_2());
    //myPath->drawOutline2(typeSang2);

    selectedWidget=nullptr;
}

PainterArea::~PainterArea()
{
    delete myPathData;
}

//绘制事件
void PainterArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
//    QBrush brush(Qt::green);
//    painter.setBrush(brush);

    QSize painterAreaSize=this->size();
    qDebug()<<"painterAreaSize = "<<painterAreaSize<<endl;
    painter.setWindow(intLeft,intUp,qRound(painterAreaSize.width()/scalingMulti),qRound(painterAreaSize.height()/scalingMulti));
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

    pen.setWidthF(0);
    pen.setColor(Qt::white);
    painter.setPen(pen);
//    if(typePants==0)
//        painter.drawPath(*(myPath->myPath));
//    else
    {
        //如果myPathData有改动（typePants!=0），则按照myPathData绘图
        painter.drawPath(myPath->outLines_data());
    }
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
