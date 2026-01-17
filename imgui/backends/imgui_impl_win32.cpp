// dear HvkGui: Platform Backend for Windows (standard windows API for 32-bits AND 64-bits applications)
// This needs to be used along with a Renderer (e.g. DirectX11, OpenGL3, Vulkan..)

// Hvkplemented features:
//  [X] Platform: Clipboard support (for Win32 this is actually part of core dear HvkGui)
//  [X] Platform: Mouse support. Can discriminate Mouse/TouchScreen/Pen.
//  [X] Platform: Keyboard support. Since 1.87 we are using the io.AddKeyEvent() function. Pass HvkGuiKey values to all key functions e.g. HvkGui::IsKeyPressed(HvkGuiKey_Space). [Legacy VK_* values are obsolete since 1.87 and not supported since 1.91.5]
//  [X] Platform: Gamepad support.
//  [X] Platform: Mouse cursor shape and visibility (HvkGuiBackendFlags_HasMouseCursors). Disable with 'io.ConfigFlags |= HvkGuiConfigFlags_NoMouseCursorChange'.

// You can use unmodified HvkGui_impl_* files in your project. See examples/ folder for examples of using this.
// Prefer including the entire HvkGui/ repository into your project (either as a copy or as a submodule), and only build the backends you need.
// Learn about Dear HvkGui:
// - FAQ                  https://dearHvkGui.com/faq
// - Getting Started      https://dearHvkGui.com/getting-started
// - Documentation        https://dearHvkGui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of HvkGui.cpp

// Configuration flags to add in your Hvkconfig file:
//#define HvkGui_IMPL_WIN32_DISABLE_GAMEPAD              // Disable gamepad support. This was meaningful before <1.81 but we now load XInput dynamically so the option is now less relevant.

// CHANGELOG
// (minor and older changes stripped away, please see git history for details)
//  2025-12-03: Inputs: handle WM_IME_CHAR/WM_IME_COMPOSITION messages to support Unicode inputs on MBCS (non-Unicode) Windows. (#9099, #3653, #5961)
//  2025-10-19: Inputs: Revert previous change to allow for io.ClearInputKeys() on focus-out not losing gamepad state.
//  2025-09-23: Inputs: Minor optimization not submitting gamepad input if packet number has not changed.
//  2025-09-18: Call platform_io.ClearPlatformHandlers() on shutdown.
//  2025-04-30: Inputs: Fixed an issue where externally losing mouse capture (due to e.g. focus loss) would fail to claim it again the next subsequent click. (#8594)
//  2025-03-10: When dealing with OEM keys, use scancodes instead of translated keycodes to choose HvkGuiKey values. (#7136, #7201, #7206, #7306, #7670, #7672, #8468)
//  2025-02-18: Added HvkGuiMouseCursor_Wait and HvkGuiMouseCursor_Progress mouse cursor support.
//  2024-07-08: Inputs: Fixed HvkGuiMod_Super being mapped to VK_APPS instead of VK_LWIN||VK_RWIN. (#7768)
//  2023-10-05: Inputs: Added support for extra HvkGuiKey values: F13 to F24 function keys, app back/forward keys.
//  2023-09-25: Inputs: Synthesize key-down event on key-up for VK_SNAPSHOT / HvkGuiKey_PrintScreen as Windows doesn't emit it (same behavior as GLFW/SDL).
//  2023-09-07: Inputs: Added support for keyboard codepage conversion for when application is compiled in MBCS mode and using a non-Unicode window.
//  2023-04-19: Added HvkGui_ImplWin32_InitForOpenGL() to facilitate combining raw Win32/Winapi with OpenGL. (#3218)
//  2023-04-04: Inputs: Added support for io.AddMouseSourceEvent() to discriminate HvkGuiMouseSource_Mouse/HvkGuiMouseSource_TouchScreen/HvkGuiMouseSource_Pen. (#2702)
//  2023-02-15: Inputs: Use WM_NCMOUSEMOVE / WM_NCMOUSELEAVE to track mouse position over non-client area (e.g. OS decorations) when app is not focused. (#6045, #6162)
//  2023-02-02: Inputs: Flipping WM_MOUSEHWHEEL (horizontal mouse-wheel) value to match other backends and offer consistent horizontal scrolling direction. (#4019, #6096, #1463)
//  2022-10-11: Using 'nullptr' instead of 'NULL' as per our switch to C++11.
//  2022-09-28: Inputs: Convert WM_CHAR values with MultiByteToWideChar() when window class was registered as MBCS (not Unicode).
//  2022-09-26: Inputs: Renamed HvkGuiKey_ModXXX introduced in 1.87 to HvkGuiMod_XXX (old names still supported).
//  2022-01-26: Inputs: replaced short-lived io.AddKeyModsEvent() (added two weeks ago) with io.AddKeyEvent() using HvkGuiKey_ModXXX flags. Sorry for the confusion.
//  2021-01-20: Inputs: calling new io.AddKeyAnalogEvent() for gamepad support, instead of writing directly to io.NavInputs[].
//  2022-01-17: Inputs: calling new io.AddMousePosEvent(), io.AddMouseButtonEvent(), io.AddMouseWheelEvent() API (1.87+).
//  2022-01-17: Inputs: always update key mods next and before a key event (not in NewFrame) to fix input queue with very low framerates.
//  2022-01-12: Inputs: Update mouse inputs using WM_MOUSEMOVE/WM_MOUSELEAVE + fallback to provide it when focused but not hovered/captured. More standard and will allow us to pass it to future input queue API.
//  2022-01-12: Inputs: Maintain our own copy of MouseButtonsDown mask instead of using HvkGui::IsAnyMouseDown() which will be obsoleted.
//  2022-01-10: Inputs: calling new io.AddKeyEvent(), io.AddKeyModsEvent() + io.SetKeyEventNativeData() API (1.87+). Support for full HvkGuiKey range.
//  2021-12-16: Inputs: Fill VK_LCONTROL/VK_RCONTROL/VK_LSHIFT/VK_RSHIFT/VK_LMENU/VK_RMENU for completeness.
//  2021-08-17: Calling io.AddFocusEvent() on WM_SETFOCUS/WM_KILLFOCUS messages.
//  2021-08-02: Inputs: Fixed keyboard modifiers being reported when host window doesn't have focus.
//  2021-07-29: Inputs: MousePos is correctly reported when the host platform window is hovered but not focused (using TrackMouseEvent() to receive WM_MOUSELEAVE events).
//  2021-06-29: Reorganized backend to pull data from a single structure to facilitate usage with multiple-contexts (all g_XXXX access changed to bd->XXXX).
//  2021-06-08: Fixed HvkGui_ImplWin32_EnableDpiAwareness() and HvkGui_ImplWin32_GetDpiScaleForMonitor() to handle Windows 8.1/10 features without a manifest (per-monitor DPI, and properly calls SetProcessDpiAwareness() on 8.1).
//  2021-03-23: Inputs: Clearing keyboard down array when losing focus (WM_KILLFOCUS).
//  2021-02-18: Added HvkGui_ImplWin32_EnableAlphaCompositing(). Non Visual Studio users will need to link with dwmapi.lib (MinGW/gcc: use -ldwmapi).
//  2021-02-17: Fixed HvkGui_ImplWin32_EnableDpiAwareness() attempting to get SetProcessDpiAwareness from shcore.dll on Windows 8 whereas it is only supported on Windows 8.1.
//  2021-01-25: Inputs: Dynamically loading XInput DLL.
//  2020-12-04: Misc: Fixed setting of io.DisplaySize to invalid/uninitialized data when after hwnd has been closed.
//  2020-03-03: Inputs: Calling AddInputCharacterUTF16() to support surrogate pairs leading to codepoint >= 0x10000 (for more complete CJK inputs)
//  2020-02-17: Added HvkGui_ImplWin32_EnableDpiAwareness(), HvkGui_ImplWin32_GetDpiScaleForHwnd(), HvkGui_ImplWin32_GetDpiScaleForMonitor() helper functions.
//  2020-01-14: Inputs: Added support for #define HvkGui_IMPL_WIN32_DISABLE_GAMEPAD/HvkGui_IMPL_WIN32_DISABLE_LINKING_XINPUT.
//  2019-12-05: Inputs: Added support for HvkGuiMouseCursor_NotAllowed mouse cursor.
//  2019-05-11: Inputs: Don't filter value from WM_CHAR before calling AddInputCharacter().
//  2019-01-17: Misc: Using GetForegroundWindow()+IsChild() instead of GetActiveWindow() to be compatible with windows created in a different thread or parent.
//  2019-01-17: Inputs: Added support for mouse buttons 4 and 5 via WM_XBUTTON* messages.
//  2019-01-15: Inputs: Added support for XInput gamepads (if HvkGuiConfigFlags_NavEnableGamepad is set by user application).
//  2018-11-30: Misc: Setting up io.BackendPlatformName so it can be displayed in the About Window.
//  2018-06-29: Inputs: Added support for the HvkGuiMouseCursor_Hand cursor.
//  2018-06-10: Inputs: Fixed handling of mouse wheel messages to support fine position messages (typically sent by track-pads).
//  2018-06-08: Misc: Extracted HvkGui_impl_win32.cpp/.h away from the old combined DX9/DX10/DX11/DX12 examples.
//  2018-03-20: Misc: Setup io.BackendFlags HvkGuiBackendFlags_HasMouseCursors and HvkGuiBackendFlags_HasSetMousePos flags + honor HvkGuiConfigFlags_NoMouseCursorChange flag.
//  2018-02-20: Inputs: Added support for mouse cursors (HvkGui::GetMouseCursor() value and WM_SETCURSOR message handling).
//  2018-02-06: Inputs: Added mapping for HvkGuiKey_Space.
//  2018-02-06: Inputs: Honoring the io.WantSetMousePos by repositioning the mouse (when using navigation and HvkGuiConfigFlags_NavMoveMouse is set).
//  2018-02-06: Misc: Removed call to HvkGui::Shutdown() which is not available from 1.60 WIP, user needs to call CreateContext/DestroyContext themselves.
//  2018-01-20: Inputs: Added Horizontal Mouse Wheel support.
//  2018-01-08: Inputs: Added mapping for HvkGuiKey_Insert.
//  2018-01-05: Inputs: Added WM_LBUTTONDBLCLK double-click handlers for window classes with the CS_DBLCLKS flag.
//  2017-10-23: Inputs: Added WM_SYSKEYDOWN / WM_SYSKEYUP handlers so e.g. the VK_MENU key can be read.
//  2017-10-23: Inputs: Using Win32 ::SetCapture/::GetCapture() to retrieve mouse positions outside the client area when dragging.
//  2016-11-12: Inputs: Only call Win32 ::SetCursor(nullptr) when io.MouseDrawCursor is set.

