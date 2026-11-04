#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <cstring>
#include <cstdlib>

using namespace std;

namespace Color {
    const string RESET = "\033[0m";
    const string RED = "\033[31m";
    const string GREEN = "\033[32m";
    const string YELLOW = "\033[33m";
    const string CYAN = "\033[36m";
    const string BOLD = "\033[1m";
}

struct stock {
    int id;
    string name;
    int quantity;
    float cost;
    float price;
    int min;
    float profit() {
        return price - cost;
    }
};

struct Admin {
    char username[50];
    char password[50];
};

void clearScreen() {
#ifdef _WIN32

#else
    system("clear");
#endif
}

string getPassword()
 {
    string password = "";
    cin >> password;
    return password;
}

bool VerfiyUser() {
    Admin admin;
    ifstream file("admin.dat", ios::binary);
    if (!file) {
        ofstream out("admin.dat", ios::binary);
        strcpy(admin.username, "admin");
        strcpy(admin.password, "password");
        out.write((char*)&admin, sizeof(Admin));
        out.close();
        cout << Color::YELLOW << "Default admin created. (admin/password)" << Color::RESET << endl;
        file.open("admin.dat", ios::binary);
    }
    file.read((char*)&admin, sizeof(Admin));
    file.close();

    string inputUser, inputPass;
    cout << "Enter username: ";
    cin >> inputUser;
    cout << "Enter password: ";
    inputPass = getPassword();

    if (inputUser == admin.username && inputPass == admin.password) {
        cout << Color::GREEN << "Login successful!" << Color::RESET << endl;
        return true;
    } else {
        cout << Color::RED << "Invalid credentials!" << Color::RESET << endl;
        return false;
    }
}

void addtostock() {
    int n;
    cout << "How many types of items received? ";
    cin >> n;
    vector<stock> s(n);
    ofstream info("stockinfo.txt", ios::app); // Changed to append mode for better logic

    for (int i = 0; i < n; i++) {
        clearScreen();
        cout << Color::CYAN << "--- Adding Item " << i + 1 << " ---" << Color::RESET << endl;
        cout << "ID: "; cin >> s[i].id;
        cin.ignore();
        cout << "Name: "; getline(cin, s[i].name);
        cout << "Quantity: "; cin >> s[i].quantity;
        cout << "Cost: "; cin >> s[i].cost;
        cout << "Price: "; cin >> s[i].price;
        cout << "Min Threshold: "; cin >> s[i].min;

          info << s[i].id << endl;
          info<< s[i].name << endl;
          info<< s[i].quantity <<endl;
          info << s[i].cost <<endl;
         info<< s[i].price <<endl;
        info << s[i].min << endl;
    }
    info.close();
    cout << Color::GREEN << "Stock updated successfully!" << Color::RESET << endl;
}

void displaystock() {
    clearScreen();
    ifstream info("stockinfo.txt");
    if (!info) {
        cout << Color::RED << "No stock data found." << Color::RESET << endl;
        return;
    }

    stock t;
    cout << Color::BOLD << Color::CYAN << "ID\tNAME\t\tQTY\tPRICE\tPROFIT" << Color::RESET << endl;
    cout << "----------------------------------------------------" << endl;

    while (info >> t.id >> t.name >> t.quantity >> t.cost >> t.price >> t.min) {
        cout << Color::YELLOW << t.id << "\t" << Color::RESET
             << t.name << "\t\t" << t.quantity << "\t"
             << t.price << "\t" << Color::GREEN << t.profit() << Color::RESET << endl;
    }
    info.close();
}

void customerservice() {
    vector<stock> s;
    ifstream info("stockinfo.txt");
    stock temp;
    while (info >> temp.id >> temp.name >> temp.quantity >> temp.cost >> temp.price >> temp.min) {
        s.push_back(temp);
    }
    info.close();

    int buyID, buyQty;
    float totalBill = 0;
while (true) {
        clearScreen();
        cout << Color::CYAN << "=== CUSTOMER SHOPPING ===" << Color::RESET << endl;
        for (auto &item : s) {
            cout << "[" << Color::YELLOW << item.id << Color::RESET << "] "
                 << item.name << " - $" << item.price
                 << " (In Stock: " << item.quantity << ")" << endl;
        }
        cout << "\nEnter ID to buy (0 to checkout): ";
        cin >> buyID;
        if (buyID == 0) break;

        bool found = false;
        for (auto &item : s) {
            if (item.id == buyID) {
                found = true;
                cout << "Quantity: ";
                cin >> buyQty;
                if (buyQty <= item.quantity) {
                    item.quantity -= buyQty;
                    totalBill += (buyQty * item.price);
                    cout << Color::GREEN << "Added to cart!" << Color::RESET << endl;
                } else {
                    cout << Color::RED << "Insufficient stock!" << Color::RESET << endl;
                }
                break;
            }
        }
        if(!found) cout << Color::RED << "Item not found!" << Color::RESET << endl;
        cout << "\nPress Enter to continue shopping...";
        cin.ignore(); cin.get();
    }

    // Save updated stock
    ofstream out("stockinfo.txt");
    for (auto &item : s) {
        out << item.id << " " << item.name << " " << item.quantity << " "
            << item.cost << " " << item.price << " " << item.min << endl;
    }
    out.close();

    clearScreen();
    cout << Color::BOLD << Color::GREEN << "TOTAL BILL: $" << totalBill << Color::RESET << endl;
}

void checkAmount()
 {
    ifstream info("stockinfo.txt");
    stock t;
    bool warning = false;
    cout << Color::CYAN << "\n--- LOW STOCK REPORT ---" << Color::RESET << endl;
    while (info >> t.id >> t.name >> t.quantity >> t.cost >> t.price >> t.min)
        {
        if (t.quantity <= t.min) {
            cout << Color::RED << "WARNING: " << t.name << " is low (" << t.quantity << " left)" << Color::RESET << endl;
            warning = true;
        }
    }
    if (!warning) cout << Color::GREEN << "All stock levels are healthy." << Color::RESET << endl;
    info.close();
}

int main()
{
    char role;
    bool isAdmin = false;

    clearScreen();
    cout << Color::BOLD << Color::CYAN << "WELCOME TO STOCK PRO v1.0" << Color::RESET << endl;
    cout << "Identify Role [M]anager / [C]ustomer: ";
    cin >> role;

    if (role == 'm' || role == 'M') {
        if (VerfiyUser()) isAdmin = true;
        else return 0;
    }

    int choice;
    do {
        cout << Color::CYAN << "\n--- MAIN MENU ---" << Color::RESET << endl;
        if (isAdmin) cout << "1. Add Stock\n6. Change Password" << endl;
        cout << "2. Display Stock\n3. Customer Service\n4. Check Low Stock\n5. Exit" << endl;
        cout << "Selection: ";
        cin >> choice;

        switch (choice) {
            case 1: if(isAdmin) addtostock(); break;
            case 2: displaystock(); break;
            case 3: customerservice(); break;
            case 4: checkAmount(); break;
            case 5: cout << "Goodbye!"; break;
            default: cout << "Invalid choice.";
        }

        if (choice != 5) {
            cout << "\nPress Enter to return to menu...";
            cin.ignore();
            cin.get();
            clearScreen();
        }
    } while (choice != 5);

    return 0;
}