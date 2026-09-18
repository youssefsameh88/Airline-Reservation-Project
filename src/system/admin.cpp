#include "Ui.h"
#include "FileManager.h"
#include "Admin.h"
#include "Pilot.h"
#include "FlightAttendant.h"
#include "Aircraft.h"
#include "Flight.h"
#include "Seat.h"
#include "CrewMember.h"
#include "Maintenance.h"
#include "Enums.h"
#include <iostream>
#include <iomanip>
#include <memory>
#include <sstream>

void ui::adminMain(FileManager* fm, ReportManager* rm, User* me){
    for(;;){
        clear();
        banner();
        std::cout << "Logged in: " << me->getName() << " (admin)\n";
        screenTitle("ADMIN MENU");
        std::cout << "  1)  Add crew member\n";
        std::cout << "  2)  Crew list\n";
        std::cout << "  3)  Update crew member\n";
        std::cout << "  4)  Delete crew member\n";
        std::cout << "  5)  Add aircraft\n";
        std::cout << "  6)  Aircraft list\n";
        std::cout << "  7)  Delete aircraft\n";
        std::cout << "  8)  Add flight\n";
        std::cout << "  9)  Flight list\n";
        std::cout << "  10) Change flight status\n";
        std::cout << "  11) Delete flight\n";
        std::cout << "  12) Add maintenance record\n";
        std::cout << "  13) Reports\n";
        std::cout << "  0)  Logout\n\n";
        int ch = pick(0, 13);
        if(ch == 0) break;
        switch(ch){
            case 1:  addCrew(fm); break;
            case 2:  showCrew(fm); break;
            case 3:  editCrew(fm); break;
            case 4:  removeCrew(fm); break;
            case 5:  addAircraft(fm); break;
            case 6:  showAircraft(fm); break;
            case 7:  removeAircraft(fm); break;
            case 8:  addFlight(fm); break;
            case 9:  showFlights(fm); break;
            case 10: changeFlightStatus(fm); break;
            case 11: removeFlight(fm); break;
            case 12: addMaintenance(fm); break;
            case 13: adminReports(fm, rm); break;
        }
    }
    std::cout << "\nLogged out.\n";
    waitEnter();
}

void ui::addCrew(FileManager* fm){
    clear();
    banner();
    screenTitle("ADD CREW MEMBER");
    std::string name = askRequired("Name: ");
    int maxFH = askInt("Max flight hours: ", 1, 10000);
    std::cout << "\nType: 1) Pilot  2) Flight attendant\n\n";
    int t = pick(1, 2);

    int id = fm->getNextId("CrewMember");
    std::unique_ptr<CrewMember> c;
    if(t == 1){
        std::string ln = askRequired("License number: ");
        std::string lt = askRequired("License type (ATPL/CPL): ");
        c = std::make_unique<Pilot>(id, name, maxFH, ln, lt);
    } else {
        c = std::make_unique<FlightAttendant>(id, name, maxFH);
    }
    fm->addCrewMember(std::move(c));
    std::cout << "\nCrew member created, ID " << id << ".\n";
    waitEnter();
}

void ui::showCrew(FileManager* fm){
    clear();
    banner();
    screenTitle("CREW LIST");
    auto crew = fm->getAllCrewMembers();
    if(crew.empty()){
        std::cout << "  No crew members.\n";
        waitEnter();
        return;
    }
    std::cout << "\n  ID   Name                 Type            Hours (cur/max)\n";
    std::cout << "  " << std::string(56, '-') << "\n";
    for(const auto& it : crew){
        CrewMember* c = it.second;
        std::cout << "  " << std::left
                  << std::setw(4) << c->getId()
                  << std::setw(21) << c->getName()
                  << std::setw(16) << (c->getCrewType() == CrewType::Pilot ? "Pilot" : "Attendant")
                  << c->getCurrentFlightHours() << "/" << c->getMaxFlightHours() << "\n";
    }
    waitEnter();
}

void ui::editCrew(FileManager* fm){
    clear();
    banner();
    screenTitle("UPDATE CREW MEMBER");
    auto crew = fm->getAllCrewMembers();
    if(crew.empty()){
        std::cout << "  No crew members.\n";
        waitEnter();
        return;
    }
    for(const auto& it : crew)
        std::cout << "  " << it.second->getId() << ": " << it.second->getName() << "\n";

    int cmId = askInt("\nCrew member ID: ", 1, INT_MAX);
    CrewMember* c = fm->getCrewMemberById(cmId);
    if(!c){
        std::cout << "  Not found.\n";
        waitEnter();
        return;
    }

    std::string nm = ask("New name [" + c->getName() + "]: ");
    if(nm.empty()) nm = c->getName();
    int maxFH = c->getMaxFlightHours();
    std::string mfh = ask("New max hours [" + std::to_string(maxFH) + "]: ");
    if(!mfh.empty()){
        try{ maxFH = std::stoi(mfh); }catch(...){}
    }

    if(c->getCrewType() == CrewType::Pilot){
        Pilot* p = dynamic_cast<Pilot*>(c);
        Pilot tmp(cmId, nm, maxFH, p->getLicenseNumber(), p->getLicenseType());
        fm->updateCrewMember(cmId, tmp);
    } else {
        FlightAttendant tmp(cmId, nm, maxFH);
        fm->updateCrewMember(cmId, tmp);
    }
    std::cout << "  Crew member updated.\n";
    waitEnter();
}