#include "hvkgui.h"
#ifndef HvkGui_DISABLE
#include "HvkGui_impl_win32.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <windowsx.h> // GET_X_LPARAM(), GET_Y_LPARAM()
#include <tchar.h>
#include <dwmapi.h>

// Using XInput for gamepad (will load DLL dynamically)
#ifndef HvkGui_IMPL_WIN32_DISABLE_GAMEPAD
#include <xinput.h>
typedef DWORD(WINAPI* PFN_XInputGetCapabilities)(DWORD, DWORD, XINPUT_CAPABILITIES*);
typedef DWORD(WINAPI* PFN_XInputGetState)(DWORD, XINPUT_STATE*);
#endif

// Clang/GCC warnings with -Weverything
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-function-type"     // warning: cast between incompatible function types (for loader)
#endif
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"                  // warning: unknown option after '#pragma GCC diagnostic' kind
#pragma GCC diagnostic ignored "-Wcast-function-type"       // warning: cast between incompatible function types (for loader)
#endif

struct HvkGui_ImplWin32_Data
{
    HWND                        hWnd;
    HWND                        MouseHwnd;
    int                         MouseTrackedArea;   // 0: not tracked, 1: client area, 2: non-client area
    int                         MouseButtonsDown;
    INT64                       Time;
    INT64                       TicksPerSecond;
    HvkGuiMouseCursor            LastMouseCursor;
    UINT32                      KeyboardCodePage;

#ifndef HvkGui_IMPL_WIN32_DISABLE_GAMEPAD
    bool                        HasGamepad;
    bool                        WantUpdateHasGamepad;
    HMODULE                     XInputDLL;
    PFN_XInputGetCapabilities   XInputGetCapabilities;
    PFN_XInputGetState          XInputGetState;
#endif

    HvkGui_ImplWin32_Data()      { memset((void*)this, 0, sizeof(*this)); }
};

// Backend data stored in io.BackendPlatformUserData to allow support for multiple Dear HvkGui contexts
// It is STRONGLY preferred that you use docking branch with multi-viewports (== single Dear HvkGui context + multiple windows) instead of multiple Dear HvkGui contexts.
// FIXME: multi-context support is not well tested and probably dysfunctional in this backend.
// FIXME: some shared resources (mouse cursor shape, gamepad) are mishandled when using multi-context.
static HvkGui_ImplWin32_Data* HvkGui_ImplWin32_GetBackendData()
{
    return HvkGui::GetCurrentContext() ? (HvkGui_ImplWin32_Data*)HvkGui::GetIO().BackendPlatformUserData : nullptr;
}
static HvkGui_ImplWin32_Data* HvkGui_ImplWin32_GetBackendData(HvkGuiIO& io)
{
    return (HvkGui_ImplWin32_Data*)io.BackendPlatformUserData;
}

// Functions
static void HvkGui_ImplWin32_UpdateKeyboardCodePage(HvkGuiIO& io)
{
    // Retrieve keyboard code page, required for handling of non-Unicode Windows.
    HvkGui_ImplWin32_Data* bd = HvkGui_ImplWin32_GetBackendData(io);
    HKL keyboard_layout = ::GetKeyboardLayout(0);
    LCID keyboard_lcid = MAKELCID(HIWORD(keyboard_layout), SORT_DEFAULT);
    if (::GetLocaleInfoA(keyboard_lcid, (LOCALE_RETURN_NUMBER | LOCALE_IDEFAULTANSICODEPAGE), (LPSTR)&bd->KeyboardCodePage, sizeof(bd->KeyboardCodePage)) == 0)
        bd->KeyboardCodePage = CP_ACP; // Fallback to default ANSI code page when fails.
}

static bool HvkGui_ImplWin32_InitEx(void* hwnd, bool platform_has_own_dc)
{
    HvkGuiIO& io = HvkGui::GetIO();
    HvkGui_CHECKVERSION();
    IM_ASSERT(io.BackendPlatformUserData == nullptr && "Already initialized a platform backend!");

    INT64 perf_frequency, perf_counter;
    if (!::QueryPerformanceFrequency((LARGE_INTEGER*)&perf_frequency))
        return false;
    if (!::QueryPerformanceCounter((LARGE_INTEGER*)&perf_counter))
        return false;

    // Setup backend capabilities flags
    HvkGui_ImplWin32_Data* bd = IM_NEW(HvkGui_ImplWin32_Data)();
    io.BackendPlatformUserData = (void*)bd;
    io.BackendPlatformName = "HvkGui_impl_win32";
    io.BackendFlags |= HvkGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |= HvkGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)

    bd->hWnd = (HWND)hwnd;
    bd->TicksPerSecond = perf_frequency;
    bd->Time = perf_counter;
    bd->LastMouseCursor = HvkGuiMouseCursor_COUNT;
    HvkGui_ImplWin32_UpdateKeyboardCodePage(io);

    // Set platform dependent data in viewport
    HvkGuiViewport* main_viewport = HvkGui::GetMainViewport();
    main_viewport->PlatformHandle = main_viewport->PlatformHandleRaw = (void*)bd->hWnd;
    IM_UNUSED(platform_has_own_dc); // Used in 'docking' branch

    // Dynamically load XInput library
