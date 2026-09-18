#pragma once
#include <vector>
#include "User.h"


class FileManager;
class ReportManager;
class Reservation;
class Passenger;

class BookingAgent : public User{
    private:
    FileManager* fm;
    public:
    BookingAgent(int id, std::string name, std::string email, std::string password, std::string phone, FileManager* fm);
    void addReservation(int pId, int flightId, int seatId);
    void updateReservation(int resId, Reservation& res);
    void cancelReservation(int resId);



};

