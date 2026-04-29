#include "Manager.h"

Manager::Manager() : Staff() { role = "Manager"; }

Manager::Manager(const std::string& id, const std::string& name, const std::string& username,
                 const std::string& password)
    : Staff(id, name, username, password, "Manager") {}