#ifndef HvkGui_IMPL_WIN32_DISABLE_GAMEPAD
    bd->WantUpdateHasGamepad = true;
    const char* xinput_dll_names[] =
    {
        "xinput1_4.dll",   // Windows 8+
        "xinput1_3.dll",   // DirectX SDK
        "xinput9_1_0.dll", // Windows Vista, Windows 7
        "xinput1_2.dll",   // DirectX SDK
        "xinput1_1.dll"    // DirectX SDK
    };
    for (int n = 0; n < IM_ARRAYSIZE(xinput_dll_names); n++)
        if (HMODULE dll = ::LoadLibraryA(xinput_dll_names[n]))
        {
            bd->XInputDLL = dll;
            bd->XInputGetCapabilities = (PFN_XInputGetCapabilities)::GetProcAddress(dll, "XInputGetCapabilities");
            bd->XInputGetState = (PFN_XInputGetState)::GetProcAddress(dll, "XInputGetState");
            break;
        }
#endif // HvkGui_IMPL_WIN32_DISABLE_GAMEPAD

    return true;
}

HvkGui_IMPL_API bool     HvkGui_ImplWin32_Init(void* hwnd)
{
    return HvkGui_ImplWin32_InitEx(hwnd, false);
}

HvkGui_IMPL_API bool     HvkGui_ImplWin32_InitForOpenGL(void* hwnd)
{
    // OpenGL needs CS_OWNDC
    return HvkGui_ImplWin32_InitEx(hwnd, true);
}

void    HvkGui_ImplWin32_Shutdown()
{
    HvkGui_ImplWin32_Data* bd = HvkGui_ImplWin32_GetBackendData();
    IM_ASSERT(bd != nullptr && "No platform backend to shutdown, or already shutdown?");
    HvkGuiIO& io = HvkGui::GetIO();
    HvkGuiPlatformIO& platform_io = HvkGui::GetPlatformIO();

    // Unload XInput library
#ifndef HvkGui_IMPL_WIN32_DISABLE_GAMEPAD
    if (bd->XInputDLL)
        ::FreeLibrary(bd->XInputDLL);
#endif // HvkGui_IMPL_WIN32_DISABLE_GAMEPAD

    io.BackendPlatformName = nullptr;
    io.BackendPlatformUserData = nullptr;
    io.BackendFlags &= ~(HvkGuiBackendFlags_HasMouseCursors | HvkGuiBackendFlags_HasSetMousePos | HvkGuiBackendFlags_HasGamepad);
    platform_io.ClearPlatformHandlers();
    IM_DELETE(bd);
}

static bool HvkGui_ImplWin32_UpdateMouseCursor(HvkGuiIO& io, HvkGuiMouseCursor HvkGui_cursor)
{
    if (io.ConfigFlags & HvkGuiConfigFlags_NoMouseCursorChange)
        return false;

    if (HvkGui_cursor == HvkGuiMouseCursor_None || io.MouseDrawCursor)
    {
        // Hide OS mouse cursor if HvkGui is drawing it or if it wants no cursor
        ::SetCursor(nullptr);
    }
    else
    {
        // Show OS mouse cursor
        LPTSTR win32_cursor = IDC_ARROW;
        switch (HvkGui_cursor)
        {
        case HvkGuiMouseCursor_Arrow:        win32_cursor = IDC_ARROW; break;
        case HvkGuiMouseCursor_TextInput:    win32_cursor = IDC_IBEAM; break;
        case HvkGuiMouseCursor_ResizeAll:    win32_cursor = IDC_SIZEALL; break;
        case HvkGuiMouseCursor_ResizeEW:     win32_cursor = IDC_SIZEWE; break;
        case HvkGuiMouseCursor_ResizeNS:     win32_cursor = IDC_SIZENS; break;
        case HvkGuiMouseCursor_ResizeNESW:   win32_cursor = IDC_SIZENESW; break;
        case HvkGuiMouseCursor_ResizeNWSE:   win32_cursor = IDC_SIZENWSE; break;
        case HvkGuiMouseCursor_Hand:         win32_cursor = IDC_HAND; break;
        case HvkGuiMouseCursor_Wait:         win32_cursor = IDC_WAIT; break;
        case HvkGuiMouseCursor_Progress:     win32_cursor = IDC_APPSTARTING; break;
        case HvkGuiMouseCursor_NotAllowed:   win32_cursor = IDC_NO; break;
        }
        ::SetCursor(::LoadCursor(nullptr, win32_cursor));
    }
    return true;
}

static bool IsVkDown(int vk)
{
    return (::GetKeyState(vk) & 0x8000) != 0;
}

static void HvkGui_ImplWin32_AddKeyEvent(HvkGuiIO& io, HvkGuiKey key, bool down, int native_keycode, int native_scancode = -1)
{
    io.AddKeyEvent(key, down);
    io.SetKeyEventNativeData(key, native_keycode, native_scancode); // To support legacy indexing (<1.87 user code)
    IM_UNUSED(native_scancode);
}

static void HvkGui_ImplWin32_ProcessKeyEventsWorkarounds(HvkGuiIO& io)
{
    // Left & right Shift keys: when both are pressed together, Windows tend to not generate the WM_KEYUP event for the first released one.
    if (HvkGui::IsKeyDown(HvkGuiKey_LeftShift) && !IsVkDown(VK_LSHIFT))
        HvkGui_ImplWin32_AddKeyEvent(io, HvkGuiKey_LeftShift, false, VK_LSHIFT);
    if (HvkGui::IsKeyDown(HvkGuiKey_RightShift) && !IsVkDown(VK_RSHIFT))
        HvkGui_ImplWin32_AddKeyEvent(io, HvkGuiKey_RightShift, false, VK_RSHIFT);

    // Sometimes WM_KEYUP for Win key is not passed down to the app (e.g. for Win+V on some setups, according to GLFW).
    if (HvkGui::IsKeyDown(HvkGuiKey_LeftSuper) && !IsVkDown(VK_LWIN))
        HvkGui_ImplWin32_AddKeyEvent(io, HvkGuiKey_LeftSuper, false, VK_LWIN);
    if (HvkGui::IsKeyDown(HvkGuiKey_RightSuper) && !IsVkDown(VK_RWIN))
        HvkGui_ImplWin32_AddKeyEvent(io, HvkGuiKey_RightSuper, false, VK_RWIN);
}

static void HvkGui_ImplWin32_UpdateKeyModifiers(HvkGuiIO& io)
{
    io.AddKeyEvent(HvkGuiMod_Ctrl, IsVkDown(VK_CONTROL));
    io.AddKeyEvent(HvkGuiMod_Shift, IsVkDown(VK_SHIFT));
    io.AddKeyEvent(HvkGuiMod_Alt, IsVkDown(VK_MENU));
    io.AddKeyEvent(HvkGuiMod_Super, IsVkDown(VK_LWIN) || IsVkDown(VK_RWIN));
}

