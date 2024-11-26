#pragma once

#include <iostream>
#include <string>
#include <regex> // Sử dụng để tìm kiếm, lọc và thay thế các chuỗi ký tự theo một mẫu cú pháp cụ thể

using namespace std;

class USER
{
private:
    int userID;
    string userName;
    string email;
    string password;

    // Hàm kiểm tra email hợp lệ
    bool isValidEmail(const string &email)
    {
        regex emailRegex("^[\\w.%+-]+@[\\w.-]+\\.[a-zA-Z]{2,}$");
        return regex_match(email, emailRegex);
    }

    // Hàm kiểm tra mật khẩu mạnh
    bool isStrongPassword(const string &password)
    {
        regex passwordRegex("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[@$!%*?&])[A-Za-z\\d@$!%*?&]{8,}$");
        return regex_match(password, passwordRegex);
    }

public:
    bool login() // Yêu cầu người dùng đăng nhập bằng username hoặc email và mật khẩu
    {
        string inputUserName, inputPassword;
        // string inputEmail;
        cout << "Enter your Username: "; // Nhập tên người dùng
        getline(cin, inputUserName);
        // Hoặc có thể nhập bằng email;
        // cout << "Enter your email: ";
        // getline(cin, inputEmail);
        cout << "Enter your password: "; // Nhập mật khẩu
        getline(cin, inputPassword);

        // Kiểm tra tính chính xác
        if (this->userName == inputUserName && this->password == inputPassword) // Có thể kiểm tra bằng email
        {
            cout << "Login Successfull!" << endl; // Hiển thị khi đăng nhập thành công
            return true;
        }
        else
        {
            cout << "Invalid email or password. Please try again." << endl; // Hiển thị khi đăng nhập thất bại
            return false;
        }
    }

    void registerUser() // Cho phép người dùng đăng ký tài khoản mới bằng những thông tin cơ bản
    {
        cout << "Registering a new user:" << endl;

        cout << "Enter user ID: ";
        cin >> this->userID;

        cout << "Enter your name: ";
        cin.ignore(); // Bỏ qua ký tự xuống dòng còn lại trong bộ đệm
        getline(cin, this->userName);

        cout << "Enter your email: ";
        getline(cin, this->email);
        // Kiểm tra email hợp lệ
        while (!isValidEmail(email))
        {
            cout << "Invalid email format. Enter a valid email: ";
            getline(cin, this->email);
        }

        // Kiểm tra độ mạnh của mật khẩu
        while (true)
        {
            cout << "Create a password (at least 8 characters, including uppercase, lowercase, special character): ";
            getline(cin, this->password);
            // Kiểm tra mật khẩu đã đáp ứng tiêu chí đề ra hay không
            if (!isStrongPassword(this->password))
            {
                cout << "Weak password. Please try again!" << endl;
                continue;
            }
            string confirmPassword;
            cout << "Confirm your password: ";
            cin >> confirmPassword;
            // Kiểm tra mật khẩu xác nhận có chính xác không
            if (this->password != confirmPassword)
            {
                cout << "Passwords do not match. Please try again!" << endl;
            }
            else
            {
                cout << "Password set successfully!" << endl;
                break;
            }
        }

        cout << "User registered successfully!" << endl; // Đăng ký thành công
    }

    void updateProfile() // Cho phép người dùng cập nhật lại những thông tin
    {
        int choice;
        cout << "Update Profile:" << endl;
        cout << "1. Update Name" << endl;
        cout << "2. Update Email" << endl;
        cout << "3. Update Password" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {

            cout << "Enter new name: ";
            cin.ignore(); // Bỏ qua ký tự xuống dòng còn lại trong bộ đệm
            getline(cin, userName);
            cout << "Name updated successfully!" << endl;
            break;
        }
        case 2:
        {
            cout << "Enter new email: ";
            getline(cin, this->email);

            // Kiểm tra email hợp lệ
            while (!isValidEmail(this->email))
            {
                cout << "Invalid email format. Enter a valid email: ";
                getline(cin, this->email);
            }

            cout << "Email updated successfully!" << endl;
            break;
        }
        case 3:
        {
            string oldPassword, newPassword, confirmPassword;

            // Yêu cầu nhập mật khẩu cũ để xác minh
            cout << "Enter your current password: ";
            cin >> oldPassword;

            if (oldPassword != this->password)
            {
                cout << "Incorrect current password. Password update failed.\n";
                break; // Thoát khỏi mục cập nhật mật khẩu nếu mật khẩu cũ không khớp
            }

            // Nếu mật khẩu cũ đúng sẽ tiếp tục yêu cầu mật khẩu mới
            while (true)
            {   
                //Mật khẩu mới yêu cầu phải là mật khẩu có tính bảo mật cao
                cout << "Enter new Password (at least 8 characters, including uppercase, lowercase, special character): ";
                cin >> newPassword;

                if (!isStrongPassword(newPassword))
                {
                    cout << "Weak password. Please try again!\n";
                    continue;
                }
                //Kiểm tra mật khẩu xác nhận chính xác không
                cout << "Confirm new Password: ";
                cin >> confirmPassword;

                if (newPassword != confirmPassword)
                {
                    cout << "Passwords do not match. Please try again!\n";
                }
                else
                {
                    password = newPassword; // Cập nhật mật khẩu mới thành công
                    cout << "Password updated successfully!\n";
                    break;
                }
            }
            break;
        }
        default:
            cout << "Invalid choice. No changes were made." << endl;
            break;
        }
    }
};
