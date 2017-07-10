#include<FastLED.h>
#define NUM_LEDS 20

//#define NUM_LEDS 3
#define DATA_PIN 7
#define CLOCK_PIN 4
//this is an ugly hack to denote an led thats not being used. need to fix this.
//consider making array with size of num_led and each running LED being the index,
//and the value either 0/1
#define AVAILABLE_LED 999
#define NUM_ACTIVE 20 //This changes how many pixels are on at any given time

CRGBArray<NUM_LEDS> leds;

//cars for test pattern
unsigned long previousTime =0;

//array of currently active LEDS
int RunningPins[NUM_ACTIVE];

int unsigned long lastPinTime;

//array of last color index for active LEDS
int LastColorIndex[NUM_ACTIVE];

//var for current pattern
int pattern = 1;

int num_patterns = 2;

//vars for button
int buttonState;
int buttonPin = 6;
int lastButtonState = LOW;

//for debouncing the button
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


//Color palette that will be sewpt through with cos function.
CRGBPalette16 currentPalette = RainbowColors_p;
TBlendType    currentBlending = LINEARBLEND;

void setup() {

  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR,DATA_RATE_MHZ(10)>(leds, NUM_LEDS);
  SetupPalette();
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  //Start Button stuff
  //detect button press to change pattern
  int reading = digitalRead(buttonPin);

  //switch changed
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
    
  }


  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        previousTime = millis();
        FastLED.clear();
        if (pattern == num_patterns) {
          pattern = 1;
        } else {
          pattern++;
        }
        Serial.println("Button pressed");
        Serial.println("pattern is");
        Serial.println(pattern);
      }
    }
  }

  lastButtonState = reading;
  //End button stuff

//enter patterns here
  switch (pattern) {
  case 1:
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
//TODO: this delay is breaking the button (maybe).
    FastLED.delay(60);
    break;

//Test pattern - runs through RGB on all LEDs
  case 2:

  int colortime = 500;
  //reset timer for first use or repeating pattern
  CRGB color;
  if (millis() - previousTime <= colortime){
    color = CRGB::Red;

  }
    if (millis() - previousTime > colortime &&  millis() - previousTime< (colortime * 2)){
    color = CRGB::Green;
  }
    if (millis() - previousTime > colortime* 2 && millis() - previousTime< colortime *3){
    color = CRGB::Blue;
  }
      if (millis() - previousTime >= colortime * 4){
  previousTime =millis();
  }
  
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = color;
    }
    FastLED.show();
    break;
  }//end switch
  
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

