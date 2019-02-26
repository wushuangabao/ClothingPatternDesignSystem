#ifndef MYPATHDATA_H
#define MYPATHDATA_H

#include <QPointF>
#include <QString>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QMap>
#include <QString>
#include "curvepoint.h"

class QPainterPath;

struct PathData{
    int id;
    bool isLine;
    CurvePoint* startPoint; //起点指针
    CurvePoint* endPoint;  //终点指针
    QPainterPath *path; //原始路径数据
};

class MyPathData
{
public:
    MyPathData(QString name);
    ~MyPathData();

    QString name="";
    QFile file;

    const static int MAX_NUM_POINT=250;
    const static int MAX_NUM_PATH=100;
    int numberPath=0;
    int numberPoint=0;
    PathData pathData[MAX_NUM_PATH];
    QPointF pointData[MAX_NUM_POINT];
    QMap<QString,int> pointMap; //记录点的名称，用于查找特殊点，如smallCroPoint

    bool addLineTo(QPointF endPoint,int idStartPoint=-1);
    void addLine(QPointF startPoint,QPointF endPoint);
    void addCurve(QList<QPointF> points,QPointF ctrlPoint1,QPointF ctrlPoint2,QPainterPath path);
    int addPoint(QPointF point,QString name=""); //将point加入pointData数组（若已存在就不会添加），返回在数组中的索引
    int addCtrlPoint(QPointF ctrlPoint); //将ctrlPoint加入pointData数组（不管是否已存在同位置的点），返回在数组中的索引
    int findPoint(QPointF point,bool addIfNotFind=false); //在pointData数组中寻找point，返回它的索引。addIfNotFind表示若没有找到，自动添加进数组。返回-1表示没找到也没添加。
    bool pointMapHas(QPointF point);
    QPointF findPoint(QString name);
//    QString findName(QPointF point);
//    QString findName(qreal x,qreal y);
    QString findName(int id);
    QString stringOf(CurvePoint *point);
    QString stringsOf(CurvePoint *point);
//    CurvePoint pointOf(QString str);
//    CurvePoint pointsOf(QString str);

    bool saveTo(QString path);
//    static bool equal(QPointF p1,CurvePoint *p2,qreal E); //判断两点是否相等（对应坐标相差不超过变量(E)mm）
    void clear(); //清空所有数据

private:
    const qreal E=0.1;
    int idCurrentPoint=0; //记录当前点在pointData数组中的索引
//    bool equal(CurvePoint *p1,CurvePoint *p2); //判断两点是否相等（对应坐标相差不超过常量(E)mm）
    bool equal(QPointF p1,QPointF p2);
};

#endif // MYPATHDATA_H
