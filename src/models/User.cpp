#include <string>
#include "User.h"

User::User(){
    id = 0;
    role = Role::Passenger;
}

User::User(int id, std::string name, std::string email, std::string password,
     std::string phone, Role role){
    this->id = id;
    this->name = name;
    this->email = email;
    this->password = password;
    this->phone = phone;
    this->role = role;
}

void User::setName(std::string n){name = n;}
void User::setEmail(std::string e){email = e;}
void User::setPassword(std::string pw){password = pw;}
void User::setPhone(std::string ph){phone = ph;}
int User::getId(){return id;}
Role User::getRole(){return role;}
std::string User::getName(){return name;}
std::string User::getEmail(){return email;}
std::string User::getPhone(){return phone;}
std::string User::getPassword(){return password;}
User::~User(){}
