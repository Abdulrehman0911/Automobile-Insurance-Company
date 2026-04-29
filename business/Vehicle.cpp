#include "Vehicle.h"

Vehicle::Vehicle() : id(""), licensePlate(""), make(""), model(""), year(0), ownerID("") {}

Vehicle::Vehicle(const std::string& id, const std::string& licensePlate, const std::string& make,
                 const std::string& model, int year, const std::string& ownerID)
    : id(id), licensePlate(licensePlate), make(make), model(model), year(year), ownerID(ownerID) {}
