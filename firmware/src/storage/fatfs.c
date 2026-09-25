#include "storage/fatfs.h"

#include "mem_utils.h"
#include "storage/sd.h"
#include "uart.h"
#include "utils.h"


// DSTATUS bit flags
#define STA_NOINIT 0x01   //< Drive not initialized
#define STA_NODISK 0x02   //< No medium in the drive
#define STA_PROTECT 0x04  //< Write protected

// NOTE: You will see `__attribute__((packed))` a lot here, it is because it
// allows to serialise data easily in order to copy it ! :)

// --- MASTER BOOT RECORD

// NOTE: To learn about partition type IDs, see:
// https://en.wikipedia.org/wiki/Partition_type#List_of_partition_IDs
typedef struct __attribute__((packed)) {
  uint8_t status;        //<
  uint8_t chs_start[3];  //< CHS is for hard disks, so ignore
  uint8_t type;  //< SD card uses Logic Block Addressing (LBA) so we expect 0x0C
  uint8_t chs_end[3];     //< CHS is for hard disks, so ignore
  uint32_t lba_start;     //< Beginning of LBA
  uint32_t size_sectors;  //< Numbers of sectors in partition
} mbr_partition_entry;

// NOTE: Master Boot Record (MBR) sector layout:
// https://fr.wikipedia.org/wiki/Master_boot_record#Structure_du_MBR
typedef struct __attribute__((packed)) {
  uint8_t bootstrap[440];             //< Between 440 and 446, allegedly (???)
  uint32_t disk_signature;            //< Optional
  uint16_t reserved;                  //< Is generally null
  mbr_partition_entry partitions[4];  //< Partitions table (important)
  uint16_t boot_signature;            //< Must be 0xAA55 for FAT32
} mbr;

// --- VOLUME BOOT RECORD

// NOTE: Want to know how to recreate your own Volume Boot Record that is as
// long as a lorem ipsum ? Check Elm-Chan's page !:
// https://elm-chan.org/docs/fat_e.html
typedef struct __attribute__((packed)) {
  uint8_t jump[3];              //< Jump instructions to bootstrap code
  uint8_t oem[8];               //< Original Equipment Manufacturer (OEM) name
  uint16_t bytes_per_sector;    //< Sector size in bytes
  uint8_t sectors_per_cluster;  //< Number of sectors per cluster
  uint16_t reserved_sectors;    //< Numbers of sectors in reserved area
  uint8_t num_fats;             //< Number of FAT copies, usually 2
  uint16_t root_entry_count;    //< Number of directories at root, 0 for FAT32
  uint16_t
      total_sectors_16;  //< Volume size, 0 for FAT32 (see total_sectors_32)
  uint8_t media_type;    //> Media descriptor byte
  uint16_t fat_size_16;  //< 0 for FAT32
  uint16_t sectors_per_track;  //< Number of sectors per track
  uint16_t num_heads;          //< Number of heads
  uint32_t hidden_sectors;  //< Number of hidden physical sectors preceding FAT
                            // volume
  uint32_t total_sectors_32;  //< Volume size, total number of sectors of the
                              // FAT volume
  // FAT32 extended
  uint32_t fat_size_32;    //< Size of a FAT in unit of sectors
  uint16_t flags;          //< See Elm-Chan's page
  uint16_t fat_version;    //< FAT32 version
  uint32_t root_cluster;   //< First cluster number of the root directory, start
                           // here to find files !!!!
  uint16_t fsinfo_sector;  //< Sector of FSInfo structure in offset from top of
                           // the FAT32 volume
  uint16_t backup_boot_sector;  //<Sector of backup boot sector in offset from
                                // top of the FAT32 volume
  uint8_t reserved[12];         //< Reserved, usually null
  uint8_t drive_number;         //<Same as the description of FAT12/16 field.
  uint8_t reserved2;            //<Same as the description of FAT12/16 field.
  uint8_t boot_signature;       //<Same as the description of FAT12/16 field.
  uint32_t volume_serial;       //<Same as the description of FAT12/16 field.
  uint8_t volume_label[11];     //<Same as the description of FAT12/16 field.
  uint8_t fs_type[8];           //< Always "FAT32   "
  // There are two more fields (Bootstrap and signature, for RAM economy
  // purposes, we'll skip on those)
} vbr;

// --- FAT32 THINGS

// NOTE: See page 23 of the FAT specification
typedef struct __attribute__((packed)) {
  uint8_t
      name[11];  //< 8 chars for name, 3 chars for extension (e.g. "IMAGE BMP")
  uint8_t attr;  //< File attributes (read/write,hidden,system,etc...)
  uint8_t reserved_nt;           //< Reserved, must be null
  uint8_t creation_time_tenths;  //< File creation time component, tenths of a
                                 // second
  uint16_t creation_time;        //< Creation time (granularity of 2 seconds)
  uint16_t creation_date;        //< Creation date
  uint16_t last_access_date;     //< Last access date
  uint16_t first_cluster_high;   //< High word of first data cluster number for
                                 // file/directory described by this entry.
  uint16_t last_write_time;      //< Last modification time
  uint16_t last_write_date;      //< Last modification date
  uint16_t first_cluster_low;    //< Low word of first data cluster number for
                                 // file/directory described by this entry.
  uint32_t file_size;  //< File size of this entry in bytes (IN BYTES, DON'T BE
                       // A FOOL)
} fat32_dir_entry;

