#include "mycomponent.h"

MyComponent::MyComponent(MyComponent* parent)
{
    this->parent=parent;
}

MyComponent::~MyComponent()
{}
