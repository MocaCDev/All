#ifndef EMU_MAIN
#define EMU_MAIN
#include "../common.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "disassembler/disassemble.h"
#include "hardware/hardware.h"
#include "hardware/registers.h"

/*
 *  Window height/width information. Default is 750x500.
 * */
#define WINDOW_WIDTH        750
#define WINDOW_HEIGHT       500

/*
 *  Fonts able to be used by emulator.
 * */
#define DEFAULT_FONT        (UNS8 *)"font.ttf"
#define ITALIC_FONT         (UNS8 *)"italic_font.ttf"

/*
 *  Standard ideals for emulator. The window, the renderer, the font.
 *
 *  All set to NULL. window + renderer will be setup in the main function of
 *  emu_main.c.
 *  The font will be used when the emulator is running. Users can set
 *  the font type/width via assembly. By default it will be assigned to
 *  DEFAULT_FONT with a font size of 24.
 * */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static TTF_Font *font = NULL;

/*
 *  Information over the font to be used.
 * 
 *  Emulator excpects:
 *  
 *  16bit: 
 *      ax:
 *          First 8 bits = Type of font(0x01 = normal, 0x02 = italic)
 *          Second 8 bits = Font Size.
 *          Example: mov ax, 0x010F
 *              01 = Normal font
 *              0F = Font size of 15
 *  32bit:
 *      eax:
 *          First 16 bits = Type of font(0x0001 = norma, 0x0002 = italic)
 *          Second 16 bits = font size.
 *          Example: mov eax, 0x0001000F
 *              0001 = Normal font
 *              000F = Font size of 15
 * 
 *  FontInfo16bit -> Information for font from 16bit register(ax)
 *  FontInfo32bit -> Information for font from 32bit register(eax)
 * */
typedef struct FontInfo16bit {
    UNS8        font_type;
    UNS8        font_size;
} _FontInfo16bit;
typedef struct FontInfo32bit {
    UNS16       font_type;
    UNS16       font_size;
} _FontInfo32bit;

/*
 *
 *  FONT_INFO_16bit -> Takes 16bit register(ax). Example:
 *      FONT_INFO_16bit(0x010F) ->
 *          .font_type = 0x01(normal)
 *          .font_size = 0x0F(15)
 *  FONT_INFO_32bit -> Takes 32bit register(eax). Example:
 *      FONT_INFO_32bit(0x0001000F) ->
 *          .font_type = 0x0001(normal)
 *          .font_size = 0x000F(15)
 * */
#define FONT_INFO_16bit(reg)        \
    (_FontInfo16bit) {              \
        .font_type = reg >> 8,      \
        .font_size = reg & 0xFF     \
    };
#define FONT_INFO_32bit(reg)        \
    (_FontInfo32bit) {              \
        .font_type = reg >> 16,     \
        .font_size = reg & 0xFFFF   \
    };

#define DESTORY_SURFACE(surface)    \
    if(!(surface == NULL)) {        \
        SDL_FreeSurface((SDL_Surface *)surface);   \
    }

void run_emulator();

#endif