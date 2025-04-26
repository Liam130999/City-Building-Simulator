#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        const int getPlanid() const;
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        SelectionPolicy* getPolicy();
        void setSelectionPolicy(SelectionPolicy *selectionPolicyInput);
        void step();
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;

        PlanStatus getStatus() const;
        const Settlement& getSettlement() const;
        void setStatus(PlanStatus stat);
        
        ~Plan();
        Plan& operator=(const Plan& other);
        Plan(const Plan& other);
        Plan(const Plan &other,const Settlement &settlementName);



    private:
        int plan_id;
        const Settlement &settlement; 
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};