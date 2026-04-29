#include "SalesmanUI.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <cctype>
using namespace std;

// ─── ANSI ──────────────────────────────────────────────────────────────────
static const string RST = "\033[0m";
static const string RED = "\033[31m";
static const string GRN = "\033[32m";
static const string YLW = "\033[33m";
static const string CYN = "\033[36m";
static const string BLD = "\033[1m";

// ─── Helpers ──────────────────────────────────────────────────────────────
static string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

static int visLen(const string& s) {
    int n = 0; bool esc = false;
    for (unsigned char c : s) {
        if (c == 27) { esc = true; continue; }
        if (esc) { if (c == 'm') esc = false; continue; }
        if ((c & 0xC0) != 0x80) n++;
    }
    return n;
}

static string padCell(const string& s, int w) {
    string r = s;
    int vl = visLen(s);
    for (int i = vl; i < w; i++) r += ' ';
    return r;
}

static string badge(const string& st) {
    if (st == "Pending")          return YLW + "● Pending"          + RST;
    if (st == "UnderInspection")  return CYN + "● Under Inspection" + RST;
    if (st == "Approved")         return GRN + "● Approved"         + RST;
    if (st == "Rejected")         return RED + "● Rejected"         + RST;
    if (st == "Active")           return GRN + "● Active"           + RST;
    if (st == "Expired")          return YLW + "● Expired"          + RST;
    if (st == "Cancelled")        return RED + "● Cancelled"        + RST;
    return st;
}

static void tblTop(const vector<int>& w) {
    cout << "╔";
    for (size_t i = 0; i < w.size(); i++) {
        for (int j = 0; j < w[i]+2; j++) cout << "═";
        cout << (i < w.size()-1 ? "╦" : "╗");
    }
    cout << "\n";
}
static void tblMid(const vector<int>& w) {
    cout << "╠";
    for (size_t i = 0; i < w.size(); i++) {
        for (int j = 0; j < w[i]+2; j++) cout << "═";
        cout << (i < w.size()-1 ? "╬" : "╣");
    }
    cout << "\n";
}
static void tblBot(const vector<int>& w) {
    cout << "╚";
    for (size_t i = 0; i < w.size(); i++) {
        for (int j = 0; j < w[i]+2; j++) cout << "═";
        cout << (i < w.size()-1 ? "╩" : "╝");
    }
    cout << "\n";
}
static void tblRow(const vector<string>& c, const vector<int>& w) {
    cout << "║";
    for (size_t i = 0; i < c.size(); i++)
        cout << " " << padCell(c[i], w[i]) << " ║";
    cout << "\n";
}
static void tblHdr(const vector<string>& h, const vector<int>& w) {
    tblTop(w); tblRow(h, w); tblMid(w);
}

static string todayStr() {
    time_t t = time(nullptr);
    tm* lt = localtime(&t);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", lt);
    return string(buf);
}

static void pause() {
    cout << "\nPress Enter to continue...";
    string dummy; getline(cin, dummy);
}

static bool checkCNIC(const string& s) {
    if (s.length() != 13) return false;
    for (char c : s) if (!isdigit((unsigned char)c)) return false;
    return true;
}
static bool checkPhone(const string& s) {
    if (s.length() != 11) return false;
    if (s.substr(0,2) != "03") return false;
    for (char c : s) if (!isdigit((unsigned char)c)) return false;
    return true;
}

// ─── Constructor ────────────────────────────────────────────────────────────
SalesmanUI::SalesmanUI(InsuranceSystem& sys) : sys(sys) {}

