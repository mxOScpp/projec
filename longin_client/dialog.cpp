#include "dialog.h"
#include "ui_dialog.h"
#include<QJsonArray>
#include<QJsonObject>
#include<QJsonDocument>
#include<QNetworkAccessManager>
#include<QCryptographicHash>
#include<QString>
#include<QUrl>
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    //lineEdit=new QLineEdit(ui->new_password);
    // connect(this, SIGNAL(clicked()), lineEdit,SLOT(clearLineEdit()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::SetUsername(QString _username)
{
    username = _username;
}

void Dialog::on_buttonBox_accepted()
{

    //1.从linetext中获取密码
    QString new_password;
    new_password = ui->new_password->text();
    QJsonObject json;
    //2.将密码进行签名加密
    QCryptographicHash md5_hash(QCryptographicHash::Md5);
    md5_hash.addData(new_password.toUtf8());
    QString md5_password(md5_hash.result().toHex());
    //将用户名与密码进行封装，存入到doc进行数据交换
    json["username"]=username;
    json["password"]=md5_password;
    QJsonDocument json_doc(json);
    QString output =  json_doc.toJson();
    //设置发送对象ip，设置请求头类型及长度
    QNetworkRequest req(QUrl("http://192.168.144.183/new_password/"));
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    req.setHeader(QNetworkRequest::ContentLengthHeader,output.size());
    //通过post模式发送
    net_mng.post(req,output.toUtf8());
    //clearLineEdit();

}

void Dialog::clearLineEdit()
{
    ui->new_password->clear();
    ui->old_password->clear();
    ui->username->clear();

}

