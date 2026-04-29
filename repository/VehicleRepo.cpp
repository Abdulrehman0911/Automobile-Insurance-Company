#include "VehicleRepo.h"
#include <fstream>
#include <sstream>
using namespace std;

static string trimV(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

vector<Vehicle> VehicleRepo::loadAll() {
    vector<Vehicle> result;
    { ofstream touch("data/vehicles.txt", ios::app); }
    ifstream file("data/vehicles.txt");
    if (!file.is_open()) return result;
    string line;
    while (getline(file, line)) {
        line = trimV(line);
        if (line.empty()) continue;
        istringstream ss(line);
        string id, plate, make, model, yearStr, ownerID;
        getline(ss, id, '|');
        getline(ss, plate, '|');
        getline(ss, make, '|');
        getline(ss, model, '|');
        getline(ss, yearStr, '|');
        getline(ss, ownerID);
        int year = 0;
        try { year = stoi(trimV(yearStr)); } catch (...) {}
        result.emplace_back(trimV(id), trimV(plate), trimV(make), trimV(model), year, trimV(ownerID));
    }
    return result;
}

void VehicleRepo::saveAll(const vector<Vehicle>& vehicles) {
    ofstream file("data/vehicles.txt");
    for (const auto& v : vehicles)
        file << v.getId() << "|" << v.getLicensePlate() << "|" << v.getMake() << "|"
             << v.getModel() << "|" << v.getYear() << "|" << v.getOwnerID() << "\n";
}

Vehicle VehicleRepo::findById(const string& id) {
    for (const auto& v : loadAll())
        if (v.getId() == id) return v;
    return Vehicle();
}

string VehicleRepo::add(const Vehicle& v) {
    auto all = loadAll();
    string newId;
    if (all.empty()) {
        newId = "V001";
    } else {
        int num = stoi(all.back().getId().substr(1));
        num++;
        string ns = to_string(num);
        while (ns.length() < 3) ns = "0" + ns;
        newId = "V" + ns;
    }
    Vehicle nv = v;
    nv.setId(newId);
    all.push_back(nv);
    saveAll(all);
    return newId;
}
