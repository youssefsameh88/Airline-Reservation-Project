#pragma once
#include <string>
#include <climits>
#include "Enums.h"

class FileManager;
class ReportManager;
class User;
class Flight;

namespace ui {

// terminal helpers
void clear();
void waitEnter();
void banner();
void screenTitle(const std::string& t);
void hr(char c = '-', int w = 66);

std::string ask(const std::string& label);
std::string askRequired(const std::string& label);
int askInt(const std::string& label, int lo = INT_MIN, int hi = INT_MAX);
int pick(int lo, int hi);
bool confirm(const std::string& message);

std::string lower(std::string s);
std::string upper(std::string s);
std::string passwordHash(const std::string& pw);
std::string statusText(FlightStatus s);
std::string seatText(SeatType t);
void seatMap(Flight* f);
void flightList(FileManager* fm, const std::string& dest, int minP, int maxP);

// session
void session(FileManager* fm, ReportManager* rm);
void signUp(FileManager* fm);
void showProfile(FileManager* fm, User* me);

// role menus
void adminMain(FileManager* fm, ReportManager* rm, User* me);
void agentMain(FileManager* fm, ReportManager* rm, User* me);
void passengerMain(FileManager* fm, User* me);

// admin screens
void addCrew(FileManager* fm);
void showCrew(FileManager* fm);
void editCrew(FileManager* fm);
void removeCrew(FileManager* fm);
void addAircraft(FileManager* fm);
void showAircraft(FileManager* fm);
void removeAircraft(FileManager* fm);
void addFlight(FileManager* fm);
void showFlights(FileManager* fm);
void changeFlightStatus(FileManager* fm);
void removeFlight(FileManager* fm);
void addMaintenance(FileManager* fm);

// agent screens
void searchFlights(FileManager* fm);
void makeReservation(FileManager* fm, User* me);
void cancelReservation(FileManager* fm, User* me);
void showAllReservations(FileManager* fm);

// passenger screens
void searchAndBook(FileManager* fm, User* me);
void myReservations(FileManager* fm, User* me);

// reports
void adminReports(FileManager* fm, ReportManager* rm);
void agentReports(FileManager* fm, ReportManager* rm);

}