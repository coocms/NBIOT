#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui_init();
}
void Widget::ui_init()
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
    animation->setDuration(500);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();

}
Widget::~Widget()
{
    delete ui;
}

void Widget::on_login_Btn_clicked(bool checked)
{
#if 1
    if(ui->nameEdit->text() == "" || ui->passEdit->text() == "")
    {
        QMessageBox box(QMessageBox::Warning,"路灯管理系统","请输入正确的用户名和密码");
        box.setStandardButtons (QMessageBox::Ok);
        box.setButtonText (QMessageBox::Ok,QString("确 定"));
        QIcon icon(":/res/icon.png");
        box.setWindowIcon(icon);
        box.exec();
    }
    else
    {
        s = new QTcpSocket(this);
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
            s->close();
        }
        else
        {
            char buf[MAX] = {0};
            QString name = ui->nameEdit->text();
            QString psw = ui->passEdit->text();
            QString cmd = "login " + name + " " + psw;
            qDebug() << cmd << endl;
            QByteArray arr = cmd.toLatin1();
            s->write(arr);
            !s->waitForReadyRead(~0);//阻塞
            s->read(buf, sizeof buf);
            qDebug() << "<<" << buf << endl;
            if(strncmp(buf, "test pass", 9) == 0)
            {
                QString username = ui->nameEdit->text();
                if(username == "root")
                {
                    static Form form(0, username, ROOT_MODE);
                    form.show();
                    this->hide();
                }
                else
                {
                    static Form form(0, username, COMMON_MODE);
                    form.show();
                    this->hide();

                }
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

#endif
}

void Widget::on_exit_Btn_clicked(bool checked)
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
        QPropertyAnimation *animation = new QPropertyAnimation(this,"windowOpacity");
        animation->setDuration(1000);
        animation->setStartValue(1);
        animation->setEndValue(0);
        animation->start();
        connect(animation, SIGNAL(finished()), this, SLOT(close()));
        s->close();
    }

}

void Widget::on_register_Btn_clicked()
{
    RegisterForm *form = new RegisterForm(0);
    form->show();
    form->set_parent(this);
    this->close();
}

#if 1
void Widget::hide_form()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this,"windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
}
void Widget::show_form()
{
    qDebug() << "show_form()" << endl;
    QPropertyAnimation *animation = new QPropertyAnimation(this,"windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
}
#endif
