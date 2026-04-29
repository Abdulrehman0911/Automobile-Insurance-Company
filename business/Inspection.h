#pragma once
#include <string>

class Inspection {
    std::string id, claimID, surveyorID, workshopID, findings, date, status;
    double estimatedLoss;
public:
    Inspection();
    Inspection(const std::string& id, const std::string& claimID, const std::string& surveyorID,
               const std::string& workshopID, const std::string& findings,
               const std::string& date, const std::string& status, double estimatedLoss);
    std::string getId() const { return id; }
    std::string getClaimID() const { return claimID; }
    std::string getSurveyorID() const { return surveyorID; }
    std::string getWorkshopID() const { return workshopID; }
    std::string getFindings() const { return findings; }
    std::string getDate() const { return date; }
    std::string getStatus() const { return status; }
    double getEstimatedLoss() const { return estimatedLoss; }
    void setId(const std::string& v) { id = v; }
    void setClaimID(const std::string& v) { claimID = v; }
    void setSurveyorID(const std::string& v) { surveyorID = v; }
    void setWorkshopID(const std::string& v) { workshopID = v; }
    void setFindings(const std::string& v) { findings = v; }
    void setDate(const std::string& v) { date = v; }
    void setStatus(const std::string& v) { status = v; }
    void setEstimatedLoss(double v) { estimatedLoss = v; }
};
