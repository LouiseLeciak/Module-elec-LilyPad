# Storage

This module handles all matters related to SD card operation as well as FAT
filesystem navigation and streaming data from the aforementioned SD card.

sd.c/h are responsible for initialising a SD card, they also provide an interface
to read block by block or several blocks.

fatfs.c/h are an abstraction layer for the FAT file system, this program only needs
read operations so you can only read files in the root directory.

sd_streaming.c/h are an abstraction layer to stream bitmap files to the main screen.

## Succinct explanations

### SD card initialisation

We followed Elm-Chan's [guide](https://elm-chan.org/docs/mmc/mmc_e.html) (as well
as the SD Physical Layer Simplified Specification) to guide us through the
initialisation of the SD Card over SPI.

Transactions with the SD card take the shape of a command (and optional data) and
response pair. When a SD card is set under tension, it only accepts a handful of
commands.

> [!Warning]
> During initialisation, the SD Card needs the SPI clock rate to be set between
> 100-400kHz !

Here is the flow chart of SD card initialisation in SPI mode :  
![sdinit](https://elm-chan.org/docs/mmc/rc/sdinit.png)

### Read operations

We wanted to be able to dynamically read and load the address of each image in our
MCU's SRAM, without having to use any tool to prepare the SD card (besides having
files named a certain way). As Elm-Chan's page pointed out, SD card use the FAT
filesystem, so we pondered whether it would be worth the effort to implement it.

As we were to only read files, implementation was easier than a full fledged one,
thankfully using [Microsoft's FAT Specification](https://academy.cba.mit.edu/classes/networking_communications/SD/FAT.pdf) and looking at the structures for Master Boot Record (MBR), partition entries inside the MBR and Volume Boot Record (VBR),
we managed to implement it, thus allowing the program to scan the files at the
root of the SD card to look for Bitmap (.bmp) files in order to store their starting
address in a look up table so we can avoid losing time trying to find where each image
is when we need to display them on the screen !

It might be the most structure-heavy part of the codebase, mainly because structs
allows for easily mapping the data (that is why the structures have `__attribute__((packed))`
, it prevents those structs from being padded, so we can just use a home made memcpy
function to retrieve the data).

### sd_streaming.c/h

Last but not least, this file is the origin point for all this module. It is because
we wanted to stream pictures that we needed to have them on a SD card, which we
had to initialise, and we also needed to navigate the file system of that card to
find the images. All that put together allowed us to stream images directly from
the SD card !

`sd_stream_bmp_to_screen` does several things :

1. It reads the Bitmap image header and parses it
2. If the image has got invalid signature, compression or bits-per-pixel, it returns
3. It computes the window needed to display the image
4. It pushes the RGB data to each pixel, until all the image has been displayed

## Challenges

### SD card causing bus contention

One of the most wizard-y issues we encountered was the SD card causing the screen
to display things in a garbled manner. It truly was a proper headache to understand
why. As soon as the SD card was inserted in its socket, the screen went rogue with
its displaying. We haven't managed to pin point exactly what the issue was but found
a way to empirically avoid this. After ordering our final PCBs, we discovered the
hypothetical reason behind this, on Elm-Chan's page about SD card use in SPI:  
![spicon](https://elm-chan.org/docs/mmc/rc/spicon.png)
Notice the $R_{PU}$ resistor ? It's a pull-up resistor on the MISO line. It
matters because otherwise the MISO line can float (which you obviously don't want).
Our hypothesis was conceptual-millimeters away: setting a pull-up resistor on the
SD's chip select. But to be honest, we just wound up not plugging the screen's MISO
pin, as we don't need it. With the screen's MISO left unplugged, the problem stopped
happening, thus it didn't hinder us.

### Conflation between block, cluster and sectors

This one is more of a human problem than a program one. It's just that the difference
between blocks (for Logical Block Addressing), sectors (storage unit for SD cards
, usually 512MB) and clusters (used in FAT filesystem, it's a grouping of sectors),
led to some issues. For instance, there was a moment where we thought the
sd_stream_bmp_to_screen function was no longer working when in fact we had just
passed the wrong argument to it (an address instead of a logical block address).
That was mainly due to an improper separation of concerns and separation of layers.

## References

- [How to Use MMC/SDC - Elm-Chan](https://elm-chan.org/docs/mmc/mmc_e.html)
- [Microsoft FAT Specification](https://academy.cba.mit.edu/classes/networking_communications/SD/FAT.pdf)
- [SD Physical Layer Simplified Specification (Download page)](https://www.sdcard.org/downloads/pls/)
- [FAT Filesystem - Elm-Chan](https://elm-chan.org/docs/fat_e.html)
- [Structure du MBR - Wikipedia](https://fr.wikipedia.org/wiki/Master_boot_record#Structure_du_MBR)
- [List of partitions ID - Wikipedia](https://en.wikipedia.org/wiki/Partition_type#List_of_partition_IDs)
- [Boot Sector - Wikipedia](https://en.wikipedia.org/wiki/Design_of_the_FAT_file_system#Boot_Sector)
