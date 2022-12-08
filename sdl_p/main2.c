#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned char boot[512*14];

// SOB -> Size Of Bite
#define SOB     (sizeof(unsigned char))
#define COM_ASSERT(condition, err_msg, ...)     \
    if(!condition) {                            \
        fprintf(stderr, err_msg, ##__VA_ARGS__);\
        exit(1);                                \
    }

void copy_to_virtual_disk(FILE *d, unsigned int size)
{
    fread(boot, size, SOB, d);

    COM_ASSERT(
        boot[510] == 0x55, 
        "Reading disk image over to virtual disk failed."
    );
}

int main() {
    memset(boot, 0x0, 512*14);

    FILE* d = fopen("t.bin", "rb");

    copy_to_virtual_disk(d, 512);

    //boot = calloc(0x7C00, sizeof(*boot));

    /*memset(boot, 1, 0x7C00);

    boot[0x7c00+510] = 0x55;
    boot[0x7C00+511] = 0xAA;

    FILE* f = fopen("test", "wb");

    fwrite(boot, 1, sizeof(boot), f);
    fclose(f);

    FILE *d = fopen("test", "rb");

    fseek(d, 0x7C00+510, SEEK_SET);

    unsigned char a[2];
    size_t b = fread(a, sizeof(unsigned char), 2, d);

    fclose(d);
    printf("%X", a[1]);*/

    return 0;
}