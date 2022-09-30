#ifndef SIZEWINDOW_H
#define SIZEWINDOW_H

#include <QDialog>

namespace Ui {
class SizeWindow;
}

class SizeWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SizeWindow(QWidget *parent = nullptr);
    ~SizeWindow();

signals:
    void sendData(QString str);

private slots:
    void on_pushButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::SizeWindow *ui;
};

#endif // SIZEWINDOW_H
