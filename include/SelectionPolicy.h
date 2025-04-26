#pragma once
#include <vector>
#include "Facility.h"
using std::vector;

class SelectionPolicy {
    public:
        virtual const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) = 0;
        virtual const string toString() const = 0;
        virtual SelectionPolicy* clone() const = 0;
        virtual ~SelectionPolicy() = default;
};

class NaiveSelection: public SelectionPolicy {
    public:
        NaiveSelection();
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        NaiveSelection* clone() const override;
        ~NaiveSelection() override = default;

        //getter setter
        int getLastSelectedIndex() const;
        void setLastSelectedIndex(int num);

    private:
        int lastSelectedIndex;
};

class BalancedSelection: public SelectionPolicy {
    public:
        BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore);
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        BalancedSelection *clone() const override;
        ~BalancedSelection() override = default;

        //getter setter
        int getLifeQualityScore() const;
        int getEconomyScore() const;
        int getEnvironmentScore() const;

        void setLifeQualityScore(int num);
        void setEconomyScore(int num);
        void setEnvironmentScore(int num);

        int calcCurrBalance(int lifeQ, int eco, int env);

    private:
        int LifeQualityScore;
        int EconomyScore;
        int EnvironmentScore;

};

class EconomySelection: public SelectionPolicy {
    public:
        EconomySelection();
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        EconomySelection* clone() const override;
        ~EconomySelection() override = default;

        //getter setter
        int getLastSelectedIndex() const;
        void setLastSelectedIndex(int num);

    private:
        int lastSelectedIndex;

};

class SustainabilitySelection: public SelectionPolicy {
    public:
        SustainabilitySelection();
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        SustainabilitySelection* clone() const override;
        ~SustainabilitySelection() override = default;

        //getter setter
        int getLastSelectedIndex() const;
        void setLastSelectedIndex(int num);

    private:
        int lastSelectedIndex;
};