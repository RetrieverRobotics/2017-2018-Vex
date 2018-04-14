#include <FastLED.h>

#define NUM_LEDS 60
#define BAUD_RATE 115200
#define DATA_PIN 6
#define NUM_OPTIONS 3

CRGB leds[NUM_LEDS];

uint8_t selectionByte = 2;
uint8_t optionsByte[NUM_OPTIONS] = {0};
const bool lightsOn = true;
uint8_t max_bright = 255;
// for display rainbow
const uint8_t hueStep = 5;

void setup() {
  delay(1000);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  set_max_power_in_volts_and_milliamps(5, 500);//10000);//100);//500
  FastLED.setBrightness(max_bright);

  Serial.begin(BAUD_RATE);

  setAllLeds(CRGB::Yellow);
  delay(1000);
  setAllLeds(CRGB::Black);
}

void loop() {
  //  readUART();
  //Serial.println(72);

  if (Serial.available() > 0) {
    Serial.write(Serial.read());
  }

  //delay(500);

  if (lightsOn) {
    switch (selectionByte) {
      default:
      case 0:
        ledsOff();
        break;

      case 1:
        setAllLeds(CRGB(optionsByte[0], optionsByte[1], optionsByte[2]));
        break;

      case 2:
        displayRainbow();
        break;

      case 3:
        rave();
        break;

      case 4:
        spinnyBoi(CRGB(optionsByte[0], optionsByte[1], optionsByte[2]));
        break;
    }
  }
  else {
    setAllLeds(CRGB::Black);
  }

  delay(10);
}

//  if (lightsOn) {
//    for (int i = 0; i < NUM_LEDS; i++) {
//      uint8_t curLedHue = startingHue + hueStep * i;
//      while (curLedHue > 255){
//        curLedHue -= 255;
//      }
//      leds[i] = CHSV(curLedHue, 255, 255);
//    }
//    startingHue += hueStep;
//    if(startingHue > 255){
//      startingHue = 0;
//    }
//  }
//  else {
//    setAllLeds(CRGB::Black);
//  }
//  show_at_max_brightness_for_power();


void readUART() {
  if (Serial.available() > 0) {
    selectionByte = Serial.read();
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
  uint8_t startingHue = 0;

  while (Serial.available() == 0) {
    for (int i = 0; i < NUM_LEDS; i++) {
      uint8_t curLedHue = startingHue + hueStep * i;
      while (curLedHue > 255) {
        curLedHue -= 255;
      }
      leds[i] = CHSV(curLedHue, 255, 255);
    }
    startingHue += hueStep;
    if (startingHue > 255) {
      startingHue = 0;
    }

    show_at_max_brightness_for_power();
    delay(10);
  }
}

void rave() {
  for (int l = 0; l < 255; l += 25) {
    for (int k = 0; k < 255; k += 25) {
      for (int j = 0; j < 255; j += 25) {
        //        for (int i = 0; i < NUM_LEDS; i++) {
        //          leds[i] = CRGB(j, l, k);
        //        }
        setAllLeds(CRGB(j, l, k));
        if (Serial.available() > 0) {
          return;
        }
        show_at_max_brightness_for_power();
        //    delayMicroseconds(5);

      }
    }
  }
}

void spinnyBoi(CRGB color) {
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = color;
    show_at_max_brightness_for_power();
    if (Serial.available() > 0) {
      return;
    }
    // clear this led for the next time around the loop
    leds[dot] = CRGB::Black;
    delay(10);
  }
}

