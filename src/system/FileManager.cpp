#include "FileManager.h"
#include "Admin.h"
#include "BookingAgent.h"
#include "Passenger.h"
#include "Pilot.h"
#include "FlightAttendant.h"
#include "Enums.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

static const std::string DATA_DIR      = "data/";
static const std::string USERS_FILE    = DATA_DIR + "users.json";
static const std::string AIRCRAFT_FILE = DATA_DIR + "aircrafts.json";
static const std::string CREW_FILE     = DATA_DIR + "crewMembers.json";
static const std::string FLIGHTS_FILE  = DATA_DIR + "flights.json";
static const std::string MAINT_FILE    = DATA_DIR + "maintenance.json";
static const std::string RES_FILE      = DATA_DIR + "reservations.json";
static const std::string IDS_FILE      = DATA_DIR + "nextIds.json";

static json readJson(const std::string& path){
    std::ifstream f(path);
    if(!f.is_open()) return json(nullptr);
    std::string content((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    if(content.empty()) return json(nullptr);
    try { return json::parse(content); }
    catch(...) { return json(nullptr); }
}

static void writeJson(const std::string& path, const json& j){
    std::ofstream f(path);
    if(!f.is_open()){
        std::cerr << "[FileManager] Cannot write to " << path << "\n";
        return;
    }
    f << j.dump(2);
}

static std::string roleToStr(Role r){
    switch(r){
        case Role::Admin:        return "Admin";
        case Role::BookingAgent: return "BookingAgent";
        default:                 return "Passenger";
    }
}
static Role strToRole(const std::string& s){
    if(s == "Admin")        return Role::Admin;
    if(s == "BookingAgent") return Role::BookingAgent;
    return Role::Passenger;
}
static std::string crewTypeToStr(CrewType ct){
    return ct == CrewType::Pilot ? "Pilot" : "FlightAttendant";
}
static CrewType strToCrewType(const std::string& s){
    return s == "Pilot" ? CrewType::Pilot : CrewType::FlightAttendant;
}
static std::string flightStatusToStr(FlightStatus fs){
    switch(fs){
        case FlightStatus::Boarding:  return "Boarding";
        case FlightStatus::Departed:  return "Departed";
        case FlightStatus::Arrived:   return "Arrived";
        case FlightStatus::Cancelled: return "Cancelled";
        default:                      return "Scheduled";
    }
}
static FlightStatus strToFlightStatus(const std::string& s){
    if(s == "Boarding")  return FlightStatus::Boarding;
    if(s == "Departed")  return FlightStatus::Departed;
    if(s == "Arrived")   return FlightStatus::Arrived;
    if(s == "Cancelled") return FlightStatus::Cancelled;
    return FlightStatus::Scheduled;
}
static std::string resStatusToStr(ReservationStatus rs){
    return rs == ReservationStatus::Accepted ? "Accepted" : "Cancelled";
}
static ReservationStatus strToResStatus(const std::string& s){
    return s == "Accepted" ? ReservationStatus::Accepted : ReservationStatus::Cancelled;
}
static std::string maintStatusToStr(MaintenanceStatus ms){
    switch(ms){
        case MaintenanceStatus::NeedsFix: return "NeedsFix";
        case MaintenanceStatus::Fixed:    return "Fixed";
        default:                          return "NoProblems";
    }
}
static MaintenanceStatus strToMaintStatus(const std::string& s){
    if(s == "NeedsFix") return MaintenanceStatus::NeedsFix;
    if(s == "Fixed")    return MaintenanceStatus::Fixed;
    return MaintenanceStatus::NoProblems;
}

static void saveUsers(const std::map<int, std::unique_ptr<User>>& users){
    json arr = json::array();
    for(auto& kv : users){
        User* u = kv.second.get();
        json obj;
        obj["id"]       = u->getId();
        obj["name"]     = u->getName();
        obj["email"]    = u->getEmail();
        obj["phone"]    = u->getPhone();
        obj["role"]     = roleToStr(u->getRole());
        obj["password"] = u->getPassword();

        Passenger* p = dynamic_cast<Passenger*>(u);
        if(p){
            obj["age"]           = p->getAge();
            obj["nationalId"]    = p->getNationalId();
            obj["loyaltyPoints"] = p->getLoyaltyPoints();
        }
        arr.push_back(obj);
    }
    writeJson(USERS_FILE, arr);
}

static void saveAircrafts(const std::map<int, std::unique_ptr<Aircraft>>& aircrafts){
    json arr = json::array();
    for(auto& kv : aircrafts){
        Aircraft* ac = kv.second.get();
        json obj;
        obj["id"]              = ac->getId();
        obj["name"]            = ac->getName();
        obj["numOfRows"]       = ac->getRows();
        obj["numOfCols"]       = ac->getCols();
        obj["nextFirstSeatId"] = ac->getSeats().empty() ? 1 : ac->getSeats()[0][0]->getId();

        json sav = json::array();
        for(auto& row : ac->getSeats()){
            for(Seat* s : row){
                if(!s->getIsAvailable()){
                    json so;
                    so["id"]          = s->getId();
                    so["isAvailable"] = false;
                    sav.push_back(so);
                }
            }
        }
        obj["seatAvailability"] = sav;
        arr.push_back(obj);
    }
    writeJson(AIRCRAFT_FILE, arr);
}

static void saveCrewMembers(const std::map<int, std::unique_ptr<CrewMember>>& crewMembers){
    json arr = json::array();
    for(auto& kv : crewMembers){
        CrewMember* cm = kv.second.get();
        json obj;
        obj["id"]                 = cm->getId();
        obj["name"]               = cm->getName();
        obj["maxFlightHours"]     = cm->getMaxFlightHours();
        obj["currentFlightHours"] = cm->getCurrentFlightHours();
        obj["crewType"]           = crewTypeToStr(cm->getCrewType());
        Pilot* p = dynamic_cast<Pilot*>(cm);
        if(p){
            obj["licenseNumber"] = p->getLicenseNumber();
            obj["licenseType"]   = p->getLicenseType();
        }
        arr.push_back(obj);
    }
    writeJson(CREW_FILE, arr);
}

static void saveFlights(const std::map<int, std::unique_ptr<Flight>>& flights){
    json arr = json::array();
    for(auto& kv : flights){
        Flight* f = kv.second.get();
        json obj;
        obj["id"]            = f->getId();
        obj["price"]         = f->getPrice();
        obj["origin"]        = f->getOrigin();
        obj["destination"]   = f->getDestination();
        obj["departureTime"] = f->getDepartureTime();
        obj["arrivalTime"]   = f->getArrivalTime();
        obj["flightStatus"]  = flightStatusToStr(f->getFlightStatus());
        obj["aircraftId"]    = f->getAircraft() ? f->getAircraft()->getId() : -1;
        json cmIds = json::array();
        for(CrewMember* cm : f->getCrewMembers()) cmIds.push_back(cm->getId());
        obj["crewMemberIds"] = cmIds;
        json resIds = json::array();
        for(const Reservation* r : f->getReservations()) resIds.push_back(r->getId());
        obj["reservationIds"] = resIds;
        arr.push_back(obj);
    }
    writeJson(FLIGHTS_FILE, arr);
}

static void saveReservations(const std::map<int, std::unique_ptr<Reservation>>& reservations){
    json arr = json::array();
    for(auto& kv : reservations){
        Reservation* r = kv.second.get();
        json obj;
        obj["id"]                = r->getId();
        obj["passengerId"]       = r->getPassengerId();
        obj["flightId"]          = r->getFlight()->getId();
        obj["seatId"]            = r->getSeat()->getId();
        obj["reservationStatus"] = resStatusToStr(r->getReservationStatus());
        obj["createdAt"]         = r->getCreatedAt();
        arr.push_back(obj);
    }
    writeJson(RES_FILE, arr);
}

static void saveMaintenance(const std::map<int, std::unique_ptr<Maintenance>>& maintenance){
    json arr = json::array();
    for(auto& kv : maintenance){
        Maintenance* m = kv.second.get();
        json obj;
        obj["id"]                = m->getId();
        obj["aircraftId"]        = m->getAircraft()->getId();
        obj["date"]              = m->getDate();
        obj["description"]       = m->getDescription();
        obj["maintenanceStatus"] = maintStatusToStr(m->getMaintenanceStatus());
        arr.push_back(obj);
    }
    writeJson(MAINT_FILE, arr);
}

FileManager::FileManager(){
    json idsJson = readJson(IDS_FILE);
    if(idsJson.is_null() || !idsJson.is_object()){
        idsJson = {
            {"User",1},{"Aircraft",1},{"CrewMember",1},
            {"Flight",1},{"Maintenance",1},{"Reservation",1}
        };
        writeJson(IDS_FILE, idsJson);
    }

    json acJson = readJson(AIRCRAFT_FILE);
    if(!acJson.is_null() && acJson.is_array()){
        for(auto& acObj : acJson){
            int id    = acObj["id"];
            std::string nm = acObj["name"];
            int rows  = acObj["numOfRows"];
            int cols  = acObj["numOfCols"];
            int nfsId = acObj["nextFirstSeatId"];

            auto ac = std::make_unique<Aircraft>(id, nm, rows, cols, nfsId);

            for(auto& row : ac->getSeats())
                for(Seat* s : row) s->setIsAvailable(true);

            if(acObj.contains("seatAvailability") && acObj["seatAvailability"].is_array()){
                for(auto& sav : acObj["seatAvailability"]){
                    int  sId   = sav["id"];
                    bool avail = sav["isAvailable"];
                    for(auto& row : ac->getSeats())
                        for(Seat* s : row)
                            if(s->getId() == sId) s->setIsAvailable(avail);
                }
            }

            for(auto& row : ac->getSeats())
                for(Seat* s : row)
                    seats[s->getId()] = s;

            aircrafts[id] = std::move(ac);
        }
    }

    json crewJson = readJson(CREW_FILE);
    if(!crewJson.is_null() && crewJson.is_array()){
        for(auto& cObj : crewJson){
            int id   = cObj["id"];
            std::string nm = cObj["name"];
            int maxFH = cObj["maxFlightHours"];
            int curFH = cObj.contains("currentFlightHours") ? (int)cObj["currentFlightHours"] : 0;
            CrewType ct = strToCrewType(cObj["crewType"]);

            std::unique_ptr<CrewMember> cm;
            if(ct == CrewType::Pilot){
                std::string ln = cObj.contains("licenseNumber") ? (std::string)cObj["licenseNumber"] : "";
                std::string lt = cObj.contains("licenseType")   ? (std::string)cObj["licenseType"]   : "";
                cm = std::make_unique<Pilot>(id, nm, maxFH, ln, lt);
            } else {
                cm = std::make_unique<FlightAttendant>(id, nm, maxFH);
            }
            cm->addFlightHours(curFH);
            crewMembers[id] = std::move(cm);
        }
    }

    json usersJson = readJson(USERS_FILE);
    if(!usersJson.is_null() && usersJson.is_array()){
        for(auto& uObj : usersJson){
            int id   = uObj["id"];
            std::string nm = uObj["name"];
            std::string em = uObj["email"];
            std::string pw = uObj.contains("password") ? (std::string)uObj["password"] : "";
            std::string ph = uObj["phone"];
            Role role = strToRole(uObj["role"]);
            emails[em] = id;

            std::unique_ptr<User> u;
            if(role == Role::Admin){
                u = std::make_unique<Admin>(id, nm, em, pw, ph, this);
            } else if(role == Role::BookingAgent){
                u = std::make_unique<BookingAgent>(id, nm, em, pw, ph, this);
            } else {
                int age        = uObj.contains("age")           ? (int)uObj["age"]               : 0;
                std::string ni = uObj.contains("nationalId")    ? (std::string)uObj["nationalId"] : "";
                int lp         = uObj.contains("loyaltyPoints") ? (int)uObj["loyaltyPoints"]      : 0;
                auto p = std::make_unique<Passenger>(id, nm, em, pw, ph, age, ni);
                p->updateLoyaltyPoints(lp);
                u = std::move(p);
            }
            users[id] = std::move(u);
        }
    }

    json flightsJson = readJson(FLIGHTS_FILE);
    if(!flightsJson.is_null() && flightsJson.is_array()){
        for(auto& fObj : flightsJson){
            int id   = fObj["id"];
            int price = fObj["price"];
            std::string ori  = fObj["origin"];
            std::string dest = fObj["destination"];
            std::string dep  = fObj["departureTime"];
            std::string arr  = fObj["arrivalTime"];
            FlightStatus fs  = strToFlightStatus(fObj["flightStatus"]);
            int acId         = fObj["aircraftId"];

            Aircraft* ac = getAircraftById(acId);
            std::vector<CrewMember*> cms;
            if(fObj.contains("crewMemberIds") && fObj["crewMemberIds"].is_array())
                for(int cmId : fObj["crewMemberIds"]){
                    CrewMember* cm = getCrewMemberById(cmId);
                    if(cm) cms.push_back(cm);
                }

            auto f = std::make_unique<Flight>(id, price, ori, dest, dep, arr, ac, cms);
            f->setFlightStatus(fs);
            flights[id] = std::move(f);
        }
    }

    json resJson = readJson(RES_FILE);
    if(!resJson.is_null() && resJson.is_array()){
        for(auto& rObj : resJson){
            int id  = rObj["id"];
            int pId = rObj["passengerId"];
            int fId = rObj["flightId"];
            int sId = rObj["seatId"];
            ReservationStatus rs = strToResStatus(rObj["reservationStatus"]);

            Passenger* p = dynamic_cast<Passenger*>(getUserById(pId));
            Flight*    f = getFlightById(fId);
            Seat*      s = getSeatById(sId);

            if(p && f && s){
                auto res = std::make_unique<Reservation>(id, p, f, s);
                res->setReservationStatus(rs);
                Reservation* rPtr = res.get();
                reservations[id] = std::move(res);
                p->addReservation(rPtr);
                f->addReservation(rPtr);
                if(rs == ReservationStatus::Accepted)
                    s->setIsAvailable(false);
            }
        }
    }

    json maintJson = readJson(MAINT_FILE);
    if(!maintJson.is_null() && maintJson.is_array()){
        for(auto& mObj : maintJson){
            int id   = mObj["id"];
            int acId = mObj["aircraftId"];
            std::string dt = mObj["date"];
            std::string ds = mObj["description"];
            MaintenanceStatus ms = strToMaintStatus(mObj["maintenanceStatus"]);
            Aircraft* ac = getAircraftById(acId);
            if(ac){
                auto m = std::make_unique<Maintenance>(id, ac, dt, ds, ms);
                Maintenance* mPtr = m.get();
                maintenance[id] = std::move(m);
                ac->addMaintenanceLog(mPtr);
            }
        }
    }
}

User* FileManager::getUserById(int id){
    auto it = users.find(id);
    return it != users.end() ? it->second.get() : nullptr;
}

Aircraft* FileManager::getAircraftById(int id){
    auto it = aircrafts.find(id);
    return it != aircrafts.end() ? it->second.get() : nullptr;
}

CrewMember* FileManager::getCrewMemberById(int id){
    auto it = crewMembers.find(id);
    return it != crewMembers.end() ? it->second.get() : nullptr;
}

Flight* FileManager::getFlightById(int id){
    auto it = flights.find(id);
    return it != flights.end() ? it->second.get() : nullptr;
}

Maintenance* FileManager::getMaintenanceById(int id){
    auto it = maintenance.find(id);
    return it != maintenance.end() ? it->second.get() : nullptr;
}

Seat* FileManager::getSeatById(int id){
    auto it = seats.find(id);
    return it != seats.end() ? it->second : nullptr;
}

std::map<int, User*> FileManager::getAllUsers(){
    std::map<int, User*> result;
    for(const auto& kv : users) result[kv.first] = kv.second.get();
    return result;
}

std::map<int, Aircraft*> FileManager::getAllAircraft(){
    std::map<int, Aircraft*> result;
    for(const auto& kv : aircrafts) result[kv.first] = kv.second.get();
    return result;
}

std::map<int, CrewMember*> FileManager::getAllCrewMembers(){
    std::map<int, CrewMember*> result;
    for(const auto& kv : crewMembers) result[kv.first] = kv.second.get();
    return result;
}

std::map<int, Flight*> FileManager::getAllFlights(){
    std::map<int, Flight*> result;
    for(const auto& kv : flights) result[kv.first] = kv.second.get();
    return result;
}

std::map<int, Maintenance*> FileManager::getAllMaintenance(){
    std::map<int, Maintenance*> result;
    for(const auto& kv : maintenance) result[kv.first] = kv.second.get();
    return result;
}

std::map<int, Reservation*> FileManager::getAllReservations(){
    std::map<int, Reservation*> result;
    for(const auto& kv : reservations) result[kv.first] = kv.second.get();
    return result;
}

std::map<int, Seat*> FileManager::getAllSeats(){
    std::map<int, Seat*> result;
    for(const auto& kv : seats) result[kv.first] = kv.second;
    return result;
}

int FileManager::getNextId(std::string className){
    json idsJson = readJson(IDS_FILE);
    if(idsJson.is_null() || !idsJson.is_object()) idsJson = json::object();
    int nextId = idsJson.contains(className) ? (int)idsJson[className] : 1;
    idsJson[className] = nextId + 1;
    writeJson(IDS_FILE, idsJson);
    return nextId;
}

int FileManager::getIdByEmail(std::string email){
    auto it = emails.find(email);
    return it != emails.end() ? it->second : -1;
}

void FileManager::addCrewMember(std::unique_ptr<CrewMember> cm){
    crewMembers[cm->getId()] = std::move(cm);
    saveCrewMembers(crewMembers);
}

void FileManager::addAircraft(std::unique_ptr<Aircraft> aircraft){
    for(auto& row : aircraft->getSeats())
        for(Seat* s : row)
            seats[s->getId()] = s;
    aircrafts[aircraft->getId()] = std::move(aircraft);
    saveAircrafts(aircrafts);
}

void FileManager::addFlight(std::unique_ptr<Flight> f){
    flights[f->getId()] = std::move(f);
    saveFlights(flights);
}

void FileManager::addUser(std::unique_ptr<User> user){
    emails[user->getEmail()] = user->getId();
    users[user->getId()] = std::move(user);
    saveUsers(users);
}

void FileManager::addReservation(std::unique_ptr<Reservation> res){
    Reservation* rPtr = res.get();
    Seat* s = rPtr->getSeat();
    if(s) s->setIsAvailable(false);
    reservations[rPtr->getId()] = std::move(res);
    saveReservations(reservations);
    saveAircrafts(aircrafts);
}

void FileManager::addMaintenance(std::unique_ptr<Maintenance> m){
    Maintenance* mPtr = m.get();
    Aircraft* ac = mPtr->getAircraft();
    if(ac) ac->addMaintenanceLog(mPtr);
    maintenance[mPtr->getId()] = std::move(m);
    saveMaintenance(maintenance);
}

void FileManager::updateCrewMember(int id, CrewMember& cm){
    auto it = crewMembers.find(id);
    if(it == crewMembers.end()){ std::cerr << "[FM] CrewMember " << id << " not found.\n"; return; }
    it->second->setName(cm.getName());
    it->second->setMaxFlightHours(cm.getMaxFlightHours());
    saveCrewMembers(crewMembers);
}

void FileManager::updateFlight(int id, Flight& f){
    auto it = flights.find(id);
    if(it == flights.end()){ std::cerr << "[FM] Flight " << id << " not found.\n"; return; }
    it->second->setFlightStatus(f.getFlightStatus());
    saveFlights(flights);
}

void FileManager::updateAircraft(int id, Aircraft& ac){
    auto it = aircrafts.find(id);
    if(it == aircrafts.end()){ std::cerr << "[FM] Aircraft " << id << " not found.\n"; return; }
    it->second->setName(ac.getName());
    saveAircrafts(aircrafts);
}

void FileManager::updateUser(int id, User& u){
    auto it = users.find(id);
    if(it == users.end()){ std::cerr << "[FM] User " << id << " not found.\n"; return; }
    it->second->setName(u.getName());
    it->second->setPhone(u.getPhone());
    saveUsers(users);
}

void FileManager::updateReservation(int id, Reservation& res){
    auto it = reservations.find(id);
    if(it == reservations.end()){ std::cerr << "[FM] Reservation " << id << " not found.\n"; return; }
    it->second->setReservationStatus(res.getReservationStatus());
    saveReservations(reservations);
}

void FileManager::deleteCrewMember(int cmId){
    auto it = crewMembers.find(cmId);
    if(it == crewMembers.end()){ std::cerr << "[FM] CrewMember " << cmId << " not found.\n"; return; }
    for(auto& kv : flights) kv.second->removeCrewMember(cmId);
    crewMembers.erase(it);
    saveCrewMembers(crewMembers);
    saveFlights(flights);
}

void FileManager::deleteFlight(int fId){
    auto it = flights.find(fId);
    if(it == flights.end()){ std::cerr << "[FM] Flight " << fId << " not found.\n"; return; }
    for(auto rit = reservations.begin(); rit != reservations.end(); ){
        if(rit->second->getFlight() == it->second.get()){
            Seat* s = rit->second->getSeat();
            if(s) s->setIsAvailable(true);
            rit = reservations.erase(rit);
        } else ++rit;
    }
    flights.erase(it);
    saveFlights(flights);
    saveReservations(reservations);
    saveAircrafts(aircrafts);
}

void FileManager::deleteAircraft(int acId){
    auto it = aircrafts.find(acId);
    if(it == aircrafts.end()){ std::cerr << "[FM] Aircraft " << acId << " not found.\n"; return; }
    for(auto& row : it->second->getSeats())
        for(Seat* s : row)
            seats.erase(s->getId());
    for(auto& kv : flights)
        if(kv.second->getAircraft() == it->second.get())
            kv.second->setAircraft(nullptr);
    for(auto mit = maintenance.begin(); mit != maintenance.end(); ){
        if(mit->second->getAircraft() == it->second.get()) mit = maintenance.erase(mit);
        else ++mit;
    }
    aircrafts.erase(it);
    saveAircrafts(aircrafts);
    saveFlights(flights);
    saveMaintenance(maintenance);
}

void FileManager::deleteUser(int uId){
    auto it = users.find(uId);
    if(it == users.end()){ std::cerr << "[FM] User " << uId << " not found.\n"; return; }
    for(auto rit = reservations.begin(); rit != reservations.end(); ){
        if(rit->second->getPassengerId() == uId){
            Seat* s = rit->second->getSeat();
            if(s) s->setIsAvailable(true);
            rit = reservations.erase(rit);
        } else ++rit;
    }
    emails.erase(it->second->getEmail());
    users.erase(it);
    saveUsers(users);
    saveReservations(reservations);
    saveAircrafts(aircrafts);
}

void FileManager::cancelReservation(int resId){
    auto it = reservations.find(resId);
    if(it == reservations.end()){ std::cerr << "[FM] Reservation " << resId << " not found.\n"; return; }
    Reservation* r = it->second.get();
    r->setReservationStatus(ReservationStatus::Cancelled);
    Seat* s = r->getSeat();
    if(s) s->setIsAvailable(true);
    saveReservations(reservations);
    saveAircrafts(aircrafts);
}
