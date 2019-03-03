#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <FastLED_NeoMatrix.h>
#include <FastLED.h>
#include <SPI.h>

#include "FS.h"

#include "GifDecoder.h"

// #define DEBUG

#ifdef DEBUG
#define DEBUG_SCREEN_CLEAR_CALLBACK
#define DEBUG_UPDATE_SCREEN_CALLBACK
#define DEBUG_DRAW_PIXEL_CALLBACK
// #define DEBUG_FILE_SEEK_CALLBACK
// #define DEBUG_FILE_POSITION_CALLBACK
// #define DEBUG_FILE_READ_CALLBACK
// #define DEBUG_FILE_READ_BLOCK_CALLBACK
#endif

#define GIF_LOOP1_FILE "/loop4.gif"

#define DATA_PIN 5

// Max is 255, 32 is a conservative value to not overload
// a USB power supply (500mA) for 12x12 pixels.
#define BRIGHTNESS 32

#define SINGLE_MATRIX_WIDTH 32
#define SINGLE_MATRIX_HEIGHT 8
#define NUM_LEDS (SINGLE_MATRIX_WIDTH*SINGLE_MATRIX_HEIGHT)
CRGB leds[NUM_LEDS];

File file;

FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(
  leds,
  SINGLE_MATRIX_WIDTH,
  SINGLE_MATRIX_HEIGHT,
  1,
  1, 
  //single matrix
  NEO_MATRIX_TOP      + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS  + NEO_MATRIX_ZIGZAG + 
  
  //matrix tiling
  NEO_TILE_TOP + NEO_TILE_LEFT + 
  NEO_TILE_PROGRESSIVE
);

/* template parameters are maxGifWidth, maxGifHeight, lzwMaxBits
 * 
 * The lzwMaxBits value of 12 supports all GIFs, but uses 16kB RAM
 * lzwMaxBits can be set to 10 or 11 for smaller displays to save RAM, but use 12 for large displays
 * All 32x32-pixel GIFs tested so far work with 11, most work with 10
 */
const uint8_t maxGifWidth = SINGLE_MATRIX_WIDTH;
const uint8_t maxGifHeight = SINGLE_MATRIX_HEIGHT;
GifDecoder<maxGifWidth, maxGifHeight, 12> decoder;

void screenClearCallback() {
  #ifdef DEBUG_SCREEN_CLEAR_CALLBACK
  Serial.println(">>> screenClearCallback");
  #endif
  // clear does not work properly with multiple matrices connected via parallel inputs
  memset(leds, 0, sizeof(leds));
}

void updateScreenCallback(){
  #ifdef DEBUG_UPDATE_SCREEN_CALLBACK
  Serial.println(">>> updateScreenCallback");
  #endif
  matrix->show();
}

void drawPixelCallback(int16_t x, int16_t y, uint8_t red, uint8_t green, uint8_t blue){
  #ifdef DEBUG_DRAW_PIXEL_CALLBACK
  Serial.println(">>> drawPixelCallback");
  if(x>8 || y>8){
    Serial.print(">>> pixel (");
    Serial.print(x);
    Serial.print(",");
    Serial.print(y);
    Serial.println(")");
  }
  #endif

  // CRGB color = CRGB(matrix->gamma[red], matrix->gamma[green], matrix->gamma[blue]);
  // matrix->setPassThruColor(color.red*65536 + color.green*256 + color.blue);
  // matrix->drawPixel(x, y, color);

  matrix->drawPixel(x, y, FastLED_NeoMatrix::Color(red, green, blue));
}

bool fileSeekCallback(unsigned long position){
  #ifdef DEBUG_FILE_SEEK_CALLBACK
  Serial.println(">>> fileSeekCallback");
  Serial.print(">>> position: ");
  Serial.println(position);
  #endif
  bool r = file.seek(position);
  #ifdef DEBUG_FILE_SEEK_CALLBACK
  Serial.println(">>> r");
  Serial.println(r);
  #endif
  return r;
}

unsigned long filePositionCallback(){
  #ifdef DEBUG_FILE_POSITION_CALLBACK
  Serial.println(">>> filePositionCallback");
  #endif
  return file.position();
}

int fileReadCallback(){
  #ifdef DEBUG_FILE_READ_CALLBACK
  Serial.println(">>> fileReadCallback");
  #endif
  return file.read();
}

int fileReadBlockCallback(void * buffer, int numberOfBytes){
  #ifdef DEBUG_FILE_READ_BLOCK_CALLBACK
  Serial.println(">>> fileReadBlockCallback");
  Serial.println(numberOfBytes);
  #endif

  int num_read = file.read((uint8_t *)buffer, numberOfBytes);

  #ifdef DEBUG_FILE_READ_BLOCK_CALLBACK
  Serial.print("read ");
  Serial.println(num_read);
  Serial.print(": ");
  Serial.println((char *)buffer);
  #endif

  return num_read;
}

void setup() {
  Serial.begin(9600);

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  // matrix->begin();
  matrix->setBrightness(BRIGHTNESS);

  SPIFFS.begin();

  FSInfo fs_info;
  SPIFFS.info(fs_info);
  printf("\nSPIFFS: %lu of %lu bytes used.\n",
    fs_info.usedBytes, fs_info.totalBytes);

  file = SPIFFS.open(GIF_LOOP1_FILE, "r");
  if (!file) {
    Serial.println("file open failed");
    return;
  }

  Serial.println(">>>");
  Serial.println(file.name());
  Serial.println(file.size());

  decoder.setScreenClearCallback(screenClearCallback);
  decoder.setUpdateScreenCallback(updateScreenCallback);
  decoder.setDrawPixelCallback(drawPixelCallback);

  decoder.setFileSeekCallback(fileSeekCallback);
  decoder.setFilePositionCallback(filePositionCallback);
  decoder.setFileReadCallback(fileReadCallback);
  decoder.setFileReadBlockCallback(fileReadBlockCallback);

  decoder.startDecoding();
}

void loop() {
  decoder.decodeFrame();
}