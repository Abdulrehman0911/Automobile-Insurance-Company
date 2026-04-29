#include "Customer.h"

Customer::Customer() : id(""), name(""), cnic(""), phone(""), address(""), dateRegistered("") {}

Customer::Customer(const std::string& id, const std::string& name, const std::string& cnic,
                   const std::string& phone, const std::string& address, const std::string& dateRegistered)
    : id(id), name(name), cnic(cnic), phone(phone), address(address), dateRegistered(dateRegistered) {}
