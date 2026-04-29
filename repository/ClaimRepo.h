#pragma once
#include <vector>
#include <string>
#include "../business/Claim.h"

class ClaimRepo {
public:
    std::vector<Claim> loadAll();
    void saveAll(const std::vector<Claim>& claims);
    Claim findById(const std::string& id);
    std::string add(const Claim& c);
};
