#pragma once

#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <sstream> // Tách các giá trị trong một chuỗi
#include <vector>
#include <iomanip> // Định dạng đầu ra
#include <filesystem>
#include <regex>

using namespace std;

struct Date
{
    int day;
    int month;
    int year;
};

vector<string> getAccountIDsFromFiles(const string &directory)
{
    vector<string> accountNames;

    // Regular expression để tìm file có dạng "transactions_<accountName>.csv"
    regex filePattern(R"(transactions_(.+)\.csv)"); // Chấp nhận tên account bất kỳ

    // Duyệt qua tất cả file trong thư mục
    for (const auto &entry : filesystem::directory_iterator(directory))
    {
        string filename = entry.path().filename().string();
        smatch match;

        // Kiểm tra nếu tên file khớp với định dạng mong muốn
        if (regex_match(filename, match, filePattern))
        {
            // Lấy accountName từ nhóm khớp đầu tiên
            string accountName = match[1].str();
            accountNames.push_back(accountName);
        }
        else
        {
            cout << "File skipped (not matching pattern): " << filename << endl;
        }
    }

    if (accountNames.empty())
    {
        cerr << "No valid transaction files found in directory: " << directory << "\n";
    }

    return accountNames;
}


class Transaction
{
public:
    int transactionID;
    string accountName;
    string transactionType;
    double amount;
    Date date;
    string note;
    string category;
    // Hàm khởi tạo
    Transaction()
    {
        this->transactionID = 0;
        this->accountName = "";
        this->amount = 0.0;
        this->date = {0, 0, 0};
        this->note = "";
        this->category = "";
    }

    void addTransaction() // Cho phép người dùng thêm một giao dịch mới
    {
        cout << "Enter transaction details:\n";

        cout << "Transaction ID: ";
        cin >> this->transactionID;

        cout << "Account name: ";
        cin.ignore(); // Để loại bỏ ký tự thừa
        getline(cin, this->accountName);

        cout << "Transaction Type: ";
        getline(cin, this->transactionType);

        cout << "Amount: ";
        cin >> this->amount;

        cout << "Enter transaction date (day month year): ";
        cin >> this->date.day >> this->date.month >> this->date.year;

        cout << "Note: ";
        cin.ignore(); // Để loại bỏ ký tự thừa
        getline(cin, this->note);

        // Tên file dựa trên ID của tài khoản
        string filename = "transactions_" + (this->accountName) + ".csv";

        ofstream outfile(filename, ios::app); // Mở file riêng cho tài khoản
        if (outfile.is_open())                // Nếu mở file thành công
        {
            outfile << this->transactionID << ","
                    << this->accountName << ","
                    << this->transactionType << ","
                    << fixed << setprecision(2) << this->amount << ","
                    << this->date.day << "/" << this->date.month << "/" << this->date.year << ","
                    << this->note << "," << "\n";
            outfile.close();
            cout << "Transaction added successfully to file: " << filename << "\n";
        }
        else // Nếu file không mở được
        {
            cerr << "Error opening file for writing.\n";
        }
    }

    void editTransaction()
    {
        cout << "Enter the account name to edit transaction: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, this->accountName);

        cout << "Enter the transaction ID to edit: ";
        cin >> this->transactionID;

        string filename = "transactions_" + this->accountName + ".csv";
        ifstream infile(filename);
        ofstream tempFile("temp.csv");
        bool isEdited = false;

        if (!infile.is_open() || !tempFile.is_open())
        {
            cerr << "Error opening file.\n";
            return;
        }

        string line;
        bool fileHasData = false;

        while (getline(infile, line))
        {
            fileHasData = true; // File có dữ liệu
            stringstream ss(line);
            string ID;
            getline(ss, ID, ','); // Lấy transactionID từ file

            try
            {
                if (stoi(ID) == this->transactionID) // Nếu trùng ID
                {
                    cout << "Editing transaction with ID: " << this->transactionID << endl;
                    cout << "Enter new details:" << endl;

                    cout << "Transaction Type: ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    getline(cin, this->transactionType);

                    cout << "Amount: ";
                    cin >> this->amount;

                    cout << "Enter transaction date (day month year): ";
                    cin >> this->date.day >> this->date.month >> this->date.year;

                    cout << "Note: ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    getline(cin, this->note);

                    tempFile << this->transactionID << ","
                             << this->accountName << ","
                             << this->transactionType << ","
                             << fixed << setprecision(2) << this->amount << ","
                             << this->date.day << "/" << this->date.month << "/" << this->date.year << ","
                             << this->note << "\n";

                    isEdited = true;
                }
                else
                {
                    tempFile << line << "\n"; // Ghi dòng cũ nếu không chỉnh sửa
                }
            }
            catch (const exception &e)
            {
                cerr << "Error parsing transaction ID: " << e.what() << "\n";
                tempFile << line << "\n"; // Ghi dòng lỗi vào file tạm
            }
        }

        infile.close();
        tempFile.close();

        if (!fileHasData)
        {
            cerr << "The file is empty or does not exist: " << filename << "\n";
            remove("temp.csv"); // Xóa file tạm nếu file gốc trống
            return;
        }

        if (isEdited)
        {
            remove(filename.c_str());
            rename("temp.csv", filename.c_str());
            cout << "Transaction edited successfully in file: " << filename << "\n";
        }
        else
        {
            cerr << "Transaction ID not found in file: " << filename << "\n";
            remove("temp.csv"); // Xóa file tạm nếu không chỉnh sửa
        }
    }

