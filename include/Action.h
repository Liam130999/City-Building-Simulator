#pragma once
#include <string>
#include <vector>
#include "Simulation.h"
enum class SettlementType;
enum class FacilityCategory;

enum class ActionStatus{
    COMPLETED, ERROR
};

class BaseAction{
    public:
        BaseAction();
        ActionStatus getStatus() const;
        virtual void act(Simulation& simulation)=0;
        virtual const string toString() const=0;
        virtual BaseAction* clone() const = 0;
        virtual ~BaseAction() = default;
        BaseAction(const BaseAction& other);


    protected:
        void complete();
        void error(string errorMsg);
        const string& getErrorMsg() const;

    private:
        string errorMsg;
        ActionStatus status;
};

class SimulateStep : public BaseAction {

    public:
        SimulateStep(const int numOfSteps);
        void act(Simulation &simulation) override;
        const string toString() const override;
        SimulateStep *clone() const override;
        SimulateStep(const SimulateStep& other);

    private:
        const int numOfSteps;
};

class AddPlan : public BaseAction {
    public:
        AddPlan(const string &settlementName, const string &selectionPolicy);
        void act(Simulation &simulation) override;
        const string toString() const override;
        AddPlan *clone() const override;
        AddPlan(const AddPlan& other);

    private:
        const string settlementName;
        const string selectionPolicy;
};


class AddSettlement : public BaseAction {
    public:
        AddSettlement(const string &settlementName,SettlementType settlementType);
        void act(Simulation &simulation) override;
        AddSettlement *clone() const override;
        const string toString() const override;
        AddSettlement(const AddSettlement& other);

    private:
        const string settlementName;
        const SettlementType settlementType;
};



class AddFacility : public BaseAction {
    public:
        AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore);
        void act(Simulation &simulation) override;
        AddFacility *clone() const override;
        const string toString() const override;
        AddFacility(const AddFacility& other);

    private:
        const string facilityName;
        const FacilityCategory facilityCategory;
        const int price;
        const int lifeQualityScore;
        const int economyScore;
        const int environmentScore;

};

class PrintPlanStatus: public BaseAction {
    public:
        PrintPlanStatus(int planId);
        void act(Simulation &simulation) override;
        PrintPlanStatus *clone() const override;
        const string toString() const override;
        PrintPlanStatus(const PrintPlanStatus& other);

    private:
        const int planId;
};


class ChangePlanPolicy : public BaseAction {
    public:
        ChangePlanPolicy(const int planId, const string &newPolicy);
        void act(Simulation &simulation) override;
        ChangePlanPolicy *clone() const override;
        const string toString() const override;
        ChangePlanPolicy(const ChangePlanPolicy& other);

    private:
        const int planId;
        const string newPolicy;
};


class PrintActionsLog : public BaseAction {
    public:
        PrintActionsLog();
        void act(Simulation &simulation) override;
        PrintActionsLog *clone() const override;
        const string toString() const override;
        PrintActionsLog(const PrintActionsLog& other);

    private:
};

class Close : public BaseAction {
    public:
        Close();
        void act(Simulation &simulation) override;
        Close *clone() const override;
        const string toString() const override;
        Close(const Close& other);

    private:
};

class BackupSimulation : public BaseAction {
    public:
        BackupSimulation();
        void act(Simulation &simulation) override;
        BackupSimulation *clone() const override;
        const string toString() const override;
        BackupSimulation(const BackupSimulation& other);

    private:
};


class RestoreSimulation : public BaseAction {
    public:
        RestoreSimulation();
        void act(Simulation &simulation) override;
        RestoreSimulation *clone() const override;
        const string toString() const override;
    private:
};