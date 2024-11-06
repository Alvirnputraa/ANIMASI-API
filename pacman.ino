#include <FastLED.h>

#define NUM_COLS 60
#define NUM_ROWS 10
#define NUM_LEDS NUM_COLS * NUM_ROWS
#define COOLING 55
#define SPARKING 120

CRGB leds[NUM_LEDS];
byte heat[NUM_COLS][NUM_ROWS];  // Array untuk menyimpan intensitas "panas" tiap LED

void setup() {
  FastLED.addLeds<WS2812B, 3, GRB>(leds, NUM_LEDS);
  FastLED.clear();
}

void loop() {
  fireEffect();
  FastLED.show();
  FastLED.delay(30);  // Mengatur kecepatan perubahan efek api
}

void fireEffect() {
  // Langkah 1: Mendinginkan setiap kolom api sedikit, dengan mengurangi intensitas panas secara acak
  for (int x = 0; x < NUM_COLS; x++) {
    for (int y = NUM_ROWS - 1; y >= 2; y--) {
      heat[x][y] = (heat[x][y] - random(0, ((COOLING * 10) / NUM_ROWS) + 2));
      heat[x][y] = max(0, heat[x][y]);  // Pastikan nilai tidak negatif
    }
  }

  // Langkah 2: Panas dari kolom bawah "naik" ke kolom atas, memindahkan intensitas dari bawah ke atas
  for (int x = 0; x < NUM_COLS; x++) {
    for (int y = NUM_ROWS - 1; y >= 1; y--) {
      heat[x][y] = (heat[x][y - 1] + heat[x][y - 2]) / 2;
    }
  }

  // Langkah 3: Spark atau percikan panas acak di bagian bawah api
  for (int x = 0; x < NUM_COLS; x++) {
    if (random(255) < SPARKING) {
      int y = random(2);  // Membatasi percikan di dua baris terbawah
      heat[x][y] = qadd8(heat[x][y], random(160, 255));  // Menambah intensitas panas
    }
  }

  // Langkah 4: Map intensitas panas ke warna dan menampilkannya pada LED
  for (int x = 0; x < NUM_COLS; x++) {
    for (int y = 0; y < NUM_ROWS; y++) {
      uint8_t colorIndex = scale8(heat[x][y], 240);  // Skala warna untuk tampilan
      leds[XY(x, y)] = HeatColor(colorIndex);        // Gunakan warna api
    }
  }
}

// Fungsi untuk mengkonversi koordinat 2D ke indeks 1D
uint16_t XY(uint8_t x, uint8_t y) {
  return y * NUM_COLS + x;
}
