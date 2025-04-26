#include "../include/Simulation.h"
#include "../include/Auxiliary.h"
#include "../include/Action.h"
#include <iostream> // For std::cin and std::cout
#include <string>   // For std::string and std::getline
#include <fstream>  // For file input/output (std::ifstream)
#include <sstream>  // For string streams (std::istringstream)
#include <vector>
#include <stdexcept>
using namespace std;

//Base Action Father
BaseAction::BaseAction():
errorMsg("Error: "), status(ActionStatus::ERROR)
{}

ActionStatus BaseAction::getStatus() const{return status;}

void BaseAction::complete() {status = ActionStatus::COMPLETED;}

void BaseAction::error(string errorMsgInput) {
    status = ActionStatus::ERROR;
    errorMsg = errorMsgInput;
    cout << "Error: "+errorMsg << endl;
}

const string& BaseAction::getErrorMsg() const {
        return errorMsg;
}


//step
SimulateStep::SimulateStep(const int numOfSteps) : 
numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation &simulation){
    //run step x times
    for (int i = 0; i < numOfSteps; i++)
    {
        simulation.step();       
    }
    this->complete();
}

const string SimulateStep::toString() const{
    string stat = "";
    if(this->getStatus()==ActionStatus::COMPLETED){
        stat = "COMPLETED";
    }
    else{
        stat = "ERROR";
    }
    return "step "+ to_string(this->numOfSteps) + " " + stat;
}

SimulateStep* SimulateStep::clone() const{
    SimulateStep* clooney = new SimulateStep(*this);  // Uses the copy constructor to create a new instance
    return clooney;
}


//addPlan
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):
settlementName(settlementName), selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation &simulation){
    bool have_pol = false;    
    std::vector<std::string> policies = {"env", "nve", "eco", "bal"}; 
    //validation

    //check policy
    for (size_t i = 0; i < policies.size() && !have_pol; i++)
    {
        if(policies[i]==selectionPolicy){
            have_pol = true;
        }
    }
   
    const Settlement& mySet = simulation.getSettlementConst(settlementName); //set refrence to settelkemnt

    if(have_pol && simulation.isSettlementExists(settlementName)){ //validation
        if(selectionPolicy=="eco"){
            EconomySelection selection;
            EconomySelection *selected = &selection;
            simulation.addPlan(mySet,selected);
        }

        else if(selectionPolicy=="bal"){
            BalancedSelection selection(0,0,0);
            BalancedSelection *selected = &selection;
            simulation.addPlan(mySet,selected);
        }

        else if(selectionPolicy=="nve"){
            NaiveSelection selection;
            NaiveSelection *selected = &selection;
            simulation.addPlan(mySet,selected);
        }

        else{ //sustain
            SustainabilitySelection selection;
            SustainabilitySelection *selected = &selection;
            simulation.addPlan(mySet,selected);
        }   

        this->complete();
     }  
    //error
    else{
        this->error("Can't create this plan");
    }
}

const string AddPlan::toString() const{
    string stat = "";
    if(this->getStatus()==ActionStatus::COMPLETED){
        stat = "COMPLETED";
    }
    else{
        stat = "ERROR";
    }

    return "plan " +settlementName+ " "+  selectionPolicy+ " "+ stat;
}

AddPlan* AddPlan::clone() const{
    AddPlan* clooney = new AddPlan(*this);  // Uses the copy constructor to create a new instance
    return clooney;
}


//add settlement
AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType) :
    settlementName(settlementName), settlementType(settlementType) {}


void AddSettlement::act(Simulation &simulation){
    bool added = simulation.addSettlement(new Settlement(this->settlementName, this->settlementType));
    if(!added){
        this->error("Settlement already exists");
    }
    else{
        this->complete();
    }
}

const string AddSettlement::toString() const{
    string stat = "";
    if(this->getStatus()==ActionStatus::COMPLETED){
        stat = "COMPLETED";
    }
    else{
        stat = "ERROR";
    }

    return "settlement " + settlementName+ " "+ to_string(static_cast<int>(settlementType)) + " "+ stat;
}

