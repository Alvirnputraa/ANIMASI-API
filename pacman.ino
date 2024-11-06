#include <FastLED.h>

#define NUM_COLS 60
#define NUM_ROWS 10
#define NUM_LEDS NUM_COLS * NUM_ROWS
#define COOLING 55
#define SPARKING 120

CRGB leds[NUM_LEDS];
byte heat[NUM_COLS][NUM_ROWS];  

void setup() {
  FastLED.addLeds<WS2812B, 3, GRB>(leds, NUM_LEDS);
  FastLED.clear();
}

void loop() {
  fireEffect();
  FastLED.show();
  FastLED.delay(40);  
}

void fireEffect() {
 
  for (int x = 0; x < NUM_COLS; x++) {
    for (int y = NUM_ROWS - 1; y >= 2; y--) {
      heat[x][y] = (heat[x][y] - random(0, ((COOLING * 10) / NUM_ROWS) + 2));
      heat[x][y] = max(0, heat[x][y]);  
    }
  }

  
  for (int x = 0; x < NUM_COLS; x++) {
    for (int y = NUM_ROWS - 1; y >= 1; y--) {
      heat[x][y] = (heat[x][y - 1] + heat[x][y - 2]) / 2;
    }
  }

  
  for (int x = 0; x < NUM_COLS; x++) {
    if (random(255) < SPARKING) {
      int y = random(2);  
      heat[x][y] = qadd8(heat[x][y], random(160, 255));  
    }
  }

 
  for (int x = 0; x < NUM_COLS; x++) {
    for (int y = 0; y < NUM_ROWS; y++) {
      uint8_t colorIndex = scale8(heat[x][y], 240);  
      leds[XY(x, y)] = HeatColor(colorIndex);       
    }
  }
}


uint16_t XY(uint8_t x, uint8_t y) {
  return y * NUM_COLS + x;
}
