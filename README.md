# Airline Reservation System

A console-based C++ airline reservation system. Admin, booking agents and
passengers log in with their accounts and manage (or use) flights, seats,
crew and reservations. Data is persisted as JSON files in the `data/` folder.

## Features

- Login / sign up, passwords stored as DJB2 hashes
- Roles: Admin, Booking Agent, Passenger
- Crew management (pilots have licenses, flight hours are tracked)
- Aircraft with a repeating seat layout (first class / business / economy)
- Flight management, seat maps with live availability
- Reservations with automatic seat locking and cancellation
- Passenger loyalty points
- Seven report screens (performance, revenue, maintenance, activity, flight hours)

## Building

Requires CMake and a C++17 compiler (MinGW or MSVC).

```
cmake -G "MinGW Makefiles" -S . -B build
cmake --build build
```

The executable is written next to a copy of the `data/` folder, so it can be
launched directly from `build/`:

```
build/AirlineReservation.exe
```

Seed accounts:

- admin: `admin@airline.com` / `admin123`
- agent: `agent@airline.com` / `agent123`
- passenger: `ahmed.hassan0@example.com` / `pass123`

## Layout

```
src/main.cpp              entry point
src/models/               domain classes
src/system/               file persistence + console UI
src/services/             report generation
include/                  headers
data/                     JSON data files used at runtime
```