#pragma once
#include <vector>
#include <string>
#include "../business/Workshop.h"

class WorkshopRepo {
public:
    std::vector<Workshop> loadAll();
    void saveAll(const std::vector<Workshop>& workshops);
    Workshop findById(const std::string& id);
    std::string add(const Workshop& w);
};
