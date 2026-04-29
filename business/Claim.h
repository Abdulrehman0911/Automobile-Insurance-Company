#pragma once
#include <string>

class Claim {
    std::string id, policyID, surveyorID, incidentDate, description, status;
    double claimAmount;
public:
    Claim();
    Claim(const std::string& id, const std::string& policyID, const std::string& surveyorID,
          const std::string& incidentDate, const std::string& description,
          const std::string& status, double claimAmount);
    std::string getId() const { return id; }
    std::string getPolicyID() const { return policyID; }
    std::string getSurveyorID() const { return surveyorID; }
    std::string getIncidentDate() const { return incidentDate; }
    std::string getDescription() const { return description; }
    std::string getStatus() const { return status; }
    double getClaimAmount() const { return claimAmount; }
    void setId(const std::string& v) { id = v; }
    void setPolicyID(const std::string& v) { policyID = v; }
    void setSurveyorID(const std::string& v) { surveyorID = v; }
    void setIncidentDate(const std::string& v) { incidentDate = v; }
    void setDescription(const std::string& v) { description = v; }
    void setStatus(const std::string& v) { status = v; }
    void setClaimAmount(double v) { claimAmount = v; }
};
