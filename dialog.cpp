#include "dialog.h"
#include "ui_dialog.h"
#include "widget.h"

Dialog::Dialog(QWidget *parent, QString buf) :
    QWidget(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //dia_body = "client history 0";
    net_init();
    //dialog_init();
}
void Dialog::net_init()
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
void Dialog::ui_init()
{

}
void Dialog::dialog_init()
{

    ui->listWidget->clear();
    char * buf;
    QByteArray ba = dia_body.toLatin1();
    buf = ba.data();
    //qDebug() << "<<" << buf << endl;
    char time1[10];
    char time2[10];
    char time3[10];
    char time4[10];
    char time5[10];
    char cmd[10];
    char tmp[100] = {0};
    int day = 0;
    int move_size;

    sscanf(buf, "%d", &day);
    if(day < 10)
    {
        move_size = 33;
    }
    else
    {
        move_size = 35;
    }
    char *p = buf;

#if 1

    while(1)
    {
        sscanf(p, " %*s %s %s %s %s %s %s",time1, time2, time3, time4, time5, cmd);
        switch (cmd[4]) {
        case '0':
            strcpy(cmd, "关灯");
            break;
        case '1':
            strcpy(cmd, "开到低");
            break;
        case '2':
            strcpy(cmd, "开到中");
            break;
        case '3':
            strcpy(cmd, "开到高");
            break;
        default:
            break;
        }
        sprintf(tmp, "%s %s %s %s %s %s", time1, time2, time3, time4, time5, cmd);
        //qDebug() << tmp << endl;;
        QListWidgetItem *item = new QListWidgetItem(tmp);
        ui->listWidget->addItem(item);
        p+=move_size;
        if(*(p) == '\0')
        {
            break;
        }
    }
#endif
}
Dialog::~Dialog()
{
    s->close();
    delete ui;
}

void Dialog::on_spinBox_editingFinished()
{
    int val = ui->spinBox->value();
    char valbuf[5] = {0};
    sprintf(valbuf, "%d", val);
    char buf[1024] = {0};
    QString cmd = "client history ";
    cmd+=valbuf;

    QByteArray arr = cmd.toLatin1();
    s->write(arr);
    qDebug() << ">>" << cmd << endl;
    !s->waitForReadyRead(~0);//阻塞
    s->read(buf, sizeof buf);
    qDebug() << "<<" << buf << endl;
    if(strncmp(buf, "no history!", 11) == 0)
    {
        ui->listWidget->clear();
        QListWidgetItem *item = new QListWidgetItem("no history");
        ui->listWidget->addItem(item);

    }
    else
    {
        dia_body = buf;
        dialog_init();
    }

}

void Dialog::on_pushButton_clicked()
{
    on_spinBox_editingFinished();
}