static DSTATUS sd_card_status = STA_NOINIT;
static mbr sd_mbr = {0};
static vbr sd_vbr;
static uint32_t fat_start_lba = 0;
static uint32_t data_start_lba = 0;
img image_lut[IMG_LUT_MAX_SIZE] = {0};

DSTATUS disk_status(BYTE pdrv) {
  (void)pdrv;
  return sd_card_status;
}

DSTATUS disk_initialize(BYTE pdrv) {
  (void)pdrv;
  if (sd_init() != 0) {
    sd_card_status = STA_NOINIT;
    return STA_NOINIT;
  }
  sd_card_status = 0;
  return 0;
}

DRESULT disk_read(BYTE pdrv, BYTE* buff, LBA_t sector, UINT count) {
  (void)pdrv;
  if (sd_card_status != 0) {
    return RES_NOTRDY;
  }

  if (count == 1) {
    sd_read_single_block((sector >> 24) & 0xFF, (sector >> 16) & 0xFF,
                         (sector >> 8) & 0xFF, sector & 0xFF, buff);
  } else {
    sd_read_multiple_block_start((sector >> 24) & 0xFF, (sector >> 16) & 0xFF,
                                 (sector >> 8) & 0xFF, sector & 0xFF);
    for (UINT i = 0; i < count; i++) {
      sd_read_multiple_block_next(
          buff + i * 512);  // Offsets the buffer for each block
    }
    sd_read_multiple_block_stop();
  }
  return RES_OK;
}

DRESULT parse_vbr(void) {
  if (sd_card_status != 0) {
    return RES_NOTRDY;
  }

  disk_read(0, (BYTE*)&sd_vbr, sd_mbr.partitions[0].lba_start, 1);

  fat_start_lba = sd_mbr.partitions[0].lba_start + sd_vbr.reserved_sectors;
  data_start_lba = fat_start_lba + sd_vbr.num_fats * sd_vbr.fat_size_32;

  return 0;
}

DRESULT parse_mbr(void) {
  if (sd_card_status != 0) return RES_NOTRDY;

  disk_read(0, (BYTE*)&sd_mbr, 0, 1);

  if (sd_mbr.boot_signature != 0xAA55) {
    uart_printstr("Bad Boot Signature: ");
    uart_printhex(sd_mbr.boot_signature >> 8);
    uart_printhex(sd_mbr.boot_signature & 0xFF);
    uart_printstr("\r\n");
    return RES_PARERR;
  }

  if (sd_mbr.partitions[0].type != 0x0C && sd_mbr.partitions[0].type != 0x0B) {
    uart_printstr("Bad Part Type: ");
    uart_printhex(sd_mbr.partitions[0].type);
    uart_printstr("\r\n");
    return RES_PARERR;
  }
  return 0;
}

uint32_t cluster_to_lba(uint32_t cluster) {
  // FAT32 data clusters always start at index 2
  return data_start_lba + ((cluster - 2) * sd_vbr.sectors_per_cluster);
}

void scan_root_dir(void) {
  fat32_dir_entry sd_dir_entries[16];
  uint32_t start_lba = cluster_to_lba(sd_vbr.root_cluster);

  uart_printstr("Scanning for images on SD Card:\r\n");

  for (uint8_t sector = 0; sector < sd_vbr.sectors_per_cluster; sector++) {
    disk_read(0, (BYTE*)&sd_dir_entries, start_lba + sector, 1);

    for (uint8_t i = 0; i < 16; i++) {
      // 0x00 means directory is empty from here on
      if (sd_dir_entries[i].name[0] == 0x00) {
        uart_printstr("Directory has no more files\r\n");
        return;
      }

      // 0xE5 means file was deleted, 0x0F means it's an Long File Name (LFN) to
      // ignore
      if (sd_dir_entries[i].name[0] == 0xE5 || sd_dir_entries[i].attr == 0x0F)
        continue;

      if (sd_dir_entries[i].name[8] != 'B' ||
          sd_dir_entries[i].name[9] != 'M' ||
          sd_dir_entries[i].name[10] != 'P') {
        uart_printstr("File is not BMP\r\n");
        continue;
      }

      // Print the 11-character name
      for (uint8_t j = 0; j < 11; j++) {
        uart_tx(sd_dir_entries[i].name[j]);
      }
      uart_printstr(" is located at Cluster: ");

      // Combine high and low 16-bit values into a 32-bit cluster number
      uint32_t file_cluster =
          ((uint32_t)sd_dir_entries[i].first_cluster_high << 16) |
          sd_dir_entries[i].first_cluster_low;

      uart_printhex_32(file_cluster);
      uart_printstr("\r\n");

      static uint8_t image_count = 0;
      if (image_count < 60) {
        image_lut[image_count].address = file_cluster;
        ft_memcpy(image_lut[image_count].name, sd_dir_entries[i].name, 11);
        image_count++;
      } else {
        uart_printstr("Image look up table is full :(\r\n");
      }
    }
  }
  uart_printstr("Finished scanning for images on SD Card:\r\n");
}
