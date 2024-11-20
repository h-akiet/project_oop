#pragma once
#include<string>
#include"User.h"
using namespace std;
class Account 
{
protected:
    string password;
    string account_type;
    double balance;//so du
public:
    virtual void AddAccount() {
        getline(cin, this->password);
        getline(cin, this->account_type);
        cin >> balance;
    }
    void Authenic() {};
    void CreateTransaction() {};
    void Transfer() {};
    virtual void ShowInfo() {};
};
//test
test


