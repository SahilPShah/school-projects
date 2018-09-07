// Pet.h
#ifndef PET_H_
#define PET_H_
#include <string>
#include "Animal.h"
using namespace  std;
class Pet:public Animal{
private:
    string name_;
    string owner_name_;
public:
    Pet();
    Pet(string type, string food, string name, string owner_name);
    string getName();
    string getOwnerName();
    void setName(string nu_name);
    void setOwnerName(string nu_owner);
    string print();
};
#endif
