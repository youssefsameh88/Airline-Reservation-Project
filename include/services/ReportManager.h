#pragma once
#include<vector>

class FileManager;
class Passenger;
class Reservation;
class Maintenance;
class Flight;
class CrewMember;

class ReportManager{
    private:
    FileManager* fm;

    public:
    ReportManager(FileManager* fm);

    void generateAllFlightsPerformanceReport();
    void generateRevenueReport();
    void generateMaintenanceReport();
    void generateAircraftMaintenanceReport(int acId);
    void generatePassengerActivityReport(int pId);
    void generateAllPassengersActivityReport();
    void generateCrewFlighHoursReport();

};