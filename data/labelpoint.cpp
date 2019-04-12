#include "labelpoint.h"
#include "curvepoint.h"
#include "../painterarea.h"
#include "../mainwindow.h"
#include "../mypath.h"
#include "mypathdata.h"
#include <QMenu>
#include <QDebug>

/**
 * @brief
 *
 * @param parent
 */
LabelPoint::LabelPoint(PainterArea *parent) : QLabel(parent)
{
    this->parent=parent;
    point = nullptr;
    actionNew=new QAction("移动点到当前位置",this);
    contextMenu=new QMenu(this);
    contextMenu->addAction(actionNew);
    connect(actionNew,SIGNAL(triggered()),this,SLOT(changePos()));
}

/**
 * @brief
 *
 */
LabelPoint::~LabelPoint()
{
    delete actionNew;
    delete contextMenu;
}

/**
 * @brief
 *
 * @param mousePos
 * @return QPoint
 */
QPoint LabelPoint::setHandlerPos(QPoint mousePos)
{
    int x = mousePos.x()-this->pos().x(),
        y = mousePos.y()-this->pos().y();
    handlerPos.setX(x);
    handlerPos.setY(y);
    return handlerPos;
}

/**
 * @brief
 *
 * @param mousePos
 */
void LabelPoint::moveTo(QPoint mousePos)
{
    this->move(mousePos.x()-handlerPos.x(),mousePos.y()-handlerPos.y());
}

/**
 * @brief
 *
 * @param event
 */
void LabelPoint::contextMenuEvent(QContextMenuEvent *event)
{
    contextMenu->move(cursor().pos());
    contextMenu->show();
}

/**
 * @brief
 *
 */
void LabelPoint::changePos()
{
    if(point)
    {
        MyPathData *pathData=parent->myPathData;
        //修改x,y的值
        pathData->pointData[point->id].setX(parent->xLogical(this->x()));
        pathData->pointData[point->id].setY(parent->yLogical(this->y()));
        //重绘图形
        parent->dataChanged=true;
        parent->update();
        //重设表格
        MainWindow *mainWin=(MainWindow*)parent->parent();
        mainWin->resetModel();
    }
}
