ENTRY(main)
SECTIONS
{
    . = 0x7C00;
    .text : AT(0x7C00)
    {
        *(__start*);
        *(.text*);
    }

    .data :
    {
        *(.bss*);
        *(.data*);
        *(.rodata*);
        *(COMMON*);
    }

    .sig : AT(0x7DFE)
    {
        SHORT(0xaa55);
    }
}