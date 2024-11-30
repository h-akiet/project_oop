#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip> // Để sử dụng std::get_time
#include <vector>

using namespace std;

class Budget
{
private:
    int budgetID;    // Mã ngân sách
    int userID;      // Mã người dùng
    double amount;   // Số tiền ngân sách
    string category; // Danh mục ngân sách
    std::tm startDate; // Ngày bắt đầu
    std::tm endDate;   // Ngày kết thúc

public:
    // Hàm để chuyển đổi từ std::tm sang chuỗi (định dạng YYYY-MM-DD)
    string tmToString(const std::tm &t) const
    {
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", &t); // Định dạng ngày tháng
        return string(buffer); // Trả về chuỗi ngày tháng
    }

    // Hàm để chuyển đổi từ chuỗi sang std::tm (sử dụng std::get_time)
    void stringToTm(const string &dateStr, std::tm &t)
    {
        std::istringstream ss(dateStr); // Chuyển chuỗi thành dòng
        ss >> std::get_time(&t, "%Y-%m-%d"); // Cố gắng chuyển chuỗi thành std::tm
        if (ss.fail()) // Nếu không thành công
        {
            cerr << "Failed to parse date: " << dateStr << endl; // Hiển thị lỗi
        }
    }

    // Hàm nhập ngân sách từ người dùng và ghi vào file
    void setBudget()
    {
        string dateStr;

        cout << "Enter Budget ID: ";
        cin >> budgetID; // Nhập mã ngân sách
        cout << "Enter User ID: ";
        cin >> userID;   // Nhập mã người dùng
        cout << "Enter Amount: ";
        cin >> amount;    // Nhập số tiền ngân sách
        cin.ignore();     // Bỏ qua ký tự newline sau khi nhập số

        cout << "Enter Category: ";
        getline(cin, category); // Nhập danh mục ngân sách

        cout << "Enter Start Date (YYYY-MM-DD): ";
        getline(cin, dateStr);  // Nhập ngày bắt đầu
        stringToTm(dateStr, startDate); // Chuyển đổi ngày bắt đầu thành std::tm

        cout << "Enter End Date (YYYY-MM-DD): ";
        getline(cin, dateStr);  // Nhập ngày kết thúc
        stringToTm(dateStr, endDate); // Chuyển đổi ngày kết thúc thành std::tm

        // Mở file để ghi ngân sách vào
        ofstream outfile("Budgets.csv", ios::app);
        if (outfile.is_open()) // Kiểm tra xem file có mở thành công không
        {
            // Ghi dữ liệu ngân sách vào file
            outfile << budgetID << "," << userID << "," << amount << "," << category << ","
                   << tmToString(startDate) << "," << tmToString(endDate) << "\n";
            cout << "Budget added successfully!" << endl;
            outfile.close(); // Đóng file sau khi ghi xong
        }
        else
        {
            cout << "Error opening file!" << endl; // Nếu không mở được file
        }
    }

    // Hàm xem các ngân sách từ file
    void viewBudget()
    {
        ifstream infile("Budgets.csv"); // Mở file đọc dữ liệu
        string line;
        if (infile.is_open()) // Kiểm tra xem file có mở thành công không
        {
            // Đọc từng dòng trong file và hiển thị ra màn hình
            while (getline(infile, line))
            {
                stringstream ss(line);
                string field;
                int tempBudgetID, tempUserID;
                double tempAmount;
                string tempCategory, tempStartDate, tempEndDate;

                getline(ss, field, ','); // Đọc từng trường trong file CSV
                tempBudgetID = stoi(field);
                getline(ss, field, ',');
                tempUserID = stoi(field);
                getline(ss, field, ',');
                tempAmount = stod(field);
                getline(ss, field, ',');
                tempCategory = field;
                getline(ss, field, ',');
                tempStartDate = field;
                getline(ss, field, ',');
                tempEndDate = field;

                // Hiển thị các thông tin ngân sách
                cout << "Budget ID: " << tempBudgetID << "\n"
                     << "User ID: " << tempUserID << "\n"
                     << "Amount: " << tempAmount << "\n"
                     << "Category: " << tempCategory << "\n"
                     << "Start Date: " << tempStartDate << "\n"
                     << "End Date: " << tempEndDate << "\n\n";
            }
            infile.close(); // Đóng file sau khi đọc xong
        }
        else
        {
            cout << "Error opening file!" << endl; // Nếu không mở được file
        }
    }

