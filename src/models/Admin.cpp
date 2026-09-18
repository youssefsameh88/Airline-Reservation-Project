#include <memory>
#include <iostream>
#include "Admin.h"
#include "FileManager.h"

Admin::Admin(int id, std::string name, std::string email, std::string password, std::string phone
    ,FileManager* fm)
:User(id, name, email, password, phone, Role::Admin)
{
    this->fm = fm;
}


void Admin::addCrewMember(CrewMember* cm){
    fm->addCrewMember(std::unique_ptr<CrewMember>(cm));
}
void Admin::addAircraft(Aircraft* aircraft){
    fm->addAircraft(std::unique_ptr<Aircraft>(aircraft));
}
void Admin::addFlight(Flight* flight){
    fm->addFlight(std::unique_ptr<Flight>(flight));
}

void Admin::updateCrewMember(int id, CrewMember& cm){
    fm->updateCrewMember(id, cm);
}
void Admin::updateFlight(int id, Flight& f){
    fm->updateFlight(id, f);
}
void Admin::updateAircraft(int id, Aircraft& ac){
    fm->updateAircraft(id, ac);
}

void Admin::deleteCrewMember(int id){
    fm->deleteCrewMember(id);
}
void Admin::deleteFlight(int id){
    fm->deleteFlight(id);
}
void Admin::deleteAircraft(int id){
    fm->deleteAircraft(id);
}