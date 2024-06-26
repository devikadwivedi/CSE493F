#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int speed = 3;
int healthVal = 1000;
float score = 0.0;
int plantarX[3] = {100, 150, 210};
int currentCatFrame = 0;
int fishX = -1;

int catY = SCREEN_HEIGHT - 20;
int catYspeed = -3;
bool isJumping = false;

const int buttonPin = 12; // Define the button pin
const int speakerPin = 6; // Define the speaker pin
const int vibroPin = 10; // Define the pin connected to the vibration motor
const int potPin = A2; // Define the pin connected to the trim potentiometer
int volume = 0; // Global variable to store the volume

// 'SittingCat1', 45x50px
const unsigned char epd_bitmap_SittingCat1 [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x80, 0x03, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x03, 0x80, 0x00, 0x00, 0x01, 0xe0, 0x07, 0x80, 
	0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 
	0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x00, 0x00, 0x00, 
	0x01, 0xe7, 0xfc, 0x00, 0x00, 0x00, 0x01, 0xc3, 0xf8, 0x00, 0x00, 0x00, 0x01, 0xc3, 0xfc, 0x00, 
	0x00, 0x00, 0x01, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 
	0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 
	0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 
	0x00, 0x00, 0x03, 0xff, 0xff, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 
	0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 
	0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 
	0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 
	0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 
	0x3f, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xe7, 0xe7, 0x80, 0x00, 0x01, 0xff, 0xc3, 0xc7, 0x80, 
	0x00, 0x01, 0xff, 0xc3, 0xc7, 0x80, 0x00, 0x01, 0xff, 0xc3, 0xc7, 0x80, 0x07, 0xc1, 0xff, 0xc3, 
	0xc7, 0x80, 0x0f, 0xe1, 0xff, 0xc3, 0xc7, 0x80, 0x07, 0xc0, 0xff, 0xc3, 0xc3, 0x80, 0x00, 0x00, 
	0x07, 0x81, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'SittingCat2', 45x50px
const unsigned char epd_bitmap_SittingCat2 [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x80, 0x03, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x03, 0x80, 0x00, 0x00, 0x01, 0xe0, 0x07, 0x80, 
	0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 
	0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x00, 0x00, 0x00, 
	0x01, 0xe7, 0xfc, 0x00, 0x00, 0x00, 0x01, 0xc3, 0xf8, 0x00, 0x00, 0x00, 0x01, 0xc3, 0xfc, 0x00, 
	0x00, 0x00, 0x01, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 
	0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 
	0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 
	0x00, 0x00, 0x03, 0xff, 0xff, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 
	0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 
	0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 
	0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 
	0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 
	0x3f, 0xff, 0xff, 0x00, 0x00, 0x40, 0xff, 0xe7, 0xe7, 0x80, 0x00, 0xe1, 0xff, 0xc3, 0xc7, 0x80, 
	0x00, 0xc1, 0xff, 0xc3, 0xc7, 0x80, 0x00, 0x01, 0xff, 0xc3, 0xc7, 0x80, 0x04, 0x01, 0xff, 0xc3, 
	0xc7, 0x80, 0x0e, 0x01, 0xff, 0xc3, 0xc7, 0x80, 0x06, 0x00, 0xff, 0xc3, 0xc3, 0x80, 0x00, 0x00, 
	0x07, 0x81, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'SittingCat3', 45x50px
const unsigned char epd_bitmap_SittingCat3 [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x80, 0x03, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x03, 0x80, 0x00, 0x00, 0x01, 0xe0, 0x07, 0x80, 
	0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 
	0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x00, 0x00, 0x00, 
	0x01, 0xe7, 0xfc, 0x00, 0x00, 0x00, 0x01, 0xc3, 0xf8, 0x00, 0x00, 0x00, 0x01, 0xc3, 0xfc, 0x00, 
	0x00, 0x00, 0x01, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 
	0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 
	0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 
	0x00, 0x00, 0x03, 0xff, 0xff, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 
	0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 
	0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 
	0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 
	0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 
	0x3f, 0xff, 0xff, 0x00, 0x07, 0xc0, 0xff, 0xe7, 0xe7, 0x80, 0x0f, 0xe1, 0xff, 0xc3, 0xc7, 0x80, 
	0x07, 0xc1, 0xff, 0xc3, 0xc7, 0x80, 0x00, 0x01, 0xff, 0xc3, 0xc7, 0x80, 0x00, 0x01, 0xff, 0xc3, 
	0xc7, 0x80, 0x00, 0x01, 0xff, 0xc3, 0xc7, 0x80, 0x00, 0x00, 0xff, 0xc3, 0xc3, 0x80, 0x00, 0x00, 
	0x07, 0x81, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'SittingCat4', 45x50px
const unsigned char epd_bitmap_SittingCat4 [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x80, 0x03, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x03, 0x80, 0x00, 0x00, 0x01, 0xe0, 0x07, 0x80, 
	0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 
	0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x00, 0x00, 0x00, 
	0x01, 0xe7, 0xfc, 0x00, 0x00, 0x00, 0x01, 0xc3, 0xf8, 0x00, 0x00, 0x00, 0x01, 0xc3, 0xfc, 0x00, 
	0x00, 0x00, 0x01, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 
	0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 
	0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 
	0x00, 0x00, 0x03, 0xff, 0xff, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 
	0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 
	0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 
	0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 
	0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00, 
	0x3f, 0xff, 0xff, 0x00, 0x04, 0x00, 0xff, 0xe7, 0xe7, 0x80, 0x0e, 0x01, 0xff, 0xc3, 0xc7, 0x80, 
	0x06, 0x01, 0xff, 0xc3, 0xc7, 0x80, 0x00, 0x01, 0xff, 0xc3, 0xc7, 0x80, 0x00, 0x41, 0xff, 0xc3, 
	0xc7, 0x80, 0x00, 0xe1, 0xff, 0xc3, 0xc7, 0x80, 0x00, 0xc0, 0xff, 0xc3, 0xc3, 0x80, 0x00, 0x00, 
	0x07, 0x81, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const int epd_bitmap_sittingCat_LEN = 4;
const unsigned char* epd_bitmap_sittingCat[4] = {
	epd_bitmap_SittingCat1,
	epd_bitmap_SittingCat2,
	epd_bitmap_SittingCat3,
	epd_bitmap_SittingCat4
};
// 'plantar', 9x13px
const unsigned char epd_bitmap_plantar [] PROGMEM = {
	0x00, 0x00, 0x63, 0x00, 0x77, 0x00, 0x3e, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x00, 0x7f, 0x00, 
	0x00, 0x00, 0x1c, 0x00, 0x3e, 0x00, 0x3e, 0x00, 0x00, 0x00
};
// 'fish', 16x7px
const unsigned char epd_bitmap_fish [] PROGMEM = {
	0x7f, 0xe3, 0xc8, 0x27, 0xa4, 0x1d, 0x84, 0x0d, 0x88, 0x1d, 0xc0, 0x23, 0x7f, 0xe3
};

// Define a struct to represent a plantar
struct Plantar {
  int x; // X position
  int y; // Y position
  int width; // Width of the plantar
  int height; // Height of the plantar
};
Plantar plantars[3]; // You can adjust the number of plantars as needed
// 'runningCat1', 20x15px
const unsigned char epd_bitmap_runningCat1 [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x60, 0x1f, 0x80, 0x60, 
	0x1d, 0xc0, 0x60, 0x1f, 0xc0, 0x60, 0x1f, 0xc0, 0x20, 0x3f, 0xc0, 0x1f, 0xff, 0x80, 0x1f, 0xff, 
	0x80, 0x1b, 0xef, 0x80, 0x19, 0xcd, 0x80, 0x00, 0x05, 0x80, 0x00, 0x00, 0x00
};
// 'runningCat2clearere-removebg-preview', 20x15px
const unsigned char epd_bitmap_runningCat2 [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x04, 0x40, 0x00, 0x0f, 0x80, 0x00, 0x1f, 0x80, 0x10, 0x1c, 0xc0, 0x60, 
	0x1f, 0xc0, 0x70, 0x1f, 0xc0, 0x0f, 0xff, 0x80, 0x0f, 0xff, 0x80, 0x0f, 0xff, 0x80, 0x0e, 0xe4, 
	0x00, 0x12, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// 'runningCat3', 20x15px
const unsigned char epd_bitmap_runningCat3 [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x40, 0x0f, 0x00, 0x20, 0x0f, 0x80, 0x10, 0x0f, 0xc0, 0x08, 
	0x0f, 0xc0, 0x00, 0x09, 0x80, 0x1f, 0xfe, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 192)
const int epd_bitmap_runningCAT_LEN = 4;
const unsigned char* epd_bitmap_runningCat[4] = {
	epd_bitmap_runningCat1,
  epd_bitmap_runningCat1,
	epd_bitmap_runningCat2,
	epd_bitmap_runningCat3
};

enum Mode {
  HOME,
  GAME,
  MENU
};
Mode currentMode = HOME;
void setup() {
  // put your setup code here, to run once:
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Initialize plantars
  for (int i = 0; i < 3; i++) { 
    plantars[i].x = random(SCREEN_WIDTH, SCREEN_WIDTH * 2);
    plantars[i].y = SCREEN_HEIGHT - 13;
    plantars[i].width = 9;
    plantars[i].height = 13;
  }

  // Clear the buffer
  display.clearDisplay();
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(speakerPin, OUTPUT);
  pinMode(vibroPin, OUTPUT);
  pinMode(potPin, INPUT);
  Serial.begin(9600); // Initialize serial communication at 9600 baud rate


}
bool prevButtonState = LOW; // Initialize previous button state to HIGH (not pressed)

void loop() {
  display.clearDisplay();
  updateVolumeFromPot();
  switch (currentMode) {
    case HOME:
      homeScreen();
      break;
    case GAME:
      gameScreen();
      break;
    case MENU:
      //displayMenu();
      break;
  }
  delay(45);
}

void updateVolumeFromPot() {
  int potValue = analogRead(potPin);
  Serial.println(potValue); // Print the volume value to the serial monitor

  volume = map(potValue, 320, 693, 0, 5);
}

int counter = 0;
void homeScreen() {
  counter++;
  if (digitalRead(buttonPin) == HIGH) {
    currentMode = GAME; 
    currentCatFrame = 0;
    delay(500);
    return;
  }
  int16_t x1, y1;
  uint16_t w, h;
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.getTextBounds("Press button to start", 0, 0, &x1, &y1, &w, &h);
  int instrX = (SCREEN_WIDTH - w) / 2;
  int instrY = 0;
  
  // Display the instructions
  display.setCursor(instrX, instrY);
  display.println("Press button to start");

  display.drawBitmap((SCREEN_WIDTH - 45) / 2, SCREEN_HEIGHT - 50, epd_bitmap_sittingCat[currentCatFrame], 45, 50, SSD1306_WHITE);
  display.display();

  if (counter == 5) {
    currentCatFrame = (currentCatFrame + 1) % epd_bitmap_sittingCat_LEN;
    counter = 0;
  }
}

void gameScreen() {
  display.clearDisplay();
  populateScreen();
  populatePath();

  if (digitalRead(buttonPin) == HIGH && prevButtonState == LOW) {
    isJumping = true;
    healthVal -= 10;
    tone(speakerPin, 300, 200);
  }
  prevButtonState = digitalRead(buttonPin);
  animateJumpingCat();

  bool collision = collisionCheck();
  if (collision || healthVal <= 0) {
    gameOverScreen();
    resetGamePlay();
    currentMode = HOME;
  }
  display.display();
}

bool collisionCheck() {
  return false;
}

void populateScreen() {
  display.drawRect(5, 3, 85, 10, SSD1306_WHITE);
  int filledHealthBarWidth = map(healthVal, 0, 1000, 0, 85);
  display.fillRect(5, 3, filledHealthBarWidth, 10, SSD1306_WHITE);
  healthVal -= 10;
  if (healthVal <= 250) {
    analogWrite(vibroPin, 150);
  } else {
    digitalWrite(vibroPin, LOW);
  }
  display.drawRect(95, 3, 28, 10, SSD1306_WHITE);
  String scoreStr = String(int(score));
  while (scoreStr.length() < 4) {
    scoreStr = "0" + scoreStr;
  }
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(97, 5);
  display.println(scoreStr);
  score += 0.3;
  display.drawLine(0, SCREEN_HEIGHT - 5, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 5, SSD1306_WHITE);
}

void populatePath() {
  int minPlantarDistance = 50;
  int maxPlantarDistance = 100;
  for (int i = 0; i < 3; i++) {
    if (checkOverlap(0, catY, plantarX[i] + 3, SCREEN_HEIGHT - 14)) {
      tone(speakerPin, 622, 700);
      tone(speakerPin, 293, 700);
      tone(speakerPin, 139, 700);
      delay(1000);
      gameOverScreen();
      resetGamePlay();

      currentMode = HOME;
    }

    display.drawBitmap(plantarX[i], SCREEN_HEIGHT - 5 - 13, epd_bitmap_plantar, 9, 13, SSD1306_WHITE);
    plantarX[i] -= speed;
    if (plantarX[i] + 9 <= 0) {
      if (i == 0) {
        plantarX[i] = plantarX[3 - 1] + random(minPlantarDistance, maxPlantarDistance);
      } else {
        plantarX[i] = plantarX[i - 1] + random(minPlantarDistance, maxPlantarDistance);
      }
    }
  }

  if (fishX <= -1) {
      fishX = random(SCREEN_WIDTH, 350); // Ensure it doesn't go off-screen
      bool intersect = false;
      // Check if the fish intersects with any of the plantars
      for (int j = 0; j < 3; j++) {
        if (abs(plantarX[j] - fishX) < 13) {
          intersect = true;
          fishX = -1;
          break;
        }
      }
  }
  if (checkOverlap(0, catY, fishX, SCREEN_HEIGHT - 16)) {
      fishX = -1;
      healthVal += 100;
    }
  // if fish has been populated show on screen and move to the left
  if (fishX > -1) {
    fishX -= speed;
    display.drawBitmap(fishX, SCREEN_HEIGHT - 16, epd_bitmap_fish, 16, 7, SSD1306_WHITE);
    
  }

}

void animateJumpingCat() {
  if (isJumping == true) {
    if (catY < SCREEN_HEIGHT - 39 - volume) {
      catYspeed *= -1;
    }
    catY += catYspeed;
    if (catY > SCREEN_HEIGHT - 20) {
      isJumping = false;
      catY = SCREEN_HEIGHT - 20;
      catYspeed *= -1;
    }
    currentCatFrame = 2;
    
  }

  display.drawBitmap(0, catY, epd_bitmap_runningCat[currentCatFrame], 20, 15, SSD1306_WHITE);
  currentCatFrame = (currentCatFrame + 1) % epd_bitmap_runningCAT_LEN;
}

bool checkOverlap(int catX, int catY, int plantX, int plantY) {
    // Define the bounding boxes for the cat and plant sprites
    int catLeft = catX;
    int catRight = catX + 17;
    int catTop = catY;
    int catBottom = catY + 13;

    int plantLeft = plantX;
    int plantRight = plantX + 7;
    int plantTop = plantY;
    int plantBottom = plantY + 10;

    // Check for overlap by comparing the bounding boxes
    if (catLeft < plantRight && catRight > plantLeft && catTop < plantBottom && catBottom > plantTop) {
        // Bounding boxes intersect, so the sprites overlap
        return true;
    }

    // Bounding boxes do not intersect, so no overlap
    return false;
}

void gameOverScreen() {
  display.clearDisplay();
  digitalWrite(vibroPin, LOW);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("GAME");
  display.setCursor(0, SCREEN_HEIGHT / 2);
  display.println("OVER");

  display.setCursor(SCREEN_WIDTH / 2, 10);
  display.println("SCORE:");
  display.setCursor(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  String scoreStr = String(int(score));
  while (scoreStr.length() < 4) {
    scoreStr = "0" + scoreStr;
  }
  display.println(scoreStr);
  display.display();  
  delay(2000);

}

void resetGamePlay() {
  speed = 3;
  healthVal = 1000;
  score = 0.0;
  plantarX[0] = 100;
  plantarX[1] = 150;
  plantarX[2] = 210;
  currentCatFrame = 0;
  fishX = -1;

  catY = SCREEN_HEIGHT - 20;
  catYspeed = -3;
  isJumping = false;
}
