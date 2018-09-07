// Animal.h
#ifndef ANIMAL_H_
#define ANIMAL_H_
#include <string>
class Animal{
private:
    std::string name_;


public:
    std::string food_;
    Animal();
    Animal(std::string name, std::string food);
    std::string getType();
    void setType(std::string name);
    std::string getFood();
    void setFood(std::string food);
    virtual std::string print();
};
#endif
