#include "Inspection.h"

Inspection::Inspection() : id(""), claimID(""), surveyorID(""), workshopID(""), findings(""), date(""), status(""), estimatedLoss(0.0) {}

Inspection::Inspection(const std::string& id, const std::string& claimID, const std::string& surveyorID,
                       const std::string& workshopID, const std::string& findings,
                       const std::string& date, const std::string& status, double estimatedLoss)
    : id(id), claimID(claimID), surveyorID(surveyorID), workshopID(workshopID),
      findings(findings), date(date), status(status), estimatedLoss(estimatedLoss) {}