AddSettlement* AddSettlement::clone() const{
    AddSettlement* clooney = new AddSettlement(*this);  // Uses the copy constructor to create a new instance
    return clooney;
}


//Add Facility
AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):
facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore)
{}

void AddFacility::act(Simulation &simulation){
    bool added = simulation.addFacility(FacilityType(facilityName, facilityCategory, price,lifeQualityScore, economyScore ,environmentScore));
    if(!added){
        this->error("Facility already exists");
    }
    else{
        this->complete();
    }
}

const string AddFacility::toString() const{
    string stat = "", ans="";
    if(this->getStatus()==ActionStatus::COMPLETED){
        stat = "COMPLETED";
    }
    else{
        stat = "ERROR";
    }

    return "facility "+ facilityName+ " "+ to_string(static_cast<int>(facilityCategory)) + " " + to_string(price) + " "
    + to_string(lifeQualityScore) +" " +to_string(economyScore) +" "+ to_string(environmentScore) + " " + stat;
}

AddFacility* AddFacility::clone() const{
    AddFacility* clooney = new AddFacility(*this);  // Uses the copy constructor to create a new instance
    return clooney;
}


//print plan status
PrintPlanStatus::PrintPlanStatus(int planId) : planId(planId) {}

void PrintPlanStatus::act(Simulation &simulation){
    if(planId>=simulation.getPlanCounter()){
        this->error("Plan doesn't exist");
    }
    else {
        (simulation.getPlan(planId)).printStatus();
        this->complete();
    }
    
}

const string PrintPlanStatus::toString() const{
    string stat = "", ans="";
    if(this->getStatus()==ActionStatus::COMPLETED){
        stat = "COMPLETED";
    }
    else{
        stat = "ERROR";
    }

    return "planstatus " + to_string(planId) + " " + stat;
}

PrintPlanStatus* PrintPlanStatus::clone() const{
    PrintPlanStatus* clooney = new PrintPlanStatus(*this);  // Uses the copy constructor to create a new instance
    return clooney;
}


//change plan policy
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy):
planId(planId), newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation &simulation){
    bool have_pol = false;    
    std::vector<std::string> policies = {"env", "nve", "eco", "bal"}; 

    //check if policy is real
    for (size_t i = 0; i < policies.size() && !have_pol; i++)
    {
        if(policies[i]==newPolicy){
            have_pol = true;
        }
    }


    if(planId>=simulation.getPlanCounter())
        this->error("Cannot change selection policy");
    else{ //legal number
        //check specific policy assuming plan is real
        string oldPolicy = simulation.getPlan(planId).getPolicy()->toString();
        if(((have_pol) && (oldPolicy != newPolicy))){
            this->complete();

            //create selection policy
            if(newPolicy=="eco"){
                simulation.getPlan(planId).setSelectionPolicy(new EconomySelection());
            }
            else if(newPolicy=="bal"){
                simulation.getPlan(planId).setSelectionPolicy(new BalancedSelection(simulation.getPlan(planId).getlifeQualityScore(), simulation.getPlan(planId).getEconomyScore(), simulation.getPlan(planId).getEnvironmentScore()));
            }
            else if(newPolicy=="nve"){
                simulation.getPlan(planId).setSelectionPolicy(new NaiveSelection());
            }
            else{ //sustain
                simulation.getPlan(planId).setSelectionPolicy(new SustainabilitySelection());
            }
    
            cout << ("planID: "+ to_string(planId)) <<endl;
            cout << ("previousPolicy: "+oldPolicy) << endl;
            cout << ("newPolicy: "+newPolicy ) <<endl;
        }
        else{
            this->error("Cannot change selection policy");
        }
    }
}

const string ChangePlanPolicy::toString() const{
    string stat = "", ans="";
    if(this->getStatus()==ActionStatus::COMPLETED){
        stat = "COMPLETED";
    }
    else{
        stat = "ERROR";
    }

    return "changePolicy " + to_string(planId) + " " + newPolicy + " " + stat;
}

ChangePlanPolicy* ChangePlanPolicy::clone() const{
    ChangePlanPolicy* clooney = new ChangePlanPolicy(*this);  // Uses the copy constructor to create a new instance
    return clooney;
}


