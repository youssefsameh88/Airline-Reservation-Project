#pragma once
#include <string>
#include <vector>
#include "User.h"

class FileManager;
class ReportManager;
class CrewMember;
class Aircraft;
class Flight;
class Maintenance;

class Admin : public User{
    private:
    FileManager* fm;
    public:
    Admin(int id, std::string name, std::string email, std::string password, std::string phone, FileManager* fm);
    void addCrewMember(CrewMember* cm);
    void addAircraft(Aircraft* aircraft);
    void addFlight(Flight* flight);
    void updateCrewMember(int id, CrewMember& cm);
    void updateFlight(int id, Flight& f);
    void updateAircraft(int id, Aircraft& ac);
    void deleteCrewMember(int id);
    void deleteFlight(int id);
    void deleteAircraft(int id);
};
