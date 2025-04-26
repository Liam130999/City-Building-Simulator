#include "../include/Settlement.h"
#include <iostream>  // For std::cout, if needed
using std::vector;

    //constructor
    Settlement::Settlement(const string &name, SettlementType type) : 
    name(name), type(type) {}

    //getters
    const string& Settlement::getName() const { return name; }

    SettlementType Settlement::getType() const { return type; }

    //toString
    const string Settlement::toString() const{
    std::string ans = "";
        if (type==SettlementType::VILLAGE){
            ans = "Settlement " + name+ " is a village";
        }
        if (type==SettlementType::CITY){
            ans = "Settlement " + name+ " is a city";
        }
        if (type==SettlementType::METROPOLIS){
            ans = "Settlement " + name+ " is a metropolis";
        }

        return ans;
    }

    
    size_t Settlement::getConstLimit() const{
        size_t ans=0;
        if (type==SettlementType::VILLAGE){
            ans = 1;
        }
        if (type==SettlementType::CITY){
            ans = 2;
        }
        if (type==SettlementType::METROPOLIS){
            ans = 3;
        } 

        return ans;
    }



