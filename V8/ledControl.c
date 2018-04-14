// control functions for leds.
#pragma systemFile // get rid of unused function warnings

void ledsOff(){
  sendChar(UART1, 0);
  sendChar(UART1, 0);
  sendChar(UART1, 0);
  sendChar(UART1, 0);
}

void setAllLeds(int R, int G, int B){
  sendChar(UART1, 1);
  sendChar(UART1, R);
  sendChar(UART1, G);
  sendChar(UART1, B);
}

void rainbowSpin(){
  sendChar(UART1, 2);
  sendChar(UART1, 0);
  sendChar(UART1, 0);
  sendChar(UART1, 0);
}

void ledsRave(){
  sendChar(UART1, 3);
  sendChar(UART1, 0);
  sendChar(UART1, 0);
  sendChar(UART1, 0);
}

void spinnyBoi(int R, int G, int B){
  sendChar(UART1, 4);
  sendChar(UART1, R);
  sendChar(UART1, G);
  sendChar(UART1, B);
}
