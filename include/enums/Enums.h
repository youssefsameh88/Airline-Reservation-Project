#pragma once
enum class Role{
    Admin, BookingAgent, Passenger
};

enum class CrewType{
    Pilot, FlightAttendant
};

enum class SeatType{
    Economy, Business, FirstClass
};

enum class FlightStatus {
   Scheduled, Boarding, Departed, Arrived, Cancelled
};

enum class ReservationStatus{
    Accepted, Cancelled
};

enum class MaintenanceStatus{
    NoProblems, NeedsFix, Fixed 
};