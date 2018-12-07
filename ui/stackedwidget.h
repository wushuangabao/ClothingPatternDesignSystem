#ifndef STACKEDWIDGET_H
#define STACKEDWIDGET_H


//该类已弃用

#include <QStackedWidget>

class QPainterPath;

class StackedWidget : public QStackedWidget
{
public:
    StackedWidget(QWidget *parent = nullptr);

    QPainterPath *path;

private:
    void paintEvent(QPaintEvent *event);
};

#endif // STACKEDWIDGET_H
