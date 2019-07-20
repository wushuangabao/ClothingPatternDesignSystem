#include "dialogtestcurve.h"
#include "ui_dialogtestcurve.h"
#include "rules/mypainter.h"

#include <QtWidgets>

DialogTestCurve::DialogTestCurve(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTestCurve)
{
    // 设置背景为黑色
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, Qt::black);
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    setStyleSheet("QLabel{background-color:white;width:5px;}");

    labels<<new QLabel(this)<<new QLabel(this)<<new QLabel(this)<<new QLabel(this);
    labels[0]->move(0,100);
    labels[1]->move(100,0);
    labels[2]->move(200,100);
    labels[3]->move(100,200);

    myPath = new MyPainter((PainterArea*)this);
    drawCurve();

    ui->setupUi(this);
}

DialogTestCurve::~DialogTestCurve()
{
    delete ui;
    delete myPath;
    int len = labels.size();
    if(len>0)
        for(int i=0;i<len;i++)
            delete labels[i];
}

void DialogTestCurve::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QPen pen(Qt::white);
    painter.setPen(pen);
    painter.drawPath(*(myPath->myPath));
}

/**
 * @brief 鼠标按压事件
 *
 * @param event
 */
void DialogTestCurve::mousePressEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();
    QWidget *w=this->childAt(pos);
    if(w!=nullptr && w->inherits("QLabel"))
        selectedLabel=reinterpret_cast<QLabel *>(w);
    else selectedLabel=nullptr;
}
/**
 * @brief 鼠标释放事件
 *
 * @param event
 */
void DialogTestCurve::mouseReleaseEvent(QMouseEvent *event)
{
    selectedLabel=nullptr;
    drawCurve();
}
/**
 * @brief 鼠标移动事件
 *
 * @param event
 */
void DialogTestCurve::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos=event->pos();
    if(selectedLabel!=nullptr)
    {
        selectedLabel->move(pos.x(),pos.y());
        update();
    }
}

void DialogTestCurve::drawCurve()
{
    int len=labels.size();
    QList<QPointF> points;
    for(int i=0;i<len;i++){
        points<<labels[i]->pos();
    }
    myPath->curveThroughData(points,points.first(),points.last());
    update();
}

/**
 * @brief 清空以前的路径
 */
void DialogTestCurve::on_pushButton_clicked()
{
    delete myPath->myPath;
    myPath->myPath = new QPainterPath;
    drawCurve();
}

/**
 * @brief 添加曲线途经点
 */
void DialogTestCurve::on_pushButton_2_clicked()
{
    labels<<new QLabel(this);
    int i = labels.size()-1;
    labels[i]->move(10,10);
    labels[i]->show();
    on_pushButton_clicked();
}
