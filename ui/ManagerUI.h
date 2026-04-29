#pragma once
#include "../business/InsuranceSystem.h"

class ManagerUI {
    InsuranceSystem& sys;
public:
    explicit ManagerUI(InsuranceSystem& sys);
    void run(Staff* staff);
private:
    void doViewPendingClaims();
    void doAssignSurveyor();
    void doViewInspectionReport();
    void doApproveClaim();
    void doRejectClaim();
    void doRegisterCustomer();
    void doAddVehicle();
    void doIssuePolicy();
    void doViewAllCustomers();
    void doCustomerClaimHistory();
    void doAddStaff();
    void doViewAllStaff();
    void doAddWorkshop();
    void doViewAllWorkshops();
    void doNewCustomersThisMonth();
    void doAllInspectionReports();
};
