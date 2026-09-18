#pragma once
#include "CrewMember.h"
#include <string>

class FlightAttendant: public CrewMember{
    public:
    FlightAttendant(int id, std::string name, int maxFH);
};