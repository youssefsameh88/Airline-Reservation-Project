#include "Reservation.h"
#include "Passenger.h"
#include "Flight.h"
#include <ctime>

static std::string nowString(){
    std::time_t t = std::time(nullptr);
    std::tm* tm = std::localtime(&t);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", tm);
    return std::string(buf);
}

Reservation::Reservation(int id, Passenger* p, Flight* f, Seat* seat){
    this->id = id;
    this->passenger = p;
    this->flight = f;
    this->seat = seat;
    this->price = f ? f->getPrice() : 0;
    this->createdAt = nowString();
    reservationStatus = ReservationStatus::Accepted;
}

int Reservation::getId() const {return id;}
int Reservation::getPassengerId() const {return passenger->getId();}
int Reservation::getPrice() const {return price;}
Seat* Reservation::getSeat() const {return seat;}
Flight* Reservation::getFlight() const {return flight;}
std::string Reservation::getCreatedAt() const {return createdAt;}
ReservationStatus Reservation::getReservationStatus() const {return reservationStatus;}
void Reservation::setReservationStatus(ReservationStatus status){reservationStatus = status;}
