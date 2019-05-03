#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QBitmap>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>
#include <QThread>

#define COMMON_MODE 0
#define ROOT_MODE 1
namespace Ui {
class Form;
}
class Mythr : public QThread{
    Q_OBJECT
public:
    explicit Mythr(QObject *parent = 0);
    ~Mythr();
    void net_init();
    QTcpSocket *s;

signals:
    void is_connect_set(QPixmap pix);
    void set_env_level(QString buf);
    void set_light_level(QString buf);
    void set_mode(QString buf);
    void set_red(QString buf);
    void set_display(QString buf);
    void set_broken(QString buf);

protected:
    void run();
private slots:

private:
    Ui::Form *ui;
};
class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0, QString username = 0, int isRoot = COMMON_MODE);
    void ui_init(QString username);
    void net_init();
    QTcpSocket *s;
    QString username;
    void show_setting();
    void hide_setting();
    void user_mode_init(int isRoot);

    ~Form();
    int user_mode;
    int work_mode;
signals:
    void start_thread();
private slots:
    void on_exit_btn_clicked();
    void on_auto_btn_clicked();
    void on_recycle_btn_clicked();
    void on_setting_btn_clicked();
    void on_confirm_Btn_clicked();
    void on_dialog_btn_clicked();
    void slot_isconnect(QPixmap pix);
    void slot_set_env_level(QString buf);
    void slot_set_light_level(QString buf);
    void slot_set_red(QString buf);
    void slot_set_mode(QString buf);
    void slot_set_display(QString buf);
    void slot_set_broken(QString buf);
    void on_send_btn_clicked();
private:
    Ui::Form *ui;
    Mythr *thr;

};

#endif // FORM_H
