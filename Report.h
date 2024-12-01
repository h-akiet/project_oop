#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <iomanip>
#include <vector>
#include <sstream>
#include <algorithm>
#include "Transaction.h"
#include "Budget.h"
#include "Loan.h"

using namespace std;

struct Date
{
    int day;
    int month;
    int year;
};

// Hàm hỗ trợ để so sánh ngày
bool isDateInRange(const Date &date, const Date &startDate, const Date &endDate)
{
    if ((date.year > startDate.year ||
         (date.year == startDate.year && (date.month > startDate.month ||
                                          (date.month == startDate.month && date.day >= startDate.day)))) &&
        (date.year < endDate.year ||
         (date.year == endDate.year && (date.month < endDate.month ||
                                        (date.month == endDate.month && date.day <= endDate.day)))))
    {
        return true;
    }
    return false;
}

// Hàm chuyển đổi từ tm sang chuỗi (YYYY-MM-DD)
string tmToString(const std::tm &t)
{
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &t);
    return string(buffer);
}

// Hàm chuyển chuỗi thành tm
void stringToTm(const string &dateStr, std::tm &t)
{
    std::istringstream ss(dateStr);
    ss >> std::get_time(&t, "%Y-%m-%d");
    if (ss.fail())
    {
        std::cerr << "Failed to parse date: " << dateStr << std::endl;
    }
}

class Report
{
private:
    int reportID;
    int userID;
    string type; // Loại báo cáo
    Date startDate;
    Date endDate;

public:
    // Constructor
    Report()
    {
        this->reportID = 0;
        this->userID = 0;
        this->type = "";
        this->startDate = {0, 0, 0};
        this->endDate = {0, 0, 0};
    }

    // Hàm tạo báo cáo giao dịch
    string generateReport_Transaction()
    {
        cout << "Generating Transaction Report...\n"; // Thông báo bắt đầu tạo báo cáo giao dịch

        list<Transaction> transactions;      // Danh sách chứa các giao dịch
        ifstream infile("transactions.csv"); // Mở file chứa dữ liệu giao dịch

        if (!infile.is_open()) // Nếu không mở được file
        {
            cerr << "Error opening transactions file.\n"; // In ra lỗi
            return "Failed to generate report.";          // Trả về thông báo lỗi
        }

        string line;                  // Biến chứa từng dòng dữ liệu từ file
        while (getline(infile, line)) // Đọc từng dòng trong file
        {
            stringstream ss(line); // Chuyển dòng thành stream
            string ID, AccountID, Type, Amount, dateStr, Note, Category;

            // Tách các trường trong dòng dữ liệu
            getline(ss, ID, ',');
            getline(ss, AccountID, ',');
            getline(ss, Type, ',');
            getline(ss, Amount, ',');
            getline(ss, dateStr, ',');
            getline(ss, Note, ',');
            getline(ss, Category, ',');

            Transaction t;                 // Tạo đối tượng giao dịch
            t.transactionID = stoi(ID);    // Chuyển ID sang kiểu int
            t.accountID = stoi(AccountID); // Chuyển AccountID sang kiểu int
            t.transactionType = Type;      // Gán loại giao dịch
            t.amount = stod(Amount);       // Chuyển Amount sang kiểu double

            sscanf(dateStr.c_str(), "%d/%d/%d", &t.date.day, &t.date.month, &t.date.year); // Chuyển ngày sang dạng struct Date
            t.note = Note;                                                                 // Gán ghi chú
            t.category = Category;                                                         // Gán loại giao dịch

            // Kiểm tra xem giao dịch có nằm trong khoảng ngày không
            if (isDateInRange(t.date, startDate, endDate))
            {
                transactions.push_back(t); // Thêm giao dịch vào danh sách
            }
        }
        infile.close(); // Đóng file

        if (transactions.empty()) // Nếu không có giao dịch nào trong phạm vi ngày
        {
            return "No transactions found within the specified range."; // Trả về thông báo không có giao dịch
        }

        ostringstream report; // Tạo một chuỗi báo cáo
        report << "Transaction Report from " << startDate.day << "/" << startDate.month << "/" << startDate.year
               << " to " << endDate.day << "/" << endDate.month << "/" << endDate.year << "\n";
        report << left << setw(10) << "ID"
               << setw(10) << "AccID"
               << setw(15) << "Type"
               << setw(10) << "Amount"
               << setw(12) << "Date"
               << setw(20) << "Note"
               << setw(15) << "Category" << "\n";
        report << string(90, '-') << "\n"; // Dòng gạch ngang để phân cách

        // Ghi chi tiết các giao dịch vào báo cáo
        for (const auto &t : transactions)
        {
            report << left << setw(10) << t.transactionID
                   << setw(10) << t.accountID
                   << setw(15) << t.transactionType
                   << setw(10) << fixed << setprecision(2) << t.amount
                   << setw(2) << t.date.day << "/" << setw(2) << t.date.month << "/" << t.date.year
                   << setw(20) << t.note
                   << setw(15) << t.category << "\n";
        }
        cout << report.str();                                // In báo cáo ra màn hình
        return "Transaction report generated successfully."; // Trả về thông báo thành công
    }

