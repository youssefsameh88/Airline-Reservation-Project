#include "Ui.h"
#include "FileManager.h"
#include "ReportManager.h"
#include "Admin.h"
#include "BookingAgent.h"
#include "Passenger.h"
#include "Enums.h"
#include <iostream>
#include <memory>

static bool doLogin(FileManager* fm, User*& out){
    std::string email = ui::ask("Email: ");
    std::string pw = ui::ask("Password: ");
    int id = fm->getIdByEmail(email);
    if(id == -1) return false;
    User* u = fm->getUserById(id);
    if(!u) return false;
    if(u->getPassword() != ui::passwordHash(pw)) return false;
    out = u;
    return true;
}

void ui::signUp(FileManager* fm){
    clear();
    banner();
    screenTitle("PASSENGER SIGN UP");

    std::string name = askRequired("Full name: ");
    std::string email = askRequired("Email: ");
    if(fm->getIdByEmail(email) != -1){
        std::cout << "\nThis email already has an account.\n";
        waitEnter();
        return;
    }
    std::string phone = askRequired("Phone: ");
    std::string pw = askRequired("Password: ");
    int age = askInt("Age: ", 1, 120);
    std::string natId = askRequired("National ID: ");

    int newId = fm->getNextId("User");
    fm->addUser(std::make_unique<Passenger>(newId, name, email,
                                            passwordHash(pw), phone, age, natId));
    std::cout << "\nAccount created, you can now log in.\n";
    waitEnter();
}

void ui::showProfile(FileManager* fm, User* me){
    (void)fm;
    clear();
    banner();
    screenTitle("MY PROFILE");
    std::cout << "  Name  : " << me->getName() << "\n";
    std::cout << "  Email : " << me->getEmail() << "\n";
    std::cout << "  Phone : " << me->getPhone() << "\n";
    Passenger* p = dynamic_cast<Passenger*>(me);
    if(p){
        std::cout << "  Age   : " << p->getAge() << "\n";
        std::cout << "  Nat. ID : " << p->getNationalId() << "\n";
        std::cout << "  Loyalty : " << p->getLoyaltyPoints() << " pts\n";
    }
    waitEnter();
}

void ui::session(FileManager* fm, ReportManager* rm){
    for(;;){
        User* me = nullptr;
        for(;;){
            clear();
            banner();
            screenTitle("MAIN MENU");
            std::cout << "  1) Login\n";
            std::cout << "  2) Sign up as passenger\n";
            std::cout << "  0) Exit\n\n";
            int ch = pick(0, 2);
            if(ch == 0){
                clear();
                std::cout << "\nGoodbye.\n\n";
                return;
            }
            if(ch == 2){
                signUp(fm);
                continue;
            }
            clear();
            banner();
            screenTitle("LOGIN");
            if(doLogin(fm, me)) break;
            std::cout << "\nWrong email or password.\n";
            waitEnter();
        }

        switch(me->getRole()){
            case Role::Admin:        adminMain(fm, rm, me); break;
            case Role::BookingAgent: agentMain(fm, rm, me); break;
            default:                 passengerMain(fm, me); break;
        }
    }
}