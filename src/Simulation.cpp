#include "../include/Simulation.h"
#include "../include/Auxiliary.h"
#include "../include/Action.h"
#include "../include/Plan.h"
#include <iostream> // For std::cin and std::cout
#include <string>   // For std::string and std::getline
#include <fstream>  // For file input/output (std::ifstream)
#include <sstream>  // For string streams (std::istringstream)
#include <vector>
#include <stdexcept>
using namespace std;

Simulation* backup = nullptr;

//Simulation

//constructor
Simulation::Simulation(const string& configFilePath) :
 config(configFilePath), isRunning(false), planCounter(0), actionsLog(), plans(), settlements(), facilitiesOptions(), currList()
{
    this->parse(); 
}


// Destructor
Simulation::~Simulation(){
    for(BaseAction *action : actionsLog){
        delete action;
    }
    actionsLog.clear();

    for(Settlement *set : settlements){
        delete set;
    }
    settlements.clear();
}

// Copy Constructor
Simulation::Simulation(const Simulation &other)
: config(other.config),isRunning(other.isRunning), planCounter(other.planCounter),actionsLog(),plans(),settlements(),facilitiesOptions(other.facilitiesOptions),
currList(){

    //Deep copy currList
    currList = other.currList;

    // Deep Copying the actionLogs vector of other
    for(BaseAction *action : other.actionsLog)
        actionsLog.push_back(action->clone());


    for(Settlement *set: other.settlements){
        settlements.push_back(new Settlement(*set));
    }

    // Deep Copying the plans vector of other
    for(const Plan &plan : other.plans){
        Settlement &copiedSet = getSettlement(plan.getSettlement().getName());
        plans.push_back(Plan(plan, copiedSet));
    }
}


// Copy Assignment operator
Simulation& Simulation::operator=(const Simulation &other){
    // If the input isn't me
    if(&other != this){
        //no need to copy the config field at the copy assignment operator
        currList.clear();
        currList = other.currList;
        isRunning = other.isRunning;
        planCounter = other.planCounter;

        //Deleting the old actionsLog
        for(BaseAction *action : actionsLog)
            delete action;
        actionsLog.clear();
        for(BaseAction *action : other.actionsLog)
            actionsLog.push_back(action->clone());

        // Deleting the old settlements
        for(Settlement *set : settlements)
            delete set;
        settlements.clear();
        for(Settlement *set : other.settlements)
            settlements.push_back(new Settlement(*set));

        // Clearing the vector and assignmenting those of the other one
        plans.clear();
        for(const Plan &plan : other.plans){
            Settlement &copiedSet = getSettlement(plan.getSettlement().getName());
            plans.push_back(Plan(plan, copiedSet));
        }

        // Clearing the vector and assignmenting those of the other one
        facilitiesOptions.clear();
        for(const FacilityType &facility : other.facilitiesOptions)
            facilitiesOptions.push_back(FacilityType(facility));
    }

    return *this;
}


//move constructor
Simulation::Simulation(Simulation &&other)
: config(other.config), isRunning(other.isRunning),
  planCounter(other.planCounter),
  actionsLog(std::move(other.actionsLog)),
  plans(std::move(other.plans)),
  settlements(std::move(other.settlements)),
  facilitiesOptions(std::move(other.facilitiesOptions)),
  currList()
{
    // We don't need to touch currList here, it's already moved properly
    currList = std::move(other.currList);
    other.actionsLog.clear();
}


// Move Assignment operator
Simulation& Simulation::operator=(Simulation &&other){
    // If the input isn't me
    if(this != &other){
        
        // Deleting the actions from the heap
        for(BaseAction *action : actionsLog)
            delete action;

        for(Settlement *set : settlements)
            delete set;

        settlements.clear();
        actionsLog.clear();
        plans.clear();
        facilitiesOptions.clear();

        // The movement
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        actionsLog = move(other.actionsLog);
        plans = move(other.plans);
        settlements = move(other.settlements);
        facilitiesOptions = move(other.facilitiesOptions);
        currList = move(other.currList); 

        //dont care about config field
    }

    return *this;

} 


