#include<FastLED.h>
#define NUM_LEDS 150

#define DATA_PIN 7
#define CLOCK_PIN 4

//Define the coat patterns

#define FLOWER_PATTERN 1
#define HEART_PULSE_PATTERN 2
#define NULL_PATTERN 3


//var for current and start pattern
int pattern = 1;

//total number of patterns
int num_patterns = 3;


//This changes how many pixels are on at any given time.
//Increasing this affects current usage
#define MAX_ACTIVE 60
#define PIN_FREQUENCY 200


//Actual FastLed array. See Fast LED docs.
CRGBArray<NUM_LEDS> leds;

//counter to keep track of how many active LEDS
int activeCounter = 0;

//cars for test pattern
unsigned long previousTime = 0;


int unsigned long lastPinTime;

//counter for heartbeat pulse
int unsigned long lastPulse = 0;

//brightness for pulsing
int brightness;

//what row is active (originally for pulse)
int row = 0;

//array of last color index for active LEDS
int LastColorIndex[NUM_LEDS];


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
  randomSeed(analogRead(0));

  //set LED array to all zeros to allow tracking
  for (int i = 0; i < NUM_LEDS; i++) {
    LastColorIndex[i] = 0;
  }
}
void loop() {
  ButtonCheck();
  //enter patterns here
  switch (pattern) {
    case FLOWER_PATTERN:
      {

        //black out all the pixels so that only the active rows are on
        FastLED.clear();

        //Is it time to turn on a new pixel?
        if ( (millis() - lastPinTime > PIN_FREQUENCY) && (activeCounter <= MAX_ACTIVE)) {
          int newPixel;
          newPixel = random(0, NUM_LEDS);

          //Keep generating random pixel locations until we find a free (0) slot.
          while (LastColorIndex[newPixel] != 0) {
            newPixel = random(0, NUM_LEDS);
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
          if (LastColorIndex[currentPixel] > 0) {

            //if cycle is complete, reset the color index and blank the pin
            if (LastColorIndex[currentPixel] >= 255) {

              //turn the pixel off
              leds[currentPixel] = CRGB::Black;

              //mark the current pixel as inactive and reset its index
              LastColorIndex[currentPixel] = 0;

              // FastLED.delay(60);
              activeCounter -= 1;
              continue;
            }



            //Change the pixels to the respective color index based on their place in the cycle.
            leds[currentPixel] = ColorFromPalette(currentPalette, LastColorIndex[i], 64, currentBlending);

            //update the index for the pin for the next loop
            LastColorIndex[currentPixel] += 1;

          }//end of active pixel if statement

        }//end of for loop to set up the array to be shown

        //Show the pixels based on the current "leds" array.
        FastLED.show();
        FastLED.delay(60);

      }//end of case
      break;

    case HEART_PULSE_PATTERN:
      {

        CRGB pulseColor = CRGB::Red;
        ///----------------------------------------------------------------------------------------------
        //wiring order:
        //Right arm, right front, back, left front, hood, left arm


        //right side 7 vertical x 3 horizontal

        //back 7 vertical x 4 horizontal

        //define the matrices for the coat parts
        //matrices for the arms starting with the wrist at the top of the matrices
        //-----------------------------------------------------------------------------------------------
        int rightArm[6][4] = {
          {0, 1, 2, 3},
          {4, 5, 6, 7},
          {8, 9, 10, 11},
          {12, 13, 14, 15},
          {16, 17, 18, 19},
          {20, 21, 22, 23}
        };

        int leftArm[6][4] = {
          {150, 149, 148, 147},
          {146, 145, 144, 143},
          {142, 141, 140, 139},
          {138, 137, 136, 135},
          {134, 133, 132, 131},
          {130, 129, 128, 127}
        };

        //right front side
        // < -- to right arm
        // -- > to center front
        int rightSide[7][3] = {

          {24, 37, 38},
          {25, 36, 39},
          {26, 35, 40},
          {27, 34, 41},
          {28, 33, 42},
          {29, 32, 43},
          {30, 31, 44}
        };

        int leftSide[7][3] = {
          {93, 80, 79},
          {92, 81, 78},
          {91, 82, 77},
          {90, 83, 76},
          {89, 84, 75},
          {88, 85, 74},
          {87, 86, 73}

        };

        int back[7][4] = {
          {51, 52, 65, 66},
          {50, 53, 64, 67},
          {49, 54, 63, 68},
          {48, 55, 62, 69},
          {47, 56, 61, 70},
          {46, 57, 60, 71},
          {45, 58, 59, 72}
        };
        //  int left side
        //pulse up both arms

        //if it is time to pulse up one row
        if (millis() - lastPulse > 500) {
          FastLED.clear();
          Serial.println("Time to pulse!");
          Serial.println("The row is: " + String(row));
          lastPulse =  millis();




          //black out all the pixels so that only the active rows are on
         // FastLED.clear();
          //pulse across the chest
          if (row > 5) {

            //start sholder transition
            if (row == 6) {
              leds[leftSide[0][2]] = pulseColor;
              leds[rightSide[0][0]] = pulseColor;
              PulseAll(1);
            }
            //Beat the actual heart twice
            if (row == 7) {
              leds[rightSide[0][2]] = pulseColor;
              leds[rightSide[1][2]] = pulseColor;
              leds[rightSide[2][2]] = pulseColor;
              leds[rightSide[3][2]] = pulseColor;
              leds[rightSide[1][1]] = pulseColor;
              leds[rightSide[2][1]] = pulseColor;
              //left side
              leds[leftSide[0][0]] = pulseColor;
              leds[leftSide[1][0]] = pulseColor;
              leds[leftSide[2][0]] = pulseColor;
              leds[leftSide[3][0]] = pulseColor;
              leds[leftSide[1][1]] = pulseColor;
              leds[leftSide[2][1]] = pulseColor;

              PulseAll(2);
              row = 0;
            }
          } else {

            //          Pulse up the arms
            leds[rightArm[row][0]] = pulseColor;
            leds[rightArm[row][1]] = pulseColor;
            leds[rightArm[row][2]] = pulseColor;
            leds[rightArm[row][3]] = pulseColor;

            leds[leftArm[row][0]] = pulseColor;
            leds[leftArm[row][1]] = pulseColor;
            leds[leftArm[row][2]] = pulseColor;
            leds[leftArm[row][3]] = pulseColor;

            //loops through brightness levels for whole coat to create pulsing effect
            PulseAll(1);

          }


          row++;
        }

      }//end heart pulse pattern
      break;

case NULL_PATTERN:
{
  ButtonCheck();
  FastLED.clear();
}
break;


  }//end switch
}//end loop

void ButtonCheck() {
  FastLED.setBrightness(255);
  //Start Button stuff
  //detect button press to change pattern
  int reading = digitalRead(buttonPin);

  //switch changed
  if (reading != lastButtonState) {
    lastDebounceTime = millis();

  }


  if ((millis() - lastDebounceTime) > debounceDelay) {
    //good press

    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        row = 0;
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
}

//PulseAll
void PulseAll(int count) {
  for (int i = 0; i < count; i++) {
    for (int i = 0; i < 255; i++) {
      ButtonCheck();
      FastLED.setBrightness(i);
      FastLED.show();
    }
    for (int i = 255; i > 0; i--) {
      ButtonCheck();
      FastLED.setBrightness(i);
      FastLED.show();
    }
  }
}
void SetupPalette()
{
  CRGB AliceBlue = CRGB::AliceBlue;
  CRGB Amethyst  = CRGB::Amethyst;
  CRGB Aqua = CRGB::Aqua;
  CRGB Aquamarine = CRGB::Aquamarine;
  CRGB Black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     Black, AliceBlue,  Amethyst,  Aqua,
                     AliceBlue,  Amethyst,  Aqua,  Aquamarine,
                     AliceBlue,  Amethyst,  Aqua,  Aquamarine,
                     AliceBlue,  Amethyst,  Aqua,  Black
                   );
}

