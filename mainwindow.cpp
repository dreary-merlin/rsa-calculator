#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sizewindow.h"
#include "rsa.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_3->setDefault(true);
    ui->pushButton_2->setEnabled(false);
    ui->tabWidget->setEnabled(false);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(close()));
    sWindow=new SizeWindow();
    connect(ui->pushButton_3,SIGNAL(clicked()),sWindow,SLOT(exec())); // подключаем сигнал к слоту
    connect(sWindow, SIGNAL(sendData(QString)), this, SLOT(recieveData(QString))); // подключение сигнала к слоту нашей формы
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(on_pushButton_2_clicked()));
    connect(ui->textEdit,SIGNAL(textChanged()), this, SLOT(on_textEdit_textChanged()));
    connect(ui->lineEdit_7,SIGNAL(textChanged(QString)), this, SLOT(on_lineEdit_9_textChanged(const QString &)));
    connect(ui->pushButton_5,SIGNAL(clicked()),this,SLOT(on_pushButton_5_clicked()));
    connect(ui->pushButton_4,SIGNAL(clicked()),this,SLOT(on_pushButton_4_clicked()));
    connect(ui->textEdit_2,SIGNAL(textChanged()), this, SLOT(on_textEdit_2_textChanged()));
    connect(ui->pushButton_6,SIGNAL(clicked()),this,SLOT(on_pushButton_6_clicked()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::N="";
QString MainWindow::E="";
QString MainWindow::D="";
QString MainWindow::cipher="";


void MainWindow::recieveData(QString str)
{
    timer.restart();
    QString size=str;
    bool ok=false;
    rsa * obj=new rsa(0, 0);
    QString p, q, phi;
    do {
            p = QString::number((obj->prime_number(size.toUShort(&ok,10))));
            do {
                q = QString::number((obj->prime_number(size.toUShort(&ok,10))));
            } while (p == q);
            N = QString::number((p.toULongLong(&ok,10))*(q.toULongLong(&ok,10)));
            phi = QString::number(((p.toULongLong(&ok,10))-1)*((q.toULongLong(&ok,10))-1));
            if (phi.toULongLong(&ok,10) > 65537)
                E = QString::number(65537);
            else if (phi.toULongLong(&ok,10) > 257)
                E = QString::number(257);
            else
                E = QString::number(17);
            D = QString::number(obj->inverse(E.toULongLong(&ok,10), phi.toULongLong(&ok,10)));
        } while (E == D);
    ui->lineEdit->setText(p);
    ui->lineEdit_2->setText(q);
    ui->lineEdit_3->setText(N);
    ui->lineEdit_4->setText(phi);
    ui->lineEdit_5->setText(E);
    ui->lineEdit_6->setText(D);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->pushButton_3->setDefault(false);
    ui->pushButton_2->setDefault(true);
    ui->pushButton_2->setEnabled(true);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->pushButton_2->setDefault(false);
    ui->lineEdit_7->setText('('+E+", "+N+')');
    ui->lineEdit_8->setText('('+D+", "+N+')');
    ui->tabWidget->setEnabled(true);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->textEdit_2->setEnabled(false);
    ui->textEdit_3->setEnabled(false);
}

void MainWindow::on_textEdit_textChanged()
{
    QString str=ui->textEdit->toPlainText();
    ui->pushButton_4->setEnabled(!str.isEmpty());
}

void MainWindow::on_pushButton_5_clicked()
{
    QString str=ui->lineEdit_9->text();
    QFile rFile(str);
    if(!rFile.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::information(this,"Error", "Некорректный путь к файлу");
        return;
    }
    QTextStream in(&rFile);
    QString buffer=in.readAll();
    ui->textEdit->setText(buffer);
    rFile.flush();
    rFile.close();
}

void MainWindow::on_lineEdit_9_textChanged(const QString &arg1)
{
    ui->pushButton_5->setEnabled(!arg1.isEmpty());
}

void MainWindow::on_pushButton_4_clicked()
{
    timer.restart();
    QString str=ui->textEdit->toPlainText();
    std::vector<std::uint8_t> tVec(str.size());
    std::string ascii="";
    for(std::int32_t i=0;i<str.size();i++){
        tVec[i]=static_cast<std::uint8_t>((str.toLocal8Bit()).at(i));
        if ((static_cast<int32_t>(tVec[i])) < 26)
            ascii += "0";
        ascii+=std::to_string(tVec[i]);
    }
    bool ok=false;
    rsa *obj=new rsa(E.toULongLong(&ok,10), N.toULongLong(&ok,10));

    std::string strcrypt=obj->crypt(ascii,obj->_key, true);
    cipher="";
    cipher=QString::fromStdString(strcrypt);

    ui->textEdit_2->setEnabled(true);
    ui->textEdit_2->setText(QString::fromStdString(strcrypt));
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_6_clicked()
{
    bool ok=false;
    rsa *obj=new rsa(D.toULongLong(&ok,10), N.toULongLong(&ok,10));
    std::string strdecrypt=obj->crypt(cipher.toStdString(),obj->_key, false);

    QMap<int, QString> rus = { {192, "А"}, {193, "Б"}, {194, "В"}, {195, "Г"}, {196, "Д"}, {197, "Е"},
                               {198, "Ж"}, {199, "З"}, {200, "И"}, {201, "Й"}, {202, "К"}, {203, "Л"},
                               {204, "М"}, {205, "Н"}, {206, "О"}, {207, "П"}, {208, "Р"}, {209, "С"},
                               {210, "Т"}, {211, "У"}, {212, "Ф"}, {213, "Х"}, {214, "Ц"}, {215, "Ч"},
                               {216, "Ш"}, {217, "Щ"}, {218, "Ъ"}, {219, "Ы"}, {220, "Ь"}, {221, "Э"},
                               {222, "Ю"}, {223, "Я"}, {224, "а"}, {225, "б"}, {226, "в"}, {227, "г"},
                               {228, "д"}, {229, "е"}, {230, "ж"}, {231, "з"}, {232, "и"}, {233, "й"},
                               {234, "к"}, {235, "л"}, {236, "м"}, {237, "н"}, {238, "о"}, {239, "п"},
                               {240, "р"}, {241, "с"}, {242, "т"}, {243, "у"}, {244, "ф"}, {245, "х"},
                               {246, "ц"}, {247, "ч"}, {248, "ш"}, {249, "щ"}, {250, "ъ"}, {251, "ы"},
                               {252, "ь"}, {253, "э"}, {254, "ю"}, {255, "я"} };

    std::size_t i = 0;
    std::string text = "";
    while (i < strdecrypt.length()) {
        std::string block = "";
        for (size_t j = i; j < (i+3); j++) {
            block += strdecrypt[j];
            if (j == strdecrypt.length())
                break;
        }
        std::int32_t num = stoi(block);
        if (num < 256) {
            if(num>=192)
                text+=rus.value(num).toStdString();
            else
                text+= static_cast<char>(num);
            i = i + 3;
        }
        else {
            num = num / 10;
            text += static_cast<char>(num);
            i = i + 2;
        }
    }

    ui->textEdit_3->setEnabled(true);
    ui->textEdit_3->setPlainText(QString::fromStdString(text));
    ui->tabWidget->setCurrentIndex(2);
}

void MainWindow::on_textEdit_2_textChanged()
{
    QString str=ui->textEdit_2->toPlainText();
    ui->pushButton_6->setEnabled(!str.isEmpty());
}
