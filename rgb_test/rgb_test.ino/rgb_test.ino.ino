#include<FastLED.h>
#define NUM_LEDS 59

#define DATA_PIN 7
#define CLOCK_PIN 4

//This changes how many pixels are on at any given time. 
//Increasing this affects current usage
#define MAX_ACTIVE 20 


//Actual FastLed array. See Fast LED docs.
CRGBArray<NUM_LEDS> leds;

//counter to keep track of how many active LEDS
int activeCounter =0;

//cars for test pattern
unsigned long previousTime =0;

//keeps track of the LEDs and their state (on/off). might be able to piggy 
//back on leds array above and remove this
int LED_ARRAY[NUM_LEDS];

int unsigned long lastPinTime;

//array of last color index for active LEDS
int LastColorIndex[NUM_LEDS];

//var for current pattern
int pattern = 2;

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
  
//set LED array to all zeros to allow tracking
  for (int i = 0; i < NUM_LEDS; i++){
    LED_ARRAY[i] = 0;
  }
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
  

      
      //Is it time to turn on a new pixel?
      if ( millis() - lastPinTime > 1000 && activeCounter <= MAX_ACTIVE) {
      int newPixel;
      newPixel = random(0, NUM_LEDS);
        //find an open pixel
        while (LED_ARRAY[newPixel]!=0){
          newPixel = random(0, NUM_LEDS);
          Serial.println("Checking if "+String(newPixel));
        }


       //Add the new pixel
       LED_ARRAY[newPixel]=1;

       //reset the counter
       lastPinTime = millis();

       //Add to active counter
       activeCounter +=1;
      }

      //loop through pixels and do what needs to be done to the active ones
      for (int i = 0; i < NUM_LEDS; i++){
      int currentPixel = i;

      //check if curentPin is active and skip is its not
      
      if (LED_ARRAY[currentPixel]==0){
        continue;
      }

      
      //if cycle is complete, reset the color index and blank the pin
      if (LastColorIndex[currentPixel] > 255) {
        leds[currentPixel] = CRGB::Black;
        
        //mark the current pixel as inactive and reset its index
        LED_ARRAY[currentPixel] =0;
        LastColorIndex[currentPixel] = 0;
        activeCounter -= 1;
      }

        //Change the pixels to the respective color index based on their place in the cycle.
        leds[currentPixel] = ColorFromPalette(currentPalette, LastColorIndex[i], 64, currentBlending);
 
        //update the index for the pin for the next loop
        LastColorIndex[currentPixel] += 1;
    
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

