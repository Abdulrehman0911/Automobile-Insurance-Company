#include "ManagerUI.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <ctime>
#include <cctype>
using namespace std;

static const string RST = "\033[0m";
static const string RED = "\033[31m";
static const string GRN = "\033[32m";
static const string YLW = "\033[33m";
static const string CYN = "\033[36m";
static const string BLD = "\033[1m";

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
    string r = s; int vl = visLen(s);
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
static void pause() {
    cout << "\nPress Enter to continue...";
    string d; getline(cin, d);
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
static string todayStr() {
    time_t t = time(nullptr);
    tm* lt = localtime(&t);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", lt);
    return string(buf);
}

// ─── Constructor ─────────────────────────────────────────────────────────────
ManagerUI::ManagerUI(InsuranceSystem& sys) : sys(sys) {}

// ─── Menu ────────────────────────────────────────────────────────────────────
void ManagerUI::run(Staff* staff) {
    string name = staff ? staff->getName() : "Manager";
    while (true) {
        cout << "\n" << CYN << BLD
             << "╔══════════════════════════════════════╗\n"
             << "║     MANAGER PANEL                    ║\n"
             << "╠══════════════════════════════════════╣\n"
             << RST
             << "║  --- Claims ---                      ║\n"
             << "║  1. View All Pending Claims          ║\n"
             << "║  2. Assign Surveyor to Claim         ║\n"
             << "║  3. View Inspection Report           ║\n"
             << "║  4. Approve Claim                    ║\n"
             << "║  5. Reject Claim                     ║\n"
             << "║  --- Customer & Policy ---           ║\n"
             << "║  6. Register New Customer            ║\n"
             << "║  7. Add Vehicle                      ║\n"
             << "║  8. Issue Policy                     ║\n"
             << "║  9. View All Customers               ║\n"
             << "║ 10. Customer Claim History           ║\n"
             << "║  --- Staff & Workshops ---           ║\n"
             << "║ 11. Add Staff Member                 ║\n"
             << "║ 12. View All Staff                   ║\n"
             << "║ 13. Add Workshop                     ║\n"
             << "║ 14. View All Workshops               ║\n"
             << "║  --- Reports ---                     ║\n"
             << "║ 15. New Customers This Month         ║\n"
             << "║ 16. All Inspection Reports           ║\n"
             << "║  0. Logout                           ║\n"
             << "╚══════════════════════════════════════╝\n";
        cout << "Welcome, " << name << ". Choice: ";
        string ch; getline(cin, ch);
        int n = -1;
        try { n = stoi(trim(ch)); } catch (...) {}
        switch (n) {
            case 1:  doViewPendingClaims(); break;
            case 2:  doAssignSurveyor(); break;
            case 3:  doViewInspectionReport(); break;
            case 4:  doApproveClaim(); break;
            case 5:  doRejectClaim(); break;
            case 6:  doRegisterCustomer(); break;
            case 7:  doAddVehicle(); break;
            case 8:  doIssuePolicy(); break;
            case 9:  doViewAllCustomers(); break;
            case 10: doCustomerClaimHistory(); break;
            case 11: doAddStaff(); break;
            case 12: doViewAllStaff(); break;
            case 13: doAddWorkshop(); break;
            case 14: doViewAllWorkshops(); break;
            case 15: doNewCustomersThisMonth(); break;
            case 16: doAllInspectionReports(); break;
            case 0:  cout << YLW << "Logging out...\n" << RST; return;
            default: cout << RED << "Invalid choice.\n" << RST;
        }
    }
}

// ─── 1. View Pending Claims ──────────────────────────────────────────────────
void ManagerUI::doViewPendingClaims() {
    cout << "\n" << CYN << BLD << "─── Pending Claims ───\n" << RST;
    auto claims = sys.getPendingClaims();
    if (claims.empty()) { cout << YLW << "  No pending claims.\n" << RST; pause(); return; }
    vector<int> w = {7, 7, 12, 22, 12, 10};
    tblHdr({"ID","Policy","Date","Status","Amount","Surveyor"}, w);
    for (const auto& c : claims)
        tblRow({c.getId(), c.getPolicyID(), c.getIncidentDate(), badge(c.getStatus()),
                to_string((long long)c.getClaimAmount()),
                c.getSurveyorID().empty() ? "Unassigned" : c.getSurveyorID()}, w);
    tblBot(w);
    pause();
}

// ─── 2. Assign Surveyor ──────────────────────────────────────────────────────
void ManagerUI::doAssignSurveyor() {
    cout << "\n" << CYN << BLD << "─── Assign Surveyor to Claim ───\n" << RST;
    auto claims = sys.getPendingClaims();
    if (claims.empty()) { cout << YLW << "  No pending claims.\n" << RST; pause(); return; }
    vector<int> cw = {7, 7, 12, 10};
    tblHdr({"ID","Policy","Date","Surveyor"}, cw);
    for (const auto& c : claims)
        tblRow({c.getId(), c.getPolicyID(), c.getIncidentDate(),
                c.getSurveyorID().empty() ? "Unassigned" : c.getSurveyorID()}, cw);
    tblBot(cw);

    string claimID;
    cout << "Enter claim ID: ";
    getline(cin, claimID); claimID = trim(claimID);

    auto surveyors = sys.getSurveyors();
    if (surveyors.empty()) { cout << RED << "✗ No surveyors available.\n" << RST; pause(); return; }
    cout << "\n  Available surveyors:\n";
    vector<int> sw = {7, 20};
    tblHdr({"ID","Name"}, sw);
    for (const auto& s : surveyors) tblRow({s.getId(), s.getName()}, sw);
    tblBot(sw);

    string survID;
    cout << "Enter surveyor ID: ";
    getline(cin, survID); survID = trim(survID);

    if (sys.assignSurveyor(claimID, survID))
        cout << GRN << "✓ Surveyor assigned successfully.\n" << RST;
    else
        cout << RED << "✗ Failed to assign. Check claim ID and surveyor ID.\n" << RST;
    pause();
}

// ─── 3. View Inspection Report ───────────────────────────────────────────────
void ManagerUI::doViewInspectionReport() {
    cout << "\n" << CYN << BLD << "─── View Inspection Report ───\n" << RST;
    string claimID;
    cout << "Enter claim ID: ";
    getline(cin, claimID); claimID = trim(claimID);
    auto ins = sys.getInspectionForClaim(claimID);
    if (ins.getId().empty()) {
        cout << RED << "✗ No inspection report found for claim " << claimID << ".\n" << RST;
        pause(); return;
    }
    cout << "\n" << CYN
         << "  ┌─────────────────────────────────────────┐\n"
         << "  │         INSPECTION REPORT               │\n"
         << "  ├─────────────────────────────────────────┤\n"
         << RST;
    cout << "  │ Inspection ID : " << ins.getId() << "\n";
    cout << "  │ Claim ID      : " << ins.getClaimID() << "\n";
    cout << "  │ Surveyor ID   : " << ins.getSurveyorID() << "\n";
    cout << "  │ Workshop ID   : " << ins.getWorkshopID() << "\n";
    cout << "  │ Date          : " << ins.getDate() << "\n";
    cout << "  │ Est. Loss     : PKR " << ins.getEstimatedLoss() << "\n";
    cout << "  │ Status        : " << ins.getStatus() << "\n";
    cout << "  │ Findings      : " << ins.getFindings() << "\n";
    cout << CYN << "  └─────────────────────────────────────────┘\n" << RST;
    pause();
}

// ─── 4. Approve Claim ───────────────────────────────────────────────────────
void ManagerUI::doApproveClaim() {
    cout << "\n" << CYN << BLD << "─── Approve Claim ───\n" << RST;
    auto claims = sys.getAllClaims();
    vector<Claim> eligible;
    for (const auto& c : claims)
        if (c.getStatus() == "UnderInspection") eligible.push_back(c);
    if (eligible.empty()) {
        cout << YLW << "  No claims under inspection.\n" << RST; pause(); return;
    }
    vector<int> w = {7, 7, 22, 12};
    tblHdr({"ID","Policy","Status","Amount"}, w);
    for (const auto& c : eligible)
        tblRow({c.getId(), c.getPolicyID(), badge(c.getStatus()),
                to_string((long long)c.getClaimAmount())}, w);
    tblBot(w);

    string claimID;
    cout << "Enter claim ID: ";
    getline(cin, claimID); claimID = trim(claimID);

    auto ins = sys.getInspectionForClaim(claimID);
    if (ins.getId().empty()) {
        cout << RED << "✗ No inspection report found. Cannot approve.\n" << RST;
        pause(); return;
    }
    cout << "\n  Inspection: " << ins.getId()
         << "  Est. Loss: PKR " << ins.getEstimatedLoss()
         << "  Findings: " << ins.getFindings() << "\n";
    cout << "Confirm approval? (y/n): ";
    string ans; getline(cin, ans);
    if (trim(ans) == "y" || trim(ans) == "Y") {
        if (sys.approveClaim(claimID))
            cout << GRN << "✓ Claim " << claimID << " approved.\n" << RST;
        else
            cout << RED << "✗ Failed to approve claim.\n" << RST;
    } else {
        cout << YLW << "  Approval cancelled.\n" << RST;
    }
    pause();
}

// ─── 5. Reject Claim ────────────────────────────────────────────────────────
void ManagerUI::doRejectClaim() {
    cout << "\n" << CYN << BLD << "─── Reject Claim ───\n" << RST;
    auto claims = sys.getAllClaims();
    vector<Claim> eligible;
    for (const auto& c : claims)
        if (c.getStatus() == "UnderInspection") eligible.push_back(c);
    if (eligible.empty()) {
        cout << YLW << "  No claims under inspection.\n" << RST; pause(); return;
    }
    vector<int> w = {7, 7, 22, 12};
    tblHdr({"ID","Policy","Status","Amount"}, w);
    for (const auto& c : eligible)
        tblRow({c.getId(), c.getPolicyID(), badge(c.getStatus()),
                to_string((long long)c.getClaimAmount())}, w);
    tblBot(w);

    string claimID;
    cout << "Enter claim ID: ";
    getline(cin, claimID); claimID = trim(claimID);
    auto ins = sys.getInspectionForClaim(claimID);
    if (ins.getId().empty()) {
        cout << RED << "✗ No inspection report found. Cannot reject.\n" << RST;
        pause(); return;
    }
    cout << "Confirm rejection? (y/n): ";
    string ans; getline(cin, ans);
    if (trim(ans) == "y" || trim(ans) == "Y") {
        if (sys.rejectClaim(claimID))
            cout << GRN << "✓ Claim " << claimID << " rejected.\n" << RST;
        else
            cout << RED << "✗ Failed to reject claim.\n" << RST;
    } else {
        cout << YLW << "  Rejection cancelled.\n" << RST;
    }
    pause();
}

// ─── 6. Register Customer ───────────────────────────────────────────────────
void ManagerUI::doRegisterCustomer() {
    cout << "\n" << CYN << BLD << "─── Register New Customer ───\n" << RST;
    string name, cnic, phone, address;
    cout << "Enter full name: ";
    getline(cin, name); name = trim(name);
    if (name.empty()) { cout << RED << "Name cannot be empty.\n" << RST; pause(); return; }
    while (true) {
        cout << "Enter CNIC (13 digits): ";
        getline(cin, cnic); cnic = trim(cnic);
        if (checkCNIC(cnic)) break;
        cout << RED << "  Invalid CNIC.\n" << RST;
    }
    while (true) {
        cout << "Enter phone (11 digits, starts with 03): ";
        getline(cin, phone); phone = trim(phone);
        if (checkPhone(phone)) break;
        cout << RED << "  Invalid phone.\n" << RST;
    }
    cout << "Enter address: ";
    getline(cin, address); address = trim(address);
    string id = sys.registerCustomer(name, cnic, phone, address, todayStr());
    if (!id.empty()) cout << GRN << "✓ Customer registered. ID: " << id << RST << "\n";
    else             cout << RED << "✗ Failed to register customer.\n" << RST;
    pause();
}

// ─── 7. Add Vehicle ─────────────────────────────────────────────────────────
void ManagerUI::doAddVehicle() {
    cout << "\n" << CYN << BLD << "─── Add Vehicle ───\n" << RST;
    string custID, plate, make, model, yearStr;
    cout << "Enter customer ID: ";
    getline(cin, custID); custID = trim(custID);
    Customer c = sys.getCustomerById(custID);
    if (c.getId().empty()) { cout << RED << "✗ Customer not found.\n" << RST; pause(); return; }
    cout << GRN << "  Customer: " << c.getName() << RST << "\n";
    cout << "Enter license plate: ";
    getline(cin, plate); plate = trim(plate);
    cout << "Enter make: ";
    getline(cin, make); make = trim(make);
    cout << "Enter model: ";
    getline(cin, model); model = trim(model);
    int year = 0;
    while (true) {
        cout << "Enter year (1980-2026): ";
        getline(cin, yearStr); yearStr = trim(yearStr);
        try { year = stoi(yearStr); if (year >= 1980 && year <= 2026) break; } catch (...) {}
        cout << RED << "  Invalid year.\n" << RST;
    }
    string id = sys.addVehicle(plate, make, model, year, custID);
    if (!id.empty()) cout << GRN << "✓ Vehicle added. ID: " << id << RST << "\n";
    else             cout << RED << "✗ Failed to add vehicle.\n" << RST;
    pause();
}

// ─── 8. Issue Policy ────────────────────────────────────────────────────────
void ManagerUI::doIssuePolicy() {
    cout << "\n" << CYN << BLD << "─── Issue Insurance Policy ───\n" << RST;
    string custID;
    cout << "Enter customer ID: ";
    getline(cin, custID); custID = trim(custID);
    Customer c = sys.getCustomerById(custID);
    if (c.getId().empty()) { cout << RED << "✗ Customer not found.\n" << RST; pause(); return; }
    auto vehicles = sys.getVehiclesByCustomer(custID);
    if (vehicles.empty()) { cout << YLW << "  No vehicles for this customer.\n" << RST; pause(); return; }
    cout << "\n  Vehicles:\n";
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
    cout << "Policy type:\n  1. Comprehensive\n  2. Third-Party\n  3. Fire & Theft\nChoice: ";
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
        cout << RED << "  Invalid date.\n" << RST;
    }
    double prem = 0;
    while (true) {
        cout << "Enter premium (PKR): ";
        getline(cin, premStr); premStr = trim(premStr);
        try { prem = stod(premStr); if (prem > 0) break; } catch (...) {}
        cout << RED << "  Invalid amount.\n" << RST;
    }
    string id = sys.issuePolicy(custID, vehID, ptype, sdate, edate, prem);
    if (!id.empty()) cout << GRN << "✓ Policy issued. ID: " << id << RST << "\n";
    else             cout << RED << "✗ Failed to issue policy.\n" << RST;
    pause();
}

