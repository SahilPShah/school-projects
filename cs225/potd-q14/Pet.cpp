// Pet.cpp
#include "Pet.h"
#include "Animal.h"
#include <string>
using namespace std;
Pet::Pet(){
    setType("cat");
    food_="fish";
    name_="Fluffy";
    owner_name_ = "Cinda";
}

Pet::Pet(string type, string food, string name, string owner_name){
    setType(type);
    food_=food;
    name_=name;
    owner_name_=owner_name;
}

string Pet::getName(){
    return name_;
}
string Pet::getOwnerName(){
    return owner_name_;
}
void Pet::setName(string nu_name){
    name_=nu_name;
}
void Pet::setOwnerName(string nu_owner){
    owner_name_ = nu_owner;
}
string Pet::print(){
    return "My name is "+name_;
}
