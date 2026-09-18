#pragma once
#include "CrewMember.h"
#include <string>

class Pilot: public CrewMember{    
    private:
    std::string licenseNumber, licenseType;
    public:
    Pilot(int id, std::string name, int maxFH, std::string ln, std::string lt);
    std::string getLicenseNumber();
    std::string getLicenseType();
};