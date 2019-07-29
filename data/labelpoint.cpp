#include "labelpoint.h"
#include "curvepoint.h"
#include "../painterarea.h"
#include "../mainwindow.h"
#include "../rules/mypath.h"
#include "mypathdata.h"
#include <QMenu>
#include <QDebug>

LabelPoint::LabelPoint(PainterArea *parent) : QLabel(parent)
{
    this->parent=parent;
    point = nullptr;
    actionNew=new QAction("恢复到黄线位置",this);
    contextMenu=new QMenu(this);
    contextMenu->addAction(actionNew);
    connect(actionNew,SIGNAL(triggered()),this,SLOT(resetPos()));
}

LabelPoint::~LabelPoint()
{
    delete actionNew;
    delete contextMenu;
}

/**
 * @brief 设置标签抓手的位置
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
 * @brief 移动标签
 *
 * @param mousePos
 */
void LabelPoint::moveTo(QPoint mousePos)
{
    this->move(mousePos.x()-handlerPos.x(),mousePos.y()-handlerPos.y());
}

/**
 * @brief 右键菜单
 *
 * @param event
 */
void LabelPoint::contextMenuEvent(QContextMenuEvent *event)
{
    contextMenu->move(cursor().pos());
    contextMenu->show();
}

/**
 * @brief 改变点的位置
 *
 */
void LabelPoint::changePos()
{
    if(point != nullptr){
        MyPathData *pathData = parent->currentPath();
        // 修改x,y的值
        pathData->pointData[point->id].setX(parent->xLogical(this->x()));
        pathData->pointData[point->id].setY(parent->yLogical(this->y()));
        // 重绘图形
        // parent->dataChanged=true;
        parent->update();
        // 重设表格
        MainWindow *mainWin=static_cast<MainWindow*>(parent->parent());
        mainWin->resetModel(parent->currentId);
    }
}

/**
 * @brief 设置旧的点坐标
 * @param p
 */
void LabelPoint::setOldPoint(QPointF p)
{
    oldX = p.x();
    oldY = p.y();
}

/**
 * @brief 重置为旧的点坐标
 */
void LabelPoint::resetPos()
{
    MyPathData *pathData = parent->currentPath();
    pathData->pointData[point->id].setX(oldX);
    pathData->pointData[point->id].setY(oldY);
    this->moveTo(QPoint(parent->xPhysical(oldX),parent->yPhysical(oldY)));
    parent->greenPath = QPainterPath();
    parent->update();
}