    // Hàm đọc dữ liệu từ file CSV vào danh sách ngân sách và hiển thị
    void readFromFile() //Nếu cần sử dụng khi in ra các kế hoạch chi tiêu đã cập nhật trong file thì có thể sử dụng
    {
        ifstream infile("Budgets.csv"); // Mở file đọc dữ liệu
        string line;
        vector<Budget> budgets; // Danh sách ngân sách từ file

        if (infile.is_open()) // Kiểm tra xem file có mở thành công không
        {
            // Đọc từng dòng trong file
            while (getline(infile, line))
            {
                stringstream ss(line);
                string field;
                int tempBudgetID, tempUserID;
                double tempAmount;
                string tempCategory, tempStartDate, tempEndDate;

                // Đọc các trường trong file CSV
                getline(ss, field, ',');
                tempBudgetID = stoi(field);
                getline(ss, field, ',');
                tempUserID = stoi(field);
                getline(ss, field, ',');
                tempAmount = stod(field);
                getline(ss, field, ',');
                tempCategory = field;
                getline(ss, field, ',');
                tempStartDate = field;
                getline(ss, field, ',');
                tempEndDate = field;

                // Tạo một đối tượng Budget và thêm vào vector
                Budget b;
                b.budgetID = tempBudgetID;
                b.userID = tempUserID;
                b.amount = tempAmount;
                b.category = tempCategory;

                // Chuyển đổi chuỗi thành std::tm và gán cho startDate và endDate
                b.stringToTm(tempStartDate, b.startDate);
                b.stringToTm(tempEndDate, b.endDate);

                budgets.push_back(b); // Thêm ngân sách vào danh sách
            }
            infile.close(); // Đóng file sau khi đọc xong

            // Hiển thị các ngân sách đã đọc từ file
            cout << "Budgets from file:\n";
            for (const auto &b : budgets)
            {
                cout << "Budget ID: " << b.budgetID << "\n"
                     << "User ID: " << b.userID << "\n"
                     << "Amount: " << b.amount << "\n"
                     << "Category: " << b.category << "\n"
                     << "Start Date: " << b.tmToString(b.startDate) << "\n"
                     << "End Date: " << b.tmToString(b.endDate) << "\n\n";
            }
        }
        else
        {
            cout << "Error opening file!" << endl; // Nếu không mở được file
        }
    }

