#pragma once
#include "Account.h"
#include<string>
#include<vector>
#include<iostream>
using namespace std;
class User :public Account
{
protected:
	string username;
	string diachi;
	string mail;
	vector <Account> listAccount;
public:
	void ShowInfo() override {
		//dung do hoa
		// show username, diachi, mail
		//listAccount[i].ShowInfo();
		
	}
	void AddAccount() override {
		Account tmp;
		tmp.AddAccount();
		this->listAccount.push_back(tmp);
	}
	void CreateUser()
	{
		getline(cin, this->username);
		getline(cin, this->diachi);
		getline(cin, this->mail);
		// them danh sach tai khoan
		this->AddAccount();
		
	}
};

