#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    this->socket = new QTcpSocket();
    connect(this->socket,SIGNAL(disconnected()),this,SLOT(disconnect()));
}

bool Client::connectToHost()
{
    this->socket->connectToHost("0.0.0.0",3500);
    this->socket->waitForConnected(3000);
    return this->socket->state() == QTcpSocket::ConnectedState;
}

void Client::disconnect()
{
    QTextStream out(stdout);
    out << "Server has closed \n";
    out.flush();
    exit(0);
}

QString Client::write(QString message, bool & success)
{
    message += ";";
    this->socket->write(message.toUtf8());
    this->socket->waitForBytesWritten(3000);
    this->socket->waitForReadyRead(3000);
    QByteArray response = this->socket->readAll();
    if (response[0] == 's')
        success = true;
    else
        success = false;
    QString reply = QString::fromUtf8(response).left(response.indexOf(';'));
    reply = reply.mid(2);
    return reply;
}

Client::~Client()
{
    this->socket->disconnectFromHost();
    this->socket->waitForDisconnected();
    delete this->socket;
}