static void HvkGui_ImplWin32_UpdateMouseData(HvkGuiIO& io)
{
    HvkGui_ImplWin32_Data* bd = HvkGui_ImplWin32_GetBackendData(io);
    IM_ASSERT(bd->hWnd != 0);

    HWND focused_window = ::GetForegroundWindow();
    const bool is_app_focused = (focused_window == bd->hWnd);
    if (is_app_focused)
    {
        // (Optional) Set OS mouse position from Dear HvkGui if requested (rarely used, only when io.ConfigNavMoveSetMousePos is enabled by user)
        if (io.WantSetMousePos)
        {
            POINT pos = { (int)io.MousePos.x, (int)io.MousePos.y };
            if (::ClientToScreen(bd->hWnd, &pos))
                ::SetCursorPos(pos.x, pos.y);
        }

        // (Optional) Fallback to provide mouse position when focused (WM_MOUSEMOVE already provides this when hovered or captured)
        // This also fills a short gap when clicking non-client area: WM_NCMOUSELEAVE -> modal OS move -> gap -> WM_NCMOUSEMOVE
        if (!io.WantSetMousePos && bd->MouseTrackedArea == 0)
        {
            POINT pos;
            if (::GetCursorPos(&pos) && ::ScreenToClient(bd->hWnd, &pos))
                io.AddMousePosEvent((float)pos.x, (float)pos.y);
        }
    }
}

// Gamepad navigation mapping
static void HvkGui_ImplWin32_UpdateGamepads(HvkGuiIO& io)
{
#ifndef HvkGui_IMPL_WIN32_DISABLE_GAMEPAD
    HvkGui_ImplWin32_Data* bd = HvkGui_ImplWin32_GetBackendData(io);

    // Calling XInputGetState() every frame on disconnected gamepads is unfortunately too slow.
    // Instead we refresh gamepad availability by calling XInputGetCapabilities() _only_ after receiving WM_DEVICECHANGE.
    if (bd->WantUpdateHasGamepad)
    {
        XINPUT_CAPABILITIES caps = {};
        bd->HasGamepad = bd->XInputGetCapabilities ? (bd->XInputGetCapabilities(0, XINPUT_FLAG_GAMEPAD, &caps) == ERROR_SUCCESS) : false;
        bd->WantUpdateHasGamepad = false;
    }

    io.BackendFlags &= ~HvkGuiBackendFlags_HasGamepad;
    XINPUT_STATE xinput_state;
    XINPUT_GAMEPAD& gamepad = xinput_state.Gamepad;
    if (!bd->HasGamepad || bd->XInputGetState == nullptr || bd->XInputGetState(0, &xinput_state) != ERROR_SUCCESS)
        return;
    io.BackendFlags |= HvkGuiBackendFlags_HasGamepad;

    #define IM_SATURATE(V)                      (V < 0.0f ? 0.0f : V > 1.0f ? 1.0f : V)
    #define MAP_BUTTON(KEY_NO, BUTTON_ENUM)     { io.AddKeyEvent(KEY_NO, (gamepad.wButtons & BUTTON_ENUM) != 0); }
    #define MAP_ANALOG(KEY_NO, VALUE, V0, V1)   { float vn = (float)(VALUE - V0) / (float)(V1 - V0); io.AddKeyAnalogEvent(KEY_NO, vn > 0.10f, IM_SATURATE(vn)); }
    MAP_BUTTON(HvkGuiKey_GamepadStart,           XINPUT_GAMEPAD_START);
    MAP_BUTTON(HvkGuiKey_GamepadBack,            XINPUT_GAMEPAD_BACK);
    MAP_BUTTON(HvkGuiKey_GamepadFaceLeft,        XINPUT_GAMEPAD_X);
    MAP_BUTTON(HvkGuiKey_GamepadFaceRight,       XINPUT_GAMEPAD_B);
    MAP_BUTTON(HvkGuiKey_GamepadFaceUp,          XINPUT_GAMEPAD_Y);
    MAP_BUTTON(HvkGuiKey_GamepadFaceDown,        XINPUT_GAMEPAD_A);
    MAP_BUTTON(HvkGuiKey_GamepadDpadLeft,        XINPUT_GAMEPAD_DPAD_LEFT);
    MAP_BUTTON(HvkGuiKey_GamepadDpadRight,       XINPUT_GAMEPAD_DPAD_RIGHT);
    MAP_BUTTON(HvkGuiKey_GamepadDpadUp,          XINPUT_GAMEPAD_DPAD_UP);
    MAP_BUTTON(HvkGuiKey_GamepadDpadDown,        XINPUT_GAMEPAD_DPAD_DOWN);
    MAP_BUTTON(HvkGuiKey_GamepadL1,              XINPUT_GAMEPAD_LEFT_SHOULDER);
    MAP_BUTTON(HvkGuiKey_GamepadR1,              XINPUT_GAMEPAD_RIGHT_SHOULDER);
    MAP_ANALOG(HvkGuiKey_GamepadL2,              gamepad.bLeftTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD, 255);
    MAP_ANALOG(HvkGuiKey_GamepadR2,              gamepad.bRightTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD, 255);
    MAP_BUTTON(HvkGuiKey_GamepadL3,              XINPUT_GAMEPAD_LEFT_THUMB);
    MAP_BUTTON(HvkGuiKey_GamepadR3,              XINPUT_GAMEPAD_RIGHT_THUMB);
    MAP_ANALOG(HvkGuiKey_GamepadLStickLeft,      gamepad.sThumbLX, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32768);
    MAP_ANALOG(HvkGuiKey_GamepadLStickRight,     gamepad.sThumbLX, +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, +32767);
    MAP_ANALOG(HvkGuiKey_GamepadLStickUp,        gamepad.sThumbLY, +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, +32767);
    MAP_ANALOG(HvkGuiKey_GamepadLStickDown,      gamepad.sThumbLY, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32768);
    MAP_ANALOG(HvkGuiKey_GamepadRStickLeft,      gamepad.sThumbRX, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32768);
    MAP_ANALOG(HvkGuiKey_GamepadRStickRight,     gamepad.sThumbRX, +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, +32767);
    MAP_ANALOG(HvkGuiKey_GamepadRStickUp,        gamepad.sThumbRY, +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, +32767);
    MAP_ANALOG(HvkGuiKey_GamepadRStickDown,      gamepad.sThumbRY, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32768);
    #undef MAP_BUTTON
    #undef MAP_ANALOG
#else // #ifndef HvkGui_IMPL_WIN32_DISABLE_GAMEPAD
    IM_UNUSED(io);
#endif
}

void    HvkGui_ImplWin32_NewFrame()
{
    HvkGui_ImplWin32_Data* bd = HvkGui_ImplWin32_GetBackendData();
    IM_ASSERT(bd != nullptr && "Context or backend not initialized? Did you call HvkGui_ImplWin32_Init()?");
    HvkGuiIO& io = HvkGui::GetIO();

    // Setup display size (every frame to accommodate for window resizing)
    RECT rect = { 0, 0, 0, 0 };
    ::GetClientRect(bd->hWnd, &rect);
    io.DisplaySize = HvkVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));

    // Setup time step
    INT64 current_time = 0;
    ::QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
    io.DeltaTime = (float)(current_time - bd->Time) / bd->TicksPerSecond;
    bd->Time = current_time;

    // Update OS mouse position
    HvkGui_ImplWin32_UpdateMouseData(io);

    // Process workarounds for known Windows key handling issues
    HvkGui_ImplWin32_ProcessKeyEventsWorkarounds(io);

    // Update OS mouse cursor with the cursor requested by HvkGui
    HvkGuiMouseCursor mouse_cursor = io.MouseDrawCursor ? HvkGuiMouseCursor_None : HvkGui::GetMouseCursor();
    if (bd->LastMouseCursor != mouse_cursor)
    {
        bd->LastMouseCursor = mouse_cursor;
        HvkGui_ImplWin32_UpdateMouseCursor(io, mouse_cursor);
    }

    // Update game controllers (if enabled and available)
    HvkGui_ImplWin32_UpdateGamepads(io);
}

