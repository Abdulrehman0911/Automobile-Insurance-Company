#pragma once
#include "../business/InsuranceSystem.h"

class SalesmanUI {
    InsuranceSystem& sys;
public:
    explicit SalesmanUI(InsuranceSystem& sys);
    void run(Staff* staff);
private:
    void doRegisterCustomer();
    void doAddVehicle();
    void doIssuePolicy();
    void doFileClaim();
    void doViewAllCustomers();
    void doViewPolicyDetails();
};