void Simulation::parse(){
    // Open the config.txt file for reading
    std::ifstream file(config);

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string config_string = buffer.str();

    std::istringstream stream(config_string);

    std::string line = "";

    // Read each line from the string (stream) into 'line'
    while (std::getline(stream, line)) {
        const std::string& currentLine = line;  // 'currentLine' is a const reference to the line
        std::vector<std::string> lineList = Auxiliary::parseArguments(currentLine);

        string first = lineList[0];
        
        if(first=="settlement"){
            SettlementType type = SettlementType::METROPOLIS;
            if (lineList[2]=="0"){
                type = SettlementType::VILLAGE;
            }
            else if (lineList[2]=="1"){
                type = SettlementType::CITY;
            }
            //if 2 already metropolis

            Settlement *set = new Settlement(lineList[1], type);
            settlements.push_back(set);
        }

        else if(first=="facility"){
            FacilityCategory cat = FacilityCategory::LIFE_QUALITY;
            if (lineList[2]=="1"){
                cat = FacilityCategory::ECONOMY;
            }
            else if (lineList[2]=="2"){
                cat = FacilityCategory::ENVIRONMENT;
            }
            //if 2 already Life Qual

            facilitiesOptions.push_back(FacilityType(lineList[1], cat, std::stoi(lineList[3]),std::stoi(lineList[4]),std::stoi(lineList[5]),std::stoi(lineList[6])));
        }

        else if(first=="plan"){
            const int planid = planCounter++;
            string setString = lineList[1];
            int index = -1;

            // Find the settlement
            for (size_t i = 0; i < settlements.size(); i++) {
                if (settlements[i]->getName() == setString) {
                    index = i;
                }
            }
            const Settlement& mySet = *(settlements[index]);
            const vector<FacilityType>& optionsListConst(facilitiesOptions);

            // Use clone function to create a new selection policy
            SelectionPolicy* selected = nullptr;

            if (lineList[2] == "eco") {
                selected = new EconomySelection();
            }
            else if (lineList[2] == "bal") {
                selected = new BalancedSelection(0, 0, 0);
            }
            else if (lineList[2] == "nve") {
                selected = new NaiveSelection();
            }
            else {
                selected = new SustainabilitySelection();
            }

            // Clone the selection policy before creating the plan
            Plan p(planid, mySet, selected->clone(), optionsListConst);
            plans.push_back(p);

            // Clean up the dynamically allocated selected policy
            delete selected;
        }
            
        else{
            continue;
        }

    }
}

void Simulation::open(){
    isRunning = true; 
}

int Simulation::getPlanCounter(){
    return planCounter;
}

int Simulation::getSettsSize(){
    return settlements.size();
}




bool Simulation::isSettlementExists(const string &settlementName){
    bool have_set = false;

    //check settlement name
    for (int i = 0; i < this->getSettsSize() && !have_set; i++)
    {
        if(settlements[i]->getName()==settlementName){
            have_set = true;
        }
    }

    return have_set;
}

//Assuming exists
Settlement &Simulation::getSettlement(const string &settlementName){
    bool have_set = false;
    int index=-1;

    //check settlement name
    for (int i = 0; i < this->getSettsSize() && !have_set; i++)
    {
        if(settlements[i]->getName()==settlementName){
            have_set = true;
            index = i;
        }
    }
        
    return *(settlements[index]); //set refrence
    
}

//assuming exists
const Settlement &Simulation::getSettlementConst(const string &settlementName){
    bool have_set = false;
    int index=-1;

    //check settlement name
    for (int i = 0; i < this->getSettsSize() && !have_set; i++)
    {
        if(settlements[i]->getName()==settlementName){
            have_set = true;
            index = i;
        }
    }

    const Settlement& mySet = *(settlements[index]); //set refrence
    return  mySet;
    
}




