#pragma once
#include<string>
#include<ctime>
using namespace std;
class Transaction 
{
protected:
	string transfer_type;
	double amount_of_money;
	tm trans_date;  //khai bao bien du lieu kieu datetime
	/*
	*	std::cout << "Enter year: ";
		std::cin >> datetime.tm_year;
		std::cout << "Enter month (1-12): ";
		std::cin >> datetime.tm_mon;
		std::cout << "Enter day (1-31): ";
		std::cin >> datetime.tm_mday;
		std::cout << "You entered: ";
		std::cout << std::put_time(&datetime, "%d/%m/%Y") << "\n";
	*/
	string subject;
public:
	void Edit_Transaction() {};
	void Delete_Transaction() {};

};
class Debt
{
protected:
	double amount_of_debt;
	tm debt_date; //ngay vay
	tm due_date; //han tra
	float interest; //lai
public:
	void Update_Status() {};
	void Add_Payment() {};
};
class Loan
{
protected:
	double amount_of_loan;
	tm loan_date;
	tm due_date;
	float interest;
public:
	void Update_Status() {};
	void Payment_History() {};
};
class Reminder :public Debt, Loan
{
protected:
	string subject; //nd nhac nho
	tm reminder_date;
public:
	void Send_Reminder() {};

};

