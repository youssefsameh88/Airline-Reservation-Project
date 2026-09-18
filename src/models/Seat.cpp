#include "Seat.h"

Seat::Seat(int id, std::string name, SeatType st){
    this->id = id;
    this->name = name;
    this->seatType = st;
    this->isAvailable = true;
}

int Seat::getId(){return id;}
std::string Seat::getName(){return name;}
SeatType Seat::getType(){return seatType;}
void Seat::setIsAvailable(bool av){isAvailable = av;}
bool Seat::getIsAvailable(){return isAvailable;}