// ─── Menu ────────────────────────────────────────────────────────────────────
void SalesmanUI::run(Staff* staff) {
    string name = staff ? staff->getName() : "Salesman";
    while (true) {
        cout << "\n" << CYN << BLD
             << "╔══════════════════════════════╗\n"
             << "║     SALESMAN PANEL  — " << name
             << string(max(0, (int)(8 - name.size())), ' ') << "      ║\n"
             << "╠══════════════════════════════╣\n"
             << RST
             << "║ 1. Register New Customer     ║\n"
             << "║ 2. Add Vehicle               ║\n"
             << "║ 3. Issue Insurance Policy    ║\n"
             << "║ 4. File a Claim              ║\n"
             << "║ 5. View All Customers        ║\n"
             << "║ 6. View Policy Details       ║\n"
             << "║ 0. Logout                    ║\n"
             << "╚══════════════════════════════╝\n";
        cout << "Choice: ";
        string ch; getline(cin, ch);
        int n = -1;
        try { n = stoi(trim(ch)); } catch (...) {}
        switch (n) {
            case 1: doRegisterCustomer(); break;
            case 2: doAddVehicle(); break;
            case 3: doIssuePolicy(); break;
            case 4: doFileClaim(); break;
            case 5: doViewAllCustomers(); break;
            case 6: doViewPolicyDetails(); break;
            case 0: cout << YLW << "Logging out...\n" << RST; return;
            default: cout << RED << "Invalid choice.\n" << RST;
        }
    }
}

// ─── Register Customer ────────────────────────────────────────────────────
void SalesmanUI::doRegisterCustomer() {
    cout << "\n" << CYN << BLD << "─── Register New Customer ───\n" << RST;
    string name, cnic, phone, address;

    cout << "Enter full name: ";
    getline(cin, name); name = trim(name);
    if (name.empty()) { cout << RED << "Name cannot be empty.\n" << RST; pause(); return; }

    while (true) {
        cout << "Enter CNIC (13 digits): ";
        getline(cin, cnic); cnic = trim(cnic);
        if (checkCNIC(cnic)) break;
        cout << RED << "  Invalid CNIC. Must be exactly 13 digits.\n" << RST;
    }
    while (true) {
        cout << "Enter phone (11 digits, starts with 03): ";
        getline(cin, phone); phone = trim(phone);
        if (checkPhone(phone)) break;
        cout << RED << "  Invalid phone. Must be 11 digits starting with 03.\n" << RST;
    }
    cout << "Enter address: ";
    getline(cin, address); address = trim(address);

    string id = sys.registerCustomer(name, cnic, phone, address, todayStr());
    if (!id.empty())
        cout << GRN << "✓ Customer registered successfully. ID: " << id << RST << "\n";
    else
        cout << RED << "✗ Failed to register customer. Check inputs.\n" << RST;
    pause();
}

// ─── Add Vehicle ─────────────────────────────────────────────────────────
void SalesmanUI::doAddVehicle() {
    cout << "\n" << CYN << BLD << "─── Add Vehicle ───\n" << RST;
    string custID, plate, make, model, yearStr;

    cout << "Enter customer ID (or 1 to list all customers): ";
    getline(cin, custID); custID = trim(custID);
    if (custID == "1") {
        doViewAllCustomers();
        cout << "Enter customer ID: ";
        getline(cin, custID); custID = trim(custID);
    }
    Customer c = sys.getCustomerById(custID);
    if (c.getId().empty()) {
        cout << RED << "✗ Customer not found.\n" << RST; pause(); return;
    }
    cout << GRN << "  Customer: " << c.getName() << RST << "\n";

    cout << "Enter license plate: ";
    getline(cin, plate); plate = trim(plate);
    cout << "Enter make (Toyota/Honda/etc.): ";
    getline(cin, make); make = trim(make);
    cout << "Enter model: ";
    getline(cin, model); model = trim(model);

    int year = 0;
    while (true) {
        cout << "Enter year (1980-2026): ";
        getline(cin, yearStr); yearStr = trim(yearStr);
        try {
            year = stoi(yearStr);
            if (year >= 1980 && year <= 2026) break;
        } catch (...) {}
        cout << RED << "  Invalid year. Must be between 1980 and 2026.\n" << RST;
    }

    string id = sys.addVehicle(plate, make, model, year, custID);
    if (!id.empty())
        cout << GRN << "✓ Vehicle added. ID: " << id << RST << "\n";
    else
        cout << RED << "✗ Failed to add vehicle.\n" << RST;
    pause();
}

