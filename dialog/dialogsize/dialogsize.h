#ifndef DIALOGSIZE_H
#define DIALOGSIZE_H

#include <QDialog>

namespace Ui {
class DialogSize;
}

class DialogSize : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSize(QWidget *parent = 0);
    ~DialogSize();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DialogSize *ui;

    void drawByRule(QString path, QString in = "");
};

#endif // DIALOGSIZE_H
