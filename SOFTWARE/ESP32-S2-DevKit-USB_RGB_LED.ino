/*
  Board: ESP32-S2-DevKit-USB
  Requires Adafruit NeoPixel library
 */

#include <Adafruit_NeoPixel.h>  // go to Main Menu --> Sketch --> Include Library --> Manage libraries... search for Adafruit NeoPixel and install it (as of 4th February 2021 latest version is 1.7.0)
#define PIN 18
#define NUMPIXELS 1
#define PERIOD  10  //ms

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int R=255, G=0, B=0;

void setup()
{
  pixels.begin();
}

void loop ()
{
  for (G=0; G<255; G++)
  {
    pixels.setPixelColor(0, pixels.Color(R, G, B));
    pixels.show();
    delay (PERIOD);
  }
  
  for (R=255; R>0; R--)
  {
    pixels.setPixelColor(0, pixels.Color(R, G, B));
    pixels.show();
    delay (PERIOD);
  }
  
  for (B=0; B<255; B++)
  {
    pixels.setPixelColor(0, pixels.Color(R, G, B));
    pixels.show();
    delay (PERIOD);
  }
  
  for (G=255; G>0; G--)
  {
    pixels.setPixelColor(0, pixels.Color(R, G, B));
    pixels.show();
    delay (PERIOD);
  }
  
  for (R=0; R<255; R++)
  {
    pixels.setPixelColor(0, pixels.Color(R, G, B));
    pixels.show();
    delay (PERIOD);
  }
  
  for (B=255; B>0; B--)
  {
    pixels.setPixelColor(0, pixels.Color(R, G, B));
    pixels.show();
    delay (PERIOD);
  }
}
