#pragma once
#include <vector>
#include <string>
#include "../business/Inspection.h"

class InspectionRepo {
public:
    std::vector<Inspection> loadAll();
    void saveAll(const std::vector<Inspection>& inspections);
    Inspection findById(const std::string& id);
    std::string add(const Inspection& i);
};
