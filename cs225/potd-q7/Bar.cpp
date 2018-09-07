#include "Bar.h"
#include "Foo.h"
#include <string>
using namespace potd;
Bar::Bar(std::string name){
    this->f_= new potd::Foo(name);
}
Bar::Bar(const Bar & other){
    this->f_ = new Foo((other.f_)->get_name());
}
Bar::~Bar(){
    delete f_;
}
Bar & Bar::operator=(const Bar & other){
    Bar temp(other.get_name());
    swap(this->f_, temp.f_);
    return *this;

}
std::string Bar::get_name() const {
    return f_->get_name();
}
