#include "StaffRepo.h"
#include <fstream>
#include <sstream>
using namespace std;

static string trimST(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

vector<Staff> StaffRepo::loadAll() {
    vector<Staff> result;
    { ofstream touch("data/staff.txt", ios::app); }
    ifstream file("data/staff.txt");
    if (!file.is_open()) return result;
    string line;
    while (getline(file, line)) {
        line = trimST(line);
        if (line.empty()) continue;
        istringstream ss(line);
        string id, name, username, password, role;
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, username, '|');
        getline(ss, password, '|');
        getline(ss, role);
        result.emplace_back(trimST(id), trimST(name), trimST(username), trimST(password), trimST(role));
    }
    return result;
}

void StaffRepo::saveAll(const vector<Staff>& staff) {
    ofstream file("data/staff.txt");
    for (const auto& s : staff)
        file << s.getId() << "|" << s.getName() << "|" << s.getUsername() << "|"
             << s.getPassword() << "|" << s.getRole() << "\n";
}

Staff StaffRepo::findById(const string& id) {
    for (const auto& s : loadAll())
        if (s.getId() == id) return s;
    return Staff();
}

string StaffRepo::add(const Staff& s) {
    auto all = loadAll();
    string newId;
    if (all.empty()) {
        newId = "ST001";
    } else {
        int num = stoi(all.back().getId().substr(2));
        num++;
        string ns = to_string(num);
        while (ns.length() < 3) ns = "0" + ns;
        newId = "ST" + ns;
    }
    Staff ns2 = s;
    ns2.setId(newId);
    all.push_back(ns2);
    saveAll(all);
    return newId;
}
