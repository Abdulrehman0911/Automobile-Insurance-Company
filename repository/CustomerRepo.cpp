#include "CustomerRepo.h"
#include <fstream>
#include <sstream>
using namespace std;

static string trimC(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

vector<Customer> CustomerRepo::loadAll() {
    vector<Customer> result;
    { ofstream touch("data/customers.txt", ios::app); }
    ifstream file("data/customers.txt");
    if (!file.is_open()) return result;
    string line;
    while (getline(file, line)) {
        line = trimC(line);
        if (line.empty()) continue;
        istringstream ss(line);
        string id, name, cnic, phone, address, dateReg;
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, cnic, '|');
        getline(ss, phone, '|');
        getline(ss, address, '|');
        getline(ss, dateReg);
        result.emplace_back(trimC(id), trimC(name), trimC(cnic), trimC(phone), trimC(address), trimC(dateReg));
    }
    return result;
}

void CustomerRepo::saveAll(const vector<Customer>& customers) {
    ofstream file("data/customers.txt");
    for (const auto& c : customers)
        file << c.getId() << "|" << c.getName() << "|" << c.getCnic() << "|"
             << c.getPhone() << "|" << c.getAddress() << "|" << c.getDateRegistered() << "\n";
}

Customer CustomerRepo::findById(const string& id) {
    for (const auto& c : loadAll())
        if (c.getId() == id) return c;
    return Customer();
}

string CustomerRepo::add(const Customer& c) {
    auto all = loadAll();
    string newId;
    if (all.empty()) {
        newId = "C001";
    } else {
        int num = stoi(all.back().getId().substr(1));
        num++;
        string ns = to_string(num);
        while (ns.length() < 3) ns = "0" + ns;
        newId = "C" + ns;
    }
    Customer nc = c;
    nc.setId(newId);
    all.push_back(nc);
    saveAll(all);
    return newId;
}
