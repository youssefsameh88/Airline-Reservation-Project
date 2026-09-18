#include "Pilot.h"


Pilot::Pilot(int id, std::string name, int maxFH, 
    std::string ln, std::string lt)
    : CrewMember(id, name, maxFH, CrewType::Pilot)
{
    licenseNumber = ln;
    licenseType = lt;
}
    std::string Pilot::getLicenseNumber(){return licenseNumber;}
    std::string Pilot::getLicenseType(){return licenseType;}