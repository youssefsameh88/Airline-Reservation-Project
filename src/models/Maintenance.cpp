#include "Maintenance.h"

Maintenance::Maintenance(int id, Aircraft* ac, std::string date, std::string description, MaintenanceStatus ms){
    this->id = id;
    this->aircraft = ac;
    this->date = date;
    this->description = description;
    this->maintenanceStatus = ms;
}
int Maintenance::getId(){return id;}
Aircraft* Maintenance::getAircraft(){return aircraft;}
std::string Maintenance::getDate(){return date;}
std::string Maintenance::getDescription(){return description;}
void Maintenance::setMaintenanceStatus(MaintenanceStatus ms){maintenanceStatus = ms;}
MaintenanceStatus Maintenance::getMaintenanceStatus(){return maintenanceStatus;}