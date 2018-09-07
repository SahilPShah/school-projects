// Your code here! :)
#ifndef STUDENT_H_
#define STUDENT_H_
#include <iostream>
namespace potd{
    class Student{
    private:
        std::string name_;
        double grade_;
    public:
        Student();
        std::string get_name();
        double get_grade();
        void set_name(std::string name);
        void set_grade(double grade);
    };
}
#endif
