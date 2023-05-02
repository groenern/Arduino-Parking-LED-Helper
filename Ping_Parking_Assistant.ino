#include <FastLED.h>

#define NUM_LEDS 60
#define LED_PIN 3 
#define PING_PIN 7

CRGB leds[NUM_LEDS];

const double ledMinDistance = 10; // distances are in cm
const double ledMaxDistance = 100; 
const int flashDelay = 100; // delay in ms

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  fill_solid(leds, NUM_LEDS, CRGB::Black); // Turn off all LEDs
  FastLED.show(); // Show the updated LED states
}

void loop() {
  long duration, inches, cm;
  int numLeds = 0;

  pinMode(PING_PIN, OUTPUT);
  digitalWrite(PING_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(PING_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(PING_PIN, LOW);

  pinMode(PING_PIN, INPUT);
  duration = pulseIn(PING_PIN, HIGH);

  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  /* DEBUGGING
  Serial.print("Distance: ");
  Serial.print(cm);
  Serial.println(" cm");
  */

  if (cm >= ledMaxDistance) {
    fill_solid(leds, NUM_LEDS, CRGB::Black); // out of range... nothing lit up 
  } else if (cm <= ledMinDistance) { // flash red on and off 
    fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.show();
    delay(flashDelay);
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    delay(flashDelay);
  } else if (cm > ledMinDistance && cm < ledMaxDistance){ // calculate numLeds and light up based on distance
    fill_solid(leds, NUM_LEDS, CRGB::Black); 
  
    numLeds = cm * NUM_LEDS / ledMaxDistance;

    if (cm >= ledMaxDistance/2){ // half is green, half is orange to indicate getting closer
      fill_solid(leds, numLeds, CRGB::Green);
    } else {
      fill_solid(leds, numLeds, CRGB::Orange);
    }
  }

  FastLED.show(); // Show the updated LED states
  delay(100);
}

long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}