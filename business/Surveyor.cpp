#include "Surveyor.h"

Surveyor::Surveyor() : Staff(), pendingInspections(0) { role = "Surveyor"; }

Surveyor::Surveyor(const std::string& id, const std::string& name, const std::string& username,
                   const std::string& password)
    : Staff(id, name, username, password, "Surveyor"), pendingInspections(0) {}
