#pragma once
#include <vector>
#include <string>
#include "Customer.h"
#include "Vehicle.h"
#include "Policy.h"
#include "Claim.h"
#include "Inspection.h"
#include "Staff.h"
#include "Workshop.h"
#include "../repository/CustomerRepo.h"
#include "../repository/VehicleRepo.h"
#include "../repository/PolicyRepo.h"
#include "../repository/ClaimRepo.h"
#include "../repository/InspectionRepo.h"
#include "../repository/StaffRepo.h"
#include "../repository/WorkshopRepo.h"

class InsuranceSystem {
    CustomerRepo   customerRepo;
    VehicleRepo    vehicleRepo;
    PolicyRepo     policyRepo;
    ClaimRepo      claimRepo;
    InspectionRepo inspectionRepo;
    StaffRepo      staffRepo;
    WorkshopRepo   workshopRepo;

public:
    Staff* login(const std::string& username, const std::string& password);

    std::string registerCustomer(const std::string& name, const std::string& cnic,
                                 const std::string& phone, const std::string& address,
                                 const std::string& date);
    std::vector<Customer> getAllCustomers();
    Customer getCustomerById(const std::string& id);

    std::string addVehicle(const std::string& plate, const std::string& make,
                           const std::string& model, int year, const std::string& ownerID);
    std::vector<Vehicle> getVehiclesByCustomer(const std::string& customerID);

    std::string issuePolicy(const std::string& customerID, const std::string& vehicleID,
                            const std::string& type, const std::string& startDate,
                            const std::string& endDate, double premium);
    std::vector<Policy> getPoliciesByCustomer(const std::string& customerID);
    Policy getPolicyById(const std::string& id);

    std::string fileClaim(const std::string& policyID, const std::string& incidentDate,
                          const std::string& description, double amount);
    bool assignSurveyor(const std::string& claimID, const std::string& surveyorID);
    std::vector<Claim> getPendingClaims();
    std::vector<Claim> getClaimsByCustomer(const std::string& customerID);
    std::vector<Claim> getClaimsForSurveyor(const std::string& surveyorID);
    std::vector<Claim> getAllClaims();

    bool submitInspection(const std::string& claimID, const std::string& surveyorID,
                          const std::string& workshopID, const std::string& findings,
                          const std::string& date, double estimatedLoss);
    Inspection getInspectionForClaim(const std::string& claimID);
    std::vector<Inspection> getAllInspections();
    std::vector<Inspection> getInspectionsBySurveyor(const std::string& surveyorID);

    bool approveClaim(const std::string& claimID);
    bool rejectClaim(const std::string& claimID);

    bool addWorkshop(const std::string& name, const std::string& address, const std::string& date);
    std::vector<Workshop> getAllWorkshops();

    bool addStaff(const std::string& name, const std::string& username,
                  const std::string& password, const std::string& role);
    std::vector<Staff> getAllStaff();
    std::vector<Staff> getSurveyors();

    std::vector<Customer> getNewCustomersInMonth(const std::string& yearMonth);

    void seedIfEmpty();
};
