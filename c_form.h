#ifndef C_FORM_H
#define C_FORM_H

#include <QWidget>
#include <QBitmap>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>
#include <QThread>
#include "widget.h"

namespace Ui {
class C_Form;
}
class Mythr_C : public QThread{
    Q_OBJECT
public:
    explicit Mythr_C(QObject *parent = 0);
    ~Mythr_C();
    void net_init();
    QTcpSocket *s;

signals:
    void sigTime(QString time);

protected:
    void run();
private:
    Ui::Form *ui;
};
class C_Form : public QWidget
{
    Q_OBJECT

public:
    explicit C_Form(QWidget *parent = 0, QString username = 0);
    ~C_Form();
    void ui_init(QString username);
    void net_init();
    QTcpSocket *s;

private slots:
    void on_exit_btn_clicked();

private:
    Ui::C_Form *ui;
    Mythr_C *thr;
};

#endif // C_FORM_H
