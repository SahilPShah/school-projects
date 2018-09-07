#include "q5.h"
void increase_quantity(Food* food){
    int new_quant = food->get_quantity() + 1;
    food->set_quantity(new_quant);
}
