#include "FlightAttendant.h"

FlightAttendant::FlightAttendant(int id, std::string name, int maxFH)
:CrewMember(id, name, maxFH, CrewType::FlightAttendant){}