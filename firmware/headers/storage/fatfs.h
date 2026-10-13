#ifndef FATFS_H
#define FATFS_H

#include "structs.h"

// NOTE: FAT specification here:
// https://academy.cba.mit.edu/classes/networking_communications/SD/FAT.pdf

// NOTE: Guide on how to port FatFS by Elm-Chan:
// https://elm-chan.org/fsw/ff/doc/appnote.html#port

// NOTE: Want to learn about FAT ? :)
// https://en.wikipedia.org/wiki/Design_of_the_FAT_file_system#Boot_Sector

#define MBR_SIZE 512
#define PART_ENTRY_SIZE 16
#define BOOT_SIGNATURE 0x01FE
#define BOOT_SIGNATURE_SIZE 2
#define IMG_LUT_MAX_SIZE 146

// extern img image_lut[IMG_LUT_MAX_SIZE];

extern img image_lut[IMG_LUT_MAX_SIZE];

// Type definitions
typedef uint8_t BYTE;    //< 8-bit unsigned integer in range of 0 to 2^8 - 1.
typedef uint16_t WORD;   //< 16-bit unsigned integer in range of 0 to 2^16 - 1.
typedef uint32_t DWORD;  //< 32-bit unsigned integer in range of 0 to 2^32 - 1.
typedef uint64_t QWORD;  //< 64-bit unsigned integer in range of 0 to 2^64 - 1.
typedef unsigned int
    UINT;               //< Alias of unsigned int used to specify any number.
typedef WORD WCHAR;     //< Alias of WORD used to specify a UTF-16 code unit.
typedef WCHAR TCHAR;    //< Alias of char, WCHAR or DWORD used to specify a
                        // character encoding unit.
typedef DWORD FSIZE_t;  //< Alias of DWORD or QWORD used to address file offset
                        // and to specify file size.
typedef QWORD LBA_t;  //< Alias of DWORD or QWORD used to address sectors in LBA
                      // and to specify number of sectors.
typedef BYTE DSTATUS;  //< Alias of BYTE used to specify a status.

typedef enum
{
  RES_OK = 0,  //< 0: Successful
  RES_ERROR,   //< 1: R/W Error
  RES_WRPRT,   //< 2: Write Protected
  RES_NOTRDY,  //< 3: Not Ready
  RES_PARERR   //< 4: Invalid Parameter
} DRESULT;

// WARNING: Those are the locations of the partition entries field inside the
// MBR, not the place they point to !!!
typedef enum
{
  PART_ENTRY_NO_1 = 0x01BE,
  PART_ENTRY_NO_2 = 0x01CE,
  PART_ENTRY_NO_3 = 0x01DE,
  PART_ENTRY_NO_4 = 0x01EE,
} PARTITION_ENTRY_NO;

/**
 * @brief Iinitialises the storage device.
 *
 * @param pdrv Physical drive number to identify the target device. Always zero
 * at single drive system.
 * @return This function returns the current drive status flags as the result.
 * For details of the drive status, refer to the disk_status function.
 */
DSTATUS disk_initialize(BYTE pdrv);

/**
 * @brief Reads data from the storage device.
 *
 * @param pdrv Physical drive number to identify the target device.
 * @param buff Pointer to the first item of the byte array to store read data.
 * Size of read data will be the sector size * count bytes.
 * @param sector Start sector number in LBA. The data type LBA_t is an alias of
 * DWORD or QWORD depends on the configuration option.
 * @param count Number of sectors to read.
 * @return
 * RES_OK (0)
 * The function succeeded.
 * RES_ERROR
 * An unrecoverable hard error occured during the read operation.
 * RES_PARERR
 * Invalid parameter.
 * RES_NOTRDY
 * The device has not been initialized.
 */
DRESULT disk_read(BYTE pdrv, BYTE* buff, LBA_t sector, UINT count);

DRESULT parse_mbr(void);
DRESULT parse_vbr(void);
void scan_root_dir(void);
uint32_t cluster_to_lba(uint32_t cluster);

#endif /* ifndef FATFS_H */
