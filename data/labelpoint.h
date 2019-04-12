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
    /**
     * @brief
     *
     * @param parent
     */
    explicit LabelPoint(PainterArea *parent = nullptr);
    /**
     * @brief
     *
     */
    ~LabelPoint();
    PainterArea *parent; /**< TODO: describe */
    CurvePoint *point; /**< TODO: describe */
    QPoint handlerPos; //拖动时鼠标的位置（在自身坐标系中） /**< TODO: describe */
    /**
     * @brief
     *
     * @param mousePos
     * @return QPoint
     */
    QPoint setHandlerPos(QPoint mousePos); //根据鼠标在painterArea中的的物理位置计算handlerPos
    /**
     * @brief
     *
     * @param mousePos
     */
    void moveTo(QPoint mousePos);
    /**
     * @brief
     *
     * @param event
     */
    void contextMenuEvent(QContextMenuEvent *event);

signals:

public slots:
    /**
     * @brief
     *
     */
    void changePos();
private:
    QAction *actionNew; /**< TODO: describe */
    QMenu *contextMenu; /**< TODO: describe */

};

#endif // LABELPOINT_H