// ─── 9. View All Customers ──────────────────────────────────────────────────
void ManagerUI::doViewAllCustomers() {
    cout << "\n" << CYN << BLD << "─── All Customers ───\n" << RST;
    auto customers = sys.getAllCustomers();
    if (customers.empty()) { cout << YLW << "  No customers.\n" << RST; pause(); return; }
    vector<int> w = {6, 20, 14, 12, 12};
    tblHdr({"ID","Name","CNIC","Phone","Registered"}, w);
    for (const auto& c : customers)
        tblRow({c.getId(), c.getName(), c.getCnic(), c.getPhone(), c.getDateRegistered()}, w);
    tblBot(w);
    pause();
}

// ─── 10. Customer Claim History ─────────────────────────────────────────────
void ManagerUI::doCustomerClaimHistory() {
    cout << "\n" << CYN << BLD << "─── Customer Claim History ───\n" << RST;
    string custID;
    cout << "Enter customer ID: ";
    getline(cin, custID); custID = trim(custID);
    Customer c = sys.getCustomerById(custID);
    if (c.getId().empty()) { cout << RED << "✗ Customer not found.\n" << RST; pause(); return; }
    cout << GRN << "\n  Customer: " << c.getName()
         << "  CNIC: " << c.getCnic()
         << "  Phone: " << c.getPhone() << RST << "\n";
    auto claims = sys.getClaimsByCustomer(custID);
    if (claims.empty()) { cout << YLW << "  No claims for this customer.\n" << RST; pause(); return; }
    vector<int> cw = {7, 7, 12, 22, 12};
    tblHdr({"ClaimID","Policy","Date","Status","Amount"}, cw);
    for (const auto& cl : claims) {
        tblRow({cl.getId(), cl.getPolicyID(), cl.getIncidentDate(),
                badge(cl.getStatus()), to_string((long long)cl.getClaimAmount())}, cw);
        auto ins = sys.getInspectionForClaim(cl.getId());
        if (!ins.getId().empty()) {
            cout << "    " << CYN << "Inspection: " << ins.getId()
                 << "  Loss: PKR " << ins.getEstimatedLoss()
                 << "  Workshop: " << ins.getWorkshopID()
                 << "  Date: " << ins.getDate() << RST << "\n";
        }
    }
    tblBot(cw);
    pause();
}

