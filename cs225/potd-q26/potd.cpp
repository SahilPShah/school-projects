// Your code here
#include "potd.h"
#include <cstring>
#include <cmath>

using namespace std;

string getFortune(const string & s){
  switch(s.size() % 5){
    case 1: return "As you wish!";
    case 2: return "Nec spe nec metu!";
    case 3: return "Do or do not, there is no try";
    case 4: return "YEP";
    default: return "Amor Fati!";
  }
}
