#include "ReportManager.h"
#include "FileManager.h"
#include "Flight.h"
#include "Reservation.h"
#include "Aircraft.h"
#include "CrewMember.h"
#include "Maintenance.h"
#include "Passenger.h"
#include "Seat.h"
#include "Enums.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>

static void printLine(char c = '-', int width = 72){
    std::cout << std::string(width, c) << "\n";
}

static void printHeader(const std::string& title){
    printLine('=');
    int pad = (72 - (int)title.size()) / 2;
    std::cout << std::string(pad, ' ') << title << "\n";
    printLine('=');
}

static std::string flightStatusStr(FlightStatus fs){
    switch(fs){
        case FlightStatus::Scheduled: return "Scheduled";
        case FlightStatus::Boarding:  return "Boarding";
        case FlightStatus::Departed:  return "Departed";
        case FlightStatus::Arrived:   return "Arrived";
        case FlightStatus::Cancelled: return "Cancelled";
    }
    return "Unknown";
}

static std::string resStatusStr(ReservationStatus rs){
    return rs == ReservationStatus::Accepted ? "Accepted" : "Cancelled";
}

static std::string maintStatusStr(MaintenanceStatus ms){
    switch(ms){
        case MaintenanceStatus::NoProblems: return "No Problems";
        case MaintenanceStatus::NeedsFix:   return "Needs Fix";
        case MaintenanceStatus::Fixed:      return "Fixed";
    }
    return "Unknown";
}

static std::string crewTypeStr(CrewType ct){
    return ct == CrewType::Pilot ? "Pilot" : "Flight Attendant";
}

ReportManager::ReportManager(FileManager* fm) : fm(fm) {}

void ReportManager::generateAllFlightsPerformanceReport(){
    printHeader("ALL FLIGHTS PERFORMANCE REPORT");

    auto flights = fm->getAllFlights();
    if(flights.empty()){
        std::cout << "  No flights found.\n";
        printLine('=');
        return;
    }

    std::cout << std::left
              << std::setw(5)  << "ID"
              << std::setw(10) << "Origin"
              << std::setw(14) << "Destination"
              << std::setw(22) << "Departure"
              << std::setw(10) << "Status"
              << std::setw(10) << "Seats"
              << std::setw(12) << "Reserved"
              << std::setw(10) << "Occupancy"
              << "\n";
    printLine();

    for(auto& kv : flights){
        Flight* f = kv.second;
        auto reservations = f->getReservations();

        int accepted = 0;
        for(const Reservation* r : reservations)
            if(r->getReservationStatus() == ReservationStatus::Accepted) accepted++;

        int totalSeats = f->getAircraft() ? f->getAircraft()->getNumOfSeats() : 0;
        double occupancy = totalSeats > 0 ? (100.0 * accepted / totalSeats) : 0.0;

        std::cout << std::left
                  << std::setw(5)  << f->getId()
                  << std::setw(10) << f->getOrigin().substr(0,9)
                  << std::setw(14) << f->getDestination().substr(0,13)
                  << std::setw(22) << f->getDepartureTime().substr(0,21)
                  << std::setw(10) << flightStatusStr(f->getFlightStatus()).substr(0,9)
                  << std::setw(10) << totalSeats
                  << std::setw(12) << accepted
                  << std::setw(10) << std::fixed << std::setprecision(1) << occupancy << "%"
                  << "\n";
    }
    printLine('=');
}

void ReportManager::generateRevenueReport(){
    printHeader("REVENUE REPORT");

    auto flights = fm->getAllFlights();
    if(flights.empty()){
        std::cout << "  No flights found.\n";
        printLine('=');
        return;
    }

    std::cout << std::left
              << std::setw(5)  << "ID"
              << std::setw(20) << "Route"
              << std::setw(10) << "Price/Seat"
              << std::setw(12) << "Reservations"
              << std::setw(14) << "Revenue (EGP)"
              << "\n";
    printLine();

    int totalRevenue = 0;
    for(auto& kv : flights){
        Flight* f = kv.second;
        auto reservations = f->getReservations();
        int accepted = 0;
        for(const Reservation* r : reservations)
            if(r->getReservationStatus() == ReservationStatus::Accepted) accepted++;
        int revenue = f->getPrice() * accepted;
        totalRevenue += revenue;

        std::string route = f->getOrigin() + " -> " + f->getDestination();
        if(route.size() > 19) route = route.substr(0, 16) + "...";

        std::cout << std::left
                  << std::setw(5)  << f->getId()
                  << std::setw(20) << route
                  << std::setw(10) << f->getPrice()
                  << std::setw(12) << accepted
                  << std::setw(14) << revenue
                  << "\n";
    }
    printLine();
    std::cout << std::right << std::setw(47) << "TOTAL REVENUE: "
              << totalRevenue << " EGP\n";
    printLine('=');
}

void ReportManager::generateMaintenanceReport(){
    printHeader("MAINTENANCE REPORT");

    auto maintMap = fm->getAllMaintenance();
    if(maintMap.empty()){
        std::cout << "  No maintenance records found.\n";
        printLine('=');
        return;
    }

    std::cout << std::left
              << std::setw(5)  << "ID"
              << std::setw(20) << "Aircraft"
              << std::setw(14) << "Date"
              << std::setw(20) << "Status"
              << "Description"
              << "\n";
    printLine();

    for(auto& kv : maintMap){
        Maintenance* m = kv.second;
        std::string acName = m->getAircraft() ? m->getAircraft()->getName() : "N/A";
        if(acName.size() > 19) acName = acName.substr(0,16) + "...";

        std::string desc = m->getDescription();
        if(desc.size() > 30) desc = desc.substr(0,27) + "...";

        std::cout << std::left
                  << std::setw(5)  << m->getId()
                  << std::setw(20) << acName
                  << std::setw(14) << m->getDate()
                  << std::setw(20) << maintStatusStr(m->getMaintenanceStatus())
                  << desc
                  << "\n";
    }
    printLine('=');
}