void Simulation::start(){
    cout << "The Simulation Has Started" << endl;
    this->open();

    string currAction = "";
    //wait for action
    while(isRunning){
        std::getline(std::cin, currAction); // Read the whole line

        currList = Auxiliary::parseArguments(currAction);
        string first = currList[0];

        if(first=="plan"){
            BaseAction* addPlan = new AddPlan(currList[1], currList[2]);
            addPlan->act(*this);
            this->addAction(addPlan);
        }

        else if(first=="settlement"){
            //build it
            SettlementType type = SettlementType::METROPOLIS; //for nothing
            if(currList[2]=="0"){
                type = SettlementType::VILLAGE;
            }
            else if(currList[2]=="1"){
                type = SettlementType::CITY;
            }
            //else =2 metropolis

            BaseAction* addSettlement = new AddSettlement(currList[1], type);  // Allocate on the heap
            addSettlement->act(*this);
            this->addAction(addSettlement);  // Pass the heap-allocated pointer

        } 

        else if(first=="facility"){
            FacilityCategory cat = FacilityCategory::LIFE_QUALITY; //0
            if (currList[2]=="1"){
                cat = FacilityCategory::ECONOMY;
            }
            else if (currList[2]=="2"){
                cat = FacilityCategory::ENVIRONMENT;
            }

            BaseAction* addFac = new AddFacility(currList[1], cat, std::stoi(currList[3]),std::stoi(currList[4]),std::stoi(currList[5]),std::stoi(currList[6]));
            addFac->act(*this);
            this->addAction(addFac);
        }

        else if(first=="step"){
            const int numofsteps = std::stoi(currList[1]);
            BaseAction* actStep = new SimulateStep(numofsteps);
            actStep->act(*this); 
            this->addAction(actStep);
        }

        else if(first=="planStatus"){
            BaseAction* planSta = new PrintPlanStatus(std::stoi(currList[1]));
            planSta->act(*this);
            this->addAction(planSta);
        }

        else if(first=="changePolicy"){
            BaseAction* changePol = new ChangePlanPolicy(std::stoi(currList[1]), currList[2]);
            changePol->act(*this);
            this->addAction(changePol);
        }

        else if(first=="log"){
            BaseAction* printLogAct = new PrintActionsLog();
            printLogAct->act(*this);
            this->addAction(printLogAct);
        }

        else if(first=="close"){
            BaseAction* closeAct = new Close();
            closeAct->act(*this);
            this->addAction(closeAct);
        }

        else if(first=="backup"){
            BaseAction* backMe = new BackupSimulation();
            backMe->act(*this);
            this->addAction(backMe);
        }

        else if(first=="restore"){
            BaseAction* restMe = new RestoreSimulation();
            restMe->act(*this);
            this->addAction(restMe);
        }

    }
}


void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){    
    //set plan id
    const int planid = planCounter;
    planCounter++;

    //copy facilities list 
    const vector<FacilityType>& optionsListConst(facilitiesOptions);

    Plan p(planid, settlement, selectionPolicy->clone(), optionsListConst);  // Stack-based allocation
    plans.push_back(p);
}


bool Simulation::addSettlement(Settlement *settlement){
    bool exists = isSettlementExists(settlement->getName());
    if(!exists){
        settlements.push_back(settlement);
        return true;
    }
    else{
       return false;
    }
}


bool Simulation::addFacility(FacilityType facility){
    //validation
    bool exists = false;
    //check facilitiy name
    for (size_t i = 0; i < facilitiesOptions.size() && !exists; i++)
    {
        if(facilitiesOptions[i].getName()==facility.getName()){
            exists = true;
        }
    }
    if(exists){
        return false;
    }
    else{
        facilitiesOptions.push_back(facility);
        return true;
    }
}


void Simulation::step(){
    //step - iterate over all plans and activte their step
    for (size_t i = 0; i < plans.size(); i++)
    {
        plans[i].step();
    } 
}

Plan& Simulation::getPlan(const int planID){
    return plans[planID];
}

vector<BaseAction*> Simulation::getActions(){
    return this->actionsLog;
}

void Simulation::addAction(BaseAction *action){
    this->actionsLog.push_back(action);
}

void Simulation::printCurrentLog(){
    for (BaseAction* action : actionsLog) {
        cout << action->toString() << endl;
    }
}

void Simulation::close() {
    // Print all plans and their accumulated results
    for (Plan& plan : plans) {
        std::cout << "PlanID: " << plan.getPlanid() << "\n";
        std::cout << "SettlementName: " << plan.getSettlement().getName() << "\n";
        std::cout << "LifeQuality_Score: " << plan.getlifeQualityScore() << "\n";
        std::cout << "Economy_Score: " << plan.getEconomyScore() << "\n";
        std::cout << "Environment_Score: " << plan.getEnvironmentScore() << "\n";
    }

    if(backup){
        delete backup;
        backup = nullptr;
    }

    // Mark simulation as not running
    isRunning = false;
}


void Simulation::backUp(){
    if(backup){
        delete backup;
    }
    backup = new Simulation(*this);  // Create a new Simulation object and assign it to the global 'backup'
}

void Simulation::restore(){
    if(backup){
        *this = *backup;  // Restore the simulation state from the backup
    }
}

