#include "Flight.h"
#include "CrewMember.h"

Flight::Flight(int id, int price, std::string origin, std::string destination, std::string departureTime,
     std::string arrivalTime, Aircraft* ac, std::vector<CrewMember*> cm){
    this->id = id;
    this->price = price;
    this->origin = origin;
    this->destination = destination;
    this->departureTime = departureTime;
    this->arrivalTime = arrivalTime;
    this->aircraft = ac;
    this->crewMembers = cm;
    this->flightStatus = FlightStatus::Scheduled;
}

void Flight::addReservation(Reservation* res){
    reservations.push_back(res);
}

void Flight::removeCrewMember(int id){
    std::vector<CrewMember*> kept;
    for (CrewMember* cm : crewMembers)
        if (cm->getId() != id) kept.push_back(cm);
    crewMembers = kept;
}

void Flight::setAircraft(Aircraft* ac){
    aircraft = ac;
}

int Flight::getId(){return id;}
int Flight::getPrice(){return price;}
std::string Flight::getOrigin(){return origin;}
std::string Flight::getDestination(){return destination;}
std::string Flight::getDepartureTime(){return departureTime;}
std::string Flight::getArrivalTime(){return arrivalTime;}
Aircraft* Flight::getAircraft(){return aircraft;}
FlightStatus Flight::getFlightStatus(){return flightStatus;}
void Flight::setFlightStatus(FlightStatus status){flightStatus = status;}
std::vector<const Reservation*> Flight::getReservations(){
    std::vector<const Reservation*> res;
    for (Reservation* r : reservations) res.push_back(r);
    return res;
}
std::vector<CrewMember*> Flight::getCrewMembers(){return crewMembers;}
