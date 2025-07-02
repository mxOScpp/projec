#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QtNetwork/QNetworkRequest>
#include<QtNetwork/QNetworkReply>
#include<QtNetwork/QNetworkAccessManager>
#include<QLineEdit>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void SetUsername(QString _username);

private slots:
    void on_buttonBox_accepted();
void clearLineEdit();
private:
    Ui::Dialog *ui;
    QString username;
    QNetworkAccessManager net_mng;
};

#endif // DIALOG_H
