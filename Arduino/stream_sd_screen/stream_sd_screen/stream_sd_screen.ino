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
uint8_t rowBuffer[640];

Arduino_DataBus *bus = new Arduino_HWSPI(11, 53);
Arduino_GFX *gfx = new Arduino_ILI9488_18bit(bus, 12);
//Arduino_GFX *gfx = new Arduino_ST7796(bus, 12);

const int sd_select = 10;
const int lcd_select = 53;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.print("\nInitializing SD card...");
  if (!card.init(SPI_QUARTER_SPEED, sd_select)) {
    Serial.println("CRITICAL ERROR: The SD card completely failed to mount at half speed!");
    while (1);
  }
  
  if (!volume.init(card)) {
    Serial.println("ERROR: Could not find a valid FAT16/FAT32 partition.");
    while (1);
  }
  root.openRoot(volume);
  Serial.println("\nCard mounted ok!");

  Serial.print("\nInitializing TFT Screen...");
  gfx->begin();
  delay(1000);
  gfx->fillScreen(BLACK);

  Serial.println("Drawing test.bmp...");
  if (myFile.open(&root, "test.bmp", O_READ)) {

    uint32_t imageOffset;
    // Offset 10 tells us where the pixels start (https://en.wikipedia.org/wiki/BMP_file_format#Bitmap_file_header)
    myFile.seekSet(10);                
    myFile.read(&imageOffset, 4); // Read that exact starting address
    myFile.seekSet(imageOffset);  // Jump straight to that address

    for (int16_t current_row = 479; current_row >= 0; current_row--) {
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