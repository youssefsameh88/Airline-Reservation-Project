#include "Ui.h"
#include "FileManager.h"
#include "Passenger.h"
#include "Aircraft.h"
#include "Flight.h"
#include "Seat.h"
#include "Reservation.h"
#include "Enums.h"
#include <iostream>
#include <iomanip>
#include <memory>

static bool isBookable(Flight* f){
    return f->getFlightStatus() != FlightStatus::Cancelled &&
           f->getFlightStatus() != FlightStatus::Arrived &&
           f->getFlightStatus() != FlightStatus::Departed;
}

void ui::passengerMain(FileManager* fm, User* me){
    for(;;){
        clear();
        banner();
        std::cout << "Logged in: " << me->getName() << " (passenger)\n";
        screenTitle("PASSENGER MENU");
        std::cout << "  1) Search flights\n";
        std::cout << "  2) My reservations\n";
        std::cout << "  3) My profile\n";
        std::cout << "  0) Logout\n\n";
        int ch = pick(0, 3);
        if(ch == 0) break;
        switch(ch){
            case 1: searchAndBook(fm, me); break;
            case 2: myReservations(fm, me); break;
            case 3: showProfile(fm, me); break;
        }
    }
    std::cout << "\nLogged out.\n";
    waitEnter();
}

void ui::searchAndBook(FileManager* fm, User* me){
    clear();
    banner();
    screenTitle("SEARCH & BOOK");
    std::cout << "Filters (Enter to skip):\n";
    std::string dest = ask("Destination: ");
    int minP = 0, maxP = 100000;
    std::string v = ask("Min price: ");
    if(!v.empty()){ try{ minP = std::stoi(v); }catch(...){} }
    v = ask("Max price: ");
    if(!v.empty()){ try{ maxP = std::stoi(v); }catch(...){} }

    std::string q = lower(dest);
    std::cout << "\n  ID   Origin      Destination   Departure          Price\n";
    std::cout << "  " << std::string(60, '-') << "\n";
    bool any = false;
    for(const auto& it : fm->getAllFlights()){
        Flight* f = it.second;
        if(!isBookable(f)) continue;
        if(!q.empty() && lower(f->getDestination()).find(q) == std::string::npos) continue;
        if(f->getPrice() < minP || f->getPrice() > maxP) continue;
        std::cout << "  " << std::left
                  << std::setw(4) << f->getId()
                  << std::setw(12) << f->getOrigin()
                  << std::setw(14) << f->getDestination()
                  << std::setw(20) << f->getDepartureTime()
                  << f->getPrice() << "\n";
        any = true;
    }
    if(!any){
        std::cout << "  No matching flights.\n";
        waitEnter();
        return;
    }

    std::string fid = ask("\nFlight ID to book (0 to cancel): ");
    if(fid.empty() || fid == "0") return;
    int fId = 0;
    try{ fId = std::stoi(fid); }catch(...){
        std::cout << "  Not a valid ID.\n";
        waitEnter();
        return;
    }
    Flight* f = fm->getFlightById(fId);
    if(!f || !isBookable(f)){
        std::cout << "  Flight not available.\n";
        waitEnter();
        return;
    }

    clear();
    banner();
    screenTitle("FLIGHT " + f->getOrigin() + " -> " + f->getDestination());
    seatMap(f);

    std::string seatName = ask("\nSeat (e.g. A01): ");
    seatName = upper(seatName);
    if(seatName.empty()) return;
    if(!f->getAircraft()){ std::cout << "  No aircraft assigned.\n"; waitEnter(); return; }
    Seat* s = f->getAircraft()->getSeat(seatName);
    if(!s){ std::cout << "  No seat named " << seatName << ".\n"; waitEnter(); return; }
    if(!s->getIsAvailable()){ std::cout << "  Seat already taken.\n"; waitEnter(); return; }

    Passenger* p = dynamic_cast<Passenger*>(me);
    if(!p){ std::cout << "  Not a passenger account.\n"; waitEnter(); return; }

    int resId = fm->getNextId("Reservation");
    auto res = std::make_unique<Reservation>(resId, p, f, s);
    p->addReservation(res.get());
    f->addReservation(res.get());
    s->setIsAvailable(false);
    fm->addReservation(std::move(res));
    p->updateLoyaltyPoints(f->getPrice() / 10);
    fm->updateUser(p->getId(), *p);

    std::cout << "\nBooked " << seatName << " (" << seatText(s->getType())
              << ") on " << f->getOrigin() << " -> " << f->getDestination()
              << " for " << f->getPrice() << " EGP.\n";
    waitEnter();
}

void ui::myReservations(FileManager* fm, User* me){
    (void)fm;
    clear();
    banner();
    screenTitle("MY RESERVATIONS");
    Passenger* p = dynamic_cast<Passenger*>(me);
    if(!p){ std::cout << "  Not a passenger account.\n"; waitEnter(); return; }

    auto list = p->getReservations();
    if(list.empty()){
        std::cout << "  You have no reservations yet.\n";
        waitEnter();
        return;
    }
    for(Reservation* r : list){
        Flight* f = r->getFlight();
        std::cout << "  " << r->getId() << "  "
                  << (f ? (f->getOrigin() + " -> " + f->getDestination()) : "?") << "  "
                  << (f ? f->getDepartureTime() : "") << "  "
                  << (r->getSeat() ? r->getSeat()->getName() : "?") << "  "
                  << (f ? std::to_string(f->getPrice()) : "0") << " EGP  "
                  << (r->getReservationStatus() == ReservationStatus::Accepted
                        ? "active" : "cancelled") << "\n";
    }
    std::cout << "\nLoyalty points: " << p->getLoyaltyPoints() << "\n";
    waitEnter();
}