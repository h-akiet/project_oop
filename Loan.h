#pragma once

#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <sstream>
#include <iomanip> // Định dạng đầu ra

using namespace std;

struct Date
{
    int day;
    int month;
    int year;
};

class Loan
{
private:
    int loanID;
    int userID;
    double amount;
    double interestRate;
    Date startDate;
    Date dueDate;
    string note;
    string status;

public:
    // Hàm khởi tạo
    Loan()
    {
        this->loanID = 0;
        this->userID = 0;
        this->amount = 0.0;
        this->interestRate = 0.0;
        this->startDate = {0, 0, 0};
        this->dueDate = {0, 0, 0};
        this->note = "";
        this->status = "";
    }

    void addLoan() // Cho phép người dùng thêm khoản vay mới
    {
        cout << "Enter loan details:\n";

        cout << "Loan ID: ";
        cin >> this->loanID;

        cout << "User ID: ";
        cin >> this->userID;

        cout << "Loan Amount: ";
        cin >> this->amount;

        cout << "Interest Rate (%): ";
        cin >> this->interestRate;

        cout << "Start Date (YYYY-MM-DD): ";
        cin >> this->startDate.day >> this->startDate.month >> this->startDate.year;

        cout << "Due Date (YYYY-MM-DD): ";
        cin >> this->dueDate.day >> this->dueDate.month >> this->dueDate.year;

        cout << "Note: ";
        cin.ignore();
        getline(cin, this->note);

        cout << "Status (e.g., Active, Completed): ";
        getline(cin, this->status);

        // Ghi dữ liệu vào file CSV
        ofstream outfile("LOAN.csv", ios::app); // Mở file ghi
        if (outfile.is_open())
        {
            outfile << this->loanID << "," << this->userID << "," << fixed << setprecision(2) << this->amount << "," << this->interestRate << ","
                    << this->startDate.day << "/" << this->startDate.month << "/" << this->startDate.year << ","
                    << this->dueDate.day << "/" << this->dueDate.month << "/" << this->dueDate.year << ","
                    << this->note << "," << this->status << "\n";
            outfile.close();
            cout << "Loan added successfully!\n";
        }
        else
        {
            cerr << "Error opening file for writing.\n";
        }
    }

    void editLoan() // Cho phép người dùng chỉnh sửa nội dung khoản vay nếu ID khoản vay có tồn tại
    {
        cout << "Enter the Loan ID to edit: ";
        cin >> this->loanID;

        ifstream infile("LOAN.csv");   // File gốc
        ofstream tempFile("temp.csv"); // File tạm dùng để lưu trữ những dữ liệu mới sau khi cập nhật
        bool isEdited = false;

        if (infile.is_open() && tempFile.is_open())
        {
            string line;
            while (getline(infile, line))
            {
                stringstream ss(line);
                string ID;
                getline(ss, ID, ',');

                if (stoi(ID) == this->loanID) // Tìm được ID vay
                {
                    // Sửa những thông tin muốn sửa
                    cout << "Editing loan with ID: " << this->loanID << endl;
                    cout << "Enter new details:" << endl;

                    cout << "User ID: ";
                    cin >> this->userID;

                    cout << "Loan Amount: ";
                    cin >> this->amount;

                    cout << "Interest Rate (%): ";
                    cin >> this->interestRate;

                    cout << "Start Date (YYYY-MM-DD): ";
                    cin >> this->startDate.day >> this->startDate.month >> this->startDate.year;

                    cout << "Due Date (YYYY-MM-DD): ";
                    cin >> this->dueDate.day >> this->dueDate.month >> this->dueDate.year;

                    cout << "Note: ";
                    getline(cin, this->note);

                    cout << "Status: ";
                    getline(cin, this->status);

                    // Ghi lại những thông tin vừa sửa vào file tạm
                    tempFile << this->loanID << "," << this->userID << "," << fixed << setprecision(2) << this->amount << "," << this->interestRate << "," 
                             << this->startDate.day << "/" << this->startDate.month << "/" << this->startDate.year << ","
                             << this->dueDate.day << "/" << this->dueDate.month << "/" << this->dueDate.year << "," 
                             << this->note << "," << this->status << "\n";

                    isEdited = true;
                }
                else
                {
                    tempFile << line << endl;
                }
            }
            infile.close();
            tempFile.close();

            remove("LOAN.csv");
            rename("temp.csv", "LOAN.csv");

            if (isEdited)
            {
                cout << "Loan edited successfully!\n";
            }
            else
            {
                cerr << "Loan ID not found.\n";
            }
        }
        else
        {
            cerr << "Error opening file.\n";
        }
    }

