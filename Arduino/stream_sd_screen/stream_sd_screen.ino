#include <SPI.h>
#include <SD.h>
#include <Arduino_GFX_Library.h>

#define BLACK 0x0000
#define WHITE 0xFFFF
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F

Sd2Card card;
SdVolume volume;
SdFile root;
File myFile;

Arduino_DataBus *bus = new Arduino_HWSPI(11, 53);
// Arduino_GFX *gfx = new Arduino_ST7796(bus, 12);
Arduino_GFX *gfx = new Arduino_ILI9488_18bit(bus, 12);

const int sd_select = 10;
const int lcd_select = 53;

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  Serial.print("\nInitializing TFT Screen...");

  gfx->begin();
  delay(1000);
  gfx->fillScreen(RED);

  Serial.print("\nInitializing SD card...");

  if (!card.init(SPI_HALF_SPEED, sd_select)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    Serial.println("Note: press reset button on the board and reopen this Serial Monitor after fixing your issue!");
    while (1);
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }

   Serial.println();
  Serial.print("Card type:         ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

    if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    while (1);
  }

   Serial.print("Clusters:          ");
  Serial.println(volume.clusterCount());
  Serial.print("Blocks x Cluster:  ");
  Serial.println(volume.blocksPerCluster());

  Serial.print("Total Blocks:      ");
  Serial.println(volume.blocksPerCluster() * volume.clusterCount());
  Serial.println();

  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("Volume type is:    FAT");
  Serial.println(volume.fatType(), DEC);

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1 KB)
  Serial.print("Volume size (KB):  ");
  Serial.println(volumesize);
  Serial.print("Volume size (MB):  ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (GB):  ");
  Serial.println((float)volumesize / 1024.0);

  root.openRoot(volume);

  // Strategy is to build a small buffer (fixed size, i.e. a single row)
  // You fill that buffer
  // Translate the data and push the pixels directly to the screen driver
  // Rinse and repeat

  //open file
  SD.begin(sd_select);
  myFile = SD.open("mads.raw", FILE_READ);

  if (myFile) {
    uint8_t rowBuffer[640];
    for (uint16_t current_row = 0 ; current_row < 480 ; current_row++) {
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
  // put your main code here, to run repeatedly:

}
