#pragma once
#include <vector>
#include <string>
#include "../business/Vehicle.h"

class VehicleRepo {
public:
    std::vector<Vehicle> loadAll();
    void saveAll(const std::vector<Vehicle>& vehicles);
    Vehicle findById(const std::string& id);
    std::string add(const Vehicle& v);
};
