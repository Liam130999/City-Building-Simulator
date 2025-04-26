#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;

//Check signatures for all fucntions for last Skeleton

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        Settlement &getSettlement(const string &settlementName);
        const Settlement &getSettlementConst(const string &settlementName);
        Plan &getPlan(const int planID);
        vector<BaseAction*> getActions();
        int getPlanCounter();
        int getSettsSize();
        void step();
        void close();
        void backUp();
        void restore();
        void open();
        void parse();
        void printCurrentLog();

        ~Simulation();
        Simulation(const Simulation &other);
        Simulation& operator=(const Simulation &other);
        Simulation(Simulation &&other);
        Simulation& operator=(Simulation &&other);



    private:
        const string& config;
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;
        std::vector<std::string> currList;
    
};

extern Simulation* backup;