// ─── Issue Policy ─────────────────────────────────────────────────────────
void SalesmanUI::doIssuePolicy() {
    cout << "\n" << CYN << BLD << "─── Issue Insurance Policy ───\n" << RST;
    string custID;
    cout << "Enter customer ID: ";
    getline(cin, custID); custID = trim(custID);
    Customer c = sys.getCustomerById(custID);
    if (c.getId().empty()) {
        cout << RED << "✗ Customer not found.\n" << RST; pause(); return;
    }
    auto vehicles = sys.getVehiclesByCustomer(custID);
    if (vehicles.empty()) {
        cout << YLW << "  No vehicles for this customer.\n" << RST; pause(); return;
    }
    cout << "\n  Vehicles for " << c.getName() << ":\n";
    vector<int> vw = {7, 12, 10, 12, 5};
    tblHdr({"ID","Plate","Make","Model","Year"}, vw);
    for (const auto& v : vehicles)
        tblRow({v.getId(), v.getLicensePlate(), v.getMake(), v.getModel(), to_string(v.getYear())}, vw);
    tblBot(vw);

    string vehID;
    cout << "Enter vehicle ID: ";
    getline(cin, vehID); vehID = trim(vehID);
    bool owned = false;
    for (const auto& v : vehicles) if (v.getId() == vehID) { owned = true; break; }
    if (!owned) { cout << RED << "✗ Vehicle not found for this customer.\n" << RST; pause(); return; }

    cout << "Policy type:\n  1. Comprehensive\n  2. Third-Party\n  3. Fire & Theft\n";
    cout << "Choice: ";
    string pch; getline(cin, pch);
    string ptype;
    if (trim(pch) == "1") ptype = "Comprehensive";
    else if (trim(pch) == "2") ptype = "Third-Party";
    else if (trim(pch) == "3") ptype = "Fire&Theft";
    else { cout << RED << "Invalid type.\n" << RST; pause(); return; }

    string sdate, edate, premStr;
    while (true) {
        cout << "Enter start date (YYYY-MM-DD): ";
        getline(cin, sdate); sdate = trim(sdate);
        if (sdate.length() == 10 && sdate[4] == '-' && sdate[7] == '-') break;
        cout << RED << "  Invalid date format.\n" << RST;
    }
    while (true) {
        cout << "Enter end date (YYYY-MM-DD): ";
        getline(cin, edate); edate = trim(edate);
        if (edate.length() == 10 && edate[4] == '-' && edate[7] == '-' && edate > sdate) break;
        cout << RED << "  Invalid date or end date not after start date.\n" << RST;
    }
    double prem = 0;
    while (true) {
        cout << "Enter annual premium (PKR): ";
        getline(cin, premStr); premStr = trim(premStr);
        try { prem = stod(premStr); if (prem > 0) break; } catch (...) {}
        cout << RED << "  Invalid amount.\n" << RST;
    }

    string id = sys.issuePolicy(custID, vehID, ptype, sdate, edate, prem);
    if (!id.empty())
        cout << GRN << "✓ Policy issued. ID: " << id << RST << "\n";
    else
        cout << RED << "✗ Failed to issue policy.\n" << RST;
    pause();
}

