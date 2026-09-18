#pragma once
#include <string>
#include <vector>
#include "Enums.h"

class FlightAttendant;
class Aircraft;
class CrewMember;
class Reservation;

class Flight{
    private:
    int id, price;
    std::string origin, destination, departureTime, arrivalTime;
    FlightStatus flightStatus;
    Aircraft* aircraft;
    std::vector<Reservation*> reservations;
    std::vector<CrewMember*> crewMembers;

    public:
    Flight(int id, int price, std::string origin, std::string destination, std::string departureTime,
     std::string arrivalTime, Aircraft* ac, std::vector<CrewMember*> cm);
    void addReservation(Reservation* res);
    void removeCrewMember(int id);
    void setAircraft(Aircraft* ac);

    int getId();
    int getPrice();
    std::string getOrigin();
    std::string getDestination();
    std::string getDepartureTime();
    std::string getArrivalTime();
    Aircraft* getAircraft();
    FlightStatus getFlightStatus();
    void setFlightStatus(FlightStatus status);
    std::vector<const Reservation*> getReservations();
    std::vector<CrewMember*> getCrewMembers();
};
