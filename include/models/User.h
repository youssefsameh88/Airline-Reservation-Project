#pragma once
#include <string>
#include "Enums.h"

class User{
  private:
    int id;
    Role role;
    std::string name, email, password, phone;

  public:
    User();
    User(int id, std::string name, std::string email, std::string password, std::string phone, Role role);
    void setName(std::string n);
    void setEmail(std::string e);
    void setPassword(std::string pw);
    void setPhone(std::string ph);
    int getId();
    Role getRole();
    std::string getName();
    std::string getEmail();
    std::string getPhone();
    std::string getPassword();
    virtual ~User();
};
