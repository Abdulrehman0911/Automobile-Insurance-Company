#pragma once
#include <string>

class Vehicle {
    std::string id, licensePlate, make, model;
    int year;
    std::string ownerID;
public:
    Vehicle();
    Vehicle(const std::string& id, const std::string& licensePlate, const std::string& make,
            const std::string& model, int year, const std::string& ownerID);
    std::string getId() const { return id; }
    std::string getLicensePlate() const { return licensePlate; }
    std::string getMake() const { return make; }
    std::string getModel() const { return model; }
    int getYear() const { return year; }
    std::string getOwnerID() const { return ownerID; }
    void setId(const std::string& v) { id = v; }
    void setLicensePlate(const std::string& v) { licensePlate = v; }
    void setMake(const std::string& v) { make = v; }
    void setModel(const std::string& v) { model = v; }
    void setYear(int v) { year = v; }
    void setOwnerID(const std::string& v) { ownerID = v; }
};
