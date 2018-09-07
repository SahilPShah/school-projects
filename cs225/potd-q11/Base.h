#ifndef BASE_H_
#define BASE_H_
#include <string>

class Base{
public:
    std::string foo();
    virtual std::string bar();
    virtual ~Base();
};
#endif
