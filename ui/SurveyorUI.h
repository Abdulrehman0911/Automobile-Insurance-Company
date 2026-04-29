#pragma once
#include "../business/InsuranceSystem.h"

class SurveyorUI {
    InsuranceSystem& sys;
public:
    explicit SurveyorUI(InsuranceSystem& sys);
    void run(Staff* staff);
private:
    void doViewAssignedClaims(const std::string& surveyorId);
    void doSubmitInspection(const std::string& surveyorId);
    void doViewMyInspections(const std::string& surveyorId);
};
