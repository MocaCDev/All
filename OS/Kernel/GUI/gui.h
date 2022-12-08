#ifndef MocaGUI
#define MocaGUI
#include "../memory.h"
#include "../mstdio.h"

#define ANTIBUFFER              0x20000
 
typedef struct FramebufferInfo
{
        uint32 *framebuffer;
        uint32 *antibuffer;
} _FB_INFO;

enum WINDOW_TYPE {
        main_wind, // main_wind is the actual screen of the OS. the "desktop". This cannot be used by others, only by the kernel.
        primary,   // primary windows cover whole screen. So when the application launches the whole screen should be taken up.
        secondary, // secondary windows require the window to be given a width and height.
        backgrnd,  // backgrnd means the application won't open up over other windows.
};
 
typedef struct Window {
        enum WINDOW_TYPE window_type;
        uint8   *title;
        uint32  id;
        uint32  x;
        uint32  y;
        uint32  x_size;
        uint32  y_size;
        uint32  *buffer;
        struct Window *next;
} _Window;
 
typedef struct buttons {
        uint8           *button_title;
        uint32          button_id;
        // Might not need?
        /*
         * is_activated:
         *      Set to zero if button is clicked, by default set to 1.
         *      (TRUE = 0, FALSE = 1)
         * */
        bool            is_activated;
        struct buttons  *next_button;
} _Buttons;
 
static _Window *windows;
static _Window *current_window;
static _FB_INFO fb_info;
static _Buttons *top_bar_buttons;
static _Buttons *current_button;
static uint32   top_bar_buttons_amnt = 0;

#define CONFIGURE_WINDOW(type_, title_, id_, x_, y_, x_size_, y_size_, buff) \
        current_window->window_type = type_;                   \
        current_window->title = (uint8 *)title_;               \ 
        current_window->id = id_;                              \
        current_window->x = x_;                                \
        current_window->y = y_;                                \
        current_window->x_size = x_size_;                      \
        current_window->y_size = y_size_;                      \
        current_window->buffer = (uint32 *)buff;               \
        current_window->next = (_Window *)moca_malloc(1);      \
        current_window = windows->next;
#define RESET_FB \
        fb_info.framebuffer = (uint32 *) FB
#define PADDING         2
 
void plot_px(uint32 x, uint32 y, uint32 color)
{
        uint32 fb_i = x + (pitch / sizeof(uint32)) * y;
        fb_info.framebuffer[fb_i] = color;
}
 
void plot_char(uint8 c, uint32 x, uint32 y, uint32 fg, uint32 bg)
{
        if(y == 0)
                y = 1;
        uint32 orig_x = x;
        uint8 *font = (uint8 *)FONT;
 
        for(uint32 i = 0; i < 16; i++)
        {
                for(uint32 j = 0; j < 7; j++)
                {
                        //for(uint8 d = 0; d < 2; d++)
                        //{
                                if((font[c * 16 + i] >> (7 - j)) & 1)
                                        plot_px(x++, y, fg);
                                else
                                        plot_px(x++, y, bg);
                                //y++;
                                //x++;
                        //}
                }
                y++;
                x = orig_x;
        }
        RESET_FB;
}
 
void vertical_line(uint32 length, uint32 x, uint32 y, uint32 color)
{
        for(uint32 i = 0; i < length; i++)
                plot_px(x, y++, color);
}

uint32 generate_button(uint8 *text, uint32 X_POS)
{
        for(uint8 i = 0; ;i++)
        {
                plot_char(text[i], X_POS, 0, BLACK, WHITE);
                X_POS+=8;
                if(text[i] == 0x0 || text[i] == '\0') break;
        }
        X_POS-=12;
        vertical_line(20, X_POS, 0, BLACK);

        X_POS++;

        return X_POS;
}
 