    // Hàm tạo báo cáo vay
    string generateReport_Loan()
    {
        cout << "Generating Loan Report...\n"; // Thông báo bắt đầu tạo báo cáo vay

        list<Loan> loans;            // Danh sách chứa các khoản vay
        ifstream infile("LOAN.csv"); // Mở file chứa dữ liệu vay

        if (!infile.is_open()) // Nếu không mở được file
        {
            cerr << "Error opening loans file.\n"; // In ra lỗi
            return "Failed to generate report.";   // Trả về thông báo lỗi
        }

        string line;                  // Biến chứa từng dòng dữ liệu từ file
        while (getline(infile, line)) // Đọc từng dòng trong file
        {
            stringstream ss(line); // Chuyển dòng thành stream
            string loanID, userID, amount, interestRate, startDateStr, dueDateStr, note, status;

            // Tách các trường trong dòng dữ liệu
            getline(ss, loanID, ',');
            getline(ss, userID, ',');
            getline(ss, amount, ',');
            getline(ss, interestRate, ',');
            getline(ss, startDateStr, ',');
            getline(ss, dueDateStr, ',');
            getline(ss, note, ',');
            getline(ss, status, ',');

            Loan loan;                              // Tạo đối tượng khoản vay
            loan.loanID = stoi(loanID);             // Chuyển loanID sang kiểu int
            loan.userID = stoi(userID);             // Chuyển userID sang kiểu int
            loan.amount = stod(amount);             // Chuyển amount sang kiểu double
            loan.interestRate = stod(interestRate); // Chuyển interestRate sang kiểu double

            sscanf(startDateStr.c_str(), "%d/%d/%d", &loan.startDate.day, &loan.startDate.month, &loan.startDate.year); // Chuyển ngày bắt đầu sang struct Date
            sscanf(dueDateStr.c_str(), "%d/%d/%d", &loan.dueDate.day, &loan.dueDate.month, &loan.dueDate.year);         // Chuyển ngày đến hạn sang struct Date

            loan.note = note;     // Gán ghi chú
            loan.status = status; // Gán trạng thái

            // Kiểm tra xem khoản vay có nằm trong phạm vi ngày không
            if (isDateInRange(loan.startDate, startDate, endDate) || isDateInRange(loan.dueDate, startDate, endDate))
            {
                loans.push_back(loan); // Thêm khoản vay vào danh sách
            }
        }
        infile.close(); // Đóng file

        if (loans.empty()) // Nếu không có khoản vay nào trong phạm vi ngày
        {
            return "No loans found within the specified range."; // Trả về thông báo không có khoản vay
        }

        ostringstream report; // Tạo một chuỗi báo cáo
        report << "Loan Report from " << startDate.day << "/" << startDate.month << "/" << startDate.year
               << " to " << endDate.day << "/" << endDate.month << "/" << endDate.year << "\n";
        report << left << setw(10) << "Loan ID"
               << setw(10) << "User ID"
               << setw(15) << "Amount"
               << setw(15) << "Interest Rate"
               << setw(15) << "Start Date"
               << setw(15) << "Due Date"
               << setw(20) << "Note"
               << setw(10) << "Status" << "\n";
        report << string(90, '-') << "\n"; // Dòng gạch ngang để phân cách

        // Ghi chi tiết các khoản vay vào báo cáo
        for (const Loan &loan : loans)
        {
            report << left << setw(10) << loan.loanID
                   << setw(10) << loan.userID
                   << setw(15) << fixed << setprecision(2) << loan.amount
                   << setw(15) << loan.interestRate
                   << setw(2) << loan.startDate.day << "/" << setw(2) << loan.startDate.month << "/" << loan.startDate.year
                   << setw(2) << loan.dueDate.day << "/" << setw(2) << loan.dueDate.month << "/" << loan.dueDate.year
                   << setw(20) << loan.note
                   << setw(10) << loan.status << "\n";
        }
        cout << report.str();                         // In báo cáo ra màn hình
        return "Loan report generated successfully."; // Trả về thông báo thành công
    }

