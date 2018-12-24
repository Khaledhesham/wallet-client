#include <QCoreApplication>
#include <QDebug>
#include "walletclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    WalletClient walletClient;

    return a.exec();
}
