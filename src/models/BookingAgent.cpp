#include <string>
#include "BookingAgent.h"
#include "Reservation.h"
#include "FileManager.h"
#include "Passenger.h"
#include "Flight.h"
#include "Seat.h"

BookingAgent::BookingAgent(int id, std::string name, std::string email, std::string password,
 std::string phone, FileManager* fm)
 :User(id, name, email, password, phone, Role::BookingAgent)
{
    this->fm = fm;
}

void BookingAgent::addReservation(int pId, int flightId, int seatId){
    Passenger* p = dynamic_cast<Passenger*>(fm->getUserById(pId));
    Flight* f = fm->getFlightById(flightId);
    Seat* s = fm->getSeatById(seatId);
    if (!p || !f || !s || !s->getIsAvailable()) return;
    int nextId = fm->getNextId("Reservation");
    Reservation* res = new Reservation(nextId, p, f, s);
    p->addReservation(res);
    f->addReservation(res);
    fm->addReservation(std::unique_ptr<Reservation>(res));
    p->updateLoyaltyPoints(f->getPrice() / 10);
    fm->updateUser(p->getId(), *p);
}

void BookingAgent::updateReservation(int resId, Reservation& res){
    fm->updateReservation(resId, res);
}

void BookingAgent::cancelReservation(int resId){
    fm->cancelReservation(resId);
}
