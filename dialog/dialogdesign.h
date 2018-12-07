#ifndef DIALOGDESIGN_H
#define DIALOGDESIGN_H

#include <QDialog>

namespace Ui {
class DialogDesign;
}

class WidgetKuoXing;
class DialogPantsH;

class DialogDesign : public QDialog
{
    Q_OBJECT

public slots:
    void enterStep2(int i);

public:
    explicit DialogDesign(QWidget *parent = 0);
    ~DialogDesign();

    int step;

private slots:
    void on_pushButton_1_clicked();

private:
    Ui::DialogDesign *ui;
    WidgetKuoXing *kuoxing;
    DialogPantsH *pantsH;

signals:
    void selectKuoXing(int i);
};

#endif // DIALOGDESIGN_H
