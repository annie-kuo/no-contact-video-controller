// SPDX-FileCopyrightText: 2021 Kattni Rembor for Adafruit Industries
//
// SPDX-License-Identifier: MIT

#include <Adafruit_NeoPixel.h>
#include "Adafruit_FreeTouch.h"
#include "Adafruit_APDS9960.h"
#include <Keyboard.h>


// Create the neopixel strip with the built in definitions NUM_NEOPIXEL and PIN_NEOPIXEL
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_NEOPIXEL, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
int16_t neo_brightness = 20; // initialize with 20 brightness (out of 255)

// Create the two touch pads on pins 1 and 2:
Adafruit_FreeTouch qt_1 = Adafruit_FreeTouch(1, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);
Adafruit_FreeTouch qt_2 = Adafruit_FreeTouch(2, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);

Adafruit_APDS9960 apds;
int paused=0;
int rewinded=0;
int brightness=0;
uint16_t prox=0;

void setup() {
  Serial.begin(9600);
  //while (!Serial);
  
  strip.begin();
  strip.setBrightness(neo_brightness);
  strip.show(); // Initialize all pixels to 'off'

  if (! qt_1.begin())  
    Serial.println("Failed to begin qt on pin 1");
  if (! qt_2.begin())  
    Serial.println("Failed to begin qt on pin 2");

  pinMode(PIN_INTERRUPT, INPUT_PULLUP);
  if(!apds.begin()){
    Serial.println("failed to initialize device! Please check your wiring.");
    while (1) {
      strip.fill(0xFF0000);
      strip.show();
      delay(100);
      strip.fill(0x00);
      strip.show();
      delay(100);
    }
  }
  
  Serial.println("APDS initialized!");
  apds.enableProximity(true);
  apds.setProxGain(APDS9960_PGAIN_8X);
  apds.setLED(APDS9960_LEDDRIVE_100MA, APDS9960_LEDBOOST_300PCNT);
  apds.setProxPulse(APDS9960_PPULSELEN_16US, 1);

  //set the interrupt threshold to fire when proximity reading goes above 2
  apds.setProximityInterruptThreshold(0, 2);
  apds.enableProximityInterrupt();

  //set keyboard
  Keyboard.begin();
}

uint8_t j=0;

void loop() {

  // print the proximity reading when the interrupt pin goes low
  if (!digitalRead(PIN_INTERRUPT)){
    prox = apds.readProximity();
    Serial.print("Proximity: ");
    Serial.println(prox);


    //set LED brightness
    if (prox <= 50){ // ignore readings up to 10
      brightness=50;
    } else {
      brightness=prox;
    }
    strip.setBrightness(brightness-50);

    //pause video if state has changed
    if(prox<150 && paused==0){
      Keyboard.write('K');
      paused=1;
    }

    //rewind video if close enough
    if(prox>=150 && rewinded==0){
      Keyboard.write('J');
      rewinded=1;
    }
    
    //clear the interrupt
    apds.clearInterrupt();
    
  } else {
    rewinded=0;
    paused=0;
    strip.setBrightness(0);
  }

  //set LED colors
  if(prox < 150) {
   strip.fill(0x5466ff);
  } else {
   strip.fill(0x54ffa2);
  }
 
  strip.show();

  // measure the captouches
  uint16_t touch1 = qt_1.measure();
  uint16_t touch2 = qt_2.measure();

  // If the first pad is touched, go forward
  if (touch1 > 500) {
    Serial.println("QT 1 touched");
    j++;
  }

  // If the second pad is touched, go backward
  if (touch2 > 500) {
    Serial.println("QT 2 touched");
    j--;
  }

  delay(10);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
