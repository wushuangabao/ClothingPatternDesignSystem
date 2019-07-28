#ifndef PATHBUTTON_H
#define PATHBUTTON_H

#include <QPushButton>

class PathButton : public QPushButton
{
    Q_OBJECT

public:
    explicit PathButton(const QString &text, QWidget *parent = Q_NULLPTR);

    int index = -1;

public slots:
    void beClicked();
};

#endif // PATHBUTTON_H