    // Hàm cập nhật ngân sách
    void updateBudget()
    {
        int updateID;
        cout << "Enter Budget ID to update: ";
        cin >> updateID; // Nhập mã ngân sách cần cập nhật

        ifstream infile("Budgets.csv"); // Mở file để đọc
        ofstream tempFile("temp.csv"); // Mở file tạm để ghi lại dữ liệu
        string line;
        bool found = false;

        if (infile.is_open() && tempFile.is_open()) // Kiểm tra xem cả hai file có mở thành công không
        {
            // Đọc từng dòng trong file CSV
            while (getline(infile, line))
            {
                stringstream ss(line);
                string field;
                int tempBudgetID, tempUserID;
                double tempAmount;
                string tempCategory, tempStartDate, tempEndDate;

                // Đọc các trường trong file
                getline(ss, field, ',');
                tempBudgetID = stoi(field);
                getline(ss, field, ',');
                tempUserID = stoi(field);
                getline(ss, field, ',');
                tempAmount = stod(field);
                getline(ss, field, ',');
                tempCategory = field;
                getline(ss, field, ',');
                tempStartDate = field;
                getline(ss, field, ',');
                tempEndDate = field;

                // Nếu tìm thấy ngân sách cần cập nhật
                if (tempBudgetID == updateID)
                {
                    found = true;
                    cout << "Enter new Amount: ";
                    cin >> tempAmount; // Nhập số tiền mới
                    cin.ignore();
                    cout << "Enter new Category: ";
                    getline(cin, tempCategory); // Nhập danh mục mới
                    cout << "Enter new Start Date (YYYY-MM-DD): ";
                    getline(cin, tempStartDate); // Nhập ngày bắt đầu mới
                    stringToTm(tempStartDate, startDate);
                    cout << "Enter new End Date (YYYY-MM-DD): ";
                    getline(cin, tempEndDate); // Nhập ngày kết thúc mới
                    stringToTm(tempEndDate, endDate);
                }

                // Ghi lại dữ liệu vào file tạm
                tempFile << tempBudgetID << "," << tempUserID << "," << tempAmount << "," << tempCategory << ","
                         << tempStartDate << "," << tempEndDate << "\n";
            }
            infile.close();
            tempFile.close();

            if (found)
            {
                // Nếu tìm thấy ngân sách cần cập nhật, thay thế file cũ bằng file tạm
                remove("Budgets.csv");
                rename("temp.csv", "Budgets.csv");
                cout << "Budget updated successfully!" << endl;
            }
            else
            {
                cout << "Budget not found!" << endl; // Nếu không tìm thấy ngân sách cần cập nhật
            }
        }
        else
        {
            cout << "Error opening files!" << endl;
        }
    }

    // Hàm xóa ngân sách
    void deleteBudget()
    {
        int deleteID;
        cout << "Enter Budget ID to delete: ";
        cin >> deleteID; // Nhập mã ngân sách cần xóa

        ifstream infile("Budgets.csv"); // Mở file để đọc
        ofstream tempFile("temp.csv"); // Mở file tạm để ghi lại dữ liệu
        string line;
        bool found = false;

        if (infile.is_open() && tempFile.is_open()) // Kiểm tra xem cả hai file có mở thành công không
        {
            // Đọc từng dòng trong file CSV
            while (getline(infile, line))
            {
                stringstream ss(line);
                string field;
                int tempBudgetID, tempUserID;
                double tempAmount;
                string tempCategory, tempStartDate, tempEndDate;

                // Đọc các trường trong file
                getline(ss, field, ',');
                tempBudgetID = stoi(field);
                getline(ss, field, ',');
                tempUserID = stoi(field);
                getline(ss, field, ',');
                tempAmount = stod(field);
                getline(ss, field, ',');
                tempCategory = field;
                getline(ss, field, ',');
                tempStartDate = field;
                getline(ss, field, ',');
                tempEndDate = field;

                // Nếu không phải ngân sách cần xóa, ghi vào file tạm
                if (tempBudgetID != deleteID)
                {
                    tempFile << tempBudgetID << "," << tempUserID << "," << tempAmount << "," << tempCategory << ","
                             << tempStartDate << "," << tempEndDate << "\n";
                }
                else
                {
                    found = true; // Đánh dấu đã tìm thấy ngân sách cần xóa
                }
            }
            infile.close();
            tempFile.close();

            if (found)
            {
                // Nếu tìm thấy ngân sách cần xóa, thay thế file cũ bằng file tạm
                remove("Budgets.csv");
                rename("temp.csv", "Budgets.csv");
                cout << "Budget deleted successfully!" << endl;
            }
            else
            {
                cout << "Budget not found!" << endl; // Nếu không tìm thấy ngân sách cần xóa
            }
        }
        else
        {
            cout << "Error opening files!" << endl;
        }
    }
};
