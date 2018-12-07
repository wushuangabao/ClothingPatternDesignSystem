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
    int startPoint; //起点在pointData数组中的id
    int endPoint; //终点在pointData数组中的id
    QPainterPath *path;
};

struct PointData{
    int id;
    CurvePoint *point;
};

class MyPathData
{
public:
    MyPathData(QString name);
    ~MyPathData();

    QString name="";
    QFile file;

    const static int MAX_NUM_POINT=500;
    const static int MAX_NUM_PATH=250;
    int numberPath=0;
    int numberPoint=0;
    PathData pathData[MAX_NUM_PATH];
    PointData pointData[MAX_NUM_POINT];
    QMap<QString,QPointF> pointMap; //记录点的名称与id，用于查找特殊点，如smallCroPoint

    void addLineTo(QPointF endPoint);
    void addLine(QPointF startPoint,QPointF endPoint);
    void addCurve(QList<QPointF> points,QPointF ctrlPoint1,QPointF ctrlPoint2,QPainterPath path);

    void addPoint(QPointF point,QString name="");
    bool pointMapHas(QPointF point);
    QPointF findPoint(QString name);
    QString findName(QPointF point);
    QString findName(qreal x,qreal y);
    QString stringOf(CurvePoint *point);
    QString stringsOf(CurvePoint *point);
    CurvePoint pointOf(QString str);
    CurvePoint pointsOf(QString str);

    bool saveTo(QString path);
    static bool equal(QPointF p1,CurvePoint *p2,qreal E); //判断两点是否相等（对应坐标相差不超过变量(E)mm）
    void clear(); //清空所有数据

private:
    const qreal E=0.1;

    bool equal(CurvePoint *p1,CurvePoint *p2); //判断两点是否相等（对应坐标相差不超过常量(E)mm）
    bool equal(CurvePoint p1,CurvePoint p2);
};

#endif // MYPATHDATA_H
