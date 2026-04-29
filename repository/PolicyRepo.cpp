#include "PolicyRepo.h"
#include <fstream>
#include <sstream>
using namespace std;

static string trimP(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

vector<Policy> PolicyRepo::loadAll() {
    vector<Policy> result;
    { ofstream touch("data/policies.txt", ios::app); }
    ifstream file("data/policies.txt");
    if (!file.is_open()) return result;
    string line;
    while (getline(file, line)) {
        line = trimP(line);
        if (line.empty()) continue;
        istringstream ss(line);
        string id, custID, vehID, type, start, end, status, premStr;
        getline(ss, id, '|');
        getline(ss, custID, '|');
        getline(ss, vehID, '|');
        getline(ss, type, '|');
        getline(ss, start, '|');
        getline(ss, end, '|');
        getline(ss, status, '|');
        getline(ss, premStr);
        double prem = 0.0;
        try { prem = stod(trimP(premStr)); } catch (...) {}
        result.emplace_back(trimP(id), trimP(custID), trimP(vehID), trimP(type),
                            trimP(start), trimP(end), trimP(status), prem);
    }
    return result;
}

void PolicyRepo::saveAll(const vector<Policy>& policies) {
    ofstream file("data/policies.txt");
    for (const auto& p : policies)
        file << p.getId() << "|" << p.getCustomerID() << "|" << p.getVehicleID() << "|"
             << p.getType() << "|" << p.getStartDate() << "|" << p.getEndDate() << "|"
             << p.getStatus() << "|" << p.getPremium() << "\n";
}

Policy PolicyRepo::findById(const string& id) {
    for (const auto& p : loadAll())
        if (p.getId() == id) return p;
    return Policy();
}

string PolicyRepo::add(const Policy& p) {
    auto all = loadAll();
    string newId;
    if (all.empty()) {
        newId = "P001";
    } else {
        int num = stoi(all.back().getId().substr(1));
        num++;
        string ns = to_string(num);
        while (ns.length() < 3) ns = "0" + ns;
        newId = "P" + ns;
    }
    Policy np = p;
    np.setId(newId);
    all.push_back(np);
    saveAll(all);
    return newId;
}
