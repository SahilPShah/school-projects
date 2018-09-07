// yourcode here
#include "Food.h"
#include "q5.h"
using namespace std;
int main(){
    Food *myFood = new Food();
    myFood->set_name("My Food");
    cout<<myFood->get_name()<<" has quantity "<<myFood->get_quantity()<<"."<<endl;
    increase_quantity(myFood);
    cout<<myFood->get_name()<<" has quantity "<<myFood->get_quantity()<<"."<<endl;
    delete myFood;    
    return 0;
}
