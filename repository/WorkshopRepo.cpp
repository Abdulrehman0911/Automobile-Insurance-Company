#include "WorkshopRepo.h"
#include <fstream>
#include <sstream>
using namespace std;

static string trimW(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

vector<Workshop> WorkshopRepo::loadAll() {
    vector<Workshop> result;
    { ofstream touch("data/workshops.txt", ios::app); }
    ifstream file("data/workshops.txt");
    if (!file.is_open()) return result;
    string line;
    while (getline(file, line)) {
        line = trimW(line);
        if (line.empty()) continue;
        istringstream ss(line);
        string id, name, address, regDate;
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, address, '|');
        getline(ss, regDate);
        result.emplace_back(trimW(id), trimW(name), trimW(address), trimW(regDate));
    }
    return result;
}

void WorkshopRepo::saveAll(const vector<Workshop>& workshops) {
    ofstream file("data/workshops.txt");
    for (const auto& w : workshops)
        file << w.getId() << "|" << w.getName() << "|"
             << w.getAddress() << "|" << w.getRegisteredDate() << "\n";
}

Workshop WorkshopRepo::findById(const string& id) {
    for (const auto& w : loadAll())
        if (w.getId() == id) return w;
    return Workshop();
}

string WorkshopRepo::add(const Workshop& w) {
    auto all = loadAll();
    string newId;
    if (all.empty()) {
        newId = "W001";
    } else {
        int num = stoi(all.back().getId().substr(1));
        num++;
        string ns = to_string(num);
        while (ns.length() < 3) ns = "0" + ns;
        newId = "W" + ns;
    }
    Workshop nw = w;
    nw.setId(newId);
    all.push_back(nw);
    saveAll(all);
    return newId;
}
