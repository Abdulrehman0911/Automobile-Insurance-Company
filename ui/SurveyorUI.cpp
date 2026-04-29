#include "SurveyorUI.h"
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

SurveyorUI::SurveyorUI(InsuranceSystem& sys) : sys(sys) {}

void SurveyorUI::run(Staff* staff) {
    string surveyorId = staff ? staff->getId() : "";
    string name = staff ? staff->getName() : "Surveyor";
    while (true) {
        cout << "\n" << CYN << BLD
             << "╔══════════════════════════════════╗\n"
             << "║     SURVEYOR PANEL               ║\n"
             << "╠══════════════════════════════════╣\n"
             << RST
             << "║ 1. View My Assigned Claims       ║\n"
             << "║ 2. Submit Inspection Report      ║\n"
             << "║ 3. View My Submitted Inspections ║\n"
             << "║ 0. Logout                        ║\n"
             << "╚══════════════════════════════════╝\n";
        cout << "Welcome, " << name << ". Choice: ";
        string ch; getline(cin, ch);
        int n = -1;
        try { n = stoi(trim(ch)); } catch (...) {}
        switch (n) {
            case 1: doViewAssignedClaims(surveyorId); break;
            case 2: doSubmitInspection(surveyorId); break;
            case 3: doViewMyInspections(surveyorId); break;
            case 0: cout << YLW << "Logging out...\n" << RST; return;
            default: cout << RED << "Invalid choice.\n" << RST;
        }
    }
}

void SurveyorUI::doViewAssignedClaims(const string& surveyorId) {
    cout << "\n" << CYN << BLD << "─── My Assigned Claims ───\n" << RST;
    auto claims = sys.getClaimsForSurveyor(surveyorId);
    if (claims.empty()) {
        cout << YLW << "  No claims assigned to you.\n" << RST;
        pause(); return;
    }
    vector<int> w = {7, 7, 12, 22, 12};
    tblHdr({"ID","Policy","Date","Status","Amount(PKR)"}, w);
    for (const auto& c : claims)
        tblRow({c.getId(), c.getPolicyID(), c.getIncidentDate(),
                badge(c.getStatus()), to_string((long long)c.getClaimAmount())}, w);
    tblBot(w);
    pause();
}

void SurveyorUI::doSubmitInspection(const string& surveyorId) {
    cout << "\n" << CYN << BLD << "─── Submit Inspection Report ───\n" << RST;
    auto claims = sys.getClaimsForSurveyor(surveyorId);
    vector<Claim> pending;
    for (const auto& c : claims) if (c.getStatus() == "Pending") pending.push_back(c);
    if (pending.empty()) {
        cout << YLW << "  No pending claims assigned to you.\n" << RST;
        pause(); return;
    }
    cout << "  Pending claims assigned to you:\n";
    vector<int> cw = {7, 7, 12, 30};
    tblHdr({"ID","Policy","Date","Description"}, cw);
    for (const auto& c : pending) {
        string desc = c.getDescription();
        if ((int)desc.size() > 28) desc = desc.substr(0, 27) + "…";
        tblRow({c.getId(), c.getPolicyID(), c.getIncidentDate(), desc}, cw);
    }
    tblBot(cw);

    string claimID;
    cout << "Enter claim ID to inspect: ";
    getline(cin, claimID); claimID = trim(claimID);
    bool found = false;
    for (const auto& c : pending) if (c.getId() == claimID) { found = true; break; }
    if (!found) { cout << RED << "✗ Claim not found in your pending list.\n" << RST; pause(); return; }

    auto workshops = sys.getAllWorkshops();
    if (workshops.empty()) { cout << RED << "✗ No workshops available.\n" << RST; pause(); return; }
    cout << "\n  Registered workshops:\n";
    for (size_t i = 0; i < workshops.size(); i++)
        cout << "  " << (i+1) << ". " << workshops[i].getName()
             << " — " << workshops[i].getAddress() << "\n";
    string wch;
    int wn = 0;
    while (true) {
        cout << "Enter workshop number: ";
        getline(cin, wch); wch = trim(wch);
        try {
            wn = stoi(wch);
            if (wn >= 1 && wn <= (int)workshops.size()) break;
        } catch (...) {}
        cout << RED << "  Invalid selection.\n" << RST;
    }
    string workshopID = workshops[wn-1].getId();

    string findings, lossStr, idate;
    cout << "Enter findings: ";
    getline(cin, findings);
    double loss = 0;
    while (true) {
        cout << "Enter estimated loss (PKR): ";
        getline(cin, lossStr); lossStr = trim(lossStr);
        try { loss = stod(lossStr); if (loss > 0) break; } catch (...) {}
        cout << RED << "  Invalid amount.\n" << RST;
    }
    while (true) {
        cout << "Enter inspection date (YYYY-MM-DD): ";
        getline(cin, idate); idate = trim(idate);
        if (idate.length() == 10 && idate[4] == '-' && idate[7] == '-') break;
        cout << RED << "  Invalid date format.\n" << RST;
    }

    if (sys.submitInspection(claimID, surveyorId, workshopID, findings, idate, loss))
        cout << GRN << "✓ Inspection report submitted.\n" << RST;
    else
        cout << RED << "✗ Failed to submit inspection. Check claim assignment.\n" << RST;
    pause();
}

void SurveyorUI::doViewMyInspections(const string& surveyorId) {
    cout << "\n" << CYN << BLD << "─── My Submitted Inspections ───\n" << RST;
    auto inspections = sys.getInspectionsBySurveyor(surveyorId);
    if (inspections.empty()) {
        cout << YLW << "  No inspections submitted yet.\n" << RST;
        pause(); return;
    }
    vector<int> w = {7, 7, 7, 12, 12, 12};
    tblHdr({"ID","Claim","Workshop","Date","Est. Loss","Status"}, w);
    for (const auto& ins : inspections)
        tblRow({ins.getId(), ins.getClaimID(), ins.getWorkshopID(), ins.getDate(),
                to_string((long long)ins.getEstimatedLoss()), ins.getStatus()}, w);
    tblBot(w);
    pause();
}
