#ifndef SPLASH_H
#define SPLASH_H

#include <QWidget>

class MainWindow;
class QTimer;

namespace Ui {
class Splash;
}

class Splash : public QWidget
{
    Q_OBJECT

public:
    explicit Splash(QWidget *parent = 0);
    ~Splash();


    void waitFor(MainWindow *mainWin,int int_ms);

private:
    Ui::Splash *ui;

    MainWindow *mainWin=nullptr;
    QTimer *timer;

public slots:
    void showMainWindow();
};

#endif // SPLASH_H
