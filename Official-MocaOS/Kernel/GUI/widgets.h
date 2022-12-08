#ifndef MocaWidgets
#define MocaWidgets
#include "../util.h"

/*
 *  Data over general widgets(buttons, text etc).
 * */

// Text widget.
typedef struct TextWidget
{
    uint8       *text;

    // Size of the text.
    uint32      text_width;
    uint32      text_height;

    // Position of the text
    uint32      text_xpos;
    uint32      text_ypos;

    // Text widget framebuffer.
    uint32      *TW_framebuffer;
} _TextWidget;

// Button click types.
enum ButtonClickType
{
    Popup,
    ContentSwitch, // change content on window.
    // Core buttons.
    WindowClose,
    WindowMinimize,
    WindowMaximize
};

// Button widget.
typedef struct ButtonWidget
{
    // The text will auto-fit to the size of the button.
    uint8                   *button_text;

    // Size of the button.
    uint32                  button_height;
    uint32                  button_width;

    // Position of the button.
    uint32                  button_xpos;
    uint32                  button_ypos;

    // Button click type.
    enum ButtonClickType    click_type;

    // Functionality for button clicks. Dependable on the click type, that will decipher which function we call internally.
    void                    (*button_click_popup)(struct ButtonWidget button);
    void                    (*button_click_CS)(struct ButtonWidget button);
    void                    (*button_click_WC)(struct ButtonWidget button);
    void                    (*button_click_WMIN)(struct ButtonWidget button);
    void                    (*button_click_WMAX)(struct ButtonWidget button);

    // Button widget framebuffer.
    uint32                  *BW_framebuffer;
} _ButtonWidget;

#endif