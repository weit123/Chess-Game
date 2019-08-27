#include "connectdialog.h"
#include "ui_connectdialog.h"
#include <QNetworkInterface>
#include <QDebug>
#include <QMessageBox>

connectDialog::connectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::connectDialog)
{
    ui->setupUi(this);

    QList<QNetworkInterface> network = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface i, network) {
        QList<QNetworkAddressEntry> ipAll = i.addressEntries();
        foreach (QNetworkAddressEntry ip, ipAll) {
            if(ip.ip().protocol()==QAbstractSocket::IPv4Protocol)
                localIP = ip.ip().toString();
        }
    }
    inputIP = localIP;
    ui->serverButton->setChecked(true);
    ui->clientButton->setChecked(false);
    ui->ipEdit->setText(localIP);
    ui->ipEdit->setDisabled(true);
    connect(ui->serverButton, SIGNAL(clicked()), this, SLOT(serverClicked()));
    connect(ui->clientButton, SIGNAL(clicked()), this, SLOT(clientClicked()));
    connect(ui->ipEdit, SIGNAL(textChanged(QString)), this, SLOT(inputIPChanged(QString)));
    connect(ui->portEdit, SIGNAL(textChanged(QString)), this, SLOT(portChanged(QString)));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelConnection()));
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(checkFormat()));
}

connectDialog::~connectDialog() { delete ui; }

void connectDialog::serverClicked() {
    ui->clientButton->setChecked(!ui->serverButton->isChecked());
    ui->ipEdit->setText(localIP);
    ui->ipEdit->setDisabled(true);
}

void connectDialog::clientClicked() {
    ui->serverButton->setChecked(!ui->clientButton->isChecked());
    ui->ipEdit->setEnabled(true);
}

void connectDialog::inputIPChanged(QString content) {
    inputIP = content;
}
void connectDialog::portChanged(QString content) {
    portNumber = content;
}

void connectDialog::cancelConnection() {
    //
}
void connectDialog::checkFormat() {
    if(ui->serverButton->isChecked())
        inputIP = localIP;
    else {
        int ip1, ip2, ip3, ip4;
        if(sscanf(inputIP.toLatin1().data(), "%d.%d.%d.%d", &ip1, &ip2, &ip3, &ip4) != 4) {
            QMessageBox::critical(this, "Error", "Error: IP address invalid!");
            return;

        }
        else if(ip1 < 0 || ip1 > 255 || ip2 < 0 || ip2 > 255 || ip3 < 0 || ip3 > 255 || ip4 < 0 || ip4 > 255) {
            QMessageBox::critical(this, "Error", "Error: IP address invalid!");
            return;
        }
    }
    const char *s = portNumber.toUtf8().data();
    while(*s && *s>='0' && *s<='9') s++;
    if(bool(*s) == true) {
        QMessageBox::critical(this, "Error", "Error: Port number invalid!");
        return;
    }
    int num;
    sscanf(portNumber.toLatin1().data(), "%d", &num);
    qDebug() << "port:" << num;
    if(num < 0 || num > 65535) {
        QMessageBox::critical(this, "Error", "Error: Port number invalid!");
        return;
    }
    startConnection();
}

void connectDialog::startConnection() {
    mode = (ui->clientButton->isChecked()) ? 1 : 0;
    QString address = QString::number(mode) + " " + inputIP + ":" + portNumber;
    qDebug() << address;
    emit ipAddress(address);
    accept();
}