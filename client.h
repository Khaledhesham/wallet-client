#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    bool connectToHost();
    QString write(QString message, bool & success);
    ~Client();

signals:

public slots:
    void disconnect();

private:
    QTcpSocket * socket;
};

#endif // CLIENT_H
