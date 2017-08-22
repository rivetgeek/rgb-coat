#include<FastLED.h>
#define NUM_LEDS 163

#define DATA_PIN 7
#define CLOCK_PIN 4

//Define the coat patterns
//#define TEST_PATTERN 1
#define FLOWER_PATTERN 1
#define HEART_PULSE_PATTERN 2
//#define STARFIELD_PATTERN 3

//TODO: i have two arrays of length NUM_LEDS. This is taking up far too much memory.
//Need to refactor code to just use one array to track state



//Actual FastLed array. See Fast LED docs.
CRGBArray<NUM_LEDS> leds;

//counter to keep track of how many active LEDS
int activeCounter = 0;

//cars for test pattern
unsigned long previousTime = 0;

//keeps track of the LEDs and their state (on/off). i think we can get rid of this and save a ton of memory
//int LED_ARRAY[NUM_LEDS];

int unsigned long lastPinTime;

//counter for heartbeat pulse
int unsigned long lastPulse;


// might be able to piggy
//back on leds array above and remove this
//array of last color index for active LEDS
int LastColorIndex[NUM_LEDS];

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
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR, DATA_RATE_MHZ(10)>(leds, NUM_LEDS);
  SetupPalette();
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);

  //set LED array to all zeros to allow tracking
  for (int i = 0; i < NUM_LEDS; i++) {
    LastColorIndex[i] = 0;
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
    case FLOWER_PATTERN:
      {
        //This changes how many pixels are on at any given time.
        //Increasing this affects current usage
#define MAX_ACTIVE 20
        //Is it time to turn on a new pixel?
        if ( millis() - lastPinTime > 1000 && activeCounter <= MAX_ACTIVE) {
          int newPixel;
          newPixel = random(0, NUM_LEDS);
          //find an open pixel
          while (LastColorIndex[newPixel] != 0) {
            newPixel = random(0, NUM_LEDS);
            Serial.println("Checking if " + String(newPixel));
          }


          //Add the new pixel
          LastColorIndex[newPixel] = 1;

          //reset the counter
          lastPinTime = millis();

          //Add to active counter
          activeCounter += 1;
        }

        //loop through pixels and do what needs to be done to the active ones
        for (int i = 0; i < NUM_LEDS; i++) {
          int currentPixel = i;

          //check if curentPin is active and skip is its not

          if (LastColorIndex[currentPixel] == 0) {
            continue;
          }


          //if cycle is complete, reset the color index and blank the pin
          if (LastColorIndex[currentPixel] > 255) {
            leds[currentPixel] = CRGB::Black;

            //mark the current pixel as inactive and reset its index

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
      }

    case HEART_PULSE_PATTERN:
      {
        //define the matrices for the coat parts
        CRGB pulseColor = CRGB::Red;

        //matrices for the arms starting with the wrist at the top of the matrices
        int leftArm[][4] = {
          {1, 2, 3, 4},
          {5, 6, 7, 8},
          {8, 9, 10, 11},
          {12, 13, 14, 15},
          {16, 17, 18, 19},
          {20, 21, 22, 23}
        };

        int rightArm[][4] = {
          {163, 162, 161, 160},
          {159, 158, 157, 156},
          {155, 154, 153, 152},
          {151, 150, 149, 148},
          {147, 146, 145, 144},
          {143, 142, 141, 140}
        };

        //pulse up both arms
        int counter = 0;
        if (millis() - lastPulse > 500) {
          counter++;
        }

        if (counter > 5) {
          counter = 0;
        }

        //black out all the pixels so that only the active rows are on
        for (int i=0; i<NUM_LEDS;i++){
          leds[i] = CRGB::Black;
        }
        

        while (counter <= 5) {
          leds[rightArm[counter][1]] = pulseColor;
          leds[rightArm[counter][2]] = pulseColor;
          leds[rightArm[counter][3]] = pulseColor;
          leds[rightArm[counter][4]] = pulseColor;

          leds[leftArm[counter][1]] = pulseColor;
          leds[leftArm[counter][2]] = pulseColor;
          leds[leftArm[counter][3]] = pulseColor;
          leds[leftArm[counter][4]] = pulseColor;

          FastLED.show();
          lastPulse =  millis();
        }
        break;
        {
        }
      }//end switch


  }
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

