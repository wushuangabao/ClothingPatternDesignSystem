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
    /**
     * @brief
     *
     * @param parent
     */
    explicit DialogPantsH(QWidget *parent = 0);
    /**
     * @brief
     *
     */
    ~DialogPantsH();

private slots:
    /**
     * @brief
     *
     */
    void on_pantsLength_clicked();
    /**
     * @brief
     *
     */
    void on_waistPostion_clicked();
    /**
     * @brief
     *
     */
    void on_waistHead_clicked();
    /**
     * @brief
     *
     */
    void on_pantsLoop_clicked();
    /**
     * @brief
     *
     */
    void on_door_clicked();
    /**
     * @brief
     *
     */
    void on_sang1_clicked();
    /**
     * @brief
     *
     */
    void on_pocket1_clicked();
    /**
     * @brief
     *
     */
    void on_sang2_clicked();
    /**
     * @brief
     *
     */
    void on_pocket2_clicked();
    /**
     * @brief
     *
     */
    void on_pantsFoot_clicked();

    /**
     * @brief
     *
     */
    void updateString();

    /**
     * @brief
     *
     */
    void pageDown();
    /**
     * @brief
     *
     */
    void pageUp();

    /**
     * @brief
     *
     */
    void on_pushButton21_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton22_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton23_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton24_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton25_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton26_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton27_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton28_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton31_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton32_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton33_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton34_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton41_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton42_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton43_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton44_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton45_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton46_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton47_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton48_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton49_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton51_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton52_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton61_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton62_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton63_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton64_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton65_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton71_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton72_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton73_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton74_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton75_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton76_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton77_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton81_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton82_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton83_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton84_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton85_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton91_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton92_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton93_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton94_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton1001_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton1002_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton1003_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton1004_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton1005_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton1006_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton1007_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton1008_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton1009_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton1010_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton1011_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton1012_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton1013_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton1014_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton1015_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton111_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton112_clicked();
    /**
     * @brief
     *
     */
    void on_pushButton113_clicked();

    /**
     * @brief
     *
     */
    void on_buttonNext_clicked();

private:
    Ui::DialogPantsH *ui; /**< TODO: describe */
    QGraphicsScene *scene; /**< TODO: describe */
    QList<QPushButton*> pushButtons; /**< TODO: describe */
    QList<QString> stringList; /**< TODO: describe */

    /**
     * @brief
     *
     * @param w
     */
    void setLabels(QWidget* w);

    /**
     * @brief
     *
     * @param buttonList
     * @param str
     */
    void selectPushButton(QList<QPushButton*> buttonList,QString str);
    /**
     * @brief
     *
     * @param id
     */
    void changePage(int id);

    /**
     * @brief
     *
     * @param str
     */
    void changePantsL(QString str);
    /**
     * @brief
     *
     * @param str
     */
    void changeWaistPostion(QString str);
    /**
     * @brief
     *
     * @param str
     */
    void changeWaistHead(QString str);
    /**
     * @brief
     *
     * @param str
     */
    void changePantsLoop(QString str);
    /**
     * @brief
     *
     * @param str
     */
    void changeDoor(QString str);
    /**
     * @brief
     *
     * @param str
     */
    void changeSang1(QString str);
    /**
     * @brief
     *
     * @param str
     */
    void changePocket1(QString str);
    /**
     * @brief
     *
     * @param str
     */
    void changeSang2(QString str);
    /**
     * @brief
     *
     * @param str
     */
    void changePocket2(QString str);
    /**
     * @brief
     *
     * @param str
     */
    void changePantsFoot(QString str);

    /**
     * @brief
     *
     * @param k
     */
    void keyPressEvent(QKeyEvent *k);
};

#endif // DIALOGPANTSH_H
