#pragma once
#include <string>

class Policy {
    std::string id, customerID, vehicleID, type, startDate, endDate, status;
    double premium;
public:
    Policy();
    Policy(const std::string& id, const std::string& customerID, const std::string& vehicleID,
           const std::string& type, const std::string& startDate, const std::string& endDate,
           const std::string& status, double premium);
    std::string getId() const { return id; }
    std::string getCustomerID() const { return customerID; }
    std::string getVehicleID() const { return vehicleID; }
    std::string getType() const { return type; }
    std::string getStartDate() const { return startDate; }
    std::string getEndDate() const { return endDate; }
    std::string getStatus() const { return status; }
    double getPremium() const { return premium; }
    void setId(const std::string& v) { id = v; }
    void setCustomerID(const std::string& v) { customerID = v; }
    void setVehicleID(const std::string& v) { vehicleID = v; }
    void setType(const std::string& v) { type = v; }
    void setStartDate(const std::string& v) { startDate = v; }
    void setEndDate(const std::string& v) { endDate = v; }
    void setStatus(const std::string& v) { status = v; }
    void setPremium(double v) { premium = v; }
    bool isActive() const { return status == "Active"; }
};
