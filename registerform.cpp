#include "registerform.h"
#include "ui_registerform.h"
#include "widget.h"
#include <QPropertyAnimation>
RegisterForm::RegisterForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterForm)
{

    ui->setupUi(this);
    ui_init();
    Widget f;
}
void RegisterForm::ui_init()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),20,20);
    this->setMask(bmp);
    QPropertyAnimation *animation = new QPropertyAnimation(this,"windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
}

RegisterForm::~RegisterForm()
{
    delete ui;
}


void RegisterForm::on_confirm_Btn_clicked()
{
    if(ui->name_edit->text() == "" || ui->pass_edit->text() == "" || ui->confirm_edit->text() == "")
    {
        QMessageBox box(QMessageBox::Warning,"路灯管理系统","请正确填写注册信息");
        box.setStandardButtons (QMessageBox::Ok);
        box.setButtonText (QMessageBox::Ok,QString("确 定"));
        QIcon icon(":/res/icon.png");
        box.setWindowIcon(icon);
        box.exec();
    }
    else if(ui->pass_edit->text() != ui->confirm_edit->text())
    {
        QMessageBox box(QMessageBox::Warning,"路灯管理系统","两次输入的密码不一致");
        box.setStandardButtons (QMessageBox::Ok);
        box.setButtonText (QMessageBox::Ok,QString("确 定"));
        QIcon icon(":/res/icon.png");
        box.setWindowIcon(icon);
        box.exec();
    }
    else
    {
        //start register
        QTcpSocket *s = new QTcpSocket(this);
        //连接服务器
        s->connectToHost(QHostAddress(IP_ADDR),5555);
        //等待连接成功
        if(!s->waitForConnected(2000))
        {
            //qDebug() << "connect failed" << endl;
            QMessageBox box(QMessageBox::Warning,"路灯管理系统","登录超时，请检查你的网络连接");
            box.setStandardButtons (QMessageBox::Ok);
            box.setButtonText (QMessageBox::Ok,QString("确 定"));
            QIcon icon(":/res/icon.png");
            box.setWindowIcon(icon);
            box.exec();
        }
        else
        {
            char buf[MAX] = {0};
            QString name = ui->name_edit->text();
            QString psw = ui->pass_edit->text();
            QString cmd = "enroll " + name + " " + psw;
            qDebug() << cmd << endl;
            QByteArray arr = cmd.toLatin1();
            s->write(arr);
            !s->waitForReadyRead(~0);//阻塞
            s->read(buf, sizeof buf);
            qDebug() << "<<" << buf << endl;
            if(strncmp(buf, "test pass", 9) == 0)
            {
                QMessageBox box(QMessageBox::Information,"路灯管理系统","恭喜你！注册成功！");
                box.setStandardButtons (QMessageBox::Ok);
                box.setButtonText (QMessageBox::Ok,QString("确 定"));
                QIcon icon(":/res/icon.png");
                box.setWindowIcon(icon);
                box.exec();
            }
            else
            {
                QMessageBox box(QMessageBox::Warning,"路灯管理系统", buf);
                box.setStandardButtons (QMessageBox::Ok);
                box.setButtonText (QMessageBox::Ok,QString("确 定"));
                QIcon icon(":/res/icon.png");
                box.setWindowIcon(icon);
                box.exec();
            }
        }
    }
}
void RegisterForm::set_parent(QWidget *prt)
{
    this->prt = prt;
}
void RegisterForm::on_exit_Btn_clicked()
{
    int reply;
    QMessageBox box(QMessageBox::Warning,"路灯管理系统","是否退出路灯管理系统");
    box.setStandardButtons (QMessageBox::Ok|QMessageBox::Cancel);
    box.setButtonText (QMessageBox::Ok,QString("是"));
    box.setButtonText (QMessageBox::Cancel,QString("否"));
    QIcon icon(":/res/icon.png");
    box.setWindowIcon(icon);
    reply = box.exec();
    if(reply == QMessageBox::Ok)
    {
        this->close();
    }
}

void RegisterForm::on_back_Btn_clicked()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this,"windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation, SIGNAL(finished()), this, SLOT(close()));
    prt->show();
}
