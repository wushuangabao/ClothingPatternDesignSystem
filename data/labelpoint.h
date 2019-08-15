#ifndef LABELPOINT_H
#define LABELPOINT_H

#include <QLabel>
#include <QPoint>

class PainterArea;
class CurvePoint;
class QAction;
class QMenu;

/**
 * @brief
 *
 */
class LabelPoint : public QLabel
{
    Q_OBJECT
public:
    explicit LabelPoint(PainterArea *parent = nullptr);
    ~LabelPoint();

    PainterArea *parent; /**< 绘图区 */
    CurvePoint *point;   /**< 对应的点 */
    QPoint handlerPos;   /**< 拖动时鼠标的位置（在自身坐标系中） */

    QPoint setHandlerPos(QPoint mousePos);  // 根据鼠标在painterArea中的的物理位置计算handlerPos
    void moveTo(QPoint mousePos);
    void contextMenuEvent(QContextMenuEvent *event);
    void changePos();
    void setOldPoint(QPointF p);

public slots:
    void resetPos();
private:
    QAction *actionNew;
    QMenu *contextMenu;
    qreal oldX;
    qreal oldY;
};

#endif // LABELPOINT_H
