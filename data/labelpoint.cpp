#include "labelpoint.h"
#include "curvepoint.h"

LabelPoint::LabelPoint(QWidget *parent) : QLabel(parent)
{
    id = -1;
    point = nullptr;
}

QPoint LabelPoint::setHandlerPos(QPoint mousePos)
{
    int x = mousePos.x()-this->pos().x(),
        y = mousePos.y()-this->pos().y();
    handlerPos.setX(x);
    handlerPos.setY(y);
    return handlerPos;
}

void LabelPoint::moveTo(QPoint mousePos)
{
    this->move(mousePos.x()-handlerPos.x(),mousePos.y()-handlerPos.y());
}
