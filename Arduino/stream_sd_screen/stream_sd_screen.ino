#include <SPI.h>
#include <SD.h>
#include <Arduino_GFX_Library.h>

#define BLACK 0x0000
#define WHITE 0xFFFF
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F

// Using the low-level engine variables
Sd2Card card;
SdVolume volume;
SdFile root;
SdFile myFile; 

Arduino_DataBus *bus = new Arduino_HWSPI(11, 53);
Arduino_GFX *gfx = new Arduino_ILI9488_18bit(bus, 12);
//Arduino_GFX *gfx = new Arduino_ST7796(bus, 12);

const int sd_select = 10;
const int lcd_select = 53;

void setup() {
  Serial.begin(9600);
  while (!Serial);

 // 1. Lock the SPI bus
  pinMode(sd_select, OUTPUT);
  digitalWrite(sd_select, HIGH);
  pinMode(lcd_select, OUTPUT);
  digitalWrite(lcd_select, HIGH);

  // 2. Wake up the SD card FIRST (Before the screen hijacks the bus!)
  Serial.print("\nInitializing SD card...");
  if (!card.init(SPI_HALF_SPEED, sd_select)) {
    Serial.println("CRITICAL ERROR: The SD card completely failed to mount at half speed!");
    while (1);
  }
  
  if (!volume.init(card)) {
    Serial.println("ERROR: Could not find a valid FAT16/FAT32 partition.");
    while (1);
  }
  root.openRoot(volume);
  Serial.println("\nCard mounted ok!");

  // 3. WAKE UP THE SCREEN LAST
  Serial.print("\nInitializing TFT Screen...");
  gfx->begin();
  delay(1000);
  gfx->fillScreen(BLACK);

  // 4. Open the file from the root directory using the low-level command
  Serial.println("Drawing test.raw...");
  if (myFile.open(&root, "test.raw", O_READ)) {
    
    // Your exact drawing loop (flawless logic!)
    uint8_t rowBuffer[640];
    for (uint16_t current_row = 0; current_row < 480; current_row++) {
      myFile.read(rowBuffer, 640);
      gfx->draw16bitRGBBitmap(0, current_row, (uint16_t *)rowBuffer, 320, 1);
    }
    
    myFile.close();
    Serial.println("Image drawn successfully!");
  } else {
    Serial.println("Error: Could not open the RAW file.");
  }
}

void loop() {
  // Main code runs repeatedly
}