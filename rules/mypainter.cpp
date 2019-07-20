#include "mypainter.h"
#include "myrule.h"
#include "../painterarea.h"
#include "../mainwindow.h"
#include "../data/mypathdata.h"
#include <QPainter>

MyPainter::MyPainter(PainterArea* parent)
{
    this->parent = parent;
    myPath = new QPainterPath;
    startPoint = new QPointF(0,0);
}

MyPainter::~MyPainter()
{
    delete  myPath;
    delete  startPoint;
}

void MyPainter::setStartPoint(qreal x, qreal y)
{
    startPoint->setX(x);
    startPoint->setY(y);
}

void MyPainter::setStartPoint(QPointF point)
{
    *startPoint = point;
}

/**
 * @brief 解析路径代码，添加数据到parent->myPahtData
 * @param rule 包含点坐标信息的MyRule数据
 * @param pathCode 形如"e 连接 i 连接 j 圆滑 a ..."
 */
void MyPainter::parseCode(MyRule *rule, QString pathCode)
{
    pathCode = pathCode.remove(' ');
    QPointF point;
    QList<QPointF> points;
    QString type = connectType(pathCode);
    int id = 0;
    // 取出第一个点，加入points队列末尾
    if(type == "连接")
        id = pathCode.indexOf("连");
    else
        id = pathCode.indexOf("圆");
    if(!addPointByRule(&points, rule, pathCode.left(id)))
        return;
    // 将点名从字符串中删除
    pathCode.remove(0,id);
    // 循环解析后面的连接
    while(pathCode.length()>0){
        // case: 一直连接
        // todo
    }
}

/**
 * @brief 根据经过的一系列点，画曲线
 * @param points
 * @param firstCtrlPoint
 * @param lastCtrlPoint
 */
void MyPainter::curve(QList<QPointF> points, QPointF firstCtrlPoint, QPointF lastCtrlPoint)
{
    int num=points.size(), i=3;
    QList<QPointF> *ctrlPoints = new QList<QPointF>;
    ctrlPoints->append(getSymmetryPoint(firstCtrlPoint,points.at(0)));
    ctrlPoints->append(getSymmetryPoint(lastCtrlPoint,points.at(num-1)));
    if(num>0){
        if(!currentPositionequal(*myPath,points.at(0)))
            myPath->moveTo(points.at(0));
        while(i<=num){
            addCtrlPoints(points.at(0),points.at(1),points.at(2),ctrlPoints);
            myPath->cubicTo(ctrlPoints->takeAt(0),ctrlPoints->takeAt(1),points.at(1)); //绘制到第2个端点的三次贝塞尔曲线
            points.takeAt(0); //删除第1个端点
            i++;
        }
        myPath->cubicTo(ctrlPoints->takeAt(0),ctrlPoints->takeAt(1),points.at(1));
    }
    delete ctrlPoints;
}

/**
 * @brief 下一个连接类型是“连接”还是“圆滑”
 * @param code
 * @return 返回""表示找不到新的连接了
 */
QString MyPainter::connectType(QString code)
{
    int idL = code.indexOf("连接"),
        idC = code.indexOf("圆滑");
    if(idL != -1 && idL < idC)
        return "连接";
    // 下一个连接类型是曲线
    else if(idC != -1)
        return "圆滑";
    else
        return "";
}

/**
 * @brief 用myPath画折线，并添加数据到myPathData数组
 * @param points 有序的折线顶点列表
 */
void MyPainter::brokenLineThrough(QList<QPointF> points)
{
    if(points.size()<2)
        return;
    QPointF point;
    point = points.takeAt(0);
    qreal dx=point.x()-myPath->currentPosition().x(),
            dy=point.y()-myPath->currentPosition().y();
    if(dx>0.1 || dx<-0.1 || dy<-0.1 ||dy>0.1)
        myPath->moveTo(point);
    myPath->lineTo(points.at(0));
    parent->myPathData->addLine(point,points.takeAt(0));
    while(!points.isEmpty())
    {
        point = points.takeAt(0);
        myPath->lineTo(point);
        parent->myPathData->addLineTo(point);
    }
}

/**
 * @brief 根据路径画折线，并添加数据到myPathData数组
 * @param brokenLine 折线(务必确认不含曲线）
 */
void MyPainter::brokenLineThrough(QPainterPath brokenLine)
{
    QList<QPointF> points;
    QPainterPath::Element e;
    int len = brokenLine.elementCount();
    if(len<2) return;
    for(int i=0;i<len;i++){
        e = brokenLine.elementAt(i);
        if(e.type>2) return;
        points<<QPointF(e.x,e.y);
    }
    brokenLineThrough(points);
}