    void deleteTransaction()
    {
        cout << "Enter the account name to delete transaction: ";
        cin.ignore();
        getline(cin, this->accountName);

        cout << "Enter the transaction ID to delete: ";
        cin >> this->transactionID;

        // Tên file dựa trên accountID
        string filename = "transactions_" + (this->accountName) + ".csv";
        ifstream infile(filename);
        ofstream tempFile("temp.csv"); // File tạm để ghi dữ liệu mới
        bool isDeleted = false;

        if (infile.is_open() && tempFile.is_open())
        {
            string line;
            while (getline(infile, line))
            {
                stringstream ss(line);
                string ID;
                getline(ss, ID, ','); // Lấy transactionID từ file

                if (stoi(ID) != this->transactionID) // Giữ lại các giao dịch khác
                {
                    tempFile << line << "\n";
                }
                else
                {
                    isDeleted = true; // Đánh dấu đã xóa
                }
            }
            infile.close();
            tempFile.close();

            // Thay thế file cũ bằng file tạm
            remove(filename.c_str());
            rename("temp.csv", filename.c_str());

            if (isDeleted)
            {
                cout << "Transaction deleted successfully from file: " << filename << "\n";
            }
            else
            {
                cerr << "Transaction ID not found in file: " << filename << "\n";
            }
        }
        else
        {
            cerr << "Error opening file.\n";
        }
    }

    list<Transaction> viewTransactionHistory(const string &directory)
    {
        list<Transaction> transactions;

        // Tự động lấy danh sách accountID từ file
        vector<string> accountNames = getAccountIDsFromFiles(directory);

        // File tổng hợp để ghi tất cả giao dịch
        string consolidatedFile = "all_transactions.csv";
        ofstream consolidatedOutput(consolidatedFile);

        if (!consolidatedOutput.is_open())
        {
            cerr << "Error opening consolidated file: " << consolidatedFile << "\n";
            return transactions;
        }

        // Thêm tiêu đề vào file tổng hợp
        consolidatedOutput << "TransactionID,AccountName,TransactionType,Amount,Date,Note\n";

        for (const string &account_Name : accountNames)
        {
            string filename = directory + "/transactions_" + account_Name + ".csv"; // Đảm bảo đúng đường dẫn
            ifstream infile(filename);

            if (infile.is_open())
            {
                string line;
                while (getline(infile, line))
                {
                    stringstream ss(line); // Tách giá trị từ mỗi dòng
                    string ID, AccountName, Type, Amount, dateStr, Note, Category;

                    getline(ss, ID, ',');          // Lấy transactionID
                    getline(ss, AccountName, ','); // Lấy accountName
                    getline(ss, Type, ',');        // Lấy transactionType
                    getline(ss, Amount, ',');      // Lấy amount
                    getline(ss, dateStr, ',');     // Lấy date
                    getline(ss, Note, ',');        // Lấy note

                    Transaction t;
                    t.transactionID = stoi(ID);
                    t.accountName = AccountName;
                    t.transactionType = Type;
                    t.amount = stod(Amount);

                    // Chuyển đổi ngày tháng
                    stringstream dateStream(dateStr);
                    string day, month, year;
                    getline(dateStream, day, '/');
                    getline(dateStream, month, '/');
                    getline(dateStream, year, '/');
                    t.date = {stoi(day), stoi(month), stoi(year)};

                    t.note = Note;

                    // Ghi giao dịch vào danh sách
                    transactions.push_back(t);

                    // Ghi vào file tổng hợp
                    consolidatedOutput << t.transactionID << ","
                                       << t.accountName << ","
                                       << t.transactionType << ","
                                       << fixed << setprecision(2) << t.amount << ","
                                       << t.date.day << "/" << t.date.month << "/" << t.date.year << ","
                                       << t.note << "\n";
                }
                infile.close();
            }
            else
            {
                cerr << "Error opening file: " << filename << "\n";
            }
        }
        consolidatedOutput.close();

        cout << "All transactions have been consolidated into: " << consolidatedFile << "\n";

        return transactions;
    }
};
