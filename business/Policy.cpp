#include "Policy.h"

Policy::Policy() : id(""), customerID(""), vehicleID(""), type(""), startDate(""), endDate(""), status(""), premium(0.0) {}

Policy::Policy(const std::string& id, const std::string& customerID, const std::string& vehicleID,
               const std::string& type, const std::string& startDate, const std::string& endDate,
               const std::string& status, double premium)
    : id(id), customerID(customerID), vehicleID(vehicleID), type(type),
      startDate(startDate), endDate(endDate), status(status), premium(premium) {}
