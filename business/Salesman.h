#pragma once
#include "Staff.h"

class Salesman : public Staff {
    int salesThisMonth;
public:
    Salesman();
    Salesman(const std::string& id, const std::string& name, const std::string& username,
             const std::string& password);
    std::string getRole() const override { return "Salesman"; }
    int getSalesThisMonth() const { return salesThisMonth; }
    void setSalesThisMonth(int v) { salesThisMonth = v; }
};
