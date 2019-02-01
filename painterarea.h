#ifndef PAINTERAREA_H
#define PAINTERAREA_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QLabel>
#include <QPoint>
#include <QPushButton>
#include <QString>
#include <QPixmap>

class MyPathData;
class MyPath;
class CurvePoint;
class LabelPoint;

class PainterArea : public QWidget
{
    Q_OBJECT

private:
    bool isMoving=false;
    int xBeforeMoving;
    int yBeforeMoving;
    int xLogical(int xPhysical);
    int yLogical(int yPhysical);
    int xPhysical(int xLogical);
    int yPhysical(int yLogical);

    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    QPointF getIntersection(qreal nA1,qreal x1,qreal y1,qreal nA2,qreal x2,qreal y2,qreal scale);

public:
    explicit PainterArea(QWidget *parent = nullptr);
    ~PainterArea();

    MyPathData *myPathData;
    MyPath *myPath;
    QPainterPath auxiliaryLines;
    QPainterPath yellowPath;
    int typePants;
    int old_typePants;

    int pantsHeight;
    int pantsL;
    int pantsW;
    int pantsH;
    int pantsCrotchH;
    int typeSang1;
    int typeSang2;

    QString stringTempStatus; //显示在状态栏的临时字符串
    qreal scalingMulti=1.0; //缩放倍数，数值越大图形越大
    int intUp=0; //原点向上偏移值
    int intLeft=0; //原点向左偏移值

    void setMyPath();

    //鼠标拖动点:
    LabelPoint *selectedLabelPoint;
    QList<LabelPoint*> labelPoints;
    void setLabelPoint(int id,CurvePoint *point);
    void clearLabelPoints();
    void updateLabelPoints();

    bool writeDXF();

    void setCenterToYellowPath();

public slots:
    void setTypeSang(int frontOrBack,int intCase);
    void changePath();

signals:
    void mouseCoordinateChanged();
    void scalingMultiChanged();
    void resetModel();
};

#endif // PAINTERAREA_H
