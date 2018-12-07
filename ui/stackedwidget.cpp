#include "stackedwidget.h"
#include <QPainter>


StackedWidget::StackedWidget(QWidget *parent) : QStackedWidget(parent)
{
    path = new QPainterPath;
}

void StackedWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    if(!path->isEmpty())
        painter.drawPath(*path);
}
