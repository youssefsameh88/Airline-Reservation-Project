#include "Ui.h"
#include "FileManager.h"
#include "BookingAgent.h"
#include "Passenger.h"
#include "Aircraft.h"
#include "Flight.h"
#include "Seat.h"
#include "Reservation.h"
#include "Enums.h"
#include <iostream>
#include <memory>

void ui::agentMain(FileManager* fm, ReportManager* rm, User* me){
    for(;;){
        clear();
        banner();
        std::cout << "Logged in: " << me->getName() << " (booking agent)\n";
        screenTitle("AGENT MENU");
        std::cout << "  1) Search flights\n";
        std::cout << "  2) Make reservation\n";
        std::cout << "  3) Cancel reservation\n";
        std::cout << "  4) All reservations\n";
        std::cout << "  5) Reports\n";
        std::cout << "  0) Logout\n\n";
        int ch = pick(0, 5);
        if(ch == 0) break;
        switch(ch){
            case 1: searchFlights(fm); break;
            case 2: makeReservation(fm, me); break;
            case 3: cancelReservation(fm, me); break;
            case 4: showAllReservations(fm); break;
            case 5: agentReports(fm, rm); break;
        }
    }
    std::cout << "\nLogged out.\n";
    waitEnter();
}

void ui::searchFlights(FileManager* fm){
    clear();
    banner();
    screenTitle("SEARCH FLIGHTS");
    std::string dest = ask("Destination (Enter for all): ");
    flightList(fm, dest, 0, INT_MAX);
    waitEnter();
}

void ui::makeReservation(FileManager* fm, User* me){
    clear();
    banner();
    screenTitle("NEW RESERVATION");
    flightList(fm, "", 0, INT_MAX);

    int fId = askInt("\nFlight ID: ", 1, INT_MAX);
    Flight* f = fm->getFlightById(fId);
    if(!f){
        std::cout << "  No such flight.\n";
        waitEnter();
        return;
    }
    if(f->getFlightStatus() == FlightStatus::Cancelled ||
       f->getFlightStatus() == FlightStatus::Arrived ||
       f->getFlightStatus() == FlightStatus::Departed){
        std::cout << "  Cannot book a " << statusText(f->getFlightStatus()) << " flight.\n";
        waitEnter();
        return;
    }

    clear();
    banner();
    screenTitle("NEW RESERVATION");
    seatMap(f);

    std::string seatName = ask("\nSeat (e.g. A01): ");
    seatName = upper(seatName);
    if(seatName.empty()) return;
    if(!f->getAircraft()){ std::cout << "  No aircraft assigned.\n"; waitEnter(); return; }
    Seat* s = f->getAircraft()->getSeat(seatName);
    if(!s){ std::cout << "  No seat named " << seatName << ".\n"; waitEnter(); return; }
    if(!s->getIsAvailable()){ std::cout << "  Seat already taken.\n"; waitEnter(); return; }

    int pId = askInt("Passenger ID: ", 1, INT_MAX);
    Passenger* p = dynamic_cast<Passenger*>(fm->getUserById(pId));
    if(!p){
        std::cout << "  No passenger with ID " << pId << ".\n";
        waitEnter();
        return;
    }

    BookingAgent* ba = dynamic_cast<BookingAgent*>(me);
    if(ba){
        ba->addReservation(pId, fId, s->getId());
    } else {
        int resId = fm->getNextId("Reservation");
        auto res = std::make_unique<Reservation>(resId, p, f, s);
        p->addReservation(res.get());
        f->addReservation(res.get());
        s->setIsAvailable(false);
        fm->addReservation(std::move(res));
        p->updateLoyaltyPoints(f->getPrice() / 10);
        fm->updateUser(p->getId(), *p);
    }
    std::cout << "\nReservation created for " << p->getName()
              << " on " << f->getOrigin() << " -> " << f->getDestination()
              << ", seat " << seatName << ".\n";
    waitEnter();
}

void ui::cancelReservation(FileManager* fm, User* me){
    clear();
    banner();
    screenTitle("CANCEL RESERVATION");
    auto res = fm->getAllReservations();
    if(res.empty()){
        std::cout << "  No reservations found.\n";
        waitEnter();
        return;
    }
    for(const auto& it : res){
        Reservation* r = it.second;
        Flight* f = r->getFlight();
        std::cout << "  " << r->getId() << "  "
                  << (f ? (f->getOrigin() + " -> " + f->getDestination()) : "?")
                  << "  seat " << (r->getSeat() ? r->getSeat()->getName() : "?")
                  << "  " << (r->getReservationStatus() == ReservationStatus::Accepted
                                ? "active" : "cancelled") << "\n";
    }

    int rId = askInt("\nReservation ID: ", 1, INT_MAX);
    auto it = res.find(rId);
    if(it == res.end()){
        std::cout << "  No reservation with ID " << rId << ".\n";
        waitEnter();
        return;
    }
    if(it->second->getReservationStatus() == ReservationStatus::Cancelled){
        std::cout << "  Already cancelled.\n";
        waitEnter();
        return;
    }
    if(!confirm("Cancel reservation " + std::to_string(rId))){
        std::cout << "  Not cancelled.\n";
        waitEnter();
        return;
    }

    BookingAgent* ba = dynamic_cast<BookingAgent*>(me);
    if(ba) ba->cancelReservation(rId);
    else   fm->cancelReservation(rId);
    std::cout << "  Reservation cancelled.\n";
    waitEnter();
}

void ui::showAllReservations(FileManager* fm){
    clear();
    banner();
    screenTitle("ALL RESERVATIONS");
    auto res = fm->getAllReservations();
    if(res.empty()){
        std::cout << "  No reservations found.\n";
        waitEnter();
        return;
    }
    for(const auto& it : res){
        Reservation* r = it.second;
        Flight* f = r->getFlight();
        std::cout << "  " << r->getId() << "  flight "
                  << (f ? f->getId() : -1) << "  "
                  << (f ? (f->getOrigin() + " -> " + f->getDestination()) : "?")
                  << "  passenger " << r->getPassengerId()
                  << "  seat " << (r->getSeat() ? r->getSeat()->getName() : "?")
                  << "  " << (r->getReservationStatus() == ReservationStatus::Accepted
                                ? "active" : "cancelled") << "\n";
    }
    waitEnter();
}