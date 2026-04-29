
// Automobile Insurance Management System

// Architecture:
//   - Business Layer: Entity classes and InsuranceSystem facade
//   - Repository Layer: File I/O with flat text files (pipe-delimited)
//   - UI Layer: Menu-driven interfaces for Salesman, Surveyor, Manager
// 

// Usage on Windows:
//   g++ -std=c++17 -Wall -o insurance.exe main.cpp business/*.cpp repository/*.cpp ui/*.cpp
//   .\insurance.exe
// ────────────────────────────────────────────────────────────────────────────

#include <iostream>
#include <string>
#include "business/InsuranceSystem.h"
#include "ui/SalesmanUI.h"
#include "ui/SurveyorUI.h"
#include "ui/ManagerUI.h"

#include <direct.h>
#include <windows.h>
#include <conio.h>

using namespace std;

static const string RST  = "\033[0m";
static const string CYN  = "\033[36m";
static const string YLW  = "\033[33m";
static const string RED  = "\033[31m";
static const string GRN  = "\033[32m";
static const string BLD  = "\033[1m";
static const string WHT  = "\033[37m";

static void enableConsole() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hOut, &mode);
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
    SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

static void createDataDir() {
    _mkdir("data");
}

static string getPasswordHidden() {
    string password;
    char ch;
    while (true) {
        ch = (char)_getch();
        if (ch == '\r' || ch == '\n') break;
        if (ch == '\b') {
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b" << flush;
            }
        } else {
            password += ch;
            cout << '*' << flush;
        }
    }
    cout << "\n";
    return password;
}

static void showBanner() {
    cout << CYN << BLD;
    cout << "  ╔══════════════════════════════════════════════════════╗\n\n";
    cout << "  ║       Welcome to the Automobile Insurance System     ║\n";
    cout << "  ║                                                      ║\n";
    cout << "  ║                24L-0911 & 24l-0515                   ║\n";
    cout << "  ╚══════════════════════════════════════════════════════╝\n\n";
    cout << RST << "\n";
}

int main() {
    enableConsole();
    createDataDir();

    InsuranceSystem sys;
    sys.seedIfEmpty();

    while (true) {
        showBanner();

        cout << CYN << "  ─── Login ───\n" << RST;
        cout << "  Username: ";
        string username;
        getline(cin, username);
        while (!username.empty() && (username.back() == '\r' || username.back() == '\n'))
            username.pop_back();

        cout << "  Password: ";
        string password = getPasswordHidden();

        Staff* staff = sys.login(username, password);

        if (!staff) {
            cout << RED << "\n  ✗ Invalid credentials. Try again.\n" << RST;
            cout << "  Press Enter to continue...";
            string d; getline(cin, d);
            continue;
        }

        string role = staff->getRole();
        cout << GRN << "\n  ✓ Welcome, " << staff->getName() << " (" << role << ")\n" << RST;

        if (role == "Manager") {
            ManagerUI ui(sys);
            ui.run(staff);
        } else if (role == "Salesman") {
            SalesmanUI ui(sys);
            ui.run(staff);
        } else if (role == "Surveyor") {
            SurveyorUI ui(sys);
            ui.run(staff);
        } else {
            cout << YLW << "  Unknown role. Logging out.\n" << RST;
        }

        delete staff;

        cout << "\n  Press Enter to login again, or Ctrl+C to exit...";
        string d; getline(cin, d);
    }

    return 0;
}