// ─── 11. Add Staff ──────────────────────────────────────────────────────────
void ManagerUI::doAddStaff() {
    cout << "\n" << CYN << BLD << "─── Add Staff Member ───\n" << RST;
    string name, username, password, roleChoice, role;
    cout << "Enter full name: ";
    getline(cin, name); name = trim(name);
    cout << "Enter username: ";
    getline(cin, username); username = trim(username);
    cout << "Enter password: ";
    getline(cin, password); password = trim(password);
    cout << "Role:\n  1. Manager\n  2. Salesman\n  3. Surveyor\nChoice: ";
    getline(cin, roleChoice);
    string rc = trim(roleChoice);
    if (rc == "1") role = "Manager";
    else if (rc == "2") role = "Salesman";
    else if (rc == "3") role = "Surveyor";
    else { cout << RED << "Invalid role.\n" << RST; pause(); return; }
    if (sys.addStaff(name, username, password, role))
        cout << GRN << "✓ Staff member added.\n" << RST;
    else
        cout << RED << "✗ Failed to add staff.\n" << RST;
    pause();
}

// ─── 12. View All Staff ─────────────────────────────────────────────────────
void ManagerUI::doViewAllStaff() {
    cout << "\n" << CYN << BLD << "─── All Staff ───\n" << RST;
    auto staff = sys.getAllStaff();
    if (staff.empty()) { cout << YLW << "  No staff found.\n" << RST; pause(); return; }
    vector<int> w = {7, 20, 14, 10};
    tblHdr({"ID","Name","Username","Role"}, w);
    for (const auto& s : staff)
        tblRow({s.getId(), s.getName(), s.getUsername(), s.getRole()}, w);
    tblBot(w);
    pause();
}

