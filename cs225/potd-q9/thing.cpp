// Your code here!
#include "thing.h"
using namespace potd;
using namespace std;
Thing::Thing(int size){
props_ct_=0;
props_max_=size;
properties_= new string [size];
values_ = new string[size];
}
Thing::Thing(const Thing & other){
props_ct_=other.props_ct_;
props_max_=other.props_ct_;
properties_= new string[props_max_];
values_ = new string[props_max_];
for(int i=0;i<props_max_;i++){
    properties_[i] = other.properties_[i];
    values_[i] = other.values_[i];
}
}
Thing & Thing::operator=(Thing const & other){
    this->props_ct_ = other.props_ct_;
    this->props_max_= other.props_max_;
    for(int i = 0; i < props_max_;i++){
        this->properties_[i] = other.properties_[i];
        this->values_[i] = other.values_[i];
    }
    return *this;
}
Thing::~Thing(){
    delete[] properties_;
    delete[] values_;
}
string Thing::get_property(std::string name){
    for(int i = 0; i < props_ct_; i++){
        if(properties_[i] == name)
            return values_[i];
    }
    return "";
}
int Thing::set_property(std::string name, std::string value){
    for(int i=0;i<props_ct_;i++){
        if(properties_[i]==value){
            values_[i]=value;
            return i;
        }
    }
    if(props_ct_ == props_max_)
        return -1;
    properties_[props_ct_]=name;
    values_[props_ct_]=value;
    props_ct_++;
    return props_ct_ -1;
}
