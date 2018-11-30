#include "splash.h"
#include "ui_splash.h"
#include "mainwindow.h"
#include <QLabel>
#include <QTimer>

Splash::Splash(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Splash)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags()); //设置成无边框
    //this->setContentsMargins(0.1,0.1,0.1,0.1); //设置边距
}

Splash::~Splash()
{
    delete ui;
    if(timer!=nullptr)
        delete timer;
}

void Splash::waitFor(MainWindow *w,int int_ms)
{
    mainWin = w;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showMainWindow()));
    timer->start(int_ms);
}

void Splash::showMainWindow()
{
    if(mainWin!=nullptr)
        mainWin->show();
    this->close();
}
