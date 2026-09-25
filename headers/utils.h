#ifndef UTILS_H
#define UTILS_H

#include <avr/io.h>

#include "storage/fatfs.h"

typedef struct __attribute__((packed)) {
  uint8_t status;
  uint8_t chs_start[3];  // ignore
  uint8_t type;
  uint8_t chs_end[3];  // ignore
  uint32_t lba_start;  // Logic Block Addressing
  uint32_t size_sectors;
} mbr_partition_entry;

typedef struct __attribute__((packed)) {
  uint8_t bootstrap[440];
  uint32_t disk_signature;
  uint16_t reserved;
  mbr_partition_entry partitions[4];
  uint16_t boot_signature;  // must be 0xAA55
} mbr;

typedef struct __attribute__((packed)) {
  uint8_t jump[3];
  uint8_t oem[8];
  uint16_t bytes_per_sector;
  uint8_t sectors_per_cluster;
  uint16_t reserved_sectors;
  uint8_t num_fats;
  uint16_t root_entry_count;  //< 0 for FAT32
  uint16_t total_sectors_16;  //< 0 for FAT32
  uint8_t media_type;
  uint16_t fat_size_16;  //< 0 for FAT32
  uint16_t sectors_per_track;
  uint16_t num_heads;
  uint32_t hidden_sectors;
  uint32_t total_sectors_32;
  // FAT32 extended
  uint32_t fat_size_32;
  uint16_t flags;
  uint16_t fat_version;
  uint32_t root_cluster;  //< start here to find files
  uint16_t fsinfo_sector;
  uint16_t backup_boot_sector;
  uint8_t reserved[12];
  uint8_t drive_number;
  uint8_t reserved2;
  uint8_t boot_signature;
  uint32_t volume_serial;
  uint8_t volume_label[11];
  uint8_t fs_type[8];
} vbr;

typedef struct __attribute__((packed)) {
  uint8_t
      name[11];  //< 8 chars for name, 3 chars for extension (e.g. "IMAGE BMP")
  uint8_t attr;
  uint8_t reserved_nt;
  uint8_t creation_time_tenths;
  uint16_t creation_time;
  uint16_t creation_date;
  uint16_t last_access_date;
  uint16_t first_cluster_high;  //< High 16 bits of the cluster
  uint16_t last_write_time;
  uint16_t last_write_date;
  uint16_t first_cluster_low;  //< Low 16 bits of the cluster
  uint32_t file_size;  //< File size in bytes (IN BYTES, DON'T BE A FOOL)
} fat32_dir_entry;

void dump_buf(uint8_t* buf, uint16_t len);

void dump_mbr(void);
void dump_partition_entry(PARTITION_ENTRY_NO entry_no);

#endif  // !UTILS_H