// Map VK_xxx to HvkGuiKey_xxx.
// Not static to allow third-party code to use that if they want to (but undocumented)
HvkGuiKey HvkGui_ImplWin32_KeyEventToHvkGuiKey(WPARAM wParam, LPARAM lParam);
HvkGuiKey HvkGui_ImplWin32_KeyEventToHvkGuiKey(WPARAM wParam, LPARAM lParam)
{
    // There is no distinct VK_xxx for keypad enter, instead it is VK_RETURN + KF_EXTENDED.
    if ((wParam == VK_RETURN) && (HIWORD(lParam) & KF_EXTENDED))
        return HvkGuiKey_KeypadEnter;

    const int scancode = (int)LOBYTE(HIWORD(lParam));
    //HvkGui_DEBUG_LOG("scancode %3d, keycode = 0x%02X\n", scancode, wParam);
    switch (wParam)
    {
        case VK_TAB: return HvkGuiKey_Tab;
        case VK_LEFT: return HvkGuiKey_LeftArrow;
        case VK_RIGHT: return HvkGuiKey_RightArrow;
        case VK_UP: return HvkGuiKey_UpArrow;
        case VK_DOWN: return HvkGuiKey_DownArrow;
        case VK_PRIOR: return HvkGuiKey_PageUp;
        case VK_NEXT: return HvkGuiKey_PageDown;
        case VK_HOME: return HvkGuiKey_Home;
        case VK_END: return HvkGuiKey_End;
        case VK_INSERT: return HvkGuiKey_Insert;
        case VK_DELETE: return HvkGuiKey_Delete;
        case VK_BACK: return HvkGuiKey_Backspace;
        case VK_SPACE: return HvkGuiKey_Space;
        case VK_RETURN: return HvkGuiKey_Enter;
        case VK_ESCAPE: return HvkGuiKey_Escape;
        //case VK_OEM_7: return HvkGuiKey_Apostrophe;
        case VK_OEM_COMMA: return HvkGuiKey_Comma;
        //case VK_OEM_MINUS: return HvkGuiKey_Minus;
        case VK_OEM_PERIOD: return HvkGuiKey_Period;
        //case VK_OEM_2: return HvkGuiKey_Slash;
        //case VK_OEM_1: return HvkGuiKey_Semicolon;
        //case VK_OEM_PLUS: return HvkGuiKey_Equal;
        //case VK_OEM_4: return HvkGuiKey_LeftBracket;
        //case VK_OEM_5: return HvkGuiKey_Backslash;
        //case VK_OEM_6: return HvkGuiKey_RightBracket;
        //case VK_OEM_3: return HvkGuiKey_GraveAccent;
        case VK_CAPITAL: return HvkGuiKey_CapsLock;
        case VK_SCROLL: return HvkGuiKey_ScrollLock;
        case VK_NUMLOCK: return HvkGuiKey_NumLock;
        case VK_SNAPSHOT: return HvkGuiKey_PrintScreen;
        case VK_PAUSE: return HvkGuiKey_Pause;
        case VK_NUMPAD0: return HvkGuiKey_Keypad0;
        case VK_NUMPAD1: return HvkGuiKey_Keypad1;
        case VK_NUMPAD2: return HvkGuiKey_Keypad2;
        case VK_NUMPAD3: return HvkGuiKey_Keypad3;
        case VK_NUMPAD4: return HvkGuiKey_Keypad4;
        case VK_NUMPAD5: return HvkGuiKey_Keypad5;
        case VK_NUMPAD6: return HvkGuiKey_Keypad6;
        case VK_NUMPAD7: return HvkGuiKey_Keypad7;
        case VK_NUMPAD8: return HvkGuiKey_Keypad8;
        case VK_NUMPAD9: return HvkGuiKey_Keypad9;
        case VK_DECIMAL: return HvkGuiKey_KeypadDecimal;
        case VK_DIVIDE: return HvkGuiKey_KeypadDivide;
        case VK_MULTIPLY: return HvkGuiKey_KeypadMultiply;
        case VK_SUBTRACT: return HvkGuiKey_KeypadSubtract;
        case VK_ADD: return HvkGuiKey_KeypadAdd;
        case VK_LSHIFT: return HvkGuiKey_LeftShift;
        case VK_LCONTROL: return HvkGuiKey_LeftCtrl;
        case VK_LMENU: return HvkGuiKey_LeftAlt;
        case VK_LWIN: return HvkGuiKey_LeftSuper;
        case VK_RSHIFT: return HvkGuiKey_RightShift;
        case VK_RCONTROL: return HvkGuiKey_RightCtrl;
        case VK_RMENU: return HvkGuiKey_RightAlt;
        case VK_RWIN: return HvkGuiKey_RightSuper;
        case VK_APPS: return HvkGuiKey_Menu;
        case '0': return HvkGuiKey_0;
        case '1': return HvkGuiKey_1;
        case '2': return HvkGuiKey_2;
        case '3': return HvkGuiKey_3;
        case '4': return HvkGuiKey_4;
        case '5': return HvkGuiKey_5;
        case '6': return HvkGuiKey_6;
        case '7': return HvkGuiKey_7;
        case '8': return HvkGuiKey_8;
        case '9': return HvkGuiKey_9;
        case 'A': return HvkGuiKey_A;
        case 'B': return HvkGuiKey_B;
        case 'C': return HvkGuiKey_C;
        case 'D': return HvkGuiKey_D;
        case 'E': return HvkGuiKey_E;
        case 'F': return HvkGuiKey_F;
        case 'G': return HvkGuiKey_G;
        case 'H': return HvkGuiKey_H;
        case 'I': return HvkGuiKey_I;
        case 'J': return HvkGuiKey_J;
        case 'K': return HvkGuiKey_K;
        case 'L': return HvkGuiKey_L;
        case 'M': return HvkGuiKey_M;
        case 'N': return HvkGuiKey_N;
        case 'O': return HvkGuiKey_O;
        case 'P': return HvkGuiKey_P;
        case 'Q': return HvkGuiKey_Q;
        case 'R': return HvkGuiKey_R;
        case 'S': return HvkGuiKey_S;
        case 'T': return HvkGuiKey_T;
        case 'U': return HvkGuiKey_U;
        case 'V': return HvkGuiKey_V;
        case 'W': return HvkGuiKey_W;
        case 'X': return HvkGuiKey_X;
        case 'Y': return HvkGuiKey_Y;
        case 'Z': return HvkGuiKey_Z;
        case VK_F1: return HvkGuiKey_F1;
        case VK_F2: return HvkGuiKey_F2;
        case VK_F3: return HvkGuiKey_F3;
        case VK_F4: return HvkGuiKey_F4;
        case VK_F5: return HvkGuiKey_F5;
        case VK_F6: return HvkGuiKey_F6;
        case VK_F7: return HvkGuiKey_F7;
        case VK_F8: return HvkGuiKey_F8;
        case VK_F9: return HvkGuiKey_F9;
        case VK_F10: return HvkGuiKey_F10;
        case VK_F11: return HvkGuiKey_F11;
        case VK_F12: return HvkGuiKey_F12;
        case VK_F13: return HvkGuiKey_F13;
        case VK_F14: return HvkGuiKey_F14;
        case VK_F15: return HvkGuiKey_F15;
        case VK_F16: return HvkGuiKey_F16;
        case VK_F17: return HvkGuiKey_F17;
        case VK_F18: return HvkGuiKey_F18;
        case VK_F19: return HvkGuiKey_F19;
        case VK_F20: return HvkGuiKey_F20;
        case VK_F21: return HvkGuiKey_F21;
        case VK_F22: return HvkGuiKey_F22;
        case VK_F23: return HvkGuiKey_F23;
        case VK_F24: return HvkGuiKey_F24;
        case VK_BROWSER_BACK: return HvkGuiKey_AppBack;
        case VK_BROWSER_FORWARD: return HvkGuiKey_AppForward;
        default: break;
    }

    // Fallback to scancode
    // https://handmade.network/forums/t/2011-keyboard_inputs_-_scancodes,_raw_input,_text_input,_key_names
    switch (scancode)
    {
    case 41: return HvkGuiKey_GraveAccent;  // VK_OEM_8 in EN-UK, VK_OEM_3 in EN-US, VK_OEM_7 in FR, VK_OEM_5 in DE, etc.
    case 12: return HvkGuiKey_Minus;
    case 13: return HvkGuiKey_Equal;
    case 26: return HvkGuiKey_LeftBracket;
    case 27: return HvkGuiKey_RightBracket;
    case 86: return HvkGuiKey_Oem102;
    case 43: return HvkGuiKey_Backslash;
    case 39: return HvkGuiKey_Semicolon;
    case 40: return HvkGuiKey_Apostrophe;
    case 51: return HvkGuiKey_Comma;
    case 52: return HvkGuiKey_Period;
    case 53: return HvkGuiKey_Slash;
    default: break;
    }

    return HvkGuiKey_None;
}

