#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sizewindow.h"
#include <QElapsedTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static QString N,E,D, cipher;
    QElapsedTimer timer;

private slots:
    void on_pushButton_3_clicked();

    void recieveData(QString str);

    void on_pushButton_2_clicked();

    void on_textEdit_textChanged();

    void on_pushButton_5_clicked();

    void on_lineEdit_9_textChanged(const QString &arg1);

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_textEdit_2_textChanged();

private:
    Ui::MainWindow *ui;
    SizeWindow *sWindow;
};

#endif // MAINWINDOW_H
