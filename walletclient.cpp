#include "walletclient.h"

static QTextStream out(stdout);
static QTextStream in(stdin);
static QString reply;

WalletClient::WalletClient()
{
    this->client = new Client();
    bool connected = false;
    while (!connected)
    {
        if (this->client->connectToHost())
        {
            connected = true;
        }
        else
        {
            connected = false;
            out << "Could not connect to server! \n";
            out << "Try to connect again? (y:n) ";
            out.flush();
            in >> reply;
            if (reply.simplified() == "y" || reply.simplified() == "")
            {
                continue;
            }
            else
            {
                exit(0);
            }
        }
    }
    this->main();
}

void WalletClient::main()
{
    out << "Welcome to Smart Wallet Server \n";
    out << "You can exit at any time by Ctrl + c \n";
    out << "Enter the number of any action to perform it \n";
    while(1)
    {
        out << "1) Create new account \n";
        out << "2) Login to existing account \n";
        out.flush();
        in >> reply;
        if (reply.simplified() == "1")
        {
            if (!this->createAccount())
            {
                continue;
            }
            break;
        }
        else if (reply.simplified() == "2")  {
            if (!this->login())
            {
                continue;
            }
            break;
        }
        else
        {
            out << "Wrong Input \n";
            out.flush();
        }
    }
    while(1)
    {
        out << "1) Deposit \n";
        out << "2) Withdraw \n";
        out << "3) Get Amount \n";
        out << "4) Undo Transaction \n";
        out << "5) Redo Transaction \n";
        out.flush();
        in >> reply;
        if (reply.simplified() == "1")
        {
            this->deposit();
            continue;
        }
        else if (reply.simplified() == "2")  {
            this->withdraw();
            continue;
        }
        else if (reply.simplified() == "3")  {
            this->getAmount();
            continue;
        }
        else if (reply.simplified() == "4")  {
            this->undo();
            continue;
        }
        else if (reply.simplified() == "5")  {
            this->redo();
            continue;
        }
        else
        {
            out << "Wrong Input \n";
            out.flush();
        }
    }
}

