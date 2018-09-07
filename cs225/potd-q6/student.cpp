// Your code here! :)
#include "student.h"
#include <cstring>
#include <iostream>
using namespace potd;
Student::Student(){
    name_="Sahil";
    grade_=12;
}
std::string Student::get_name(){
    return name_;
}
double Student::get_grade(){
    return grade_;
}
void Student::set_name(std::string name){
    name_=name;
}
void Student::set_grade(double grade){
    grade_=grade;
}
