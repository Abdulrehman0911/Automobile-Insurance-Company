#include "Staff.h"

Staff::Staff() : id(""), name(""), username(""), password(""), role("") {}

Staff::Staff(const std::string& id, const std::string& name, const std::string& username,
             const std::string& password, const std::string& role)
    : id(id), name(name), username(username), password(password), role(role) {}
