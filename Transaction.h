#pragma once

#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <sstream> // Tách các giá trị trong một chuỗi
#include <vector>
#include <iomanip> // Định dạng đầu ra

using namespace std;

struct Date
{
    int day;
    int month;
    int year;
};

class Transaction
{
private:
    int transactionID;
    int accountID;
    string transactionType;
    double amount;
    Date date;
    string note;
    string category;
    string paymentMethod;

public:
    // Hàm khởi tạo
    Transaction()
    {
        this->transactionID = 0;
        this->accountID = 0;
        this->amount = 0.0;
        this->date = {0, 0, 0};
    }

    void addTransaction() // Cho phép người dùng thêm một giao dịch mới
    {
        cout << "Enter transaction details:\n";

        cout << "Transaction ID: ";
        cin >> this->transactionID;

        cout << "Account ID: ";
        cin >> this->accountID;

        cout << "Transaction Type: ";
        cin.ignore(); // Để loại bỏ ký tự thừa
        getline(cin, this->transactionType);

        cout << "Amount: ";
        cin >> this->amount;

        cout << "Enter transaction date (day month year): ";
        cin >> this->date.day >> this->date.month >> this->date.year;

        cout << "Note: ";
        cin.ignore(); // Để loại bỏ ký tự thừa
        getline(cin, this->note);

        cout << "Category: ";
        getline(cin, this->category);

        cout << "Payment Method (e.g., Cash, Credit): ";
        getline(cin, this->paymentMethod);

        ofstream outfile("transactions.csv", ios::app); // Mở file ở chế độ ghi
        if (outfile.is_open())                          // Nếu mở file thành công
        {
            outfile << this->transactionID << ","
                    << this->accountID << ","
                    << this->transactionType << ","
                    << fixed << setprecision(2) << this->amount << "," // Định dạng số dư đến chữ số thập phân thứ 2
                    << this->date.day << "/" << this->date.month << "/" << this->date.year << ","
                    << this->note << ","
                    << this->category << ","
                    << this->paymentMethod << "\n";
            outfile.close();
            cout << "Transaction added successfully!\n";
        }
        else // Nếu file không mở được
        {
            cerr << "Error opening file for writing.\n";
        }
    }

    void editTransaction() // Cho phép người dùng chỉnh sửa một giao dịch nếu ID giao dịch cần chỉnh sửa có tồn tại
    {
        cout << "Enter the transaction ID to edit: "; // Tìm kiếm ID giao dịch cần chỉnh sửa
        cin >> this->transactionID;

        ifstream infile("transactions.csv"); // file chính dùng để đọc
        ofstream tempFile("temp.csv");       // file ghi tạm
        bool isEdited = false;

        if (infile.is_open() && tempFile.is_open())
        {
            string line;
            while (getline(infile, line))
            {
                stringstream ss(line);
                string ID;
                getline(ss, ID, ',');

                if (stoi(ID) == transactionID) // Nếu trùng ID
                {
                    // Tiếp tuc sửa những nội dung muốn sửa
                    cout << "Editing transaction with ID: " << this->transactionID << endl;
                    cout << "Enter new details:" << endl;

                    cout << "Transaction ID: ";
                    cin >> this->transactionID;

                    cout << "Account ID: ";
                    cin >> this->accountID;

                    cout << "Transaction Type: ";
                    cin.ignore(); // Để loại bỏ ký tự thừa
                    getline(cin, this->transactionType);

                    cout << "Amount: ";
                    cin >> this->amount;

                    cout << "Enter transaction date (day month year): ";
                    cin >> this->date.day >> this->date.month >> this->date.year;

                    cout << "Note: ";
                    cin.ignore(); // Để loại bỏ ký tự thừa
                    getline(cin, this->note);

                    cout << "Category: ";
                    getline(cin, this->category);

                    cout << "Payment Method (e.g., Cash, Credit): ";
                    getline(cin, this->paymentMethod);

                    //Ghi những thông tin vừa thay đổi vào file tạm
                    tempFile << this->transactionID << ","
                             << this->accountID << ","
                             << this->transactionType << ","
                             << fixed << setprecision(2) << this->amount << ","
                             << this->date.day << "/" << this->date.month << "/" << this->date.year << ","
                             << this->note << ","
                             << this->category << ","
                             << this->paymentMethod << "\n";
                    isEdited = true;
                }
                else
                {
                    tempFile << line << "\n";
                }
            }
            infile.close();
            tempFile.close();

            remove("transactions.csv");             // xoá file cũ
            rename("temp.csv", "transactions.csv"); // dùng file tạm để làm file mới và đổi tên

            if (isEdited)
            {
                cout << "Transaction edited successfully!\n";
            }
            else
            {
                cerr << "Transaction ID not found.\n";
            }
        }
        else
        {
            cerr << "Error opening file.\n";
        }
    }

    void deleteTransaction() // Cho phép người dùng xoá một giao dịch nếu ID giao dịch cần xoá có tồn tại
    {
        cout << "Enter the transaction ID to delete: "; // Tìm kiếm ID giao dịch cần xoá
        cin >> this->transactionID;

        ifstream infile("transactions.csv");
        ofstream tempFile("temp.csv");
        bool isDeleted = false;

        if (infile.is_open() && tempFile.is_open())
        {
            string line;
            while (getline(infile, line))
            {
                stringstream ss(line);
                string id;
                getline(ss, id, ',');

                if (stoi(id) != this->transactionID) // Giữ lại các dòng khác
                {
                    tempFile << line << "\n";
                }
                else
                {
                    isDeleted = true;
                }
            }
            infile.close();
            tempFile.close();

            remove("transactions.csv");
            rename("temp.csv", "transactions.csv");

            if (isDeleted)
            {
                cout << "Transaction deleted successfully!\n";
            }
            else
            {
                cerr << "Transaction ID not found.\n";
            }
        }
        else
        {
            cerr << "Error opening file.\n";
        }
    }

    list<Transaction> viewTransactionHistory() // Hiển thị các giao dịch trước đó và những giao dịch gần nhất
    {
        list<Transaction> transactions;
        ifstream infile("transactions.csv");

        if (infile.is_open())
        {
            string line;
            while (getline(infile, line))
            {
                stringstream ss(line); // tách giá trị
                string ID, AccountID, Type, Amount, dateStr, Note, Category, PaymentMethod;

                getline(ss, ID, ',');            // Lấy ID giao dịch.
                getline(ss, AccountID, ',');     // Lấy ID tài khoản.
                getline(ss, Type, ',');          // Lấy loại giao dịch.
                getline(ss, Amount, ',');        // Lấy số tiền.
                getline(ss, dateStr, ',');       // Lấy ngày.
                getline(ss, Note, ',');          // Lấy ghi chú.
                getline(ss, Category, ',');      // Lấy danh mục.
                getline(ss, PaymentMethod, ','); // Lấy phương thức thanh toán.

                Transaction t;
                t.transactionID = stoi(ID);
                t.accountID = stoi(AccountID);
                t.transactionType = Type;
                t.amount = stod(Amount);

                // Chuyển đổi chuỗi ngày thành Date
                sscanf(dateStr.c_str(), "%d/%d/%d", &t.date.day, &t.date.month, &t.date.year);

                t.note = Note;
                t.category = Category;
                t.paymentMethod = PaymentMethod;

                transactions.push_back(t);
            }
            infile.close();
        }
        else
        {
            cerr << "Error opening file.\n";
        }

        return transactions;
    }
};
