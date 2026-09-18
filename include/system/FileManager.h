#pragma once
#include <string>
#include <vector>
#include <memory>
#include <map>
#include "User.h"
#include "Maintenance.h"
#include "Aircraft.h"
#include "Flight.h"
#include "CrewMember.h"
#include "Reservation.h"
#include "Seat.h"

class FileManager{
    private:
    std::map<int, std::unique_ptr<User>> users;
    std::map<int, std::unique_ptr<Aircraft>> aircrafts;
    std::map<int, std::unique_ptr<CrewMember>> crewMembers;
    std::map<int, std::unique_ptr<Flight>> flights;
    std::map<int, std::unique_ptr<Maintenance>> maintenance;
    std::map<int, std::unique_ptr<Reservation>> reservations;
    std::map<int, Seat*> seats;
    std::map<std::string, int> emails;

    public:
    FileManager();
    User* getUserById(int id);
    Aircraft* getAircraftById(int id);
    CrewMember* getCrewMemberById(int id);
    Flight* getFlightById(int id);
    Maintenance* getMaintenanceById(int id);
    Seat* getSeatById(int id);

    std::map<int, User*> getAllUsers();
    std::map<int, Aircraft*> getAllAircraft();
    std::map<int, CrewMember*> getAllCrewMembers();
    std::map<int, Flight*> getAllFlights();
    std::map<int, Maintenance*> getAllMaintenance();
    std::map<int, Reservation*> getAllReservations();
    std::map<int, Seat*> getAllSeats();

    void addCrewMember(std::unique_ptr<CrewMember> cm);
    void addAircraft(std::unique_ptr<Aircraft> aircraft);
    void addFlight(std::unique_ptr<Flight> f);
    void addUser(std::unique_ptr<User> user);
    void addReservation(std::unique_ptr<Reservation> res);
    void addMaintenance(std::unique_ptr<Maintenance> m);

    void updateCrewMember(int id, CrewMember& cm);
    void updateFlight(int id, Flight& f);
    void updateAircraft(int id, Aircraft& ac);
    void updateUser(int id, User& u);
    void updateReservation(int id, Reservation& res);

    void deleteCrewMember(int cmId);
    void deleteFlight(int fId);
    void deleteAircraft(int acId);
    void deleteUser(int uId);
    void cancelReservation(int resId);

    int getNextId(std::string className);
    int getIdByEmail(std::string email);
};
