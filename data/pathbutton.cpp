#include "pathbutton.h"
#include "../mainwindow.h"
#include "../painterarea.h"

PathButton::PathButton(const QString &text, QWidget *parent): QPushButton(text, parent)
{}

/**
 * @brief 被点击：选择对应的路径
 */
void PathButton::beClicked()
{
    MainWindow* p = static_cast<MainWindow*>(parent()->parent());
    PainterArea* pa = static_cast<PainterArea*>(p->painterArea);
    pa->setCurrentPath(index);
}