//print action log
PrintActionsLog::PrintActionsLog(){}

void PrintActionsLog::act(Simulation &simulation){
    simulation.printCurrentLog();
    this->complete();
}

const string PrintActionsLog::toString() const{
    string stat = "", ans="";
    if(this->getStatus()==ActionStatus::COMPLETED){
        stat = "COMPLETED";
    }
    else{
        
        stat = "ERROR";
    }

    return "log "+ stat;

}

PrintActionsLog* PrintActionsLog::clone() const{
    PrintActionsLog* clooney = new PrintActionsLog(*this);  // Uses the copy constructor to create a new instance
    return clooney;
}


//Close
Close::Close(){}

void Close::act(Simulation &simulation){
    simulation.close();
    this->complete();
}

const string Close::toString() const{
    string stat = "", ans="";
    if(this->getStatus()==ActionStatus::COMPLETED){
        stat = "COMPLETED";
    }
    else{
        stat = "ERROR";
    }

    return "close "+ stat;

}

Close* Close::clone() const{
    Close* clooney = new Close(*this);  // Uses the copy constructor to create a new instance
    return clooney;
}


//Backup
BackupSimulation::BackupSimulation(){}

void BackupSimulation::act(Simulation &simulation){
    simulation.backUp();
    this->complete();
}

const string BackupSimulation::toString() const{
    string stat = "", ans="";
    if(this->getStatus()==ActionStatus::COMPLETED){
        stat = "COMPLETED";
    }
    else{
        stat = "ERROR";
    }

    return "backup "+ stat;
}

BackupSimulation* BackupSimulation::clone() const{
    BackupSimulation* clooney = new BackupSimulation(*this);  // Uses the copy constructor to create a new instance
    return clooney;
}


//Restore
RestoreSimulation::RestoreSimulation(){}

void RestoreSimulation::act(Simulation &simulation){
    if(backup==nullptr){
        this->error("No backup available");
    }
    else{
        simulation.restore();
        this->complete();       
    }
}

const string RestoreSimulation::toString() const{
    string stat = "", ans="";
    if(this->getStatus()==ActionStatus::COMPLETED){
        stat = "COMPLETED";
    }
    else{
        stat = "ERROR";
    }

    return "restore "+ stat;
}

RestoreSimulation* RestoreSimulation::clone() const{
    RestoreSimulation* clooney = new RestoreSimulation(*this);  // Uses the copy constructor to create a new instance
    return clooney;
}


//Copy Constructores where needed
// BaseAction
BaseAction::BaseAction(const BaseAction& other)
    : errorMsg(other.errorMsg), status(other.status) {
    }

// SimulateStep
SimulateStep::SimulateStep(const SimulateStep& other)
    : BaseAction(other), numOfSteps(other.numOfSteps) {}

// AddPlan
AddPlan::AddPlan(const AddPlan& other)
    : BaseAction(other), settlementName(other.settlementName), selectionPolicy(other.selectionPolicy) {}

// AddSettlement
AddSettlement::AddSettlement(const AddSettlement& other)
    : BaseAction(other), settlementName(other.settlementName), settlementType(other.settlementType) {}

// AddFacility
AddFacility::AddFacility(const AddFacility& other)
    : BaseAction(other), facilityName(other.facilityName), facilityCategory(other.facilityCategory),
      price(other.price), lifeQualityScore(other.lifeQualityScore),
      economyScore(other.economyScore), environmentScore(other.environmentScore) {}

// PrintPlanStatus
PrintPlanStatus::PrintPlanStatus(const PrintPlanStatus& other)
    : BaseAction(other), planId(other.planId) {}

// ChangePlanPolicy
ChangePlanPolicy::ChangePlanPolicy(const ChangePlanPolicy& other)
    : BaseAction(other), planId(other.planId), newPolicy(other.newPolicy) {}

// PrintActionsLog
PrintActionsLog::PrintActionsLog(const PrintActionsLog& other) : BaseAction(other)  {}

// Close
Close::Close(const Close& other): BaseAction(other)  {}

//BackupSimulation
BackupSimulation::BackupSimulation(const BackupSimulation& other) : BaseAction(other)  {}
