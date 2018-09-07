// Your code here! :)
#include "student.h"
#include "q6.h"
#include <iostream>
int main(){
    potd::Student stud;
    std::cout<<stud.get_name()<<" is in "<<stud.get_grade()<<std::endl;
    graduate(stud);
    std::cout<<stud.get_name()<<" is in "<<stud.get_grade()<<std::endl;
    return 0;
}