// Allow compilation with old Windows SDK. MinGW doesn't have default _WIN32_WINNT/WINVER versions.
#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL 0x020E
#endif
#ifndef DBT_DEVNODES_CHANGED
#define DBT_DEVNODES_CHANGED 0x0007
#endif

// Helper to obtain the source of mouse messages.
// See https://learn.microsoft.com/en-us/windows/win32/tablet/system-events-and-mouse-messages
// Prefer to call this at the top of the message handler to avoid the possibility of other Win32 calls interfering with this.
static HvkGuiMouseSource HvkGui_ImplWin32_GetMouseSourceFromMessageExtraInfo()
{
    LPARAM extra_info = ::GetMessageExtraInfo();
    if ((extra_info & 0xFFFFFF80) == 0xFF515700)
        return HvkGuiMouseSource_Pen;
    if ((extra_info & 0xFFFFFF80) == 0xFF515780)
        return HvkGuiMouseSource_TouchScreen;
    return HvkGuiMouseSource_Mouse;
}

// Win32 message handler (process Win32 mouse/keyboard inputs, etc.)
// Call from your application's message handler. Keep calling your message handler unless this function returns TRUE.
// When Hvkplementing your own backend, you can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if Dear HvkGui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to Dear HvkGui, and hide them from your application based on those two flags.
// PS: We treat DBLCLK messages as regular mouse down messages, so this code will work on windows classes that have the CS_DBLCLKS flag set. Our own example app code doesn't set this flag.

// Copy either line into your .cpp file to forward declare the function:
extern HvkGui_IMPL_API LRESULT HvkGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);                // Use HvkGui::GetCurrentContext()
extern HvkGui_IMPL_API LRESULT HvkGui_ImplWin32_WndProcHandlerEx(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, HvkGuiIO& io); // Doesn't use HvkGui::GetCurrentContext()

HvkGui_IMPL_API LRESULT HvkGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Most backends don't have silent checks like this one, but we need it because WndProc are called early in CreateWindow().
    // We silently allow both context or just only backend data to be nullptr.
    if (HvkGui::GetCurrentContext() == nullptr)
        return 0;
    return HvkGui_ImplWin32_WndProcHandlerEx(hwnd, msg, wParam, lParam, HvkGui::GetIO());
}

