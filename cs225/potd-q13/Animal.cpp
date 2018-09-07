#include "Animal.h"
#include <string>

Animal::Animal(){
    setType("cat");
    setFood("fish");
}
Animal::Animal(std::string name, std::string food){
    setType(name);
    setFood(food);
}

std::string Animal::getType(){
    return name_;
}

void Animal::setType(std::string name){
    name_=name;
}
std::string Animal::print(){
    return "I am a "+getType()+".";
}
std::string Animal::getFood(){
    return food_;
}

void Animal::setFood(std::string food){
    food_=food;
}
