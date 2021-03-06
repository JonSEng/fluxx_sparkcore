// This is a demonstration on how to use an input device to trigger changes on your neo pixels.
// You should wire a momentary push button to connect from ground to a digital IO pin.  When you
// press the button it will change to a new pixel animation.  Note that you need to press the
// button once to start the first animation!

#include "neopixel/neopixel.h"
#include "Wtv020sd16p/Wtv020sd16p.h"


int resetPin = 3;  // The pin number of the reset pin.
int clockPin = 1;  // The pin number of the clock pin.
int dataPin = 2;  // The pin number of the data pin.
int busyPin = 0;  // The pin number of the busy pin.


#define MAGNET_SWITCH   5    // Digital IO pin connected to the button.  This will be
#define BUTTON 6                      // driven with a pull-up resistor so the switch should
// pull the pin to ground momentarily.  On a high -> low
// transition the button press logic will execute.

#define PIXEL_PIN    4   // Digital IO pin connected to the NeoPixels.
#define ledPin 7
#define PIXEL_COUNT 16

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick

#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

bool oldState = HIGH;
bool oldmswitch = HIGH;
int showType = 1;
//int leds = 16;
int power = 0;
int bpress = 0;
int hits = 0;
Wtv020sd16p wtv020sd16p(resetPin, clockPin, dataPin, busyPin);

void setup() {
  Spark.function("powrrup", powrrup);
  wtv020sd16p.reset();
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(MAGNET_SWITCH, INPUT_PULLUP);
  //pinMode(PIXEL_PIN, OUTPUT);
  wtv020sd16p.reset();
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

int powrrup(String poww){
    int powww = poww.toInt();
      if (powww < 0 || powww > 15) return -1;
      for (int i = 0; i < powww; i++){
       //power++ ;
       strip.show();
       powerUp(strip.Color(0, 0, 255), 50);
      }
    

}


void loop() {

 // Get current button state.
  bool newState = digitalRead(BUTTON);
  bool mswitch = digitalRead(MAGNET_SWITCH);
  
   if (mswitch == LOW && oldState == HIGH) {
    delay(20);
    // Check if button is still low after debounce.
    mswitch = digitalRead(MAGNET_SWITCH);
    if (mswitch == LOW) {
      digitalWrite(ledPin, HIGH);
      strip.show();
      powerUp(strip.Color(255, 0, 0), 50);
    }
      //
  }
 
  // Check if state changed from high to low (button press).
  if (newState == LOW && oldState == HIGH) {
    delay(20);
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON);
    if (newState == LOW) {
      digitalWrite(ledPin, HIGH);
      showType++;
      bpress++;
      if (bpress % 3 == 0) {
        hits++;
      }
      if (showType > 3) {
        showType = 1;
      }
      strip.show();
      startShow(showType);
    }
      //
  }
  

  // Set the last button state to the old state.
  oldState = newState;
  oldmswitch = mswitch;
}




void startShow(int i) {
  strip.setBrightness(100);
  switch (i) {
    case 0: colorWipe(strip.Color(0, 0, 0), 50);    // Black/off
      wtv020sd16p.playVoice(1);
      break;
    case 1: colorWipe(strip.Color(255, 0, 0), 50); //red
      wtv020sd16p.playVoice(1);
      break;
    case 2: colorWipe(strip.Color(0, 255, 0), 50);  // Green
       wtv020sd16p.playVoice(2);
      break;
    case 3: colorWipe(strip.Color(0, 0, 255), 50);  // Blue
       wtv020sd16p.playVoice(3);
      break;
    case 4: theaterChase(strip.Color(127, 127, 127), 50); // White
      break;
    case 5: theaterChase(strip.Color(127,   0,   0), 50); // Red
      break;
    case 6: theaterChase(strip.Color(  0,   0, 127), 50); // Blue
      break;
    case 7: rainbow(20);
      break;
    case 8: rainbowCycle(20);
      break;
    case 9: theaterChaseRainbow(50);
      break;
  }
  for (int blinkloop = 0; blinkloop < 3; blinkloop++){
       digitalWrite(ledPin, HIGH);
       delay(500);               // wait for a second
       digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
       delay(500);
     }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {



  for (uint16_t i = 0; i < (power - hits); i++) {
    strip.setPixelColor(i, c);
    delay(wait);
    strip.show();
  }
  
  for (uint16_t i = (power - hits); i < 16; i++) {
    strip.setPixelColor(i, 0);
    delay(wait);
    strip.show();

  }
}

void powerUp(uint32_t c, uint8_t wait) {

  power++;

  for (uint16_t i = 0; i < (power); i++) {
    strip.setPixelColor(i, c);
    delay(wait);
    strip.show();
  }
  
  for (uint16_t i = power; i < 16; i++) {
    strip.setPixelColor(i, 0);
    delay(wait);
    strip.show();

  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}


