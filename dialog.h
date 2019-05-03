#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>
#include <QTcpSocket>
#include <type_traits>

namespace Ui {
class Dialog;
}

class Dialog : public QWidget
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0, QString buf = 0);
    ~Dialog();
    void ui_init();
    void net_init();
    void dialog_init();
    QString dia_body;
    QTcpSocket *s;

private slots:
    void on_spinBox_editingFinished();

    void on_pushButton_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
