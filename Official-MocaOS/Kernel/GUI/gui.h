#ifndef MocaGUI
#define MocaGUI
#include "windows.h"

#define SECONDARY_FRAMEBUFFER       (uint32 *)0x22000

// Initialize the main window(the "window" that covers the entirity of the qemu emulator screen.)
void init_main_window()
{
    MProcess main_window = m_alloc(1);

    // Window
    _MainWindow *MW = (_MainWindow *) main_window.address;

    // Assignments.
    MW->MW_height = height;
    MW->MW_width = width;
    MW->SW_begin_spotX = 1;
    MW->SW_begin_spotY = 1;

    // Setting points to start generation of secondary windows.
    MW->SW_end_spotX = MW->MW_width- 1;
    MW->SW_end_spotY = MW->MW_height - 1;

    // Main Window Framebuffer.
    MW->framebuffer = (uint32 *)FB;

    // Completely clear framebuffer.
    for(uint32 i = 0; i < MW->MW_height; i++)
    {
        for(uint32 x = 0; x < MW->MW_width; x++)
        {
            *MW->framebuffer = MakeColor(98, 47, 34);
            MW->framebuffer++;
        }
    }
}

void plot_pixel(uint32 x, uint32 y, uint32 color, uint32 *fb)
{
    //uint32 FB_INDEX = x + (pitch / sizeof(uint32)) * y;

    fb[(x + pitch)*y] = color;
}

void generate_text(uint8 *text, uint32 tw, uint32 th, uint32 xpos, uint32 ypos)
{
    _TextWidget text_widget;

    // Font
    uint8 *font = (uint8 *)FONT;

    text_widget.text = text;
    text_widget.text_height = th;
    text_widget.text_width = tw;
    text_widget.text_xpos = xpos;
    text_widget.text_ypos = ypos;

    text_widget.TW_framebuffer = SECONDARY_FRAMEBUFFER;

    uint32 *fb = (uint32 *)FB;
    uint32 orig_x = xpos;
    uint8 c = 'h';

    //Print(text, WHITE, BLACK);

    for(uint32 i = 0; i < 16; i++)
    {
        for(uint32 j = 0; j < 7; j++)
        {
            if((font[c * 16 + i] >> ((7) - i)) & 1)
                plot_pixel(xpos++, ypos, WHITE, fb);
            //else
            //    plot_pixel(xpos++, ypos, MakeColor(98, 47, 34), fb);
        }
        ypos++;
        //xpos = orig_x;
    }
}

#endif