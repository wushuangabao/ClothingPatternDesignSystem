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

class PainterArea : public QWidget
{
    Q_OBJECT

private:
    bool isMoving=false;
    int xBeforeMoving;
    int yBeforeMoving;

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
    int typePants;
    int old_typePants;

    int pantsHeight;
    int pantsL;
    int pantsW;
    int pantsH;
    int pantsCrotchH;
    int typeSang1;
    int typeSang2;

    QString mouseCoordinate; //鼠标坐标，与painter坐标系不同
    qreal scalingMulti=1.0; //缩放倍数，数值越大图形越大
    int intUp=0; //原点向上偏移值
    int intLeft=0; //原点向左偏移值

    QPoint startPoint;

    //鼠标拖动点:
    QLabel *startLabel;
    QLabel *ctrlLabel1;
    QLabel *ctrlLabel2;
    QLabel *ctrlLabel3;
    QLabel *ctrlLabel4;
    QWidget *selectedWidget;

public slots:
    void setTypeSang(int frontOrBack,int intCase);

signals:
    void mouseCoordinateChanged();
    void scalingMultiChanged();
    void resetModel();
};

#endif // PAINTERAREA_H
