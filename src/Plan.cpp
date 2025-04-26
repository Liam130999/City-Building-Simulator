#include "../include/Plan.h"
#include <iostream>  // For std::cout, if needed
#include <sstream>  // For std::ostringstream
#include <vector>
using namespace std;


//constructor
Plan::Plan(
    const int planId, 
    const Settlement& settlement, 
    SelectionPolicy* selectionPolicy, 
    const vector<FacilityType>& facilityOptions
)
    : plan_id(planId), 
      settlement(settlement), 
      selectionPolicy(selectionPolicy), 
      status(PlanStatus::AVALIABLE), // Assuming a default status
      facilities(), //operatinal
      underConstruction(), //under const
      facilityOptions(facilityOptions), 
      life_quality_score(0), 
      economy_score(0), 
      environment_score(0) 
    {}

//Desctructor
Plan::~Plan() {
    delete selectionPolicy; // Free the dynamically allocated SelectionPolicy

    // Clean up dynamically allocated facilities
    for (Facility* facility : underConstruction) {
        delete facility;
    }
    underConstruction.clear();

    for (Facility* facility : facilities) {
        delete facility;
    }
    facilities.clear();
}


//copy constructor
Plan::Plan(const Plan& other)
    : plan_id(other.plan_id),
      settlement(other.settlement),
      selectionPolicy(other.selectionPolicy->clone()),  // Clone the selection policy
      status(other.status),
      facilities(), underConstruction(), facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score) {



    for (Facility* facility : other.facilities) {
        facilities.push_back(new Facility(*facility));
    }

    for (Facility* facility : other.underConstruction) {
        underConstruction.push_back(new Facility(*facility));
    }
}

//another copy consructor with set name
Plan::Plan(const Plan &other,const Settlement &settlementName)
    : plan_id(other.plan_id),
    settlement(settlementName),
    selectionPolicy(other.selectionPolicy->clone()), // Because selectionPolicy is by pointer we use the clone method of its class
    status(other.status),
    facilities(),
    underConstruction(),
    facilityOptions(other.facilityOptions),
    life_quality_score(other.life_quality_score),
    economy_score(other.economy_score),
    environment_score(other.environment_score){

    for(Facility* facility : other.facilities)
        facilities.push_back(new Facility(*facility));

    for(Facility* facility : other.underConstruction)
        underConstruction.push_back(new Facility(*facility));
}


// Copy Assignment Operator
Plan& Plan::operator=(const Plan& other){
    if(&other != this){
        plan_id = other.plan_id;

        // Deleting the old selectionPolicy and clone the one of other
        delete selectionPolicy;
        selectionPolicy = other.selectionPolicy->clone();

        status = other.status;
        life_quality_score = other.getlifeQualityScore();
        economy_score = other.getEconomyScore();
        environment_score = other.getEnvironmentScore();

        //Delete and clear old lists
        for(Facility* facility : facilities)
            delete facility;
        facilities.clear();
        for(const Facility* facility : other.facilities)
            facilities.push_back(new Facility(*facility));

        for(Facility* facility : underConstruction)
            delete facility;
        underConstruction.clear();
        for(const Facility* facility : other.underConstruction)
            underConstruction.push_back(new Facility(*facility));
    }
    return *this;
}

//getters setters
const int Plan::getPlanid() const {return plan_id;}
const int Plan::getlifeQualityScore() const {return life_quality_score;}
const int Plan::getEconomyScore() const {return economy_score;}
const int Plan::getEnvironmentScore() const {return environment_score;}
PlanStatus Plan::getStatus() const {return status;}
const Settlement& Plan::getSettlement() const {return settlement;}


void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicyInput){
    if(selectionPolicy)
        delete selectionPolicy;

    selectionPolicy=selectionPolicyInput;
}

void Plan::setStatus(PlanStatus stat){status=stat;}

