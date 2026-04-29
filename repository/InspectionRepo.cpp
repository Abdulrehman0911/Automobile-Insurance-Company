#include "InspectionRepo.h"
#include <fstream>
#include <sstream>
using namespace std;

static string trimIN(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

vector<Inspection> InspectionRepo::loadAll() {
    vector<Inspection> result;
    { ofstream touch("data/inspections.txt", ios::app); }
    ifstream file("data/inspections.txt");
    if (!file.is_open()) return result;
    string line;
    while (getline(file, line)) {
        line = trimIN(line);
        if (line.empty()) continue;
        istringstream ss(line);
        string id, claimID, survID, workshopID, findings, date, status, lossStr;
        getline(ss, id, '|');
        getline(ss, claimID, '|');
        getline(ss, survID, '|');
        getline(ss, workshopID, '|');
        getline(ss, findings, '|');
        getline(ss, date, '|');
        getline(ss, status, '|');
        getline(ss, lossStr);
        double loss = 0.0;
        try { loss = stod(trimIN(lossStr)); } catch (...) {}
        result.emplace_back(trimIN(id), trimIN(claimID), trimIN(survID), trimIN(workshopID),
                            trimIN(findings), trimIN(date), trimIN(status), loss);
    }
    return result;
}

void InspectionRepo::saveAll(const vector<Inspection>& inspections) {
    ofstream file("data/inspections.txt");
    for (const auto& i : inspections)
        file << i.getId() << "|" << i.getClaimID() << "|" << i.getSurveyorID() << "|"
             << i.getWorkshopID() << "|" << i.getFindings() << "|"
             << i.getDate() << "|" << i.getStatus() << "|" << i.getEstimatedLoss() << "\n";
}

Inspection InspectionRepo::findById(const string& id) {
    for (const auto& i : loadAll())
        if (i.getId() == id) return i;
    return Inspection();
}

string InspectionRepo::add(const Inspection& i) {
    auto all = loadAll();
    string newId;
    if (all.empty()) {
        newId = "IN001";
    } else {
        int num = stoi(all.back().getId().substr(2));
        num++;
        string ns = to_string(num);
        while (ns.length() < 3) ns = "0" + ns;
        newId = "IN" + ns;
    }
    Inspection ni = i;
    ni.setId(newId);
    all.push_back(ni);
    saveAll(all);
    return newId;
}
