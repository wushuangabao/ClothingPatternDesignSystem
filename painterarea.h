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

/**
 * @brief
 *
 */
class PainterArea : public QWidget
{
    Q_OBJECT

private:
    bool isMoving=false; /**< TODO: describe */
    int xBeforeMoving; /**< TODO: describe */
    int yBeforeMoving; /**< TODO: describe */

    /**
     * @brief
     *
     * @param event
     */
    void paintEvent(QPaintEvent *event);
    /**
     * @brief
     *
     * @param event
     */
    void mousePressEvent(QMouseEvent *event);
    /**
     * @brief
     *
     * @param event
     */
    void mouseMoveEvent(QMouseEvent *event);
    /**
     * @brief
     *
     * @param event
     */
    void mouseReleaseEvent(QMouseEvent *event);
    /**
     * @brief
     *
     * @param event
     */
    void wheelEvent(QWheelEvent *event);

    /**
     * @brief
     *
     * @param nA1
     * @param x1
     * @param y1
     * @param nA2
     * @param x2
     * @param y2
     * @param scale
     * @return QPointF
     */
    QPointF getIntersection(qreal nA1,qreal x1,qreal y1,qreal nA2,qreal x2,qreal y2,qreal scale);

public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit PainterArea(QWidget *parent = nullptr);
    /**
     * @brief
     *
     */
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

    QString stringTempStatus; //显示在状态栏的临时字符串 /**< TODO: describe */
    qreal scalingMulti=1.0; //缩放倍数，数值越大图形越大 /**< TODO: describe */
    int intUp=0; //原点向上偏移值 /**< TODO: describe */
    int intLeft=0; //原点向左偏移值 /**< TODO: describe */

    bool dataChanged=false; /**< TODO: describe */
    /**
     * @brief
     *
     */
    void setMyPath();

    //鼠标拖动点:
    LabelPoint *selectedLabelPoint; /**< TODO: describe */
    QList<LabelPoint*> labelPoints; /**< TODO: describe */
    /**
     * @brief
     *
     * @param point
     */
    void setLabelPoint(CurvePoint *point);
    /**
     * @brief
     *
     */
    void clearLabelPoints();
    /**
     * @brief
     *
     */
    void updateLabelPoints();

    /**
     * @brief
     *
     * @param xPhysical
     * @return int
     */
    int xLogical(int xPhysical);
    /**
     * @brief
     *
     * @param yPhysical
     * @return int
     */
    int yLogical(int yPhysical);
    /**
     * @brief
     *
     * @param xLogical
     * @return int
     */
    int xPhysical(int xLogical);
    /**
     * @brief
     *
     * @param yLogical
     * @return int
     */
    int yPhysical(int yLogical);

    /**
     * @brief
     *
     * @return bool
     */
    bool writeDXF();

    /**
     * @brief
     *
     */
    void setCenterToYellowPath();

public slots:
    /**
     * @brief
     *
     * @param frontOrBack
     * @param intCase
     */
    void setTypeSang(int frontOrBack,int intCase);
    /**
     * @brief
     *
     */
    void changePath();

signals:
    /**
     * @brief
     *
     */
    void mouseCoordinateChanged();
    /**
     * @brief
     *
     */
    void scalingMultiChanged();
    /**
     * @brief
     *
     */
    void resetModel();
};

#endif // PAINTERAREA_H
