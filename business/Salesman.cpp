#include "Salesman.h"

Salesman::Salesman() : Staff(), salesThisMonth(0) { role = "Salesman"; }

Salesman::Salesman(const std::string& id, const std::string& name, const std::string& username,
                   const std::string& password)
    : Staff(id, name, username, password, "Salesman"), salesThisMonth(0) {}
