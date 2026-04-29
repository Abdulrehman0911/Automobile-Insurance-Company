#pragma once
#include <vector>
#include <string>
#include "../business/Staff.h"

class StaffRepo {
public:
    std::vector<Staff> loadAll();
    void saveAll(const std::vector<Staff>& staff);
    Staff findById(const std::string& id);
    std::string add(const Staff& s);
};
