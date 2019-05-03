#include "form.h"
#include "ui_form.h"
#include "widget.h"
#include "dialog.h"
#include <QPicture>
#if 1
char sendbuf[100];

Mythr::Mythr(QObject *parent):
    QThread(parent)
{

}
#endif
void Mythr::net_init()
{
    int reply;
    s = new QTcpSocket(this);
    //连接服务器
    s->connectToHost(QHostAddress(IP_ADDR),8888);
    //等待连接成功
    if(!s->waitForConnected(2000))
    {
        //qDebug() << "connect failed" << endl;
        QMessageBox box(QMessageBox::Warning,"路灯管理系统","线程连接 控制服务器连接超时，请检查你的网络连接");
        box.setStandardButtons (QMessageBox::Ok);
        box.setButtonText (QMessageBox::Ok,QString("确 定"));
        QIcon icon(":/res/icon.png");
        box.setWindowIcon(icon);
        reply = box.exec();
        s->close();
        if(reply == QMessageBox::Ok)
        {
            this->exit();
        }
    }
}
void Mythr::run()
{
    net_init();
    QString arr;
    while(1)
    {
        //qDebug() << "thr id: " << QThread::currentThreadId() << endl;
        char buf[1024] = {0};
        QString cmd = "client get";
        //qDebug() << cmd << endl;
        QByteArray arr = cmd.toLatin1();
        s->write(arr);
        !s->waitForReadyRead(~0);//阻塞
        s->read(buf, sizeof buf);
        qDebug() << "<<" << buf << endl;
        char mode[10] = {0};
        char env_level[20] = {0};
        char light_level[20] = {0};
        char red_level[10] = {0};
        char is_broken[10] = {0};
        char word[100];
        sscanf(buf, "%s %s %s %s %s", mode, env_level, light_level, red_level, is_broken);

        if(strncmp(red_level, "no", 2) == 0)
        {
            strcpy(is_broken, "no");
            strcpy(red_level, "no");
            if(strncmp(mode, "auto", 4) == 0)
            {
                strcpy(word, &buf[16]);
            }
            else
            {
                strcpy(word, &buf[17]);
            }
            QPixmap pix;
            pix.load(":/res/not_connected.png");
            emit is_connect_set(pix);
        }
        else
        {
            if(strncmp(mode, "auto", 4) == 0)
            {
                strcpy(word, &buf[19]);
            }
            else
            {
                strcpy(word, &buf[20]);
            }
            QPixmap pix;
            pix.load(":/res/is_connected.png");
            emit is_connect_set(pix);
        }
        qDebug() << word << endl;
        if(strncmp(word, sendbuf, 10) != 0)
        {
            if(strcmp(word, " ") != 0)
            {
                QString display_string = 0;
                display_string = word;
                strcpy(sendbuf, word);
                emit set_display(display_string);
            }
        }

        if(strncmp(mode, "auto", 4) == 0)
        {
            QString buf = "自动";
            emit set_mode(buf);
        }
        else
        {
            QString buf = "手动";
            emit set_mode(buf);
        }
        emit set_env_level(env_level);
        emit set_light_level(light_level);
        emit set_red(red_level);
        emit set_broken(is_broken);
        QThread::sleep(1);
    }
}
void Form::slot_set_red(QString buf)
{
    if(buf == "0")
    {
        ui->red_now->setText("无行人");
    }
    else if(buf == "no")
    {
        ui->red_now->setText("未知");
    }
    else
    {
        ui->red_now->setText("有行人");
    }

}
void Form::slot_set_mode(QString buf)
{
    ui->mode_now->setText(buf);
}
void Form::slot_set_env_level(QString buf)
{
    if(buf == "no")
    {
        ui->env_now->setText("未知");
    }
    else
    {
        ui->env_now->setText(buf);
    }

}
void Form::slot_set_light_level(QString buf)
{
    if(buf == "no")
    {
        ui->light_now->setText("未知");
    }
    else
    {
        ui->light_now->setText(buf);
    }

}
void Form::slot_set_display(QString buf)
{
    QListWidgetItem *item = new QListWidgetItem(buf);
    ui->listWidget->addItem(item);
}
void Form::slot_set_broken(QString buf)
{
    if(buf == "0")
    {
        ui->brocken_state->setText("正常");
    }
    else if(buf == "no")
    {
        ui->brocken_state->setText("未知");
    }
    else
    {
        ui->brocken_state->setText("故障");
    }

}
Form::Form(QWidget *parent, QString username, int isRoot) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    user_mode_init(isRoot);
    ui_init(username);
    net_init();
    hide_setting();
    thr = new Mythr(this);
    thr->start();
    connect(thr,SIGNAL(is_connect_set(QPixmap)),
                this, SLOT(slot_isconnect(QPixmap)));
    connect(thr,SIGNAL(set_env_level(QString)),
                this, SLOT(slot_set_env_level(QString)));
    connect(thr,SIGNAL(set_light_level(QString)),
                this, SLOT(slot_set_light_level(QString)));
    connect(thr,SIGNAL(set_mode(QString)),
                this, SLOT(slot_set_mode(QString)));
    connect(thr,SIGNAL(set_red(QString)),
                this, SLOT(slot_set_red(QString)));
    connect(thr,SIGNAL(set_display(QString)),
                this, SLOT(slot_set_display(QString)));
    connect(thr,SIGNAL(set_broken(QString)),
                this, SLOT(slot_set_broken(QString)));
}
void Form::net_init()
{
    s = new QTcpSocket(this);
    //连接服务器
    s->connectToHost(QHostAddress(IP_ADDR),8888);
    //等待连接成功
    if(!s->waitForConnected(2000))
    {
        //qDebug() << "connect failed" << endl;
        QMessageBox box(QMessageBox::Warning,"路灯管理系统","控制服务器连接超时，请检查你的网络连接");
        box.setStandardButtons (QMessageBox::Ok);
        box.setButtonText (QMessageBox::Ok,QString("确 定"));
        QIcon icon(":/res/icon.png");
        box.setWindowIcon(icon);
        box.exec();
        this->destroy();
    }
}
void Form::show_setting()
{
    ui->radio0->show();
    ui->radio1->show();
    ui->radio2->show();
    ui->radio3->show();
    ui->confirm_Btn->show();
}
void Form::hide_setting()
{
    ui->radio0->hide();
    ui->radio1->hide();
    ui->radio2->hide();
    ui->radio3->hide();
    ui->confirm_Btn->hide();
}
Form::~Form()
{
    disconnect(thr,SIGNAL(is_connect_set(QPixmap)),
                this, SLOT(slot_isconnect(QPixmap)));
    disconnect(thr,SIGNAL(set_env_level(QString)),
                this, SLOT(slot_set_env_level(QString)));
    disconnect(thr,SIGNAL(set_light_level(QString)),
                this, SLOT(slot_set_light_level(QString)));
    disconnect(thr,SIGNAL(set_mode(QString)),
                this, SLOT(slot_set_mode(QString)));
    disconnect(thr,SIGNAL(set_red(QString)),
                this, SLOT(slot_set_red(QString)));
    disconnect(thr,SIGNAL(set_display(QString)),
                this, SLOT(slot_set_display(QString)));
    s->close();
    delete ui;
}
void Form::ui_init(QString username)
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
    this->username = username;
    ui->username->setText(username);
    ui->recycle_btn->hide();
    ui->rc_pic->hide();


    if(user_mode == COMMON_MODE)
    {
        ui->auto_btn->hide();
        ui->setting_btn->hide();
        ui->dialog_btn->hide();
        ui->set_pic->hide();
        ui->auto_pic->hide();
        ui->listWidget->hide();
    }
    else
    {
        ui->textEdit->hide();
        ui->send_btn->hide();
    }

}
void Form::user_mode_init(int isRoot)
{
    if(isRoot == ROOT_MODE)
    {
        this->user_mode = ROOT_MODE;
        user_mode = ROOT_MODE;
        ui->isroot->setText("root");
    }
    else if(isRoot == COMMON_MODE)
    {
        this->user_mode = COMMON_MODE;
        user_mode = COMMON_MODE;
        ui->isroot->setText("common");
    }

}

