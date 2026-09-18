#pragma once
#include <string>
#include "Enums.h"
class Seat{
    private:
    int id;
    std::string name;
    SeatType seatType;
    bool isAvailable;

    public:
    Seat(int id, std::string name, SeatType st);
    int getId();
    std::string getName();
    SeatType getType();
    void setIsAvailable(bool av);
    bool getIsAvailable();
};