void Plan::step(){
    size_t constLimit = (this->getSettlement()).getConstLimit();
        //1
    if(this->getStatus()==PlanStatus::AVALIABLE){
        //2
       
        //construction limit
        size_t current_underConst = this->underConstruction.size();
        while(current_underConst<constLimit){
            Facility *fac = new Facility(this->selectionPolicy->selectFacility(facilityOptions), this->getSettlement().getName());
            underConstruction.push_back(fac);
            current_underConst = this->underConstruction.size(); //update size
        }
        this->setStatus(PlanStatus::BUSY);//no more place
    }

    //Then:
    //3
    for (size_t i = 0; i < underConstruction.size(); i++){ 
        if(underConstruction[i]->step()==FacilityStatus::OPERATIONAL){ //facility step
            this->addFacility(underConstruction[i]); //update numbers
        }
    }

    //physical transfer
    for (size_t i = 0; i < underConstruction.size(); /* no increment here */) {

        if (underConstruction[i]->getStatus() == FacilityStatus::OPERATIONAL) {
        // Move the element to facilities
        facilities.push_back((underConstruction[i]));

        // Erase the element from underConstruction
        underConstruction.erase(underConstruction.begin() + i);

        // No need to increment i, as erase shifts elements left
        }
        else {
            // Increment i only if no element was erased
            ++i;
        }
    } 
   

    //4
    if(underConstruction.size()<constLimit){
        this->setStatus(PlanStatus::AVALIABLE); //back having place
    }
}

void Plan::addFacility(Facility* facility){
    this->life_quality_score += facility->getLifeQualityScore();
    this->economy_score += facility->getEconomyScore();
    this->environment_score += facility->getEnvironmentScore();
}


void Plan::printStatus(){
    cout << "PlanID: "+ to_string((this->plan_id)) << endl;
    cout << "SettlementName: "+ this->getSettlement().getName() << endl;
    
    string statStr;
    if(this->getStatus()==PlanStatus::AVALIABLE){
        statStr = "AVAILABLE";
    }
    else{
        statStr = "BUSY";

    }
    cout << "PlanSatus : "+ statStr << endl;
    cout << "Selection Policy: " << this->selectionPolicy->toString() << endl;
    cout << "LifeQualityScore : "<< this->getlifeQualityScore() << endl;
    cout << "EconomyScore : "<< this->getEconomyScore() << endl;
    cout << "EnvironmentScore : "<< this->getEnvironmentScore() << endl;

    for (size_t i = 0; i < underConstruction.size(); i++){
        cout << "FacilityName : " + underConstruction[i]->getName()<< endl;
        cout << "FacilityStatus : UNDER_CONSTRUCTIONS"<< endl;
    }

    for (size_t i = 0; i < facilities.size(); i++){
        cout << "FacilityName : "<< facilities[i]->getName()<< endl;
        cout << "FacilityStatus : OPERATIONAL"<< endl;
    }

}


//return operational
const vector<Facility*>& Plan::getFacilities() const{
    return this->facilities;
}

const string Plan::toString() const {
    std::ostringstream output;
    output << "PlanID: " << this->plan_id << "\n";
    output << "SettlementName: " << this->getSettlement().getName() << "\n";
    std::string statStr;
    if (this->getStatus() == PlanStatus::AVALIABLE) {
        statStr = "AVAILABLE";
    } else {
        statStr = "BUSY";
    }
    output << "PlanStatus: " << statStr << "\n";
    output << "Selection Policy: " << this->selectionPolicy->toString() << "\n";
    output << "LifeQualityScore: " << this->getlifeQualityScore() << "\n";
    output << "EconomyScore: " << this->getEconomyScore() << "\n";
    output << "EnvironmentScore: " << this->getEnvironmentScore() << "\n";

    // Under Construction Facilities
    for (size_t i = 0; i < underConstruction.size(); i++) {
        output << "FacilityName: " << underConstruction[i]->getName() << "\n";
        output << "Facility Status: UNDER_CONSTRUCTION\n";
    }

    // Operational Facilities
    for (size_t i = 0; i < facilities.size(); i++) {
        output << "FacilityName: " << facilities[i]->getName() << "\n";
        output << "Facility Status: OPERATIONAL\n";
    }

    return output.str();
}

SelectionPolicy* Plan::getPolicy(){return this->selectionPolicy;}
