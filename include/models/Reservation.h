#pragma once
#include <string>
#include "Enums.h"

class Passenger;
class Flight;
class Seat;

class Reservation{
    private:
    int id, price;
    std::string createdAt;
    Passenger* passenger;
    Flight* flight;
    Seat* seat;
    ReservationStatus reservationStatus;

    public:
    Reservation(int id, Passenger* p, Flight* f, Seat* seat);
    int getId() const;
    int getPassengerId() const;
    int getPrice() const;
    Seat* getSeat() const;
    Flight* getFlight() const;
    std::string getCreatedAt() const;
    ReservationStatus getReservationStatus() const;
    void setReservationStatus(ReservationStatus status);
};