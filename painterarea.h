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
    bool isMoving=false; /**< TODO: describe */
    int xBeforeMoving; /**< TODO: describe */
    int yBeforeMoving; /**< TODO: describe */

    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

public:
    explicit PainterArea(QWidget *parent = nullptr);
    ~PainterArea();

    MyPathData *myPathData; /**< TODO: describe */
    MyPath *myPath; /**< TODO: describe */
    QPainterPath auxiliaryLines; /**< TODO: describe */
    QPainterPath yellowPath; /**< TODO: describe */
    QPainterPath greenPath; /**< TODO: describe */
    int typePants; /**< TODO: describe */
    int old_typePants; /**< TODO: describe */

    int pantsHeight; /**< TODO: describe */
    int pantsL; /**< TODO: describe */
    int pantsW; /**< TODO: describe */
    int pantsH; /**< TODO: describe */
    int pantsCrotchH; /**< TODO: describe */
    int typeSang1; /**< TODO: describe */
    int typeSang2; /**< TODO: describe */

    QString stringTempStatus; /**< 显示在状态栏的临时字符串 */
    qreal scalingMulti=1.0; /**< 缩放倍数，数值越大图形越大 */
    int intUp=0; /**< 原点向上偏移值 */
    int intLeft=0; /**< 原点向左偏移值 */

//    bool dataChanged=false; /**< TODO: describe */
    void setMyPath();

    //鼠标拖动点:
    LabelPoint *selectedLabelPoint; /**< TODO: describe */
    QList<LabelPoint*> labelPoints; /**< TODO: describe */
    void setLabelPoint(CurvePoint *point);
    void clearLabelPoints();
    void updateLabelPoints();

    int xLogical(int xPhysical);
    int yLogical(int yPhysical);
    int xPhysical(int xLogical);
    int yPhysical(int yLogical);
    int xLogical(qreal xPhysical);
    int yLogical(qreal yPhysical);
    int xPhysical(qreal xLogical);
    int yPhysical(qreal yLogical);

    bool writeDXF();

    void setCenterToYellowPath();

public slots:
    void setTypeSang(int frontOrBack,int intCase);

signals:
    void mouseCoordinateChanged();
    void scalingMultiChanged();
    void resetModel();
};

#endif // PAINTERAREA_H
