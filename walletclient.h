#ifndef WALLETCLIENT_H
#define WALLETCLIENT_H

#include "client.h"

class WalletClient
{
public:
    WalletClient();
    void main();
    bool createAccount();
    bool login();
    bool withdraw();
    bool deposit();
    bool getAmount();
    bool getTransactions(QString type);
    bool undo();
    bool redo();
    ~WalletClient();

private:
    Client * client;
};

#endif // WALLETCLIE_H
