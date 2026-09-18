#include "Ui.h"
#include "FileManager.h"
#include "ReportManager.h"
#include <iostream>

void ui::adminReports(FileManager* fm, ReportManager* rm){
    (void)fm;
    for(;;){
        clear();
        banner();
        screenTitle("ADMIN REPORTS");
        std::cout << "  1) Flight performance\n";
        std::cout << "  2) Revenue\n";
        std::cout << "  3) Maintenance (all)\n";
        std::cout << "  4) Maintenance (one aircraft)\n";
        std::cout << "  5) Passenger activity\n";
        std::cout << "  6) All passengers\n";
        std::cout << "  7) Crew flight hours\n";
        std::cout << "  0) Back\n\n";
        int ch = pick(0, 7);
        if(ch == 0) return;
        clear();
        switch(ch){
            case 1: rm->generateAllFlightsPerformanceReport(); break;
            case 2: rm->generateRevenueReport(); break;
            case 3: rm->generateMaintenanceReport(); break;
            case 4: rm->generateAircraftMaintenanceReport(askInt("Aircraft ID: ", 1, INT_MAX)); break;
            case 5: rm->generatePassengerActivityReport(askInt("Passenger ID: ", 1, INT_MAX)); break;
            case 6: rm->generateAllPassengersActivityReport(); break;
            case 7: rm->generateCrewFlighHoursReport(); break;
        }
        waitEnter();
    }
}

void ui::agentReports(FileManager* fm, ReportManager* rm){
    (void)fm;
    for(;;){
        clear();
        banner();
        screenTitle("AGENT REPORTS");
        std::cout << "  1) Revenue\n";
        std::cout << "  2) Flight performance\n";
        std::cout << "  3) Passenger activity\n";
        std::cout << "  4) All passengers\n";
        std::cout << "  0) Back\n\n";
        int ch = pick(0, 4);
        if(ch == 0) return;
        clear();
        switch(ch){
            case 1: rm->generateRevenueReport(); break;
            case 2: rm->generateAllFlightsPerformanceReport(); break;
            case 3: rm->generatePassengerActivityReport(askInt("Passenger ID: ", 1, INT_MAX)); break;
            case 4: rm->generateAllPassengersActivityReport(); break;
        }
        waitEnter();
    }
}