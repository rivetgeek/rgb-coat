#include<FastLED.h>
#define NUM_LEDS 96
//#define NUM_LEDS 3
#define DATA_PIN 3
#define CLOCK_PIN 4
#define AVAILABLE_LED 999
#define NUM_ACTIVE 20 //This changes how many pixels are on at any given time

CRGBArray<NUM_LEDS> leds;

//array of currently active LEDS
int RunningPins[NUM_ACTIVE];

//array of last color index for active LEDS
int LastColorIndex[NUM_ACTIVE];

int unsigned long lastPinTime;

//Color palette that will be sewpt through with cos function.
CRGBPalette16 currentPalette = RainbowColors_p;
TBlendType    currentBlending = LINEARBLEND;

void setup() {

  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);
  SetupPalette();
}

void loop() {

  //Loop through all the pixels
  for (int i = 0; i < NUM_ACTIVE; i++) {

    //Is it time to turn on a new pixel?
    if ( millis() - lastPinTime > 1000) {
      //make sure the new pixel isnt overwriting a running pixel.
      if (RunningPins[i] == AVAILABLE_LED) {
        //add a random pixel to the Running Pins array.
        RunningPins[i] = random(0, NUM_LEDS);
        //reset the counter
        lastPinTime = millis();
      }
    }

    //if cycle is complete, reset the color index and blank the pin
    if (LastColorIndex[i] > 255) {
      leds[RunningPins[i]] = CRGB::Black;
      //mark the current pixel as inactive and reset its index
      RunningPins[i] = AVAILABLE_LED;
      LastColorIndex[i] = 0;
    }

    if (RunningPins[i] != AVAILABLE_LED) {
      //Change the pixels to the respective color index based on their place in the cycle.
      leds[RunningPins[i]] = ColorFromPalette(currentPalette, LastColorIndex[i], 64, currentBlending);
      //update the index for the pin for the next loop

      LastColorIndex[i] += 1;
    }
  }

  FastLED.delay(60);



}



void SetupPalette()
{
  CRGB AliceBlue = CRGB::AliceBlue;
  CRGB Amethyst  = CRGB::Amethyst;
  CRGB Aqua = CRGB::Aqua;
  CRGB Aquamarine = CRGB::Aquamarine;
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     black, AliceBlue,  Amethyst,  Aqua,
                     AliceBlue,  Amethyst,  Aqua,  Aquamarine,
                     AliceBlue,  Amethyst,  Aqua,  Aquamarine,
                     AliceBlue,  Amethyst,  Aqua,  black
                   );
}

