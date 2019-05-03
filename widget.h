#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QApplication>
#include <QBitmap>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>
#include <QIcon>
#include <QPropertyAnimation>
#include <QThread>
#include "form.h"
#include "c_form.h"
#include "dialog.h"
#include "registerform.h"
#define MAX 50
#if 1
#define IP_ADDR "39.108.95.238"
#else
#define IP_ADDR "192.168.4.156"
#endif


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    QString name;
    QTcpSocket *s;
    void ui_init();
    void show_body();
    void show_form();
    ~Widget();

private slots:
    void on_login_Btn_clicked(bool checked);
    void on_exit_Btn_clicked(bool checked);
    void on_register_Btn_clicked();
    void hide_form();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
