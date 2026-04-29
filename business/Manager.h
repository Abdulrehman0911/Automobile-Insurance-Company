#pragma once
#include "Staff.h"

class Manager : public Staff {
public:
    Manager();
    Manager(const std::string& id, const std::string& name, const std::string& username,
            const std::string& password);
    std::string getRole() const override { return "Manager"; }
};
