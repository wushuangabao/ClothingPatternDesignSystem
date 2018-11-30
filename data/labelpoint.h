#ifndef LABELPOINT_H
#define LABELPOINT_H

#include <QLabel>
#include <QPoint>

class CurvePoint;

class LabelPoint : public QLabel
{
    Q_OBJECT
public:
    explicit LabelPoint(QWidget *parent = nullptr);
    int id;
    CurvePoint *point;
    QPoint handlerPos; //拖动时鼠标的位置（在自身坐标系中）
    QPoint setHandlerPos(QPoint mousePos); //根据鼠标在painterArea中的的物理位置计算handlerPos
    void moveTo(QPoint mousePos);

signals:

public slots:
};

#endif // LABELPOINT_H
