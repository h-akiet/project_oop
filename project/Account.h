#pragma once
#include<string>
using namespace std;
class Account 
{
protected:
    string password;
    string account_type;
    double balance;
public:
    void Authenic() {};
    void CreateTransaction() {};
    void Transfer() {};
    void ShowInfo() {};
};

