#pragma once
#include "Staff.h"

class Surveyor : public Staff {
    int pendingInspections;
public:
    Surveyor();
    Surveyor(const std::string& id, const std::string& name, const std::string& username,
             const std::string& password);
    std::string getRole() const override { return "Surveyor"; }
    int getPendingInspections() const { return pendingInspections; }
    void setPendingInspections(int v) { pendingInspections = v; }
};
