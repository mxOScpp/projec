#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QJsonDocument>
#include<QJsonArray>
#include<QJsonObject>
#include<QDebug>
#include<QCryptographicHash>
#include<QUrl>
#include<QMessageBox>

#include<QDesktopServices>
#include<QProcess>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Req_login()
{
    //1.取出用户名与密码
    QString username = ui->username->text();
    QString password = ui->password->text();

    //在使用json封装时将passwd进行签名
    QCryptographicHash md5_hash(QCryptographicHash::Md5);
    md5_hash.addData(password.toUtf8());
    QString md5_passwd(md5_hash.result().toHex());
    //使用json进行数据封装
    QJsonObject json;
    json["username"] = username;
    json["password"] = md5_passwd;
    //将封装好的json对象添加到json文档中以便发出
    QJsonDocument json_doc(json);
    QString output= json_doc.toJson();


    //2.将用户名与密码拼接为请求数据
    //（1）设置请求头
    QNetworkRequest req(QUrl("http://192.168.144.183/login/"));
    req.setHeader(QNetworkRequest::ContentLengthHeader,output.size());
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    //3.发送至登录服务器
    //（2）将数据通过post模式发出,数据最好转换格式

    net_rep= net_mng.post(req,output.toUtf8());
    //4.异步处理服务器的回复
    connect(net_rep,SIGNAL(finished()),this,SLOT(proc_login_rep()));
}
void MainWindow::Req_login(QString _opt)
{
    //1.取出用户名与密码
    QString username = ui->username->text();
    QString password = ui->password->text();

    //在使用json封装时将passwd进行签名
    QCryptographicHash md5_hash(QCryptographicHash::Md5);
    md5_hash.addData(password.toUtf8());
    QString md5_passwd(md5_hash.result().toHex());
    //使用json进行数据封装
    QJsonObject json;
    json["username"] = username;
    json["password"] = md5_passwd;

    json["opt"] = _opt;
    //将封装好的json对象添加到json文档中以便发出
    QJsonDocument json_doc(json);
    QString output= json_doc.toJson();


    //2.将用户名与密码拼接为请求数据
    //（1）设置请求头
    QNetworkRequest req(QUrl("http://192.168.144.183/login/"));
    req.setHeader(QNetworkRequest::ContentLengthHeader,output.size());
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    //3.发送至登录服务器
    //（2）将数据通过post模式发出,数据最好转换格式

    net_rep= net_mng.post(req,output.toUtf8());
    //4.异步处理服务器的回复
    connect(net_rep,SIGNAL(finished()),this,SLOT(proc_login_rep()));
}
void MainWindow::Req_login(QString _opt,QString _room_no)
{

    //1.取出用户名与密码
    QString username = ui->username->text();
    QString password = ui->password->text();

    //在使用json封装时将passwd进行签名
    QCryptographicHash md5_hash(QCryptographicHash::Md5);
    md5_hash.addData(password.toUtf8());
    QString md5_passwd(md5_hash.result().toHex());
    //使用json进行数据封装
    QJsonObject json;
    json["username"] = username;
    json["password"] = md5_passwd;

    json["opt"] = _opt;
    json["room_no"] = _room_no;
    //将封装好的json对象添加到json文档中以便发出
    QJsonDocument json_doc(json);
    QString output= json_doc.toJson();


    //2.将用户名与密码拼接为请求数据
    //（1）设置请求头
    QNetworkRequest req(QUrl("http://192.168.144.183/login/"));
    req.setHeader(QNetworkRequest::ContentLengthHeader,output.size());
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    //3.发送至登录服务器
    //（2）将数据通过post模式发出,数据最好转换格式

    net_rep= net_mng.post(req,output.toUtf8());
    //4.异步处理服务器的回复
    connect(net_rep,SIGNAL(finished()),this,SLOT(proc_login_rep()));
}
void MainWindow::on_createroom_clicked()
{
    m_cur_opt = create_room;
    Req_login("create_room");
//docker ps -n 1 -f ancestor=game:v1 |grep game_start|awk -F '->' '{print $1}'|awk -F : '{print $NF}'
}

void MainWindow::proc_login_rep()
{
    //qDebug()<<net_rep->readAll();
    //接收服务器的回复
    //1.服务器回复为字符串，无法方便的解析,需使用json来解析
    //(1).创建doc对象，将字符串导入进行转换。
    QJsonDocument reply = QJsonDocument::fromJson(net_rep->readAll());
    //(2).创建json对象，将doc中json对象取出赋值
    QJsonObject json = reply.object();
    //(3).取出json对象中解析到的值，进行判断
    if("OK"==json["login_result"].toString())//注意在json中字符串还是json对象，需要进行转换
    {
        if(m_cur_opt==create_room)
        {
            //登录成功后最好直接退出此界面，并启动游戏界面
            //游戏界面要独立于登录器程序界面
            qDebug()<<"login success";
            //1.创建QProcess对象使用startDetached函数来进行进程独立
            QProcess proc;
            //2.将startDetached函数第二参数所需的值，初始化为ip端口，这是client打开必须要有的
            QStringList argv;
            int room_no = json["room_no"].toInt();

            QMessageBox::information(this, "房间号","房间号："+QString::number(room_no));

            argv.push_back(json["server_IP"].toString());
            argv.push_back(json["server_PORT"].toString());
            proc.startDetached("client.exe",argv,".");
            //3.打开游戏界面后退出登录器
            //使用指向（QApplication 或 QGuiApplication、QCoreApplication）等类的qApp全局指针
            qApp->exit();
        }
        else if(m_cur_opt==follow_room)
        {
            if(	"OK"==json["follow_result"].toString())
            {
                qDebug()<<"login success";
                //1.创建QProcess对象使用startDetached函数来进行进程独立
                QProcess proc;
                //2.将startDetached函数第二参数所需的值，初始化为ip端口，这是client打开必须要有的
                QStringList argv;
                QString room_no = ui->room_no->text();
                argv.push_back(json["server_IP"].toString());
                argv.push_back(json["server_PORT"].toString());
                proc.startDetached("client.exe",argv,".");
                //3.打开游戏界面后退出登录器
                //使用指向（QApplication 或 QGuiApplication、QCoreApplication）等类的qApp全局指针
                qApp->exit();
            }
            else
            {
                QMessageBox::information(this, "跟房失败", "房间不存在");
            }
        }
        else if(m_cur_opt == change_password)
        {
            d.SetUsername(ui->username->text());
            d.show();
        }
    }
    else
    {
        if((m_cur_opt == change_password))
        {
            QMessageBox::information(this,"修改密码失败","请正确填写用户名与旧密码");
        }
        else{
            //登录失败,弹出窗口
            QMessageBox::information(this,"登录失败","用户名或密码错误");
        }

    }


}

//使用QDesktopServices类来直接跳转到注册界面
void MainWindow::on_register_2_clicked()
{
    QDesktopServices::openUrl(QUrl("http://192.168.144.183:80"));
}

void MainWindow::on_new_password_clicked()
{
    m_cur_opt=change_password;
    Req_login();

}

void MainWindow::on_forgot_password_clicked()
{
    //Req_login();
    d.SetUsername(ui->username->text());
    d.show();
}


void MainWindow::on_follow_clicked()
{
    m_cur_opt = follow_room;
    Req_login("follow_room",ui->room_no->text());
}

