#pragma once
#include <string>

class Workshop {
    std::string id, name, address, registeredDate;
public:
    Workshop();
    Workshop(const std::string& id, const std::string& name,
             const std::string& address, const std::string& registeredDate);
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    std::string getAddress() const { return address; }
    std::string getRegisteredDate() const { return registeredDate; }
    void setId(const std::string& v) { id = v; }
    void setName(const std::string& v) { name = v; }
    void setAddress(const std::string& v) { address = v; }
    void setRegisteredDate(const std::string& v) { registeredDate = v; }
};
