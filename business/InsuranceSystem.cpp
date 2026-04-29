#include "InsuranceSystem.h"
#include <ctime>
#include <algorithm>
#include <cctype>
using namespace std;

// ─── Helpers ─────────────────────────────────────────────────────────────────

static string currentDate() {
    time_t t = time(nullptr);
    tm* lt = localtime(&t);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", lt);
    return string(buf);
}

static bool isValidCNIC(const string& s) {
    if (s.length() != 13) return false;
    for (char c : s) if (!isdigit(c)) return false;
    return true;
}

static bool isValidPhone(const string& s) {
    if (s.length() != 11) return false;
    if (s.substr(0, 2) != "03") return false;
    for (char c : s) if (!isdigit(c)) return false;
    return true;
}

static bool isValidYear(int y) {
    return y >= 1980 && y <= 2026;
}

static bool isValidDate(const string& d) {
    if (d.length() != 10) return false;
    if (d[4] != '-' || d[7] != '-') return false;
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(d[i])) return false;
    }
    return true;
}

static bool isValidAmount(double a) { return a > 0.0; }

static bool isNonEmpty(const string& s) {
    for (char c : s) if (!isspace((unsigned char)c)) return true;
    return false;
}

// ─── Auth ─────────────────────────────────────────────────────────────────────

Staff* InsuranceSystem::login(const string& username, const string& password) {
    for (const auto& s : staffRepo.loadAll())
        if (s.getUsername() == username && s.getPassword() == password)
            return new Staff(s);
    return nullptr;
}

// ─── Customer ────────────────────────────────────────────────────────────────

string InsuranceSystem::registerCustomer(const string& name, const string& cnic,
                                         const string& phone, const string& address,
                                         const string& date) {
    if (!isNonEmpty(name) || !isValidCNIC(cnic) || !isValidPhone(phone) || !isNonEmpty(address))
        return "";
    Customer c;
    c.setName(name);
    c.setCnic(cnic);
    c.setPhone(phone);
    c.setAddress(address);
    c.setDateRegistered(date.empty() ? currentDate() : date);
    return customerRepo.add(c);
}

vector<Customer> InsuranceSystem::getAllCustomers() {
    return customerRepo.loadAll();
}

Customer InsuranceSystem::getCustomerById(const string& id) {
    return customerRepo.findById(id);
}

// ─── Vehicle ─────────────────────────────────────────────────────────────────

string InsuranceSystem::addVehicle(const string& plate, const string& make,
                                   const string& model, int year, const string& ownerID) {
    if (!isNonEmpty(plate) || !isNonEmpty(make) || !isNonEmpty(model) || !isValidYear(year))
        return "";
    if (customerRepo.findById(ownerID).getId().empty()) return "";
    Vehicle v;
    v.setLicensePlate(plate);
    v.setMake(make);
    v.setModel(model);
    v.setYear(year);
    v.setOwnerID(ownerID);
    return vehicleRepo.add(v);
}

vector<Vehicle> InsuranceSystem::getVehiclesByCustomer(const string& customerID) {
    vector<Vehicle> result;
    for (const auto& v : vehicleRepo.loadAll())
        if (v.getOwnerID() == customerID) result.push_back(v);
    return result;
}

// ─── Policy ──────────────────────────────────────────────────────────────────

string InsuranceSystem::issuePolicy(const string& customerID, const string& vehicleID,
                                    const string& type, const string& startDate,
                                    const string& endDate, double premium) {
    if (customerRepo.findById(customerID).getId().empty()) return "";
    if (vehicleRepo.findById(vehicleID).getId().empty()) return "";
    if (!isValidDate(startDate) || !isValidDate(endDate)) return "";
    if (!isValidAmount(premium)) return "";
    Policy p;
    p.setCustomerID(customerID);
    p.setVehicleID(vehicleID);
    p.setType(type);
    p.setStartDate(startDate);
    p.setEndDate(endDate);
    p.setStatus("Active");
    p.setPremium(premium);
    return policyRepo.add(p);
}

