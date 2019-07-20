#ifndef DIALOGDESIGN_H
#define DIALOGDESIGN_H

#include <QDialog>

namespace Ui {
class DialogDesign;
}

class WidgetKuoXing;
class DialogPantsH;

/**
 * @brief 款式设计
 */
class DialogDesign : public QDialog
{
    Q_OBJECT

public slots:
    void enterStep2(int i);

public:
    explicit DialogDesign(QWidget *parent = nullptr);
    ~DialogDesign();

    int step; /**< TODO: describe */

private slots:
    void on_pushButton_1_clicked();

private:
    Ui::DialogDesign *ui; /**< TODO: describe */
    WidgetKuoXing *kuoxing; /**< TODO: describe */
    DialogPantsH *pantsH; /**< TODO: describe */

signals:
    void selectKuoXing(int i);
};

#endif // DIALOGDESIGN_H
