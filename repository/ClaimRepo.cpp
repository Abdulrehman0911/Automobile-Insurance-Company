#include "ClaimRepo.h"
#include <fstream>
#include <sstream>
using namespace std;

static string trimCL(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

vector<Claim> ClaimRepo::loadAll() {
    vector<Claim> result;
    { ofstream touch("data/claims.txt", ios::app); }
    ifstream file("data/claims.txt");
    if (!file.is_open()) return result;
    string line;
    while (getline(file, line)) {
        line = trimCL(line);
        if (line.empty()) continue;
        istringstream ss(line);
        string id, polID, survID, date, desc, status, amtStr;
        getline(ss, id, '|');
        getline(ss, polID, '|');
        getline(ss, survID, '|');
        getline(ss, date, '|');
        getline(ss, desc, '|');
        getline(ss, status, '|');
        getline(ss, amtStr);
        double amt = 0.0;
        try { amt = stod(trimCL(amtStr)); } catch (...) {}
        result.emplace_back(trimCL(id), trimCL(polID), trimCL(survID),
                            trimCL(date), trimCL(desc), trimCL(status), amt);
    }
    return result;
}

void ClaimRepo::saveAll(const vector<Claim>& claims) {
    ofstream file("data/claims.txt");
    for (const auto& c : claims)
        file << c.getId() << "|" << c.getPolicyID() << "|" << c.getSurveyorID() << "|"
             << c.getIncidentDate() << "|" << c.getDescription() << "|"
             << c.getStatus() << "|" << c.getClaimAmount() << "\n";
}

Claim ClaimRepo::findById(const string& id) {
    for (const auto& c : loadAll())
        if (c.getId() == id) return c;
    return Claim();
}

string ClaimRepo::add(const Claim& c) {
    auto all = loadAll();
    string newId;
    if (all.empty()) {
        newId = "CL001";
    } else {
        int num = stoi(all.back().getId().substr(2));
        num++;
        string ns = to_string(num);
        while (ns.length() < 3) ns = "0" + ns;
        newId = "CL" + ns;
    }
    Claim nc = c;
    nc.setId(newId);
    all.push_back(nc);
    saveAll(all);
    return newId;
}
