int main(){
  volatile unsigned *led = (unsigned int*)0x20;
  volatile unsigned *key2 = (unsigned int*)0x60;
  volatile unsigned *key3 = (unsigned int*)0x50;
  volatile unsigned *sw = (unsigned int*)0x10;

  unsigned accumulator = 0;
  //initially clear the LEDs
  *led = 0;
  //infinite loop
  while(1){
    //RESET -- if pressed, change the value of the accumulator to 0
    if(!(*key2))
      accumulator = 0;
    //ACCUMULATE -- ifs pressed, add the value of the switches into the value held in the green LEDs.
    //it should overflow at 256
    if(!(*key3)){
      accumulator += *sw;
      accumulator%=256;//ensure overflow at 256
      *led = accumulator;//set the value of the LEDs
    }
    while(!(*key3)){
      //while the key is still pressed
    }
  }
  return 1;//should never get here
}
