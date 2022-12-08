#ifndef BIOS_EMULATOR
#define BIOS_EMULATOR
#include "BIOS.h"
#include "../../common.h"

// Boot signature that the BIOS expects to be at the 510th/511th byte.
#define BOOT_SECT_SIG       0x55AA

/* 
 * Size of the virtual "disk" the emulator will use.
 * This helps us "allocate" X amount of "sectors", which in turn
 * is just marking 512 * x amount of indexes.
 * This value does not include the boot sector program.
 * Disk size is 6MB(13 sectors total, 512 * 13 = 6,656 bytes)
 * @TODO: Change the size. A first stage/secondstage bootloader might
 *        not fit in the given size.
 *        AS of right now, this size works for testing purposes only.
 * */
#define VIRTUAL_DISK_SIZE   512*13

/* 
 * Size of the "sectors" on the virtual disk image.
 * Same as an actual floppy disk/hard disk. The sector size is
 * 512 bytes per sector.
 * */
#define BYTES_PER_SECTOR    512

/* 
 * The virtual "disk" image.
 * Think of this UNS8 array as a virtual hard disk that the emulator
 * uses to mimic an actual hard drive.
 * This "disk" image will be partitioned to 512 byte chuncks,
 * also known as "sectors". This array is statically allocated
 * and only exists in the emulator. We copy the data from the initial
 * device(boot sector program) onto this "disk" image, so that way
 * we have easy access to all the data. This also helps for when the
 * user invokes `0x13`, AH = 0x02. We can "reserve" x amount of "sectors",
 * or, 512-byte chunks, within this UNS8 array and fill them out with the
 * appropriate data.
 * */
static UNS8     *disk_image;

/*
 *  Emulation of the BIOS and all it's functionality dwell here.
 *
 *  check_boot_sig:
 *      @param device       device of boot sector program(FILE*)
 * */
void check_boot_sig(FILE *device);


#endif