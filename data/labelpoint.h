#ifndef LABELPOINT_H
#define LABELPOINT_H

#include <QLabel>
#include <QPoint>

class PainterArea;
class CurvePoint;
class QAction;
class QMenu;

class LabelPoint : public QLabel
{
    Q_OBJECT
public:
    explicit LabelPoint(PainterArea *parent = nullptr);
    ~LabelPoint();
    PainterArea *parent;
    CurvePoint *point;
    QPoint handlerPos; //拖动时鼠标的位置（在自身坐标系中）
    QPoint setHandlerPos(QPoint mousePos); //根据鼠标在painterArea中的的物理位置计算handlerPos
    void moveTo(QPoint mousePos);
    void contextMenuEvent(QContextMenuEvent *event);

signals:

public slots:
    void changePos();
private:
    QAction *actionNew;
    QMenu *contextMenu;

};

#endif // LABELPOINT_H
