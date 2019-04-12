#ifndef DIALOGSIZE_H
#define DIALOGSIZE_H

#include <QDialog>

namespace Ui {
class DialogSize;
}

/**
 * @brief
 *
 */
class DialogSize : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit DialogSize(QWidget *parent = 0);
    /**
     * @brief
     *
     */
    ~DialogSize();

private slots:
    /**
     * @brief
     *
     */
    void on_pushButton_clicked();

private:
    Ui::DialogSize *ui; /**< TODO: describe */
};

#endif // DIALOGSIZE_H
