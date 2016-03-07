#include<FastLED.h>
#define NUM_LEDS 96
#define DATA_PIN 3
#define CLOCK_PIN 4
#define NUM_ACTIVE 20

CRGBArray<NUM_LEDS> leds;

//array of active LEDS
int RunningPins[NUM_ACTIVE];

//array of last setting for active LEDS
int LastColorIndex[NUM_ACTIVE];
int unsigned long lastPinTime;
CRGBPalette16 currentPalette = RainbowColors_p;
TBlendType    currentBlending = LINEARBLEND;

void setup() {
  
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);
SetupPalette();
}

void loop() {

    //set the color for each pin in the running array
   for (int i=0;i<NUM_ACTIVE;i++){
    
    if ( millis() - lastPinTime > 3000){
      while 
  RunningPins[i] = random(0,NUM_LEDS);
  lastPinTime = millis();
}

      //if cycle is complete, reset the color index and blank the pin
      if (LastColorIndex[i] >255){
          leds[RunningPins[i]] = CRGB::Black;
          //if time to start new pin
//*****
//*****
          RunningPins[i] = NULL;
          LastColorIndex[i]=0;
   
        }

    leds[RunningPins[i]] = ColorFromPalette(currentPalette, LastColorIndex[i], 64, currentBlending);
    LastColorIndex[i]+=1;
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




