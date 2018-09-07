/* Your code here! */
#include "epoch.h"

int hours(time_t time_since){
    return time_since/3600; //3600 seconds in an hour
}
int days(time_t time_since){
    return time_since/86400; //86400 seconds in a day
}
int years(time_t time_since){
    return time_since/31536000; //31536000 seconds in a year
}
