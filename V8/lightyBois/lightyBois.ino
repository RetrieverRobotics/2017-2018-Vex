#include <FastLED.h>

#define NUM_LEDS 60
#define BAUD_RATE 115200
#define DATA_PIN 6
#define NUM_OPTIONS 3

CRGB leds[NUM_LEDS];

int selectionByte = 0;
int optionsByte[NUM_OPTIONS] = {0, 0, 0};
char serialIn;
const bool lightsOn = true;
uint8_t max_bright = 255;
uint8_t startingHue = 0;
uint8_t hueStep = 5;

void setup() {
  delay(1000);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  set_max_power_in_volts_and_milliamps(5, 500);//10000);//100);//500
  FastLED.setBrightness(max_bright);

  Serial.begin(BAUD_RATE);

  //RGB gradient
  //  DEFINE_GRADIENT_PALLETE(heatmap_gp){
  //    0,    255,  0,    0,
  //    128,  0,    255,  0,
  //    255,  0,    0,    255
  //  };

  setAllLeds(CRGB::Yellow);
  delay(1000);
  setAllLeds(CRGB::Black);
}

void loop() {
  //  if (lightsOn) {
  //    switch (selectionByte) {
  //      default:
  //      case 0:
  //        ledsOff();
  //        break;
  //
  //      case 1:
  //        setAllLeds(CRGB(optionsByte[0], optionsByte[1], optionsByte[2]));
  //        break;
  //
  //      //      case 1:
  //      //        rave();
  //      //        break;
  //
  //      case 2:
  //        setAllLeds(CRGB::Red);
  //        //        spinnyBoi();
  //        break;
  //
  //      case 3:
  //        //        rave();
  //        break;
  //    }
  //  }

  //  setAllLeds(CRGB(optionsByte[0], optionsByte[1], optionsByte[2]));

  if (lightsOn) {
    for (int i = 0; i < NUM_LEDS; i++) {
      uint8_t curLedHue = startingHue + hueStep * i;
      while (curLedHue > 255){
        curLedHue -= 255;
      }
      leds[i] = CHSV(curLedHue, 255, 255);
    }
    startingHue += hueStep;
    if(startingHue > 255){
      startingHue = 0;
    }
  }
  else {
    setAllLeds(CRGB::Black);
  }
  show_at_max_brightness_for_power();

  delay(10);
}



void readUART() {
  //
  //  if (Serial.available() > 0) {
  //    selectionByte = Serial.read();
  ////    Serial.print("sel: ");
  //    Serial.write(selectionByte);
  //
  ////    Serial.println("other: ");
  //    for (int i = 0; i < NUM_OPTIONS; i++) {
  //      while (Serial.available() == 0) {delay(1);} //wait for next byte
  ////      delay(10);
  //      optionsByte[i] = Serial.read();
  //      Serial.write(optionsByte[i]);
  //    }
  //
  //    while (Serial.available() > 0) {
  //      Serial.read();
  //    }
  //
  //  }
  //


  if (Serial.available() > 0) {
    selectionByte = Serial.read();
    Serial.println("new");
    Serial.println(selectionByte);

    for (int i = 0; i < NUM_OPTIONS; i++) {
      optionsByte[i] = Serial.read();
      Serial.println(optionsByte[i]);
    }
    while (Serial.available() > 0) {
      Serial.read();
    }
  }
}

void ledsOff() {
  setAllLeds(CRGB::Black);
  show_at_max_brightness_for_power();
}

void setAllLeds(CRGB color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
  show_at_max_brightness_for_power();
}

void displayRainbow() {
  for (int i = 0; i < NUM_LEDS; i++) {

  }
  show_at_max_brightness_for_power();
}

void rave() {
  for (int l = 0; l < 255; l += 25) {
    for (int k = 0; k < 255; k += 25) {
      for (int j = 0; j < 255; j += 25) {
        for (int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CRGB(j, l, k);
          //      delayMicroseconds(1);

        }
        show_at_max_brightness_for_power();
        //    delayMicroseconds(5);

      }
    }
  }
}

void spinnyBoi() {
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = CRGB::Yellow;
    show_at_max_brightness_for_power();
    // clear this led for the next time around the loop
    leds[dot] = CRGB::Black;
    delay(10);
  }
}

