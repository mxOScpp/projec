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
    //1.ȡ���û���������
    QString username = ui->username->text();
    QString password = ui->password->text();

    //��ʹ��json��װʱ��passwd����ǩ��
    QCryptographicHash md5_hash(QCryptographicHash::Md5);
    md5_hash.addData(password.toUtf8());
    QString md5_passwd(md5_hash.result().toHex());
    //ʹ��json�������ݷ�װ
    QJsonObject json;
    json["username"] = username;
    json["password"] = md5_passwd;
    //����װ�õ�json������ӵ�json�ĵ����Ա㷢��
    QJsonDocument json_doc(json);
    QString output= json_doc.toJson();


    //2.���û���������ƴ��Ϊ��������
    //��1����������ͷ
    QNetworkRequest req(QUrl("http://192.168.144.183/login/"));
    req.setHeader(QNetworkRequest::ContentLengthHeader,output.size());
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    //3.��������¼������
    //��2��������ͨ��postģʽ����,�������ת����ʽ

    net_rep= net_mng.post(req,output.toUtf8());
    //4.�첽����������Ļظ�
    connect(net_rep,SIGNAL(finished()),this,SLOT(proc_login_rep()));
}
void MainWindow::Req_login(QString _opt)
{
    //1.ȡ���û���������
    QString username = ui->username->text();
    QString password = ui->password->text();

    //��ʹ��json��װʱ��passwd����ǩ��
    QCryptographicHash md5_hash(QCryptographicHash::Md5);
    md5_hash.addData(password.toUtf8());
    QString md5_passwd(md5_hash.result().toHex());
    //ʹ��json�������ݷ�װ
    QJsonObject json;
    json["username"] = username;
    json["password"] = md5_passwd;

    json["opt"] = _opt;
    //����װ�õ�json������ӵ�json�ĵ����Ա㷢��
    QJsonDocument json_doc(json);
    QString output= json_doc.toJson();


    //2.���û���������ƴ��Ϊ��������
    //��1����������ͷ
    QNetworkRequest req(QUrl("http://192.168.144.183/login/"));
    req.setHeader(QNetworkRequest::ContentLengthHeader,output.size());
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    //3.��������¼������
    //��2��������ͨ��postģʽ����,�������ת����ʽ

    net_rep= net_mng.post(req,output.toUtf8());
    //4.�첽����������Ļظ�
    connect(net_rep,SIGNAL(finished()),this,SLOT(proc_login_rep()));
}
void MainWindow::Req_login(QString _opt,QString _room_no)
{

    //1.ȡ���û���������
    QString username = ui->username->text();
    QString password = ui->password->text();

    //��ʹ��json��װʱ��passwd����ǩ��
    QCryptographicHash md5_hash(QCryptographicHash::Md5);
    md5_hash.addData(password.toUtf8());
    QString md5_passwd(md5_hash.result().toHex());
    //ʹ��json�������ݷ�װ
    QJsonObject json;
    json["username"] = username;
    json["password"] = md5_passwd;

    json["opt"] = _opt;
    json["room_no"] = _room_no;
    //����װ�õ�json������ӵ�json�ĵ����Ա㷢��
    QJsonDocument json_doc(json);
    QString output= json_doc.toJson();


    //2.���û���������ƴ��Ϊ��������
    //��1����������ͷ
    QNetworkRequest req(QUrl("http://192.168.144.183/login/"));
    req.setHeader(QNetworkRequest::ContentLengthHeader,output.size());
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    //3.��������¼������
    //��2��������ͨ��postģʽ����,�������ת����ʽ

    net_rep= net_mng.post(req,output.toUtf8());
    //4.�첽����������Ļظ�
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
    //���շ������Ļظ�
    //1.�������ظ�Ϊ�ַ������޷�����Ľ���,��ʹ��json������
    //(1).����doc���󣬽��ַ����������ת����
    QJsonDocument reply = QJsonDocument::fromJson(net_rep->readAll());
    //(2).����json���󣬽�doc��json����ȡ����ֵ
    QJsonObject json = reply.object();
    //(3).ȡ��json�����н�������ֵ�������ж�
    if("OK"==json["login_result"].toString())//ע����json���ַ�������json������Ҫ����ת��
    {
        if(m_cur_opt==create_room)
        {
            //��¼�ɹ������ֱ���˳��˽��棬��������Ϸ����
            //��Ϸ����Ҫ�����ڵ�¼���������
            qDebug()<<"login success";
            //1.����QProcess����ʹ��startDetached���������н��̶���
            QProcess proc;
            //2.��startDetached�����ڶ����������ֵ����ʼ��Ϊip�˿ڣ�����client�򿪱���Ҫ�е�
            QStringList argv;
            int room_no = json["room_no"].toInt();

            QMessageBox::information(this, "�����","����ţ�"+QString::number(room_no));

            argv.push_back(json["server_IP"].toString());
            argv.push_back(json["server_PORT"].toString());
            proc.startDetached("client.exe",argv,".");
            //3.����Ϸ������˳���¼��
            //ʹ��ָ��QApplication �� QGuiApplication��QCoreApplication�������qAppȫ��ָ��
            qApp->exit();
        }
        else if(m_cur_opt==follow_room)
        {
            if(	"OK"==json["follow_result"].toString())
            {
                qDebug()<<"login success";
                //1.����QProcess����ʹ��startDetached���������н��̶���
                QProcess proc;
                //2.��startDetached�����ڶ����������ֵ����ʼ��Ϊip�˿ڣ�����client�򿪱���Ҫ�е�
                QStringList argv;
                QString room_no = ui->room_no->text();
                argv.push_back(json["server_IP"].toString());
                argv.push_back(json["server_PORT"].toString());
                proc.startDetached("client.exe",argv,".");
                //3.����Ϸ������˳���¼��
                //ʹ��ָ��QApplication �� QGuiApplication��QCoreApplication�������qAppȫ��ָ��
                qApp->exit();
            }
            else
            {
                QMessageBox::information(this, "����ʧ��", "���䲻����");
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
            QMessageBox::information(this,"�޸�����ʧ��","����ȷ��д�û����������");
        }
        else{
            //��¼ʧ��,��������
            QMessageBox::information(this,"��¼ʧ��","�û������������");
        }

    }


}

//ʹ��QDesktopServices����ֱ����ת��ע�����
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