vector<Policy> InsuranceSystem::getPoliciesByCustomer(const string& customerID) {
    vector<Policy> result;
    for (const auto& p : policyRepo.loadAll())
        if (p.getCustomerID() == customerID) result.push_back(p);
    return result;
}

Policy InsuranceSystem::getPolicyById(const string& id) {
    return policyRepo.findById(id);
}

// ─── Claim ───────────────────────────────────────────────────────────────────

string InsuranceSystem::fileClaim(const string& policyID, const string& incidentDate,
                                  const string& description, double amount) {
    Policy p = policyRepo.findById(policyID);
    if (p.getId().empty()) return "";
    if (!p.isActive()) return "";
    if (!isValidDate(incidentDate) || !isValidAmount(amount)) return "";
    Claim c;
    c.setPolicyID(policyID);
    c.setSurveyorID("");
    c.setIncidentDate(incidentDate);
    c.setDescription(description);
    c.setStatus("Pending");
    c.setClaimAmount(amount);
    return claimRepo.add(c);
}

bool InsuranceSystem::assignSurveyor(const string& claimID, const string& surveyorID) {
    Staff s = staffRepo.findById(surveyorID);
    if (s.getId().empty() || s.getRole() != "Surveyor") return false;
    auto claims = claimRepo.loadAll();
    for (auto& c : claims) {
        if (c.getId() == claimID) {
            c.setSurveyorID(surveyorID);
            claimRepo.saveAll(claims);
            return true;
        }
    }
    return false;
}

vector<Claim> InsuranceSystem::getPendingClaims() {
    vector<Claim> result;
    for (const auto& c : claimRepo.loadAll())
        if (c.getStatus() == "Pending") result.push_back(c);
    return result;
}

vector<Claim> InsuranceSystem::getClaimsByCustomer(const string& customerID) {
    auto policies = policyRepo.loadAll();
    auto claims   = claimRepo.loadAll();
    vector<Claim> result;
    for (const auto& c : claims)
        for (const auto& p : policies)
            if (p.getId() == c.getPolicyID() && p.getCustomerID() == customerID) {
                result.push_back(c);
                break;
            }
    return result;
}

vector<Claim> InsuranceSystem::getClaimsForSurveyor(const string& surveyorID) {
    vector<Claim> result;
    for (const auto& c : claimRepo.loadAll())
        if (c.getSurveyorID() == surveyorID) result.push_back(c);
    return result;
}

vector<Claim> InsuranceSystem::getAllClaims() {
    return claimRepo.loadAll();
}

// ─── Inspection ───────────────────────────────────────────────────────────────

bool InsuranceSystem::submitInspection(const string& claimID, const string& surveyorID,
                                       const string& workshopID, const string& findings,
                                       const string& date, double estimatedLoss) {
    auto claims = claimRepo.loadAll();
    bool claimFound = false;
    for (auto& c : claims) {
        if (c.getId() == claimID) {
            if (c.getSurveyorID() != surveyorID) return false;
            if (workshopRepo.findById(workshopID).getId().empty()) return false;
            Inspection ins;
            ins.setClaimID(claimID);
            ins.setSurveyorID(surveyorID);
            ins.setWorkshopID(workshopID);
            ins.setFindings(findings);
            ins.setDate(date);
            ins.setStatus("Submitted");
            ins.setEstimatedLoss(estimatedLoss);
            inspectionRepo.add(ins);
            c.setStatus("UnderInspection");
            claimFound = true;
            break;
        }
    }
    if (!claimFound) return false;
    claimRepo.saveAll(claims);
    return true;
}

Inspection InsuranceSystem::getInspectionForClaim(const string& claimID) {
    for (const auto& i : inspectionRepo.loadAll())
        if (i.getClaimID() == claimID) return i;
    return Inspection();
}

vector<Inspection> InsuranceSystem::getAllInspections() {
    return inspectionRepo.loadAll();
}

