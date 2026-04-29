#include "Claim.h"

Claim::Claim() : id(""), policyID(""), surveyorID(""), incidentDate(""), description(""), status(""), claimAmount(0.0) {}

Claim::Claim(const std::string& id, const std::string& policyID, const std::string& surveyorID,
             const std::string& incidentDate, const std::string& description,
             const std::string& status, double claimAmount)
    : id(id), policyID(policyID), surveyorID(surveyorID), incidentDate(incidentDate),
      description(description), status(status), claimAmount(claimAmount) {}
