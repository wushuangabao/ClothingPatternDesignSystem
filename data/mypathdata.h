#ifndef MYPATHDATA_H
#define MYPATHDATA_H

#include <QPointF>
#include <QString>

struct PathData{
    int index;
    bool isLine;
    int startPoint;
    int endPoint;
    QPointF ctrlPoint1;
    QPointF ctrlPoint2;
};

struct PointData{
    int index;
    qreal x;
    qreal y;
};

class MyPathData
{
public:
    MyPathData();

    QString name;

    const static int MAX_NUM=999;
    int number=0; //数据量
    PathData pathData[MAX_NUM];
    PointData pointData[MAX_NUM];
};

#endif // MYPATHDATA_H