/**
 * @brief 画曲线并添加数据（有首尾控制点）
 * @param points 曲线经过点的列表
 * @param firstCtrlPoint
 * @param lastCtrlPoint
 */
void MyPainter::curveThrough(QList<QPointF> points,QPointF firstCtrlPoint,QPointF lastCtrlPoint)
{
    curve(points,firstCtrlPoint,lastCtrlPoint);
    MyPainter path(parent);
    path.curve(points,firstCtrlPoint,lastCtrlPoint);
    parent->myPathData->addCurve(points,firstCtrlPoint,lastCtrlPoint,*(path.myPath));
}

/**
 * @brief 画曲线并添加数据（无控制点）
 * @param points 曲线经过点的列表
 */
void MyPainter::curveThrough(QList<QPointF> points)
{
    if(!points.isEmpty())
        curveThrough(points,points.first(),points.last());
    else
        return;
}

/**
 * @brief 点point关于点center的对称点
 * @param point
 * @param center
 * @return QPointF
 */
QPointF MyPainter::getSymmetryPoint(QPointF point, QPointF center)
{
    qreal px=point.x(), py=point.y(),
            cx=center.x(), cy=center.y(),
            x=2*cx-px, y=2*cy-py;
    return QPointF(x,y);
}

/**
 * @brief path的currentPosition是否和p的位置相符
 * @param path
 * @param p
 * @return
 */
bool MyPainter::currentPositionequal(QPainterPath path, QPointF p)
{
    QPointF pp = path.currentPosition();
    return equal(pp,p);
}

/**
 * @brief MyPainter::addPointById
 * @param points
 * @param rule
 * @param namePoint
 * @return
 */
bool MyPainter::addPointByRule(QList<QPointF> *points, MyRule *rule, QString namePoint)
{
    bool ok = true;
    QPointF point = rule->point(namePoint, &ok);
    if(ok){
        points->append(point);
        return true;
    }
    else{
        rule->info("点"+namePoint+"无法解析");
        return false;
    }
}

qreal MyPainter::distanceBetween(QPointF p1, QPointF p2)
{
    qreal x1=p1.x(), y1=p1.y(), x2=p2.x(),y2=p2.y();
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

/**
 * @brief 线段上某等分点
 * @param p1 线段端点1
 * @param p2 线段端点2
 * @param proportion 点到端点1的距离占线段长度的比例
 * @return QPointF
 */
QPointF MyPainter::getPointOnLine(QPointF p1,QPointF p2,qreal proportion)
{
    qreal x1=p1.x(), y1=p1.y(), dx=x1-p2.x(), dy=y1-p2.y();
    qreal x=x1-dx*proportion, y=y1-dy*proportion;
    return QPointF(x,y);
}

/**
 * @brief 添加点的数据
 * @param point
 * @param name
 */
void MyPainter::addPointData(QPointF point, QString name)
{
    parent->myPathData->addPoint(point,name);
}

/**
 * @brief 2个实数是否相等
 * @param v1 实数1
 * @param v2 实数2
 * @return bool
 */
bool MyPainter::equal(qreal v1, qreal v2)
{
    qreal d = v1-v2;
    if(d>-minUnit && d<minUnit)
        return true;
    else
        return false;
}

/**
 * @brief 2个点是否相等
 * @param p1
 * @param p2
 * @return bool
 */
bool MyPainter::equal(QPointF p1,QPointF p2)
{
    if(equal(p1.x(),p2.x()) && equal(p1.y(),p2.y()))
        return true;
    else
        return false;
}

/**
 * @brief 向3点曲线中加入中间2个控制点
 * @param A
 * @param B
 * @param C
 * @param ctrlPoints
 */
void MyPainter::addCtrlPoints(QPointF A,QPointF B,QPointF C,QList<QPointF> *ctrlPoints)
{
    QPointF E((A.x()+B.x())/2.0,(A.y()+B.y())/2.0);
    QPointF F((B.x()+C.x())/2.0,(B.y()+C.y())/2.0);
    qreal d1 = distanceBetween(A,B),    d2 = distanceBetween(B,C),    scale = d1/(d1+d2);
    QPointF D = getPointOnLine(E,F,scale);
    qreal dx = B.x()-D.x(),    dy = B.y()-D.y();
    QPointF ctrl2(E.x()+dx,E.y()+dy);
    QPointF ctrl3(F.x()+dx,F.y()+dy);
    ctrlPoints->insert(1,ctrl2);
    ctrlPoints->insert(2,ctrl3);
}