void not_root_mode()
{
    QMessageBox box(QMessageBox::Warning,"路灯管理系统","当前用户不是root管理员");
    box.setStandardButtons (QMessageBox::Ok);
    box.setButtonText (QMessageBox::Ok,QString("确 定"));
    QIcon icon(":/res/icon.png");
    box.setWindowIcon(icon);
    box.exec();
}
void no_history()
{
    QMessageBox box(QMessageBox::Warning,"路灯管理系统","no history");
    box.setStandardButtons (QMessageBox::Ok);
    box.setButtonText (QMessageBox::Ok,QString("确 定"));
    QIcon icon(":/res/icon.png");
    box.setWindowIcon(icon);
    box.exec();
}
void Form::on_dialog_btn_clicked()
{
    Dialog *form = new Dialog(0, 0);
    form->show();
}
void Form::on_auto_btn_clicked()
{
    if(this->user_mode == ROOT_MODE)
    {
        //ui->mode_now->setText("auto");//模式切换为自动模式
        QString cmd = "client auto";
        QByteArray arr = cmd.toLatin1();
        s->write(arr);

        hide_setting();
    }
    else
    {
        not_root_mode();
    }

}

void Form::on_recycle_btn_clicked()
{
    if(this->user_mode == ROOT_MODE)
    {
        //ui->mode_now->setText("recycle");//模式切换为节能模式
    }
    else
    {
        not_root_mode();
    }

}

void Form::on_setting_btn_clicked()
{
    if(this->user_mode == ROOT_MODE)
    {
        //ui->mode_now->setText("setting");//模式切换为手动模式
        show_setting();
    }
    else
    {
        not_root_mode();
    }

}
void Form::on_exit_btn_clicked()
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
        s->close();
        thr->terminate();
    }
}

void Form::on_confirm_Btn_clicked()
{
    QString cmd;
    if(ui->radio0->isChecked())
    {
        cmd = "client turn0";
    }
    else if(ui->radio1->isChecked())
    {
        cmd = "client turn1";
    }
    else if(ui->radio2->isChecked())
    {
        cmd = "client turn2";
    }
    else if(ui->radio3->isChecked())
    {
        cmd = "client turn3";
    }
    QByteArray arr = cmd.toLatin1();
    s->write(arr);
    qDebug() << cmd << endl;
}

Mythr::~Mythr()
{
    s->close();
}


void Form::slot_isconnect(QPixmap pix)
{
    ui->is_connet->setPixmap(pix);
    ui->is_connet->setScaledContents(true);

}

void Form::on_send_btn_clicked()
{

    QString buf = ui->textEdit->toPlainText();
    ui->textEdit->clear();
    if(buf!= 0)
    {
        QString cmd = "client send ";
        cmd = cmd + username + ": ";
        cmd+=buf;
        QByteArray arr = cmd.toLatin1();
        s->write(arr);
        qDebug() << cmd << endl;
        int reply;
        QMessageBox box(QMessageBox::Warning,"路灯管理系统","报告成功");
        box.setStandardButtons (QMessageBox::Ok);
        box.setButtonText (QMessageBox::Ok,QString("是"));
        QIcon icon(":/res/icon.png");
        box.setWindowIcon(icon);
        reply = box.exec();
    }
    else
    {
        qDebug() << "cant send empty" << endl;
    }

}