// ─── 13. Add Workshop ───────────────────────────────────────────────────────
void ManagerUI::doAddWorkshop() {
    cout << "\n" << CYN << BLD << "─── Add Workshop ───\n" << RST;
    string name, address;
    cout << "Enter workshop name: ";
    getline(cin, name); name = trim(name);
    cout << "Enter address: ";
    getline(cin, address); address = trim(address);
    if (sys.addWorkshop(name, address, todayStr()))
        cout << GRN << "✓ Workshop added.\n" << RST;
    else
        cout << RED << "✗ Failed to add workshop.\n" << RST;
    pause();
}

// ─── 14. View All Workshops ─────────────────────────────────────────────────
void ManagerUI::doViewAllWorkshops() {
    cout << "\n" << CYN << BLD << "─── All Workshops ───\n" << RST;
    auto workshops = sys.getAllWorkshops();
    if (workshops.empty()) { cout << YLW << "  No workshops found.\n" << RST; pause(); return; }
    vector<int> w = {7, 25, 30, 12};
    tblHdr({"ID","Name","Address","Registered"}, w);
    for (const auto& wk : workshops)
        tblRow({wk.getId(), wk.getName(), wk.getAddress(), wk.getRegisteredDate()}, w);
    tblBot(w);
    pause();
}

