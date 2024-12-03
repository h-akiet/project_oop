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

vector<int> getAccountIDsFromFiles(const string &directory)
{
    vector<int> accountIDs;

    // Regular expression để tìm các file có dạng "transactions_<accountID>.csv"
    regex filePattern(R"(transactions_(\d+)\.csv)");

    // Duyệt qua tất cả file trong thư mục
    for (const auto &entry : std::filesystem::directory_iterator(directory))
    {
        string filename = entry.path().filename().string();
        smatch match;

        // Kiểm tra nếu tên file khớp với định dạng mong muốn
        if (regex_match(filename, match, filePattern))
        {
            // Lấy accountID từ nhóm khớp đầu tiên
            int accountID = stoi(match[1].str());
            accountIDs.push_back(accountID);
        }
    }

    return accountIDs;
}

class Transaction
{
public:
    int transactionID;
    int accountID;
    string transactionType;
    double amount;
    Date date;
    string note;
    string category;
    // Hàm khởi tạo
    Transaction()
    {
        this->transactionID = 0;
        this->accountID = 0;
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

        // Tên file dựa trên ID của tài khoản
        string filename = "transactions_" + to_string(this->accountID) + ".csv";

        ofstream outfile(filename, ios::app); // Mở file riêng cho tài khoản
        if (outfile.is_open())                // Nếu mở file thành công
        {
            outfile << this->transactionID << ","
                    << this->accountID << ","
                    << this->transactionType << ","
                    << fixed << setprecision(2) << this->amount << ","
                    << this->date.day << "/" << this->date.month << "/" << this->date.year << ","
                    << this->note << ","
                    << this->category << "\n";

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
        cout << "Enter the account ID to edit transaction: ";
        cin >> this->accountID;

        cout << "Enter the transaction ID to edit: ";
        cin >> this->transactionID;

        // Tên file dựa trên accountID
        string filename = "transactions_" + to_string(this->accountID) + ".csv";
        ifstream infile(filename);
        ofstream tempFile("temp.csv"); // File tạm để ghi dữ liệu mới
        bool isEdited = false;

        if (infile.is_open() && tempFile.is_open())
        {
            string line;
            while (getline(infile, line))
            {
                stringstream ss(line);
                string ID;
                getline(ss, ID, ','); // Lấy transactionID từ file

                if (stoi(ID) == this->transactionID) // Nếu trùng ID
                {
                    // Yêu cầu nhập dữ liệu mới
                    cout << "Editing transaction with ID: " << this->transactionID << endl;
                    cout << "Enter new details:" << endl;

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

                    // Ghi dữ liệu mới vào file tạm
                    tempFile << this->transactionID << ","
                             << this->accountID << ","
                             << this->transactionType << ","
                             << fixed << setprecision(2) << this->amount << ","
                             << this->date.day << "/" << this->date.month << "/" << this->date.year << ","
                             << this->note << ","
                             << this->category << "\n";
                    isEdited = true;
                }
                else
                {
                    tempFile << line << "\n"; // Ghi dòng cũ nếu không chỉnh sửa
                }
            }
            infile.close();
            tempFile.close();

            // Thay thế file cũ bằng file tạm
            remove(filename.c_str());
            rename("temp.csv", filename.c_str());

            if (isEdited)
            {
                cout << "Transaction edited successfully in file: " << filename << "\n";
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

    void deleteTransaction()
    {
        cout << "Enter the account ID to delete transaction: ";
        cin >> this->accountID;

        cout << "Enter the transaction ID to delete: ";
        cin >> this->transactionID;

        // Tên file dựa trên accountID
        string filename = "transactions_" + to_string(this->accountID) + ".csv";
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
        vector<int> accountIDs = getAccountIDsFromFiles(directory);

        // File tổng hợp để ghi tất cả giao dịch
        string consolidatedFile = "all_transactions.csv";
        ofstream consolidatedOutput(consolidatedFile);

        if (!consolidatedOutput.is_open())
        {
            cerr << "Error opening consolidated file: " << consolidatedFile << "\n";
            return transactions;
        }

        // Thêm tiêu đề vào file tổng hợp
        consolidatedOutput << "TransactionID,AccountID,TransactionType,Amount,Date,Note,Category\n";

        for (int accountID : accountIDs)
        {
            string filename = directory + "/transactions_" + to_string(accountID) + ".csv";
            ifstream infile(filename);

            if (infile.is_open())
            {
                string line;
                while (getline(infile, line))
                {
                    stringstream ss(line); // Tách giá trị từ mỗi dòng
                    string ID, AccountID, Type, Amount, dateStr, Note, Category;

                    getline(ss, ID, ',');        // Lấy transactionID
                    getline(ss, AccountID, ','); // Lấy accountID
                    getline(ss, Type, ',');      // Lấy loại giao dịch
                    getline(ss, Amount, ',');    // Lấy số tiền
                    getline(ss, dateStr, ',');   // Lấy ngày
                    getline(ss, Note, ',');      // Lấy ghi chú
                    getline(ss, Category, ',');  // Lấy danh mục

                    Transaction t;
                    t.transactionID = stoi(ID);
                    t.accountID = stoi(AccountID);
                    t.transactionType = Type;
                    t.amount = stod(Amount);

                    // Chuyển đổi chuỗi ngày thành đối tượng Date
                    sscanf(dateStr.c_str(), "%d/%d/%d", &t.date.day, &t.date.month, &t.date.year);

                    t.note = Note;
                    t.category = Category;

                    // Thêm giao dịch vào danh sách
                    transactions.push_back(t);

                    // Ghi dòng vào file tổng hợp
                    consolidatedOutput << line << "\n";
                }
                infile.close();
            }
            else
            {
                cerr << "Error opening file: " << filename << "\n";
            }
        }

        consolidatedOutput.close(); // Đóng file tổng hợp sau khi ghi xong
        cout << "All transactions have been consolidated into file: " << consolidatedFile << "\n";

        return transactions;
    }
};
