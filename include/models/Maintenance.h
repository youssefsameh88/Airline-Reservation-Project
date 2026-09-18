#pragma once
#include <string>
#include "Enums.h"

class Aircraft;
class Maintenance{
    private:
    int id;
    Aircraft* aircraft;
    std::string date, description;
    MaintenanceStatus maintenanceStatus;

    public: 
    Maintenance(int id, Aircraft* ac, std::string date, std::string description, MaintenanceStatus ms);
    int getId();
    Aircraft* getAircraft();
    std::string getDate();
    std::string getDescription();
    void setMaintenanceStatus(MaintenanceStatus ms);
    MaintenanceStatus getMaintenanceStatus();
};