// your code here
#ifndef BAR_H_
#define BAR_H_
#include "Foo.h"
#include <string>
namespace potd{
class Bar{
public:
    Bar(std::string name);
    Bar(const Bar & other);
    ~Bar();
    Bar & operator=(const Bar & other);
    string get_name() const;
private:
    potd::Foo* f_;
};
}
#endif
