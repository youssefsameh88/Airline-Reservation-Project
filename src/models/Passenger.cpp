#include "Passenger.h"
#include "Reservation.h"
#include <algorithm>

Passenger::Passenger(int id, std::string name, std::string email, std::string password,
std::string phone, int age, std::string nationalId)
: User(id, name, email, password, phone, Role::Passenger)
{
    this->age = age;
    this->nationalId = nationalId;
    loyaltyPoints = 0;
}

int Passenger::getAge(){return age;}
int Passenger::getLoyaltyPoints(){return loyaltyPoints;}
void Passenger::addReservation(Reservation* reservation){
    reservations.push_back(reservation);
}
void Passenger::removeReservation(int resId){
    reservations.erase(
        std::remove_if(reservations.begin(), reservations.end(),
            [resId](Reservation* r){ return r->getId() == resId; }),
        reservations.end());
}
void Passenger::updateLoyaltyPoints(int value){
    loyaltyPoints += value;
    if(loyaltyPoints < 0) loyaltyPoints = 0;
}
std::string Passenger::getNationalId(){return nationalId;}
std::vector<Reservation*> Passenger::getReservations(){return reservations;}