// This version is in theory thread-safe in the sense that no path should access HvkGui::GetCurrentContext().
HvkGui_IMPL_API LRESULT HvkGui_ImplWin32_WndProcHandlerEx(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, HvkGuiIO& io)
{
    HvkGui_ImplWin32_Data* bd = HvkGui_ImplWin32_GetBackendData(io);
    if (bd == nullptr)
        return 0;
    switch (msg)
    {
    case WM_MOUSEMOVE:
    case WM_NCMOUSEMOVE:
    {
        // We need to call TrackMouseEvent in order to receive WM_MOUSELEAVE events
        HvkGuiMouseSource mouse_source = HvkGui_ImplWin32_GetMouseSourceFromMessageExtraInfo();
        const int area = (msg == WM_MOUSEMOVE) ? 1 : 2;
        bd->MouseHwnd = hwnd;
        if (bd->MouseTrackedArea != area)
        {
            TRACKMOUSEEVENT tme_cancel = { sizeof(tme_cancel), TME_CANCEL, hwnd, 0 };
            TRACKMOUSEEVENT tme_track = { sizeof(tme_track), (DWORD)((area == 2) ? (TME_LEAVE | TME_NONCLIENT) : TME_LEAVE), hwnd, 0 };
            if (bd->MouseTrackedArea != 0)
                ::TrackMouseEvent(&tme_cancel);
            ::TrackMouseEvent(&tme_track);
            bd->MouseTrackedArea = area;
        }
        POINT mouse_pos = { (LONG)GET_X_LPARAM(lParam), (LONG)GET_Y_LPARAM(lParam) };
        if (msg == WM_NCMOUSEMOVE && ::ScreenToClient(hwnd, &mouse_pos) == FALSE) // WM_NCMOUSEMOVE are provided in absolute coordinates.
            return 0;
        io.AddMouseSourceEvent(mouse_source);
        io.AddMousePosEvent((float)mouse_pos.x, (float)mouse_pos.y);
        return 0;
    }
    case WM_MOUSELEAVE:
    case WM_NCMOUSELEAVE:
    {
        const int area = (msg == WM_MOUSELEAVE) ? 1 : 2;
        if (bd->MouseTrackedArea == area)
        {
            if (bd->MouseHwnd == hwnd)
                bd->MouseHwnd = nullptr;
            bd->MouseTrackedArea = 0;
            io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
        }
        return 0;
    }
    case WM_DESTROY:
        if (bd->MouseHwnd == hwnd && bd->MouseTrackedArea != 0)
        {
            TRACKMOUSEEVENT tme_cancel = { sizeof(tme_cancel), TME_CANCEL, hwnd, 0 };
            ::TrackMouseEvent(&tme_cancel);
            bd->MouseHwnd = nullptr;
            bd->MouseTrackedArea = 0;
            io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
        }
        return 0;
    case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
    case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
    case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
    {
        HvkGuiMouseSource mouse_source = HvkGui_ImplWin32_GetMouseSourceFromMessageExtraInfo();
        int button = 0;
        if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK) { button = 0; }
        if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK) { button = 1; }
        if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK) { button = 2; }
        if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONDBLCLK) { button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4; }
        HWND hwnd_with_capture = ::GetCapture();
        if (bd->MouseButtonsDown != 0 && hwnd_with_capture != hwnd) // Did we externally lost capture?
            bd->MouseButtonsDown = 0;
        if (bd->MouseButtonsDown == 0 && hwnd_with_capture == nullptr)
            ::SetCapture(hwnd); // Allow us to read mouse coordinates when dragging mouse outside of our window bounds.
        bd->MouseButtonsDown |= 1 << button;
        io.AddMouseSourceEvent(mouse_source);
        io.AddMouseButtonEvent(button, true);
        return 0;
    }
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
    case WM_XBUTTONUP:
    {
        HvkGuiMouseSource mouse_source = HvkGui_ImplWin32_GetMouseSourceFromMessageExtraInfo();
        int button = 0;
        if (msg == WM_LBUTTONUP) { button = 0; }
        if (msg == WM_RBUTTONUP) { button = 1; }
        if (msg == WM_MBUTTONUP) { button = 2; }
        if (msg == WM_XBUTTONUP) { button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4; }
        bd->MouseButtonsDown &= ~(1 << button);
        if (bd->MouseButtonsDown == 0 && ::GetCapture() == hwnd)
            ::ReleaseCapture();
        io.AddMouseSourceEvent(mouse_source);
        io.AddMouseButtonEvent(button, false);
        return 0;
    }
    case WM_MOUSEWHEEL:
        io.AddMouseWheelEvent(0.0f, (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA);
        return 0;
    case WM_MOUSEHWHEEL:
        io.AddMouseWheelEvent(-(float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA, 0.0f);
        return 0;
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    {
        const bool is_key_down = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
        if (wParam < 256)
        {
            // Submit modifiers
            HvkGui_ImplWin32_UpdateKeyModifiers(io);

            // Obtain virtual key code and convert to HvkGuiKey
            const HvkGuiKey key = HvkGui_ImplWin32_KeyEventToHvkGuiKey(wParam, lParam);
            const int vk = (int)wParam;
            const int scancode = (int)LOBYTE(HIWORD(lParam));

            // Special behavior for VK_SNAPSHOT / HvkGuiKey_PrintScreen as Windows doesn't emit the key down event.
            if (key == HvkGuiKey_PrintScreen && !is_key_down)
                HvkGui_ImplWin32_AddKeyEvent(io, key, true, vk, scancode);

            // Submit key event
            if (key != HvkGuiKey_None)
                HvkGui_ImplWin32_AddKeyEvent(io, key, is_key_down, vk, scancode);

            // Submit individual left/right modifier events
            if (vk == VK_SHIFT)
            {
                // Hvkportant: Shift keys tend to get stuck when pressed together, missing key-up events are corrected in HvkGui_ImplWin32_ProcessKeyEventsWorkarounds()
                if (IsVkDown(VK_LSHIFT) == is_key_down) { HvkGui_ImplWin32_AddKeyEvent(io, HvkGuiKey_LeftShift, is_key_down, VK_LSHIFT, scancode); }
                if (IsVkDown(VK_RSHIFT) == is_key_down) { HvkGui_ImplWin32_AddKeyEvent(io, HvkGuiKey_RightShift, is_key_down, VK_RSHIFT, scancode); }
            }
            else if (vk == VK_CONTROL)
            {
                if (IsVkDown(VK_LCONTROL) == is_key_down) { HvkGui_ImplWin32_AddKeyEvent(io, HvkGuiKey_LeftCtrl, is_key_down, VK_LCONTROL, scancode); }
                if (IsVkDown(VK_RCONTROL) == is_key_down) { HvkGui_ImplWin32_AddKeyEvent(io, HvkGuiKey_RightCtrl, is_key_down, VK_RCONTROL, scancode); }
            }
            else if (vk == VK_MENU)
            {
                if (IsVkDown(VK_LMENU) == is_key_down) { HvkGui_ImplWin32_AddKeyEvent(io, HvkGuiKey_LeftAlt, is_key_down, VK_LMENU, scancode); }
                if (IsVkDown(VK_RMENU) == is_key_down) { HvkGui_ImplWin32_AddKeyEvent(io, HvkGuiKey_RightAlt, is_key_down, VK_RMENU, scancode); }
            }
        }
        return 0;
    }
    case WM_SETFOCUS:
    case WM_KILLFOCUS:
        io.AddFocusEvent(msg == WM_SETFOCUS);
        return 0;
    case WM_INPUTLANGCHANGE:
        HvkGui_ImplWin32_UpdateKeyboardCodePage(io);
        return 0;
    case WM_CHAR:
        if (::IsWindowUnicode(hwnd))
        {
            // You can also use ToAscii()+GetKeyboardState() to retrieve characters.
            if (wParam > 0 && wParam < 0x10000)
                io.AddInputCharacterUTF16((unsigned short)wParam);
        }
        else
        {
            wchar_t wch = 0;
            ::MultiByteToWideChar(bd->KeyboardCodePage, MB_PRECOMPOSED, (char*)&wParam, 2, &wch, 1);
            io.AddInputCharacter(wch);
        }
        return 0;
    case WM_IME_COMPOSITION:
    {
        // Handling WM_IME_COMPOSITION ensure that WM_IME_CHAR value is correct even for MBCS apps.
        // (see #9099, #3653 and https://stackoverflow.com/questions/77450354 topics) 
        LRESULT result = ::DefWindowProcW(hwnd, msg, wParam, lParam);
        return (lParam & GCS_RESULTSTR) ? 1 : result;
    }
    case WM_IME_CHAR:
        if (::IsWindowUnicode(hwnd) == FALSE)
        {
            if (::IsDBCSLeadByte(HIBYTE(wParam)))
                wParam = (WPARAM)MAKEWORD(HIBYTE(wParam), LOBYTE(wParam));
            wchar_t wch = 0;
            ::MultiByteToWideChar(bd->KeyboardCodePage, MB_PRECOMPOSED, (char*)&wParam, 2, &wch, 1);
            io.AddInputCharacterUTF16(wch);
            return 1;
        }
        return 0;
    case WM_SETCURSOR:
        // This is required to restore cursor when transitioning from e.g resize borders to client area.
        if (LOWORD(lParam) == HTCLIENT && HvkGui_ImplWin32_UpdateMouseCursor(io, bd->LastMouseCursor))
            return 1;
        return 0;
    case WM_DEVICECHANGE:
#ifndef HvkGui_IMPL_WIN32_DISABLE_GAMEPAD
        if ((UINT)wParam == DBT_DEVNODES_CHANGED)
            bd->WantUpdateHasGamepad = true;
#endif
        return 0;
    }
    return 0;
}


//--------------------------------------------------------------------------------------------------------
// DPI-related helpers (optional)
//--------------------------------------------------------------------------------------------------------
// - Use to enable DPI awareness without having to create an application manifest.
// - Your own app may already do this via a manifest or explicit calls. This is mostly useful for our examples/ apps.
// - In theory we could call simple functions from Windows SDK such as SetProcessDPIAware(), SetProcessDpiAwareness(), etc.
//   but most of the functions provided by Microsoft require Windows 8.1/10+ SDK at compile time and Windows 8/10+ at runtime,
//   neither we want to require the user to have. So we dynamically select and load those functions to avoid dependencies.
//---------------------------------------------------------------------------------------------------------
// This is the scheme successfully used by GLFW (from which we borrowed some of the code) and other apps aiming to be highly portable.
// HvkGui_ImplWin32_EnableDpiAwareness() is just a helper called by main.cpp, we don't call it automatically.
// If you are trying to Hvkplement your own backend for your own engine, you may ignore that noise.
//---------------------------------------------------------------------------------------------------------

// Perform our own check with RtlVerifyVersionInfo() instead of using functions from <VersionHelpers.h> as they
// require a manifest to be functional for checks above 8.1. See https://github.com/ocornut/HvkGui/issues/4200
static BOOL _IsWindowsVersionOrGreater(WORD major, WORD minor, WORD)
{
    typedef LONG(WINAPI* PFN_RtlVerifyVersionInfo)(OSVERSIONINFOEXW*, ULONG, ULONGLONG);
    static PFN_RtlVerifyVersionInfo RtlVerifyVersionInfoFn = nullptr;
    if (RtlVerifyVersionInfoFn == nullptr)
        if (HMODULE ntdllModule = ::GetModuleHandleA("ntdll.dll"))
            RtlVerifyVersionInfoFn = (PFN_RtlVerifyVersionInfo)GetProcAddress(ntdllModule, "RtlVerifyVersionInfo");
    if (RtlVerifyVersionInfoFn == nullptr)
        return FALSE;

    RTL_OSVERSIONINFOEXW versionInfo = { };
    ULONGLONG conditionMask = 0;
    versionInfo.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOEXW);
    versionInfo.dwMajorVersion = major;
    versionInfo.dwMinorVersion = minor;
    VER_SET_CONDITION(conditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
    VER_SET_CONDITION(conditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);
    return (RtlVerifyVersionInfoFn(&versionInfo, VER_MAJORVERSION | VER_MINORVERSION, conditionMask) == 0) ? TRUE : FALSE;
}

