#include <Arduino.h>
#include <Wire.h>        // Instantiate the Wire library
#include <TFLI2C.h>      // TFLuna-I2C Library v.0.1.1
#include <FastLED.h>

#define NUM_LEDS 60
#define LED_PIN 3

TFLI2C tflI2C; // Object for lidar sensor

int16_t  tfDist;    // distance in centimeters
int16_t  tfAddr = TFL_DEF_ADR;  // Use this default I2C address

// LED Essentials
CRGB leds[NUM_LEDS];
const int ledMaxDistance = 200; 
const int ledMinDistance = 30; 
const int flashDelay = 200; // Delay between red blinking
const int numFlashes = 75; // Number of flashes before LED strip turns off

// Returns Distance from Sensor in CM
int16_t getDistance() {
  if(tflI2C.getData(tfDist, tfAddr)) {
    return tfDist;
  }
}

// Setup Sensor And LED
void setup(){
  // Serial.begin(9600); DEBUGGING
  Wire.begin();           // Initalize Wire library
  
  // Add FASTLED and Set all Lights to blank
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  fill_solid(leds, NUM_LEDS, CRGB::Black); 
  FastLED.show();
}

void loop(){
  // Variables used for calculations
  int16_t distance;
  int numLeds;
  int loopCounter = 0;

  distance = getDistance();
  // Serial.println(String(distance) + " cm"); DEBUGGING

  if (distance >= ledMaxDistance ){ // If the distance reading is out of max led range... turn off all lights 
    fill_solid(leds, NUM_LEDS, CRGB::Black); 
  } else if (distance > ledMinDistance && distance < ledMaxDistance){ // Distance is inbetween min and max.. calculate numLeds and light up based on distance
    fill_solid(leds, NUM_LEDS, CRGB::Black); 
  
    numLeds = distance * NUM_LEDS / ledMaxDistance;

    if (distance >= ledMaxDistance / 2){ // half is green, half is orange to indicate getting closer
      fill_solid(leds, numLeds, CRGB::Green);
    } else {
      fill_solid(leds, numLeds, CRGB::Orange);
    }
  } else if (distance <= ledMinDistance){ // Distance is below max... flash red light for a little... then turn off
    while (distance < ledMinDistance){
       if (loopCounter < numFlashes) { 
        fill_solid(leds, NUM_LEDS, CRGB::Red);
        FastLED.show();
        delay(flashDelay);
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show();
        delay(flashDelay);

        loopCounter++;
      } else {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show();
        delay(10000); // Delay to Check Only every 10 Seconds 
      }

      distance = getDistance(); // Update distance sensor during while loop
    }
  }

  FastLED.show();
  delay(100); // reduces jiterry LEDS while remaining at a stable distance
}