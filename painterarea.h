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
class CurvePoint;
class LabelPoint;

class PainterArea : public QWidget
{
    Q_OBJECT

private:
    bool isMoving=false;
    int xBeforeMoving;
    int yBeforeMoving;
    QColor color;

    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

public:
    explicit PainterArea(QWidget *parent = nullptr);
    ~PainterArea();

    QList<MyPathData*> myPaths ; /**< 纸样数据 */
    QPainterPath auxiliaryLines; /**< 辅助线 */
    QPainterPath yellowPath; /**< 黄线 */
    QPainterPath greenPath; /**< 绿线 */
    int typePants; /**< 裤子类型 */
    int old_typePants; /**< 原来的裤子类型 */

    int pantsHeight; /**< 身高 */
    int pantsL; /**< 裤长 */
    int pantsW; /**< 腰围 */
    int pantsH; /**< 臀围 */
    int pantsCrotchH; /**< 裆深 */
    int typeSang1; /**< 省褶类型 前片 */
    int typeSang2; /**< 省褶类型 后片 */

    QString stringTempStatus; /**< 显示在状态栏的临时字符串 */
    qreal scalingMulti=1.0; /**< 缩放倍数，数值越大图形越大 */
    int intUp=0; /**< 原点向上偏移值 */
    int intLeft=0; /**< 原点向左偏移值 */

    LabelPoint *selectedLabelPoint; /**< 选中的点标签 */
    QList<LabelPoint*> labelPoints; /**< 点标签列表 */
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
    void setColor(QString color = "");

public slots:
    void setTypeSang(int frontOrBack,int intCase);

signals:
    void mouseCoordinateChanged();
    void scalingMultiChanged();
    void resetModel();
};

#endif // PAINTERAREA_H