#define _IsWindowsVistaOrGreater()   _IsWindowsVersionOrGreater(HIBYTE(0x0600), LOBYTE(0x0600), 0) // _WIN32_WINNT_VISTA
#define _IsWindows8OrGreater()       _IsWindowsVersionOrGreater(HIBYTE(0x0602), LOBYTE(0x0602), 0) // _WIN32_WINNT_WIN8
#define _IsWindows8Point1OrGreater() _IsWindowsVersionOrGreater(HIBYTE(0x0603), LOBYTE(0x0603), 0) // _WIN32_WINNT_WINBLUE
#define _IsWindows10OrGreater()      _IsWindowsVersionOrGreater(HIBYTE(0x0A00), LOBYTE(0x0A00), 0) // _WIN32_WINNT_WINTHRESHOLD / _WIN32_WINNT_WIN10

#ifndef DPI_ENUMS_DECLARED
typedef enum { PROCESS_DPI_UNAWARE = 0, PROCESS_SYSTEM_DPI_AWARE = 1, PROCESS_PER_MONITOR_DPI_AWARE = 2 } PROCESS_DPI_AWARENESS;
typedef enum { MDT_EFFECTIVE_DPI = 0, MDT_ANGULAR_DPI = 1, MDT_RAW_DPI = 2, MDT_DEFAULT = MDT_EFFECTIVE_DPI } MONITOR_DPI_TYPE;
#endif
#ifndef _DPI_AWARENESS_CONTEXTS_
DECLARE_HANDLE(DPI_AWARENESS_CONTEXT);
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE    (DPI_AWARENESS_CONTEXT)-3
#endif
#ifndef DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 (DPI_AWARENESS_CONTEXT)-4
#endif
typedef HRESULT(WINAPI* PFN_SetProcessDpiAwareness)(PROCESS_DPI_AWARENESS);                     // Shcore.lib + dll, Windows 8.1+
typedef HRESULT(WINAPI* PFN_GetDpiForMonitor)(HMONITOR, MONITOR_DPI_TYPE, UINT*, UINT*);        // Shcore.lib + dll, Windows 8.1+
typedef DPI_AWARENESS_CONTEXT(WINAPI* PFN_SetThreadDpiAwarenessContext)(DPI_AWARENESS_CONTEXT); // User32.lib + dll, Windows 10 v1607+ (Creators Update)

// Helper function to enable DPI awareness without setting up a manifest
void HvkGui_ImplWin32_EnableDpiAwareness()
{
    if (_IsWindows10OrGreater())
    {
        static HINSTANCE user32_dll = ::LoadLibraryA("user32.dll"); // Reference counted per-process
        if (PFN_SetThreadDpiAwarenessContext SetThreadDpiAwarenessContextFn = (PFN_SetThreadDpiAwarenessContext)::GetProcAddress(user32_dll, "SetThreadDpiAwarenessContext"))
        {
            SetThreadDpiAwarenessContextFn(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
            return;
        }
    }
    if (_IsWindows8Point1OrGreater())
    {
        static HINSTANCE shcore_dll = ::LoadLibraryA("shcore.dll"); // Reference counted per-process
        if (PFN_SetProcessDpiAwareness SetProcessDpiAwarenessFn = (PFN_SetProcessDpiAwareness)::GetProcAddress(shcore_dll, "SetProcessDpiAwareness"))
        {
            SetProcessDpiAwarenessFn(PROCESS_PER_MONITOR_DPI_AWARE);
            return;
        }
    }
#if _WIN32_WINNT >= 0x0600
    ::SetProcessDPIAware();
#endif
}

#if defined(_MSC_VER) && !defined(NOGDI)
#pragma comment(lib, "gdi32")   // Link with gdi32.lib for GetDeviceCaps(). MinGW will require linking with '-lgdi32'
#endif

float HvkGui_ImplWin32_GetDpiScaleForMonitor(void* monitor)
{
    UINT xdpi = 96, ydpi = 96;
    if (_IsWindows8Point1OrGreater())
    {
        static HINSTANCE shcore_dll = ::LoadLibraryA("shcore.dll"); // Reference counted per-process
        static PFN_GetDpiForMonitor GetDpiForMonitorFn = nullptr;
        if (GetDpiForMonitorFn == nullptr && shcore_dll != nullptr)
            GetDpiForMonitorFn = (PFN_GetDpiForMonitor)::GetProcAddress(shcore_dll, "GetDpiForMonitor");
        if (GetDpiForMonitorFn != nullptr)
        {
            GetDpiForMonitorFn((HMONITOR)monitor, MDT_EFFECTIVE_DPI, &xdpi, &ydpi);
            IM_ASSERT(xdpi == ydpi); // Please contact me if you hit this assert!
            return xdpi / 96.0f;
        }
    }
#ifndef NOGDI
    const HDC dc = ::GetDC(nullptr);
    xdpi = ::GetDeviceCaps(dc, LOGPIXELSX);
    ydpi = ::GetDeviceCaps(dc, LOGPIXELSY);
    IM_ASSERT(xdpi == ydpi); // Please contact me if you hit this assert!
    ::ReleaseDC(nullptr, dc);
#endif
    return xdpi / 96.0f;
}

float HvkGui_ImplWin32_GetDpiScaleForHwnd(void* hwnd)
{
    HMONITOR monitor = ::MonitorFromWindow((HWND)hwnd, MONITOR_DEFAULTTONEAREST);
    return HvkGui_ImplWin32_GetDpiScaleForMonitor(monitor);
}

//---------------------------------------------------------------------------------------------------------
// Transparency related helpers (optional)
//--------------------------------------------------------------------------------------------------------

#if defined(_MSC_VER)
#pragma comment(lib, "dwmapi")  // Link with dwmapi.lib. MinGW will require linking with '-ldwmapi'
#endif

// [experimental]
// Borrowed from GLFW's function updateFramebufferTransparency() in src/win32_window.c
// (the Dwm* functions are Vista era functions but we are borrowing logic from GLFW)
void HvkGui_ImplWin32_EnableAlphaCompositing(void* hwnd)
{
    if (!_IsWindowsVistaOrGreater())
        return;

    BOOL composition;
    if (FAILED(::DwmIsCompositionEnabled(&composition)) || !composition)
        return;

    BOOL opaque;
    DWORD color;
    if (_IsWindows8OrGreater() || (SUCCEEDED(::DwmGetColorizationColor(&color, &opaque)) && !opaque))
    {
        HRGN region = ::CreateRectRgn(0, 0, -1, -1);
        DWM_BLURBEHIND bb = {};
        bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
        bb.hRgnBlur = region;
        bb.fEnable = TRUE;
        ::DwmEnableBlurBehindWindow((HWND)hwnd, &bb);
        ::DeleteObject(region);
    }
    else
    {
        DWM_BLURBEHIND bb = {};
        bb.dwFlags = DWM_BB_ENABLE;
        ::DwmEnableBlurBehindWindow((HWND)hwnd, &bb);
    }
}

//---------------------------------------------------------------------------------------------------------

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif // #ifndef HvkGui_DISABLE
