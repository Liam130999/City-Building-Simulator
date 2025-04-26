#include "../include/Facility.h"
#include <iostream>  // For std::cout, if needed
using std::vector;

//FacilityType

//constructor
FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score) :
name(name), category(category), price(price), lifeQuality_score(lifeQuality_score),economy_score(economy_score), environment_score(environment_score){}

//copy constructor
FacilityType::FacilityType(const FacilityType& other) :
name(other.name), category(other.category), price(other.price), lifeQuality_score(other.lifeQuality_score),economy_score(other.economy_score), environment_score(other.environment_score){}



//getter
const string& FacilityType::getName() const {return name;}
int FacilityType::getCost() const {return price;}
int FacilityType::getLifeQualityScore() const {return lifeQuality_score;}
int FacilityType::getEnvironmentScore() const {return environment_score;}
int FacilityType::getEconomyScore() const  {return economy_score;}
FacilityCategory FacilityType::getCategory() const  {return category;}



//Facility

//constructors
Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score) :
FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score),
settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price) {}

Facility::Facility(const FacilityType &type, const string &settlementName):
FacilityType(type.getName(), type.getCategory(), type.getCost(), type.getLifeQualityScore(), type.getEconomyScore(), type.getEnvironmentScore()),
settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price) {}

Facility::Facility(const Facility& other)
    : FacilityType(other), // Call the base class copy constructor
      settlementName(other.settlementName),
      status(other.status),
      timeLeft(other.timeLeft) {
}


//getters
const string& Facility::getSettlementName() const {return settlementName;}
const int Facility::getTimeLeft() const {return timeLeft;}
const FacilityStatus& Facility::getStatus() const {return status;}

FacilityStatus Facility::step(){
        if (this->getStatus()==FacilityStatus::UNDER_CONSTRUCTIONS){
                this->setTimeLeft((this->getTimeLeft())-1);
                if(this->getTimeLeft()==0){
                       this->setStatus(FacilityStatus::OPERATIONAL); 
                }
        }
        return this->getStatus();
}

//setter
void Facility::setStatus(FacilityStatus statusInput) {status = statusInput;}
void Facility::setTimeLeft(int time) {timeLeft = time;}


//toString
const string Facility::toString() const{
    std::string cat = "";
    if (this->getCategory()==FacilityCategory::ECONOMY){
            cat = "economy";
    }
    else if (this->getCategory()==FacilityCategory::ENVIRONMENT){
            cat = "environment";
    } 
    else
            cat = "life quality";

    std::string sta = "";
    if (status==FacilityStatus::OPERATIONAL){
            sta = "operational";
    }
    else
            sta = "under constructions";


    return "Name: "+ this->getName() + ", Category: " + cat + ", Price: " + std::to_string(this->getCost()) +
    ", Life Quality Score: "+ std::to_string(this->getLifeQualityScore()) + ", Economy Score: " + std::to_string(this->getEconomyScore()) + ", Environment Score: "+
    std::to_string(this->getEnvironmentScore()) + ", Settlement Name: " + settlementName + ", Status: " + sta + ", Time Left: "+ std::to_string(timeLeft) ;
}


