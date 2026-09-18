#include "Aircraft.h"
#include "Seat.h"
#include <cctype>

Aircraft::Aircraft(int id, std::string name, int numOfRows, int numOfCols, int nextFirstSeatId){
    this->id = id;
    this->name = name;
    this->numOfRows = numOfRows;
    this->numOfCols = numOfCols;
    int sId = nextFirstSeatId;
    seats.resize(numOfRows);
    for(int i = 1; i <= numOfRows; i++){
        for(int j = 0; j < numOfCols; j++){
            SeatType sType = i <= 5 ? SeatType::FirstClass : i <= 15 ?
                        SeatType::Business : SeatType::Economy;
            std::string sName = (char)(j+'A') + std::to_string(i);
            if(sName.size() == 2) sName.insert(1, "0");
            seats[i-1].push_back(new Seat(sId++, sName, sType));
        }
    }
}

Aircraft::~Aircraft(){
    for(int i = 0; i < numOfRows; i++){
        for(int j = 0; j < numOfCols; j++){
            delete seats[i][j];
        }
    }
}

int Aircraft::getId(){return id;}
int Aircraft::getRows(){return numOfRows;}
int Aircraft::getCols(){return numOfCols;}
int Aircraft::getNumOfSeats(){return numOfRows * numOfCols;}
std::string Aircraft::getName(){return name;}
void Aircraft::setName(std::string n){name = n;}
const std::vector<std::vector<Seat*>>& Aircraft::getSeats(){return seats;}
const std::vector<Maintenance*>& Aircraft::getMaintenanceLog(){return maintenanceLog;}
void Aircraft::addMaintenanceLog(Maintenance* m){
    maintenanceLog.push_back(m);
}

Seat* Aircraft::getSeat(std::string name){
    if(name.empty()) return nullptr;
    int col = std::toupper((unsigned char)name[0]) - 'A';
    if(col < 0 || col >= numOfCols) return nullptr;
    int row = 0;
    for(size_t i = 1; i < name.size(); i++){
        if(!std::isdigit((unsigned char)name[i])) return nullptr;
        row = row * 10 + (name[i] - '0');
    }
    if(row < 1 || row > numOfRows) return nullptr;
    return seats[row-1][col];
}
