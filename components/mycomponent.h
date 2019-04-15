#ifndef MYCOMPONENT_H
#define MYCOMPONENT_H

#include <QList>

class MyPath;

/**
 * @brief 组件模板类
 */
class MyComponent
{
public:
    MyComponent(MyComponent* parent);
    ~MyComponent();

    MyComponent* parent;
    QList<MyComponent*> children; /**< 子组件列表 */

};

#endif // MYCOMPONENT_H