// ─── 15. New Customers This Month ───────────────────────────────────────────
void ManagerUI::doNewCustomersThisMonth() {
    cout << "\n" << CYN << BLD << "─── New Customers This Month ───\n" << RST;
    cout << "Enter month (YYYY-MM, or press Enter for current month): ";
    string ym; getline(cin, ym); ym = trim(ym);
    if (ym.empty()) {
        time_t t = time(nullptr);
        tm* lt = localtime(&t);
        char buf[8];
        strftime(buf, sizeof(buf), "%Y-%m", lt);
        ym = string(buf);
    }
    auto customers = sys.getNewCustomersInMonth(ym);
    cout << "\n  New customers for " << ym << ": " << customers.size() << "\n";
    if (!customers.empty()) {
        vector<int> w = {6, 20, 14, 12};
        tblHdr({"ID","Name","CNIC","Registered"}, w);
        for (const auto& c : customers)
            tblRow({c.getId(), c.getName(), c.getCnic(), c.getDateRegistered()}, w);
        tblBot(w);
    }
    pause();
}

// ─── 16. All Inspection Reports ─────────────────────────────────────────────
void ManagerUI::doAllInspectionReports() {
    cout << "\n" << CYN << BLD << "─── All Inspection Reports ───\n" << RST;
    auto inspections = sys.getAllInspections();
    if (inspections.empty()) { cout << YLW << "  No inspection reports.\n" << RST; pause(); return; }
    vector<int> w = {7, 7, 7, 7, 12, 12, 12};
    tblHdr({"ID","Claim","Surv.","Workshop","Date","Est.Loss","Status"}, w);
    for (const auto& ins : inspections)
        tblRow({ins.getId(), ins.getClaimID(), ins.getSurveyorID(), ins.getWorkshopID(),
                ins.getDate(), to_string((long long)ins.getEstimatedLoss()), ins.getStatus()}, w);
    tblBot(w);
    pause();
}