vector<Inspection> InsuranceSystem::getInspectionsBySurveyor(const string& surveyorID) {
    vector<Inspection> result;
    for (const auto& i : inspectionRepo.loadAll())
        if (i.getSurveyorID() == surveyorID) result.push_back(i);
    return result;
}

// ─── Approve / Reject ─────────────────────────────────────────────────────────

bool InsuranceSystem::approveClaim(const string& claimID) {
    auto claims = claimRepo.loadAll();
    for (auto& c : claims) {
        if (c.getId() == claimID) {
            if (c.getStatus() != "UnderInspection") return false;
            if (getInspectionForClaim(claimID).getId().empty()) return false;
            c.setStatus("Approved");
            claimRepo.saveAll(claims);
            return true;
        }
    }
    return false;
}

bool InsuranceSystem::rejectClaim(const string& claimID) {
    auto claims = claimRepo.loadAll();
    for (auto& c : claims) {
        if (c.getId() == claimID) {
            if (c.getStatus() != "UnderInspection") return false;
            if (getInspectionForClaim(claimID).getId().empty()) return false;
            c.setStatus("Rejected");
            claimRepo.saveAll(claims);
            return true;
        }
    }
    return false;
}

// ─── Workshop ─────────────────────────────────────────────────────────────────

bool InsuranceSystem::addWorkshop(const string& name, const string& address, const string& date) {
    if (!isNonEmpty(name) || !isNonEmpty(address)) return false;
    Workshop w;
    w.setName(name);
    w.setAddress(address);
    w.setRegisteredDate(date.empty() ? currentDate() : date);
    workshopRepo.add(w);
    return true;
}

vector<Workshop> InsuranceSystem::getAllWorkshops() {
    return workshopRepo.loadAll();
}

// ─── Staff ────────────────────────────────────────────────────────────────────

bool InsuranceSystem::addStaff(const string& name, const string& username,
                               const string& password, const string& role) {
    if (!isNonEmpty(name) || !isNonEmpty(username) || !isNonEmpty(password)) return false;
    if (role != "Manager" && role != "Salesman" && role != "Surveyor") return false;
    Staff s;
    s.setName(name);
    s.setUsername(username);
    s.setPassword(password);
    s.setRole(role);
    staffRepo.add(s);
    return true;
}

vector<Staff> InsuranceSystem::getAllStaff() {
    return staffRepo.loadAll();
}

vector<Staff> InsuranceSystem::getSurveyors() {
    vector<Staff> result;
    for (const auto& s : staffRepo.loadAll())
        if (s.getRole() == "Surveyor") result.push_back(s);
    return result;
}

// ─── Reports ──────────────────────────────────────────────────────────────────

vector<Customer> InsuranceSystem::getNewCustomersInMonth(const string& yearMonth) {
    vector<Customer> result;
    for (const auto& c : customerRepo.loadAll())
        if (c.getDateRegistered().length() >= 7 &&
            c.getDateRegistered().substr(0, 7) == yearMonth)
            result.push_back(c);
    return result;
}

// ─── Seed ────────────────────────────────────────────────────────────────────

void InsuranceSystem::seedIfEmpty() {
    if (!staffRepo.loadAll().empty()) return;

    vector<Staff> staff = {
        Staff("ST001", "Admin User",   "admin", "admin123", "Manager"),
        Staff("ST002", "Bilal Ahmed",  "bilal", "pass123",  "Salesman"),
        Staff("ST003", "Sara Khan",    "sara",  "pass123",  "Surveyor")
    };
    staffRepo.saveAll(staff);

    vector<Workshop> workshops = {
        Workshop("W001", "AutoFix Workshop",    "Jail Road Lahore",     "2023-01-01"),
        Workshop("W002", "SpeedCraft Repairs",  "MM Alam Road Lahore",  "2023-01-01")
    };
    workshopRepo.saveAll(workshops);
}