void ui::removeCrew(FileManager* fm){
    clear();
    banner();
    screenTitle("DELETE CREW MEMBER");
    auto crew = fm->getAllCrewMembers();
    if(crew.empty()){
        std::cout << "  No crew members.\n";
        waitEnter();
        return;
    }
    for(const auto& it : crew)
        std::cout << "  " << it.second->getId() << ": " << it.second->getName() << "\n";

    int cmId = askInt("\nCrew member ID: ", 1, INT_MAX);
    CrewMember* c = fm->getCrewMemberById(cmId);
    if(!c){
        std::cout << "  Not found.\n";
        waitEnter();
        return;
    }
    if(confirm("Delete " + c->getName())){
        fm->deleteCrewMember(cmId);
        std::cout << "  Crew member deleted.\n";
    } else {
        std::cout << "  Not deleted.\n";
    }
    waitEnter();
}

void ui::addAircraft(FileManager* fm){
    clear();
    banner();
    screenTitle("ADD AIRCRAFT");
    std::string name = askRequired("Name (e.g. Boeing 737): ");
    int rows = askInt("Rows: ", 1, 200);
    int cols = askInt("Seats per row: ", 1, 20);
    std::cout << "\nRows 1-5 first class, 6-15 business, the rest economy.\n";

    int id = fm->getNextId("Aircraft");
    auto all = fm->getAllSeats();
    int nextSeatId = all.empty() ? 1 : (int)all.rbegin()->first + 1;
    fm->addAircraft(std::make_unique<Aircraft>(id, name, rows, cols, nextSeatId));
    std::cout << "\nAircraft added, ID " << id << " (" << rows * cols << " seats).\n";
    waitEnter();
}

void ui::showAircraft(FileManager* fm){
    clear();
    banner();
    screenTitle("AIRCRAFT LIST");
    auto acs = fm->getAllAircraft();
    if(acs.empty()){
        std::cout << "  No aircraft.\n";
        waitEnter();
        return;
    }
    std::cout << "\n  ID   Name                Rows  Cols  Seats\n";
    std::cout << "  " << std::string(44, '-') << "\n";
    for(const auto& it : acs){
        Aircraft* a = it.second;
        std::cout << "  " << std::left
                  << std::setw(4) << a->getId()
                  << std::setw(20) << a->getName()
                  << std::setw(5) << a->getRows()
                  << std::setw(5) << a->getCols()
                  << a->getNumOfSeats() << "\n";
    }
    waitEnter();
}

void ui::removeAircraft(FileManager* fm){
    clear();
    banner();
    screenTitle("DELETE AIRCRAFT");
    auto acs = fm->getAllAircraft();
    if(acs.empty()){
        std::cout << "  No aircraft.\n";
        waitEnter();
        return;
    }
    for(const auto& it : acs)
        std::cout << "  " << it.second->getId() << ": " << it.second->getName() << "\n";

    int acId = askInt("\nAircraft ID: ", 1, INT_MAX);
    Aircraft* a = fm->getAircraftById(acId);
    if(!a){
        std::cout << "  Not found.\n";
        waitEnter();
        return;
    }
    if(confirm("Delete " + a->getName())){
        fm->deleteAircraft(acId);
        std::cout << "  Aircraft deleted.\n";
    } else {
        std::cout << "  Not deleted.\n";
    }
    waitEnter();
}

void ui::addFlight(FileManager* fm){
    clear();
    banner();
    screenTitle("ADD FLIGHT");
    auto acs = fm->getAllAircraft();
    if(acs.empty()){
        std::cout << "  Add an aircraft first.\n";
        waitEnter();
        return;
    }
    for(const auto& it : acs)
        std::cout << "  " << it.second->getId() << ": " << it.second->getName()
                  << " (" << it.second->getNumOfSeats() << " seats)\n";
    int acId = askInt("\nAircraft ID: ", 1, INT_MAX);
    Aircraft* ac = fm->getAircraftById(acId);
    if(!ac){
        std::cout << "  Not found.\n";
        waitEnter();
        return;
    }

    std::string from = askRequired("Origin: ");
    std::string to = askRequired("Destination: ");
    std::string dep = askRequired("Departure (YYYY-MM-DD HH:MM): ");
    std::string arr = askRequired("Arrival (YYYY-MM-DD HH:MM): ");
    int price = askInt("Price in EGP: ", 0, 1000000);

    auto crew = fm->getAllCrewMembers();
    if(!crew.empty()){
        std::cout << "\nCrew:\n";
        for(const auto& it : crew)
            std::cout << "  " << it.second->getId() << ": " << it.second->getName()
                      << " (" << (it.second->getCrewType() == CrewType::Pilot ? "Pilot" : "Attendant") << ")\n";
    }
    std::cout << "\nCrew IDs (comma separated, optional): ";
    std::vector<CrewMember*> crewList;
    std::istringstream ss(ask(""));
    std::string tok;
    while(std::getline(ss, tok, ',')){
        try{
            int cid = std::stoi(tok);
            CrewMember* c = fm->getCrewMemberById(cid);
            if(c) crewList.push_back(c);
            else std::cout << "  Skipping unknown crew ID " << cid << "\n";
        }catch(...){}
    }

    int id = fm->getNextId("Flight");
    fm->addFlight(std::make_unique<Flight>(id, price, from, to, dep, arr, ac, crewList));
    std::cout << "\nFlight added, ID " << id << ".\n";
    waitEnter();
}