bool WalletClient::createAccount()
{
    QString name,age,national_id,email,mobile_number;
    out << "Enter your name: \n";
    out.flush();
    while(1)
    {
        reply = in.readLine();
        name = reply.simplified();
        if (name == "")
        {
            continue;
        }
        if (!QRegExp("([A-Za-z]|\\s|\\.)+").exactMatch(name))
        {
            out << "Name consists of letters only with full stops or white spaces \n";
            out.flush();
        }
        else
        {
            break;
        }
    }
    out << "Enter your age: \n";
    out.flush();
    while(1)
    {
        in >> reply;
        age = reply.simplified();
        if (age == "")
        {
            continue;
        }
        if (!QRegExp("\\d{1,3}").exactMatch(age))
        {
            out << "Age consists of numbers only with max of three numbers \n";
            out.flush();
        }
        else
        {
            break;
        }
    }
    out << "Enter your national id number: \n";
    out.flush();
    while(1)
    {
        in >> reply;
        national_id = reply.simplified();
        if (national_id == "")
        {
            continue;
        }
        if (!QRegExp("\\d*").exactMatch(age))
        {
            out << "National id consists of numbers only \n";
            out.flush();
        }
        else
        {
            break;
        }
    }
    out << "Enter your email: \n";
    out.flush();
    QRegExp mailREX("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
    mailREX.setCaseSensitivity(Qt::CaseInsensitive);
    mailREX.setPatternSyntax(QRegExp::RegExp);
    while(1)
    {
        in >> reply;
        email = reply.simplified();
        if (email == "")
        {
            continue;
        }
        if (!mailREX.exactMatch(email))
        {
            out << "Not a valid email \n";
            out.flush();
        }
        else
        {
            break;
        }
    }
    out << "Enter your mobile number: \n";
    out.flush();
    while(1)
    {
        in >> reply;
        mobile_number = reply.simplified();
        if (mobile_number == "")
        {
            continue;
        }
        if (!QRegExp("^\\+?(\\d*)").exactMatch(mobile_number))
        {
            out << "Not a valid mobile number \n";
            out.flush();
        }
        else
        {
            break;
        }
    }
    QString message = "c " + name + "," + age + "," + national_id + "," + email + "," + mobile_number;
    bool success;
    QString reply = this->client->write(message,success);
    if (success)
    {
        out << "Your account has been created! \n";
        out << "Your account amount: 0\n";
        out << "Account id: " << reply + "\n";
        out.flush();
        return true;
    }
    else {
        out << "Error: " << reply + "\n";
        out.flush();
        return false;
    }
}

bool WalletClient::login()
{
    QString account_id;
    out << "Enter your account id: \n";
    out.flush();
    while(1)
    {
        in >> reply;
        account_id = reply.simplified();
        if (account_id == "")
        {
            continue;
        }
        if (!QRegExp("\\d*").exactMatch(account_id))
        {
            out << "Account id consists of numbers only \n";
            out.flush();
        }
        else
        {
            break;
        }
    }
    QString message = "l " + account_id;
    bool success;
    QString reply = this->client->write(message,success);
    if (success)
    {
        out << "Login Successfully! \n";
        out << "Your account amount: " + reply + "\n";
        out.flush();
        return true;
    }
    else {
        out << "Error: " << reply + "\n";
        out.flush();
        return false;
    }
}

bool WalletClient::deposit()
{
    QString amount;
    out << "Enter your deposit amount: \n";
    out.flush();
    while(1)
    {
        in >> reply;
        amount = reply.simplified();
        if (amount == "")
        {
            continue;
        }
        if (!QRegExp("\\d*").exactMatch(amount))
        {
            out << "Amount consists of numbers only and not negative \n";
            out.flush();
        }
        else if (amount.toUInt() == 0)
        {
            out << "Amount cannot be 0 \n";
            out.flush();
        }
        else
        {
            break;
        }
    }
    QString message = "d " + amount;
    bool success;
    QString reply = this->client->write(message,success);
    if (success)
    {
        out << "Deposit Successfully! \n";
        out << "Your account amount: " + reply + "\n";
        out.flush();
        return true;
    }
    else {
        out << "Error: " << reply + "\n";
        out.flush();
        return false;
    }
}

bool WalletClient::withdraw()
{
    QString amount;
    out << "Enter your withdraw amount: \n";
    out.flush();
    while(1)
    {
        in >> reply;
        amount = reply.simplified();
        if (amount == "")
        {
            continue;
        }
        if (!QRegExp("\\d*").exactMatch(amount))
        {
            out << "Amount consists of numbers only and not negative \n";
            out.flush();
        }
        else if (amount.toUInt() == 0)
        {
            out << "Amount cannot be 0 \n";
            out.flush();
        }
        else
        {
            break;
        }
    }
    QString message = "w " + amount;
    bool success;
    QString reply = this->client->write(message,success);
    if (success)
    {
        out << "Withdraw Successfully! \n";
        out << "Your account amount: " + reply + "\n";
        out.flush();
        return true;
    }
    else {
        out << "Error: " << reply + "\n";
        out.flush();
        return false;
    }
}

bool WalletClient::getAmount()
{
    QString message = "g ";
    bool success;
    QString reply = this->client->write(message,success);
    if (success)
    {
        out << "Your account amount: " + reply + "\n";
        out.flush();
        return true;
    }
    else {
        out << "Error: " << reply + "\n";
        out.flush();
        return false;
    }
}

bool WalletClient::getTransactions(QString type)
{
    QString message = "t " + type;
    bool success;
    QString reply = this->client->write(message,success);
    if (success)
    {
        out.setFieldWidth(15);
        out.setFieldAlignment(QTextStream::AlignCenter);
        out.setPadChar(' ');
        out << "ID" << "TYPE" << "AMOUNT";
        out.setFieldWidth(40);
        out << "Date";
        out.setFieldWidth(1);
        out << "\n";
        QStringList transactions_list = reply.split(',');
        for(int i = 0 ; i < transactions_list.size();i++)
        {
            QStringList transaction_variables = transactions_list[i].split(' ');
            out.setFieldWidth(15);
            out << transaction_variables[0] << transaction_variables[1] << transaction_variables[2];
            out.setFieldWidth(40);
            out << transaction_variables[3] + " " + transaction_variables[4] + " " + transaction_variables[5] + " " + transaction_variables[6] + " " + transaction_variables[7];
            out.setFieldWidth(1);
            out << "\n";
        }
        out.flush();
        out.reset();
        return true;
    }
    else {
        out << "Error: " << reply + "\n";
        out.flush();
        return false;
    }
}

bool WalletClient::undo()
{
    QString transaction_id;
    if (this->getTransactions("u"))
    {
        out << "Enter transaction id: \n";
        out.flush();
        while(1)
        {
            in >> reply;
            transaction_id = reply.simplified();
            if (transaction_id == "")
            {
                continue;
            }
            if (!QRegExp("\\d*").exactMatch(transaction_id))
            {
                out << "Id consists of numbers only and not negative \n";
                out.flush();
            }
            else
            {
                break;
            }
        }
        QString message = "u " + transaction_id;
        bool success;
        QString reply = this->client->write(message,success);
        if (success)
        {
            out << "Transaction undone Successfully! \n";
            out << "Your account amount: " + reply + "\n";
            out.flush();
            return true;
        }
        else {
            out << "Error: " << reply + "\n";
            out.flush();
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool WalletClient::redo()
{
    QString transaction_id;
    if (this->getTransactions("r"))
    {
        out << "Enter transaction id: \n";
        out.flush();
        while(1)
        {
            in >> reply;
            transaction_id = reply.simplified();
            if (transaction_id == "")
            {
                continue;
            }
            if (!QRegExp("\\d*").exactMatch(transaction_id))
            {
                out << "Id consists of numbers only and not negative \n";
                out.flush();
            }
            else
            {
                break;
            }
        }
        QString message = "r " + transaction_id;
        bool success;
        QString reply = this->client->write(message,success);
        if (success)
        {
            out << "Transaction redone Successfully! \n";
            out << "Your account amount: " + reply + "\n";
            out.flush();
            return true;
        }
        else {
            out << "Error: " << reply + "\n";
            out.flush();
            return false;
        }
    }
    else
    {
        return false;
    }
}

WalletClient::~WalletClient()
{
    delete this->client;
}
