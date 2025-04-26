#include "../include/SelectionPolicy.h"
#include <iostream>  // For std::cout, if needed
using namespace std;
#include <algorithm>

//Naive
NaiveSelection::NaiveSelection() : lastSelectedIndex(-1){}

int NaiveSelection::getLastSelectedIndex() const {return lastSelectedIndex;}
void NaiveSelection::setLastSelectedIndex(int num) {lastSelectedIndex = num;}


const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    size_t location = this->getLastSelectedIndex();  
    location++;
    //prevent array out of bounds
    if (location>=facilitiesOptions.size()){
        location = location%(int)(facilitiesOptions.size()); 
    }
    NaiveSelection::setLastSelectedIndex(location);
    return facilitiesOptions[location];
}

const string NaiveSelection::toString() const{
    return "nve";
}

NaiveSelection* NaiveSelection::clone() const{
    NaiveSelection* newNaive = new NaiveSelection(*this);  // Uses the copy constructor to create a new instance
    return newNaive;
}

//Balanced
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore ) : LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore){}

int BalancedSelection::calcCurrBalance(int lifeQ, int eco, int env){
    int maxVal = std::max(eco, std::max(env, lifeQ));
    int minVal = std::min(eco, std::min(env, lifeQ));
    int balance = maxVal - minVal;
    return balance;
}

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions)  {
    int currBalance = calcCurrBalance(getLifeQualityScore(), getEconomyScore(), getEnvironmentScore());
    int indexToRtrn = -1;

    for (size_t i = 0; i < facilitiesOptions.size(); i++){
        int tmpEco = this->getEconomyScore() + facilitiesOptions[i].getEconomyScore();
        int tmpEnv = this->getEnvironmentScore() + facilitiesOptions[i].getEnvironmentScore();
        int tmpLifeQ = this->getLifeQualityScore() + facilitiesOptions[i].getLifeQualityScore();
        int tmpBalance = calcCurrBalance(tmpEco,tmpEnv,tmpLifeQ);

        if (tmpBalance < currBalance ){
            currBalance = tmpBalance;
            indexToRtrn = i;
        }
    }

    if(indexToRtrn==-1){ //did not find anyone better than me, choose the best
        currBalance = INT16_MAX;
        for (size_t i = 0; i < facilitiesOptions.size(); i++){
            int tmpEco = this->getEconomyScore() + facilitiesOptions[i].getEconomyScore();
            int tmpEnv = this->getEnvironmentScore() + facilitiesOptions[i].getEnvironmentScore();
            int tmpLifeQ = this->getLifeQualityScore() + facilitiesOptions[i].getLifeQualityScore();
            int tmpBalance = calcCurrBalance(tmpEco,tmpEnv,tmpLifeQ);

        if (tmpBalance < currBalance ){
            currBalance = tmpBalance;
            indexToRtrn = i;
        }
    }
    }

    this->setEconomyScore(this->getEconomyScore()+facilitiesOptions[indexToRtrn].getEconomyScore());
    this->setEnvironmentScore(this->getEnvironmentScore()+facilitiesOptions[indexToRtrn].getEnvironmentScore());
    this->setLifeQualityScore(this->getLifeQualityScore()+facilitiesOptions[indexToRtrn].getLifeQualityScore());
    
    return facilitiesOptions[indexToRtrn];

}


const string BalancedSelection::toString() const {
     return "bal";
}

BalancedSelection* BalancedSelection::clone() const{  
    BalancedSelection* newBalanced = new BalancedSelection(*this);  // Uses the copy constructor to create a new instance
    return newBalanced;
}

//getter setter
int BalancedSelection::getLifeQualityScore() const {return LifeQualityScore;}
int BalancedSelection::getEconomyScore() const {return EconomyScore;}
int BalancedSelection::getEnvironmentScore() const {return EnvironmentScore;}


void BalancedSelection::setLifeQualityScore(int num) {LifeQualityScore = num;}
void BalancedSelection::setEconomyScore(int num) {EconomyScore = num;}
void BalancedSelection::setEnvironmentScore(int num) {EnvironmentScore = num;}



//Economy
EconomySelection::EconomySelection() : lastSelectedIndex(-1){}

int EconomySelection::getLastSelectedIndex() const {return lastSelectedIndex;}
void EconomySelection::setLastSelectedIndex(int num) {lastSelectedIndex = num;}


const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    size_t location = this->getLastSelectedIndex();
    //look for next Economy
    do {
        location++;
        location = location%facilitiesOptions.size();     //prevent array out of bounds
    }while((facilitiesOptions[location].getCategory()!=(FacilityCategory::ECONOMY)));

    EconomySelection::setLastSelectedIndex(location);
    return facilitiesOptions[location];
}

const string EconomySelection::toString() const{
    return "eco";
}

EconomySelection* EconomySelection::clone() const{
    EconomySelection* newEconomy = new EconomySelection(*this);  // Uses the copy constructor to create a new instance
    return newEconomy;
}


//Sustainability
SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(-1){}

int SustainabilitySelection::getLastSelectedIndex() const {return lastSelectedIndex;}
void SustainabilitySelection::setLastSelectedIndex(int num) {lastSelectedIndex = num;}


const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    size_t location = this->getLastSelectedIndex();
    //look for next Sustainability
    do{
        location++;
        location = location%facilitiesOptions.size(); 
    }while((facilitiesOptions[location].getCategory()!=(FacilityCategory::ENVIRONMENT)));
    SustainabilitySelection::setLastSelectedIndex(location);
    return facilitiesOptions[location];
}

const string SustainabilitySelection::toString() const{
    return "env";
}

SustainabilitySelection* SustainabilitySelection::clone() const{
    SustainabilitySelection* clooney = new SustainabilitySelection(*this);  // Uses the copy constructor to create a new instance
    return clooney;
}


