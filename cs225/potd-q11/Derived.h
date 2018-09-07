#ifndef DERIVED_H_
#define DERIVED_H_
#include "Base.h"
#include<string>
class Derived: public Base{
public:
    std::string foo();
    std::string bar();
    ~Derived();
};
#endif
