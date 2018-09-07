// Your code here
#include "Food.h"
Food::Food(){
    name_="gluten";
    quantity_ = 1;
}

std::string Food::get_name(){
    return name_;
}

int Food::get_quantity(){
    return quantity_;
}
void Food::set_name(std::string name){
    name_=name;
}
void Food::set_quantity(int quant){
    quantity_=quant;
}
