#pragma once
#include <string>
#include "Enums.h"
class CrewMember{
    private:
    int id, currentFlightHours, maxFlightHours;
    std::string name;
    CrewType crewType;

    public:
    CrewMember(int id, std::string name, int maxFH, CrewType ct);
    void setName(std::string name);
    void setMaxFlightHours(int mfh);
    void addFlightHours(int hours);
    int getId();
    int getCurrentFlightHours();
    int getMaxFlightHours();
    std::string getName();
    CrewType getCrewType();
    virtual ~CrewMember();

};