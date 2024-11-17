#pragma once
#include<string>
using namespace std;
class User
{
protected:
	string username;
	string diachi;
	string mail;
public:
	virtual void ShowInfo() = 0;
	virtual void AddAccount() = 0;
};

