#ifndef REGISTERFORM_H
#define REGISTERFORM_H

#include <QWidget>
#include <QApplication>
#include <QBitmap>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>
#include <QIcon>
#include "widget.h"
namespace Ui {
class RegisterForm;
}

class RegisterForm : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterForm(QWidget *parent = 0);
    QWidget *prt;
    void ui_init();
    void set_parent(QWidget *prt);
    ~RegisterForm();

private slots:

    void on_confirm_Btn_clicked();

    void on_exit_Btn_clicked();

    void on_back_Btn_clicked();
signals:
    void open_parent();
private:
    Ui::RegisterForm *ui;
};

#endif // REGISTERFORM_H
