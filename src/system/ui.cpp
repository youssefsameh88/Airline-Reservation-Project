#include "Ui.h"
#include "FileManager.h"
#include "Aircraft.h"
#include "Flight.h"
#include "Seat.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <limits>

static std::string trim(const std::string& s){
    size_t a = s.find_first_not_of(" \t\r\n");
    if(a == std::string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

static bool toInt(const std::string& s, int& out){
    try{
        size_t pos = 0;
        int n = std::stoi(s, &pos);
        if(pos == s.size()){
            out = n;
            return true;
        }
    }catch(...){}
    return false;
}

namespace ui {

void clear(){
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waitEnter(){
    std::cout << "\nPress Enter to continue...";
    std::string line;
    std::getline(std::cin, line);
}

void banner(){
    std::cout << "\n";
    std::cout << "===============================\n";
    std::cout << "   AIRLINE RESERVATION SYSTEM\n";
    std::cout << "===============================\n";
}

void screenTitle(const std::string& t){
    std::cout << "\n--- " << t << " ---\n";
}

void hr(char c, int w){
    std::cout << std::string(w, c) << "\n";
}

std::string ask(const std::string& label){
    std::cout << label;
    std::string line;
    std::getline(std::cin, line);
    return trim(line);
}

std::string askRequired(const std::string& label){
    for(;;){
        std::string v = ask(label);
        if(!v.empty()) return v;
        std::cout << "  (cannot be empty)\n";
    }
}

int askInt(const std::string& label, int lo, int hi){
    for(;;){
        int n = 0;
        if(toInt(ask(label), n) && n >= lo && n <= hi) return n;
        if(lo == INT_MIN && hi == INT_MAX) std::cout << "  Not a valid number.\n";
        else std::cout << "  Enter a number between " << lo << " and " << hi << ".\n";
    }
}

int pick(int lo, int hi){
    return askInt("Your choice [" + std::to_string(lo) + "-" + std::to_string(hi) + "]: ", lo, hi);
}

bool confirm(const std::string& message){
    std::string v = ask(message + " [y/n]: ");
    return v == "y" || v == "Y";
}

std::string lower(std::string s){
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return (char)::tolower(c); });
    return s;
}

std::string upper(std::string s){
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return (char)::toupper(c); });
    return s;
}

std::string passwordHash(const std::string& pw){
    unsigned long h = 5381;
    for(char c : pw)
        h = ((h << 5) + h) + (unsigned char)c;
    std::ostringstream out;
    out << std::hex << std::setw(16) << std::setfill('0') << h;
    return out.str();
}

std::string statusText(FlightStatus s){
    switch(s){
        case FlightStatus::Scheduled: return "Scheduled";
        case FlightStatus::Boarding:  return "Boarding";
        case FlightStatus::Departed:  return "Departed";
        case FlightStatus::Arrived:   return "Arrived";
        case FlightStatus::Cancelled: return "Cancelled";
    }
    return "Unknown";
}

std::string seatText(SeatType t){
    switch(t){
        case SeatType::FirstClass: return "First class";
        case SeatType::Business:   return "Business";
        default:                   return "Economy";
    }
}

void seatMap(Flight* f){
    if(!f || !f->getAircraft()){
        std::cout << "  (no aircraft assigned)\n";
        return;
    }
    Aircraft* ac = f->getAircraft();
    int cols = ac->getCols();
    std::cout << "\nSeat map  (O = free, X = taken)\n\n";
    std::cout << "     ";
    for(int j = 0; j < cols; j++)
        std::cout << " " << (char)('A' + j);
    std::cout << "\n";
    std::cout << "     " << std::string(cols * 2 + 1, '-') << "\n";
    for(const auto& row : ac->getSeats()){
        std::cout << "  " << std::setw(3) << row[0]->getName().substr(1) << " ";
        for(Seat* s : row)
            std::cout << (s->getIsAvailable() ? " O" : " X");
        std::cout << "\n";
    }
}

void flightList(FileManager* fm, const std::string& dest, int minP, int maxP){
    auto flights = fm->getAllFlights();
    std::string q = lower(dest);
    std::cout << "\n  ID   Origin      Destination    Departure           Price   Status      Free\n";
    std::cout << "  " << std::string(72, '-') << "\n";
    bool any = false;
    for(const auto& it : flights){
        Flight* f = it.second;
        if(!q.empty() && lower(f->getDestination()).find(q) == std::string::npos) continue;
        if(f->getPrice() < minP || f->getPrice() > maxP) continue;
        int free = 0;
        if(f->getAircraft())
            for(const auto& row : f->getAircraft()->getSeats())
                for(Seat* s : row)
                    if(s->getIsAvailable()) free++;
        std::cout << "  " << std::left
                  << std::setw(4) << f->getId()
                  << std::setw(12) << f->getOrigin()
                  << std::setw(14) << f->getDestination()
                  << std::setw(20) << f->getDepartureTime()
                  << std::setw(8) << f->getPrice()
                  << std::setw(12) << statusText(f->getFlightStatus())
                  << free << "\n";
        any = true;
    }
    if(!any) std::cout << "  No flights found.\n";
}

} // namespace ui