void generate_top_bar()
{
        uint32 color = 0x0;
        uint32 x = 32;
        uint32 y = 32;
        uint32 x1 = 0;
 
        /*for(uint32 x = 0; x < HEIGHT; x++)
                for(uint32 y = 0; y < WIDTH; y++)
                {
                        if(x < 20)
                                color = WHITE;
                        else
                                color = BLACK;
                        *fb_info.framebuffer = color;
                        fb_info.framebuffer++;
                }*/
        
        for(uint32 x = 0; x < 20; x++)
        {
                for(uint32 y = 0; y < WIDTH; y++)
                {
                        //*fb_info.framebuffer = WHITE;
                        //fb_info.framebuffer++;
                        *fb_info.antibuffer = WHITE;
                        fb_info.antibuffer++;
                }
                //fb_info.framebuffer++;
        }

        *fb_info.antibuffer = 0x0;
        fb_info.antibuffer = (uint32 *)ANTIBUFFER;

        while(1) {
                if(*fb_info.antibuffer == 0x0) break;

                *fb_info.framebuffer = *fb_info.antibuffer;
                fb_info.framebuffer++;
                fb_info.antibuffer++;
        }
        RESET_FB;

        /*int total = 0;
        while(1) {
                if(*fb_info.antibuffer == 0x0) break;
                
                total++;
                fb_info.antibuffer++;
        }
        PrintHex((uint32) total);*/
        //RESET_FB;
 
        uint8 a[5+(PADDING*2)] = "  HOME  ";
        for(uint8 i = 0; i < SIZEOF(a); i++)
        {
                plot_char(a[i], x1, 0, BLACK, WHITE);
                x1+=8;
        }
        x1-=8;
        vertical_line(20, x1, 0, BLACK);
        x1++;
        RESET_FB;

        uint8 b[8+(PADDING*2)] = "  SETTINGS  ";
        x1 = generate_button(b, x1);
        RESET_FB;

        top_bar_buttons = (_Buttons *)moca_malloc(1);

        top_bar_buttons->button_title = (uint8 *)"home";
        top_bar_buttons->button_id    = top_bar_buttons_amnt;
        top_bar_buttons->is_activated = FALSE;
        top_bar_buttons->next_button = moca_malloc(1);
        current_button = top_bar_buttons->next_button;
        top_bar_buttons_amnt++;

        // TODO: complete this lol
        current_button->button_title = (uint8 *)"settings";
        current_button->button_id = top_bar_buttons_amnt;
        current_button->next_button = moca_malloc(1);
        top_bar_buttons_amnt++;
        //current_button = current_button->next_button;

        current_button = top_bar_buttons;

        int x2 = 20;
        int y2 = 20;

        for(uint32 i = 0; i < top_bar_buttons_amnt; i++)
        {
                for(uint32 x = 0; ; x++)
                {
                        if(current_button->button_title[x] == 0x0) break;

                        plot_char(current_button->button_title[x], x2, y2, WHITE, BLACK);
                        x2+=8;
                }
                current_button = top_bar_buttons->next_button;
        }
}

void generate_desktop() {
        windows = (_Window *)moca_malloc(1);
        current_window = windows;

        for(uint32 x = 0; x < HEIGHT; x++)
                for(uint32 y = 0; y < WIDTH; y++)
                {
                        //*fb_info.antibuffer = BLACK;
                        //fb_info.antibuffer++;
                        *fb_info.framebuffer = BLACK;
                        fb_info.framebuffer++;
                }
        
       // RESET_FB;
        CONFIGURE_WINDOW(main_wind, (uint8 *)"Desktop", 0, WIDTH, HEIGHT, 0, 0, fb_info.framebuffer);
        //Print((uint8 *)"HEY", WHITE, BLACK);
        //__asm__ __volatile__ ("hlt");
        
        RESET_FB;
        generate_top_bar();
        RESET_FB;

        for(uint32 x = 0; x < WIDTH/2; x++) {
                fb_info.framebuffer++;
        }
        for(uint32 x = 0; x < HEIGHT; x++) {
                if(*fb_info.framebuffer == WHITE)
                        *fb_info.framebuffer = WHITE;
                else
                        *fb_info.framebuffer = BLUE;
                fb_info.framebuffer += WIDTH;
        }
        RESET_FB;
        for(uint32 x = 0; x < WIDTH*(HEIGHT/2); x++)
                fb_info.framebuffer++;
        for(uint32 x = 0; x < WIDTH; x++) {
                *fb_info.framebuffer = BLUE;
                fb_info.framebuffer++;
        }
        
}

void generate_window(uint8 *title, enum WINDOW_TYPE wind_type) {
        windows->window_type = wind_type;
}
 
void refresh_screen() {
        //_Window *w = moca_alloc(1, 4);

        generate_desktop();
}
 
 
void init_GUI() {
        fb_info = (_FB_INFO) {
                .framebuffer = (uint32 *)FB,
                .antibuffer = (uint32 *)ANTIBUFFER
        };
 
        refresh_screen();
}
 
 
#endif
