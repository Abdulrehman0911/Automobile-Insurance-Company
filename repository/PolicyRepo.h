#pragma once
#include <vector>
#include <string>
#include "../business/Policy.h"

class PolicyRepo {
public:
    std::vector<Policy> loadAll();
    void saveAll(const std::vector<Policy>& policies);
    Policy findById(const std::string& id);
    std::string add(const Policy& p);
};
