#pragma once
#include <string>
#include <vector>
#include "User.h"

class Reservation;

class Passenger : public User{
    private:
    int age, loyaltyPoints;
    std::string nationalId;
    std::vector<Reservation*> reservations;

    public:
    Passenger(int id, std::string name, std::string email, std::string password, std::string phone,
         int age, std::string nId);
    int getAge();
    int getLoyaltyPoints();
    void addReservation(Reservation* reservation);
    void removeReservation(int resId);
    void updateLoyaltyPoints(int value);
    std::string getNationalId();
    std::vector<Reservation*> getReservations();
};
