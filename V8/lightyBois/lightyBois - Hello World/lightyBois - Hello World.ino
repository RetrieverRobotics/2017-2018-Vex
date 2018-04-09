#include <FastLED.h>

#define NUM_LEDS 60

CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, 6>(leds, NUM_LEDS);
}

void loop() {
  
  // put your main code here, to run repeatedly:
  for(int l = 0; l < 255; l+=25){
  for(int k = 0; k < 255; k+=25){
  for(int j = 0; j < 255; j+=25){
    for(int i = 0; i < NUM_LEDS; i++){
      leds[i] = CRGB(j,l,k);
//      delayMicroseconds(1);
      
    }
    FastLED.show();
//    delayMicroseconds(5);
    
  }
  }
  }
  
   
  
//  delay(500);
//  for(int i = 0; i < NUM_LEDS; i++){
//    leds[i] = CRGB::Black;
//  }
//  FastLED.show();
//  delay(500);


// void loop() {
      // Turn the first led red for 1 second
//      leds[0] = CRGB::Red; 
//      FastLED.show();
//      delay(1000);
//      
//      // Set the first led back to black for 1 second
//      leds[0] = CRGB::Black;
//      FastLED.show();
//      delay(1000);
//    }
  
// void loop() {
//        for(int dot = 0; dot < NUM_LEDS; dot++) { 
//            leds[dot] = CRGB::Yellow;
//            FastLED.show();
//            // clear this led for the next time around the loop
//            leds[dot] = CRGB::Black;
//            delay(30);
//        }
//    }


}
