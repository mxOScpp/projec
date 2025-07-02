#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma execution_character_set("UTF-8")
#include <QMainWindow>
#include<QtNetwork/QNetworkRequest>
#include<QtNetwork/QNetworkReply>
#include<QtNetwork/QNetworkAccessManager>
#include"dialog.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_createroom_clicked();
    void proc_login_rep();

    void on_register_2_clicked();

    void on_new_password_clicked();


    void on_forgot_password_clicked();

    void on_follow_clicked();

private:
    Ui::MainWindow *ui;
    //在头文件中声明管理对象
    QNetworkAccessManager net_mng;
    QNetworkReply* net_rep = NULL;
    Dialog d;
    enum login_opt{
        create_room,follow_room,change_password
    } m_cur_opt;

    void Req_login();
    void Req_login(QString _opt);
    void Req_login(QString _opt,QString _room_no);
};
#endif // MAINWINDOW_H