    // Hàm tạo báo cáo ngân sách
    string generateReport_Budget(const string &startDateStr, const string &endDateStr)
    {
        cout << "Generating Budget Report...\n";

        vector<Budget> budgets;
        ifstream infile("Budgets.csv");

        if (!infile.is_open())
        {
            cerr << "Error opening budgets file.\n";
            return "Failed to generate report.";
        }

        tm startDate = {};
        tm endDate = {};
        stringToTm(startDateStr, startDate);
        stringToTm(endDateStr, endDate);

        string line;
        while (getline(infile, line))
        {
            stringstream ss(line);
            string field;
            Budget tempBudget;
            string tempStartDate, tempEndDate;

            // Đọc các trường trong file
            getline(ss, field, ',');
            tempBudget.budgetID = stoi(field);
            getline(ss, field, ',');
            tempBudget.userID = stoi(field);
            getline(ss, field, ',');
            tempBudget.amount = stod(field);
            getline(ss, field, ',');
            tempBudget.category = field;
            getline(ss, tempStartDate, ',');
            getline(ss, tempEndDate, ',');

            stringToTm(tempStartDate, startDate);
            stringToTm(tempEndDate, endDate);

            budgets.push_back(tempBudget);
        }
        infile.close();

        // Tạo báo cáo ngân sách
        ostringstream report;
        report << "Budget Report from " << startDateStr << " to " << endDateStr << "\n";
        report << left << setw(10) << "Budget ID"
               << setw(10) << "User ID"
               << setw(10) << "Amount"
               << setw(15) << "Category" << "\n";

        report << string(45, '-') << "\n";
        for (const auto &budget : budgets)
        {
            report << left << setw(10) << budget.budgetID
                   << setw(10) << budget.userID
                   << setw(10) << budget.amount
                   << setw(15) << budget.category << "\n";
        }
        cout << report.str(); // In báo cáo ra màn hình
        return "Budget report generated successfully.";
    }
    string exportedReport(const string &reportType, const string &startDateStr, const string &endDateStr)
    {
        string reportContent;
        if (reportType == "Transaction")
        {
            reportContent = generateReport_Transaction(); // Lấy báo cáo giao dịch
        }
        else if (reportType == "Loan")
        {
            reportContent = generateReport_Loan(); // Lấy báo cáo vay
        }
        else if (reportType == "Budget")
        {
            reportContent = generateReport_Budget(startDateStr, endDateStr); // Lấy báo cáo ngân sách
        }
        else
        {
            return "Invalid report type specified.";
        }

        if (reportContent.find("generated successfully") != string::npos) // Nếu báo cáo đã được tạo thành công
        {
            ofstream outfile("report.txt"); // Mở tệp để ghi báo cáo
            if (outfile.is_open())
            {
                outfile << reportContent; // Ghi nội dung báo cáo vào tệp
                outfile.close();          // Đóng tệp
                return "Report exported to report.txt successfully.";
            }
            else
            {
                return "Failed to open file for exporting report.";
            }
        }
        else
        {
            return "Report generation failed, unable to export.";
        }
    }
};
