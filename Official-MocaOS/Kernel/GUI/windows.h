#ifndef MocaWindows
#define MocaWindows
#include "widgets.h"

/*
 *  All information for Window GUI generation.
 * */

// What are we generating pixels for? Primary window or secondary window?
// If primary, we are generating the actual window for the main window.
// If secondary, we are generating ideals within the window.
enum WindowType
{
    Primary,
    Secondary
};

// Modes of windows:
//  Invisible - This window is stored in secondary framebuffer and will be loaded on a ContextSwitch.
//  Visible - This window is generated when the window frame is generated.
//  Background - The window frame/window content generates in the background.
enum WindowModes
{
    Invisible,
    Visible,
    Background
};

// Secondary window-frame information
typedef struct SecondaryWindowFrame
{
    uint32              frame_width;
    uint32              frame_height;

    // The framebuffer for the frame.
    uint32              *SWF_framebuffer;
} _SecondaryWindowFrame;

// Window metadata(what is generated inside the window?)
typedef struct SecondaryWindow_Meta
{
    // Mode of the secondary window. This tells us how to use the secondary_frame framebuffer.
    enum WindowModes            secondary_mode;

    // Framebuffer for secondary window.
    _SecondaryWindowFrame       secondary_frame;

    // Secondary window title.
    uint8                       *secondary_title;

    // Framebuffer for inside the window frame.
    uint32                      *secondary_framebuffer;
} _SecondaryWindow_Meta;

// Popup window.
enum PopupWindowType
{
    Inform,
    Warn,
    Error
};

typedef struct PopupWindow
{
    // Popup window width/height.
    uint32                  PUPW_width;
    uint32                  PUPW_height;

    // Popup window title.
    uint8                   *PUPW_title;
    
    // Popup window type.
    enum PopupWindowType    PUPW_type;

    // Popup window framebuffer.
    uint32                  *PUPW_framebuffer;
} _PopupWindow;

// Main window, which will include information for other windows/applications
typedef struct MainWindow
{
    // Main window width/height(should be width/height of emulator).
    uint32                  MW_width;
    uint32                  MW_height;

    // Primary windows. (MW_SW = MainWindow_SecondaryWindows)
    _SecondaryWindow_Meta   **MW_SW;

    // Main window framebuffer.
    uint32                  *framebuffer;

    // Where can secondary windows begin, and where do they end?
    uint32                  SW_begin_spotX;
    uint32                  SW_begin_spotY;
    uint32                  SW_end_spotX;
    uint32                  SW_end_spotY;
} _MainWindow;

#endif