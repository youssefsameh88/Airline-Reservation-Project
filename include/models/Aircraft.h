#pragma once
#include <string>
#include <vector>

class Maintenance;
class Seat;

class Aircraft{
    private:
    int id, numOfRows, numOfCols;
    std::string name;
    std::vector<std::vector<Seat*>> seats;
    std::vector<Maintenance*> maintenanceLog;
    public:
    Aircraft(int id, std::string name, int numOfRows, int numOfCols, int nextFirstSeatId);
    ~Aircraft();
    int getId();
    int getRows();
    int getCols();
    int getNumOfSeats();
    std::string getName();
    void setName(std::string name);
    const std::vector<std::vector<Seat*>>& getSeats();
    const std::vector<Maintenance*>& getMaintenanceLog();
    void addMaintenanceLog(Maintenance* m);
    Seat* getSeat(std::string name);
};
