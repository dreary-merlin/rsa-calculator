#include "sizewindow.h"
#include "ui_sizewindow.h"
#include "rsa.h"

SizeWindow::SizeWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SizeWindow)
{
    ui->setupUi(this);
    QRegExp nameRag("^[1-9][0-9]+");
    QRegExpValidator *nameValidator = new QRegExpValidator(nameRag, this);
    ui->lineEdit->setValidator(nameValidator);
    ui->pushButton->setEnabled(false);
    connect(ui->lineEdit,SIGNAL(textChanged(QString)), this, SLOT(on_lineEdit_textChanged(const QString &)));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(on_pushButton_clicked()));

}

SizeWindow::~SizeWindow()
{
    delete ui;
}

void SizeWindow::on_pushButton_clicked()
{
    rsa * obj=new rsa(0,0);
    bool ok=false;
    QString size=ui->lineEdit->text();
    if(size.toUInt(&ok,10)>=5 && size.toUInt(&ok,10)<=65535){
        if((obj->trial_div(size.toUInt(&ok,10)))){
            if ((obj->Miller_Rabin_test(size.toUInt(&ok,10)))){
                emit sendData(ui->lineEdit->text()); // вызываем сигнал, в котором передаём введённые данные
                this->close();
            }
        }
    }
}

void SizeWindow::on_lineEdit_textChanged(const QString &arg1)
{
    ui->pushButton->setEnabled(!arg1.isEmpty());
}