void ui::showFlights(FileManager* fm){
    clear();
    banner();
    screenTitle("FLIGHT LIST");
    flightList(fm, "", 0, INT_MAX);
    waitEnter();
}

void ui::changeFlightStatus(FileManager* fm){
    clear();
    banner();
    screenTitle("CHANGE FLIGHT STATUS");
    auto flights = fm->getAllFlights();
    if(flights.empty()){
        std::cout << "  No flights.\n";
        waitEnter();
        return;
    }
    for(const auto& it : flights)
        std::cout << "  " << it.second->getId() << ": " << it.second->getOrigin()
                  << " -> " << it.second->getDestination()
                  << "  [" << statusText(it.second->getFlightStatus()) << "]\n";

    int fId = askInt("\nFlight ID: ", 1, INT_MAX);
    Flight* f = fm->getFlightById(fId);
    if(!f){
        std::cout << "  Not found.\n";
        waitEnter();
        return;
    }
    std::cout << "\nNew status:\n  1) Scheduled  2) Boarding  3) Departed  4) Arrived  5) Cancelled\n\n";
    int ch = pick(1, 5);
    FlightStatus st;
    switch(ch){
        case 2: st = FlightStatus::Boarding; break;
        case 3: st = FlightStatus::Departed; break;
        case 4: st = FlightStatus::Arrived; break;
        case 5: st = FlightStatus::Cancelled; break;
        default: st = FlightStatus::Scheduled; break;
    }
    f->setFlightStatus(st);
    fm->updateFlight(fId, *f);
    std::cout << "  Status is now " << statusText(st) << ".\n";
    waitEnter();
}

void ui::removeFlight(FileManager* fm){
    clear();
    banner();
    screenTitle("DELETE FLIGHT");
    auto flights = fm->getAllFlights();
    if(flights.empty()){
        std::cout << "  No flights.\n";
        waitEnter();
        return;
    }
    for(const auto& it : flights)
        std::cout << "  " << it.second->getId() << ": " << it.second->getOrigin()
                  << " -> " << it.second->getDestination() << "\n";

    int fId = askInt("\nFlight ID: ", 1, INT_MAX);
    Flight* f = fm->getFlightById(fId);
    if(!f){
        std::cout << "  Not found.\n";
        waitEnter();
        return;
    }
    if(confirm("Delete flight " + f->getOrigin() + " -> " + f->getDestination())){
        fm->deleteFlight(fId);
        std::cout << "  Flight deleted.\n";
    } else {
        std::cout << "  Not deleted.\n";
    }
    waitEnter();
}

void ui::addMaintenance(FileManager* fm){
    clear();
    banner();
    screenTitle("ADD MAINTENANCE RECORD");
    auto acs = fm->getAllAircraft();
    if(acs.empty()){
        std::cout << "  No aircraft.\n";
        waitEnter();
        return;
    }
    for(const auto& it : acs)
        std::cout << "  " << it.second->getId() << ": " << it.second->getName() << "\n";

    int acId = askInt("\nAircraft ID: ", 1, INT_MAX);
    Aircraft* ac = fm->getAircraftById(acId);
    if(!ac){
        std::cout << "  Not found.\n";
        waitEnter();
        return;
    }
    std::string date = askRequired("Date (YYYY-MM-DD): ");
    std::string desc = ask("Description: ");
    if(desc.empty()) desc = "-";
    std::cout << "\nStatus: 1) No problems  2) Needs fix  3) Fixed\n\n";
    int ch = pick(1, 3);
    MaintenanceStatus ms = ch == 1 ? MaintenanceStatus::NoProblems
                         : ch == 2 ? MaintenanceStatus::NeedsFix
                                   : MaintenanceStatus::Fixed;

    int id = fm->getNextId("Maintenance");
    fm->addMaintenance(std::make_unique<Maintenance>(id, ac, date, desc, ms));
    std::cout << "\nMaintenance record added, ID " << id << ".\n";
    waitEnter();
}