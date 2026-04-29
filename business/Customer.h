#pragma once
#include <string>

class Customer {
    std::string id, name, cnic, phone, address, dateRegistered;
public:
    Customer();
    Customer(const std::string& id, const std::string& name, const std::string& cnic,
             const std::string& phone, const std::string& address, const std::string& dateRegistered);
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    std::string getCnic() const { return cnic; }
    std::string getPhone() const { return phone; }
    std::string getAddress() const { return address; }
    std::string getDateRegistered() const { return dateRegistered; }
    void setId(const std::string& v) { id = v; }
    void setName(const std::string& v) { name = v; }
    void setCnic(const std::string& v) { cnic = v; }
    void setPhone(const std::string& v) { phone = v; }
    void setAddress(const std::string& v) { address = v; }
    void setDateRegistered(const std::string& v) { dateRegistered = v; }
};