void ReportManager::generateAircraftMaintenanceReport(int acId){
    Aircraft* ac = fm->getAircraftById(acId);
    if(!ac){
        std::cout << "  [!] Aircraft ID " << acId << " not found.\n";
        return;
    }

    printHeader("MAINTENANCE REPORT - " + ac->getName());

    auto logs = ac->getMaintenanceLog();
    if(logs.empty()){
        std::cout << "  No maintenance records for this aircraft.\n";
        printLine('=');
        return;
    }

    std::cout << std::left
              << std::setw(5)  << "ID"
              << std::setw(14) << "Date"
              << std::setw(20) << "Status"
              << "Description"
              << "\n";
    printLine();

    for(Maintenance* m : logs){
        std::string desc = m->getDescription();
        if(desc.size() > 35) desc = desc.substr(0,32) + "...";
        std::cout << std::left
                  << std::setw(5)  << m->getId()
                  << std::setw(14) << m->getDate()
                  << std::setw(20) << maintStatusStr(m->getMaintenanceStatus())
                  << desc
                  << "\n";
    }
    printLine('=');
}

void ReportManager::generatePassengerActivityReport(int pId){
    User* u = fm->getUserById(pId);
    Passenger* p = dynamic_cast<Passenger*>(u);
    if(!p){
        std::cout << "  [!] Passenger ID " << pId << " not found.\n";
        return;
    }

    printHeader("PASSENGER ACTIVITY - " + p->getName());

    std::cout << "  ID            : " << p->getId()            << "\n";
    std::cout << "  Name          : " << p->getName()          << "\n";
    std::cout << "  Email         : " << p->getEmail()         << "\n";
    std::cout << "  Phone         : " << p->getPhone()         << "\n";
    std::cout << "  Age           : " << p->getAge()           << "\n";
    std::cout << "  National ID   : " << p->getNationalId()    << "\n";
    std::cout << "  Loyalty Points: " << p->getLoyaltyPoints() << "\n";
    printLine();

    auto reservations = p->getReservations();
    if(reservations.empty()){
        std::cout << "  No reservations.\n";
    } else {
        std::cout << std::left
                  << std::setw(6)  << "ResID"
                  << std::setw(8)  << "FlightID"
                  << std::setw(18) << "Route"
                  << std::setw(8)  << "Seat"
                  << std::setw(10) << "Price"
                  << "Status"
                  << "\n";
        printLine('-', 60);
        for(Reservation* r : reservations){
            Flight* f = r->getFlight();
            std::string route = f ? (f->getOrigin() + "->" + f->getDestination()) : "N/A";
            if(route.size() > 17) route = route.substr(0,14) + "...";
            std::string seatName = r->getSeat() ? r->getSeat()->getName() : "N/A";
            std::cout << std::left
                      << std::setw(6)  << r->getId()
                      << std::setw(8)  << (f ? f->getId() : -1)
                      << std::setw(18) << route
                      << std::setw(8)  << seatName
                      << std::setw(10) << (f ? f->getPrice() : 0)
                      << resStatusStr(r->getReservationStatus())
                      << "\n";
        }
    }
    printLine('=');
}

void ReportManager::generateAllPassengersActivityReport(){
    printHeader("ALL PASSENGERS ACTIVITY REPORT");

    auto allUsers = fm->getAllUsers();
    bool found = false;

    for(auto& kv : allUsers){
        Passenger* p = dynamic_cast<Passenger*>(kv.second);
        if(!p) continue;
        found = true;

        auto reservations = p->getReservations();
        int accepted = 0, cancelled = 0;
        for(Reservation* r : reservations){
            if(r->getReservationStatus() == ReservationStatus::Accepted) accepted++;
            else cancelled++;
        }

        std::cout << "  [" << p->getId() << "] " << p->getName()
                  << "  |  Loyalty: " << p->getLoyaltyPoints()
                  << " pts  |  Reservations: " << accepted << " active, "
                  << cancelled << " cancelled\n";
    }

    if(!found) std::cout << "  No passengers found.\n";
    printLine('=');
}

void ReportManager::generateCrewFlighHoursReport(){
    printHeader("CREW FLIGHT HOURS REPORT");

    auto crewMap = fm->getAllCrewMembers();
    if(crewMap.empty()){
        std::cout << "  No crew members found.\n";
        printLine('=');
        return;
    }

    std::cout << std::left
              << std::setw(5)  << "ID"
              << std::setw(22) << "Name"
              << std::setw(18) << "Type"
              << std::setw(12) << "Current FH"
              << std::setw(12) << "Max FH"
              << "Usage"
              << "\n";
    printLine();

    for(auto& kv : crewMap){
        CrewMember* cm = kv.second;
        double usage = cm->getMaxFlightHours() > 0
                       ? (100.0 * cm->getCurrentFlightHours() / cm->getMaxFlightHours())
                       : 0.0;

        std::string warn = usage >= 90.0 ? " [!] Near limit" :
                           usage >= 75.0 ? " [~] High"        : "";

        std::cout << std::left
                  << std::setw(5)  << cm->getId()
                  << std::setw(22) << cm->getName().substr(0,21)
                  << std::setw(18) << crewTypeStr(cm->getCrewType())
                  << std::setw(12) << cm->getCurrentFlightHours()
                  << std::setw(12) << cm->getMaxFlightHours()
                  << std::fixed << std::setprecision(1) << usage << "%" << warn
                  << "\n";
    }
    printLine('=');
}
