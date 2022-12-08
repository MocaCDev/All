__asm__(".code16gcc\n");

__attribute__((section("__start"))) void main()
{
    unsigned char *vid_mem = (unsigned char *)0xB8000;

    *vid_mem = 'a';
    vid_mem+=2;
}