    void deleteLoan() // Cho phép người dùng cho xoá một khoản vay
    {
        cout << "Enter the Loan ID to delete: ";
        cin >> this->loanID;

        ifstream infile("LOAN.csv");
        ofstream tempFile("temp.csv");
        bool isDeleted = false;

        if (infile.is_open() && tempFile.is_open())
        {
            string line;
            while (getline(infile, line))
            {
                stringstream ss(line);
                string ID;
                getline(ss, ID, ',');

                if (stoi(ID) != this->loanID)
                {
                    tempFile << line << endl;
                    ;
                }
                else
                {
                    isDeleted = true;
                }
            }
            infile.close();
            tempFile.close();

            remove("LOAN.csv");
            rename("temp.csv", "LOAN.csv");

            if (isDeleted)
            {
                cout << "Loan deleted successfully!\n";
            }
            else
            {
                cerr << "Loan ID not found.\n";
            }
        }
        else
        {
            cerr << "Error opening file.\n";
        }
    }

    list<Loan> viewLoanHistory()
    {
        list<Loan> loans;
        ifstream infile("LOAN.csv");

        if (infile.is_open())
        {
            string line;
            while (getline(infile, line))
            {
                stringstream ss(line);
                string ID, UserID, Amount, InterestRate, startDateStr, dueDateStr, Note, Status;

                getline(ss, ID, ',');
                getline(ss, UserID, ',');
                getline(ss, Amount, ',');
                getline(ss, InterestRate, ',');
                getline(ss, startDateStr, ',');
                getline(ss, dueDateStr, ',');
                getline(ss, Note, ',');
                getline(ss, Status, ',');

                Loan loan;
                loan.loanID = stoi(ID);
                loan.userID = stoi(UserID);
                loan.amount = stod(Amount);
                loan.interestRate = stod(InterestRate);

                // Chuyển đổi chuỗi ngày thành Date
                sscanf(startDateStr.c_str(), "%d/%d/%d", &loan.startDate.day, &loan.startDate.month, &loan.startDate.year);

                // Chuyển đổi chuỗi ngày thành Date
                sscanf(dueDateStr.c_str(), "%d/%d/%d", &loan.dueDate.day, &loan.dueDate.month, &loan.dueDate.year);

                loan.note = Note;
                loan.status = Status;

                loans.push_back(loan);
            }
            infile.close();
        }
        else
        {
            cerr << "Error opening file.\n";
        }
        return loans;
    }

    double calculateInterest() //Tính tiền lãi
    {
        cout << "Enter Loan ID to calculate interest: "; //Nhập ID để tìm ID khoản vay trong file LOAN 
        cin >> this->loanID;

        ifstream infile("LOAN.csv");
        ofstream tempFile("temp.csv");
        bool isUpdated = false;

        if (infile.is_open() && tempFile.is_open()) 
        {
            string line;
            while (getline(infile, line))
            {
                stringstream ss(line);
                string ID, UserID, Amount, InterestRate, StartDate, DueDate, Note, Status;

                getline(ss, ID, ',');
                getline(ss, UserID, ',');
                getline(ss, Amount, ',');
                getline(ss, InterestRate, ',');
                getline(ss, StartDate, ',');
                getline(ss, DueDate, ',');
                getline(ss, Note, ',');
                getline(ss, Status, ',');

                if (stoi(ID) == this->loanID)
                {
                    this->amount = stod(Amount);
                    this->interestRate = stod(InterestRate);

                    // Tính toán lãi suất
                    double interest = (this->amount * this->interestRate) / 100;
                    cout << "Calculated Interest: $" << fixed << setprecision(2) << interest << "\n";

                    // Cập nhật dòng với thông tin mới
                    tempFile << ID << "," << UserID << "," << fixed << setprecision(2) << this->amount << "," << this->interestRate << ","
                             << this->startDate.day << "/" << this->startDate.month << "/" << this->startDate.year << ","
                             << this->dueDate.day << "/" << this->dueDate.month << "/" << this->dueDate.year << "," << Note << ","
                             << Status << ",Interest:" << fixed << setprecision(2) << interest << "\n";

                    isUpdated = true;
                }
                else
                {
                    tempFile << line << endl;
                }
            }
            infile.close();
            tempFile.close();

            // Ghi lại file
            remove("LOAN.csv");
            rename("temp.csv", "LOAN.csv");

            if (isUpdated) // Sau khi thêm mục mới là Interest (tiền lãi) thành công thì hiển thị
            {
                cout << "Interest updated in CSV file.\n";
            }
            else
            {
                cerr << "Loan ID not found.\n";
            }
        }
        else
        {
            cerr << "Error opening file.\n";
        }
        return 0.0;
    }
};
