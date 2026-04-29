#include "Workshop.h"

Workshop::Workshop() : id(""), name(""), address(""), registeredDate("") {}

Workshop::Workshop(const std::string& id, const std::string& name,
                   const std::string& address, const std::string& registeredDate)
    : id(id), name(name), address(address), registeredDate(registeredDate) {}
