#pragma once
#include <vector>
#include <string>
#include "../business/Customer.h"

class CustomerRepo {
public:
    std::vector<Customer> loadAll();
    void saveAll(const std::vector<Customer>& customers);
    Customer findById(const std::string& id);
    std::string add(const Customer& c);
};
