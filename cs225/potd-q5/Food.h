// Your code here
#ifndef FOOD_H_
#define FOOD_H_
#include <cstring>
#include <iostream>
class Food{
private:
    std::string name_;
    int quantity_;
public:
    Food();
    std::string get_name();
    int get_quantity();
    void set_name(std::string name);
    void set_quantity(int quant);

};
#endif
