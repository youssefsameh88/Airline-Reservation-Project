#include "CrewMember.h"

CrewMember::CrewMember(int id, std::string name, int maxFH, CrewType ct){
    this->id = id;
    this->name = name;
    this->maxFlightHours = maxFH;
    this->crewType = ct;
    currentFlightHours = 0;
}
void CrewMember::setName(std::string name){this->name = name;}
void CrewMember::setMaxFlightHours(int mfh){ maxFlightHours = mfh;}
void CrewMember::addFlightHours(int hours){currentFlightHours += hours;}
int CrewMember::getId(){return id;}
int CrewMember::getCurrentFlightHours(){return currentFlightHours;}
int CrewMember::getMaxFlightHours(){return maxFlightHours;}
std::string CrewMember::getName(){return name;}
CrewType CrewMember::getCrewType(){return crewType;}
CrewMember::~CrewMember(){}