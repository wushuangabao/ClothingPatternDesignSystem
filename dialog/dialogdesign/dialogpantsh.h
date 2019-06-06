#ifndef DIALOGPANTSH_H
#define DIALOGPANTSH_H

#include <QDialog>
#include <QList>

namespace Ui {
class DialogPantsH;
}

class QGraphicsScene;
class QPushButton;
class QString;

/**
 * @brief
 *
 */
class DialogPantsH : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPantsH(QWidget *parent = nullptr);
    ~DialogPantsH();

private slots:
    void on_pantsLength_clicked();
    void on_waistPostion_clicked();
    void on_waistHead_clicked();
    void on_pantsLoop_clicked();
    void on_door_clicked();
    void on_sang1_clicked();
    void on_pocket1_clicked();
    void on_sang2_clicked();
    void on_pocket2_clicked();
    void on_pantsFoot_clicked();

    void updateString();

    void pageDown();
    void pageUp();

    void on_pushButton21_clicked();
    void on_pushButton22_clicked();
    void on_pushButton23_clicked();
    void on_pushButton24_clicked();
    void on_pushButton25_clicked();
    void on_pushButton26_clicked();
    void on_pushButton27_clicked();
    void on_pushButton28_clicked();
    void on_pushButton31_clicked();
    void on_pushButton32_clicked();
    void on_pushButton33_clicked();
    void on_pushButton34_clicked();
    void on_pushButton41_clicked();
    void on_pushButton42_clicked();
    void on_pushButton43_clicked();
    void on_pushButton44_clicked();
    void on_pushButton45_clicked();
    void on_pushButton46_clicked();
    void on_pushButton47_clicked();
    void on_pushButton48_clicked();
    void on_pushButton49_clicked();
    void on_pushButton51_clicked();
    void on_pushButton52_clicked();
    void on_pushButton61_clicked();
    void on_pushButton62_clicked();
    void on_pushButton63_clicked();
    void on_pushButton64_clicked();
    void on_pushButton65_clicked();
    void on_pushButton71_clicked();
    void on_pushButton72_clicked();
    void on_pushButton73_clicked();
    void on_pushButton74_clicked();
    void on_pushButton75_clicked();
    void on_pushButton76_clicked();
    void on_pushButton77_clicked();
    void on_pushButton81_clicked();
    void on_pushButton82_clicked();
    void on_pushButton83_clicked();
    void on_pushButton84_clicked();
    void on_pushButton85_clicked();
    void on_pushButton91_clicked();
    void on_pushButton92_clicked();
    void on_pushButton93_clicked();
    void on_pushButton94_clicked();
    void on_pushButton1001_clicked();
    void on_pushButton1002_clicked();
    void on_pushButton1003_clicked();
    void on_pushButton1004_clicked();
    void on_pushButton1005_clicked();
    void on_pushButton1006_clicked();
    void on_pushButton1007_clicked();
    void on_pushButton1008_clicked();
    void on_pushButton1009_clicked();
    void on_pushButton1010_clicked();
    void on_pushButton1011_clicked();
    void on_pushButton1012_clicked();
    void on_pushButton1013_clicked();
    void on_pushButton1014_clicked();
    void on_pushButton1015_clicked();
    void on_pushButton111_clicked();
    void on_pushButton112_clicked();
    void on_pushButton113_clicked();

    void on_buttonNext_clicked();

private:
    Ui::DialogPantsH *ui;
    QGraphicsScene *scene; /**< TODO: describe */
    QList<QPushButton*> pushButtons; /**< TODO: describe */
    QList<QString> stringList; /**< TODO: describe */

    void setLabels(QWidget* w);

    void selectPushButton(QList<QPushButton*> buttonList,QString str);
    void changePage(int id);

    void changePantsL(QString str);
    void changeWaistPostion(QString str);
    void changeWaistHead(QString str);
    void changePantsLoop(QString str);
    void changeDoor(QString str);
    void changeSang1(QString str);
    void changePocket1(QString str);
    void changeSang2(QString str);
    void changePocket2(QString str);
    void changePantsFoot(QString str);

    void keyPressEvent(QKeyEvent *k);
};

#endif // DIALOGPANTSH_H
