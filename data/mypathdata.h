#ifndef MYPATHDATA_H
#define MYPATHDATA_H

#include <QPointF>
#include <QString>
#include <QTextStream>
#include <QList>
#include <QMap>
#include "curvepoint.h"

class QPainterPath;
class QFile;
class QPainterPath;
class PainterArea;

enum astm {Boundary, InLines, WarpLine, Auxiliary}; // 轮廓线：棕色，内部线：白色，经线：红色，辅助线：青色

struct PathData{
    int id; /**< 路径在数组中的序号 */
    bool isLine; /**< 是否为直线 */
    astm astmTag;
    CurvePoint* startPoint; /**< 起点指针 */
    CurvePoint* endPoint; /**< 终点指针 */
};

class MyPathData
{
public:
    MyPathData(QString name);
    MyPathData(const MyPathData &copyObj);
    ~MyPathData();

    QString name=""; /**< MyPathData实例的名称 */
    QPointF basePoint; /**< 基准点 */
    QString params; /**< 参数字符串 */
    int numberPath=0; /**< 路径数目 */
    int numberPoint=0; /**< 点的数目 */
    QList<PathData> pathData; /**< 路径数组 */
    QList<QPointF> pointData; /**< 点的数组 */
    QMap<QString,int> pointMap; /**< 记录点的名称 */
    int astmTag = 0; // 记录当前添加路径的 astm 标志

    bool addLineTo(QPointF endPoint,int idStartPoint=-1);
    void addLine(QPointF startPoint,QPointF endPoint);
    void addCurve(QList<QPointF> points,QPointF ctrlPoint1,QPointF ctrlPoint2);
    int addPoint(QPointF point,QString name=""); //将point加入pointData数组（若已存在就不会添加），返回在数组中的索引
    int addCtrlPoint(QPointF ctrlPoint); //将ctrlPoint加入pointData数组（不管是否已存在同位置的点），返回在数组中的索引
    int findPoint(QPointF point,bool addIfNotFind=false); //在pointData数组中寻找point，返回它的索引。
    bool pointMapHas(QPointF point);
    QPointF findPoint(QString name);
    QString findName(int id);
    QString stringOf(CurvePoint *point);
    QString stringsOf(CurvePoint *point);

    void setName(QString name);
    void setNotch(int pointId);
    void setInLine(int pathId);
    void setWarpLine(int pathId);
    void moveBasePointTo(QPointF p);
    int warpLineId();
    bool saveTxtTo(QString path);
    bool writeASTM(QString filePath);
    void writeASTMBlock(QFile* file, QString infoSize = "M");
    void clear();

    static QString stringReal(qreal n);
    static QString stringPoint(QPointF p);
    static QString stringPoint(qreal x,qreal y);
    static void writeText(QFile* file,QString str,qreal x=0,qreal y=0,int layer=1);

private:
    const qreal E=0.1;
    int idCurrentPoint = 0; //记录当前点在pointData数组中的索引
    int idWrapLine = -1;
    QList<int> notches;
    int findPathBySP(int idSP);
    bool equal(QPointF p1,QPointF p2); //判断两点是否相等（对应坐标相差不超过常量(E)mm）
    void writePolyL(QFile* file,PathData path,int layer=1);
    void writePolyL(QFile* file,const QPainterPath& path,int layer);
    void writePolyLHead(QFile* file,int layer=1);
    void writeBoundary(QFile* file);
    void writeNotches(QFile* file);
    void writeInLines(QFile* file);
};

#endif // MYPATHDATA_H
