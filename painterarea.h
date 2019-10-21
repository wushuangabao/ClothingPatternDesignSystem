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

    bool isCurvePath(QPainterPath* path);
    void drawPathNames(QList<QPointF> posList, QList<QString> names);

public:
    explicit PainterArea(QWidget *parent = nullptr);
    ~PainterArea();

    QList<MyPathData*> myPaths; /**< 纸样数据 */
    QList<QPushButton*> btnPaths; /**< 纸样数据对应的按钮 */
    QList<QLabel*> pathNames; /**< 纸样名称（标签）数组 */
    QPainterPath auxiliaryLines; /**< 辅助线 */
    QPainterPath yellowPath; /**< 黄线 */
    QPainterPath greenPath; /**< 绿线 */
    int currentId; /**< 当前正在操作的纸样数据的索引 */

    void addPath(MyPathData* path);
    MyPathData* currentPath();

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

    bool writeDXF(QString fileName);

    void setCenterToYellowPath();
    void setColor(QString color = "");

public slots:
    void setTypeSang(int frontOrBack,int intCase);
    bool setCurrentPath(int i = -1);

signals:
    void mouseCoordinateChanged();
    void scalingMultiChanged();
    void resetModel(int i);
};

#endif // PAINTERAREA_H