// ─── File Claim ───────────────────────────────────────────────────────────
void SalesmanUI::doFileClaim() {
    cout << "\n" << CYN << BLD << "─── File a Claim ───\n" << RST;
    string custID;
    cout << "Enter customer ID: ";
    getline(cin, custID); custID = trim(custID);
    Customer c = sys.getCustomerById(custID);
    if (c.getId().empty()) {
        cout << RED << "✗ Customer not found.\n" << RST; pause(); return;
    }
    auto policies = sys.getPoliciesByCustomer(custID);
    vector<Policy> active;
    for (const auto& p : policies) if (p.isActive()) active.push_back(p);
    if (active.empty()) {
        cout << YLW << "  No active policies for this customer.\n" << RST; pause(); return;
    }
    cout << "\n  Active policies for " << c.getName() << ":\n";
    vector<int> pw = {7, 14, 12, 12, 12};
    tblHdr({"ID","Type","Start","End","Premium(PKR)"}, pw);
    for (const auto& p : active)
        tblRow({p.getId(), p.getType(), p.getStartDate(), p.getEndDate(),
                to_string((int)p.getPremium())}, pw);
    tblBot(pw);

    string polID;
    cout << "Enter policy ID: ";
    getline(cin, polID); polID = trim(polID);
    bool found = false;
    for (const auto& p : active) if (p.getId() == polID) { found = true; break; }
    if (!found) { cout << RED << "✗ Policy not found or not active.\n" << RST; pause(); return; }

    string idate, desc, amtStr;
    while (true) {
        cout << "Enter incident date (YYYY-MM-DD): ";
        getline(cin, idate); idate = trim(idate);
        if (idate.length() == 10 && idate[4] == '-' && idate[7] == '-') break;
        cout << RED << "  Invalid date format.\n" << RST;
    }
    cout << "Enter description: ";
    getline(cin, desc);
    double amt = 0;
    while (true) {
        cout << "Enter claim amount (PKR): ";
        getline(cin, amtStr); amtStr = trim(amtStr);
        try { amt = stod(amtStr); if (amt > 0) break; } catch (...) {}
        cout << RED << "  Invalid amount.\n" << RST;
    }

    string id = sys.fileClaim(polID, idate, desc, amt);
    if (!id.empty())
        cout << GRN << "✓ Claim filed. ID: " << id << RST << "\n";
    else
        cout << RED << "✗ Failed to file claim.\n" << RST;
    pause();
}

// ─── View All Customers ───────────────────────────────────────────────────
void SalesmanUI::doViewAllCustomers() {
    cout << "\n" << CYN << BLD << "─── All Customers ───\n" << RST;
    auto customers = sys.getAllCustomers();
    if (customers.empty()) { cout << YLW << "  No customers found.\n" << RST; return; }
    vector<int> w = {6, 20, 14, 12, 12};
    tblHdr({"ID","Name","CNIC","Phone","Registered"}, w);
    for (const auto& c : customers)
        tblRow({c.getId(), c.getName(), c.getCnic(), c.getPhone(), c.getDateRegistered()}, w);
    tblBot(w);
}

// ─── View Policy Details ──────────────────────────────────────────────────
void SalesmanUI::doViewPolicyDetails() {
    cout << "\n" << CYN << BLD << "─── View Policy Details ───\n" << RST;
    string custID;
    cout << "Enter customer ID: ";
    getline(cin, custID); custID = trim(custID);
    Customer c = sys.getCustomerById(custID);
    if (c.getId().empty()) {
        cout << RED << "✗ Customer not found.\n" << RST; pause(); return;
    }
    auto policies = sys.getPoliciesByCustomer(custID);
    if (policies.empty()) {
        cout << YLW << "  No policies for this customer.\n" << RST; pause(); return;
    }
    cout << "\n  Policies for " << c.getName() << ":\n";
    vector<int> w = {7, 14, 12, 12, 22, 12};
    tblHdr({"ID","Type","Start","End","Status","Premium"}, w);
    for (const auto& p : policies)
        tblRow({p.getId(), p.getType(), p.getStartDate(), p.getEndDate(),
                badge(p.getStatus()), to_string((long long)p.getPremium())}, w);
    tblBot(w);
    pause();
}
