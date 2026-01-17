// dear imgui, v1.92.6 WIP
// (demo code)

// Help:
// - Read FAQ at http://dearimgui.com/faq
// - Call and read HvkGui::ShowDemoWindow() in imgui_demo.cpp. All applications in examples/ are doing that.
// - Need help integrating Dear ImGui in your codebase?
//   - Read Getting Started https://github.com/ocornut/imgui/wiki/Getting-Started
//   - Read 'Programmer guide' in imgui.cpp for notes on how to setup Dear ImGui in your codebase.
// Read top of imgui.cpp and imgui.h for many details, documentation, comments, links.
// Get the latest version at https://github.com/ocornut/imgui

// How to easily locate code?
// - Use Tools->Item Picker to debug break in code by clicking any widgets: https://github.com/ocornut/imgui/wiki/Debug-Tools
// - Browse an online version the demo with code linked to hovered widgets: https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html
// - Find a visible string and search for it in the code!

//---------------------------------------------------
// PLEASE DO NOT REMOVE THIS FILE FROM YOUR PROJECT!
//---------------------------------------------------
// Message to the person tempted to delete this file when integrating Dear ImGui into their codebase:
// Think again! It is the most useful reference code that you and other coders will want to refer to and call.
// Have the HvkGui::ShowDemoWindow() function wired in an always-available debug menu of your game/app!
// Also include Metrics! ItemPicker! DebugLog! and other debug features.
// Removing this file from your project is hindering access to documentation for everyone in your team,
// likely leading you to poorer usage of the library.
// Everything in this file will be stripped out by the linker if you don't call HvkGui::ShowDemoWindow().
// If you want to link core Dear ImGui in your shipped builds but want a thorough guarantee that the demo will not be
// linked, you can setup your imconfig.h with #define IMGUI_DISABLE_DEMO_WINDOWS and those functions will be empty.
// In another situation, whenever you have Dear ImGui available you probably want this to be available for reference.
// Thank you,
// -Your beloved friend, imgui_demo.cpp (which you won't delete)

//--------------------------------------------
// ABOUT THE MEANING OF THE 'static' KEYWORD:
//--------------------------------------------
// In this demo code, we frequently use 'static' variables inside functions.
// A static variable persists across calls. It is essentially a global variable but declared inside the scope of the function.
// Think of "static int n = 0;" as "global int n = 0;" !
// We do this IN THE DEMO because we want:
// - to gather code and data in the same place.
// - to make the demo source code faster to read, faster to change, smaller in size.
// - it is also a convenient way of storing simple UI related information as long as your function
//   doesn't need to be reentrant or used in multiple threads.
// This might be a pattern you will want to use in your code, but most of the data you would be working
// with in a complex codebase is likely going to be stored outside your functions.

//-----------------------------------------
// ABOUT THE CODING STYLE OF OUR DEMO CODE
//-----------------------------------------
// The Demo code in this file is designed to be easy to copy-and-paste into your application!
// Because of this:
// - We never omit the HvkGui:: prefix when calling functions, even though most code here is in the same namespace.
// - We try to declare static variables in the local scope, as close as possible to the code using them.
// - We never use any of the helpers/facilities used internally by Dear ImGui, unless available in the public API.
// - We never use maths operators on ImVec2/ImVec4. For our other sources files we use them, and they are provided
//   by imgui.h using the IMGUI_DEFINE_MATH_OPERATORS define. For your own sources file they are optional
//   and require you either enable those, either provide your own via IM_VEC2_CLASS_EXTRA in imconfig.h.
//   Because we can't assume anything about your support of maths operators, we cannot use them in imgui_demo.cpp.

// Navigating this file:
// - In Visual Studio: Ctrl+Comma ("Edit.GoToAll") can follow symbols inside comments, whereas Ctrl+F12 ("Edit.GoToImplementation") cannot.
// - In Visual Studio w/ Visual Assist installed: Alt+G ("VAssistX.GoToImplementation") can also follow symbols inside comments.
// - In VS Code, CLion, etc.: Ctrl+Click can follow symbols inside comments.
// - You can search/grep for all sections listed in the index to find the section.

/*

Index of this file:

// [SECTION] Forward Declarations
// [SECTION] Helpers
// [SECTION] Demo Window / ShowDemoWindow()
// [SECTION] DemoWindowMenuBar()
// [SECTION] Helpers: ExampleTreeNode, ExampleMemberInfo (for use by Property Editor & Multi-Select demos)
// [SECTION] DemoWindowWidgetsBasic()
// [SECTION] DemoWindowWidgetsBullets()
// [SECTION] DemoWindowWidgetsCollapsingHeaders()
// [SECTION] DemoWindowWidgetsComboBoxes()
// [SECTION] DemoWindowWidgetsColorAndPickers()
// [SECTION] DemoWindowWidgetsDataTypes()
// [SECTION] DemoWindowWidgetsDisableBlocks()
// [SECTION] DemoWindowWidgetsDragAndDrop()
// [SECTION] DemoWindowWidgetsDragsAndSliders()
// [SECTION] DemoWindowWidgetsFonts()
// [SECTION] DemoWindowWidgetsImages()
// [SECTION] DemoWindowWidgetsListBoxes()
// [SECTION] DemoWindowWidgetsMultiComponents()
// [SECTION] DemoWindowWidgetsPlotting()
// [SECTION] DemoWindowWidgetsProgressBars()
// [SECTION] DemoWindowWidgetsQueryingStatuses()
// [SECTION] DemoWindowWidgetsSelectables()
// [SECTION] DemoWindowWidgetsSelectionAndMultiSelect()
// [SECTION] DemoWindowWidgetsTabs()
// [SECTION] DemoWindowWidgetsText()
// [SECTION] DemoWindowWidgetsTextFilter()
// [SECTION] DemoWindowWidgetsTextInput()
// [SECTION] DemoWindowWidgetsTooltips()
// [SECTION] DemoWindowWidgetsTreeNodes()
// [SECTION] DemoWindowWidgetsVerticalSliders()
// [SECTION] DemoWindowWidgets()
// [SECTION] DemoWindowLayout()
// [SECTION] DemoWindowPopups()
// [SECTION] DemoWindowTables()
// [SECTION] DemoWindowInputs()
// [SECTION] About Window / ShowAboutWindow()
// [SECTION] Style Editor / ShowStyleEditor()
// [SECTION] User Guide / ShowUserGuide()
// [SECTION] Example App: Main Menu Bar / ShowExampleAppMainMenuBar()
// [SECTION] Example App: Debug Console / ShowExampleAppConsole()
// [SECTION] Example App: Debug Log / ShowExampleAppLog()
// [SECTION] Example App: Simple Layout / ShowExampleAppLayout()
// [SECTION] Example App: Property Editor / ShowExampleAppPropertyEditor()
// [SECTION] Example App: Long Text / ShowExampleAppLongText()
// [SECTION] Example App: Auto Resize / ShowExampleAppAutoResize()
// [SECTION] Example App: Constrained Resize / ShowExampleAppConstrainedResize()
// [SECTION] Example App: Simple overlay / ShowExampleAppSimpleOverlay()
// [SECTION] Example App: Fullscreen window / ShowExampleAppFullscreen()
// [SECTION] Example App: Manipulating window titles / ShowExampleAppWindowTitles()
// [SECTION] Example App: Custom Rendering using ImDrawList API / ShowExampleAppCustomRendering()
// [SECTION] Example App: Documents Handling / ShowExampleAppDocuments()
// [SECTION] Example App: Assets Browser / ShowExampleAppAssetsBrowser()

*/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui.h"
#ifndef IMGUI_DISABLE

// System includes
#include <ctype.h>          // toupper
#include <limits.h>         // INT_MIN, INT_MAX
#include <math.h>           // sqrtf, powf, cosf, sinf, floorf, ceilf
#include <stdio.h>          // vsnprintf, sscanf, printf
#include <stdlib.h>         // NULL, malloc, free, atoi
#include <stdint.h>         // intptr_t
#if !defined(_MSC_VER) || _MSC_VER >= 1800
#include <inttypes.h>       // PRId64/PRIu64, not avail in some MinGW headers.
#endif
#ifdef __EMSCRIPTEN__
#include <emscripten/version.h>     // __EMSCRIPTEN_major__ etc.
#endif

// Visual Studio warnings
#ifdef _MSC_VER
#pragma warning (disable: 4127)     // condition expression is constant
#pragma warning (disable: 4996)     // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#pragma warning (disable: 26451)    // [Static Analyzer] Arithmetic overflow : Using operator 'xxx' on a 4 byte value and then casting the result to an 8 byte value. Cast the value to the wider type before calling operator 'xxx' to avoid overflow(io.2).
#endif

// Clang/GCC warnings with -Weverything
#if defined(__clang__)
#if __has_warning("-Wunknown-warning-option")
#pragma clang diagnostic ignored "-Wunknown-warning-option"         // warning: unknown warning group 'xxx'                     // not all warnings are known by all Clang versions and they tend to be rename-happy.. so ignoring warnings triggers new warnings on some configuration. Great!
#endif
#pragma clang diagnostic ignored "-Wunknown-pragmas"                // warning: unknown warning group 'xxx'
#pragma clang diagnostic ignored "-Wold-style-cast"                 // warning: use of old-style cast                           // yes, they are more terse.
#pragma clang diagnostic ignored "-Wdeprecated-declarations"        // warning: 'xx' is deprecated: The POSIX name for this..   // for strdup used in demo code (so user can copy & paste the code)
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"       // warning: cast to 'void *' from smaller integer type
#pragma clang diagnostic ignored "-Wformat"                         // warning: format specifies type 'int' but the argument has type 'unsigned int'
#pragma clang diagnostic ignored "-Wformat-security"                // warning: format string is not a string literal
#pragma clang diagnostic ignored "-Wexit-time-destructors"          // warning: declaration requires an exit-time destructor    // exit-time destruction order is undefined. if MemFree() leads to users code that has been disabled before exit it might cause problems. ImGui coding style welcomes static/globals.
#pragma clang diagnostic ignored "-Wunused-macros"                  // warning: macro is not used                               // we define snprintf/vsnprintf on Windows so they are available, but not always used.
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"  // warning: zero as null pointer constant                   // some standard header variations use #define NULL 0
#pragma clang diagnostic ignored "-Wdouble-promotion"               // warning: implicit conversion from 'float' to 'double' when passing argument to function  // using printf() is a misery with this as C++ va_arg ellipsis changes float to double.
#pragma clang diagnostic ignored "-Wreserved-id-macro"              // warning: macro name is a reserved identifier
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"  // warning: implicit conversion from 'xxx' to 'float' may lose precision
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"            // warning: 'xxx' is an unsafe pointer used for buffer access
#pragma clang diagnostic ignored "-Wswitch-default"                 // warning: 'switch' missing 'default' label
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpragmas"                          // warning: unknown option after '#pragma GCC diagnostic' kind
#pragma GCC diagnostic ignored "-Wfloat-equal"                      // warning: comparing floating-point with '==' or '!=' is unsafe
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"              // warning: cast to pointer from integer of different size
#pragma GCC diagnostic ignored "-Wformat"                           // warning: format '%p' expects argument of type 'int'/'void*', but argument X has type 'unsigned int'/'ImGuiWindow*'
#pragma GCC diagnostic ignored "-Wformat-security"                  // warning: format string is not a string literal (potentially insecure)
#pragma GCC diagnostic ignored "-Wdouble-promotion"                 // warning: implicit conversion from 'float' to 'double' when passing argument to function
#pragma GCC diagnostic ignored "-Wconversion"                       // warning: conversion to 'xxxx' from 'xxxx' may alter its value
#pragma GCC diagnostic ignored "-Wmisleading-indentation"           // [__GNUC__ >= 6] warning: this 'if' clause does not guard this statement      // GCC 6.0+ only. See #883 on GitHub.
#pragma GCC diagnostic ignored "-Wstrict-overflow"                  // warning: assuming signed overflow does not occur when simplifying division / ..when changing X +- C1 cmp C2 to X cmp C2 -+ C1
#pragma GCC diagnostic ignored "-Wcast-qual"                        // warning: cast from type 'const xxxx *' to type 'xxxx *' casts away qualifiers
#endif

// Play it nice with Windows users (Update: May 2018, Notepad now supports Unix-style carriage returns!)
#ifdef _WIN32
#define IM_NEWLINE  "\r\n"
#else
#define IM_NEWLINE  "\n"
#endif

// Helpers
#if defined(_MSC_VER) && !defined(snprintf)
#define snprintf    _snprintf
#endif
#if defined(_MSC_VER) && !defined(vsnprintf)
#define vsnprintf   _vsnprintf
#endif

// Format specifiers for 64-bit values (hasn't been decently standardized before VS2013)
#if !defined(PRId64) && defined(_MSC_VER)
#define PRId64 "I64d"
#define PRIu64 "I64u"
#elif !defined(PRId64)
#define PRId64 "lld"
#define PRIu64 "llu"
#endif

// Helpers macros
// We normally try to not use many helpers in imgui_demo.cpp in order to make code easier to copy and paste,
// but making an exception here as those are largely simplifying code...
// In other imgui sources we can use nicer internal functions from imgui_internal.h (ImMin/ImMax) but not in the demo.
#define IM_MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

// Enforce cdecl calling convention for functions called by the standard library,
// in case compilation settings changed the default to e.g. __vectorcall
#ifndef IMGUI_CDECL
#ifdef _MSC_VER
#define IMGUI_CDECL __cdecl
#else
#define IMGUI_CDECL
#endif
#endif

//-----------------------------------------------------------------------------
// [SECTION] Forward Declarations
//-----------------------------------------------------------------------------

#if !defined(IMGUI_DISABLE_DEMO_WINDOWS)

// Forward Declarations
struct ImGuiDemoWindowData;
static void ShowExampleAppMainMenuBar();
static void ShowExampleAppAssetsBrowser(bool* p_open);
static void ShowExampleAppConsole(bool* p_open);
static void ShowExampleAppCustomRendering(bool* p_open);
static void ShowExampleAppDocuments(bool* p_open);
static void ShowExampleAppLog(bool* p_open);
static void ShowExampleAppLayout(bool* p_open);
static void ShowExampleAppPropertyEditor(bool* p_open, ImGuiDemoWindowData* demo_data);
static void ShowExampleAppSimpleOverlay(bool* p_open);
static void ShowExampleAppAutoResize(bool* p_open);
static void ShowExampleAppConstrainedResize(bool* p_open);
static void ShowExampleAppFullscreen(bool* p_open);
static void ShowExampleAppLongText(bool* p_open);
static void ShowExampleAppWindowTitles(bool* p_open);
static void ShowExampleMenuFile();

// We split the contents of the big ShowDemoWindow() function into smaller functions
// (because the link time of very large functions tends to grow non-linearly)
static void DemoWindowMenuBar(ImGuiDemoWindowData* demo_data);
static void DemoWindowWidgets(ImGuiDemoWindowData* demo_data);
static void DemoWindowLayout();
static void DemoWindowPopups();
static void DemoWindowTables();
static void DemoWindowColumns();
static void DemoWindowInputs();

// Helper tree functions used by Property Editor & Multi-Select demos
struct ExampleTreeNode;
static ExampleTreeNode* ExampleTree_CreateNode(const char* name, int uid, ExampleTreeNode* parent);
static void             ExampleTree_DestroyNode(ExampleTreeNode* node);

//-----------------------------------------------------------------------------
// [SECTION] Helpers
//-----------------------------------------------------------------------------

// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
static void HelpMarker(const char* desc)
{
    HvkGui::TextDisabled("(?)");
    if (HvkGui::BeginItemTooltip())
    {
        HvkGui::PushTextWrapPos(HvkGui::GetFontSize() * 35.0f);
        HvkGui::TextUnformatted(desc);
        HvkGui::PopTextWrapPos();
        HvkGui::EndTooltip();
    }
}

// Helper to wire demo markers located in code to an interactive browser
typedef void (*ImGuiDemoMarkerCallback)(const char* file, int line, const char* section, void* user_data);
extern ImGuiDemoMarkerCallback      GImGuiDemoMarkerCallback;
extern void*                        GImGuiDemoMarkerCallbackUserData;
ImGuiDemoMarkerCallback             GImGuiDemoMarkerCallback = NULL;
void*                               GImGuiDemoMarkerCallbackUserData = NULL;
#define IMGUI_DEMO_MARKER(section)  do { if (GImGuiDemoMarkerCallback != NULL) GImGuiDemoMarkerCallback("imgui_demo.cpp", __LINE__, section, GImGuiDemoMarkerCallbackUserData); } while (0)

//-----------------------------------------------------------------------------
// [SECTION] Demo Window / ShowDemoWindow()
//-----------------------------------------------------------------------------

// Data to be shared across different functions of the demo.
struct ImGuiDemoWindowData
{
    // Examples Apps (accessible from the "Examples" menu)
    bool ShowMainMenuBar = false;
    bool ShowAppAssetsBrowser = false;
    bool ShowAppConsole = false;
    bool ShowAppCustomRendering = false;
    bool ShowAppDocuments = false;
    bool ShowAppLog = false;
    bool ShowAppLayout = false;
    bool ShowAppPropertyEditor = false;
    bool ShowAppSimpleOverlay = false;
    bool ShowAppAutoResize = false;
    bool ShowAppConstrainedResize = false;
    bool ShowAppFullscreen = false;
    bool ShowAppLongText = false;
    bool ShowAppWindowTitles = false;

    // Dear ImGui Tools (accessible from the "Tools" menu)
    bool ShowMetrics = false;
    bool ShowDebugLog = false;
    bool ShowIDStackTool = false;
    bool ShowStyleEditor = false;
    bool ShowAbout = false;

    // Other data
    bool DisableSections = false;
    ExampleTreeNode* DemoTree = NULL;

    ~ImGuiDemoWindowData() { if (DemoTree) ExampleTree_DestroyNode(DemoTree); }
};

// Demonstrate most Dear ImGui features (this is big function!)
// You may execute this function to experiment with the UI and understand what it does.
// You may then search for keywords in the code when you are interested by a specific feature.
void HvkGui::ShowDemoWindow(bool* p_open)
{
    // Exceptionally add an extra assert here for people confused about initial Dear ImGui setup
    // Most functions would normally just assert/crash if the context is missing.
    IM_ASSERT(HvkGui::GetCurrentContext() != NULL && "Missing Dear ImGui context. Refer to examples app!");

    // Verify ABI compatibility between caller code and compiled version of Dear ImGui. This helps detects some build issues.
    IMGUI_CHECKVERSION();

    // Stored data
    static ImGuiDemoWindowData demo_data;

    // Examples Apps (accessible from the "Examples" menu)
    if (demo_data.ShowMainMenuBar)          { ShowExampleAppMainMenuBar(); }
    if (demo_data.ShowAppDocuments)         { ShowExampleAppDocuments(&demo_data.ShowAppDocuments); }
    if (demo_data.ShowAppAssetsBrowser)     { ShowExampleAppAssetsBrowser(&demo_data.ShowAppAssetsBrowser); }
    if (demo_data.ShowAppConsole)           { ShowExampleAppConsole(&demo_data.ShowAppConsole); }
    if (demo_data.ShowAppCustomRendering)   { ShowExampleAppCustomRendering(&demo_data.ShowAppCustomRendering); }
    if (demo_data.ShowAppLog)               { ShowExampleAppLog(&demo_data.ShowAppLog); }
    if (demo_data.ShowAppLayout)            { ShowExampleAppLayout(&demo_data.ShowAppLayout); }
    if (demo_data.ShowAppPropertyEditor)    { ShowExampleAppPropertyEditor(&demo_data.ShowAppPropertyEditor, &demo_data); }
    if (demo_data.ShowAppSimpleOverlay)     { ShowExampleAppSimpleOverlay(&demo_data.ShowAppSimpleOverlay); }
    if (demo_data.ShowAppAutoResize)        { ShowExampleAppAutoResize(&demo_data.ShowAppAutoResize); }
    if (demo_data.ShowAppConstrainedResize) { ShowExampleAppConstrainedResize(&demo_data.ShowAppConstrainedResize); }
    if (demo_data.ShowAppFullscreen)        { ShowExampleAppFullscreen(&demo_data.ShowAppFullscreen); }
    if (demo_data.ShowAppLongText)          { ShowExampleAppLongText(&demo_data.ShowAppLongText); }
    if (demo_data.ShowAppWindowTitles)      { ShowExampleAppWindowTitles(&demo_data.ShowAppWindowTitles); }

    // Dear ImGui Tools (accessible from the "Tools" menu)
    if (demo_data.ShowMetrics)              { HvkGui::ShowMetricsWindow(&demo_data.ShowMetrics); }
    if (demo_data.ShowDebugLog)             { HvkGui::ShowDebugLogWindow(&demo_data.ShowDebugLog); }
    if (demo_data.ShowIDStackTool)          { HvkGui::ShowIDStackToolWindow(&demo_data.ShowIDStackTool); }
    if (demo_data.ShowAbout)                { HvkGui::ShowAboutWindow(&demo_data.ShowAbout); }
    if (demo_data.ShowStyleEditor)
    {
        HvkGui::Begin("Dear ImGui Style Editor", &demo_data.ShowStyleEditor);
        HvkGui::ShowStyleEditor();
        HvkGui::End();
    }

    // Demonstrate the various window flags. Typically you would just use the default!
    static bool no_titlebar = false;
    static bool no_scrollbar = false;
    static bool no_menu = false;
    static bool no_move = false;
    static bool no_resize = false;
    static bool no_collapse = false;
    static bool no_close = false;
    static bool no_nav = false;
    static bool no_background = false;
    static bool no_bring_to_front = false;
    static bool unsaved_document = false;

    ImGuiWindowFlags window_flags = 0;
    if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
    if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
    if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
    if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
    if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
    if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
    if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;
    if (no_close)           p_open = NULL; // Don't pass our bool* to Begin

    // We specify a default position/size in case there's no data in the .ini file.
    // We only do it to make the demo applications a little more welcoming, but typically this isn't required.
    const ImGuiViewport* main_viewport = HvkGui::GetMainViewport();
    HvkGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    HvkGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

    // Main body of the Demo window starts here.
    if (!HvkGui::Begin("Dear ImGui Demo", p_open, window_flags))
    {
        // Early out if the window is collapsed, as an optimization.
        HvkGui::End();
        return;
    }

    // Most framed widgets share a common width settings. Remaining width is used for the label.
    // The width of the frame may be changed with PushItemWidth() or SetNextItemWidth().
    // - Positive value for absolute size, negative value for right-alignment.
    // - The default value is about GetWindowWidth() * 0.65f.
    // - See 'Demo->Layout->Widgets Width' for details.
    // Here we change the frame width based on how much width we want to give to the label.
    const float label_width_base = HvkGui::GetFontSize() * 12;               // Some amount of width for label, based on font size.
    const float label_width_max = HvkGui::GetContentRegionAvail().x * 0.40f; // ...but always leave some room for framed widgets.
    const float label_width = IM_MIN(label_width_base, label_width_max);
    HvkGui::PushItemWidth(-label_width);                                     // Right-align: framed items will leave 'label_width' available for the label.
    //HvkGui::PushItemWidth(HvkGui::GetContentRegionAvail().x * 0.40f);       // e.g. Use 40% width for framed widgets, leaving 60% width for labels.
    //HvkGui::PushItemWidth(-HvkGui::GetContentRegionAvail().x * 0.40f);      // e.g. Use 40% width for labels, leaving 60% width for framed widgets.
    //HvkGui::PushItemWidth(HvkGui::GetFontSize() * -12);                     // e.g. Use XXX width for labels, leaving the rest for framed widgets.

    // Menu Bar
    DemoWindowMenuBar(&demo_data);

    HvkGui::Text("dear imgui says hello! (%s) (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
    HvkGui::Spacing();

    IMGUI_DEMO_MARKER("Help");
    if (HvkGui::CollapsingHeader("Help"))
    {
        HvkGui::SeparatorText("ABOUT THIS DEMO:");
        HvkGui::BulletText("Sections below are demonstrating many aspects of the library.");
        HvkGui::BulletText("The \"Examples\" menu above leads to more demo contents.");
        HvkGui::BulletText("The \"Tools\" menu above gives access to: About Box, Style Editor,\n"
                          "and Metrics/Debugger (general purpose Dear ImGui debugging tool).");

        HvkGui::SeparatorText("PROGRAMMER GUIDE:");
        HvkGui::BulletText("See the ShowDemoWindow() code in imgui_demo.cpp. <- you are here!");
        HvkGui::BulletText("See comments in imgui.cpp.");
        HvkGui::BulletText("See example applications in the examples/ folder.");
        HvkGui::BulletText("Read the FAQ at ");
        HvkGui::SameLine(0, 0);
        HvkGui::TextLinkOpenURL("https://www.dearimgui.com/faq/");
        HvkGui::BulletText("Set 'io.ConfigFlags |= NavEnableKeyboard' for keyboard controls.");
        HvkGui::BulletText("Set 'io.ConfigFlags |= NavEnableGamepad' for gamepad controls.");

        HvkGui::SeparatorText("USER GUIDE:");
        HvkGui::ShowUserGuide();
    }

    IMGUI_DEMO_MARKER("Configuration");
    if (HvkGui::CollapsingHeader("Configuration"))
    {
        ImGuiIO& io = HvkGui::GetIO();

        if (HvkGui::TreeNode("Configuration##2"))
        {
            HvkGui::SeparatorText("General");
            HvkGui::CheckboxFlags("io.ConfigFlags: NavEnableKeyboard",    &io.ConfigFlags, ImGuiConfigFlags_NavEnableKeyboard);
            HvkGui::SameLine(); HelpMarker("Enable keyboard controls.");
            HvkGui::CheckboxFlags("io.ConfigFlags: NavEnableGamepad",     &io.ConfigFlags, ImGuiConfigFlags_NavEnableGamepad);
            HvkGui::SameLine(); HelpMarker("Enable gamepad controls. Require backend to set io.BackendFlags |= ImGuiBackendFlags_HasGamepad.\n\nRead instructions in imgui.cpp for details.");
            HvkGui::CheckboxFlags("io.ConfigFlags: NoMouse",              &io.ConfigFlags, ImGuiConfigFlags_NoMouse);
            HvkGui::SameLine(); HelpMarker("Instruct dear imgui to disable mouse inputs and interactions.");

            // The "NoMouse" option can get us stuck with a disabled mouse! Let's provide an alternative way to fix it:
            if (io.ConfigFlags & ImGuiConfigFlags_NoMouse)
            {
                if (fmodf((float)HvkGui::GetTime(), 0.40f) < 0.20f)
                {
                    HvkGui::SameLine();
                    HvkGui::Text("<<PRESS SPACE TO DISABLE>>");
                }
                // Prevent both being checked
                if (HvkGui::IsKeyPressed(ImGuiKey_Space) || (io.ConfigFlags & ImGuiConfigFlags_NoKeyboard))
                    io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
            }

            HvkGui::CheckboxFlags("io.ConfigFlags: NoMouseCursorChange",  &io.ConfigFlags, ImGuiConfigFlags_NoMouseCursorChange);
            HvkGui::SameLine(); HelpMarker("Instruct backend to not alter mouse cursor shape and visibility.");
            HvkGui::CheckboxFlags("io.ConfigFlags: NoKeyboard", &io.ConfigFlags, ImGuiConfigFlags_NoKeyboard);
            HvkGui::SameLine(); HelpMarker("Instruct dear imgui to disable keyboard inputs and interactions.");

            HvkGui::Checkbox("io.ConfigInputTrickleEventQueue", &io.ConfigInputTrickleEventQueue);
            HvkGui::SameLine(); HelpMarker("Enable input queue trickling: some types of events submitted during the same frame (e.g. button down + up) will be spread over multiple frames, improving interactions with low framerates.");
            HvkGui::Checkbox("io.MouseDrawCursor", &io.MouseDrawCursor);
            HvkGui::SameLine(); HelpMarker("Instruct Dear ImGui to render a mouse cursor itself. Note that a mouse cursor rendered via your application GPU rendering path will feel more laggy than hardware cursor, but will be more in sync with your other visuals.\n\nSome desktop applications may use both kinds of cursors (e.g. enable software cursor only when resizing/dragging something).");

            HvkGui::SeparatorText("Keyboard/Gamepad Navigation");
            HvkGui::Checkbox("io.ConfigNavSwapGamepadButtons", &io.ConfigNavSwapGamepadButtons);
            HvkGui::Checkbox("io.ConfigNavMoveSetMousePos", &io.ConfigNavMoveSetMousePos);
            HvkGui::SameLine(); HelpMarker("Directional/tabbing navigation teleports the mouse cursor. May be useful on TV/console systems where moving a virtual mouse is difficult");
            HvkGui::Checkbox("io.ConfigNavCaptureKeyboard", &io.ConfigNavCaptureKeyboard);
            HvkGui::Checkbox("io.ConfigNavEscapeClearFocusItem", &io.ConfigNavEscapeClearFocusItem);
            HvkGui::SameLine(); HelpMarker("Pressing Escape clears focused item.");
            HvkGui::Checkbox("io.ConfigNavEscapeClearFocusWindow", &io.ConfigNavEscapeClearFocusWindow);
            HvkGui::SameLine(); HelpMarker("Pressing Escape clears focused window.");
            HvkGui::Checkbox("io.ConfigNavCursorVisibleAuto", &io.ConfigNavCursorVisibleAuto);
            HvkGui::SameLine(); HelpMarker("Using directional navigation key makes the cursor visible. Mouse click hides the cursor.");
            HvkGui::Checkbox("io.ConfigNavCursorVisibleAlways", &io.ConfigNavCursorVisibleAlways);
            HvkGui::SameLine(); HelpMarker("Navigation cursor is always visible.");

            HvkGui::SeparatorText("Windows");
            HvkGui::Checkbox("io.ConfigWindowsResizeFromEdges", &io.ConfigWindowsResizeFromEdges);
            HvkGui::SameLine(); HelpMarker("Enable resizing of windows from their edges and from the lower-left corner.\nThis requires ImGuiBackendFlags_HasMouseCursors for better mouse cursor feedback.");
            HvkGui::Checkbox("io.ConfigWindowsMoveFromTitleBarOnly", &io.ConfigWindowsMoveFromTitleBarOnly);
            HvkGui::Checkbox("io.ConfigWindowsCopyContentsWithCtrlC", &io.ConfigWindowsCopyContentsWithCtrlC); // [EXPERIMENTAL]
            HvkGui::SameLine(); HelpMarker("*EXPERIMENTAL* Ctrl+C copy the contents of focused window into the clipboard.\n\nExperimental because:\n- (1) has known issues with nested Begin/End pairs.\n- (2) text output quality varies.\n- (3) text output is in submission order rather than spatial order.");
            HvkGui::Checkbox("io.ConfigScrollbarScrollByPage", &io.ConfigScrollbarScrollByPage);
            HvkGui::SameLine(); HelpMarker("Enable scrolling page by page when clicking outside the scrollbar grab.\nWhen disabled, always scroll to clicked location.\nWhen enabled, Shift+Click scrolls to clicked location.");

            HvkGui::SeparatorText("Widgets");
            HvkGui::Checkbox("io.ConfigInputTextCursorBlink", &io.ConfigInputTextCursorBlink);
            HvkGui::SameLine(); HelpMarker("Enable blinking cursor (optional as some users consider it to be distracting).");
            HvkGui::Checkbox("io.ConfigInputTextEnterKeepActive", &io.ConfigInputTextEnterKeepActive);
            HvkGui::SameLine(); HelpMarker("Pressing Enter will keep item active and select contents (single-line only).");
            HvkGui::Checkbox("io.ConfigDragClickToInputText", &io.ConfigDragClickToInputText);
            HvkGui::SameLine(); HelpMarker("Enable turning DragXXX widgets into text input with a simple mouse click-release (without moving).");
            HvkGui::Checkbox("io.ConfigMacOSXBehaviors", &io.ConfigMacOSXBehaviors);
            HvkGui::SameLine(); HelpMarker("Swap Cmd<>Ctrl keys, enable various MacOS style behaviors.");
            HvkGui::Text("Also see Style->Rendering for rendering options.");

            // Also read: https://github.com/ocornut/imgui/wiki/Error-Handling
            HvkGui::SeparatorText("Error Handling");

            HvkGui::Checkbox("io.ConfigErrorRecovery", &io.ConfigErrorRecovery);
            HvkGui::SameLine(); HelpMarker(
                "Options to configure how we handle recoverable errors.\n"
                "- Error recovery is not perfect nor guaranteed! It is a feature to ease development.\n"
                "- You not are not supposed to rely on it in the course of a normal application run.\n"
                "- Possible usage: facilitate recovery from errors triggered from a scripting language or after specific exceptions handlers.\n"
                "- Always ensure that on programmers seat you have at minimum Asserts or Tooltips enabled when making direct imgui API call! "
                "Otherwise it would severely hinder your ability to catch and correct mistakes!");
            HvkGui::Checkbox("io.ConfigErrorRecoveryEnableAssert", &io.ConfigErrorRecoveryEnableAssert);
            HvkGui::Checkbox("io.ConfigErrorRecoveryEnableDebugLog", &io.ConfigErrorRecoveryEnableDebugLog);
            HvkGui::Checkbox("io.ConfigErrorRecoveryEnableTooltip", &io.ConfigErrorRecoveryEnableTooltip);
            if (!io.ConfigErrorRecoveryEnableAssert && !io.ConfigErrorRecoveryEnableDebugLog && !io.ConfigErrorRecoveryEnableTooltip)
                io.ConfigErrorRecoveryEnableAssert = io.ConfigErrorRecoveryEnableDebugLog = io.ConfigErrorRecoveryEnableTooltip = true;

            // Also read: https://github.com/ocornut/imgui/wiki/Debug-Tools
            HvkGui::SeparatorText("Debug");
            HvkGui::Checkbox("io.ConfigDebugIsDebuggerPresent", &io.ConfigDebugIsDebuggerPresent);
            HvkGui::SameLine(); HelpMarker("Enable various tools calling IM_DEBUG_BREAK().\n\nRequires a debugger being attached, otherwise IM_DEBUG_BREAK() options will appear to crash your application.");
            HvkGui::Checkbox("io.ConfigDebugHighlightIdConflicts", &io.ConfigDebugHighlightIdConflicts);
            HvkGui::SameLine(); HelpMarker("Highlight and show an error message when multiple items have conflicting identifiers.");
            HvkGui::BeginDisabled();
            HvkGui::Checkbox("io.ConfigDebugBeginReturnValueOnce", &io.ConfigDebugBeginReturnValueOnce);
            HvkGui::EndDisabled();
            HvkGui::SameLine(); HelpMarker("First calls to Begin()/BeginChild() will return false.\n\nTHIS OPTION IS DISABLED because it needs to be set at application boot-time to make sense. Showing the disabled option is a way to make this feature easier to discover.");
            HvkGui::Checkbox("io.ConfigDebugBeginReturnValueLoop", &io.ConfigDebugBeginReturnValueLoop);
            HvkGui::SameLine(); HelpMarker("Some calls to Begin()/BeginChild() will return false.\n\nWill cycle through window depths then repeat. Windows should be flickering while running.");
            HvkGui::Checkbox("io.ConfigDebugIgnoreFocusLoss", &io.ConfigDebugIgnoreFocusLoss);
            HvkGui::SameLine(); HelpMarker("Option to deactivate io.AddFocusEvent(false) handling. May facilitate interactions with a debugger when focus loss leads to clearing inputs data.");
            HvkGui::Checkbox("io.ConfigDebugIniSettings", &io.ConfigDebugIniSettings);
            HvkGui::SameLine(); HelpMarker("Option to save .ini data with extra comments (particularly helpful for Docking, but makes saving slower).");

            HvkGui::TreePop();
            HvkGui::Spacing();
        }

        IMGUI_DEMO_MARKER("Configuration/Backend Flags");
        if (HvkGui::TreeNode("Backend Flags"))
        {
            HelpMarker(
                "Those flags are set by the backends (imgui_impl_xxx files) to specify their capabilities.\n"
                "Here we expose them as read-only fields to avoid breaking interactions with your backend.");

            // FIXME: Maybe we need a BeginReadonly() equivalent to keep label bright?
            HvkGui::BeginDisabled();
            HvkGui::CheckboxFlags("io.BackendFlags: HasGamepad",           &io.BackendFlags, ImGuiBackendFlags_HasGamepad);
            HvkGui::CheckboxFlags("io.BackendFlags: HasMouseCursors",      &io.BackendFlags, ImGuiBackendFlags_HasMouseCursors);
            HvkGui::CheckboxFlags("io.BackendFlags: HasSetMousePos",       &io.BackendFlags, ImGuiBackendFlags_HasSetMousePos);
            HvkGui::CheckboxFlags("io.BackendFlags: RendererHasVtxOffset", &io.BackendFlags, ImGuiBackendFlags_RendererHasVtxOffset);
            HvkGui::CheckboxFlags("io.BackendFlags: RendererHasTextures",  &io.BackendFlags, ImGuiBackendFlags_RendererHasTextures);
            HvkGui::EndDisabled();

            HvkGui::TreePop();
            HvkGui::Spacing();
        }

        IMGUI_DEMO_MARKER("Configuration/Style, Fonts");
        if (HvkGui::TreeNode("Style, Fonts"))
        {
            HvkGui::Checkbox("Style Editor", &demo_data.ShowStyleEditor);
            HvkGui::SameLine();
            HelpMarker("The same contents can be accessed in 'Tools->Style Editor' or by calling the ShowStyleEditor() function.");
            HvkGui::TreePop();
            HvkGui::Spacing();
        }

        IMGUI_DEMO_MARKER("Configuration/Capture, Logging");
        if (HvkGui::TreeNode("Capture/Logging"))
        {
            HelpMarker(
                "The logging API redirects all text output so you can easily capture the content of "
                "a window or a block. Tree nodes can be automatically expanded.\n"
                "Try opening any of the contents below in this window and then click one of the \"Log To\" button.");
            HvkGui::LogButtons();

            HelpMarker("You can also call HvkGui::LogText() to output directly to the log without a visual output.");
            if (HvkGui::Button("Copy \"Hello, world!\" to clipboard"))
            {
                HvkGui::LogToClipboard();
                HvkGui::LogText("Hello, world!");
                HvkGui::LogFinish();
            }
            HvkGui::TreePop();
        }
    }

    IMGUI_DEMO_MARKER("Window options");
    if (HvkGui::CollapsingHeader("Window options"))
    {
        if (HvkGui::BeginTable("split", 3))
        {
            HvkGui::TableNextColumn(); HvkGui::Checkbox("No titlebar", &no_titlebar);
            HvkGui::TableNextColumn(); HvkGui::Checkbox("No scrollbar", &no_scrollbar);
            HvkGui::TableNextColumn(); HvkGui::Checkbox("No menu", &no_menu);
            HvkGui::TableNextColumn(); HvkGui::Checkbox("No move", &no_move);
            HvkGui::TableNextColumn(); HvkGui::Checkbox("No resize", &no_resize);
            HvkGui::TableNextColumn(); HvkGui::Checkbox("No collapse", &no_collapse);
            HvkGui::TableNextColumn(); HvkGui::Checkbox("No close", &no_close);
            HvkGui::TableNextColumn(); HvkGui::Checkbox("No nav", &no_nav);
            HvkGui::TableNextColumn(); HvkGui::Checkbox("No background", &no_background);
            HvkGui::TableNextColumn(); HvkGui::Checkbox("No bring to front", &no_bring_to_front);
            HvkGui::TableNextColumn(); HvkGui::Checkbox("Unsaved document", &unsaved_document);
            HvkGui::EndTable();
        }
    }

    // All demo contents
    DemoWindowWidgets(&demo_data);
    DemoWindowLayout();
    DemoWindowPopups();
    DemoWindowTables();
    DemoWindowInputs();

    // End of ShowDemoWindow()
    HvkGui::PopItemWidth();
    HvkGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowMenuBar()
//-----------------------------------------------------------------------------

static void DemoWindowMenuBar(ImGuiDemoWindowData* demo_data)
{
    IMGUI_DEMO_MARKER("Menu");
    if (HvkGui::BeginMenuBar())
    {
        if (HvkGui::BeginMenu("Menu"))
        {
            IMGUI_DEMO_MARKER("Menu/File");
            ShowExampleMenuFile();
            HvkGui::EndMenu();
        }
        if (HvkGui::BeginMenu("Examples"))
        {
            IMGUI_DEMO_MARKER("Menu/Examples");
            HvkGui::MenuItem("Main menu bar", NULL, &demo_data->ShowMainMenuBar);

            HvkGui::SeparatorText("Mini apps");
            HvkGui::MenuItem("Assets Browser", NULL, &demo_data->ShowAppAssetsBrowser);
            HvkGui::MenuItem("Console", NULL, &demo_data->ShowAppConsole);
            HvkGui::MenuItem("Custom rendering", NULL, &demo_data->ShowAppCustomRendering);
            HvkGui::MenuItem("Documents", NULL, &demo_data->ShowAppDocuments);
            HvkGui::MenuItem("Log", NULL, &demo_data->ShowAppLog);
            HvkGui::MenuItem("Property editor", NULL, &demo_data->ShowAppPropertyEditor);
            HvkGui::MenuItem("Simple layout", NULL, &demo_data->ShowAppLayout);
            HvkGui::MenuItem("Simple overlay", NULL, &demo_data->ShowAppSimpleOverlay);

            HvkGui::SeparatorText("Concepts");
            HvkGui::MenuItem("Auto-resizing window", NULL, &demo_data->ShowAppAutoResize);
            HvkGui::MenuItem("Constrained-resizing window", NULL, &demo_data->ShowAppConstrainedResize);
            HvkGui::MenuItem("Fullscreen window", NULL, &demo_data->ShowAppFullscreen);
            HvkGui::MenuItem("Long text display", NULL, &demo_data->ShowAppLongText);
            HvkGui::MenuItem("Manipulating window titles", NULL, &demo_data->ShowAppWindowTitles);

            HvkGui::EndMenu();
        }
        //if (HvkGui::MenuItem("MenuItem")) {} // You can also use MenuItem() inside a menu bar!
        if (HvkGui::BeginMenu("Tools"))
        {
            IMGUI_DEMO_MARKER("Menu/Tools");
            ImGuiIO& io = HvkGui::GetIO();
#ifndef IMGUI_DISABLE_DEBUG_TOOLS
            const bool has_debug_tools = true;
#else
            const bool has_debug_tools = false;
#endif
            HvkGui::MenuItem("Metrics/Debugger", NULL, &demo_data->ShowMetrics, has_debug_tools);
            if (HvkGui::BeginMenu("Debug Options"))
            {
                HvkGui::BeginDisabled(!has_debug_tools);
                HvkGui::Checkbox("Highlight ID Conflicts", &io.ConfigDebugHighlightIdConflicts);
                HvkGui::EndDisabled();
                HvkGui::Checkbox("Assert on error recovery", &io.ConfigErrorRecoveryEnableAssert);
                HvkGui::TextDisabled("(see Demo->Configuration for details & more)");
                HvkGui::EndMenu();
            }
            HvkGui::MenuItem("Debug Log", NULL, &demo_data->ShowDebugLog, has_debug_tools);
            HvkGui::MenuItem("ID Stack Tool", NULL, &demo_data->ShowIDStackTool, has_debug_tools);
            bool is_debugger_present = io.ConfigDebugIsDebuggerPresent;
            if (HvkGui::MenuItem("Item Picker", NULL, false, has_debug_tools))// && is_debugger_present))
                HvkGui::DebugStartItemPicker();
            if (!is_debugger_present)
                HvkGui::SetItemTooltip("Requires io.ConfigDebugIsDebuggerPresent=true to be set.\n\nWe otherwise disable some extra features to avoid casual users crashing the application.");
            HvkGui::MenuItem("Style Editor", NULL, &demo_data->ShowStyleEditor);
            HvkGui::MenuItem("About Dear ImGui", NULL, &demo_data->ShowAbout);

            HvkGui::EndMenu();
        }
        HvkGui::EndMenuBar();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] Helpers: ExampleTreeNode, ExampleMemberInfo (for use by Property Editor & Multi-Select demos)
//-----------------------------------------------------------------------------

// Simple representation for a tree
// (this is designed to be simple to understand for our demos, not to be fancy or efficient etc.)
struct ExampleTreeNode
{
    // Tree structure
    char                        Name[28] = "";
    int                         UID = 0;
    ExampleTreeNode* Parent = NULL;
    ImVector<ExampleTreeNode*>  Childs;
    unsigned short              IndexInParent = 0;  // Maintaining this allows us to implement linear traversal more easily

    // Leaf Data
    bool                        HasData = false;    // All leaves have data
    bool                        DataMyBool = true;
    int                         DataMyInt = 128;
    ImVec2                      DataMyVec2 = ImVec2(0.0f, 3.141592f);
};

// Simple representation of struct metadata/serialization data.
// (this is a minimal version of what a typical advanced application may provide)
struct ExampleMemberInfo
{
    const char* Name;       // Member name
    ImGuiDataType   DataType;   // Member type
    int             DataCount;  // Member count (1 when scalar)
    int             Offset;     // Offset inside parent structure
};

// Metadata description of ExampleTreeNode struct.
static const ExampleMemberInfo ExampleTreeNodeMemberInfos[]
{
    { "MyName",     ImGuiDataType_String,  1, offsetof(ExampleTreeNode, Name) },
    { "MyBool",     ImGuiDataType_Bool,    1, offsetof(ExampleTreeNode, DataMyBool) },
    { "MyInt",      ImGuiDataType_S32,     1, offsetof(ExampleTreeNode, DataMyInt) },
    { "MyVec2",     ImGuiDataType_Float,   2, offsetof(ExampleTreeNode, DataMyVec2) },
};

static ExampleTreeNode* ExampleTree_CreateNode(const char* name, int uid, ExampleTreeNode* parent)
{
    ExampleTreeNode* node = IM_NEW(ExampleTreeNode);
    snprintf(node->Name, IM_ARRAYSIZE(node->Name), "%s", name);
    node->UID = uid;
    node->Parent = parent;
    node->IndexInParent = parent ? (unsigned short)parent->Childs.Size : 0;
    if (parent)
        parent->Childs.push_back(node);
    return node;
}

static void ExampleTree_DestroyNode(ExampleTreeNode* node)
{
    for (ExampleTreeNode* child_node : node->Childs)
        ExampleTree_DestroyNode(child_node);
    IM_DELETE(node);
}

// Create example tree data
// (this allocates _many_ more times than most other code in either Dear ImGui or others demo)
static ExampleTreeNode* ExampleTree_CreateDemoTree()
{
    static const char* root_names[] = { "Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pear", "Pineapple", "Strawberry", "Watermelon" };
    const size_t NAME_MAX_LEN = sizeof(ExampleTreeNode::Name);
    char name_buf[NAME_MAX_LEN];
    int uid = 0;
    ExampleTreeNode* node_L0 = ExampleTree_CreateNode("<ROOT>", ++uid, NULL);
    const int root_items_multiplier = 2;
    for (int idx_L0 = 0; idx_L0 < IM_ARRAYSIZE(root_names) * root_items_multiplier; idx_L0++)
    {
        snprintf(name_buf, IM_ARRAYSIZE(name_buf), "%s %d", root_names[idx_L0 / root_items_multiplier], idx_L0 % root_items_multiplier);
        ExampleTreeNode* node_L1 = ExampleTree_CreateNode(name_buf, ++uid, node_L0);
        const int number_of_childs = (int)strlen(node_L1->Name);
        for (int idx_L1 = 0; idx_L1 < number_of_childs; idx_L1++)
        {
            snprintf(name_buf, IM_ARRAYSIZE(name_buf), "Child %d", idx_L1);
            ExampleTreeNode* node_L2 = ExampleTree_CreateNode(name_buf, ++uid, node_L1);
            node_L2->HasData = true;
            if (idx_L1 == 0)
            {
                snprintf(name_buf, IM_ARRAYSIZE(name_buf), "Sub-child %d", 0);
                ExampleTreeNode* node_L3 = ExampleTree_CreateNode(name_buf, ++uid, node_L2);
                node_L3->HasData = true;
            }
        }
    }
    return node_L0;
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsBasic()
//-----------------------------------------------------------------------------

static void DemoWindowWidgetsBasic()
{
    IMGUI_DEMO_MARKER("Widgets/Basic");
    if (HvkGui::TreeNode("Basic"))
    {
        HvkGui::SeparatorText("General");

        IMGUI_DEMO_MARKER("Widgets/Basic/Button");
        static int clicked = 0;
        if (HvkGui::Button("Button"))
            clicked++;
        if (clicked & 1)
        {
            HvkGui::SameLine();
            HvkGui::Text("Thanks for clicking me!");
        }

        IMGUI_DEMO_MARKER("Widgets/Basic/Checkbox");
        static bool check = true;
        HvkGui::Checkbox("checkbox", &check);

        IMGUI_DEMO_MARKER("Widgets/Basic/RadioButton");
        static int e = 0;
        HvkGui::RadioButton("radio a", &e, 0); HvkGui::SameLine();
        HvkGui::RadioButton("radio b", &e, 1); HvkGui::SameLine();
        HvkGui::RadioButton("radio c", &e, 2);

        HvkGui::AlignTextToFramePadding();
        HvkGui::TextLinkOpenURL("Hyperlink", "https://github.com/ocornut/imgui/wiki/Error-Handling");

        // Color buttons, demonstrate using PushID() to add unique identifier in the ID stack, and changing style.
        IMGUI_DEMO_MARKER("Widgets/Basic/Buttons (Colored)");
        for (int i = 0; i < 7; i++)
        {
            if (i > 0)
                HvkGui::SameLine();
            HvkGui::PushID(i);
            HvkGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
            HvkGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
            HvkGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));
            HvkGui::Button("Click");
            HvkGui::PopStyleColor(3);
            HvkGui::PopID();
        }

        // Use AlignTextToFramePadding() to align text baseline to the baseline of framed widgets elements
        // (otherwise a Text+SameLine+Button sequence will have the text a little too high by default!)
        // See 'Demo->Layout->Text Baseline Alignment' for details.
        HvkGui::AlignTextToFramePadding();
        HvkGui::Text("Hold to repeat:");
        HvkGui::SameLine();

        // Arrow buttons with Repeater
        IMGUI_DEMO_MARKER("Widgets/Basic/Buttons (Repeating)");
        static int counter = 0;
        float spacing = HvkGui::GetStyle().ItemInnerSpacing.x;
        HvkGui::PushItemFlag(ImGuiItemFlags_ButtonRepeat, true);
        if (HvkGui::ArrowButton("##left", ImGuiDir_Left)) { counter--; }
        HvkGui::SameLine(0.0f, spacing);
        if (HvkGui::ArrowButton("##right", ImGuiDir_Right)) { counter++; }
        HvkGui::PopItemFlag();
        HvkGui::SameLine();
        HvkGui::Text("%d", counter);

        HvkGui::Button("Tooltip");
        HvkGui::SetItemTooltip("I am a tooltip");

        HvkGui::LabelText("label", "Value");

        HvkGui::SeparatorText("Inputs");

        {
            // If you want to use InputText() with std::string or any custom dynamic string type:
            // - For std::string: use the wrapper in misc/cpp/imgui_stdlib.h/.cpp
            // - Otherwise, see the 'Dear ImGui Demo->Widgets->Text Input->Resize Callback' for using ImGuiInputTextFlags_CallbackResize.
            IMGUI_DEMO_MARKER("Widgets/Basic/InputText");
            static char str0[128] = "Hello, world!";
            HvkGui::InputText("input text", str0, IM_ARRAYSIZE(str0));
            HvkGui::SameLine(); HelpMarker(
                "USER:\n"
                "Hold Shift or use mouse to select text.\n"
                "Ctrl+Left/Right to word jump.\n"
                "Ctrl+A or Double-Click to select all.\n"
                "Ctrl+X,Ctrl+C,Ctrl+V for clipboard.\n"
                "Ctrl+Z to undo, Ctrl+Y/Ctrl+Shift+Z to redo.\n"
                "Escape to revert.\n\n"
                "PROGRAMMER:\n"
                "You can use the ImGuiInputTextFlags_CallbackResize facility if you need to wire InputText() "
                "to a dynamic string type. See misc/cpp/imgui_stdlib.h for an example (this is not demonstrated "
                "in imgui_demo.cpp).");

            static char str1[128] = "";
            HvkGui::InputTextWithHint("input text (w/ hint)", "enter text here", str1, IM_ARRAYSIZE(str1));

            IMGUI_DEMO_MARKER("Widgets/Basic/InputInt, InputFloat");
            static int i0 = 123;
            HvkGui::InputInt("input int", &i0);

            static float f0 = 0.001f;
            HvkGui::InputFloat("input float", &f0, 0.01f, 1.0f, "%.3f");

            static double d0 = 999999.00000001;
            HvkGui::InputDouble("input double", &d0, 0.01f, 1.0f, "%.8f");

            static float f1 = 1.e10f;
            HvkGui::InputFloat("input scientific", &f1, 0.0f, 0.0f, "%e");
            HvkGui::SameLine(); HelpMarker(
                "You can input value using the scientific notation,\n"
                "  e.g. \"1e+8\" becomes \"100000000\".");

            static float vec4a[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
            HvkGui::InputFloat3("input float3", vec4a);
        }

        HvkGui::SeparatorText("Drags");

        {
            IMGUI_DEMO_MARKER("Widgets/Basic/DragInt, DragFloat");
            static int i1 = 50, i2 = 42, i3 = 128;
            HvkGui::DragInt("drag int", &i1, 1);
            HvkGui::SameLine(); HelpMarker(
                "Click and drag to edit value.\n"
                "Hold Shift/Alt for faster/slower edit.\n"
                "Double-Click or Ctrl+Click to input value.");
            HvkGui::DragInt("drag int 0..100", &i2, 1, 0, 100, "%d%%", ImGuiSliderFlags_AlwaysClamp);
            HvkGui::DragInt("drag int wrap 100..200", &i3, 1, 100, 200, "%d", ImGuiSliderFlags_WrapAround);

            static float f1 = 1.00f, f2 = 0.0067f;
            HvkGui::DragFloat("drag float", &f1, 0.005f);
            HvkGui::DragFloat("drag small float", &f2, 0.0001f, 0.0f, 0.0f, "%.06f ns");
            //HvkGui::DragFloat("drag wrap -1..1", &f3, 0.005f, -1.0f, 1.0f, NULL, ImGuiSliderFlags_WrapAround);
        }

        HvkGui::SeparatorText("Sliders");

        {
            IMGUI_DEMO_MARKER("Widgets/Basic/SliderInt, SliderFloat");
            static int i1 = 0;
            HvkGui::SliderInt("slider int", &i1, -1, 3);
            HvkGui::SameLine(); HelpMarker("Ctrl+Click to input value.");

            static float f1 = 0.123f, f2 = 0.0f;
            HvkGui::SliderFloat("slider float", &f1, 0.0f, 1.0f, "ratio = %.3f");
            HvkGui::SliderFloat("slider float (log)", &f2, -10.0f, 10.0f, "%.4f", ImGuiSliderFlags_Logarithmic);

            IMGUI_DEMO_MARKER("Widgets/Basic/SliderAngle");
            static float angle = 0.0f;
            HvkGui::SliderAngle("slider angle", &angle);

            // Using the format string to display a name instead of an integer.
            // Here we completely omit '%d' from the format string, so it'll only display a name.
            // This technique can also be used with DragInt().
            IMGUI_DEMO_MARKER("Widgets/Basic/Slider (enum)");
            enum Element { Element_Fire, Element_Earth, Element_Air, Element_Water, Element_COUNT };
            static int elem = Element_Fire;
            const char* elems_names[Element_COUNT] = { "Fire", "Earth", "Air", "Water" };
            const char* elem_name = (elem >= 0 && elem < Element_COUNT) ? elems_names[elem] : "Unknown";
            HvkGui::SliderInt("slider enum", &elem, 0, Element_COUNT - 1, elem_name); // Use ImGuiSliderFlags_NoInput flag to disable Ctrl+Click here.
            HvkGui::SameLine(); HelpMarker("Using the format string parameter to display a name instead of the underlying integer.");
        }

        HvkGui::SeparatorText("Selectors/Pickers");

        {
            IMGUI_DEMO_MARKER("Widgets/Basic/ColorEdit3, ColorEdit4");
            static float col1[3] = { 1.0f, 0.0f, 0.2f };
            static float col2[4] = { 0.4f, 0.7f, 0.0f, 0.5f };
            HvkGui::ColorEdit3("color 1", col1);
            HvkGui::SameLine(); HelpMarker(
                "Click on the color square to open a color picker.\n"
                "Click and hold to use drag and drop.\n"
                "Right-Click on the color square to show options.\n"
                "Ctrl+Click on individual component to input value.\n");

            HvkGui::ColorEdit4("color 2", col2);
        }

        {
            // Using the _simplified_ one-liner Combo() api here
            // See "Combo" section for examples of how to use the more flexible BeginCombo()/EndCombo() api.
            IMGUI_DEMO_MARKER("Widgets/Basic/Combo");
            const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIIIIII", "JJJJ", "KKKKKKK" };
            static int item_current = 0;
            HvkGui::Combo("combo", &item_current, items, IM_ARRAYSIZE(items));
            HvkGui::SameLine(); HelpMarker(
                "Using the simplified one-liner Combo API here.\n"
                "Refer to the \"Combo\" section below for an explanation of how to use the more flexible and general BeginCombo/EndCombo API.");
        }

        {
            // Using the _simplified_ one-liner ListBox() api here
            // See "List boxes" section for examples of how to use the more flexible BeginListBox()/EndListBox() api.
            IMGUI_DEMO_MARKER("Widgets/Basic/ListBox");
            const char* items[] = { "Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon" };
            static int item_current = 1;
            HvkGui::ListBox("listbox", &item_current, items, IM_ARRAYSIZE(items), 4);
            HvkGui::SameLine(); HelpMarker(
                "Using the simplified one-liner ListBox API here.\n"
                "Refer to the \"List boxes\" section below for an explanation of how to use the more flexible and general BeginListBox/EndListBox API.");
        }

        // Testing ImGuiOnceUponAFrame helper.
        //static ImGuiOnceUponAFrame once;
        //for (int i = 0; i < 5; i++)
        //    if (once)
        //        HvkGui::Text("This will be displayed only once.");

        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsBullets()
//-----------------------------------------------------------------------------

static void DemoWindowWidgetsBullets()
{
    IMGUI_DEMO_MARKER("Widgets/Bullets");
    if (HvkGui::TreeNode("Bullets"))
    {
        HvkGui::BulletText("Bullet point 1");
        HvkGui::BulletText("Bullet point 2\nOn multiple lines");
        if (HvkGui::TreeNode("Tree node"))
        {
            HvkGui::BulletText("Another bullet point");
            HvkGui::TreePop();
        }
        HvkGui::Bullet(); HvkGui::Text("Bullet point 3 (two calls)");
        HvkGui::Bullet(); HvkGui::SmallButton("Button");
        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsCollapsingHeaders()
//-----------------------------------------------------------------------------

static void DemoWindowWidgetsCollapsingHeaders()
{
    IMGUI_DEMO_MARKER("Widgets/Collapsing Headers");
    if (HvkGui::TreeNode("Collapsing Headers"))
    {
        static bool closable_group = true;
        HvkGui::Checkbox("Show 2nd header", &closable_group);
        if (HvkGui::CollapsingHeader("Header", ImGuiTreeNodeFlags_None))
        {
            HvkGui::Text("IsItemHovered: %d", HvkGui::IsItemHovered());
            for (int i = 0; i < 5; i++)
                HvkGui::Text("Some content %d", i);
        }
        if (HvkGui::CollapsingHeader("Header with a close button", &closable_group))
        {
            HvkGui::Text("IsItemHovered: %d", HvkGui::IsItemHovered());
            for (int i = 0; i < 5; i++)
                HvkGui::Text("More content %d", i);
        }
        /*
        if (HvkGui::CollapsingHeader("Header with a bullet", ImGuiTreeNodeFlags_Bullet))
            HvkGui::Text("IsItemHovered: %d", HvkGui::IsItemHovered());
        */
        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsColorAndPickers()
//-----------------------------------------------------------------------------

static void DemoWindowWidgetsColorAndPickers()
{
    IMGUI_DEMO_MARKER("Widgets/Color");
    if (HvkGui::TreeNode("Color/Picker Widgets"))
    {
        static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
        static ImGuiColorEditFlags base_flags = ImGuiColorEditFlags_None;

        HvkGui::SeparatorText("Options");
        HvkGui::CheckboxFlags("ImGuiColorEditFlags_NoAlpha", &base_flags, ImGuiColorEditFlags_NoAlpha);
        HvkGui::CheckboxFlags("ImGuiColorEditFlags_AlphaOpaque", &base_flags, ImGuiColorEditFlags_AlphaOpaque);
        HvkGui::CheckboxFlags("ImGuiColorEditFlags_AlphaNoBg", &base_flags, ImGuiColorEditFlags_AlphaNoBg);
        HvkGui::CheckboxFlags("ImGuiColorEditFlags_AlphaPreviewHalf", &base_flags, ImGuiColorEditFlags_AlphaPreviewHalf);
        HvkGui::CheckboxFlags("ImGuiColorEditFlags_NoOptions", &base_flags, ImGuiColorEditFlags_NoOptions); HvkGui::SameLine(); HelpMarker("Right-click on the individual color widget to show options.");
        HvkGui::CheckboxFlags("ImGuiColorEditFlags_NoDragDrop", &base_flags, ImGuiColorEditFlags_NoDragDrop);
        HvkGui::CheckboxFlags("ImGuiColorEditFlags_NoColorMarkers", &base_flags, ImGuiColorEditFlags_NoColorMarkers);
        HvkGui::CheckboxFlags("ImGuiColorEditFlags_HDR", &base_flags, ImGuiColorEditFlags_HDR); HvkGui::SameLine(); HelpMarker("Currently all this does is to lift the 0..1 limits on dragging widgets.");

        IMGUI_DEMO_MARKER("Widgets/Color/ColorEdit");
        HvkGui::SeparatorText("Inline color editor");
        HvkGui::Text("Color widget:");
        HvkGui::SameLine(); HelpMarker(
            "Click on the color square to open a color picker.\n"
            "Ctrl+Click on individual component to input value.\n");
        HvkGui::ColorEdit3("MyColor##1", (float*)&color, base_flags);

        IMGUI_DEMO_MARKER("Widgets/Color/ColorEdit (HSV, with Alpha)");
        HvkGui::Text("Color widget HSV with Alpha:");
        HvkGui::ColorEdit4("MyColor##2", (float*)&color, ImGuiColorEditFlags_DisplayHSV | base_flags);

        IMGUI_DEMO_MARKER("Widgets/Color/ColorEdit (float display)");
        HvkGui::Text("Color widget with Float Display:");
        HvkGui::ColorEdit4("MyColor##2f", (float*)&color, ImGuiColorEditFlags_Float | base_flags);

        IMGUI_DEMO_MARKER("Widgets/Color/ColorButton (with Picker)");
        HvkGui::Text("Color button with Picker:");
        HvkGui::SameLine(); HelpMarker(
            "With the ImGuiColorEditFlags_NoInputs flag you can hide all the slider/text inputs.\n"
            "With the ImGuiColorEditFlags_NoLabel flag you can pass a non-empty label which will only "
            "be used for the tooltip and picker popup.");
        HvkGui::ColorEdit4("MyColor##3", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | base_flags);

        IMGUI_DEMO_MARKER("Widgets/Color/ColorButton (with custom Picker popup)");
        HvkGui::Text("Color button with Custom Picker Popup:");

        // Generate a default palette. The palette will persist and can be edited.
        static bool saved_palette_init = true;
        static ImVec4 saved_palette[32] = {};
        if (saved_palette_init)
        {
            for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
            {
                HvkGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f,
                    saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
                saved_palette[n].w = 1.0f; // Alpha
            }
            saved_palette_init = false;
        }

        static ImVec4 backup_color;
        bool open_popup = HvkGui::ColorButton("MyColor##3b", color, base_flags);
        HvkGui::SameLine(0, HvkGui::GetStyle().ItemInnerSpacing.x);
        open_popup |= HvkGui::Button("Palette");
        if (open_popup)
        {
            HvkGui::OpenPopup("mypicker");
            backup_color = color;
        }
        if (HvkGui::BeginPopup("mypicker"))
        {
            HvkGui::Text("MY CUSTOM COLOR PICKER WITH AN AMAZING PALETTE!");
            HvkGui::Separator();
            HvkGui::ColorPicker4("##picker", (float*)&color, base_flags | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
            HvkGui::SameLine();

            HvkGui::BeginGroup(); // Lock X position
            HvkGui::Text("Current");
            HvkGui::ColorButton("##current", color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40));
            HvkGui::Text("Previous");
            if (HvkGui::ColorButton("##previous", backup_color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40)))
                color = backup_color;
            HvkGui::Separator();
            HvkGui::Text("Palette");
            for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
            {
                HvkGui::PushID(n);
                if ((n % 8) != 0)
                    HvkGui::SameLine(0.0f, HvkGui::GetStyle().ItemSpacing.y);

                ImGuiColorEditFlags palette_button_flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip;
                if (HvkGui::ColorButton("##palette", saved_palette[n], palette_button_flags, ImVec2(20, 20)))
                    color = ImVec4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, color.w); // Preserve alpha!

                // Allow user to drop colors into each palette entry. Note that ColorButton() is already a
                // drag source by default, unless specifying the ImGuiColorEditFlags_NoDragDrop flag.
                if (HvkGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = HvkGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
                        memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 3);
                    if (const ImGuiPayload* payload = HvkGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
                        memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 4);
                    HvkGui::EndDragDropTarget();
                }

                HvkGui::PopID();
            }
            HvkGui::EndGroup();
            HvkGui::EndPopup();
        }

        IMGUI_DEMO_MARKER("Widgets/Color/ColorButton (simple)");
        HvkGui::Text("Color button only:");
        static bool no_border = false;
        HvkGui::Checkbox("ImGuiColorEditFlags_NoBorder", &no_border);
        HvkGui::ColorButton("MyColor##3c", *(ImVec4*)&color, base_flags | (no_border ? ImGuiColorEditFlags_NoBorder : 0), ImVec2(80, 80));

        IMGUI_DEMO_MARKER("Widgets/Color/ColorPicker");
        HvkGui::SeparatorText("Color picker");

        static bool ref_color = false;
        static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);
        static int picker_mode = 0;
        static int display_mode = 0;
        static ImGuiColorEditFlags color_picker_flags = ImGuiColorEditFlags_AlphaBar;

        HvkGui::PushID("Color picker");
        HvkGui::CheckboxFlags("ImGuiColorEditFlags_NoAlpha", &color_picker_flags, ImGuiColorEditFlags_NoAlpha);
        HvkGui::CheckboxFlags("ImGuiColorEditFlags_AlphaBar", &color_picker_flags, ImGuiColorEditFlags_AlphaBar);
        HvkGui::CheckboxFlags("ImGuiColorEditFlags_NoSidePreview", &color_picker_flags, ImGuiColorEditFlags_NoSidePreview);
        if (color_picker_flags & ImGuiColorEditFlags_NoSidePreview)
        {
            HvkGui::SameLine();
            HvkGui::Checkbox("With Ref Color", &ref_color);
            if (ref_color)
            {
                HvkGui::SameLine();
                HvkGui::ColorEdit4("##RefColor", &ref_color_v.x, ImGuiColorEditFlags_NoInputs | base_flags);
            }
        }

        HvkGui::Combo("Picker Mode", &picker_mode, "Auto/Current\0ImGuiColorEditFlags_PickerHueBar\0ImGuiColorEditFlags_PickerHueWheel\0");
        HvkGui::SameLine(); HelpMarker("When not specified explicitly, user can right-click the picker to change mode.");

        HvkGui::Combo("Display Mode", &display_mode, "Auto/Current\0ImGuiColorEditFlags_NoInputs\0ImGuiColorEditFlags_DisplayRGB\0ImGuiColorEditFlags_DisplayHSV\0ImGuiColorEditFlags_DisplayHex\0");
        HvkGui::SameLine(); HelpMarker(
            "ColorEdit defaults to displaying RGB inputs if you don't specify a display mode, "
            "but the user can change it with a right-click on those inputs.\n\nColorPicker defaults to displaying RGB+HSV+Hex "
            "if you don't specify a display mode.\n\nYou can change the defaults using SetColorEditOptions().");

        ImGuiColorEditFlags flags = base_flags | color_picker_flags;
        if (picker_mode == 1)  flags |= ImGuiColorEditFlags_PickerHueBar;
        if (picker_mode == 2)  flags |= ImGuiColorEditFlags_PickerHueWheel;
        if (display_mode == 1) flags |= ImGuiColorEditFlags_NoInputs;       // Disable all RGB/HSV/Hex displays
        if (display_mode == 2) flags |= ImGuiColorEditFlags_DisplayRGB;     // Override display mode
        if (display_mode == 3) flags |= ImGuiColorEditFlags_DisplayHSV;
        if (display_mode == 4) flags |= ImGuiColorEditFlags_DisplayHex;
        HvkGui::ColorPicker4("MyColor##4", (float*)&color, flags, ref_color ? &ref_color_v.x : NULL);

        HvkGui::Text("Set defaults in code:");
        HvkGui::SameLine(); HelpMarker(
            "SetColorEditOptions() is designed to allow you to set boot-time default.\n"
            "We don't have Push/Pop functions because you can force options on a per-widget basis if needed, "
            "and the user can change non-forced ones with the options menu.\nWe don't have a getter to avoid "
            "encouraging you to persistently save values that aren't forward-compatible.");
        if (HvkGui::Button("Default: Uint8 + HSV + Hue Bar"))
            HvkGui::SetColorEditOptions(ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_PickerHueBar);
        if (HvkGui::Button("Default: Float + HDR + Hue Wheel"))
            HvkGui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);

        // Always display a small version of both types of pickers
        // (that's in order to make it more visible in the demo to people who are skimming quickly through it)
        HvkGui::Text("Both types:");
        float w = (HvkGui::GetContentRegionAvail().x - HvkGui::GetStyle().ItemSpacing.y) * 0.40f;
        HvkGui::SetNextItemWidth(w);
        HvkGui::ColorPicker3("##MyColor##5", (float*)&color, ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
        HvkGui::SameLine();
        HvkGui::SetNextItemWidth(w);
        HvkGui::ColorPicker3("##MyColor##6", (float*)&color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
        HvkGui::PopID();

        // HSV encoded support (to avoid RGB<>HSV round trips and singularities when S==0 or V==0)
        static ImVec4 color_hsv(0.23f, 1.0f, 1.0f, 1.0f); // Stored as HSV!
        HvkGui::Spacing();
        HvkGui::Text("HSV encoded colors");
        HvkGui::SameLine(); HelpMarker(
            "By default, colors are given to ColorEdit and ColorPicker in RGB, but ImGuiColorEditFlags_InputHSV "
            "allows you to store colors as HSV and pass them to ColorEdit and ColorPicker as HSV. This comes with the "
            "added benefit that you can manipulate hue values with the picker even when saturation or value are zero.");
        HvkGui::Text("Color widget with InputHSV:");
        HvkGui::ColorEdit4("HSV shown as RGB##1", (float*)&color_hsv, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputHSV | ImGuiColorEditFlags_Float);
        HvkGui::ColorEdit4("HSV shown as HSV##1", (float*)&color_hsv, ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_InputHSV | ImGuiColorEditFlags_Float);
        HvkGui::DragFloat4("Raw HSV values", (float*)&color_hsv, 0.01f, 0.0f, 1.0f);

        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsComboBoxes()
//-----------------------------------------------------------------------------

static void DemoWindowWidgetsComboBoxes()
{
    IMGUI_DEMO_MARKER("Widgets/Combo");
    if (HvkGui::TreeNode("Combo"))
    {
        // Combo Boxes are also called "Dropdown" in other systems
        // Expose flags as checkbox for the demo
        static ImGuiComboFlags flags = 0;
        HvkGui::CheckboxFlags("ImGuiComboFlags_PopupAlignLeft", &flags, ImGuiComboFlags_PopupAlignLeft);
        HvkGui::SameLine(); HelpMarker("Only makes a difference if the popup is larger than the combo");
        if (HvkGui::CheckboxFlags("ImGuiComboFlags_NoArrowButton", &flags, ImGuiComboFlags_NoArrowButton))
            flags &= ~ImGuiComboFlags_NoPreview;     // Clear incompatible flags
        if (HvkGui::CheckboxFlags("ImGuiComboFlags_NoPreview", &flags, ImGuiComboFlags_NoPreview))
            flags &= ~(ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_WidthFitPreview); // Clear incompatible flags
        if (HvkGui::CheckboxFlags("ImGuiComboFlags_WidthFitPreview", &flags, ImGuiComboFlags_WidthFitPreview))
            flags &= ~ImGuiComboFlags_NoPreview;

        // Override default popup height
        if (HvkGui::CheckboxFlags("ImGuiComboFlags_HeightSmall", &flags, ImGuiComboFlags_HeightSmall))
            flags &= ~(ImGuiComboFlags_HeightMask_ & ~ImGuiComboFlags_HeightSmall);
        if (HvkGui::CheckboxFlags("ImGuiComboFlags_HeightRegular", &flags, ImGuiComboFlags_HeightRegular))
            flags &= ~(ImGuiComboFlags_HeightMask_ & ~ImGuiComboFlags_HeightRegular);
        if (HvkGui::CheckboxFlags("ImGuiComboFlags_HeightLargest", &flags, ImGuiComboFlags_HeightLargest))
            flags &= ~(ImGuiComboFlags_HeightMask_ & ~ImGuiComboFlags_HeightLargest);

        // Using the generic BeginCombo() API, you have full control over how to display the combo contents.
        // (your selection data could be an index, a pointer to the object, an id for the object, a flag intrusively
        // stored in the object itself, etc.)
        const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
        static int item_selected_idx = 0; // Here we store our selection data as an index.

        // Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
        const char* combo_preview_value = items[item_selected_idx];
        if (HvkGui::BeginCombo("combo 1", combo_preview_value, flags))
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                const bool is_selected = (item_selected_idx == n);
                if (HvkGui::Selectable(items[n], is_selected))
                    item_selected_idx = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    HvkGui::SetItemDefaultFocus();
            }
            HvkGui::EndCombo();
        }

        // Show case embedding a filter using a simple trick: displaying the filter inside combo contents.
        // See https://github.com/ocornut/imgui/issues/718 for advanced/esoteric alternatives.
        if (HvkGui::BeginCombo("combo 2 (w/ filter)", combo_preview_value, flags))
        {
            static ImGuiTextFilter filter;
            if (HvkGui::IsWindowAppearing())
            {
                HvkGui::SetKeyboardFocusHere();
                filter.Clear();
            }
            HvkGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_F);
            filter.Draw("##Filter", -FLT_MIN);

            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                const bool is_selected = (item_selected_idx == n);
                if (filter.PassFilter(items[n]))
                    if (HvkGui::Selectable(items[n], is_selected))
                        item_selected_idx = n;
            }
            HvkGui::EndCombo();
        }

        HvkGui::Spacing();
        HvkGui::SeparatorText("One-liner variants");
        HelpMarker("The Combo() function is not greatly useful apart from cases were you want to embed all options in a single strings.\nFlags above don't apply to this section.");

        // Simplified one-liner Combo() API, using values packed in a single constant string
        // This is a convenience for when the selection set is small and known at compile-time.
        static int item_current_2 = 0;
        HvkGui::Combo("combo 3 (one-liner)", &item_current_2, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");

        // Simplified one-liner Combo() using an array of const char*
        // This is not very useful (may obsolete): prefer using BeginCombo()/EndCombo() for full control.
        static int item_current_3 = -1; // If the selection isn't within 0..count, Combo won't display a preview
        HvkGui::Combo("combo 4 (array)", &item_current_3, items, IM_ARRAYSIZE(items));

        // Simplified one-liner Combo() using an accessor function
        static int item_current_4 = 0;
        HvkGui::Combo("combo 5 (function)", &item_current_4, [](void* data, int n) { return ((const char**)data)[n]; }, items, IM_ARRAYSIZE(items));

        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsDataTypes()
//-----------------------------------------------------------------------------

static void DemoWindowWidgetsDataTypes()
{
    IMGUI_DEMO_MARKER("Widgets/Data Types");
    if (HvkGui::TreeNode("Data Types"))
    {
        // DragScalar/InputScalar/SliderScalar functions allow various data types
        // - signed/unsigned
        // - 8/16/32/64-bits
        // - integer/float/double
        // To avoid polluting the public API with all possible combinations, we use the ImGuiDataType enum
        // to pass the type, and passing all arguments by pointer.
        // This is the reason the test code below creates local variables to hold "zero" "one" etc. for each type.
        // In practice, if you frequently use a given type that is not covered by the normal API entry points,
        // you can wrap it yourself inside a 1 line function which can take typed argument as value instead of void*,
        // and then pass their address to the generic function. For example:
        //   bool MySliderU64(const char *label, u64* value, u64 min = 0, u64 max = 0, const char* format = "%lld")
        //   {
        //      return SliderScalar(label, ImGuiDataType_U64, value, &min, &max, format);
        //   }

        // Setup limits (as helper variables so we can take their address, as explained above)
        // Note: SliderScalar() functions have a maximum usable range of half the natural type maximum, hence the /2.
        #ifndef LLONG_MIN
        ImS64 LLONG_MIN = -9223372036854775807LL - 1;
        ImS64 LLONG_MAX = 9223372036854775807LL;
        ImU64 ULLONG_MAX = (2ULL * 9223372036854775807LL + 1);
        #endif
        const char    s8_zero  = 0,   s8_one  = 1,   s8_fifty  = 50, s8_min  = -128,        s8_max = 127;
        const ImU8    u8_zero  = 0,   u8_one  = 1,   u8_fifty  = 50, u8_min  = 0,           u8_max = 255;
        const short   s16_zero = 0,   s16_one = 1,   s16_fifty = 50, s16_min = -32768,      s16_max = 32767;
        const ImU16   u16_zero = 0,   u16_one = 1,   u16_fifty = 50, u16_min = 0,           u16_max = 65535;
        const ImS32   s32_zero = 0,   s32_one = 1,   s32_fifty = 50, s32_min = INT_MIN/2,   s32_max = INT_MAX/2,    s32_hi_a = INT_MAX/2 - 100,    s32_hi_b = INT_MAX/2;
        const ImU32   u32_zero = 0,   u32_one = 1,   u32_fifty = 50, u32_min = 0,           u32_max = UINT_MAX/2,   u32_hi_a = UINT_MAX/2 - 100,   u32_hi_b = UINT_MAX/2;
        const ImS64   s64_zero = 0,   s64_one = 1,   s64_fifty = 50, s64_min = LLONG_MIN/2, s64_max = LLONG_MAX/2,  s64_hi_a = LLONG_MAX/2 - 100,  s64_hi_b = LLONG_MAX/2;
        const ImU64   u64_zero = 0,   u64_one = 1,   u64_fifty = 50, u64_min = 0,           u64_max = ULLONG_MAX/2, u64_hi_a = ULLONG_MAX/2 - 100, u64_hi_b = ULLONG_MAX/2;
        const float   f32_zero = 0.f, f32_one = 1.f, f32_lo_a = -10000000000.0f, f32_hi_a = +10000000000.0f;
        const double  f64_zero = 0.,  f64_one = 1.,  f64_lo_a = -1000000000000000.0, f64_hi_a = +1000000000000000.0;

        // State
        static char   s8_v  = 127;
        static ImU8   u8_v  = 255;
        static short  s16_v = 32767;
        static ImU16  u16_v = 65535;
        static ImS32  s32_v = -1;
        static ImU32  u32_v = (ImU32)-1;
        static ImS64  s64_v = -1;
        static ImU64  u64_v = (ImU64)-1;
        static float  f32_v = 0.123f;
        static double f64_v = 90000.01234567890123456789;

        const float drag_speed = 0.2f;
        static bool drag_clamp = false;
        IMGUI_DEMO_MARKER("Widgets/Data Types/Drags");
        HvkGui::SeparatorText("Drags");
        HvkGui::Checkbox("Clamp integers to 0..50", &drag_clamp);
        HvkGui::SameLine(); HelpMarker(
            "As with every widget in dear imgui, we never modify values unless there is a user interaction.\n"
            "You can override the clamping limits by using Ctrl+Click to input a value.");
        HvkGui::DragScalar("drag s8",        ImGuiDataType_S8,     &s8_v,  drag_speed, drag_clamp ? &s8_zero  : NULL, drag_clamp ? &s8_fifty  : NULL);
        HvkGui::DragScalar("drag u8",        ImGuiDataType_U8,     &u8_v,  drag_speed, drag_clamp ? &u8_zero  : NULL, drag_clamp ? &u8_fifty  : NULL, "%u ms");
        HvkGui::DragScalar("drag s16",       ImGuiDataType_S16,    &s16_v, drag_speed, drag_clamp ? &s16_zero : NULL, drag_clamp ? &s16_fifty : NULL);
        HvkGui::DragScalar("drag u16",       ImGuiDataType_U16,    &u16_v, drag_speed, drag_clamp ? &u16_zero : NULL, drag_clamp ? &u16_fifty : NULL, "%u ms");
        HvkGui::DragScalar("drag s32",       ImGuiDataType_S32,    &s32_v, drag_speed, drag_clamp ? &s32_zero : NULL, drag_clamp ? &s32_fifty : NULL);
        HvkGui::DragScalar("drag s32 hex",   ImGuiDataType_S32,    &s32_v, drag_speed, drag_clamp ? &s32_zero : NULL, drag_clamp ? &s32_fifty : NULL, "0x%08X");
        HvkGui::DragScalar("drag u32",       ImGuiDataType_U32,    &u32_v, drag_speed, drag_clamp ? &u32_zero : NULL, drag_clamp ? &u32_fifty : NULL, "%u ms");
        HvkGui::DragScalar("drag s64",       ImGuiDataType_S64,    &s64_v, drag_speed, drag_clamp ? &s64_zero : NULL, drag_clamp ? &s64_fifty : NULL);
        HvkGui::DragScalar("drag u64",       ImGuiDataType_U64,    &u64_v, drag_speed, drag_clamp ? &u64_zero : NULL, drag_clamp ? &u64_fifty : NULL);
        HvkGui::DragScalar("drag float",     ImGuiDataType_Float,  &f32_v, 0.005f,  &f32_zero, &f32_one, "%f");
        HvkGui::DragScalar("drag float log", ImGuiDataType_Float,  &f32_v, 0.005f,  &f32_zero, &f32_one, "%f", ImGuiSliderFlags_Logarithmic);
        HvkGui::DragScalar("drag double",    ImGuiDataType_Double, &f64_v, 0.0005f, &f64_zero, NULL,     "%.10f grams");
        HvkGui::DragScalar("drag double log",ImGuiDataType_Double, &f64_v, 0.0005f, &f64_zero, &f64_one, "0 < %.10f < 1", ImGuiSliderFlags_Logarithmic);

        IMGUI_DEMO_MARKER("Widgets/Data Types/Sliders");
        HvkGui::SeparatorText("Sliders");
        HvkGui::SliderScalar("slider s8 full",       ImGuiDataType_S8,     &s8_v,  &s8_min,   &s8_max,   "%d");
        HvkGui::SliderScalar("slider u8 full",       ImGuiDataType_U8,     &u8_v,  &u8_min,   &u8_max,   "%u");
        HvkGui::SliderScalar("slider s16 full",      ImGuiDataType_S16,    &s16_v, &s16_min,  &s16_max,  "%d");
        HvkGui::SliderScalar("slider u16 full",      ImGuiDataType_U16,    &u16_v, &u16_min,  &u16_max,  "%u");
        HvkGui::SliderScalar("slider s32 low",       ImGuiDataType_S32,    &s32_v, &s32_zero, &s32_fifty,"%d");
        HvkGui::SliderScalar("slider s32 high",      ImGuiDataType_S32,    &s32_v, &s32_hi_a, &s32_hi_b, "%d");
        HvkGui::SliderScalar("slider s32 full",      ImGuiDataType_S32,    &s32_v, &s32_min,  &s32_max,  "%d");
        HvkGui::SliderScalar("slider s32 hex",       ImGuiDataType_S32,    &s32_v, &s32_zero, &s32_fifty, "0x%04X");
        HvkGui::SliderScalar("slider u32 low",       ImGuiDataType_U32,    &u32_v, &u32_zero, &u32_fifty,"%u");
        HvkGui::SliderScalar("slider u32 high",      ImGuiDataType_U32,    &u32_v, &u32_hi_a, &u32_hi_b, "%u");
        HvkGui::SliderScalar("slider u32 full",      ImGuiDataType_U32,    &u32_v, &u32_min,  &u32_max,  "%u");
        HvkGui::SliderScalar("slider s64 low",       ImGuiDataType_S64,    &s64_v, &s64_zero, &s64_fifty,"%" PRId64);
        HvkGui::SliderScalar("slider s64 high",      ImGuiDataType_S64,    &s64_v, &s64_hi_a, &s64_hi_b, "%" PRId64);
        HvkGui::SliderScalar("slider s64 full",      ImGuiDataType_S64,    &s64_v, &s64_min,  &s64_max,  "%" PRId64);
        HvkGui::SliderScalar("slider u64 low",       ImGuiDataType_U64,    &u64_v, &u64_zero, &u64_fifty,"%" PRIu64 " ms");
        HvkGui::SliderScalar("slider u64 high",      ImGuiDataType_U64,    &u64_v, &u64_hi_a, &u64_hi_b, "%" PRIu64 " ms");
        HvkGui::SliderScalar("slider u64 full",      ImGuiDataType_U64,    &u64_v, &u64_min,  &u64_max,  "%" PRIu64 " ms");
        HvkGui::SliderScalar("slider float low",     ImGuiDataType_Float,  &f32_v, &f32_zero, &f32_one);
        HvkGui::SliderScalar("slider float low log", ImGuiDataType_Float,  &f32_v, &f32_zero, &f32_one,  "%.10f", ImGuiSliderFlags_Logarithmic);
        HvkGui::SliderScalar("slider float high",    ImGuiDataType_Float,  &f32_v, &f32_lo_a, &f32_hi_a, "%e");
        HvkGui::SliderScalar("slider double low",    ImGuiDataType_Double, &f64_v, &f64_zero, &f64_one,  "%.10f grams");
        HvkGui::SliderScalar("slider double low log",ImGuiDataType_Double, &f64_v, &f64_zero, &f64_one,  "%.10f", ImGuiSliderFlags_Logarithmic);
        HvkGui::SliderScalar("slider double high",   ImGuiDataType_Double, &f64_v, &f64_lo_a, &f64_hi_a, "%e grams");

        HvkGui::SeparatorText("Sliders (reverse)");
        HvkGui::SliderScalar("slider s8 reverse",    ImGuiDataType_S8,   &s8_v,  &s8_max,    &s8_min,   "%d");
        HvkGui::SliderScalar("slider u8 reverse",    ImGuiDataType_U8,   &u8_v,  &u8_max,    &u8_min,   "%u");
        HvkGui::SliderScalar("slider s32 reverse",   ImGuiDataType_S32,  &s32_v, &s32_fifty, &s32_zero, "%d");
        HvkGui::SliderScalar("slider u32 reverse",   ImGuiDataType_U32,  &u32_v, &u32_fifty, &u32_zero, "%u");
        HvkGui::SliderScalar("slider s64 reverse",   ImGuiDataType_S64,  &s64_v, &s64_fifty, &s64_zero, "%" PRId64);
        HvkGui::SliderScalar("slider u64 reverse",   ImGuiDataType_U64,  &u64_v, &u64_fifty, &u64_zero, "%" PRIu64 " ms");

        IMGUI_DEMO_MARKER("Widgets/Data Types/Inputs");
        static bool inputs_step = true;
        static ImGuiInputTextFlags flags = ImGuiInputTextFlags_None;
        HvkGui::SeparatorText("Inputs");
        HvkGui::Checkbox("Show step buttons", &inputs_step);
        HvkGui::CheckboxFlags("ImGuiInputTextFlags_ReadOnly", &flags, ImGuiInputTextFlags_ReadOnly);
        HvkGui::CheckboxFlags("ImGuiInputTextFlags_ParseEmptyRefVal", &flags, ImGuiInputTextFlags_ParseEmptyRefVal);
        HvkGui::CheckboxFlags("ImGuiInputTextFlags_DisplayEmptyRefVal", &flags, ImGuiInputTextFlags_DisplayEmptyRefVal);
        HvkGui::InputScalar("input s8",      ImGuiDataType_S8,     &s8_v,  inputs_step ? &s8_one  : NULL, NULL, "%d", flags);
        HvkGui::InputScalar("input u8",      ImGuiDataType_U8,     &u8_v,  inputs_step ? &u8_one  : NULL, NULL, "%u", flags);
        HvkGui::InputScalar("input s16",     ImGuiDataType_S16,    &s16_v, inputs_step ? &s16_one : NULL, NULL, "%d", flags);
        HvkGui::InputScalar("input u16",     ImGuiDataType_U16,    &u16_v, inputs_step ? &u16_one : NULL, NULL, "%u", flags);
        HvkGui::InputScalar("input s32",     ImGuiDataType_S32,    &s32_v, inputs_step ? &s32_one : NULL, NULL, "%d", flags);
        HvkGui::InputScalar("input s32 hex", ImGuiDataType_S32,    &s32_v, inputs_step ? &s32_one : NULL, NULL, "%04X", flags);
        HvkGui::InputScalar("input u32",     ImGuiDataType_U32,    &u32_v, inputs_step ? &u32_one : NULL, NULL, "%u", flags);
        HvkGui::InputScalar("input u32 hex", ImGuiDataType_U32,    &u32_v, inputs_step ? &u32_one : NULL, NULL, "%08X", flags);
        HvkGui::InputScalar("input s64",     ImGuiDataType_S64,    &s64_v, inputs_step ? &s64_one : NULL, NULL, NULL, flags);
        HvkGui::InputScalar("input u64",     ImGuiDataType_U64,    &u64_v, inputs_step ? &u64_one : NULL, NULL, NULL, flags);
        HvkGui::InputScalar("input float",   ImGuiDataType_Float,  &f32_v, inputs_step ? &f32_one : NULL, NULL, NULL, flags);
        HvkGui::InputScalar("input double",  ImGuiDataType_Double, &f64_v, inputs_step ? &f64_one : NULL, NULL, NULL, flags);

        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsDisableBlocks()
//-----------------------------------------------------------------------------

static void DemoWindowWidgetsDisableBlocks(ImGuiDemoWindowData* demo_data)
{
    IMGUI_DEMO_MARKER("Widgets/Disable Blocks");
    if (HvkGui::TreeNode("Disable Blocks"))
    {
        HvkGui::Checkbox("Disable entire section above", &demo_data->DisableSections);
        HvkGui::SameLine(); HelpMarker("Demonstrate using BeginDisabled()/EndDisabled() across other sections.");
        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsDragAndDrop()
//-----------------------------------------------------------------------------

static void DemoWindowWidgetsDragAndDrop()
{
    IMGUI_DEMO_MARKER("Widgets/Drag and drop");
    if (HvkGui::TreeNode("Drag and Drop"))
    {
        IMGUI_DEMO_MARKER("Widgets/Drag and drop/Standard widgets");
        if (HvkGui::TreeNode("Drag and drop in standard widgets"))
        {
            // ColorEdit widgets automatically act as drag source and drag target.
            // They are using standardized payload strings IMGUI_PAYLOAD_TYPE_COLOR_3F and IMGUI_PAYLOAD_TYPE_COLOR_4F
            // to allow your own widgets to use colors in their drag and drop interaction.
            // Also see 'Demo->Widgets->Color/Picker Widgets->Palette' demo.
            HelpMarker("You can drag from the color squares.");
            static float col1[3] = { 1.0f, 0.0f, 0.2f };
            static float col2[4] = { 0.4f, 0.7f, 0.0f, 0.5f };
            HvkGui::ColorEdit3("color 1", col1);
            HvkGui::ColorEdit4("color 2", col2);
            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Drag and drop/Copy-swap items");
        if (HvkGui::TreeNode("Drag and drop to copy/swap items"))
        {
            enum Mode
            {
                Mode_Copy,
                Mode_Move,
                Mode_Swap
            };
            static int mode = 0;
            if (HvkGui::RadioButton("Copy", mode == Mode_Copy)) { mode = Mode_Copy; } HvkGui::SameLine();
            if (HvkGui::RadioButton("Move", mode == Mode_Move)) { mode = Mode_Move; } HvkGui::SameLine();
            if (HvkGui::RadioButton("Swap", mode == Mode_Swap)) { mode = Mode_Swap; }
            static const char* names[9] =
            {
                "Bobby", "Beatrice", "Betty",
                "Brianna", "Barry", "Bernard",
                "Bibi", "Blaine", "Bryn"
            };
            for (int n = 0; n < IM_ARRAYSIZE(names); n++)
            {
                HvkGui::PushID(n);
                if ((n % 3) != 0)
                    HvkGui::SameLine();
                HvkGui::Button(names[n], ImVec2(60, 60));

                // Our buttons are both drag sources and drag targets here!
                if (HvkGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                {
                    // Set payload to carry the index of our item (could be anything)
                    HvkGui::SetDragDropPayload("DND_DEMO_CELL", &n, sizeof(int));

                    // Display preview (could be anything, e.g. when dragging an image we could decide to display
                    // the filename and a small preview of the image, etc.)
                    if (mode == Mode_Copy) { HvkGui::Text("Copy %s", names[n]); }
                    if (mode == Mode_Move) { HvkGui::Text("Move %s", names[n]); }
                    if (mode == Mode_Swap) { HvkGui::Text("Swap %s", names[n]); }
                    HvkGui::EndDragDropSource();
                }
                if (HvkGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = HvkGui::AcceptDragDropPayload("DND_DEMO_CELL"))
                    {
                        IM_ASSERT(payload->DataSize == sizeof(int));
                        int payload_n = *(const int*)payload->Data;
                        if (mode == Mode_Copy)
                        {
                            names[n] = names[payload_n];
                        }
                        if (mode == Mode_Move)
                        {
                            names[n] = names[payload_n];
                            names[payload_n] = "";
                        }
                        if (mode == Mode_Swap)
                        {
                            const char* tmp = names[n];
                            names[n] = names[payload_n];
                            names[payload_n] = tmp;
                        }
                    }
                    HvkGui::EndDragDropTarget();
                }
                HvkGui::PopID();
            }
            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Drag and Drop/Drag to reorder items (simple)");
        if (HvkGui::TreeNode("Drag to reorder items (simple)"))
        {
            // FIXME: there is temporary (usually single-frame) ID Conflict during reordering as a same item may be submitting twice.
            // This code was always slightly faulty but in a way which was not easily noticeable.
            // Until we fix this, enable ImGuiItemFlags_AllowDuplicateId to disable detecting the issue.
            HvkGui::PushItemFlag(ImGuiItemFlags_AllowDuplicateId, true);

            // Simple reordering
            HelpMarker(
                "We don't use the drag and drop api at all here! "
                "Instead we query when the item is held but not hovered, and order items accordingly.");
            static const char* item_names[] = { "Item One", "Item Two", "Item Three", "Item Four", "Item Five" };
            for (int n = 0; n < IM_ARRAYSIZE(item_names); n++)
            {
                const char* item = item_names[n];
                HvkGui::Selectable(item);

                if (HvkGui::IsItemActive() && !HvkGui::IsItemHovered())
                {
                    int n_next = n + (HvkGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
                    if (n_next >= 0 && n_next < IM_ARRAYSIZE(item_names))
                    {
                        item_names[n] = item_names[n_next];
                        item_names[n_next] = item;
                        HvkGui::ResetMouseDragDelta();
                    }
                }
            }

            HvkGui::PopItemFlag();
            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Drag and Drop/Tooltip at target location");
        if (HvkGui::TreeNode("Tooltip at target location"))
        {
            for (int n = 0; n < 2; n++)
            {
                // Drop targets
                HvkGui::Button(n ? "drop here##1" : "drop here##0");
                if (HvkGui::BeginDragDropTarget())
                {
                    ImGuiDragDropFlags drop_target_flags = ImGuiDragDropFlags_AcceptBeforeDelivery | ImGuiDragDropFlags_AcceptNoPreviewTooltip;
                    if (const ImGuiPayload* payload = HvkGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F, drop_target_flags))
                    {
                        IM_UNUSED(payload);
                        HvkGui::SetMouseCursor(ImGuiMouseCursor_NotAllowed);
                        HvkGui::SetTooltip("Cannot drop here!");
                    }
                    HvkGui::EndDragDropTarget();
                }

                // Drop source
                static ImVec4 col4 = { 1.0f, 0.0f, 0.2f, 1.0f };
                if (n == 0)
                    HvkGui::ColorButton("drag me", col4);

            }
            HvkGui::TreePop();
        }

        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsDragsAndSliders()
//-----------------------------------------------------------------------------

static void DemoWindowWidgetsDragsAndSliders()
{
    IMGUI_DEMO_MARKER("Widgets/Drag and Slider Flags");
    if (HvkGui::TreeNode("Drag/Slider Flags"))
    {
        // Demonstrate using advanced flags for DragXXX and SliderXXX functions. Note that the flags are the same!
        static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
        HvkGui::CheckboxFlags("ImGuiSliderFlags_AlwaysClamp", &flags, ImGuiSliderFlags_AlwaysClamp);
        HvkGui::CheckboxFlags("ImGuiSliderFlags_ClampOnInput", &flags, ImGuiSliderFlags_ClampOnInput);
        HvkGui::SameLine(); HelpMarker("Clamp value to min/max bounds when input manually with Ctrl+Click. By default Ctrl+Click allows going out of bounds.");
        HvkGui::CheckboxFlags("ImGuiSliderFlags_ClampZeroRange", &flags, ImGuiSliderFlags_ClampZeroRange);
        HvkGui::SameLine(); HelpMarker("Clamp even if min==max==0.0f. Otherwise DragXXX functions don't clamp.");
        HvkGui::CheckboxFlags("ImGuiSliderFlags_Logarithmic", &flags, ImGuiSliderFlags_Logarithmic);
        HvkGui::SameLine(); HelpMarker("Enable logarithmic editing (more precision for small values).");
        HvkGui::CheckboxFlags("ImGuiSliderFlags_NoRoundToFormat", &flags, ImGuiSliderFlags_NoRoundToFormat);
        HvkGui::SameLine(); HelpMarker("Disable rounding underlying value to match precision of the format string (e.g. %.3f values are rounded to those 3 digits).");
        HvkGui::CheckboxFlags("ImGuiSliderFlags_NoInput", &flags, ImGuiSliderFlags_NoInput);
        HvkGui::SameLine(); HelpMarker("Disable Ctrl+Click or Enter key allowing to input text directly into the widget.");
        HvkGui::CheckboxFlags("ImGuiSliderFlags_NoSpeedTweaks", &flags, ImGuiSliderFlags_NoSpeedTweaks);
        HvkGui::SameLine(); HelpMarker("Disable keyboard modifiers altering tweak speed. Useful if you want to alter tweak speed yourself based on your own logic.");
        HvkGui::CheckboxFlags("ImGuiSliderFlags_WrapAround", &flags, ImGuiSliderFlags_WrapAround);
        HvkGui::SameLine(); HelpMarker("Enable wrapping around from max to min and from min to max (only supported by DragXXX() functions)");
        HvkGui::CheckboxFlags("ImGuiSliderFlags_ColorMarkers", &flags, ImGuiSliderFlags_ColorMarkers);
        //HvkGui::CheckboxFlags("ImGuiSliderFlags_ColorMarkersG", &flags, 1 << ImGuiSliderFlags_ColorMarkersIndexShift_); // Not explicitly documented but possible.

        // Drags
        static float drag_f = 0.5f;
        static float drag_f4[4];
        static int drag_i = 50;
        HvkGui::Text("Underlying float value: %f", drag_f);
        HvkGui::DragFloat("DragFloat (0 -> 1)", &drag_f, 0.005f, 0.0f, 1.0f, "%.3f", flags);
        HvkGui::DragFloat("DragFloat (0 -> +inf)", &drag_f, 0.005f, 0.0f, FLT_MAX, "%.3f", flags);
        HvkGui::DragFloat("DragFloat (-inf -> 1)", &drag_f, 0.005f, -FLT_MAX, 1.0f, "%.3f", flags);
        HvkGui::DragFloat("DragFloat (-inf -> +inf)", &drag_f, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
        //HvkGui::DragFloat("DragFloat (0 -> 0)", &drag_f, 0.005f, 0.0f, 0.0f, "%.3f", flags);           // To test ClampZeroRange
        //HvkGui::DragFloat("DragFloat (100 -> 100)", &drag_f, 0.005f, 100.0f, 100.0f, "%.3f", flags);
        HvkGui::DragInt("DragInt (0 -> 100)", &drag_i, 0.5f, 0, 100, "%d", flags);
        HvkGui::DragFloat4("DragFloat4 (0 -> 1)", drag_f4, 0.005f, 0.0f, 1.0f, "%.3f", flags); // Multi-component item, mostly here to document the effect of ImGuiSliderFlags_ColorMarkers.

        // Sliders
        static float slider_f = 0.5f;
        static float slider_f4[4];
        static int slider_i = 50;
        const ImGuiSliderFlags flags_for_sliders = (flags & ~ImGuiSliderFlags_WrapAround);
        HvkGui::Text("Underlying float value: %f", slider_f);
        HvkGui::SliderFloat("SliderFloat (0 -> 1)", &slider_f, 0.0f, 1.0f, "%.3f", flags_for_sliders);
        HvkGui::SliderInt("SliderInt (0 -> 100)", &slider_i, 0, 100, "%d", flags_for_sliders);
        HvkGui::SliderFloat4("SliderFloat4 (0 -> 1)", slider_f4, 0.0f, 1.0f, "%.3f", flags); // Multi-component item, mostly here to document the effect of ImGuiSliderFlags_ColorMarkers.

        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsFonts()
//-----------------------------------------------------------------------------

// Forward declare ShowFontAtlas() which isn't worth putting in public API yet
namespace ImGui { IMGUI_API void ShowFontAtlas(ImFontAtlas* atlas); }

static void DemoWindowWidgetsFonts()
{
    IMGUI_DEMO_MARKER("Widgets/Fonts");
    if (HvkGui::TreeNode("Fonts"))
    {
        ImFontAtlas* atlas = HvkGui::GetIO().Fonts;
        HvkGui::ShowFontAtlas(atlas);
        // FIXME-NEWATLAS: Provide a demo to add/create a procedural font?
        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsImages()
//-----------------------------------------------------------------------------

static void DemoWindowWidgetsImages()
{
    IMGUI_DEMO_MARKER("Widgets/Images");
    if (HvkGui::TreeNode("Images"))
    {
        ImGuiIO& io = HvkGui::GetIO();
        HvkGui::TextWrapped(
            "Below we are displaying the font texture (which is the only texture we have access to in this demo). "
            "Use the 'ImTextureID' type as storage to pass pointers or identifier to your own texture data. "
            "Hover the texture for a zoomed view!");

        // Below we are displaying the font texture because it is the only texture we have access to inside the demo!
        // Read description about ImTextureID/ImTextureRef and FAQ for details about texture identifiers.
        // If you use one of the default imgui_impl_XXXX.cpp rendering backend, they all have comments at the top
        // of their respective source file to specify what they are using as texture identifier, for example:
        // - The imgui_impl_dx11.cpp renderer expect a 'ID3D11ShaderResourceView*' pointer.
        // - The imgui_impl_opengl3.cpp renderer expect a GLuint OpenGL texture identifier, etc.
        // So with the DirectX11 backend, you call HvkGui::Image() with a 'ID3D11ShaderResourceView*' cast to ImTextureID.
        // - If you decided that ImTextureID = MyEngineTexture*, then you can pass your MyEngineTexture* pointers
        //   to HvkGui::Image(), and gather width/height through your own functions, etc.
        // - You can use ShowMetricsWindow() to inspect the draw data that are being passed to your renderer,
        //   it will help you debug issues if you are confused about it.
        // - Consider using the lower-level ImDrawList::AddImage() API, via HvkGui::GetWindowDrawList()->AddImage().
        // - Read https://github.com/ocornut/imgui/blob/master/docs/FAQ.md
        // - Read https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples

        // Grab the current texture identifier used by the font atlas.
        ImTextureRef my_tex_id = io.Fonts->TexRef;

        // Regular user code should never have to care about TexData-> fields, but since we want to display the entire texture here, we pull Width/Height from it.
        float my_tex_w = (float)io.Fonts->TexData->Width;
        float my_tex_h = (float)io.Fonts->TexData->Height;

        {
            HvkGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
            ImVec2 pos = HvkGui::GetCursorScreenPos();
            ImVec2 uv_min = ImVec2(0.0f, 0.0f); // Top-left
            ImVec2 uv_max = ImVec2(1.0f, 1.0f); // Lower-right
            HvkGui::PushStyleVar(ImGuiStyleVar_ImageBorderSize, IM_MAX(1.0f, HvkGui::GetStyle().ImageBorderSize));
            HvkGui::ImageWithBg(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
            if (HvkGui::BeginItemTooltip())
            {
                float region_sz = 32.0f;
                float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
                float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
                float zoom = 4.0f;
                if (region_x < 0.0f) { region_x = 0.0f; }
                else if (region_x > my_tex_w - region_sz) { region_x = my_tex_w - region_sz; }
                if (region_y < 0.0f) { region_y = 0.0f; }
                else if (region_y > my_tex_h - region_sz) { region_y = my_tex_h - region_sz; }
                HvkGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
                HvkGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
                ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
                ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
                HvkGui::ImageWithBg(my_tex_id, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                HvkGui::EndTooltip();
            }
            HvkGui::PopStyleVar();
        }

        IMGUI_DEMO_MARKER("Widgets/Images/Textured buttons");
        HvkGui::TextWrapped("And now some textured buttons..");
        static int pressed_count = 0;
        for (int i = 0; i < 8; i++)
        {
            // UV coordinates are often (0.0f, 0.0f) and (1.0f, 1.0f) to display an entire textures.
            // Here are trying to display only a 32x32 pixels area of the texture, hence the UV computation.
            // Read about UV coordinates here: https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
            HvkGui::PushID(i);
            if (i > 0)
                HvkGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(i - 1.0f, i - 1.0f));
            ImVec2 size = ImVec2(32.0f, 32.0f);                         // Size of the image we want to make visible
            ImVec2 uv0 = ImVec2(0.0f, 0.0f);                            // UV coordinates for lower-left
            ImVec2 uv1 = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h);    // UV coordinates for (32,32) in our texture
            ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
            ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint
            if (HvkGui::ImageButton("", my_tex_id, size, uv0, uv1, bg_col, tint_col))
                pressed_count += 1;
            if (i > 0)
                HvkGui::PopStyleVar();
            HvkGui::PopID();
            HvkGui::SameLine();
        }
        HvkGui::NewLine();
        HvkGui::Text("Pressed %d times.", pressed_count);
        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsListBoxes()
//-----------------------------------------------------------------------------

static void DemoWindowWidgetsListBoxes()
{
    IMGUI_DEMO_MARKER("Widgets/List Boxes");
    if (HvkGui::TreeNode("List Boxes"))
    {
        // BeginListBox() is essentially a thin wrapper to using BeginChild()/EndChild()
        // using the ImGuiChildFlags_FrameStyle flag for stylistic changes + displaying a label.
        // You may be tempted to simply use BeginChild() directly. However note that BeginChild() requires EndChild()
        // to always be called (inconsistent with BeginListBox()/EndListBox()).

        // Using the generic BeginListBox() API, you have full control over how to display the combo contents.
        // (your selection data could be an index, a pointer to the object, an id for the object, a flag intrusively
        // stored in the object itself, etc.)
        const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
        static int item_selected_idx = 0; // Here we store our selected data as an index.

        static bool item_highlight = false;
        int item_highlighted_idx = -1; // Here we store our highlighted data as an index.
        HvkGui::Checkbox("Highlight hovered item in second listbox", &item_highlight);

        if (HvkGui::BeginListBox("listbox 1"))
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                const bool is_selected = (item_selected_idx == n);
                if (HvkGui::Selectable(items[n], is_selected))
                    item_selected_idx = n;

                if (item_highlight && HvkGui::IsItemHovered())
                    item_highlighted_idx = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    HvkGui::SetItemDefaultFocus();
            }
            HvkGui::EndListBox();
        }
        HvkGui::SameLine(); HelpMarker("Here we are sharing selection state between both boxes.");

        // Custom size: use all width, 5 items tall
        HvkGui::Text("Full-width:");
        if (HvkGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 5 * HvkGui::GetTextLineHeightWithSpacing())))
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                bool is_selected = (item_selected_idx == n);
                ImGuiSelectableFlags flags = (item_highlighted_idx == n) ? ImGuiSelectableFlags_Highlight : 0;
                if (HvkGui::Selectable(items[n], is_selected, flags))
                    item_selected_idx = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    HvkGui::SetItemDefaultFocus();
            }
            HvkGui::EndListBox();
        }

        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsMultiComponents()
//-----------------------------------------------------------------------------

static void DemoWindowWidgetsMultiComponents()
{
    IMGUI_DEMO_MARKER("Widgets/Multi-component Widgets");
    if (HvkGui::TreeNode("Multi-component Widgets"))
    {
        static float vec4f[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
        static int vec4i[4] = { 1, 5, 100, 255 };

        HvkGui::SeparatorText("2-wide");
        HvkGui::InputFloat2("input float2", vec4f);
        HvkGui::DragFloat2("drag float2", vec4f, 0.01f, 0.0f, 1.0f);
        HvkGui::SliderFloat2("slider float2", vec4f, 0.0f, 1.0f);
        HvkGui::InputInt2("input int2", vec4i);
        HvkGui::DragInt2("drag int2", vec4i, 1, 0, 255);
        HvkGui::SliderInt2("slider int2", vec4i, 0, 255);

        HvkGui::SeparatorText("3-wide");
        HvkGui::InputFloat3("input float3", vec4f);
        HvkGui::DragFloat3("drag float3", vec4f, 0.01f, 0.0f, 1.0f);
        HvkGui::SliderFloat3("slider float3", vec4f, 0.0f, 1.0f);
        HvkGui::InputInt3("input int3", vec4i);
        HvkGui::DragInt3("drag int3", vec4i, 1, 0, 255);
        HvkGui::SliderInt3("slider int3", vec4i, 0, 255);

        HvkGui::SeparatorText("4-wide");
        HvkGui::InputFloat4("input float4", vec4f);
        HvkGui::DragFloat4("drag float4", vec4f, 0.01f, 0.0f, 1.0f);
        HvkGui::SliderFloat4("slider float4", vec4f, 0.0f, 1.0f);
        HvkGui::InputInt4("input int4", vec4i);
        HvkGui::DragInt4("drag int4", vec4i, 1, 0, 255);
        HvkGui::SliderInt4("slider int4", vec4i, 0, 255);

        HvkGui::SeparatorText("Ranges");
        static float begin = 10, end = 90;
        static int begin_i = 100, end_i = 1000;
        HvkGui::DragFloatRange2("range float", &begin, &end, 0.25f, 0.0f, 100.0f, "Min: %.1f %%", "Max: %.1f %%", ImGuiSliderFlags_AlwaysClamp);
        HvkGui::DragIntRange2("range int", &begin_i, &end_i, 5, 0, 1000, "Min: %d units", "Max: %d units");
        HvkGui::DragIntRange2("range int (no bounds)", &begin_i, &end_i, 5, 0, 0, "Min: %d units", "Max: %d units");

        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsPlotting()
//-----------------------------------------------------------------------------

static void DemoWindowWidgetsPlotting()
{
    // Plot/Graph widgets are not very good.
// Consider using a third-party library such as ImPlot: https://github.com/epezent/implot
// (see others https://github.com/ocornut/imgui/wiki/Useful-Extensions)
    IMGUI_DEMO_MARKER("Widgets/Plotting");
    if (HvkGui::TreeNode("Plotting"))
    {
        HvkGui::Text("Need better plotting and graphing? Consider using ImPlot:");
        HvkGui::TextLinkOpenURL("https://github.com/epezent/implot");
        HvkGui::Separator();

        static bool animate = true;
        HvkGui::Checkbox("Animate", &animate);

        // Plot as lines and plot as histogram
        static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
        HvkGui::PlotLines("Frame Times", arr, IM_ARRAYSIZE(arr));
        HvkGui::PlotHistogram("Histogram", arr, IM_ARRAYSIZE(arr), 0, NULL, 0.0f, 1.0f, ImVec2(0, 80.0f));
        //HvkGui::SameLine(); HelpMarker("Consider using ImPlot instead!");

        // Fill an array of contiguous float values to plot
        // Tip: If your float aren't contiguous but part of a structure, you can pass a pointer to your first float
        // and the sizeof() of your structure in the "stride" parameter.
        static float values[90] = {};
        static int values_offset = 0;
        static double refresh_time = 0.0;
        if (!animate || refresh_time == 0.0)
            refresh_time = HvkGui::GetTime();
        while (refresh_time < HvkGui::GetTime()) // Create data at fixed 60 Hz rate for the demo
        {
            static float phase = 0.0f;
            values[values_offset] = cosf(phase);
            values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
            phase += 0.10f * values_offset;
            refresh_time += 1.0f / 60.0f;
        }

        // Plots can display overlay texts
        // (in this example, we will display an average value)
        {
            float average = 0.0f;
            for (int n = 0; n < IM_ARRAYSIZE(values); n++)
                average += values[n];
            average /= (float)IM_ARRAYSIZE(values);
            char overlay[32];
            sprintf(overlay, "avg %f", average);
            HvkGui::PlotLines("Lines", values, IM_ARRAYSIZE(values), values_offset, overlay, -1.0f, 1.0f, ImVec2(0, 80.0f));
        }

        // Use functions to generate output
        // FIXME: This is actually VERY awkward because current plot API only pass in indices.
        // We probably want an API passing floats and user provide sample rate/count.
        struct Funcs
        {
            static float Sin(void*, int i) { return sinf(i * 0.1f); }
            static float Saw(void*, int i) { return (i & 1) ? 1.0f : -1.0f; }
        };
        static int func_type = 0, display_count = 70;
        HvkGui::SeparatorText("Functions");
        HvkGui::SetNextItemWidth(HvkGui::GetFontSize() * 8);
        HvkGui::Combo("func", &func_type, "Sin\0Saw\0");
        HvkGui::SameLine();
        HvkGui::SliderInt("Sample count", &display_count, 1, 400);
        float (*func)(void*, int) = (func_type == 0) ? Funcs::Sin : Funcs::Saw;
        HvkGui::PlotLines("Lines##2", func, NULL, display_count, 0, NULL, -1.0f, 1.0f, ImVec2(0, 80));
        HvkGui::PlotHistogram("Histogram##2", func, NULL, display_count, 0, NULL, -1.0f, 1.0f, ImVec2(0, 80));

        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsProgressBars()
//-----------------------------------------------------------------------------

static void DemoWindowWidgetsProgressBars()
{
    IMGUI_DEMO_MARKER("Widgets/Progress Bars");
    if (HvkGui::TreeNode("Progress Bars"))
    {
        // Animate a simple progress bar
        static float progress_accum = 0.0f, progress_dir = 1.0f;
        progress_accum += progress_dir * 0.4f * HvkGui::GetIO().DeltaTime;
        if (progress_accum >= +1.1f) { progress_accum = +1.1f; progress_dir *= -1.0f; }
        if (progress_accum <= -0.1f) { progress_accum = -0.1f; progress_dir *= -1.0f; }

        const float progress = IM_CLAMP(progress_accum, 0.0f, 1.0f);

        // Typically we would use ImVec2(-1.0f,0.0f) or ImVec2(-FLT_MIN,0.0f) to use all available width,
        // or ImVec2(width,0.0f) for a specified width. ImVec2(0.0f,0.0f) uses ItemWidth.
        HvkGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));
        HvkGui::SameLine(0.0f, HvkGui::GetStyle().ItemInnerSpacing.x);
        HvkGui::Text("Progress Bar");

        char buf[32];
        sprintf(buf, "%d/%d", (int)(progress * 1753), 1753);
        HvkGui::ProgressBar(progress, ImVec2(0.f, 0.f), buf);

        // Pass an animated negative value, e.g. -1.0f * (float)HvkGui::GetTime() is the recommended value.
        // Adjust the factor if you want to adjust the animation speed.
        HvkGui::ProgressBar(-1.0f * (float)HvkGui::GetTime(), ImVec2(0.0f, 0.0f), "Searching..");
        HvkGui::SameLine(0.0f, HvkGui::GetStyle().ItemInnerSpacing.x);
        HvkGui::Text("Indeterminate");

        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsQueryingStatuses()
//-----------------------------------------------------------------------------

static void DemoWindowWidgetsQueryingStatuses()
{
    IMGUI_DEMO_MARKER("Widgets/Querying Item Status (Edited,Active,Hovered etc.)");
    if (HvkGui::TreeNode("Querying Item Status (Edited/Active/Hovered etc.)"))
    {
        // Select an item type
        const char* item_names[] =
        {
            "Text", "Button", "Button (w/ repeat)", "Checkbox", "SliderFloat", "InputText", "InputTextMultiline", "InputFloat",
            "InputFloat3", "ColorEdit4", "Selectable", "MenuItem", "TreeNode", "TreeNode (w/ double-click)", "Combo", "ListBox"
        };
        static int item_type = 4;
        static bool item_disabled = false;
        HvkGui::Combo("Item Type", &item_type, item_names, IM_ARRAYSIZE(item_names), IM_ARRAYSIZE(item_names));
        HvkGui::SameLine();
        HelpMarker("Testing how various types of items are interacting with the IsItemXXX functions. Note that the bool return value of most ImGui function is generally equivalent to calling HvkGui::IsItemHovered().");
        HvkGui::Checkbox("Item Disabled", &item_disabled);

        // Submit selected items so we can query their status in the code following it.
        bool ret = false;
        static bool b = false;
        static float col4f[4] = { 1.0f, 0.5, 0.0f, 1.0f };
        static char str[16] = {};
        if (item_disabled)
            HvkGui::BeginDisabled(true);
        if (item_type == 0) { HvkGui::Text("ITEM: Text"); }                                              // Testing text items with no identifier/interaction
        if (item_type == 1) { ret = HvkGui::Button("ITEM: Button"); }                                    // Testing button
        if (item_type == 2) { HvkGui::PushItemFlag(ImGuiItemFlags_ButtonRepeat, true); ret = HvkGui::Button("ITEM: Button"); HvkGui::PopItemFlag(); } // Testing button (with repeater)
        if (item_type == 3) { ret = HvkGui::Checkbox("ITEM: Checkbox", &b); }                            // Testing checkbox
        if (item_type == 4) { ret = HvkGui::SliderFloat("ITEM: SliderFloat", &col4f[0], 0.0f, 1.0f); }   // Testing basic item
        if (item_type == 5) { ret = HvkGui::InputText("ITEM: InputText", &str[0], IM_ARRAYSIZE(str)); }  // Testing input text (which handles tabbing)
        if (item_type == 6) { ret = HvkGui::InputTextMultiline("ITEM: InputTextMultiline", &str[0], IM_ARRAYSIZE(str)); } // Testing input text (which uses a child window)
        if (item_type == 7) { ret = HvkGui::InputFloat("ITEM: InputFloat", col4f, 1.0f); }               // Testing +/- buttons on scalar input
        if (item_type == 8) { ret = HvkGui::InputFloat3("ITEM: InputFloat3", col4f); }                   // Testing multi-component items (IsItemXXX flags are reported merged)
        if (item_type == 9) { ret = HvkGui::ColorEdit4("ITEM: ColorEdit4", col4f); }                     // Testing multi-component items (IsItemXXX flags are reported merged)
        if (item_type == 10) { ret = HvkGui::Selectable("ITEM: Selectable"); }                            // Testing selectable item
        if (item_type == 11) { ret = HvkGui::MenuItem("ITEM: MenuItem"); }                                // Testing menu item (they use ImGuiButtonFlags_PressedOnRelease button policy)
        if (item_type == 12) { ret = HvkGui::TreeNode("ITEM: TreeNode"); if (ret) HvkGui::TreePop(); }     // Testing tree node
        if (item_type == 13) { ret = HvkGui::TreeNodeEx("ITEM: TreeNode w/ ImGuiTreeNodeFlags_OpenOnDoubleClick", ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_NoTreePushOnOpen); } // Testing tree node with ImGuiButtonFlags_PressedOnDoubleClick button policy.
        if (item_type == 14) { const char* items[] = { "Apple", "Banana", "Cherry", "Kiwi" }; static int current = 1; ret = HvkGui::Combo("ITEM: Combo", &current, items, IM_ARRAYSIZE(items)); }
        if (item_type == 15) { const char* items[] = { "Apple", "Banana", "Cherry", "Kiwi" }; static int current = 1; ret = HvkGui::ListBox("ITEM: ListBox", &current, items, IM_ARRAYSIZE(items), IM_ARRAYSIZE(items)); }

        bool hovered_delay_none = HvkGui::IsItemHovered();
        bool hovered_delay_stationary = HvkGui::IsItemHovered(ImGuiHoveredFlags_Stationary);
        bool hovered_delay_short = HvkGui::IsItemHovered(ImGuiHoveredFlags_DelayShort);
        bool hovered_delay_normal = HvkGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal);
        bool hovered_delay_tooltip = HvkGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip); // = Normal + Stationary

        // Display the values of IsItemHovered() and other common item state functions.
        // Note that the ImGuiHoveredFlags_XXX flags can be combined.
        // Because BulletText is an item itself and that would affect the output of IsItemXXX functions,
        // we query every state in a single call to avoid storing them and to simplify the code.
        HvkGui::BulletText(
            "Return value = %d\n"
            "IsItemFocused() = %d\n"
            "IsItemHovered() = %d\n"
            "IsItemHovered(_AllowWhenBlockedByPopup) = %d\n"
            "IsItemHovered(_AllowWhenBlockedByActiveItem) = %d\n"
            "IsItemHovered(_AllowWhenOverlappedByItem) = %d\n"
            "IsItemHovered(_AllowWhenOverlappedByWindow) = %d\n"
            "IsItemHovered(_AllowWhenDisabled) = %d\n"
            "IsItemHovered(_RectOnly) = %d\n"
            "IsItemActive() = %d\n"
            "IsItemEdited() = %d\n"
            "IsItemActivated() = %d\n"
            "IsItemDeactivated() = %d\n"
            "IsItemDeactivatedAfterEdit() = %d\n"
            "IsItemVisible() = %d\n"
            "IsItemClicked() = %d\n"
            "IsItemToggledOpen() = %d\n"
            "GetItemRectMin() = (%.1f, %.1f)\n"
            "GetItemRectMax() = (%.1f, %.1f)\n"
            "GetItemRectSize() = (%.1f, %.1f)",
            ret,
            HvkGui::IsItemFocused(),
            HvkGui::IsItemHovered(),
            HvkGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup),
            HvkGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem),
            HvkGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlappedByItem),
            HvkGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlappedByWindow),
            HvkGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled),
            HvkGui::IsItemHovered(ImGuiHoveredFlags_RectOnly),
            HvkGui::IsItemActive(),
            HvkGui::IsItemEdited(),
            HvkGui::IsItemActivated(),
            HvkGui::IsItemDeactivated(),
            HvkGui::IsItemDeactivatedAfterEdit(),
            HvkGui::IsItemVisible(),
            HvkGui::IsItemClicked(),
            HvkGui::IsItemToggledOpen(),
            HvkGui::GetItemRectMin().x, HvkGui::GetItemRectMin().y,
            HvkGui::GetItemRectMax().x, HvkGui::GetItemRectMax().y,
            HvkGui::GetItemRectSize().x, HvkGui::GetItemRectSize().y
        );
        HvkGui::BulletText(
            "with Hovering Delay or Stationary test:\n"
            "IsItemHovered() = %d\n"
            "IsItemHovered(_Stationary) = %d\n"
            "IsItemHovered(_DelayShort) = %d\n"
            "IsItemHovered(_DelayNormal) = %d\n"
            "IsItemHovered(_Tooltip) = %d",
            hovered_delay_none, hovered_delay_stationary, hovered_delay_short, hovered_delay_normal, hovered_delay_tooltip);

        if (item_disabled)
            HvkGui::EndDisabled();

        char buf[1] = "";
        HvkGui::InputText("unused", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_ReadOnly);
        HvkGui::SameLine();
        HelpMarker("This widget is only here to be able to tab-out of the widgets above and see e.g. Deactivated() status.");

        HvkGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Widgets/Querying Window Status (Focused,Hovered etc.)");
    if (HvkGui::TreeNode("Querying Window Status (Focused/Hovered etc.)"))
    {
        static bool embed_all_inside_a_child_window = false;
        HvkGui::Checkbox("Embed everything inside a child window for testing _RootWindow flag.", &embed_all_inside_a_child_window);
        if (embed_all_inside_a_child_window)
            HvkGui::BeginChild("outer_child", ImVec2(0, HvkGui::GetFontSize() * 20.0f), ImGuiChildFlags_Borders);

        // Testing IsWindowFocused() function with its various flags.
        HvkGui::BulletText(
            "IsWindowFocused() = %d\n"
            "IsWindowFocused(_ChildWindows) = %d\n"
            "IsWindowFocused(_ChildWindows|_NoPopupHierarchy) = %d\n"
            "IsWindowFocused(_ChildWindows|_RootWindow) = %d\n"
            "IsWindowFocused(_ChildWindows|_RootWindow|_NoPopupHierarchy) = %d\n"
            "IsWindowFocused(_RootWindow) = %d\n"
            "IsWindowFocused(_RootWindow|_NoPopupHierarchy) = %d\n"
            "IsWindowFocused(_AnyWindow) = %d\n",
            HvkGui::IsWindowFocused(),
            HvkGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows),
            HvkGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows | ImGuiFocusedFlags_NoPopupHierarchy),
            HvkGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows | ImGuiFocusedFlags_RootWindow),
            HvkGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows | ImGuiFocusedFlags_RootWindow | ImGuiFocusedFlags_NoPopupHierarchy),
            HvkGui::IsWindowFocused(ImGuiFocusedFlags_RootWindow),
            HvkGui::IsWindowFocused(ImGuiFocusedFlags_RootWindow | ImGuiFocusedFlags_NoPopupHierarchy),
            HvkGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow));

        // Testing IsWindowHovered() function with its various flags.
        HvkGui::BulletText(
            "IsWindowHovered() = %d\n"
            "IsWindowHovered(_AllowWhenBlockedByPopup) = %d\n"
            "IsWindowHovered(_AllowWhenBlockedByActiveItem) = %d\n"
            "IsWindowHovered(_ChildWindows) = %d\n"
            "IsWindowHovered(_ChildWindows|_NoPopupHierarchy) = %d\n"
            "IsWindowHovered(_ChildWindows|_RootWindow) = %d\n"
            "IsWindowHovered(_ChildWindows|_RootWindow|_NoPopupHierarchy) = %d\n"
            "IsWindowHovered(_RootWindow) = %d\n"
            "IsWindowHovered(_RootWindow|_NoPopupHierarchy) = %d\n"
            "IsWindowHovered(_ChildWindows|_AllowWhenBlockedByPopup) = %d\n"
            "IsWindowHovered(_AnyWindow) = %d\n"
            "IsWindowHovered(_Stationary) = %d\n",
            HvkGui::IsWindowHovered(),
            HvkGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup),
            HvkGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem),
            HvkGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows),
            HvkGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_NoPopupHierarchy),
            HvkGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_RootWindow),
            HvkGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_NoPopupHierarchy),
            HvkGui::IsWindowHovered(ImGuiHoveredFlags_RootWindow),
            HvkGui::IsWindowHovered(ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_NoPopupHierarchy),
            HvkGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_AllowWhenBlockedByPopup),
            HvkGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow),
            HvkGui::IsWindowHovered(ImGuiHoveredFlags_Stationary));

        HvkGui::BeginChild("child", ImVec2(0, 50), ImGuiChildFlags_Borders);
        HvkGui::Text("This is another child window for testing the _ChildWindows flag.");
        HvkGui::EndChild();
        if (embed_all_inside_a_child_window)
            HvkGui::EndChild();

        // Calling IsItemHovered() after begin returns the hovered status of the title bar.
        // This is useful in particular if you want to create a context menu associated to the title bar of a window.
        static bool test_window = false;
        HvkGui::Checkbox("Hovered/Active tests after Begin() for title bar testing", &test_window);
        if (test_window)
        {
            HvkGui::Begin("Title bar Hovered/Active tests", &test_window);
            if (HvkGui::BeginPopupContextItem()) // <-- This is using IsItemHovered()
            {
                if (HvkGui::MenuItem("Close")) { test_window = false; }
                HvkGui::EndPopup();
            }
            HvkGui::Text(
                "IsItemHovered() after begin = %d (== is title bar hovered)\n"
                "IsItemActive() after begin = %d (== is window being clicked/moved)\n",
                HvkGui::IsItemHovered(), HvkGui::IsItemActive());
            HvkGui::End();
        }

        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsSelectables()
//-----------------------------------------------------------------------------

static void DemoWindowWidgetsSelectables()
{
    IMGUI_DEMO_MARKER("Widgets/Selectables");
    //HvkGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (HvkGui::TreeNode("Selectables"))
    {
        // Selectable() has 2 overloads:
        // - The one taking "bool selected" as a read-only selection information.
        //   When Selectable() has been clicked it returns true and you can alter selection state accordingly.
        // - The one taking "bool* p_selected" as a read-write selection information (convenient in some cases)
        // The earlier is more flexible, as in real application your selection may be stored in many different ways
        // and not necessarily inside a bool value (e.g. in flags within objects, as an external list, etc).
        IMGUI_DEMO_MARKER("Widgets/Selectables/Basic");
        if (HvkGui::TreeNode("Basic"))
        {
            static bool selection[5] = { false, true, false, false };
            HvkGui::Selectable("1. I am selectable", &selection[0]);
            HvkGui::Selectable("2. I am selectable", &selection[1]);
            HvkGui::Selectable("3. I am selectable", &selection[2]);
            if (HvkGui::Selectable("4. I am double clickable", selection[3], ImGuiSelectableFlags_AllowDoubleClick))
                if (HvkGui::IsMouseDoubleClicked(0))
                    selection[3] = !selection[3];
            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Selectables/Rendering more items on the same line");
        if (HvkGui::TreeNode("Rendering more items on the same line"))
        {
            // (1) Using SetNextItemAllowOverlap()
            // (2) Using the Selectable() override that takes "bool* p_selected" parameter, the bool value is toggled automatically.
            static bool selected[3] = { false, false, false };
            HvkGui::SetNextItemAllowOverlap(); HvkGui::Selectable("main.c", &selected[0]); HvkGui::SameLine(); HvkGui::SmallButton("Link 1");
            HvkGui::SetNextItemAllowOverlap(); HvkGui::Selectable("Hello.cpp", &selected[1]); HvkGui::SameLine(); HvkGui::SmallButton("Link 2");
            HvkGui::SetNextItemAllowOverlap(); HvkGui::Selectable("Hello.h", &selected[2]); HvkGui::SameLine(); HvkGui::SmallButton("Link 3");
            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Selectables/In Tables");
        if (HvkGui::TreeNode("In Tables"))
        {
            static bool selected[10] = {};

            if (HvkGui::BeginTable("split1", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
            {
                for (int i = 0; i < 10; i++)
                {
                    char label[32];
                    sprintf(label, "Item %d", i);
                    HvkGui::TableNextColumn();
                    HvkGui::Selectable(label, &selected[i]); // FIXME-TABLE: Selection overlap
                }
                HvkGui::EndTable();
            }
            HvkGui::Spacing();
            if (HvkGui::BeginTable("split2", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
            {
                for (int i = 0; i < 10; i++)
                {
                    char label[32];
                    sprintf(label, "Item %d", i);
                    HvkGui::TableNextRow();
                    HvkGui::TableNextColumn();
                    HvkGui::Selectable(label, &selected[i], ImGuiSelectableFlags_SpanAllColumns);
                    HvkGui::TableNextColumn();
                    HvkGui::Text("Some other contents");
                    HvkGui::TableNextColumn();
                    HvkGui::Text("123456");
                }
                HvkGui::EndTable();
            }
            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Selectables/Grid");
        if (HvkGui::TreeNode("Grid"))
        {
            static char selected[4][4] = { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } };

            // Add in a bit of silly fun...
            const float time = (float)HvkGui::GetTime();
            const bool winning_state = memchr(selected, 0, sizeof(selected)) == NULL; // If all cells are selected...
            if (winning_state)
                HvkGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f + 0.5f * cosf(time * 2.0f), 0.5f + 0.5f * sinf(time * 3.0f)));

            for (int y = 0; y < 4; y++)
                for (int x = 0; x < 4; x++)
                {
                    if (x > 0)
                        HvkGui::SameLine();
                    HvkGui::PushID(y * 4 + x);
                    if (HvkGui::Selectable("Sailor", selected[y][x] != 0, 0, ImVec2(50, 50)))
                    {
                        // Toggle clicked cell + toggle neighbors
                        selected[y][x] ^= 1;
                        if (x > 0) { selected[y][x - 1] ^= 1; }
                        if (x < 3) { selected[y][x + 1] ^= 1; }
                        if (y > 0) { selected[y - 1][x] ^= 1; }
                        if (y < 3) { selected[y + 1][x] ^= 1; }
                    }
                    HvkGui::PopID();
                }

            if (winning_state)
                HvkGui::PopStyleVar();
            HvkGui::TreePop();
        }
        IMGUI_DEMO_MARKER("Widgets/Selectables/Alignment");
        if (HvkGui::TreeNode("Alignment"))
        {
            HelpMarker(
                "By default, Selectables uses style.SelectableTextAlign but it can be overridden on a per-item "
                "basis using PushStyleVar(). You'll probably want to always keep your default situation to "
                "left-align otherwise it becomes difficult to layout multiple items on a same line");
            static bool selected[3 * 3] = { true, false, true, false, true, false, true, false, true };
            for (int y = 0; y < 3; y++)
            {
                for (int x = 0; x < 3; x++)
                {
                    ImVec2 alignment = ImVec2((float)x / 2.0f, (float)y / 2.0f);
                    char name[32];
                    sprintf(name, "(%.1f,%.1f)", alignment.x, alignment.y);
                    if (x > 0) HvkGui::SameLine();
                    HvkGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, alignment);
                    HvkGui::Selectable(name, &selected[3 * y + x], ImGuiSelectableFlags_None, ImVec2(80, 80));
                    HvkGui::PopStyleVar();
                }
            }
            HvkGui::TreePop();
        }
        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsSelectionAndMultiSelect()
//-----------------------------------------------------------------------------
// Multi-selection demos
// Also read: https://github.com/ocornut/imgui/wiki/Multi-Select
//-----------------------------------------------------------------------------

static const char* ExampleNames[] =
{
    "Artichoke", "Arugula", "Asparagus", "Avocado", "Bamboo Shoots", "Bean Sprouts", "Beans", "Beet", "Belgian Endive", "Bell Pepper",
    "Bitter Gourd", "Bok Choy", "Broccoli", "Brussels Sprouts", "Burdock Root", "Cabbage", "Calabash", "Capers", "Carrot", "Cassava",
    "Cauliflower", "Celery", "Celery Root", "Celcuce", "Chayote", "Chinese Broccoli", "Corn", "Cucumber"
};

// Extra functions to add deletion support to ImGuiSelectionBasicStorage
struct ExampleSelectionWithDeletion : ImGuiSelectionBasicStorage
{
    // Find which item should be Focused after deletion.
    // Call _before_ item submission. Return an index in the before-deletion item list, your item loop should call SetKeyboardFocusHere() on it.
    // The subsequent ApplyDeletionPostLoop() code will use it to apply Selection.
    // - We cannot provide this logic in core Dear ImGui because we don't have access to selection data.
    // - We don't actually manipulate the ImVector<> here, only in ApplyDeletionPostLoop(), but using similar API for consistency and flexibility.
    // - Important: Deletion only works if the underlying ImGuiID for your items are stable: aka not depend on their index, but on e.g. item id/ptr.
    // FIXME-MULTISELECT: Doesn't take account of the possibility focus target will be moved during deletion. Need refocus or scroll offset.
    int ApplyDeletionPreLoop(ImGuiMultiSelectIO* ms_io, int items_count)
    {
        if (Size == 0)
            return -1;

        // If focused item is not selected...
        const int focused_idx = (int)ms_io->NavIdItem;  // Index of currently focused item
        if (ms_io->NavIdSelected == false)  // This is merely a shortcut, == Contains(adapter->IndexToStorage(items, focused_idx))
        {
            ms_io->RangeSrcReset = true;    // Request to recover RangeSrc from NavId next frame. Would be ok to reset even when NavIdSelected==true, but it would take an extra frame to recover RangeSrc when deleting a selected item.
            return focused_idx;             // Request to focus same item after deletion.
        }

        // If focused item is selected: land on first unselected item after focused item.
        for (int idx = focused_idx + 1; idx < items_count; idx++)
            if (!Contains(GetStorageIdFromIndex(idx)))
                return idx;

        // If focused item is selected: otherwise return last unselected item before focused item.
        for (int idx = IM_MIN(focused_idx, items_count) - 1; idx >= 0; idx--)
            if (!Contains(GetStorageIdFromIndex(idx)))
                return idx;

        return -1;
    }

    // Rewrite item list (delete items) + update selection.
    // - Call after EndMultiSelect()
    // - We cannot provide this logic in core Dear ImGui because we don't have access to your items, nor to selection data.
    template<typename ITEM_TYPE>
    void ApplyDeletionPostLoop(ImGuiMultiSelectIO* ms_io, ImVector<ITEM_TYPE>& items, int item_curr_idx_to_select)
    {
        // Rewrite item list (delete items) + convert old selection index (before deletion) to new selection index (after selection).
        // If NavId was not part of selection, we will stay on same item.
        ImVector<ITEM_TYPE> new_items;
        new_items.reserve(items.Size - Size);
        int item_next_idx_to_select = -1;
        for (int idx = 0; idx < items.Size; idx++)
        {
            if (!Contains(GetStorageIdFromIndex(idx)))
                new_items.push_back(items[idx]);
            if (item_curr_idx_to_select == idx)
                item_next_idx_to_select = new_items.Size - 1;
        }
        items.swap(new_items);

        // Update selection
        Clear();
        if (item_next_idx_to_select != -1 && ms_io->NavIdSelected)
            SetItemSelected(GetStorageIdFromIndex(item_next_idx_to_select), true);
    }
};

// Example: Implement dual list box storage and interface
struct ExampleDualListBox
{
    ImVector<ImGuiID>           Items[2];               // ID is index into ExampleName[]
    ImGuiSelectionBasicStorage  Selections[2];          // Store ExampleItemId into selection
    bool                        OptKeepSorted = true;

    void MoveAll(int src, int dst)
    {
        IM_ASSERT((src == 0 && dst == 1) || (src == 1 && dst == 0));
        for (ImGuiID item_id : Items[src])
            Items[dst].push_back(item_id);
        Items[src].clear();
        SortItems(dst);
        Selections[src].Swap(Selections[dst]);
        Selections[src].Clear();
    }
    void MoveSelected(int src, int dst)
    {
        for (int src_n = 0; src_n < Items[src].Size; src_n++)
        {
            ImGuiID item_id = Items[src][src_n];
            if (!Selections[src].Contains(item_id))
                continue;
            Items[src].erase(&Items[src][src_n]); // FIXME-OPT: Could be implemented more optimally (rebuild src items and swap)
            Items[dst].push_back(item_id);
            src_n--;
        }
        if (OptKeepSorted)
            SortItems(dst);
        Selections[src].Swap(Selections[dst]);
        Selections[src].Clear();
    }
    void ApplySelectionRequests(ImGuiMultiSelectIO* ms_io, int side)
    {
        // In this example we store item id in selection (instead of item index)
        Selections[side].UserData = Items[side].Data;
        Selections[side].AdapterIndexToStorageId = [](ImGuiSelectionBasicStorage* self, int idx) { ImGuiID* items = (ImGuiID*)self->UserData; return items[idx]; };
        Selections[side].ApplyRequests(ms_io);
    }
    static int IMGUI_CDECL CompareItemsByValue(const void* lhs, const void* rhs)
    {
        const int* a = (const int*)lhs;
        const int* b = (const int*)rhs;
        return *a - *b;
    }
    void SortItems(int n)
    {
        qsort(Items[n].Data, (size_t)Items[n].Size, sizeof(Items[n][0]), CompareItemsByValue);
    }
    void Show()
    {
        //if (HvkGui::Checkbox("Sorted", &OptKeepSorted) && OptKeepSorted) { SortItems(0); SortItems(1); }
        if (HvkGui::BeginTable("split", 3, ImGuiTableFlags_None))
        {
            HvkGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);    // Left side
            HvkGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);      // Buttons
            HvkGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);    // Right side
            HvkGui::TableNextRow();

            int request_move_selected = -1;
            int request_move_all = -1;
            float child_height_0 = 0.0f;
            for (int side = 0; side < 2; side++)
            {
                // FIXME-MULTISELECT: Dual List Box: Add context menus
                // FIXME-NAV: Using ImGuiWindowFlags_NavFlattened exhibit many issues.
                ImVector<ImGuiID>& items = Items[side];
                ImGuiSelectionBasicStorage& selection = Selections[side];

                HvkGui::TableSetColumnIndex((side == 0) ? 0 : 2);
                HvkGui::Text("%s (%d)", (side == 0) ? "Available" : "Basket", items.Size);

                // Submit scrolling range to avoid glitches on moving/deletion
                const float items_height = HvkGui::GetTextLineHeightWithSpacing();
                HvkGui::SetNextWindowContentSize(ImVec2(0.0f, items.Size * items_height));

                bool child_visible;
                if (side == 0)
                {
                    // Left child is resizable
                    HvkGui::SetNextWindowSizeConstraints(ImVec2(0.0f, HvkGui::GetFrameHeightWithSpacing() * 4), ImVec2(FLT_MAX, FLT_MAX));
                    child_visible = HvkGui::BeginChild("0", ImVec2(-FLT_MIN, HvkGui::GetFontSize() * 20), ImGuiChildFlags_FrameStyle | ImGuiChildFlags_ResizeY);
                    child_height_0 = HvkGui::GetWindowSize().y;
                }
                else
                {
                    // Right child use same height as left one
                    child_visible = HvkGui::BeginChild("1", ImVec2(-FLT_MIN, child_height_0), ImGuiChildFlags_FrameStyle);
                }
                if (child_visible)
                {
                    ImGuiMultiSelectFlags flags = ImGuiMultiSelectFlags_None;
                    ImGuiMultiSelectIO* ms_io = HvkGui::BeginMultiSelect(flags, selection.Size, items.Size);
                    ApplySelectionRequests(ms_io, side);

                    for (int item_n = 0; item_n < items.Size; item_n++)
                    {
                        ImGuiID item_id = items[item_n];
                        bool item_is_selected = selection.Contains(item_id);
                        HvkGui::SetNextItemSelectionUserData(item_n);
                        HvkGui::Selectable(ExampleNames[item_id], item_is_selected, ImGuiSelectableFlags_AllowDoubleClick);
                        if (HvkGui::IsItemFocused())
                        {
                            // FIXME-MULTISELECT: Dual List Box: Transfer focus
                            if (HvkGui::IsKeyPressed(ImGuiKey_Enter) || HvkGui::IsKeyPressed(ImGuiKey_KeypadEnter))
                                request_move_selected = side;
                            if (HvkGui::IsMouseDoubleClicked(0)) // FIXME-MULTISELECT: Double-click on multi-selection?
                                request_move_selected = side;
                        }
                    }

                    ms_io = HvkGui::EndMultiSelect();
                    ApplySelectionRequests(ms_io, side);
                }
                HvkGui::EndChild();
            }

            // Buttons columns
            HvkGui::TableSetColumnIndex(1);
            HvkGui::NewLine();
            //ImVec2 button_sz = { HvkGui::CalcTextSize(">>").x + HvkGui::GetStyle().FramePadding.x * 2.0f, HvkGui::GetFrameHeight() + padding.y * 2.0f };
            ImVec2 button_sz = { HvkGui::GetFrameHeight(), HvkGui::GetFrameHeight() };

            // (Using BeginDisabled()/EndDisabled() works but feels distracting given how it is currently visualized)
            if (HvkGui::Button(">>", button_sz))
                request_move_all = 0;
            if (HvkGui::Button(">", button_sz))
                request_move_selected = 0;
            if (HvkGui::Button("<", button_sz))
                request_move_selected = 1;
            if (HvkGui::Button("<<", button_sz))
                request_move_all = 1;

            // Process requests
            if (request_move_all != -1)
                MoveAll(request_move_all, request_move_all ^ 1);
            if (request_move_selected != -1)
                MoveSelected(request_move_selected, request_move_selected ^ 1);

            // FIXME-MULTISELECT: Support action from outside
            /*
            if (OptKeepSorted == false)
            {
                HvkGui::NewLine();
                if (HvkGui::ArrowButton("MoveUp", ImGuiDir_Up)) {}
                if (HvkGui::ArrowButton("MoveDown", ImGuiDir_Down)) {}
            }
            */

            HvkGui::EndTable();
        }
    }
};

static void DemoWindowWidgetsSelectionAndMultiSelect(ImGuiDemoWindowData* demo_data)
{
    IMGUI_DEMO_MARKER("Widgets/Selection State & Multi-Select");
    if (HvkGui::TreeNode("Selection State & Multi-Select"))
    {
        HelpMarker("Selections can be built using Selectable(), TreeNode() or other widgets. Selection state is owned by application code/data.");

        HvkGui::BulletText("Wiki page:");
        HvkGui::SameLine();
        HvkGui::TextLinkOpenURL("imgui/wiki/Multi-Select", "https://github.com/ocornut/imgui/wiki/Multi-Select");

        // Without any fancy API: manage single-selection yourself.
        IMGUI_DEMO_MARKER("Widgets/Selection State/Single-Select");
        if (HvkGui::TreeNode("Single-Select"))
        {
            static int selected = -1;
            for (int n = 0; n < 5; n++)
            {
                char buf[32];
                sprintf(buf, "Object %d", n);
                if (HvkGui::Selectable(buf, selected == n))
                    selected = n;
            }
            HvkGui::TreePop();
        }

        // Demonstrate implementation a most-basic form of multi-selection manually
        // This doesn't support the Shift modifier which requires BeginMultiSelect()!
        IMGUI_DEMO_MARKER("Widgets/Selection State/Multi-Select (manual/simplified, without BeginMultiSelect)");
        if (HvkGui::TreeNode("Multi-Select (manual/simplified, without BeginMultiSelect)"))
        {
            HelpMarker("Hold Ctrl and Click to select multiple items.");
            static bool selection[5] = { false, false, false, false, false };
            for (int n = 0; n < 5; n++)
            {
                char buf[32];
                sprintf(buf, "Object %d", n);
                if (HvkGui::Selectable(buf, selection[n]))
                {
                    if (!HvkGui::GetIO().KeyCtrl) // Clear selection when Ctrl is not held
                        memset(selection, 0, sizeof(selection));
                    selection[n] ^= 1; // Toggle current item
                }
            }
            HvkGui::TreePop();
        }

        // Demonstrate handling proper multi-selection using the BeginMultiSelect/EndMultiSelect API.
        // Shift+Click w/ Ctrl and other standard features are supported.
        // We use the ImGuiSelectionBasicStorage helper which you may freely reimplement.
        IMGUI_DEMO_MARKER("Widgets/Selection State/Multi-Select");
        if (HvkGui::TreeNode("Multi-Select"))
        {
            HvkGui::Text("Supported features:");
            HvkGui::BulletText("Keyboard navigation (arrows, page up/down, home/end, space).");
            HvkGui::BulletText("Ctrl modifier to preserve and toggle selection.");
            HvkGui::BulletText("Shift modifier for range selection.");
            HvkGui::BulletText("Ctrl+A to select all.");
            HvkGui::BulletText("Escape to clear selection.");
            HvkGui::BulletText("Click and drag to box-select.");
            HvkGui::Text("Tip: Use 'Demo->Tools->Debug Log->Selection' to see selection requests as they happen.");

            // Use default selection.Adapter: Pass index to SetNextItemSelectionUserData(), store index in Selection
            const int ITEMS_COUNT = 50;
            static ImGuiSelectionBasicStorage selection;
            HvkGui::Text("Selection: %d/%d", selection.Size, ITEMS_COUNT);

            // The BeginChild() has no purpose for selection logic, other that offering a scrolling region.
            if (HvkGui::BeginChild("##Basket", ImVec2(-FLT_MIN, HvkGui::GetFontSize() * 20), ImGuiChildFlags_FrameStyle | ImGuiChildFlags_ResizeY))
            {
                ImGuiMultiSelectFlags flags = ImGuiMultiSelectFlags_ClearOnEscape | ImGuiMultiSelectFlags_BoxSelect1d;
                ImGuiMultiSelectIO* ms_io = HvkGui::BeginMultiSelect(flags, selection.Size, ITEMS_COUNT);
                selection.ApplyRequests(ms_io);

                for (int n = 0; n < ITEMS_COUNT; n++)
                {
                    char label[64];
                    sprintf(label, "Object %05d: %s", n, ExampleNames[n % IM_ARRAYSIZE(ExampleNames)]);
                    bool item_is_selected = selection.Contains((ImGuiID)n);
                    HvkGui::SetNextItemSelectionUserData(n);
                    HvkGui::Selectable(label, item_is_selected);
                }

                ms_io = HvkGui::EndMultiSelect();
                selection.ApplyRequests(ms_io);
            }
            HvkGui::EndChild();
            HvkGui::TreePop();
        }

        // Demonstrate using the clipper with BeginMultiSelect()/EndMultiSelect()
        IMGUI_DEMO_MARKER("Widgets/Selection State/Multi-Select (with clipper)");
        if (HvkGui::TreeNode("Multi-Select (with clipper)"))
        {
            // Use default selection.Adapter: Pass index to SetNextItemSelectionUserData(), store index in Selection
            static ImGuiSelectionBasicStorage selection;

            HvkGui::Text("Added features:");
            HvkGui::BulletText("Using ImGuiListClipper.");

            const int ITEMS_COUNT = 10000;
            HvkGui::Text("Selection: %d/%d", selection.Size, ITEMS_COUNT);
            if (HvkGui::BeginChild("##Basket", ImVec2(-FLT_MIN, HvkGui::GetFontSize() * 20), ImGuiChildFlags_FrameStyle | ImGuiChildFlags_ResizeY))
            {
                ImGuiMultiSelectFlags flags = ImGuiMultiSelectFlags_ClearOnEscape | ImGuiMultiSelectFlags_BoxSelect1d;
                ImGuiMultiSelectIO* ms_io = HvkGui::BeginMultiSelect(flags, selection.Size, ITEMS_COUNT);
                selection.ApplyRequests(ms_io);

                ImGuiListClipper clipper;
                clipper.Begin(ITEMS_COUNT);
                if (ms_io->RangeSrcItem != -1)
                    clipper.IncludeItemByIndex((int)ms_io->RangeSrcItem); // Ensure RangeSrc item is not clipped.
                while (clipper.Step())
                {
                    for (int n = clipper.DisplayStart; n < clipper.DisplayEnd; n++)
                    {
                        char label[64];
                        sprintf(label, "Object %05d: %s", n, ExampleNames[n % IM_ARRAYSIZE(ExampleNames)]);
                        bool item_is_selected = selection.Contains((ImGuiID)n);
                        HvkGui::SetNextItemSelectionUserData(n);
                        HvkGui::Selectable(label, item_is_selected);
                    }
                }

                ms_io = HvkGui::EndMultiSelect();
                selection.ApplyRequests(ms_io);
            }
            HvkGui::EndChild();
            HvkGui::TreePop();
        }

        // Demonstrate dynamic item list + deletion support using the BeginMultiSelect/EndMultiSelect API.
        // In order to support Deletion without any glitches you need to:
        // - (1) If items are submitted in their own scrolling area, submit contents size SetNextWindowContentSize() ahead of time to prevent one-frame readjustment of scrolling.
        // - (2) Items needs to have persistent ID Stack identifier = ID needs to not depends on their index. PushID(index) = KO. PushID(item_id) = OK. This is in order to focus items reliably after a selection.
        // - (3) BeginXXXX process
        // - (4) Focus process
        // - (5) EndXXXX process
        IMGUI_DEMO_MARKER("Widgets/Selection State/Multi-Select (with deletion)");
        if (HvkGui::TreeNode("Multi-Select (with deletion)"))
        {
            // Storing items data separately from selection data.
            // (you may decide to store selection data inside your item (aka intrusive storage) if you don't need multiple views over same items)
            // Use a custom selection.Adapter: store item identifier in Selection (instead of index)
            static ImVector<ImGuiID> items;
            static ExampleSelectionWithDeletion selection;
            selection.UserData = (void*)&items;
            selection.AdapterIndexToStorageId = [](ImGuiSelectionBasicStorage* self, int idx) { ImVector<ImGuiID>* p_items = (ImVector<ImGuiID>*)self->UserData; return (*p_items)[idx]; }; // Index -> ID

            HvkGui::Text("Added features:");
            HvkGui::BulletText("Dynamic list with Delete key support.");
            HvkGui::Text("Selection size: %d/%d", selection.Size, items.Size);

            // Initialize default list with 50 items + button to add/remove items.
            static ImGuiID items_next_id = 0;
            if (items_next_id == 0)
                for (ImGuiID n = 0; n < 50; n++)
                    items.push_back(items_next_id++);
            if (HvkGui::SmallButton("Add 20 items"))     { for (int n = 0; n < 20; n++) { items.push_back(items_next_id++); } }
            HvkGui::SameLine();
            if (HvkGui::SmallButton("Remove 20 items"))  { for (int n = IM_MIN(20, items.Size); n > 0; n--) { selection.SetItemSelected(items.back(), false); items.pop_back(); } }

            // (1) Extra to support deletion: Submit scrolling range to avoid glitches on deletion
            const float items_height = HvkGui::GetTextLineHeightWithSpacing();
            HvkGui::SetNextWindowContentSize(ImVec2(0.0f, items.Size * items_height));

            if (HvkGui::BeginChild("##Basket", ImVec2(-FLT_MIN, HvkGui::GetFontSize() * 20), ImGuiChildFlags_FrameStyle | ImGuiChildFlags_ResizeY))
            {
                ImGuiMultiSelectFlags flags = ImGuiMultiSelectFlags_ClearOnEscape | ImGuiMultiSelectFlags_BoxSelect1d;
                ImGuiMultiSelectIO* ms_io = HvkGui::BeginMultiSelect(flags, selection.Size, items.Size);
                selection.ApplyRequests(ms_io);

                const bool want_delete = HvkGui::Shortcut(ImGuiKey_Delete, ImGuiInputFlags_Repeat) && (selection.Size > 0);
                const int item_curr_idx_to_focus = want_delete ? selection.ApplyDeletionPreLoop(ms_io, items.Size) : -1;

                for (int n = 0; n < items.Size; n++)
                {
                    const ImGuiID item_id = items[n];
                    char label[64];
                    sprintf(label, "Object %05u: %s", item_id, ExampleNames[item_id % IM_ARRAYSIZE(ExampleNames)]);

                    bool item_is_selected = selection.Contains(item_id);
                    HvkGui::SetNextItemSelectionUserData(n);
                    HvkGui::Selectable(label, item_is_selected);
                    if (item_curr_idx_to_focus == n)
                        HvkGui::SetKeyboardFocusHere(-1);
                }

                // Apply multi-select requests
                ms_io = HvkGui::EndMultiSelect();
                selection.ApplyRequests(ms_io);
                if (want_delete)
                    selection.ApplyDeletionPostLoop(ms_io, items, item_curr_idx_to_focus);
            }
            HvkGui::EndChild();
            HvkGui::TreePop();
        }

        // Implement a Dual List Box (#6648)
        IMGUI_DEMO_MARKER("Widgets/Selection State/Multi-Select (dual list box)");
        if (HvkGui::TreeNode("Multi-Select (dual list box)"))
        {
            // Init default state
            static ExampleDualListBox dlb;
            if (dlb.Items[0].Size == 0 && dlb.Items[1].Size == 0)
                for (int item_id = 0; item_id < IM_ARRAYSIZE(ExampleNames); item_id++)
                    dlb.Items[0].push_back((ImGuiID)item_id);

            // Show
            dlb.Show();

            HvkGui::TreePop();
        }

        // Demonstrate using the clipper with BeginMultiSelect()/EndMultiSelect()
        IMGUI_DEMO_MARKER("Widgets/Selection State/Multi-Select (in a table)");
        if (HvkGui::TreeNode("Multi-Select (in a table)"))
        {
            static ImGuiSelectionBasicStorage selection;

            const int ITEMS_COUNT = 10000;
            HvkGui::Text("Selection: %d/%d", selection.Size, ITEMS_COUNT);
            if (HvkGui::BeginTable("##Basket", 2, ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter, ImVec2(0.0f, HvkGui::GetFontSize() * 20)))
            {
                HvkGui::TableSetupColumn("Object");
                HvkGui::TableSetupColumn("Action");
                HvkGui::TableSetupScrollFreeze(0, 1);
                HvkGui::TableHeadersRow();

                ImGuiMultiSelectFlags flags = ImGuiMultiSelectFlags_ClearOnEscape | ImGuiMultiSelectFlags_BoxSelect1d;
                ImGuiMultiSelectIO* ms_io = HvkGui::BeginMultiSelect(flags, selection.Size, ITEMS_COUNT);
                selection.ApplyRequests(ms_io);

                ImGuiListClipper clipper;
                clipper.Begin(ITEMS_COUNT);
                if (ms_io->RangeSrcItem != -1)
                    clipper.IncludeItemByIndex((int)ms_io->RangeSrcItem); // Ensure RangeSrc item is not clipped.
                while (clipper.Step())
                {
                    for (int n = clipper.DisplayStart; n < clipper.DisplayEnd; n++)
                    {
                        HvkGui::TableNextRow();
                        HvkGui::TableNextColumn();
                        HvkGui::PushID(n);
                        char label[64];
                        sprintf(label, "Object %05d: %s", n, ExampleNames[n % IM_ARRAYSIZE(ExampleNames)]);
                        bool item_is_selected = selection.Contains((ImGuiID)n);
                        HvkGui::SetNextItemSelectionUserData(n);
                        HvkGui::Selectable(label, item_is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap);
                        HvkGui::TableNextColumn();
                        HvkGui::SmallButton("hello");
                        HvkGui::PopID();
                    }
                }

                ms_io = HvkGui::EndMultiSelect();
                selection.ApplyRequests(ms_io);
                HvkGui::EndTable();
            }
            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Selection State/Multi-Select (checkboxes)");
        if (HvkGui::TreeNode("Multi-Select (checkboxes)"))
        {
            HvkGui::Text("In a list of checkboxes (not selectable):");
            HvkGui::BulletText("Using _NoAutoSelect + _NoAutoClear flags.");
            HvkGui::BulletText("Shift+Click to check multiple boxes.");
            HvkGui::BulletText("Shift+Keyboard to copy current value to other boxes.");

            // If you have an array of checkboxes, you may want to use NoAutoSelect + NoAutoClear and the ImGuiSelectionExternalStorage helper.
            static bool items[20] = {};
            static ImGuiMultiSelectFlags flags = ImGuiMultiSelectFlags_NoAutoSelect | ImGuiMultiSelectFlags_NoAutoClear | ImGuiMultiSelectFlags_ClearOnEscape;
            HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_NoAutoSelect", &flags, ImGuiMultiSelectFlags_NoAutoSelect);
            HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_NoAutoClear", &flags, ImGuiMultiSelectFlags_NoAutoClear);
            HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_BoxSelect2d", &flags, ImGuiMultiSelectFlags_BoxSelect2d); // Cannot use ImGuiMultiSelectFlags_BoxSelect1d as checkboxes are varying width.

            if (HvkGui::BeginChild("##Basket", ImVec2(-FLT_MIN, HvkGui::GetFontSize() * 20), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY))
            {
                ImGuiMultiSelectIO* ms_io = HvkGui::BeginMultiSelect(flags, -1, IM_ARRAYSIZE(items));
                ImGuiSelectionExternalStorage storage_wrapper;
                storage_wrapper.UserData = (void*)items;
                storage_wrapper.AdapterSetItemSelected = [](ImGuiSelectionExternalStorage* self, int n, bool selected) { bool* array = (bool*)self->UserData; array[n] = selected; };
                storage_wrapper.ApplyRequests(ms_io);
                for (int n = 0; n < 20; n++)
                {
                    char label[32];
                    sprintf(label, "Item %d", n);
                    HvkGui::SetNextItemSelectionUserData(n);
                    HvkGui::Checkbox(label, &items[n]);
                }
                ms_io = HvkGui::EndMultiSelect();
                storage_wrapper.ApplyRequests(ms_io);
            }
            HvkGui::EndChild();

            HvkGui::TreePop();
        }

        // Demonstrate individual selection scopes in same window
        IMGUI_DEMO_MARKER("Widgets/Selection State/Multi-Select (multiple scopes)");
        if (HvkGui::TreeNode("Multi-Select (multiple scopes)"))
        {
            // Use default select: Pass index to SetNextItemSelectionUserData(), store index in Selection
            const int SCOPES_COUNT = 3;
            const int ITEMS_COUNT = 8; // Per scope
            static ImGuiSelectionBasicStorage selections_data[SCOPES_COUNT];

            // Use ImGuiMultiSelectFlags_ScopeRect to not affect other selections in same window.
            static ImGuiMultiSelectFlags flags = ImGuiMultiSelectFlags_ScopeRect | ImGuiMultiSelectFlags_ClearOnEscape;// | ImGuiMultiSelectFlags_ClearOnClickVoid;
            if (HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_ScopeWindow", &flags, ImGuiMultiSelectFlags_ScopeWindow) && (flags & ImGuiMultiSelectFlags_ScopeWindow))
                flags &= ~ImGuiMultiSelectFlags_ScopeRect;
            if (HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_ScopeRect", &flags, ImGuiMultiSelectFlags_ScopeRect) && (flags & ImGuiMultiSelectFlags_ScopeRect))
                flags &= ~ImGuiMultiSelectFlags_ScopeWindow;
            HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_ClearOnClickVoid", &flags, ImGuiMultiSelectFlags_ClearOnClickVoid);
            HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_BoxSelect1d", &flags, ImGuiMultiSelectFlags_BoxSelect1d);

            for (int selection_scope_n = 0; selection_scope_n < SCOPES_COUNT; selection_scope_n++)
            {
                HvkGui::PushID(selection_scope_n);
                ImGuiSelectionBasicStorage* selection = &selections_data[selection_scope_n];
                ImGuiMultiSelectIO* ms_io = HvkGui::BeginMultiSelect(flags, selection->Size, ITEMS_COUNT);
                selection->ApplyRequests(ms_io);

                HvkGui::SeparatorText("Selection scope");
                HvkGui::Text("Selection size: %d/%d", selection->Size, ITEMS_COUNT);

                for (int n = 0; n < ITEMS_COUNT; n++)
                {
                    char label[64];
                    sprintf(label, "Object %05d: %s", n, ExampleNames[n % IM_ARRAYSIZE(ExampleNames)]);
                    bool item_is_selected = selection->Contains((ImGuiID)n);
                    HvkGui::SetNextItemSelectionUserData(n);
                    HvkGui::Selectable(label, item_is_selected);
                }

                // Apply multi-select requests
                ms_io = HvkGui::EndMultiSelect();
                selection->ApplyRequests(ms_io);
                HvkGui::PopID();
            }
            HvkGui::TreePop();
        }

        // See ShowExampleAppAssetsBrowser()
        if (HvkGui::TreeNode("Multi-Select (tiled assets browser)"))
        {
            HvkGui::Checkbox("Assets Browser", &demo_data->ShowAppAssetsBrowser);
            HvkGui::Text("(also access from 'Examples->Assets Browser' in menu)");
            HvkGui::TreePop();
        }

        // Demonstrate supporting multiple-selection in a tree.
        // - We don't use linear indices for selection user data, but our ExampleTreeNode* pointer directly!
        //   This showcase how SetNextItemSelectionUserData() never assume indices!
        // - The difficulty here is to "interpolate" from RangeSrcItem to RangeDstItem in the SetAll/SetRange request.
        //   We want this interpolation to match what the user sees: in visible order, skipping closed nodes.
        //   This is implemented by our TreeGetNextNodeInVisibleOrder() user-space helper.
        // - Important: In a real codebase aiming to implement full-featured selectable tree with custom filtering, you
        //   are more likely to build an array mapping sequential indices to visible tree nodes, since your
        //   filtering/search + clipping process will benefit from it. Having this will make this interpolation much easier.
        // - Consider this a prototype: we are working toward simplifying some of it.
        IMGUI_DEMO_MARKER("Widgets/Selection State/Multi-Select (trees)");
        if (HvkGui::TreeNode("Multi-Select (trees)"))
        {
            HelpMarker(
                "This is rather advanced and experimental. If you are getting started with multi-select, "
                "please don't start by looking at how to use it for a tree!\n\n"
                "Future versions will try to simplify and formalize some of this.");

            struct ExampleTreeFuncs
            {
                static void DrawNode(ExampleTreeNode* node, ImGuiSelectionBasicStorage* selection)
                {
                    ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
                    tree_node_flags |= ImGuiTreeNodeFlags_NavLeftJumpsToParent; // Enable pressing left to jump to parent
                    if (node->Childs.Size == 0)
                        tree_node_flags |= ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_Leaf;
                    if (selection->Contains((ImGuiID)node->UID))
                        tree_node_flags |= ImGuiTreeNodeFlags_Selected;

                    // Using SetNextItemStorageID() to specify storage id, so we can easily peek into
                    // the storage holding open/close stage, using our TreeNodeGetOpen/TreeNodeSetOpen() functions.
                    HvkGui::SetNextItemSelectionUserData((ImGuiSelectionUserData)(intptr_t)node);
                    HvkGui::SetNextItemStorageID((ImGuiID)node->UID);
                    if (HvkGui::TreeNodeEx(node->Name, tree_node_flags))
                    {
                        for (ExampleTreeNode* child : node->Childs)
                            DrawNode(child, selection);
                        HvkGui::TreePop();
                    }
                    else if (HvkGui::IsItemToggledOpen())
                    {
                        TreeCloseAndUnselectChildNodes(node, selection);
                    }
                }

                static bool TreeNodeGetOpen(ExampleTreeNode* node)
                {
                    return HvkGui::GetStateStorage()->GetBool((ImGuiID)node->UID);
                }

                static void TreeNodeSetOpen(ExampleTreeNode* node, bool open)
                {
                    HvkGui::GetStateStorage()->SetBool((ImGuiID)node->UID, open);
                }

                // When closing a node: 1) close and unselect all child nodes, 2) select parent if any child was selected.
                // FIXME: This is currently handled by user logic but I'm hoping to eventually provide tree node
                // features to do this automatically, e.g. a ImGuiTreeNodeFlags_AutoCloseChildNodes etc.
                static int TreeCloseAndUnselectChildNodes(ExampleTreeNode* node, ImGuiSelectionBasicStorage* selection, int depth = 0)
                {
                    // Recursive close (the test for depth == 0 is because we call this on a node that was just closed!)
                    int unselected_count = selection->Contains((ImGuiID)node->UID) ? 1 : 0;
                    if (depth == 0 || TreeNodeGetOpen(node))
                    {
                        for (ExampleTreeNode* child : node->Childs)
                            unselected_count += TreeCloseAndUnselectChildNodes(child, selection, depth + 1);
                        TreeNodeSetOpen(node, false);
                    }

                    // Select root node if any of its child was selected, otherwise unselect
                    selection->SetItemSelected((ImGuiID)node->UID, (depth == 0 && unselected_count > 0));
                    return unselected_count;
                }

                // Apply multi-selection requests
                static void ApplySelectionRequests(ImGuiMultiSelectIO* ms_io, ExampleTreeNode* tree, ImGuiSelectionBasicStorage* selection)
                {
                    for (ImGuiSelectionRequest& req : ms_io->Requests)
                    {
                        if (req.Type == ImGuiSelectionRequestType_SetAll)
                        {
                            if (req.Selected)
                                TreeSetAllInOpenNodes(tree, selection, req.Selected);
                            else
                                selection->Clear();
                        }
                        else if (req.Type == ImGuiSelectionRequestType_SetRange)
                        {
                            ExampleTreeNode* first_node = (ExampleTreeNode*)(intptr_t)req.RangeFirstItem;
                            ExampleTreeNode* last_node = (ExampleTreeNode*)(intptr_t)req.RangeLastItem;
                            for (ExampleTreeNode* node = first_node; node != NULL; node = TreeGetNextNodeInVisibleOrder(node, last_node))
                                selection->SetItemSelected((ImGuiID)node->UID, req.Selected);
                        }
                    }
                }

                static void TreeSetAllInOpenNodes(ExampleTreeNode* node, ImGuiSelectionBasicStorage* selection, bool selected)
                {
                    if (node->Parent != NULL) // Root node isn't visible nor selectable in our scheme
                        selection->SetItemSelected((ImGuiID)node->UID, selected);
                    if (node->Parent == NULL || TreeNodeGetOpen(node))
                        for (ExampleTreeNode* child : node->Childs)
                            TreeSetAllInOpenNodes(child, selection, selected);
                }

                // Interpolate in *user-visible order* AND only *over opened nodes*.
                // If you have a sequential mapping tables (e.g. generated after a filter/search pass) this would be simpler.
                // Here the tricks are that:
                // - we store/maintain ExampleTreeNode::IndexInParent which allows implementing a linear iterator easily, without searches, without recursion.
                //   this could be replaced by a search in parent, aka 'int index_in_parent = curr_node->Parent->Childs.find_index(curr_node)'
                //   which would only be called when crossing from child to a parent, aka not too much.
                // - we call SetNextItemStorageID() before our TreeNode() calls with an ID which doesn't relate to UI stack,
                //   making it easier to call TreeNodeGetOpen()/TreeNodeSetOpen() from any location.
                static ExampleTreeNode* TreeGetNextNodeInVisibleOrder(ExampleTreeNode* curr_node, ExampleTreeNode* last_node)
                {
                    // Reached last node
                    if (curr_node == last_node)
                        return NULL;

                    // Recurse into childs. Query storage to tell if the node is open.
                    if (curr_node->Childs.Size > 0 && TreeNodeGetOpen(curr_node))
                        return curr_node->Childs[0];

                    // Next sibling, then into our own parent
                    while (curr_node->Parent != NULL)
                    {
                        if (curr_node->IndexInParent + 1 < curr_node->Parent->Childs.Size)
                            return curr_node->Parent->Childs[curr_node->IndexInParent + 1];
                        curr_node = curr_node->Parent;
                    }
                    return NULL;
                }

            }; // ExampleTreeFuncs

            static ImGuiSelectionBasicStorage selection;
            if (demo_data->DemoTree == NULL)
                demo_data->DemoTree = ExampleTree_CreateDemoTree(); // Create tree once
            HvkGui::Text("Selection size: %d", selection.Size);

            if (HvkGui::BeginChild("##Tree", ImVec2(-FLT_MIN, HvkGui::GetFontSize() * 20), ImGuiChildFlags_FrameStyle | ImGuiChildFlags_ResizeY))
            {
                ExampleTreeNode* tree = demo_data->DemoTree;
                ImGuiMultiSelectFlags ms_flags = ImGuiMultiSelectFlags_ClearOnEscape | ImGuiMultiSelectFlags_BoxSelect2d;
                ImGuiMultiSelectIO* ms_io = HvkGui::BeginMultiSelect(ms_flags, selection.Size, -1);
                ExampleTreeFuncs::ApplySelectionRequests(ms_io, tree, &selection);
                for (ExampleTreeNode* node : tree->Childs)
                    ExampleTreeFuncs::DrawNode(node, &selection);
                ms_io = HvkGui::EndMultiSelect();
                ExampleTreeFuncs::ApplySelectionRequests(ms_io, tree, &selection);
            }
            HvkGui::EndChild();

            HvkGui::TreePop();
        }

        // Advanced demonstration of BeginMultiSelect()
        // - Showcase clipping.
        // - Showcase deletion.
        // - Showcase basic drag and drop.
        // - Showcase TreeNode variant (note that tree node don't expand in the demo: supporting expanding tree nodes + clipping a separate thing).
        // - Showcase using inside a table.
        IMGUI_DEMO_MARKER("Widgets/Selection State/Multi-Select (advanced)");
        //HvkGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (HvkGui::TreeNode("Multi-Select (advanced)"))
        {
            // Options
            enum WidgetType { WidgetType_Selectable, WidgetType_TreeNode };
            static bool use_clipper = true;
            static bool use_deletion = true;
            static bool use_drag_drop = true;
            static bool show_in_table = false;
            static bool show_color_button = true;
            static ImGuiMultiSelectFlags flags = ImGuiMultiSelectFlags_ClearOnEscape | ImGuiMultiSelectFlags_BoxSelect1d;
            static WidgetType widget_type = WidgetType_Selectable;

            if (HvkGui::TreeNode("Options"))
            {
                if (HvkGui::RadioButton("Selectables", widget_type == WidgetType_Selectable)) { widget_type = WidgetType_Selectable; }
                HvkGui::SameLine();
                if (HvkGui::RadioButton("Tree nodes", widget_type == WidgetType_TreeNode)) { widget_type = WidgetType_TreeNode; }
                HvkGui::SameLine();
                HelpMarker("TreeNode() is technically supported but... using this correctly is more complicated (you need some sort of linear/random access to your tree, which is suited to advanced trees setups already implementing filters and clipper. We will work toward simplifying and demoing this.\n\nFor now the tree demo is actually a little bit meaningless because it is an empty tree with only root nodes.");
                HvkGui::Checkbox("Enable clipper", &use_clipper);
                HvkGui::Checkbox("Enable deletion", &use_deletion);
                HvkGui::Checkbox("Enable drag & drop", &use_drag_drop);
                HvkGui::Checkbox("Show in a table", &show_in_table);
                HvkGui::Checkbox("Show color button", &show_color_button);
                HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_SingleSelect", &flags, ImGuiMultiSelectFlags_SingleSelect);
                HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_NoSelectAll", &flags, ImGuiMultiSelectFlags_NoSelectAll);
                HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_NoRangeSelect", &flags, ImGuiMultiSelectFlags_NoRangeSelect);
                HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_NoAutoSelect", &flags, ImGuiMultiSelectFlags_NoAutoSelect);
                HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_NoAutoClear", &flags, ImGuiMultiSelectFlags_NoAutoClear);
                HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_NoAutoClearOnReselect", &flags, ImGuiMultiSelectFlags_NoAutoClearOnReselect);
                HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_NoSelectOnRightClick", &flags, ImGuiMultiSelectFlags_NoSelectOnRightClick);
                HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_BoxSelect1d", &flags, ImGuiMultiSelectFlags_BoxSelect1d);
                HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_BoxSelect2d", &flags, ImGuiMultiSelectFlags_BoxSelect2d);
                HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_BoxSelectNoScroll", &flags, ImGuiMultiSelectFlags_BoxSelectNoScroll);
                HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_ClearOnEscape", &flags, ImGuiMultiSelectFlags_ClearOnEscape);
                HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_ClearOnClickVoid", &flags, ImGuiMultiSelectFlags_ClearOnClickVoid);
                if (HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_ScopeWindow", &flags, ImGuiMultiSelectFlags_ScopeWindow) && (flags & ImGuiMultiSelectFlags_ScopeWindow))
                    flags &= ~ImGuiMultiSelectFlags_ScopeRect;
                if (HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_ScopeRect", &flags, ImGuiMultiSelectFlags_ScopeRect) && (flags & ImGuiMultiSelectFlags_ScopeRect))
                    flags &= ~ImGuiMultiSelectFlags_ScopeWindow;
                if (HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_SelectOnClick", &flags, ImGuiMultiSelectFlags_SelectOnClick) && (flags & ImGuiMultiSelectFlags_SelectOnClick))
                    flags &= ~ImGuiMultiSelectFlags_SelectOnClickRelease;
                if (HvkGui::CheckboxFlags("ImGuiMultiSelectFlags_SelectOnClickRelease", &flags, ImGuiMultiSelectFlags_SelectOnClickRelease) && (flags & ImGuiMultiSelectFlags_SelectOnClickRelease))
                    flags &= ~ImGuiMultiSelectFlags_SelectOnClick;
                HvkGui::SameLine(); HelpMarker("Allow dragging an unselected item without altering selection.");
                HvkGui::TreePop();
            }

            // Initialize default list with 1000 items.
            // Use default selection.Adapter: Pass index to SetNextItemSelectionUserData(), store index in Selection
            static ImVector<int> items;
            static int items_next_id = 0;
            if (items_next_id == 0) { for (int n = 0; n < 1000; n++) { items.push_back(items_next_id++); } }
            static ExampleSelectionWithDeletion selection;
            static bool request_deletion_from_menu = false; // Queue deletion triggered from context menu

            HvkGui::Text("Selection size: %d/%d", selection.Size, items.Size);

            const float items_height = (widget_type == WidgetType_TreeNode) ? HvkGui::GetTextLineHeight() : HvkGui::GetTextLineHeightWithSpacing();
            HvkGui::SetNextWindowContentSize(ImVec2(0.0f, items.Size * items_height));
            if (HvkGui::BeginChild("##Basket", ImVec2(-FLT_MIN, HvkGui::GetFontSize() * 20), ImGuiChildFlags_FrameStyle | ImGuiChildFlags_ResizeY))
            {
                ImVec2 color_button_sz(HvkGui::GetFontSize(), HvkGui::GetFontSize());
                if (widget_type == WidgetType_TreeNode)
                    HvkGui::PushStyleVarY(ImGuiStyleVar_ItemSpacing, 0.0f);

                ImGuiMultiSelectIO* ms_io = HvkGui::BeginMultiSelect(flags, selection.Size, items.Size);
                selection.ApplyRequests(ms_io);

                const bool want_delete = (HvkGui::Shortcut(ImGuiKey_Delete, ImGuiInputFlags_Repeat) && (selection.Size > 0)) || request_deletion_from_menu;
                const int item_curr_idx_to_focus = want_delete ? selection.ApplyDeletionPreLoop(ms_io, items.Size) : -1;
                request_deletion_from_menu = false;

                if (show_in_table)
                {
                    if (widget_type == WidgetType_TreeNode)
                        HvkGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0.0f, 0.0f));
                    HvkGui::BeginTable("##Split", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_NoPadOuterX);
                    HvkGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 0.70f);
                    HvkGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 0.30f);
                    //HvkGui::PushStyleVarY(ImGuiStyleVar_ItemSpacing, 0.0f);
                }

                ImGuiListClipper clipper;
                if (use_clipper)
                {
                    clipper.Begin(items.Size);
                    if (item_curr_idx_to_focus != -1)
                        clipper.IncludeItemByIndex(item_curr_idx_to_focus); // Ensure focused item is not clipped.
                    if (ms_io->RangeSrcItem != -1)
                        clipper.IncludeItemByIndex((int)ms_io->RangeSrcItem); // Ensure RangeSrc item is not clipped.
                }

                while (!use_clipper || clipper.Step())
                {
                    const int item_begin = use_clipper ? clipper.DisplayStart : 0;
                    const int item_end = use_clipper ? clipper.DisplayEnd : items.Size;
                    for (int n = item_begin; n < item_end; n++)
                    {
                        if (show_in_table)
                            HvkGui::TableNextColumn();

                        const int item_id = items[n];
                        const char* item_category = ExampleNames[item_id % IM_ARRAYSIZE(ExampleNames)];
                        char label[64];
                        sprintf(label, "Object %05d: %s", item_id, item_category);

                        // IMPORTANT: for deletion refocus to work we need object ID to be stable,
                        // aka not depend on their index in the list. Here we use our persistent item_id
                        // instead of index to build a unique ID that will persist.
                        // (If we used PushID(index) instead, focus wouldn't be restored correctly after deletion).
                        HvkGui::PushID(item_id);

                        // Emit a color button, to test that Shift+LeftArrow landing on an item that is not part
                        // of the selection scope doesn't erroneously alter our selection.
                        if (show_color_button)
                        {
                            ImU32 dummy_col = (ImU32)((unsigned int)n * 0xC250B74B) | IM_COL32_A_MASK;
                            HvkGui::ColorButton("##", ImColor(dummy_col), ImGuiColorEditFlags_NoTooltip, color_button_sz);
                            HvkGui::SameLine();
                        }

                        // Submit item
                        bool item_is_selected = selection.Contains((ImGuiID)n);
                        bool item_is_open = false;
                        HvkGui::SetNextItemSelectionUserData(n);
                        if (widget_type == WidgetType_Selectable)
                        {
                            HvkGui::Selectable(label, item_is_selected, ImGuiSelectableFlags_None);
                        }
                        else if (widget_type == WidgetType_TreeNode)
                        {
                            ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
                            if (item_is_selected)
                                tree_node_flags |= ImGuiTreeNodeFlags_Selected;
                            item_is_open = HvkGui::TreeNodeEx(label, tree_node_flags);
                        }

                        // Focus (for after deletion)
                        if (item_curr_idx_to_focus == n)
                            HvkGui::SetKeyboardFocusHere(-1);

                        // Drag and Drop
                        if (use_drag_drop && HvkGui::BeginDragDropSource())
                        {
                            // Create payload with full selection OR single unselected item.
                            // (the later is only possible when using ImGuiMultiSelectFlags_SelectOnClickRelease)
                            if (HvkGui::GetDragDropPayload() == NULL)
                            {
                                ImVector<int> payload_items;
                                void* it = NULL;
                                ImGuiID id = 0;
                                if (!item_is_selected)
                                    payload_items.push_back(item_id);
                                else
                                    while (selection.GetNextSelectedItem(&it, &id))
                                        payload_items.push_back((int)id);
                                HvkGui::SetDragDropPayload("MULTISELECT_DEMO_ITEMS", payload_items.Data, (size_t)payload_items.size_in_bytes());
                            }

                            // Display payload content in tooltip
                            const ImGuiPayload* payload = HvkGui::GetDragDropPayload();
                            const int* payload_items = (int*)payload->Data;
                            const int payload_count = (int)payload->DataSize / (int)sizeof(int);
                            if (payload_count == 1)
                                HvkGui::Text("Object %05d: %s", payload_items[0], ExampleNames[payload_items[0] % IM_ARRAYSIZE(ExampleNames)]);
                            else
                                HvkGui::Text("Dragging %d objects", payload_count);

                            HvkGui::EndDragDropSource();
                        }

                        if (widget_type == WidgetType_TreeNode && item_is_open)
                            HvkGui::TreePop();

                        // Right-click: context menu
                        if (HvkGui::BeginPopupContextItem())
                        {
                            HvkGui::BeginDisabled(!use_deletion || selection.Size == 0);
                            sprintf(label, "Delete %d item(s)###DeleteSelected", selection.Size);
                            if (HvkGui::Selectable(label))
                                request_deletion_from_menu = true;
                            HvkGui::EndDisabled();
                            HvkGui::Selectable("Close");
                            HvkGui::EndPopup();
                        }

                        // Demo content within a table
                        if (show_in_table)
                        {
                            HvkGui::TableNextColumn();
                            HvkGui::SetNextItemWidth(-FLT_MIN);
                            HvkGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                            HvkGui::InputText("###NoLabel", (char*)(void*)item_category, strlen(item_category), ImGuiInputTextFlags_ReadOnly);
                            HvkGui::PopStyleVar();
                        }

                        HvkGui::PopID();
                    }
                    if (!use_clipper)
                        break;
                }

                if (show_in_table)
                {
                    HvkGui::EndTable();
                    if (widget_type == WidgetType_TreeNode)
                        HvkGui::PopStyleVar();
                }

                // Apply multi-select requests
                ms_io = HvkGui::EndMultiSelect();
                selection.ApplyRequests(ms_io);
                if (want_delete)
                    selection.ApplyDeletionPostLoop(ms_io, items, item_curr_idx_to_focus);

                if (widget_type == WidgetType_TreeNode)
                    HvkGui::PopStyleVar();
            }
            HvkGui::EndChild();
            HvkGui::TreePop();
        }
        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsTabs()
//-----------------------------------------------------------------------------

static void EditTabBarFittingPolicyFlags(ImGuiTabBarFlags* p_flags)
{
    if ((*p_flags & ImGuiTabBarFlags_FittingPolicyMask_) == 0)
        *p_flags |= ImGuiTabBarFlags_FittingPolicyDefault_;
    if (HvkGui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyMixed", p_flags, ImGuiTabBarFlags_FittingPolicyMixed))
        *p_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyMixed);
    if (HvkGui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyShrink", p_flags, ImGuiTabBarFlags_FittingPolicyShrink))
        *p_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyShrink);
    if (HvkGui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyScroll", p_flags, ImGuiTabBarFlags_FittingPolicyScroll))
        *p_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyScroll);
}

static void DemoWindowWidgetsTabs()
{
    IMGUI_DEMO_MARKER("Widgets/Tabs");
    if (HvkGui::TreeNode("Tabs"))
    {
        IMGUI_DEMO_MARKER("Widgets/Tabs/Basic");
        if (HvkGui::TreeNode("Basic"))
        {
            ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
            if (HvkGui::BeginTabBar("MyTabBar", tab_bar_flags))
            {
                if (HvkGui::BeginTabItem("Avocado"))
                {
                    HvkGui::Text("This is the Avocado tab!\nblah blah blah blah blah");
                    HvkGui::EndTabItem();
                }
                if (HvkGui::BeginTabItem("Broccoli"))
                {
                    HvkGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
                    HvkGui::EndTabItem();
                }
                if (HvkGui::BeginTabItem("Cucumber"))
                {
                    HvkGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
                    HvkGui::EndTabItem();
                }
                HvkGui::EndTabBar();
            }
            HvkGui::Separator();
            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Tabs/Advanced & Close Button");
        if (HvkGui::TreeNode("Advanced & Close Button"))
        {
            // Expose a couple of the available flags. In most cases you may just call BeginTabBar() with no flags (0).
            static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
            HvkGui::CheckboxFlags("ImGuiTabBarFlags_Reorderable", &tab_bar_flags, ImGuiTabBarFlags_Reorderable);
            HvkGui::CheckboxFlags("ImGuiTabBarFlags_AutoSelectNewTabs", &tab_bar_flags, ImGuiTabBarFlags_AutoSelectNewTabs);
            HvkGui::CheckboxFlags("ImGuiTabBarFlags_TabListPopupButton", &tab_bar_flags, ImGuiTabBarFlags_TabListPopupButton);
            HvkGui::CheckboxFlags("ImGuiTabBarFlags_NoCloseWithMiddleMouseButton", &tab_bar_flags, ImGuiTabBarFlags_NoCloseWithMiddleMouseButton);
            HvkGui::CheckboxFlags("ImGuiTabBarFlags_DrawSelectedOverline", &tab_bar_flags, ImGuiTabBarFlags_DrawSelectedOverline);
            EditTabBarFittingPolicyFlags(&tab_bar_flags);

            // Tab Bar
            HvkGui::AlignTextToFramePadding();
            HvkGui::Text("Opened:");
            const char* names[4] = { "Artichoke", "Beetroot", "Celery", "Daikon" };
            static bool opened[4] = { true, true, true, true }; // Persistent user state
            for (int n = 0; n < IM_ARRAYSIZE(opened); n++)
            {
                HvkGui::SameLine();
                HvkGui::Checkbox(names[n], &opened[n]);
            }

            // Passing a bool* to BeginTabItem() is similar to passing one to Begin():
            // the underlying bool will be set to false when the tab is closed.
            if (HvkGui::BeginTabBar("MyTabBar", tab_bar_flags))
            {
                for (int n = 0; n < IM_ARRAYSIZE(opened); n++)
                    if (opened[n] && HvkGui::BeginTabItem(names[n], &opened[n], ImGuiTabItemFlags_None))
                    {
                        HvkGui::Text("This is the %s tab!", names[n]);
                        if (n & 1)
                            HvkGui::Text("I am an odd tab.");
                        HvkGui::EndTabItem();
                    }
                HvkGui::EndTabBar();
            }
            HvkGui::Separator();
            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Tabs/TabItemButton & Leading-Trailing flags");
        if (HvkGui::TreeNode("TabItemButton & Leading/Trailing flags"))
        {
            static ImVector<int> active_tabs;
            static int next_tab_id = 0;
            if (next_tab_id == 0) // Initialize with some default tabs
                for (int i = 0; i < 3; i++)
                    active_tabs.push_back(next_tab_id++);

            // TabItemButton() and Leading/Trailing flags are distinct features which we will demo together.
            // (It is possible to submit regular tabs with Leading/Trailing flags, or TabItemButton tabs without Leading/Trailing flags...
            // but they tend to make more sense together)
            static bool show_leading_button = true;
            static bool show_trailing_button = true;
            HvkGui::Checkbox("Show Leading TabItemButton()", &show_leading_button);
            HvkGui::Checkbox("Show Trailing TabItemButton()", &show_trailing_button);

            // Expose some other flags which are useful to showcase how they interact with Leading/Trailing tabs
            static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyShrink;
            EditTabBarFittingPolicyFlags(&tab_bar_flags);

            if (HvkGui::BeginTabBar("MyTabBar", tab_bar_flags))
            {
                // Demo a Leading TabItemButton(): click the "?" button to open a menu
                if (show_leading_button)
                    if (HvkGui::TabItemButton("?", ImGuiTabItemFlags_Leading | ImGuiTabItemFlags_NoTooltip))
                        HvkGui::OpenPopup("MyHelpMenu");
                if (HvkGui::BeginPopup("MyHelpMenu"))
                {
                    HvkGui::Selectable("Hello!");
                    HvkGui::EndPopup();
                }

                // Demo Trailing Tabs: click the "+" button to add a new tab.
                // (In your app you may want to use a font icon instead of the "+")
                // We submit it before the regular tabs, but thanks to the ImGuiTabItemFlags_Trailing flag it will always appear at the end.
                if (show_trailing_button)
                    if (HvkGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
                        active_tabs.push_back(next_tab_id++); // Add new tab

                // Submit our regular tabs
                for (int n = 0; n < active_tabs.Size; )
                {
                    bool open = true;
                    char name[16];
                    snprintf(name, IM_ARRAYSIZE(name), "%04d", active_tabs[n]);
                    if (HvkGui::BeginTabItem(name, &open, ImGuiTabItemFlags_None))
                    {
                        HvkGui::Text("This is the %s tab!", name);
                        HvkGui::EndTabItem();
                    }

                    if (!open)
                        active_tabs.erase(active_tabs.Data + n);
                    else
                        n++;
                }

                HvkGui::EndTabBar();
            }
            HvkGui::Separator();
            HvkGui::TreePop();
        }
        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsText()
//-----------------------------------------------------------------------------

static void DemoWindowWidgetsText()
{
    IMGUI_DEMO_MARKER("Widgets/Text");
    if (HvkGui::TreeNode("Text"))
    {
        IMGUI_DEMO_MARKER("Widgets/Text/Colored Text");
        if (HvkGui::TreeNode("Colorful Text"))
        {
            // Using shortcut. You can use PushStyleColor()/PopStyleColor() for more flexibility.
            HvkGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Pink");
            HvkGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Yellow");
            HvkGui::TextDisabled("Disabled");
            HvkGui::SameLine(); HelpMarker("The TextDisabled color is stored in ImGuiStyle.");
            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Text/Font Size");
        if (HvkGui::TreeNode("Font Size"))
        {
            ImGuiStyle& style = HvkGui::GetStyle();
            const float global_scale = style.FontScaleMain * style.FontScaleDpi;
            HvkGui::Text("style.FontScaleMain = %0.2f", style.FontScaleMain);
            HvkGui::Text("style.FontScaleDpi = %0.2f", style.FontScaleDpi);
            HvkGui::Text("global_scale = ~%0.2f", global_scale); // This is not technically accurate as internal scales may apply, but conceptually let's pretend it is.
            HvkGui::Text("FontSize = %0.2f", HvkGui::GetFontSize());

            HvkGui::SeparatorText("");
            static float custom_size = 16.0f;
            HvkGui::SliderFloat("custom_size", &custom_size, 10.0f, 100.0f, "%.0f");
            HvkGui::Text("HvkGui::PushFont(nullptr, custom_size);");
            HvkGui::PushFont(NULL, custom_size);
            HvkGui::Text("FontSize = %.2f (== %.2f * global_scale)", HvkGui::GetFontSize(), custom_size);
            HvkGui::PopFont();

            HvkGui::SeparatorText("");
            static float custom_scale = 1.0f;
            HvkGui::SliderFloat("custom_scale", &custom_scale, 0.5f, 4.0f, "%.2f");
            HvkGui::Text("HvkGui::PushFont(nullptr, style.FontSizeBase * custom_scale);");
            HvkGui::PushFont(NULL, style.FontSizeBase * custom_scale);
            HvkGui::Text("FontSize = %.2f (== style.FontSizeBase * %.2f * global_scale)", HvkGui::GetFontSize(), custom_scale);
            HvkGui::PopFont();

            HvkGui::SeparatorText("");
            for (float scaling = 0.5f; scaling <= 4.0f; scaling += 0.5f)
            {
                HvkGui::PushFont(NULL, style.FontSizeBase * scaling);
                HvkGui::Text("FontSize = %.2f (== style.FontSizeBase * %.2f * global_scale)", HvkGui::GetFontSize(), scaling);
                HvkGui::PopFont();
            }

            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Text/Word Wrapping");
        if (HvkGui::TreeNode("Word Wrapping"))
        {
            // Using shortcut. You can use PushTextWrapPos()/PopTextWrapPos() for more flexibility.
            HvkGui::TextWrapped(
                "This text should automatically wrap on the edge of the window. The current implementation "
                "for text wrapping follows simple rules suitable for English and possibly other languages.");
            HvkGui::Spacing();

            static float wrap_width = 200.0f;
            HvkGui::SliderFloat("Wrap width", &wrap_width, -20, 600, "%.0f");

            ImDrawList* draw_list = HvkGui::GetWindowDrawList();
            for (int n = 0; n < 2; n++)
            {
                HvkGui::Text("Test paragraph %d:", n);
                ImVec2 pos = HvkGui::GetCursorScreenPos();
                ImVec2 marker_min = ImVec2(pos.x + wrap_width, pos.y);
                ImVec2 marker_max = ImVec2(pos.x + wrap_width + 10, pos.y + HvkGui::GetTextLineHeight());
                HvkGui::PushTextWrapPos(HvkGui::GetCursorPos().x + wrap_width);
                if (n == 0)
                    HvkGui::Text("The lazy dog is a good dog. This paragraph should fit within %.0f pixels. Testing a 1 character word. The quick brown fox jumps over the lazy dog.", wrap_width);
                else
                    HvkGui::Text("aaaaaaaa bbbbbbbb, c cccccccc,dddddddd. d eeeeeeee   ffffffff. gggggggg!hhhhhhhh");

                // Draw actual text bounding box, following by marker of our expected limit (should not overlap!)
                draw_list->AddRect(HvkGui::GetItemRectMin(), HvkGui::GetItemRectMax(), IM_COL32(255, 255, 0, 255));
                draw_list->AddRectFilled(marker_min, marker_max, IM_COL32(255, 0, 255, 255));
                HvkGui::PopTextWrapPos();
            }

            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Text/UTF-8 Text");
        if (HvkGui::TreeNode("UTF-8 Text"))
        {
            // UTF-8 test with Japanese characters
            // (Needs a suitable font? Try "Google Noto" or "Arial Unicode". See docs/FONTS.md for details.)
            // - From C++11 you can use the u8"my text" syntax to encode literal strings as UTF-8
            // - For earlier compiler, you may be able to encode your sources as UTF-8 (e.g. in Visual Studio, you
            //   can save your source files as 'UTF-8 without signature').
            // - FOR THIS DEMO FILE ONLY, BECAUSE WE WANT TO SUPPORT OLD COMPILERS, WE ARE *NOT* INCLUDING RAW UTF-8
            //   CHARACTERS IN THIS SOURCE FILE. Instead we are encoding a few strings with hexadecimal constants.
            //   Don't do this in your application! Please use u8"text in any language" in your application!
            // Note that characters values are preserved even by InputText() if the font cannot be displayed,
            // so you can safely copy & paste garbled characters into another application.
            HvkGui::TextWrapped(
                "CJK text will only appear if the font was loaded with the appropriate CJK character ranges. "
                "Call io.Fonts->AddFontFromFileTTF() manually to load extra character ranges. "
                "Read docs/FONTS.md for details.");
            HvkGui::Text("Hiragana: \xe3\x81\x8b\xe3\x81\x8d\xe3\x81\x8f\xe3\x81\x91\xe3\x81\x93 (kakikukeko)");
            HvkGui::Text("Kanjis: \xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e (nihongo)");
            static char buf[32] = "\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e";
            //static char buf[32] = u8"NIHONGO"; // <- this is how you would write it with C++11, using real kanjis
            HvkGui::InputText("UTF-8 input", buf, IM_ARRAYSIZE(buf));
            HvkGui::TreePop();
        }
        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsTextFilter()
//-----------------------------------------------------------------------------

static void DemoWindowWidgetsTextFilter()
{
    IMGUI_DEMO_MARKER("Widgets/Text Filter");
    if (HvkGui::TreeNode("Text Filter"))
    {
        // Helper class to easy setup a text filter.
        // You may want to implement a more feature-full filtering scheme in your own application.
        HelpMarker("Not a widget per-se, but ImGuiTextFilter is a helper to perform simple filtering on text strings.");
        static ImGuiTextFilter filter;
        HvkGui::Text("Filter usage:\n"
            "  \"\"         display all lines\n"
            "  \"xxx\"      display lines containing \"xxx\"\n"
            "  \"xxx,yyy\"  display lines containing \"xxx\" or \"yyy\"\n"
            "  \"-xxx\"     hide lines containing \"xxx\"");
        filter.Draw();
        const char* lines[] = { "aaa1.c", "bbb1.c", "ccc1.c", "aaa2.cpp", "bbb2.cpp", "ccc2.cpp", "abc.h", "hello, world" };
        for (int i = 0; i < IM_ARRAYSIZE(lines); i++)
            if (filter.PassFilter(lines[i]))
                HvkGui::BulletText("%s", lines[i]);
        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsTextInput()
//-----------------------------------------------------------------------------

static void DemoWindowWidgetsTextInput()
{
    // To wire InputText() with std::string or any other custom string type,
    // see the "Text Input > Resize Callback" section of this demo, and the misc/cpp/imgui_stdlib.h file.
    IMGUI_DEMO_MARKER("Widgets/Text Input");
    if (HvkGui::TreeNode("Text Input"))
    {
        IMGUI_DEMO_MARKER("Widgets/Text Input/Multi-line Text Input");
        if (HvkGui::TreeNode("Multi-line Text Input"))
        {
            // WE ARE USING A FIXED-SIZE BUFFER FOR SIMPLICITY HERE.
            // If you want to use InputText() with std::string or any custom dynamic string type:
            // - For std::string: use the wrapper in misc/cpp/imgui_stdlib.h/.cpp
            // - Otherwise, see the 'Dear ImGui Demo->Widgets->Text Input->Resize Callback' for using ImGuiInputTextFlags_CallbackResize.
            static char text[1024 * 16] =
                "/*\n"
                " The Pentium F00F bug, shorthand for F0 0F C7 C8,\n"
                " the hexadecimal encoding of one offending instruction,\n"
                " more formally, the invalid operand with locked CMPXCHG8B\n"
                " instruction bug, is a design flaw in the majority of\n"
                " Intel Pentium, Pentium MMX, and Pentium OverDrive\n"
                " processors (all in the P5 microarchitecture).\n"
                "*/\n\n"
                "label:\n"
                "\tlock cmpxchg8b eax\n";

            static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
            HelpMarker("You can use the ImGuiInputTextFlags_CallbackResize facility if you need to wire InputTextMultiline() to a dynamic string type. See misc/cpp/imgui_stdlib.h for an example. (This is not demonstrated in imgui_demo.cpp because we don't want to include <string> in here)");
            HvkGui::CheckboxFlags("ImGuiInputTextFlags_ReadOnly", &flags, ImGuiInputTextFlags_ReadOnly);
            HvkGui::CheckboxFlags("ImGuiInputTextFlags_WordWrap", &flags, ImGuiInputTextFlags_WordWrap);
            HvkGui::SameLine(); HelpMarker("Feature is currently in Beta. Please read comments in imgui.h");
            HvkGui::CheckboxFlags("ImGuiInputTextFlags_AllowTabInput", &flags, ImGuiInputTextFlags_AllowTabInput);
            HvkGui::SameLine(); HelpMarker("When _AllowTabInput is set, passing through the widget with Tabbing doesn't automatically activate it, in order to also cycling through subsequent widgets.");
            HvkGui::CheckboxFlags("ImGuiInputTextFlags_CtrlEnterForNewLine", &flags, ImGuiInputTextFlags_CtrlEnterForNewLine);
            HvkGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, HvkGui::GetTextLineHeight() * 16), flags);
            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Text Input/Filtered Text Input");
        if (HvkGui::TreeNode("Filtered Text Input"))
        {
            struct TextFilters
            {
                // Modify character input by altering 'data->Eventchar' (ImGuiInputTextFlags_CallbackCharFilter callback)
                static int FilterCasingSwap(ImGuiInputTextCallbackData* data)
                {
                    if (data->EventChar >= 'a' && data->EventChar <= 'z') { data->EventChar -= 'a' - 'A'; } // Lowercase becomes uppercase
                    else if (data->EventChar >= 'A' && data->EventChar <= 'Z') { data->EventChar += 'a' - 'A'; } // Uppercase becomes lowercase
                    return 0;
                }

                // Return 0 (pass) if the character is 'i' or 'm' or 'g' or 'u' or 'i', otherwise return 1 (filter out)
                static int FilterImGuiLetters(ImGuiInputTextCallbackData* data)
                {
                    if (data->EventChar < 256 && strchr("imgui", (char)data->EventChar))
                        return 0;
                    return 1;
                }
            };

            static char buf1[32] = ""; HvkGui::InputText("default", buf1, IM_ARRAYSIZE(buf1));
            static char buf2[32] = ""; HvkGui::InputText("decimal", buf2, IM_ARRAYSIZE(buf2), ImGuiInputTextFlags_CharsDecimal);
            static char buf3[32] = ""; HvkGui::InputText("hexadecimal", buf3, IM_ARRAYSIZE(buf3), ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase);
            static char buf4[32] = ""; HvkGui::InputText("uppercase", buf4, IM_ARRAYSIZE(buf4), ImGuiInputTextFlags_CharsUppercase);
            static char buf5[32] = ""; HvkGui::InputText("no blank", buf5, IM_ARRAYSIZE(buf5), ImGuiInputTextFlags_CharsNoBlank);
            static char buf6[32] = ""; HvkGui::InputText("casing swap", buf6, IM_ARRAYSIZE(buf6), ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterCasingSwap); // Use CharFilter callback to replace characters.
            static char buf7[32] = ""; HvkGui::InputText("\"imgui\"", buf7, IM_ARRAYSIZE(buf7), ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters); // Use CharFilter callback to disable some characters.
            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Text Input/Password input");
        if (HvkGui::TreeNode("Password Input"))
        {
            static char password[64] = "password123";
            HvkGui::InputText("password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);
            HvkGui::SameLine(); HelpMarker("Display all characters as '*'.\nDisable clipboard cut and copy.\nDisable logging.\n");
            HvkGui::InputTextWithHint("password (w/ hint)", "<password>", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);
            HvkGui::InputText("password (clear)", password, IM_ARRAYSIZE(password));
            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Text Input/Completion, History, Edit Callbacks");
        if (HvkGui::TreeNode("Completion, History, Edit Callbacks"))
        {
            struct Funcs
            {
                static int MyCallback(ImGuiInputTextCallbackData* data)
                {
                    if (data->EventFlag == ImGuiInputTextFlags_CallbackCompletion)
                    {
                        data->InsertChars(data->CursorPos, "..");
                    }
                    else if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory)
                    {
                        if (data->EventKey == ImGuiKey_UpArrow)
                        {
                            data->DeleteChars(0, data->BufTextLen);
                            data->InsertChars(0, "Pressed Up!");
                            data->SelectAll();
                        }
                        else if (data->EventKey == ImGuiKey_DownArrow)
                        {
                            data->DeleteChars(0, data->BufTextLen);
                            data->InsertChars(0, "Pressed Down!");
                            data->SelectAll();
                        }
                    }
                    else if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit)
                    {
                        // Toggle casing of first character
                        char c = data->Buf[0];
                        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) data->Buf[0] ^= 32;
                        data->BufDirty = true;

                        // Increment a counter
                        int* p_int = (int*)data->UserData;
                        *p_int = *p_int + 1;
                    }
                    return 0;
                }
            };
            static char buf1[64];
            HvkGui::InputText("Completion", buf1, IM_ARRAYSIZE(buf1), ImGuiInputTextFlags_CallbackCompletion, Funcs::MyCallback);
            HvkGui::SameLine(); HelpMarker(
                "Here we append \"..\" each time Tab is pressed. "
                "See 'Examples>Console' for a more meaningful demonstration of using this callback.");

            static char buf2[64];
            HvkGui::InputText("History", buf2, IM_ARRAYSIZE(buf2), ImGuiInputTextFlags_CallbackHistory, Funcs::MyCallback);
            HvkGui::SameLine(); HelpMarker(
                "Here we replace and select text each time Up/Down are pressed. "
                "See 'Examples>Console' for a more meaningful demonstration of using this callback.");

            static char buf3[64];
            static int edit_count = 0;
            HvkGui::InputText("Edit", buf3, IM_ARRAYSIZE(buf3), ImGuiInputTextFlags_CallbackEdit, Funcs::MyCallback, (void*)&edit_count);
            HvkGui::SameLine(); HelpMarker(
                "Here we toggle the casing of the first character on every edit + count edits.");
            HvkGui::SameLine(); HvkGui::Text("(%d)", edit_count);

            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Text Input/Resize Callback");
        if (HvkGui::TreeNode("Resize Callback"))
        {
            // To wire InputText() with std::string or any other custom string type,
            // you can use the ImGuiInputTextFlags_CallbackResize flag + create a custom HvkGui::InputText() wrapper
            // using your preferred type. See misc/cpp/imgui_stdlib.h for an implementation of this using std::string.
            HelpMarker(
                "Using ImGuiInputTextFlags_CallbackResize to wire your custom string type to InputText().\n\n"
                "See misc/cpp/imgui_stdlib.h for an implementation of this for std::string.");
            struct Funcs
            {
                static int MyResizeCallback(ImGuiInputTextCallbackData* data)
                {
                    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
                    {
                        ImVector<char>* my_str = (ImVector<char>*)data->UserData;
                        IM_ASSERT(my_str->begin() == data->Buf);
                        my_str->resize(data->BufSize); // NB: On resizing calls, generally data->BufSize == data->BufTextLen + 1
                        data->Buf = my_str->begin();
                    }
                    return 0;
                }

                // Note: Because HvkGui:: is a namespace you would typically add your own function into the namespace.
                // For example, you code may declare a function 'HvkGui::InputText(const char* label, MyString* my_str)'
                static bool MyInputTextMultiline(const char* label, ImVector<char>* my_str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0)
                {
                    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
                    return HvkGui::InputTextMultiline(label, my_str->begin(), (size_t)my_str->size(), size, flags | ImGuiInputTextFlags_CallbackResize, Funcs::MyResizeCallback, (void*)my_str);
                }
            };

            // For this demo we are using ImVector as a string container.
            // Note that because we need to store a terminating zero character, our size/capacity are 1 more
            // than usually reported by a typical string class.
            static ImGuiInputTextFlags flags = ImGuiInputTextFlags_None;
            HvkGui::CheckboxFlags("ImGuiInputTextFlags_WordWrap", &flags, ImGuiInputTextFlags_WordWrap);

            static ImVector<char> my_str;
            if (my_str.empty())
                my_str.push_back(0);
            Funcs::MyInputTextMultiline("##MyStr", &my_str, ImVec2(-FLT_MIN, HvkGui::GetTextLineHeight() * 16), flags);
            HvkGui::Text("Data: %p\nSize: %d\nCapacity: %d", (void*)my_str.begin(), my_str.size(), my_str.capacity());
            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Text Input/Eliding, Alignment");
        if (HvkGui::TreeNode("Eliding, Alignment"))
        {
            static char buf1[128] = "/path/to/some/folder/with/long/filename.cpp";
            static ImGuiInputTextFlags flags = ImGuiInputTextFlags_ElideLeft;
            HvkGui::CheckboxFlags("ImGuiInputTextFlags_ElideLeft", &flags, ImGuiInputTextFlags_ElideLeft);
            HvkGui::InputText("Path", buf1, IM_ARRAYSIZE(buf1), flags);
            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Text Input/Miscellaneous");
        if (HvkGui::TreeNode("Miscellaneous"))
        {
            static char buf1[16];
            static ImGuiInputTextFlags flags = ImGuiInputTextFlags_EscapeClearsAll;
            HvkGui::CheckboxFlags("ImGuiInputTextFlags_EscapeClearsAll", &flags, ImGuiInputTextFlags_EscapeClearsAll);
            HvkGui::CheckboxFlags("ImGuiInputTextFlags_ReadOnly", &flags, ImGuiInputTextFlags_ReadOnly);
            HvkGui::CheckboxFlags("ImGuiInputTextFlags_NoUndoRedo", &flags, ImGuiInputTextFlags_NoUndoRedo);
            HvkGui::InputText("Hello", buf1, IM_ARRAYSIZE(buf1), flags);
            HvkGui::TreePop();
        }

        HvkGui::TreePop();
    }

}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsTooltips()
//-----------------------------------------------------------------------------

static void DemoWindowWidgetsTooltips()
{
    IMGUI_DEMO_MARKER("Widgets/Tooltips");
    if (HvkGui::TreeNode("Tooltips"))
    {
        // Tooltips are windows following the mouse. They do not take focus away.
        HvkGui::SeparatorText("General");

        // Typical use cases:
        // - Short-form (text only):      SetItemTooltip("Hello");
        // - Short-form (any contents):   if (BeginItemTooltip()) { Text("Hello"); EndTooltip(); }

        // - Full-form (text only):       if (IsItemHovered(...)) { SetTooltip("Hello"); }
        // - Full-form (any contents):    if (IsItemHovered(...) && BeginTooltip()) { Text("Hello"); EndTooltip(); }

        HelpMarker(
            "Tooltip are typically created by using a IsItemHovered() + SetTooltip() sequence.\n\n"
            "We provide a helper SetItemTooltip() function to perform the two with standards flags.");

        ImVec2 sz = ImVec2(-FLT_MIN, 0.0f);

        HvkGui::Button("Basic", sz);
        HvkGui::SetItemTooltip("I am a tooltip");

        HvkGui::Button("Fancy", sz);
        if (HvkGui::BeginItemTooltip())
        {
            HvkGui::Text("I am a fancy tooltip");
            static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
            HvkGui::PlotLines("Curve", arr, IM_ARRAYSIZE(arr));
            HvkGui::Text("Sin(time) = %f", sinf((float)HvkGui::GetTime()));
            HvkGui::EndTooltip();
        }

        HvkGui::SeparatorText("Always On");

        // Showcase NOT relying on a IsItemHovered() to emit a tooltip.
        // Here the tooltip is always emitted when 'always_on == true'.
        static int always_on = 0;
        HvkGui::RadioButton("Off", &always_on, 0);
        HvkGui::SameLine();
        HvkGui::RadioButton("Always On (Simple)", &always_on, 1);
        HvkGui::SameLine();
        HvkGui::RadioButton("Always On (Advanced)", &always_on, 2);
        if (always_on == 1)
            HvkGui::SetTooltip("I am following you around.");
        else if (always_on == 2 && HvkGui::BeginTooltip())
        {
            HvkGui::ProgressBar(sinf((float)HvkGui::GetTime()) * 0.5f + 0.5f, ImVec2(HvkGui::GetFontSize() * 25, 0.0f));
            HvkGui::EndTooltip();
        }

        HvkGui::SeparatorText("Custom");

        HelpMarker(
            "Passing ImGuiHoveredFlags_ForTooltip to IsItemHovered() is the preferred way to standardize "
            "tooltip activation details across your application. You may however decide to use custom "
            "flags for a specific tooltip instance.");

        // The following examples are passed for documentation purpose but may not be useful to most users.
        // Passing ImGuiHoveredFlags_ForTooltip to IsItemHovered() will pull ImGuiHoveredFlags flags values from
        // 'style.HoverFlagsForTooltipMouse' or 'style.HoverFlagsForTooltipNav' depending on whether mouse or keyboard/gamepad is being used.
        // With default settings, ImGuiHoveredFlags_ForTooltip is equivalent to ImGuiHoveredFlags_DelayShort + ImGuiHoveredFlags_Stationary.
        HvkGui::Button("Manual", sz);
        if (HvkGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip))
            HvkGui::SetTooltip("I am a manually emitted tooltip.");

        HvkGui::Button("DelayNone", sz);
        if (HvkGui::IsItemHovered(ImGuiHoveredFlags_DelayNone))
            HvkGui::SetTooltip("I am a tooltip with no delay.");

        HvkGui::Button("DelayShort", sz);
        if (HvkGui::IsItemHovered(ImGuiHoveredFlags_DelayShort | ImGuiHoveredFlags_NoSharedDelay))
            HvkGui::SetTooltip("I am a tooltip with a short delay (%0.2f sec).", HvkGui::GetStyle().HoverDelayShort);

        HvkGui::Button("DelayLong", sz);
        if (HvkGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay))
            HvkGui::SetTooltip("I am a tooltip with a long delay (%0.2f sec).", HvkGui::GetStyle().HoverDelayNormal);

        HvkGui::Button("Stationary", sz);
        if (HvkGui::IsItemHovered(ImGuiHoveredFlags_Stationary))
            HvkGui::SetTooltip("I am a tooltip requiring mouse to be stationary before activating.");

        // Using ImGuiHoveredFlags_ForTooltip will pull flags from 'style.HoverFlagsForTooltipMouse' or 'style.HoverFlagsForTooltipNav',
        // which default value include the ImGuiHoveredFlags_AllowWhenDisabled flag.
        HvkGui::BeginDisabled();
        HvkGui::Button("Disabled item", sz);
        if (HvkGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip))
            HvkGui::SetTooltip("I am a a tooltip for a disabled item.");
        HvkGui::EndDisabled();

        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsTreeNodes()
//-----------------------------------------------------------------------------

static void DemoWindowWidgetsTreeNodes()
{
    IMGUI_DEMO_MARKER("Widgets/Tree Nodes");
    if (HvkGui::TreeNode("Tree Nodes"))
    {
        // See see "Examples -> Property Editor" (ShowExampleAppPropertyEditor() function) for a fancier, data-driven tree.
        IMGUI_DEMO_MARKER("Widgets/Tree Nodes/Basic trees");
        if (HvkGui::TreeNode("Basic trees"))
        {
            for (int i = 0; i < 5; i++)
            {
                // Use SetNextItemOpen() so set the default state of a node to be open. We could
                // also use TreeNodeEx() with the ImGuiTreeNodeFlags_DefaultOpen flag to achieve the same thing!
                if (i == 0)
                    HvkGui::SetNextItemOpen(true, ImGuiCond_Once);

                // Here we use PushID() to generate a unique base ID, and then the "" used as TreeNode id won't conflict.
                // An alternative to using 'PushID() + TreeNode("", ...)' to generate a unique ID is to use 'TreeNode((void*)(intptr_t)i, ...)',
                // aka generate a dummy pointer-sized value to be hashed. The demo below uses that technique. Both are fine.
                HvkGui::PushID(i);
                if (HvkGui::TreeNode("", "Child %d", i))
                {
                    HvkGui::Text("blah blah");
                    HvkGui::SameLine();
                    if (HvkGui::SmallButton("button")) {}
                    HvkGui::TreePop();
                }
                HvkGui::PopID();
            }
            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Tree Nodes/Hierarchy lines");
        if (HvkGui::TreeNode("Hierarchy lines"))
        {
            static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_DefaultOpen;
            HelpMarker("Default option for DrawLinesXXX is stored in style.TreeLinesFlags");
            HvkGui::CheckboxFlags("ImGuiTreeNodeFlags_DrawLinesNone", &base_flags, ImGuiTreeNodeFlags_DrawLinesNone);
            HvkGui::CheckboxFlags("ImGuiTreeNodeFlags_DrawLinesFull", &base_flags, ImGuiTreeNodeFlags_DrawLinesFull);
            HvkGui::CheckboxFlags("ImGuiTreeNodeFlags_DrawLinesToNodes", &base_flags, ImGuiTreeNodeFlags_DrawLinesToNodes);

            if (HvkGui::TreeNodeEx("Parent", base_flags))
            {
                if (HvkGui::TreeNodeEx("Child 1", base_flags))
                {
                    HvkGui::Button("Button for Child 1");
                    HvkGui::TreePop();
                }
                if (HvkGui::TreeNodeEx("Child 2", base_flags))
                {
                    HvkGui::Button("Button for Child 2");
                    HvkGui::TreePop();
                }
                HvkGui::Text("Remaining contents");
                HvkGui::Text("Remaining contents");
                HvkGui::TreePop();
            }

            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Widgets/Tree Nodes/Advanced, with Selectable nodes");
        if (HvkGui::TreeNode("Advanced, with Selectable nodes"))
        {
            HelpMarker(
                "This is a more typical looking tree with selectable nodes.\n"
                "Click to select, Ctrl+Click to toggle, click on arrows or double-click to open.");
            static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
            static bool align_label_with_current_x_position = false;
            static bool test_drag_and_drop = false;
            HvkGui::CheckboxFlags("ImGuiTreeNodeFlags_OpenOnArrow", &base_flags, ImGuiTreeNodeFlags_OpenOnArrow);
            HvkGui::CheckboxFlags("ImGuiTreeNodeFlags_OpenOnDoubleClick", &base_flags, ImGuiTreeNodeFlags_OpenOnDoubleClick);
            HvkGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanAvailWidth", &base_flags, ImGuiTreeNodeFlags_SpanAvailWidth); HvkGui::SameLine(); HelpMarker("Extend hit area to all available width instead of allowing more items to be laid out after the node.");
            HvkGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanFullWidth", &base_flags, ImGuiTreeNodeFlags_SpanFullWidth);
            HvkGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanLabelWidth", &base_flags, ImGuiTreeNodeFlags_SpanLabelWidth); HvkGui::SameLine(); HelpMarker("Reduce hit area to the text label and a bit of margin.");
            HvkGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanAllColumns", &base_flags, ImGuiTreeNodeFlags_SpanAllColumns); HvkGui::SameLine(); HelpMarker("For use in Tables only.");
            HvkGui::CheckboxFlags("ImGuiTreeNodeFlags_AllowOverlap", &base_flags, ImGuiTreeNodeFlags_AllowOverlap);
            HvkGui::CheckboxFlags("ImGuiTreeNodeFlags_Framed", &base_flags, ImGuiTreeNodeFlags_Framed); HvkGui::SameLine(); HelpMarker("Draw frame with background (e.g. for CollapsingHeader)");
            HvkGui::CheckboxFlags("ImGuiTreeNodeFlags_FramePadding", &base_flags, ImGuiTreeNodeFlags_FramePadding);
            HvkGui::CheckboxFlags("ImGuiTreeNodeFlags_NavLeftJumpsToParent", &base_flags, ImGuiTreeNodeFlags_NavLeftJumpsToParent);

            HelpMarker("Default option for DrawLinesXXX is stored in style.TreeLinesFlags");
            HvkGui::CheckboxFlags("ImGuiTreeNodeFlags_DrawLinesNone", &base_flags, ImGuiTreeNodeFlags_DrawLinesNone);
            HvkGui::CheckboxFlags("ImGuiTreeNodeFlags_DrawLinesFull", &base_flags, ImGuiTreeNodeFlags_DrawLinesFull);
            HvkGui::CheckboxFlags("ImGuiTreeNodeFlags_DrawLinesToNodes", &base_flags, ImGuiTreeNodeFlags_DrawLinesToNodes);

            HvkGui::Checkbox("Align label with current X position", &align_label_with_current_x_position);
            HvkGui::Checkbox("Test tree node as drag source", &test_drag_and_drop);
            HvkGui::Text("Hello!");
            if (align_label_with_current_x_position)
                HvkGui::Unindent(HvkGui::GetTreeNodeToLabelSpacing());

            // 'selection_mask' is dumb representation of what may be user-side selection state.
            //  You may retain selection state inside or outside your objects in whatever format you see fit.
            // 'node_clicked' is temporary storage of what node we have clicked to process selection at the end
            /// of the loop. May be a pointer to your own node type, etc.
            static int selection_mask = (1 << 2);
            int node_clicked = -1;
            for (int i = 0; i < 6; i++)
            {
                // Disable the default "open on single-click behavior" + set Selected flag according to our selection.
                // To alter selection we use IsItemClicked() && !IsItemToggledOpen(), so clicking on an arrow doesn't alter selection.
                ImGuiTreeNodeFlags node_flags = base_flags;
                const bool is_selected = (selection_mask & (1 << i)) != 0;
                if (is_selected)
                    node_flags |= ImGuiTreeNodeFlags_Selected;
                if (i < 3)
                {
                    // Items 0..2 are Tree Node
                    bool node_open = HvkGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
                    if (HvkGui::IsItemClicked() && !HvkGui::IsItemToggledOpen())
                        node_clicked = i;
                    if (test_drag_and_drop && HvkGui::BeginDragDropSource())
                    {
                        HvkGui::SetDragDropPayload("_TREENODE", NULL, 0);
                        HvkGui::Text("This is a drag and drop source");
                        HvkGui::EndDragDropSource();
                    }
                    if (i == 2 && (base_flags & ImGuiTreeNodeFlags_SpanLabelWidth))
                    {
                        // Item 2 has an additional inline button to help demonstrate SpanLabelWidth.
                        HvkGui::SameLine();
                        if (HvkGui::SmallButton("button")) {}
                    }
                    if (node_open)
                    {
                        HvkGui::BulletText("Blah blah\nBlah Blah");
                        HvkGui::SameLine();
                        HvkGui::SmallButton("Button");
                        HvkGui::TreePop();
                    }
                }
                else
                {
                    // Items 3..5 are Tree Leaves
                    // The only reason we use TreeNode at all is to allow selection of the leaf. Otherwise we can
                    // use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
                    node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
                    HvkGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Leaf %d", i);
                    if (HvkGui::IsItemClicked() && !HvkGui::IsItemToggledOpen())
                        node_clicked = i;
                    if (test_drag_and_drop && HvkGui::BeginDragDropSource())
                    {
                        HvkGui::SetDragDropPayload("_TREENODE", NULL, 0);
                        HvkGui::Text("This is a drag and drop source");
                        HvkGui::EndDragDropSource();
                    }
                }
            }
            if (node_clicked != -1)
            {
                // Update selection state
                // (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
                if (HvkGui::GetIO().KeyCtrl)
                    selection_mask ^= (1 << node_clicked);          // Ctrl+Click to toggle
                else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
                    selection_mask = (1 << node_clicked);           // Click to single-select
            }
            if (align_label_with_current_x_position)
                HvkGui::Indent(HvkGui::GetTreeNodeToLabelSpacing());
            HvkGui::TreePop();
        }
        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgetsVerticalSliders()
//-----------------------------------------------------------------------------

static void DemoWindowWidgetsVerticalSliders()
{
    IMGUI_DEMO_MARKER("Widgets/Vertical Sliders");
    if (HvkGui::TreeNode("Vertical Sliders"))
    {
        const float spacing = 4;
        HvkGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, spacing));

        static int int_value = 0;
        HvkGui::VSliderInt("##int", ImVec2(18, 160), &int_value, 0, 5);
        HvkGui::SameLine();

        static float values[7] = { 0.0f, 0.60f, 0.35f, 0.9f, 0.70f, 0.20f, 0.0f };
        HvkGui::PushID("set1");
        for (int i = 0; i < 7; i++)
        {
            if (i > 0) HvkGui::SameLine();
            HvkGui::PushID(i);
            HvkGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(i / 7.0f, 0.5f, 0.5f));
            HvkGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.5f));
            HvkGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.5f));
            HvkGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(i / 7.0f, 0.9f, 0.9f));
            HvkGui::VSliderFloat("##v", ImVec2(18, 160), &values[i], 0.0f, 1.0f, "");
            if (HvkGui::IsItemActive() || HvkGui::IsItemHovered())
                HvkGui::SetTooltip("%.3f", values[i]);
            HvkGui::PopStyleColor(4);
            HvkGui::PopID();
        }
        HvkGui::PopID();

        HvkGui::SameLine();
        HvkGui::PushID("set2");
        static float values2[4] = { 0.20f, 0.80f, 0.40f, 0.25f };
        const int rows = 3;
        const ImVec2 small_slider_size(18, (float)(int)((160.0f - (rows - 1) * spacing) / rows));
        for (int nx = 0; nx < 4; nx++)
        {
            if (nx > 0) HvkGui::SameLine();
            HvkGui::BeginGroup();
            for (int ny = 0; ny < rows; ny++)
            {
                HvkGui::PushID(nx * rows + ny);
                HvkGui::VSliderFloat("##v", small_slider_size, &values2[nx], 0.0f, 1.0f, "");
                if (HvkGui::IsItemActive() || HvkGui::IsItemHovered())
                    HvkGui::SetTooltip("%.3f", values2[nx]);
                HvkGui::PopID();
            }
            HvkGui::EndGroup();
        }
        HvkGui::PopID();

        HvkGui::SameLine();
        HvkGui::PushID("set3");
        for (int i = 0; i < 4; i++)
        {
            if (i > 0) HvkGui::SameLine();
            HvkGui::PushID(i);
            HvkGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 40);
            HvkGui::VSliderFloat("##v", ImVec2(40, 160), &values[i], 0.0f, 1.0f, "%.2f\nsec");
            HvkGui::PopStyleVar();
            HvkGui::PopID();
        }
        HvkGui::PopID();
        HvkGui::PopStyleVar();
        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowWidgets()
//-----------------------------------------------------------------------------

static void DemoWindowWidgets(ImGuiDemoWindowData* demo_data)
{
    IMGUI_DEMO_MARKER("Widgets");
    //HvkGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (!HvkGui::CollapsingHeader("Widgets"))
        return;

    const bool disable_all = demo_data->DisableSections; // The Checkbox for that is inside the "Disabled" section at the bottom
    if (disable_all)
        HvkGui::BeginDisabled();

    DemoWindowWidgetsBasic();
    DemoWindowWidgetsBullets();
    DemoWindowWidgetsCollapsingHeaders();
    DemoWindowWidgetsComboBoxes();
    DemoWindowWidgetsColorAndPickers();
    DemoWindowWidgetsDataTypes();

    if (disable_all)
        HvkGui::EndDisabled();
    DemoWindowWidgetsDisableBlocks(demo_data);
    if (disable_all)
        HvkGui::BeginDisabled();

    DemoWindowWidgetsDragAndDrop();
    DemoWindowWidgetsDragsAndSliders();
    DemoWindowWidgetsFonts();
    DemoWindowWidgetsImages();
    DemoWindowWidgetsListBoxes();
    DemoWindowWidgetsMultiComponents();
    DemoWindowWidgetsPlotting();
    DemoWindowWidgetsProgressBars();
    DemoWindowWidgetsQueryingStatuses();
    DemoWindowWidgetsSelectables();
    DemoWindowWidgetsSelectionAndMultiSelect(demo_data);
    DemoWindowWidgetsTabs();
    DemoWindowWidgetsText();
    DemoWindowWidgetsTextFilter();
    DemoWindowWidgetsTextInput();
    DemoWindowWidgetsTooltips();
    DemoWindowWidgetsTreeNodes();
    DemoWindowWidgetsVerticalSliders();

    if (disable_all)
        HvkGui::EndDisabled();
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowLayout()
//-----------------------------------------------------------------------------

static void DemoWindowLayout()
{
    IMGUI_DEMO_MARKER("Layout");
    if (!HvkGui::CollapsingHeader("Layout & Scrolling"))
        return;

    IMGUI_DEMO_MARKER("Layout/Child windows");
    if (HvkGui::TreeNode("Child windows"))
    {
        HvkGui::SeparatorText("Child windows");

        HelpMarker("Use child windows to begin into a self-contained independent scrolling/clipping regions within a host window.");
        static bool disable_mouse_wheel = false;
        static bool disable_menu = false;
        HvkGui::Checkbox("Disable Mouse Wheel", &disable_mouse_wheel);
        HvkGui::Checkbox("Disable Menu", &disable_menu);

        // Child 1: no border, enable horizontal scrollbar
        {
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
            if (disable_mouse_wheel)
                window_flags |= ImGuiWindowFlags_NoScrollWithMouse;
            HvkGui::BeginChild("ChildL", ImVec2(HvkGui::GetContentRegionAvail().x * 0.5f, 260), ImGuiChildFlags_None, window_flags);
            for (int i = 0; i < 100; i++)
                HvkGui::Text("%04d: scrollable region", i);
            HvkGui::EndChild();
        }

        HvkGui::SameLine();

        // Child 2: rounded border
        {
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
            if (disable_mouse_wheel)
                window_flags |= ImGuiWindowFlags_NoScrollWithMouse;
            if (!disable_menu)
                window_flags |= ImGuiWindowFlags_MenuBar;
            HvkGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
            HvkGui::BeginChild("ChildR", ImVec2(0, 260), ImGuiChildFlags_Borders, window_flags);
            if (!disable_menu && HvkGui::BeginMenuBar())
            {
                if (HvkGui::BeginMenu("Menu"))
                {
                    ShowExampleMenuFile();
                    HvkGui::EndMenu();
                }
                HvkGui::EndMenuBar();
            }
            if (HvkGui::BeginTable("split", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
            {
                for (int i = 0; i < 100; i++)
                {
                    char buf[32];
                    sprintf(buf, "%03d", i);
                    HvkGui::TableNextColumn();
                    HvkGui::Button(buf, ImVec2(-FLT_MIN, 0.0f));
                }
                HvkGui::EndTable();
            }
            HvkGui::EndChild();
            HvkGui::PopStyleVar();
        }

        // Child 3: manual-resize
        HvkGui::SeparatorText("Manual-resize");
        {
            HelpMarker("Drag bottom border to resize. Double-click bottom border to auto-fit to vertical contents.");
            //if (HvkGui::Button("Set Height to 200"))
            //    HvkGui::SetNextWindowSize(ImVec2(-FLT_MIN, 200.0f));

            HvkGui::PushStyleColor(ImGuiCol_ChildBg, HvkGui::GetStyleColorVec4(ImGuiCol_FrameBg));
            if (HvkGui::BeginChild("ResizableChild", ImVec2(-FLT_MIN, HvkGui::GetTextLineHeightWithSpacing() * 8), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY))
                for (int n = 0; n < 10; n++)
                    HvkGui::Text("Line %04d", n);
            HvkGui::PopStyleColor();
            HvkGui::EndChild();
        }

        // Child 4: auto-resizing height with a limit
        HvkGui::SeparatorText("Auto-resize with constraints");
        {
            static int draw_lines = 3;
            static int max_height_in_lines = 10;
            HvkGui::SetNextItemWidth(HvkGui::GetFontSize() * 8);
            HvkGui::DragInt("Lines Count", &draw_lines, 0.2f);
            HvkGui::SetNextItemWidth(HvkGui::GetFontSize() * 8);
            HvkGui::DragInt("Max Height (in Lines)", &max_height_in_lines, 0.2f);

            HvkGui::SetNextWindowSizeConstraints(ImVec2(0.0f, HvkGui::GetTextLineHeightWithSpacing() * 1), ImVec2(FLT_MAX, HvkGui::GetTextLineHeightWithSpacing() * max_height_in_lines));
            if (HvkGui::BeginChild("ConstrainedChild", ImVec2(-FLT_MIN, 0.0f), ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY))
                for (int n = 0; n < draw_lines; n++)
                    HvkGui::Text("Line %04d", n);
            HvkGui::EndChild();
        }

        HvkGui::SeparatorText("Misc/Advanced");

        // Demonstrate a few extra things
        // - Changing ImGuiCol_ChildBg (which is transparent black in default styles)
        // - Using SetCursorPos() to position child window (the child window is an item from the POV of parent window)
        //   You can also call SetNextWindowPos() to position the child window. The parent window will effectively
        //   layout from this position.
        // - Using HvkGui::GetItemRectMin/Max() to query the "item" state (because the child window is an item from
        //   the POV of the parent window). See 'Demo->Querying Status (Edited/Active/Hovered etc.)' for details.
        {
            static int offset_x = 0;
            static bool override_bg_color = true;
            static ImGuiChildFlags child_flags = ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX | ImGuiChildFlags_ResizeY;
            HvkGui::SetNextItemWidth(HvkGui::GetFontSize() * 8);
            HvkGui::DragInt("Offset X", &offset_x, 1.0f, -1000, 1000);
            HvkGui::Checkbox("Override ChildBg color", &override_bg_color);
            HvkGui::CheckboxFlags("ImGuiChildFlags_Borders", &child_flags, ImGuiChildFlags_Borders);
            HvkGui::CheckboxFlags("ImGuiChildFlags_AlwaysUseWindowPadding", &child_flags, ImGuiChildFlags_AlwaysUseWindowPadding);
            HvkGui::CheckboxFlags("ImGuiChildFlags_ResizeX", &child_flags, ImGuiChildFlags_ResizeX);
            HvkGui::CheckboxFlags("ImGuiChildFlags_ResizeY", &child_flags, ImGuiChildFlags_ResizeY);
            HvkGui::CheckboxFlags("ImGuiChildFlags_FrameStyle", &child_flags, ImGuiChildFlags_FrameStyle);
            HvkGui::SameLine(); HelpMarker("Style the child window like a framed item: use FrameBg, FrameRounding, FrameBorderSize, FramePadding instead of ChildBg, ChildRounding, ChildBorderSize, WindowPadding.");
            if (child_flags & ImGuiChildFlags_FrameStyle)
                override_bg_color = false;

            HvkGui::SetCursorPosX(HvkGui::GetCursorPosX() + (float)offset_x);
            if (override_bg_color)
                HvkGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(255, 0, 0, 100));
            HvkGui::BeginChild("Red", ImVec2(200, 100), child_flags, ImGuiWindowFlags_None);
            if (override_bg_color)
                HvkGui::PopStyleColor();

            for (int n = 0; n < 50; n++)
                HvkGui::Text("Some test %d", n);
            HvkGui::EndChild();
            bool child_is_hovered = HvkGui::IsItemHovered();
            ImVec2 child_rect_min = HvkGui::GetItemRectMin();
            ImVec2 child_rect_max = HvkGui::GetItemRectMax();
            HvkGui::Text("Hovered: %d", child_is_hovered);
            HvkGui::Text("Rect of child window is: (%.0f,%.0f) (%.0f,%.0f)", child_rect_min.x, child_rect_min.y, child_rect_max.x, child_rect_max.y);
        }

        HvkGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Layout/Widgets Width");
    if (HvkGui::TreeNode("Widgets Width"))
    {
        static float f = 0.0f;
        static bool show_indented_items = true;
        HvkGui::Checkbox("Show indented items", &show_indented_items);

        // Use SetNextItemWidth() to set the width of a single upcoming item.
        // Use PushItemWidth()/PopItemWidth() to set the width of a group of items.
        // In real code use you'll probably want to choose width values that are proportional to your font size
        // e.g. Using '20.0f * GetFontSize()' as width instead of '200.0f', etc.

        HvkGui::Text("SetNextItemWidth/PushItemWidth(100)");
        HvkGui::SameLine(); HelpMarker("Fixed width.");
        HvkGui::PushItemWidth(100);
        HvkGui::DragFloat("float##1b", &f);
        if (show_indented_items)
        {
            HvkGui::Indent();
            HvkGui::DragFloat("float (indented)##1b", &f);
            HvkGui::Unindent();
        }
        HvkGui::PopItemWidth();

        HvkGui::Text("SetNextItemWidth/PushItemWidth(-100)");
        HvkGui::SameLine(); HelpMarker("Align to right edge minus 100");
        HvkGui::PushItemWidth(-100);
        HvkGui::DragFloat("float##2a", &f);
        if (show_indented_items)
        {
            HvkGui::Indent();
            HvkGui::DragFloat("float (indented)##2b", &f);
            HvkGui::Unindent();
        }
        HvkGui::PopItemWidth();

        HvkGui::Text("SetNextItemWidth/PushItemWidth(GetContentRegionAvail().x * 0.5f)");
        HvkGui::SameLine(); HelpMarker("Half of available width.\n(~ right-cursor_pos)\n(works within a column set)");
        HvkGui::PushItemWidth(HvkGui::GetContentRegionAvail().x * 0.5f);
        HvkGui::DragFloat("float##3a", &f);
        if (show_indented_items)
        {
            HvkGui::Indent();
            HvkGui::DragFloat("float (indented)##3b", &f);
            HvkGui::Unindent();
        }
        HvkGui::PopItemWidth();

        HvkGui::Text("SetNextItemWidth/PushItemWidth(-GetContentRegionAvail().x * 0.5f)");
        HvkGui::SameLine(); HelpMarker("Align to right edge minus half");
        HvkGui::PushItemWidth(-HvkGui::GetContentRegionAvail().x * 0.5f);
        HvkGui::DragFloat("float##4a", &f);
        if (show_indented_items)
        {
            HvkGui::Indent();
            HvkGui::DragFloat("float (indented)##4b", &f);
            HvkGui::Unindent();
        }
        HvkGui::PopItemWidth();

        HvkGui::Text("SetNextItemWidth/PushItemWidth(-Min(GetContentRegionAvail().x * 0.40f, GetFontSize() * 12))");
        HvkGui::PushItemWidth(-IM_MIN(HvkGui::GetFontSize() * 12, HvkGui::GetContentRegionAvail().x * 0.40f));
        HvkGui::DragFloat("float##5a", &f);
        if (show_indented_items)
        {
            HvkGui::Indent();
            HvkGui::DragFloat("float (indented)##5b", &f);
            HvkGui::Unindent();
        }
        HvkGui::PopItemWidth();

        // Demonstrate using PushItemWidth to surround three items.
        // Calling SetNextItemWidth() before each of them would have the same effect.
        HvkGui::Text("SetNextItemWidth/PushItemWidth(-FLT_MIN)");
        HvkGui::SameLine(); HelpMarker("Align to right edge");
        HvkGui::PushItemWidth(-FLT_MIN);
        HvkGui::DragFloat("##float6a", &f);
        if (show_indented_items)
        {
            HvkGui::Indent();
            HvkGui::DragFloat("float (indented)##6b", &f);
            HvkGui::Unindent();
        }
        HvkGui::PopItemWidth();

        HvkGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Layout/Basic Horizontal Layout");
    if (HvkGui::TreeNode("Basic Horizontal Layout"))
    {
        HvkGui::TextWrapped("(Use HvkGui::SameLine() to keep adding items to the right of the preceding item)");

        // Text
        IMGUI_DEMO_MARKER("Layout/Basic Horizontal Layout/SameLine");
        HvkGui::Text("Two items: Hello"); HvkGui::SameLine();
        HvkGui::TextColored(ImVec4(1, 1, 0, 1), "Sailor");

        // Adjust spacing
        HvkGui::Text("More spacing: Hello"); HvkGui::SameLine(0, 20);
        HvkGui::TextColored(ImVec4(1, 1, 0, 1), "Sailor");

        // Button
        HvkGui::AlignTextToFramePadding();
        HvkGui::Text("Normal buttons"); HvkGui::SameLine();
        HvkGui::Button("Banana"); HvkGui::SameLine();
        HvkGui::Button("Apple"); HvkGui::SameLine();
        HvkGui::Button("Corniflower");

        // Button
        HvkGui::Text("Small buttons"); HvkGui::SameLine();
        HvkGui::SmallButton("Like this one"); HvkGui::SameLine();
        HvkGui::Text("can fit within a text block.");

        // Aligned to arbitrary position. Easy/cheap column.
        IMGUI_DEMO_MARKER("Layout/Basic Horizontal Layout/SameLine (with offset)");
        HvkGui::Text("Aligned");
        HvkGui::SameLine(150); HvkGui::Text("x=150");
        HvkGui::SameLine(300); HvkGui::Text("x=300");
        HvkGui::Text("Aligned");
        HvkGui::SameLine(150); HvkGui::SmallButton("x=150");
        HvkGui::SameLine(300); HvkGui::SmallButton("x=300");

        // Checkbox
        IMGUI_DEMO_MARKER("Layout/Basic Horizontal Layout/SameLine (more)");
        static bool c1 = false, c2 = false, c3 = false, c4 = false;
        HvkGui::Checkbox("My", &c1); HvkGui::SameLine();
        HvkGui::Checkbox("Tailor", &c2); HvkGui::SameLine();
        HvkGui::Checkbox("Is", &c3); HvkGui::SameLine();
        HvkGui::Checkbox("Rich", &c4);

        // Various
        static float f0 = 1.0f, f1 = 2.0f, f2 = 3.0f;
        HvkGui::PushItemWidth(HvkGui::CalcTextSize("AAAAAAA").x);
        const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD" };
        static int item = -1;
        HvkGui::Combo("Combo", &item, items, IM_ARRAYSIZE(items)); HvkGui::SameLine();
        HvkGui::SliderFloat("X", &f0, 0.0f, 5.0f); HvkGui::SameLine();
        HvkGui::SliderFloat("Y", &f1, 0.0f, 5.0f); HvkGui::SameLine();
        HvkGui::SliderFloat("Z", &f2, 0.0f, 5.0f);

        HvkGui::Text("Lists:");
        static int selection[4] = { 0, 1, 2, 3 };
        for (int i = 0; i < 4; i++)
        {
            if (i > 0) HvkGui::SameLine();
            HvkGui::PushID(i);
            HvkGui::ListBox("", &selection[i], items, IM_ARRAYSIZE(items));
            HvkGui::PopID();
            //HvkGui::SetItemTooltip("ListBox %d hovered", i);
        }
        HvkGui::PopItemWidth();

        // Dummy
        IMGUI_DEMO_MARKER("Layout/Basic Horizontal Layout/Dummy");
        ImVec2 button_sz(40, 40);
        HvkGui::Button("A", button_sz); HvkGui::SameLine();
        HvkGui::Dummy(button_sz); HvkGui::SameLine();
        HvkGui::Button("B", button_sz);

        // Manually wrapping
        // (we should eventually provide this as an automatic layout feature, but for now you can do it manually)
        IMGUI_DEMO_MARKER("Layout/Basic Horizontal Layout/Manual wrapping");
        HvkGui::Text("Manual wrapping:");
        ImGuiStyle& style = HvkGui::GetStyle();
        int buttons_count = 20;
        float window_visible_x2 = HvkGui::GetCursorScreenPos().x + HvkGui::GetContentRegionAvail().x;
        for (int n = 0; n < buttons_count; n++)
        {
            HvkGui::PushID(n);
            HvkGui::Button("Box", button_sz);
            float last_button_x2 = HvkGui::GetItemRectMax().x;
            float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
            if (n + 1 < buttons_count && next_button_x2 < window_visible_x2)
                HvkGui::SameLine();
            HvkGui::PopID();
        }

        HvkGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Layout/Groups");
    if (HvkGui::TreeNode("Groups"))
    {
        HelpMarker(
            "BeginGroup() basically locks the horizontal position for new line. "
            "EndGroup() bundles the whole group so that you can use \"item\" functions such as "
            "IsItemHovered()/IsItemActive() or SameLine() etc. on the whole group.");
        HvkGui::BeginGroup();
        {
            HvkGui::BeginGroup();
            HvkGui::Button("AAA");
            HvkGui::SameLine();
            HvkGui::Button("BBB");
            HvkGui::SameLine();
            HvkGui::BeginGroup();
            HvkGui::Button("CCC");
            HvkGui::Button("DDD");
            HvkGui::EndGroup();
            HvkGui::SameLine();
            HvkGui::Button("EEE");
            HvkGui::EndGroup();
            HvkGui::SetItemTooltip("First group hovered");
        }
        // Capture the group size and create widgets using the same size
        ImVec2 size = HvkGui::GetItemRectSize();
        const float values[5] = { 0.5f, 0.20f, 0.80f, 0.60f, 0.25f };
        HvkGui::PlotHistogram("##values", values, IM_ARRAYSIZE(values), 0, NULL, 0.0f, 1.0f, size);

        HvkGui::Button("ACTION", ImVec2((size.x - HvkGui::GetStyle().ItemSpacing.x) * 0.5f, size.y));
        HvkGui::SameLine();
        HvkGui::Button("REACTION", ImVec2((size.x - HvkGui::GetStyle().ItemSpacing.x) * 0.5f, size.y));
        HvkGui::EndGroup();
        HvkGui::SameLine();

        HvkGui::Button("LEVERAGE\nBUZZWORD", size);
        HvkGui::SameLine();

        if (HvkGui::BeginListBox("List", size))
        {
            HvkGui::Selectable("Selected", true);
            HvkGui::Selectable("Not Selected", false);
            HvkGui::EndListBox();
        }

        HvkGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Layout/Text Baseline Alignment");
    if (HvkGui::TreeNode("Text Baseline Alignment"))
    {
        {
            HvkGui::BulletText("Text baseline:");
            HvkGui::SameLine(); HelpMarker(
                "This is testing the vertical alignment that gets applied on text to keep it aligned with widgets. "
                "Lines only composed of text or \"small\" widgets use less vertical space than lines with framed widgets.");
            HvkGui::Indent();

            HvkGui::Text("KO Blahblah"); HvkGui::SameLine();
            HvkGui::Button("Some framed item"); HvkGui::SameLine();
            HelpMarker("Baseline of button will look misaligned with text..");

            // If your line starts with text, call AlignTextToFramePadding() to align text to upcoming widgets.
            // (because we don't know what's coming after the Text() statement, we need to move the text baseline
            // down by FramePadding.y ahead of time)
            HvkGui::AlignTextToFramePadding();
            HvkGui::Text("OK Blahblah"); HvkGui::SameLine();
            HvkGui::Button("Some framed item##2"); HvkGui::SameLine();
            HelpMarker("We call AlignTextToFramePadding() to vertically align the text baseline by +FramePadding.y");

            // SmallButton() uses the same vertical padding as Text
            HvkGui::Button("TEST##1"); HvkGui::SameLine();
            HvkGui::Text("TEST"); HvkGui::SameLine();
            HvkGui::SmallButton("TEST##2");

            // If your line starts with text, call AlignTextToFramePadding() to align text to upcoming widgets.
            HvkGui::AlignTextToFramePadding();
            HvkGui::Text("Text aligned to framed item"); HvkGui::SameLine();
            HvkGui::Button("Item##1"); HvkGui::SameLine();
            HvkGui::Text("Item"); HvkGui::SameLine();
            HvkGui::SmallButton("Item##2"); HvkGui::SameLine();
            HvkGui::Button("Item##3");

            HvkGui::Unindent();
        }

        HvkGui::Spacing();

        {
            HvkGui::BulletText("Multi-line text:");
            HvkGui::Indent();
            HvkGui::Text("One\nTwo\nThree"); HvkGui::SameLine();
            HvkGui::Text("Hello\nWorld"); HvkGui::SameLine();
            HvkGui::Text("Banana");

            HvkGui::Text("Banana"); HvkGui::SameLine();
            HvkGui::Text("Hello\nWorld"); HvkGui::SameLine();
            HvkGui::Text("One\nTwo\nThree");

            HvkGui::Button("HOP##1"); HvkGui::SameLine();
            HvkGui::Text("Banana"); HvkGui::SameLine();
            HvkGui::Text("Hello\nWorld"); HvkGui::SameLine();
            HvkGui::Text("Banana");

            HvkGui::Button("HOP##2"); HvkGui::SameLine();
            HvkGui::Text("Hello\nWorld"); HvkGui::SameLine();
            HvkGui::Text("Banana");
            HvkGui::Unindent();
        }

        HvkGui::Spacing();

        {
            HvkGui::BulletText("Misc items:");
            HvkGui::Indent();

            // SmallButton() sets FramePadding to zero. Text baseline is aligned to match baseline of previous Button.
            HvkGui::Button("80x80", ImVec2(80, 80));
            HvkGui::SameLine();
            HvkGui::Button("50x50", ImVec2(50, 50));
            HvkGui::SameLine();
            HvkGui::Button("Button()");
            HvkGui::SameLine();
            HvkGui::SmallButton("SmallButton()");

            // Tree
            // (here the node appears after a button and has odd intent, so we use ImGuiTreeNodeFlags_DrawLinesNone to disable hierarchy outline)
            const float spacing = HvkGui::GetStyle().ItemInnerSpacing.x;
            HvkGui::Button("Button##1"); // Will make line higher
            HvkGui::SameLine(0.0f, spacing);
            if (HvkGui::TreeNodeEx("Node##1", ImGuiTreeNodeFlags_DrawLinesNone))
            {
                // Placeholder tree data
                for (int i = 0; i < 6; i++)
                    HvkGui::BulletText("Item %d..", i);
                HvkGui::TreePop();
            }

            const float padding = (float)(int)(HvkGui::GetFontSize() * 1.20f); // Large padding
            HvkGui::PushStyleVarY(ImGuiStyleVar_FramePadding, padding);
            HvkGui::Button("Button##2");
            HvkGui::PopStyleVar();
            HvkGui::SameLine(0.0f, spacing);
            if (HvkGui::TreeNodeEx("Node##2", ImGuiTreeNodeFlags_DrawLinesNone))
                HvkGui::TreePop();

            // Vertically align text node a bit lower so it'll be vertically centered with upcoming widget.
            // Otherwise you can use SmallButton() (smaller fit).
            HvkGui::AlignTextToFramePadding();

            // Common mistake to avoid: if we want to SameLine after TreeNode we need to do it before we add
            // other contents "inside" the node.
            bool node_open = HvkGui::TreeNode("Node##3");
            HvkGui::SameLine(0.0f, spacing); HvkGui::Button("Button##3");
            if (node_open)
            {
                // Placeholder tree data
                for (int i = 0; i < 6; i++)
                    HvkGui::BulletText("Item %d..", i);
                HvkGui::TreePop();
            }

            // Bullet
            HvkGui::Button("Button##4");
            HvkGui::SameLine(0.0f, spacing);
            HvkGui::BulletText("Bullet text");

            HvkGui::AlignTextToFramePadding();
            HvkGui::BulletText("Node");
            HvkGui::SameLine(0.0f, spacing); HvkGui::Button("Button##5");
            HvkGui::Unindent();
        }

        HvkGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Layout/Scrolling");
    if (HvkGui::TreeNode("Scrolling"))
    {
        // Vertical scroll functions
        IMGUI_DEMO_MARKER("Layout/Scrolling/Vertical");
        HelpMarker("Use SetScrollHereY() or SetScrollFromPosY() to scroll to a given vertical position.");

        static int track_item = 50;
        static bool enable_track = true;
        static bool enable_extra_decorations = false;
        static float scroll_to_off_px = 0.0f;
        static float scroll_to_pos_px = 200.0f;

        HvkGui::Checkbox("Decoration", &enable_extra_decorations);

        HvkGui::PushItemWidth(HvkGui::GetFontSize() * 10);
        enable_track |= HvkGui::DragInt("##item", &track_item, 0.25f, 0, 99, "Item = %d");
        HvkGui::SameLine();
        HvkGui::Checkbox("Track", &enable_track);

        bool scroll_to_off = HvkGui::DragFloat("##off", &scroll_to_off_px, 1.00f, 0, FLT_MAX, "+%.0f px");
        HvkGui::SameLine();
        scroll_to_off |= HvkGui::Button("Scroll Offset");

        bool scroll_to_pos = HvkGui::DragFloat("##pos", &scroll_to_pos_px, 1.00f, -10, FLT_MAX, "X/Y = %.0f px");
        HvkGui::SameLine();
        scroll_to_pos |= HvkGui::Button("Scroll To Pos");
        HvkGui::PopItemWidth();

        if (scroll_to_off || scroll_to_pos)
            enable_track = false;

        ImGuiStyle& style = HvkGui::GetStyle();
        float child_w = (HvkGui::GetContentRegionAvail().x - 4 * style.ItemSpacing.x) / 5;
        if (child_w < 1.0f)
            child_w = 1.0f;
        HvkGui::PushID("##VerticalScrolling");
        for (int i = 0; i < 5; i++)
        {
            if (i > 0) HvkGui::SameLine();
            HvkGui::BeginGroup();
            const char* names[] = { "Top", "25%", "Center", "75%", "Bottom" };
            HvkGui::TextUnformatted(names[i]);

            const ImGuiWindowFlags child_flags = enable_extra_decorations ? ImGuiWindowFlags_MenuBar : 0;
            const ImGuiID child_id = HvkGui::GetID((void*)(intptr_t)i);
            const bool child_is_visible = HvkGui::BeginChild(child_id, ImVec2(child_w, 200.0f), ImGuiChildFlags_Borders, child_flags);
            if (HvkGui::BeginMenuBar())
            {
                HvkGui::TextUnformatted("abc");
                HvkGui::EndMenuBar();
            }
            if (scroll_to_off)
                HvkGui::SetScrollY(scroll_to_off_px);
            if (scroll_to_pos)
                HvkGui::SetScrollFromPosY(HvkGui::GetCursorStartPos().y + scroll_to_pos_px, i * 0.25f);
            if (child_is_visible) // Avoid calling SetScrollHereY when running with culled items
            {
                for (int item = 0; item < 100; item++)
                {
                    if (enable_track && item == track_item)
                    {
                        HvkGui::TextColored(ImVec4(1, 1, 0, 1), "Item %d", item);
                        HvkGui::SetScrollHereY(i * 0.25f); // 0.0f:top, 0.5f:center, 1.0f:bottom
                    }
                    else
                    {
                        HvkGui::Text("Item %d", item);
                    }
                }
            }
            float scroll_y = HvkGui::GetScrollY();
            float scroll_max_y = HvkGui::GetScrollMaxY();
            HvkGui::EndChild();
            HvkGui::Text("%.0f/%.0f", scroll_y, scroll_max_y);
            HvkGui::EndGroup();
        }
        HvkGui::PopID();

        // Horizontal scroll functions
        IMGUI_DEMO_MARKER("Layout/Scrolling/Horizontal");
        HvkGui::Spacing();
        HelpMarker(
            "Use SetScrollHereX() or SetScrollFromPosX() to scroll to a given horizontal position.\n\n"
            "Because the clipping rectangle of most window hides half worth of WindowPadding on the "
            "left/right, using SetScrollFromPosX(+1) will usually result in clipped text whereas the "
            "equivalent SetScrollFromPosY(+1) wouldn't.");
        HvkGui::PushID("##HorizontalScrolling");
        for (int i = 0; i < 5; i++)
        {
            float child_height = HvkGui::GetTextLineHeight() + style.ScrollbarSize + style.WindowPadding.y * 2.0f;
            ImGuiWindowFlags child_flags = ImGuiWindowFlags_HorizontalScrollbar | (enable_extra_decorations ? ImGuiWindowFlags_AlwaysVerticalScrollbar : 0);
            ImGuiID child_id = HvkGui::GetID((void*)(intptr_t)i);
            bool child_is_visible = HvkGui::BeginChild(child_id, ImVec2(-100, child_height), ImGuiChildFlags_Borders, child_flags);
            if (scroll_to_off)
                HvkGui::SetScrollX(scroll_to_off_px);
            if (scroll_to_pos)
                HvkGui::SetScrollFromPosX(HvkGui::GetCursorStartPos().x + scroll_to_pos_px, i * 0.25f);
            if (child_is_visible) // Avoid calling SetScrollHereY when running with culled items
            {
                for (int item = 0; item < 100; item++)
                {
                    if (item > 0)
                        HvkGui::SameLine();
                    if (enable_track && item == track_item)
                    {
                        HvkGui::TextColored(ImVec4(1, 1, 0, 1), "Item %d", item);
                        HvkGui::SetScrollHereX(i * 0.25f); // 0.0f:left, 0.5f:center, 1.0f:right
                    }
                    else
                    {
                        HvkGui::Text("Item %d", item);
                    }
                }
            }
            float scroll_x = HvkGui::GetScrollX();
            float scroll_max_x = HvkGui::GetScrollMaxX();
            HvkGui::EndChild();
            HvkGui::SameLine();
            const char* names[] = { "Left", "25%", "Center", "75%", "Right" };
            HvkGui::Text("%s\n%.0f/%.0f", names[i], scroll_x, scroll_max_x);
            HvkGui::Spacing();
        }
        HvkGui::PopID();

        // Miscellaneous Horizontal Scrolling Demo
        IMGUI_DEMO_MARKER("Layout/Scrolling/Horizontal (more)");
        HelpMarker(
            "Horizontal scrolling for a window is enabled via the ImGuiWindowFlags_HorizontalScrollbar flag.\n\n"
            "You may want to also explicitly specify content width by using SetNextWindowContentWidth() before Begin().");
        static int lines = 7;
        HvkGui::SliderInt("Lines", &lines, 1, 15);
        HvkGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
        HvkGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));
        ImVec2 scrolling_child_size = ImVec2(0, HvkGui::GetFrameHeightWithSpacing() * 7 + 30);
        HvkGui::BeginChild("scrolling", scrolling_child_size, ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar);
        for (int line = 0; line < lines; line++)
        {
            // Display random stuff. For the sake of this trivial demo we are using basic Button() + SameLine()
            // If you want to create your own time line for a real application you may be better off manipulating
            // the cursor position yourself, aka using SetCursorPos/SetCursorScreenPos to position the widgets
            // yourself. You may also want to use the lower-level ImDrawList API.
            int num_buttons = 10 + ((line & 1) ? line * 9 : line * 3);
            for (int n = 0; n < num_buttons; n++)
            {
                if (n > 0) HvkGui::SameLine();
                HvkGui::PushID(n + line * 1000);
                char num_buf[16];
                sprintf(num_buf, "%d", n);
                const char* label = (!(n % 15)) ? "FizzBuzz" : (!(n % 3)) ? "Fizz" : (!(n % 5)) ? "Buzz" : num_buf;
                float hue = n * 0.05f;
                HvkGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue, 0.6f, 0.6f));
                HvkGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue, 0.7f, 0.7f));
                HvkGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue, 0.8f, 0.8f));
                HvkGui::Button(label, ImVec2(40.0f + sinf((float)(line + n)) * 20.0f, 0.0f));
                HvkGui::PopStyleColor(3);
                HvkGui::PopID();
            }
        }
        float scroll_x = HvkGui::GetScrollX();
        float scroll_max_x = HvkGui::GetScrollMaxX();
        HvkGui::EndChild();
        HvkGui::PopStyleVar(2);
        float scroll_x_delta = 0.0f;
        HvkGui::SmallButton("<<");
        if (HvkGui::IsItemActive())
            scroll_x_delta = -HvkGui::GetIO().DeltaTime * 1000.0f;
        HvkGui::SameLine();
        HvkGui::Text("Scroll from code"); HvkGui::SameLine();
        HvkGui::SmallButton(">>");
        if (HvkGui::IsItemActive())
            scroll_x_delta = +HvkGui::GetIO().DeltaTime * 1000.0f;
        HvkGui::SameLine();
        HvkGui::Text("%.0f/%.0f", scroll_x, scroll_max_x);
        if (scroll_x_delta != 0.0f)
        {
            // Demonstrate a trick: you can use Begin to set yourself in the context of another window
            // (here we are already out of your child window)
            HvkGui::BeginChild("scrolling");
            HvkGui::SetScrollX(HvkGui::GetScrollX() + scroll_x_delta);
            HvkGui::EndChild();
        }
        HvkGui::Spacing();

        static bool show_horizontal_contents_size_demo_window = false;
        HvkGui::Checkbox("Show Horizontal contents size demo window", &show_horizontal_contents_size_demo_window);

        if (show_horizontal_contents_size_demo_window)
        {
            static bool show_h_scrollbar = true;
            static bool show_button = true;
            static bool show_tree_nodes = true;
            static bool show_text_wrapped = false;
            static bool show_columns = true;
            static bool show_tab_bar = true;
            static bool show_child = false;
            static bool explicit_content_size = false;
            static float contents_size_x = 300.0f;
            if (explicit_content_size)
                HvkGui::SetNextWindowContentSize(ImVec2(contents_size_x, 0.0f));
            HvkGui::Begin("Horizontal contents size demo window", &show_horizontal_contents_size_demo_window, show_h_scrollbar ? ImGuiWindowFlags_HorizontalScrollbar : 0);
            IMGUI_DEMO_MARKER("Layout/Scrolling/Horizontal contents size demo window");
            HvkGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 0));
            HvkGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 0));
            HelpMarker(
                "Test how different widgets react and impact the work rectangle growing when horizontal scrolling is enabled.\n\n"
                "Use 'Metrics->Tools->Show windows rectangles' to visualize rectangles.");
            HvkGui::Checkbox("H-scrollbar", &show_h_scrollbar);
            HvkGui::Checkbox("Button", &show_button);            // Will grow contents size (unless explicitly overwritten)
            HvkGui::Checkbox("Tree nodes", &show_tree_nodes);    // Will grow contents size and display highlight over full width
            HvkGui::Checkbox("Text wrapped", &show_text_wrapped);// Will grow and use contents size
            HvkGui::Checkbox("Columns", &show_columns);          // Will use contents size
            HvkGui::Checkbox("Tab bar", &show_tab_bar);          // Will use contents size
            HvkGui::Checkbox("Child", &show_child);              // Will grow and use contents size
            HvkGui::Checkbox("Explicit content size", &explicit_content_size);
            HvkGui::Text("Scroll %.1f/%.1f %.1f/%.1f", HvkGui::GetScrollX(), HvkGui::GetScrollMaxX(), HvkGui::GetScrollY(), HvkGui::GetScrollMaxY());
            if (explicit_content_size)
            {
                HvkGui::SameLine();
                HvkGui::SetNextItemWidth(HvkGui::CalcTextSize("123456").x);
                HvkGui::DragFloat("##csx", &contents_size_x);
                ImVec2 p = HvkGui::GetCursorScreenPos();
                HvkGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + 10, p.y + 10), IM_COL32_WHITE);
                HvkGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + contents_size_x - 10, p.y), ImVec2(p.x + contents_size_x, p.y + 10), IM_COL32_WHITE);
                HvkGui::Dummy(ImVec2(0, 10));
            }
            HvkGui::PopStyleVar(2);
            HvkGui::Separator();
            if (show_button)
            {
                HvkGui::Button("this is a 300-wide button", ImVec2(300, 0));
            }
            if (show_tree_nodes)
            {
                bool open = true;
                if (HvkGui::TreeNode("this is a tree node"))
                {
                    if (HvkGui::TreeNode("another one of those tree node..."))
                    {
                        HvkGui::Text("Some tree contents");
                        HvkGui::TreePop();
                    }
                    HvkGui::TreePop();
                }
                HvkGui::CollapsingHeader("CollapsingHeader", &open);
            }
            if (show_text_wrapped)
            {
                HvkGui::TextWrapped("This text should automatically wrap on the edge of the work rectangle.");
            }
            if (show_columns)
            {
                HvkGui::Text("Tables:");
                if (HvkGui::BeginTable("table", 4, ImGuiTableFlags_Borders))
                {
                    for (int n = 0; n < 4; n++)
                    {
                        HvkGui::TableNextColumn();
                        HvkGui::Text("Width %.2f", HvkGui::GetContentRegionAvail().x);
                    }
                    HvkGui::EndTable();
                }
                HvkGui::Text("Columns:");
                HvkGui::Columns(4);
                for (int n = 0; n < 4; n++)
                {
                    HvkGui::Text("Width %.2f", HvkGui::GetColumnWidth());
                    HvkGui::NextColumn();
                }
                HvkGui::Columns(1);
            }
            if (show_tab_bar && HvkGui::BeginTabBar("Hello"))
            {
                if (HvkGui::BeginTabItem("OneOneOne")) { HvkGui::EndTabItem(); }
                if (HvkGui::BeginTabItem("TwoTwoTwo")) { HvkGui::EndTabItem(); }
                if (HvkGui::BeginTabItem("ThreeThreeThree")) { HvkGui::EndTabItem(); }
                if (HvkGui::BeginTabItem("FourFourFour")) { HvkGui::EndTabItem(); }
                HvkGui::EndTabBar();
            }
            if (show_child)
            {
                HvkGui::BeginChild("child", ImVec2(0, 0), ImGuiChildFlags_Borders);
                HvkGui::EndChild();
            }
            HvkGui::End();
        }

        HvkGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Layout/Text Clipping");
    if (HvkGui::TreeNode("Text Clipping"))
    {
        static ImVec2 size(100.0f, 100.0f);
        static ImVec2 offset(30.0f, 30.0f);
        HvkGui::DragFloat2("size", (float*)&size, 0.5f, 1.0f, 200.0f, "%.0f");
        HvkGui::TextWrapped("(Click and drag to scroll)");

        HelpMarker(
            "(Left) Using HvkGui::PushClipRect():\n"
            "Will alter ImGui hit-testing logic + ImDrawList rendering.\n"
            "(use this if you want your clipping rectangle to affect interactions)\n\n"
            "(Center) Using ImDrawList::PushClipRect():\n"
            "Will alter ImDrawList rendering only.\n"
            "(use this as a shortcut if you are only using ImDrawList calls)\n\n"
            "(Right) Using ImDrawList::AddText() with a fine ClipRect:\n"
            "Will alter only this specific ImDrawList::AddText() rendering.\n"
            "This is often used internally to avoid altering the clipping rectangle and minimize draw calls.");

        for (int n = 0; n < 3; n++)
        {
            if (n > 0)
                HvkGui::SameLine();

            HvkGui::PushID(n);
            HvkGui::InvisibleButton("##canvas", size);
            if (HvkGui::IsItemActive() && HvkGui::IsMouseDragging(ImGuiMouseButton_Left))
            {
                offset.x += HvkGui::GetIO().MouseDelta.x;
                offset.y += HvkGui::GetIO().MouseDelta.y;
            }
            HvkGui::PopID();
            if (!HvkGui::IsItemVisible()) // Skip rendering as ImDrawList elements are not clipped.
                continue;

            const ImVec2 p0 = HvkGui::GetItemRectMin();
            const ImVec2 p1 = HvkGui::GetItemRectMax();
            const char* text_str = "Line 1 hello\nLine 2 clip me!";
            const ImVec2 text_pos = ImVec2(p0.x + offset.x, p0.y + offset.y);
            ImDrawList* draw_list = HvkGui::GetWindowDrawList();
            switch (n)
            {
            case 0:
                HvkGui::PushClipRect(p0, p1, true);
                draw_list->AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 255));
                draw_list->AddText(text_pos, IM_COL32_WHITE, text_str);
                HvkGui::PopClipRect();
                break;
            case 1:
                draw_list->PushClipRect(p0, p1, true);
                draw_list->AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 255));
                draw_list->AddText(text_pos, IM_COL32_WHITE, text_str);
                draw_list->PopClipRect();
                break;
            case 2:
                ImVec4 clip_rect(p0.x, p0.y, p1.x, p1.y); // AddText() takes a ImVec4* here so let's convert.
                draw_list->AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 255));
                draw_list->AddText(HvkGui::GetFont(), HvkGui::GetFontSize(), text_pos, IM_COL32_WHITE, text_str, NULL, 0.0f, &clip_rect);
                break;
            }
        }

        HvkGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Layout/Overlap Mode");
    if (HvkGui::TreeNode("Overlap Mode"))
    {
        static bool enable_allow_overlap = true;

        HelpMarker(
            "Hit-testing is by default performed in item submission order, which generally is perceived as 'back-to-front'.\n\n"
            "By using SetNextItemAllowOverlap() you can notify that an item may be overlapped by another. "
            "Doing so alters the hovering logic: items using AllowOverlap mode requires an extra frame to accept hovered state.");
        HvkGui::Checkbox("Enable AllowOverlap", &enable_allow_overlap);

        ImVec2 button1_pos = HvkGui::GetCursorScreenPos();
        ImVec2 button2_pos = ImVec2(button1_pos.x + 50.0f, button1_pos.y + 50.0f);
        if (enable_allow_overlap)
            HvkGui::SetNextItemAllowOverlap();
        HvkGui::Button("Button 1", ImVec2(80, 80));
        HvkGui::SetCursorScreenPos(button2_pos);
        HvkGui::Button("Button 2", ImVec2(80, 80));

        // This is typically used with width-spanning items.
        // (note that Selectable() has a dedicated flag ImGuiSelectableFlags_AllowOverlap, which is a shortcut
        // for using SetNextItemAllowOverlap(). For demo purpose we use SetNextItemAllowOverlap() here.)
        if (enable_allow_overlap)
            HvkGui::SetNextItemAllowOverlap();
        HvkGui::Selectable("Some Selectable", false);
        HvkGui::SameLine();
        HvkGui::SmallButton("++");

        HvkGui::TreePop();
    }
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowPopups()
//-----------------------------------------------------------------------------

static void DemoWindowPopups()
{
    IMGUI_DEMO_MARKER("Popups");
    if (!HvkGui::CollapsingHeader("Popups & Modal windows"))
        return;

    // The properties of popups windows are:
    // - They block normal mouse hovering detection outside them. (*)
    // - Unless modal, they can be closed by clicking anywhere outside them, or by pressing ESCAPE.
    // - Their visibility state (~bool) is held internally by Dear ImGui instead of being held by the programmer as
    //   we are used to with regular Begin() calls. User can manipulate the visibility state by calling OpenPopup().
    // (*) One can use IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) to bypass it and detect hovering even
    //     when normally blocked by a popup.
    // Those three properties are connected. The library needs to hold their visibility state BECAUSE it can close
    // popups at any time.

    // Typical use for regular windows:
    //   bool my_tool_is_active = false; if (HvkGui::Button("Open")) my_tool_is_active = true; [...] if (my_tool_is_active) Begin("My Tool", &my_tool_is_active) { [...] } End();
    // Typical use for popups:
    //   if (HvkGui::Button("Open")) HvkGui::OpenPopup("MyPopup"); if (HvkGui::BeginPopup("MyPopup")) { [...] EndPopup(); }

    // With popups we have to go through a library call (here OpenPopup) to manipulate the visibility state.
    // This may be a bit confusing at first but it should quickly make sense. Follow on the examples below.

    IMGUI_DEMO_MARKER("Popups/Popups");
    if (HvkGui::TreeNode("Popups"))
    {
        HvkGui::TextWrapped(
            "When a popup is active, it inhibits interacting with windows that are behind the popup. "
            "Clicking outside the popup closes it.");

        static int selected_fish = -1;
        const char* names[] = { "Bream", "Haddock", "Mackerel", "Pollock", "Tilefish" };
        static bool toggles[] = { true, false, false, false, false };

        // Simple selection popup (if you want to show the current selection inside the Button itself,
        // you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
        if (HvkGui::Button("Select.."))
            HvkGui::OpenPopup("my_select_popup");
        HvkGui::SameLine();
        HvkGui::TextUnformatted(selected_fish == -1 ? "<None>" : names[selected_fish]);
        if (HvkGui::BeginPopup("my_select_popup"))
        {
            HvkGui::SeparatorText("Aquarium");
            for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                if (HvkGui::Selectable(names[i]))
                    selected_fish = i;
            HvkGui::EndPopup();
        }

        // Showing a menu with toggles
        if (HvkGui::Button("Toggle.."))
            HvkGui::OpenPopup("my_toggle_popup");
        if (HvkGui::BeginPopup("my_toggle_popup"))
        {
            for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                HvkGui::MenuItem(names[i], "", &toggles[i]);
            if (HvkGui::BeginMenu("Sub-menu"))
            {
                HvkGui::MenuItem("Click me");
                HvkGui::EndMenu();
            }

            HvkGui::Separator();
            HvkGui::Text("Tooltip here");
            HvkGui::SetItemTooltip("I am a tooltip over a popup");

            if (HvkGui::Button("Stacked Popup"))
                HvkGui::OpenPopup("another popup");
            if (HvkGui::BeginPopup("another popup"))
            {
                for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                    HvkGui::MenuItem(names[i], "", &toggles[i]);
                if (HvkGui::BeginMenu("Sub-menu"))
                {
                    HvkGui::MenuItem("Click me");
                    if (HvkGui::Button("Stacked Popup"))
                        HvkGui::OpenPopup("another popup");
                    if (HvkGui::BeginPopup("another popup"))
                    {
                        HvkGui::Text("I am the last one here.");
                        HvkGui::EndPopup();
                    }
                    HvkGui::EndMenu();
                }
                HvkGui::EndPopup();
            }
            HvkGui::EndPopup();
        }

        // Call the more complete ShowExampleMenuFile which we use in various places of this demo
        if (HvkGui::Button("With a menu.."))
            HvkGui::OpenPopup("my_file_popup");
        if (HvkGui::BeginPopup("my_file_popup", ImGuiWindowFlags_MenuBar))
        {
            if (HvkGui::BeginMenuBar())
            {
                if (HvkGui::BeginMenu("File"))
                {
                    ShowExampleMenuFile();
                    HvkGui::EndMenu();
                }
                if (HvkGui::BeginMenu("Edit"))
                {
                    HvkGui::MenuItem("Dummy");
                    HvkGui::EndMenu();
                }
                HvkGui::EndMenuBar();
            }
            HvkGui::Text("Hello from popup!");
            HvkGui::Button("This is a dummy button..");
            HvkGui::EndPopup();
        }

        HvkGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Popups/Context menus");
    if (HvkGui::TreeNode("Context menus"))
    {
        HelpMarker("\"Context\" functions are simple helpers to associate a Popup to a given Item or Window identifier.");

        // BeginPopupContextItem() is a helper to provide common/simple popup behavior of essentially doing:
        //     if (id == 0)
        //         id = GetItemID(); // Use last item id
        //     if (IsItemHovered() && IsMouseReleased(ImGuiMouseButton_Right))
        //         OpenPopup(id);
        //     return BeginPopup(id);
        // For advanced uses you may want to replicate and customize this code.
        // See more details in BeginPopupContextItem().

        // Example 1
        // When used after an item that has an ID (e.g. Button), we can skip providing an ID to BeginPopupContextItem(),
        // and BeginPopupContextItem() will use the last item ID as the popup ID.
        {
            const char* names[5] = { "Label1", "Label2", "Label3", "Label4", "Label5" };
            static int selected = -1;
            for (int n = 0; n < 5; n++)
            {
                if (HvkGui::Selectable(names[n], selected == n))
                    selected = n;
                if (HvkGui::BeginPopupContextItem()) // <-- use last item id as popup id
                {
                    selected = n;
                    HvkGui::Text("This is a popup for \"%s\"!", names[n]);
                    if (HvkGui::Button("Close"))
                        HvkGui::CloseCurrentPopup();
                    HvkGui::EndPopup();
                }
                HvkGui::SetItemTooltip("Right-click to open popup");
            }
        }

        // Example 2
        // Popup on a Text() element which doesn't have an identifier: we need to provide an identifier to BeginPopupContextItem().
        // Using an explicit identifier is also convenient if you want to activate the popups from different locations.
        {
            HelpMarker("Text() elements don't have stable identifiers so we need to provide one.");
            static float value = 0.5f;
            HvkGui::Text("Value = %.3f <-- (1) right-click this text", value);
            if (HvkGui::BeginPopupContextItem("my popup"))
            {
                if (HvkGui::Selectable("Set to zero")) value = 0.0f;
                if (HvkGui::Selectable("Set to PI")) value = 3.1415f;
                HvkGui::SetNextItemWidth(-FLT_MIN);
                HvkGui::DragFloat("##Value", &value, 0.1f, 0.0f, 0.0f);
                HvkGui::EndPopup();
            }

            // We can also use OpenPopupOnItemClick() to toggle the visibility of a given popup.
            // Here we make it that right-clicking this other text element opens the same popup as above.
            // The popup itself will be submitted by the code above.
            HvkGui::Text("(2) Or right-click this text");
            HvkGui::OpenPopupOnItemClick("my popup", ImGuiPopupFlags_MouseButtonRight);

            // Back to square one: manually open the same popup.
            if (HvkGui::Button("(3) Or click this button"))
                HvkGui::OpenPopup("my popup");
        }

        // Example 3
        // When using BeginPopupContextItem() with an implicit identifier (NULL == use last item ID),
        // we need to make sure your item identifier is stable.
        // In this example we showcase altering the item label while preserving its identifier, using the ### operator (see FAQ).
        {
            HelpMarker("Showcase using a popup ID linked to item ID, with the item having a changing label + stable ID using the ### operator.");
            static char name[32] = "Label1";
            char buf[64];
            sprintf(buf, "Button: %s###Button", name); // ### operator override ID ignoring the preceding label
            HvkGui::Button(buf);
            if (HvkGui::BeginPopupContextItem())
            {
                HvkGui::Text("Edit name:");
                HvkGui::InputText("##edit", name, IM_ARRAYSIZE(name));
                if (HvkGui::Button("Close"))
                    HvkGui::CloseCurrentPopup();
                HvkGui::EndPopup();
            }
            HvkGui::SameLine(); HvkGui::Text("(<-- right-click here)");
        }

        HvkGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Popups/Modals");
    if (HvkGui::TreeNode("Modals"))
    {
        HvkGui::TextWrapped("Modal windows are like popups but the user cannot close them by clicking outside.");

        if (HvkGui::Button("Delete.."))
            HvkGui::OpenPopup("Delete?");

        // Always center this window when appearing
        ImVec2 center = HvkGui::GetMainViewport()->GetCenter();
        HvkGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (HvkGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            HvkGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!");
            HvkGui::Separator();

            //static int unused_i = 0;
            //HvkGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

            static bool dont_ask_me_next_time = false;
            HvkGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            HvkGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
            HvkGui::PopStyleVar();

            if (HvkGui::Button("OK", ImVec2(120, 0))) { HvkGui::CloseCurrentPopup(); }
            HvkGui::SetItemDefaultFocus();
            HvkGui::SameLine();
            if (HvkGui::Button("Cancel", ImVec2(120, 0))) { HvkGui::CloseCurrentPopup(); }
            HvkGui::EndPopup();
        }

        if (HvkGui::Button("Stacked modals.."))
            HvkGui::OpenPopup("Stacked 1");
        if (HvkGui::BeginPopupModal("Stacked 1", NULL, ImGuiWindowFlags_MenuBar))
        {
            if (HvkGui::BeginMenuBar())
            {
                if (HvkGui::BeginMenu("File"))
                {
                    if (HvkGui::MenuItem("Some menu item")) {}
                    HvkGui::EndMenu();
                }
                HvkGui::EndMenuBar();
            }
            HvkGui::Text("Hello from Stacked The First\nUsing style.Colors[ImGuiCol_ModalWindowDimBg] behind it.");

            // Testing behavior of widgets stacking their own regular popups over the modal.
            static int item = 1;
            static float color[4] = { 0.4f, 0.7f, 0.0f, 0.5f };
            HvkGui::Combo("Combo", &item, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");
            HvkGui::ColorEdit4("Color", color);

            if (HvkGui::Button("Add another modal.."))
                HvkGui::OpenPopup("Stacked 2");

            // Also demonstrate passing a bool* to BeginPopupModal(), this will create a regular close button which
            // will close the popup. Note that the visibility state of popups is owned by imgui, so the input value
            // of the bool actually doesn't matter here.
            bool unused_open = true;
            if (HvkGui::BeginPopupModal("Stacked 2", &unused_open))
            {
                HvkGui::Text("Hello from Stacked The Second!");
                HvkGui::ColorEdit4("Color", color); // Allow opening another nested popup
                if (HvkGui::Button("Close"))
                    HvkGui::CloseCurrentPopup();
                HvkGui::EndPopup();
            }

            if (HvkGui::Button("Close"))
                HvkGui::CloseCurrentPopup();
            HvkGui::EndPopup();
        }

        HvkGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Popups/Menus inside a regular window");
    if (HvkGui::TreeNode("Menus inside a regular window"))
    {
        HvkGui::TextWrapped("Below we are testing adding menu items to a regular window. It's rather unusual but should work!");
        HvkGui::Separator();

        HvkGui::MenuItem("Menu item", "Ctrl+M");
        if (HvkGui::BeginMenu("Menu inside a regular window"))
        {
            ShowExampleMenuFile();
            HvkGui::EndMenu();
        }
        HvkGui::Separator();
        HvkGui::TreePop();
    }
}

// Dummy data structure that we use for the Table demo.
// (pre-C++11 doesn't allow us to instantiate ImVector<MyItem> template if this structure is defined inside the demo function)
namespace
{
// We are passing our own identifier to TableSetupColumn() to facilitate identifying columns in the sorting code.
// This identifier will be passed down into ImGuiTableSortSpec::ColumnUserID.
// But it is possible to omit the user id parameter of TableSetupColumn() and just use the column index instead! (ImGuiTableSortSpec::ColumnIndex)
// If you don't use sorting, you will generally never care about giving column an ID!
enum MyItemColumnID
{
    MyItemColumnID_ID,
    MyItemColumnID_Name,
    MyItemColumnID_Action,
    MyItemColumnID_Quantity,
    MyItemColumnID_Description
};

struct MyItem
{
    int         ID;
    const char* Name;
    int         Quantity;

    // We have a problem which is affecting _only this demo_ and should not affect your code:
    // As we don't rely on std:: or other third-party library to compile dear imgui, we only have reliable access to qsort(),
    // however qsort doesn't allow passing user data to comparing function.
    // As a workaround, we are storing the sort specs in a static/global for the comparing function to access.
    // In your own use case you would probably pass the sort specs to your sorting/comparing functions directly and not use a global.
    // We could technically call HvkGui::TableGetSortSpecs() in CompareWithSortSpecs(), but considering that this function is called
    // very often by the sorting algorithm it would be a little wasteful.
    static const ImGuiTableSortSpecs* s_current_sort_specs;

    static void SortWithSortSpecs(ImGuiTableSortSpecs* sort_specs, MyItem* items, int items_count)
    {
        s_current_sort_specs = sort_specs; // Store in variable accessible by the sort function.
        if (items_count > 1)
            qsort(items, (size_t)items_count, sizeof(items[0]), MyItem::CompareWithSortSpecs);
        s_current_sort_specs = NULL;
    }

    // Compare function to be used by qsort()
    static int IMGUI_CDECL CompareWithSortSpecs(const void* lhs, const void* rhs)
    {
        const MyItem* a = (const MyItem*)lhs;
        const MyItem* b = (const MyItem*)rhs;
        for (int n = 0; n < s_current_sort_specs->SpecsCount; n++)
        {
            // Here we identify columns using the ColumnUserID value that we ourselves passed to TableSetupColumn()
            // We could also choose to identify columns based on their index (sort_spec->ColumnIndex), which is simpler!
            const ImGuiTableColumnSortSpecs* sort_spec = &s_current_sort_specs->Specs[n];
            int delta = 0;
            switch (sort_spec->ColumnUserID)
            {
            case MyItemColumnID_ID:             delta = (a->ID - b->ID);                break;
            case MyItemColumnID_Name:           delta = (strcmp(a->Name, b->Name));     break;
            case MyItemColumnID_Quantity:       delta = (a->Quantity - b->Quantity);    break;
            case MyItemColumnID_Description:    delta = (strcmp(a->Name, b->Name));     break;
            default: IM_ASSERT(0); break;
            }
            if (delta > 0)
                return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? +1 : -1;
            if (delta < 0)
                return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? -1 : +1;
        }

        // qsort() is instable so always return a way to differentiate items.
        // Your own compare function may want to avoid fallback on implicit sort specs.
        // e.g. a Name compare if it wasn't already part of the sort specs.
        return a->ID - b->ID;
    }
};
const ImGuiTableSortSpecs* MyItem::s_current_sort_specs = NULL;
}

// Make the UI compact because there are so many fields
static void PushStyleCompact()
{
    ImGuiStyle& style = HvkGui::GetStyle();
    HvkGui::PushStyleVarY(ImGuiStyleVar_FramePadding, (float)(int)(style.FramePadding.y * 0.60f));
    HvkGui::PushStyleVarY(ImGuiStyleVar_ItemSpacing, (float)(int)(style.ItemSpacing.y * 0.60f));
}

static void PopStyleCompact()
{
    HvkGui::PopStyleVar(2);
}

// Show a combo box with a choice of sizing policies
static void EditTableSizingFlags(ImGuiTableFlags* p_flags)
{
    struct EnumDesc { ImGuiTableFlags Value; const char* Name; const char* Tooltip; };
    static const EnumDesc policies[] =
    {
        { ImGuiTableFlags_None,               "Default",                            "Use default sizing policy:\n- ImGuiTableFlags_SizingFixedFit if ScrollX is on or if host window has ImGuiWindowFlags_AlwaysAutoResize.\n- ImGuiTableFlags_SizingStretchSame otherwise." },
        { ImGuiTableFlags_SizingFixedFit,     "ImGuiTableFlags_SizingFixedFit",     "Columns default to _WidthFixed (if resizable) or _WidthAuto (if not resizable), matching contents width." },
        { ImGuiTableFlags_SizingFixedSame,    "ImGuiTableFlags_SizingFixedSame",    "Columns are all the same width, matching the maximum contents width.\nImplicitly disable ImGuiTableFlags_Resizable and enable ImGuiTableFlags_NoKeepColumnsVisible." },
        { ImGuiTableFlags_SizingStretchProp,  "ImGuiTableFlags_SizingStretchProp",  "Columns default to _WidthStretch with weights proportional to their widths." },
        { ImGuiTableFlags_SizingStretchSame,  "ImGuiTableFlags_SizingStretchSame",  "Columns default to _WidthStretch with same weights." }
    };
    int idx;
    for (idx = 0; idx < IM_ARRAYSIZE(policies); idx++)
        if (policies[idx].Value == (*p_flags & ImGuiTableFlags_SizingMask_))
            break;
    const char* preview_text = (idx < IM_ARRAYSIZE(policies)) ? policies[idx].Name + (idx > 0 ? strlen("ImGuiTableFlags") : 0) : "";
    if (HvkGui::BeginCombo("Sizing Policy", preview_text))
    {
        for (int n = 0; n < IM_ARRAYSIZE(policies); n++)
            if (HvkGui::Selectable(policies[n].Name, idx == n))
                *p_flags = (*p_flags & ~ImGuiTableFlags_SizingMask_) | policies[n].Value;
        HvkGui::EndCombo();
    }
    HvkGui::SameLine();
    HvkGui::TextDisabled("(?)");
    if (HvkGui::BeginItemTooltip())
    {
        HvkGui::PushTextWrapPos(HvkGui::GetFontSize() * 50.0f);
        for (int m = 0; m < IM_ARRAYSIZE(policies); m++)
        {
            HvkGui::Separator();
            HvkGui::Text("%s:", policies[m].Name);
            HvkGui::Separator();
            HvkGui::SetCursorPosX(HvkGui::GetCursorPosX() + HvkGui::GetStyle().IndentSpacing * 0.5f);
            HvkGui::TextUnformatted(policies[m].Tooltip);
        }
        HvkGui::PopTextWrapPos();
        HvkGui::EndTooltip();
    }
}

static void EditTableColumnsFlags(ImGuiTableColumnFlags* p_flags)
{
    HvkGui::CheckboxFlags("_Disabled", p_flags, ImGuiTableColumnFlags_Disabled); HvkGui::SameLine(); HelpMarker("Master disable flag (also hide from context menu)");
    HvkGui::CheckboxFlags("_DefaultHide", p_flags, ImGuiTableColumnFlags_DefaultHide);
    HvkGui::CheckboxFlags("_DefaultSort", p_flags, ImGuiTableColumnFlags_DefaultSort);
    if (HvkGui::CheckboxFlags("_WidthStretch", p_flags, ImGuiTableColumnFlags_WidthStretch))
        *p_flags &= ~(ImGuiTableColumnFlags_WidthMask_ ^ ImGuiTableColumnFlags_WidthStretch);
    if (HvkGui::CheckboxFlags("_WidthFixed", p_flags, ImGuiTableColumnFlags_WidthFixed))
        *p_flags &= ~(ImGuiTableColumnFlags_WidthMask_ ^ ImGuiTableColumnFlags_WidthFixed);
    HvkGui::CheckboxFlags("_NoResize", p_flags, ImGuiTableColumnFlags_NoResize);
    HvkGui::CheckboxFlags("_NoReorder", p_flags, ImGuiTableColumnFlags_NoReorder);
    HvkGui::CheckboxFlags("_NoHide", p_flags, ImGuiTableColumnFlags_NoHide);
    HvkGui::CheckboxFlags("_NoClip", p_flags, ImGuiTableColumnFlags_NoClip);
    HvkGui::CheckboxFlags("_NoSort", p_flags, ImGuiTableColumnFlags_NoSort);
    HvkGui::CheckboxFlags("_NoSortAscending", p_flags, ImGuiTableColumnFlags_NoSortAscending);
    HvkGui::CheckboxFlags("_NoSortDescending", p_flags, ImGuiTableColumnFlags_NoSortDescending);
    HvkGui::CheckboxFlags("_NoHeaderLabel", p_flags, ImGuiTableColumnFlags_NoHeaderLabel);
    HvkGui::CheckboxFlags("_NoHeaderWidth", p_flags, ImGuiTableColumnFlags_NoHeaderWidth);
    HvkGui::CheckboxFlags("_PreferSortAscending", p_flags, ImGuiTableColumnFlags_PreferSortAscending);
    HvkGui::CheckboxFlags("_PreferSortDescending", p_flags, ImGuiTableColumnFlags_PreferSortDescending);
    HvkGui::CheckboxFlags("_IndentEnable", p_flags, ImGuiTableColumnFlags_IndentEnable); HvkGui::SameLine(); HelpMarker("Default for column 0");
    HvkGui::CheckboxFlags("_IndentDisable", p_flags, ImGuiTableColumnFlags_IndentDisable); HvkGui::SameLine(); HelpMarker("Default for column >0");
    HvkGui::CheckboxFlags("_AngledHeader", p_flags, ImGuiTableColumnFlags_AngledHeader);
}

static void ShowTableColumnsStatusFlags(ImGuiTableColumnFlags flags)
{
    HvkGui::CheckboxFlags("_IsEnabled", &flags, ImGuiTableColumnFlags_IsEnabled);
    HvkGui::CheckboxFlags("_IsVisible", &flags, ImGuiTableColumnFlags_IsVisible);
    HvkGui::CheckboxFlags("_IsSorted", &flags, ImGuiTableColumnFlags_IsSorted);
    HvkGui::CheckboxFlags("_IsHovered", &flags, ImGuiTableColumnFlags_IsHovered);
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowTables()
//-----------------------------------------------------------------------------

static void DemoWindowTables()
{
    //HvkGui::SetNextItemOpen(true, ImGuiCond_Once);
    IMGUI_DEMO_MARKER("Tables");
    if (!HvkGui::CollapsingHeader("Tables & Columns"))
        return;

    // Using those as a base value to create width/height that are factor of the size of our font
    const float TEXT_BASE_WIDTH = HvkGui::CalcTextSize("A").x;
    const float TEXT_BASE_HEIGHT = HvkGui::GetTextLineHeightWithSpacing();

    HvkGui::PushID("Tables");

    int open_action = -1;
    if (HvkGui::Button("Expand all"))
        open_action = 1;
    HvkGui::SameLine();
    if (HvkGui::Button("Collapse all"))
        open_action = 0;
    HvkGui::SameLine();

    // Options
    static bool disable_indent = false;
    HvkGui::Checkbox("Disable tree indentation", &disable_indent);
    HvkGui::SameLine();
    HelpMarker("Disable the indenting of tree nodes so demo tables can use the full window width.");
    HvkGui::Separator();
    if (disable_indent)
        HvkGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);

    // About Styling of tables
    // Most settings are configured on a per-table basis via the flags passed to BeginTable() and TableSetupColumns APIs.
    // There are however a few settings that a shared and part of the ImGuiStyle structure:
    //   style.CellPadding                          // Padding within each cell
    //   style.Colors[ImGuiCol_TableHeaderBg]       // Table header background
    //   style.Colors[ImGuiCol_TableBorderStrong]   // Table outer and header borders
    //   style.Colors[ImGuiCol_TableBorderLight]    // Table inner borders
    //   style.Colors[ImGuiCol_TableRowBg]          // Table row background when ImGuiTableFlags_RowBg is enabled (even rows)
    //   style.Colors[ImGuiCol_TableRowBgAlt]       // Table row background when ImGuiTableFlags_RowBg is enabled (odds rows)

    // Demos
    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Basic");
    if (HvkGui::TreeNode("Basic"))
    {
        // Here we will showcase three different ways to output a table.
        // They are very simple variations of a same thing!

        // [Method 1] Using TableNextRow() to create a new row, and TableSetColumnIndex() to select the column.
        // In many situations, this is the most flexible and easy to use pattern.
        HelpMarker("Using TableNextRow() + calling TableSetColumnIndex() _before_ each cell, in a loop.");
        if (HvkGui::BeginTable("table1", 3))
        {
            for (int row = 0; row < 4; row++)
            {
                HvkGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    HvkGui::TableSetColumnIndex(column);
                    HvkGui::Text("Row %d Column %d", row, column);
                }
            }
            HvkGui::EndTable();
        }

        // [Method 2] Using TableNextColumn() called multiple times, instead of using a for loop + TableSetColumnIndex().
        // This is generally more convenient when you have code manually submitting the contents of each column.
        HelpMarker("Using TableNextRow() + calling TableNextColumn() _before_ each cell, manually.");
        if (HvkGui::BeginTable("table2", 3))
        {
            for (int row = 0; row < 4; row++)
            {
                HvkGui::TableNextRow();
                HvkGui::TableNextColumn();
                HvkGui::Text("Row %d", row);
                HvkGui::TableNextColumn();
                HvkGui::Text("Some contents");
                HvkGui::TableNextColumn();
                HvkGui::Text("123.456");
            }
            HvkGui::EndTable();
        }

        // [Method 3] We call TableNextColumn() _before_ each cell. We never call TableNextRow(),
        // as TableNextColumn() will automatically wrap around and create new rows as needed.
        // This is generally more convenient when your cells all contains the same type of data.
        HelpMarker(
            "Only using TableNextColumn(), which tends to be convenient for tables where every cell contains "
            "the same type of contents.\n This is also more similar to the old NextColumn() function of the "
            "Columns API, and provided to facilitate the Columns->Tables API transition.");
        if (HvkGui::BeginTable("table3", 3))
        {
            for (int item = 0; item < 14; item++)
            {
                HvkGui::TableNextColumn();
                HvkGui::Text("Item %d", item);
            }
            HvkGui::EndTable();
        }

        HvkGui::TreePop();
    }

    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Borders, background");
    if (HvkGui::TreeNode("Borders, background"))
    {
        // Expose a few Borders related flags interactively
        enum ContentsType { CT_Text, CT_FillButton };
        static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
        static bool display_headers = false;
        static int contents_type = CT_Text;

        PushStyleCompact();
        HvkGui::CheckboxFlags("ImGuiTableFlags_RowBg", &flags, ImGuiTableFlags_RowBg);
        HvkGui::CheckboxFlags("ImGuiTableFlags_Borders", &flags, ImGuiTableFlags_Borders);
        HvkGui::SameLine(); HelpMarker("ImGuiTableFlags_Borders\n = ImGuiTableFlags_BordersInnerV\n | ImGuiTableFlags_BordersOuterV\n | ImGuiTableFlags_BordersInnerH\n | ImGuiTableFlags_BordersOuterH");
        HvkGui::Indent();

        HvkGui::CheckboxFlags("ImGuiTableFlags_BordersH", &flags, ImGuiTableFlags_BordersH);
        HvkGui::Indent();
        HvkGui::CheckboxFlags("ImGuiTableFlags_BordersOuterH", &flags, ImGuiTableFlags_BordersOuterH);
        HvkGui::CheckboxFlags("ImGuiTableFlags_BordersInnerH", &flags, ImGuiTableFlags_BordersInnerH);
        HvkGui::Unindent();

        HvkGui::CheckboxFlags("ImGuiTableFlags_BordersV", &flags, ImGuiTableFlags_BordersV);
        HvkGui::Indent();
        HvkGui::CheckboxFlags("ImGuiTableFlags_BordersOuterV", &flags, ImGuiTableFlags_BordersOuterV);
        HvkGui::CheckboxFlags("ImGuiTableFlags_BordersInnerV", &flags, ImGuiTableFlags_BordersInnerV);
        HvkGui::Unindent();

        HvkGui::CheckboxFlags("ImGuiTableFlags_BordersOuter", &flags, ImGuiTableFlags_BordersOuter);
        HvkGui::CheckboxFlags("ImGuiTableFlags_BordersInner", &flags, ImGuiTableFlags_BordersInner);
        HvkGui::Unindent();

        HvkGui::AlignTextToFramePadding(); HvkGui::Text("Cell contents:");
        HvkGui::SameLine(); HvkGui::RadioButton("Text", &contents_type, CT_Text);
        HvkGui::SameLine(); HvkGui::RadioButton("FillButton", &contents_type, CT_FillButton);
        HvkGui::Checkbox("Display headers", &display_headers);
        HvkGui::CheckboxFlags("ImGuiTableFlags_NoBordersInBody", &flags, ImGuiTableFlags_NoBordersInBody); HvkGui::SameLine(); HelpMarker("Disable vertical borders in columns Body (borders will always appear in Headers)");
        PopStyleCompact();

        if (HvkGui::BeginTable("table1", 3, flags))
        {
            // Display headers so we can inspect their interaction with borders
            // (Headers are not the main purpose of this section of the demo, so we are not elaborating on them now. See other sections for details)
            if (display_headers)
            {
                HvkGui::TableSetupColumn("One");
                HvkGui::TableSetupColumn("Two");
                HvkGui::TableSetupColumn("Three");
                HvkGui::TableHeadersRow();
            }

            for (int row = 0; row < 5; row++)
            {
                HvkGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    HvkGui::TableSetColumnIndex(column);
                    char buf[32];
                    sprintf(buf, "Hello %d,%d", column, row);
                    if (contents_type == CT_Text)
                        HvkGui::TextUnformatted(buf);
                    else if (contents_type == CT_FillButton)
                        HvkGui::Button(buf, ImVec2(-FLT_MIN, 0.0f));
                }
            }
            HvkGui::EndTable();
        }
        HvkGui::TreePop();
    }

    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Resizable, stretch");
    if (HvkGui::TreeNode("Resizable, stretch"))
    {
        // By default, if we don't enable ScrollX the sizing policy for each column is "Stretch"
        // All columns maintain a sizing weight, and they will occupy all available width.
        static ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody;
        PushStyleCompact();
        HvkGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags, ImGuiTableFlags_Resizable);
        HvkGui::CheckboxFlags("ImGuiTableFlags_BordersV", &flags, ImGuiTableFlags_BordersV);
        HvkGui::SameLine(); HelpMarker(
            "Using the _Resizable flag automatically enables the _BordersInnerV flag as well, "
            "this is why the resize borders are still showing when unchecking this.");
        PopStyleCompact();

        if (HvkGui::BeginTable("table1", 3, flags))
        {
            for (int row = 0; row < 5; row++)
            {
                HvkGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    HvkGui::TableSetColumnIndex(column);
                    HvkGui::Text("Hello %d,%d", column, row);
                }
            }
            HvkGui::EndTable();
        }
        HvkGui::TreePop();
    }

    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Resizable, fixed");
    if (HvkGui::TreeNode("Resizable, fixed"))
    {
        // Here we use ImGuiTableFlags_SizingFixedFit (even though _ScrollX is not set)
        // So columns will adopt the "Fixed" policy and will maintain a fixed width regardless of the whole available width (unless table is small)
        // If there is not enough available width to fit all columns, they will however be resized down.
        // FIXME-TABLE: Providing a stretch-on-init would make sense especially for tables which don't have saved settings
        HelpMarker(
            "Using _Resizable + _SizingFixedFit flags.\n"
            "Fixed-width columns generally makes more sense if you want to use horizontal scrolling.\n\n"
            "Double-click a column border to auto-fit the column to its contents.");
        PushStyleCompact();
        static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody;
        HvkGui::CheckboxFlags("ImGuiTableFlags_NoHostExtendX", &flags, ImGuiTableFlags_NoHostExtendX);
        PopStyleCompact();

        if (HvkGui::BeginTable("table1", 3, flags))
        {
            for (int row = 0; row < 5; row++)
            {
                HvkGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    HvkGui::TableSetColumnIndex(column);
                    HvkGui::Text("Hello %d,%d", column, row);
                }
            }
            HvkGui::EndTable();
        }
        HvkGui::TreePop();
    }

    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Resizable, mixed");
    if (HvkGui::TreeNode("Resizable, mixed"))
    {
        HelpMarker(
            "Using TableSetupColumn() to alter resizing policy on a per-column basis.\n\n"
            "When combining Fixed and Stretch columns, generally you only want one, maybe two trailing columns to use _WidthStretch.");
        static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

        if (HvkGui::BeginTable("table1", 3, flags))
        {
            HvkGui::TableSetupColumn("AAA", ImGuiTableColumnFlags_WidthFixed);
            HvkGui::TableSetupColumn("BBB", ImGuiTableColumnFlags_WidthFixed);
            HvkGui::TableSetupColumn("CCC", ImGuiTableColumnFlags_WidthStretch);
            HvkGui::TableHeadersRow();
            for (int row = 0; row < 5; row++)
            {
                HvkGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    HvkGui::TableSetColumnIndex(column);
                    HvkGui::Text("%s %d,%d", (column == 2) ? "Stretch" : "Fixed", column, row);
                }
            }
            HvkGui::EndTable();
        }
        if (HvkGui::BeginTable("table2", 6, flags))
        {
            HvkGui::TableSetupColumn("AAA", ImGuiTableColumnFlags_WidthFixed);
            HvkGui::TableSetupColumn("BBB", ImGuiTableColumnFlags_WidthFixed);
            HvkGui::TableSetupColumn("CCC", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultHide);
            HvkGui::TableSetupColumn("DDD", ImGuiTableColumnFlags_WidthStretch);
            HvkGui::TableSetupColumn("EEE", ImGuiTableColumnFlags_WidthStretch);
            HvkGui::TableSetupColumn("FFF", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_DefaultHide);
            HvkGui::TableHeadersRow();
            for (int row = 0; row < 5; row++)
            {
                HvkGui::TableNextRow();
                for (int column = 0; column < 6; column++)
                {
                    HvkGui::TableSetColumnIndex(column);
                    HvkGui::Text("%s %d,%d", (column >= 3) ? "Stretch" : "Fixed", column, row);
                }
            }
            HvkGui::EndTable();
        }
        HvkGui::TreePop();
    }

    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Reorderable, hideable, with headers");
    if (HvkGui::TreeNode("Reorderable, hideable, with headers"))
    {
        HelpMarker(
            "Click and drag column headers to reorder columns.\n\n"
            "Right-click on a header to open a context menu.");
        static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV;
        PushStyleCompact();
        HvkGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags, ImGuiTableFlags_Resizable);
        HvkGui::CheckboxFlags("ImGuiTableFlags_Reorderable", &flags, ImGuiTableFlags_Reorderable);
        HvkGui::CheckboxFlags("ImGuiTableFlags_Hideable", &flags, ImGuiTableFlags_Hideable);
        HvkGui::CheckboxFlags("ImGuiTableFlags_NoBordersInBody", &flags, ImGuiTableFlags_NoBordersInBody);
        HvkGui::CheckboxFlags("ImGuiTableFlags_NoBordersInBodyUntilResize", &flags, ImGuiTableFlags_NoBordersInBodyUntilResize); HvkGui::SameLine(); HelpMarker("Disable vertical borders in columns Body until hovered for resize (borders will always appear in Headers)");
        HvkGui::CheckboxFlags("ImGuiTableFlags_HighlightHoveredColumn", &flags, ImGuiTableFlags_HighlightHoveredColumn);
        PopStyleCompact();

        if (HvkGui::BeginTable("table1", 3, flags))
        {
            // Submit columns name with TableSetupColumn() and call TableHeadersRow() to create a row with a header in each column.
            // (Later we will show how TableSetupColumn() has other uses, optional flags, sizing weight etc.)
            HvkGui::TableSetupColumn("One");
            HvkGui::TableSetupColumn("Two");
            HvkGui::TableSetupColumn("Three");
            HvkGui::TableHeadersRow();
            for (int row = 0; row < 6; row++)
            {
                HvkGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    HvkGui::TableSetColumnIndex(column);
                    HvkGui::Text("Hello %d,%d", column, row);
                }
            }
            HvkGui::EndTable();
        }

        // Use outer_size.x == 0.0f instead of default to make the table as tight as possible
        // (only valid when no scrolling and no stretch column)
        if (HvkGui::BeginTable("table2", 3, flags | ImGuiTableFlags_SizingFixedFit, ImVec2(0.0f, 0.0f)))
        {
            HvkGui::TableSetupColumn("One");
            HvkGui::TableSetupColumn("Two");
            HvkGui::TableSetupColumn("Three");
            HvkGui::TableHeadersRow();
            for (int row = 0; row < 6; row++)
            {
                HvkGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    HvkGui::TableSetColumnIndex(column);
                    HvkGui::Text("Fixed %d,%d", column, row);
                }
            }
            HvkGui::EndTable();
        }
        HvkGui::TreePop();
    }

    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Padding");
    if (HvkGui::TreeNode("Padding"))
    {
        // First example: showcase use of padding flags and effect of BorderOuterV/BorderInnerV on X padding.
        // We don't expose BorderOuterH/BorderInnerH here because they have no effect on X padding.
        HelpMarker(
            "We often want outer padding activated when any using features which makes the edges of a column visible:\n"
            "e.g.:\n"
            "- BorderOuterV\n"
            "- any form of row selection\n"
            "Because of this, activating BorderOuterV sets the default to PadOuterX. "
            "Using PadOuterX or NoPadOuterX you can override the default.\n\n"
            "Actual padding values are using style.CellPadding.\n\n"
            "In this demo we don't show horizontal borders to emphasize how they don't affect default horizontal padding.");

        static ImGuiTableFlags flags1 = ImGuiTableFlags_BordersV;
        PushStyleCompact();
        HvkGui::CheckboxFlags("ImGuiTableFlags_PadOuterX", &flags1, ImGuiTableFlags_PadOuterX);
        HvkGui::SameLine(); HelpMarker("Enable outer-most padding (default if ImGuiTableFlags_BordersOuterV is set)");
        HvkGui::CheckboxFlags("ImGuiTableFlags_NoPadOuterX", &flags1, ImGuiTableFlags_NoPadOuterX);
        HvkGui::SameLine(); HelpMarker("Disable outer-most padding (default if ImGuiTableFlags_BordersOuterV is not set)");
        HvkGui::CheckboxFlags("ImGuiTableFlags_NoPadInnerX", &flags1, ImGuiTableFlags_NoPadInnerX);
        HvkGui::SameLine(); HelpMarker("Disable inner padding between columns (double inner padding if BordersOuterV is on, single inner padding if BordersOuterV is off)");
        HvkGui::CheckboxFlags("ImGuiTableFlags_BordersOuterV", &flags1, ImGuiTableFlags_BordersOuterV);
        HvkGui::CheckboxFlags("ImGuiTableFlags_BordersInnerV", &flags1, ImGuiTableFlags_BordersInnerV);
        static bool show_headers = false;
        HvkGui::Checkbox("show_headers", &show_headers);
        PopStyleCompact();

        if (HvkGui::BeginTable("table_padding", 3, flags1))
        {
            if (show_headers)
            {
                HvkGui::TableSetupColumn("One");
                HvkGui::TableSetupColumn("Two");
                HvkGui::TableSetupColumn("Three");
                HvkGui::TableHeadersRow();
            }

            for (int row = 0; row < 5; row++)
            {
                HvkGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    HvkGui::TableSetColumnIndex(column);
                    if (row == 0)
                    {
                        HvkGui::Text("Avail %.2f", HvkGui::GetContentRegionAvail().x);
                    }
                    else
                    {
                        char buf[32];
                        sprintf(buf, "Hello %d,%d", column, row);
                        HvkGui::Button(buf, ImVec2(-FLT_MIN, 0.0f));
                    }
                    //if (HvkGui::TableGetColumnFlags() & ImGuiTableColumnFlags_IsHovered)
                    //    HvkGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(0, 100, 0, 255));
                }
            }
            HvkGui::EndTable();
        }

        // Second example: set style.CellPadding to (0.0) or a custom value.
        // FIXME-TABLE: Vertical border effectively not displayed the same way as horizontal one...
        HelpMarker("Setting style.CellPadding to (0,0) or a custom value.");
        static ImGuiTableFlags flags2 = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
        static ImVec2 cell_padding(0.0f, 0.0f);
        static bool show_widget_frame_bg = true;

        PushStyleCompact();
        HvkGui::CheckboxFlags("ImGuiTableFlags_Borders", &flags2, ImGuiTableFlags_Borders);
        HvkGui::CheckboxFlags("ImGuiTableFlags_BordersH", &flags2, ImGuiTableFlags_BordersH);
        HvkGui::CheckboxFlags("ImGuiTableFlags_BordersV", &flags2, ImGuiTableFlags_BordersV);
        HvkGui::CheckboxFlags("ImGuiTableFlags_BordersInner", &flags2, ImGuiTableFlags_BordersInner);
        HvkGui::CheckboxFlags("ImGuiTableFlags_BordersOuter", &flags2, ImGuiTableFlags_BordersOuter);
        HvkGui::CheckboxFlags("ImGuiTableFlags_RowBg", &flags2, ImGuiTableFlags_RowBg);
        HvkGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags2, ImGuiTableFlags_Resizable);
        HvkGui::Checkbox("show_widget_frame_bg", &show_widget_frame_bg);
        HvkGui::SliderFloat2("CellPadding", &cell_padding.x, 0.0f, 10.0f, "%.0f");
        PopStyleCompact();

        HvkGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);
        if (HvkGui::BeginTable("table_padding_2", 3, flags2))
        {
            static char text_bufs[3 * 5][16]; // Mini text storage for 3x5 cells
            static bool init = true;
            if (!show_widget_frame_bg)
                HvkGui::PushStyleColor(ImGuiCol_FrameBg, 0);
            for (int cell = 0; cell < 3 * 5; cell++)
            {
                HvkGui::TableNextColumn();
                if (init)
                    strcpy(text_bufs[cell], "edit me");
                HvkGui::SetNextItemWidth(-FLT_MIN);
                HvkGui::PushID(cell);
                HvkGui::InputText("##cell", text_bufs[cell], IM_ARRAYSIZE(text_bufs[cell]));
                HvkGui::PopID();
            }
            if (!show_widget_frame_bg)
                HvkGui::PopStyleColor();
            init = false;
            HvkGui::EndTable();
        }
        HvkGui::PopStyleVar();

        HvkGui::TreePop();
    }

    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Explicit widths");
    if (HvkGui::TreeNode("Sizing policies"))
    {
        static ImGuiTableFlags flags1 = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_RowBg | ImGuiTableFlags_ContextMenuInBody;
        PushStyleCompact();
        HvkGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags1, ImGuiTableFlags_Resizable);
        HvkGui::CheckboxFlags("ImGuiTableFlags_NoHostExtendX", &flags1, ImGuiTableFlags_NoHostExtendX);
        PopStyleCompact();

        static ImGuiTableFlags sizing_policy_flags[4] = { ImGuiTableFlags_SizingFixedFit, ImGuiTableFlags_SizingFixedSame, ImGuiTableFlags_SizingStretchProp, ImGuiTableFlags_SizingStretchSame };
        for (int table_n = 0; table_n < 4; table_n++)
        {
            HvkGui::PushID(table_n);
            HvkGui::SetNextItemWidth(TEXT_BASE_WIDTH * 30);
            EditTableSizingFlags(&sizing_policy_flags[table_n]);

            // To make it easier to understand the different sizing policy,
            // For each policy: we display one table where the columns have equal contents width,
            // and one where the columns have different contents width.
            if (HvkGui::BeginTable("table1", 3, sizing_policy_flags[table_n] | flags1))
            {
                for (int row = 0; row < 3; row++)
                {
                    HvkGui::TableNextRow();
                    HvkGui::TableNextColumn(); HvkGui::Text("Oh dear");
                    HvkGui::TableNextColumn(); HvkGui::Text("Oh dear");
                    HvkGui::TableNextColumn(); HvkGui::Text("Oh dear");
                }
                HvkGui::EndTable();
            }
            if (HvkGui::BeginTable("table2", 3, sizing_policy_flags[table_n] | flags1))
            {
                for (int row = 0; row < 3; row++)
                {
                    HvkGui::TableNextRow();
                    HvkGui::TableNextColumn(); HvkGui::Text("AAAA");
                    HvkGui::TableNextColumn(); HvkGui::Text("BBBBBBBB");
                    HvkGui::TableNextColumn(); HvkGui::Text("CCCCCCCCCCCC");
                }
                HvkGui::EndTable();
            }
            HvkGui::PopID();
        }

        HvkGui::Spacing();
        HvkGui::TextUnformatted("Advanced");
        HvkGui::SameLine();
        HelpMarker(
            "This section allows you to interact and see the effect of various sizing policies "
            "depending on whether Scroll is enabled and the contents of your columns.");

        enum ContentsType { CT_ShowWidth, CT_ShortText, CT_LongText, CT_Button, CT_FillButton, CT_InputText };
        static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;
        static int contents_type = CT_ShowWidth;
        static int column_count = 3;

        PushStyleCompact();
        HvkGui::PushID("Advanced");
        HvkGui::PushItemWidth(TEXT_BASE_WIDTH * 30);
        EditTableSizingFlags(&flags);
        HvkGui::Combo("Contents", &contents_type, "Show width\0Short Text\0Long Text\0Button\0Fill Button\0InputText\0");
        if (contents_type == CT_FillButton)
        {
            HvkGui::SameLine();
            HelpMarker(
                "Be mindful that using right-alignment (e.g. size.x = -FLT_MIN) creates a feedback loop "
                "where contents width can feed into auto-column width can feed into contents width.");
        }
        HvkGui::DragInt("Columns", &column_count, 0.1f, 1, 64, "%d", ImGuiSliderFlags_AlwaysClamp);
        HvkGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags, ImGuiTableFlags_Resizable);
        HvkGui::CheckboxFlags("ImGuiTableFlags_PreciseWidths", &flags, ImGuiTableFlags_PreciseWidths);
        HvkGui::SameLine(); HelpMarker("Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: Without this flag: 33,33,34. With this flag: 33,33,33). With larger number of columns, resizing will appear to be less smooth.");
        HvkGui::CheckboxFlags("ImGuiTableFlags_ScrollX", &flags, ImGuiTableFlags_ScrollX);
        HvkGui::CheckboxFlags("ImGuiTableFlags_ScrollY", &flags, ImGuiTableFlags_ScrollY);
        HvkGui::CheckboxFlags("ImGuiTableFlags_NoClip", &flags, ImGuiTableFlags_NoClip);
        HvkGui::PopItemWidth();
        HvkGui::PopID();
        PopStyleCompact();

        if (HvkGui::BeginTable("table2", column_count, flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * 7)))
        {
            for (int cell = 0; cell < 10 * column_count; cell++)
            {
                HvkGui::TableNextColumn();
                int column = HvkGui::TableGetColumnIndex();
                int row = HvkGui::TableGetRowIndex();

                HvkGui::PushID(cell);
                char label[32];
                static char text_buf[32] = "";
                sprintf(label, "Hello %d,%d", column, row);
                switch (contents_type)
                {
                case CT_ShortText:  HvkGui::TextUnformatted(label); break;
                case CT_LongText:   HvkGui::Text("Some %s text %d,%d\nOver two lines..", column == 0 ? "long" : "longeeer", column, row); break;
                case CT_ShowWidth:  HvkGui::Text("W: %.1f", HvkGui::GetContentRegionAvail().x); break;
                case CT_Button:     HvkGui::Button(label); break;
                case CT_FillButton: HvkGui::Button(label, ImVec2(-FLT_MIN, 0.0f)); break;
                case CT_InputText:  HvkGui::SetNextItemWidth(-FLT_MIN); HvkGui::InputText("##", text_buf, IM_ARRAYSIZE(text_buf)); break;
                }
                HvkGui::PopID();
            }
            HvkGui::EndTable();
        }
        HvkGui::TreePop();
    }

    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Vertical scrolling, with clipping");
    if (HvkGui::TreeNode("Vertical scrolling, with clipping"))
    {
        HelpMarker(
            "Here we activate ScrollY, which will create a child window container to allow hosting scrollable contents.\n\n"
            "We also demonstrate using ImGuiListClipper to virtualize the submission of many items.");
        static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

        PushStyleCompact();
        HvkGui::CheckboxFlags("ImGuiTableFlags_ScrollY", &flags, ImGuiTableFlags_ScrollY);
        PopStyleCompact();

        // When using ScrollX or ScrollY we need to specify a size for our table container!
        // Otherwise by default the table will fit all available space, like a BeginChild() call.
        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
        if (HvkGui::BeginTable("table_scrolly", 3, flags, outer_size))
        {
            HvkGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
            HvkGui::TableSetupColumn("One", ImGuiTableColumnFlags_None);
            HvkGui::TableSetupColumn("Two", ImGuiTableColumnFlags_None);
            HvkGui::TableSetupColumn("Three", ImGuiTableColumnFlags_None);
            HvkGui::TableHeadersRow();

            // Demonstrate using clipper for large vertical lists
            ImGuiListClipper clipper;
            clipper.Begin(1000);
            while (clipper.Step())
            {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
                {
                    HvkGui::TableNextRow();
                    for (int column = 0; column < 3; column++)
                    {
                        HvkGui::TableSetColumnIndex(column);
                        HvkGui::Text("Hello %d,%d", column, row);
                    }
                }
            }
            HvkGui::EndTable();
        }
        HvkGui::TreePop();
    }

    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Horizontal scrolling");
    if (HvkGui::TreeNode("Horizontal scrolling"))
    {
        HelpMarker(
            "When ScrollX is enabled, the default sizing policy becomes ImGuiTableFlags_SizingFixedFit, "
            "as automatically stretching columns doesn't make much sense with horizontal scrolling.\n\n"
            "Also note that as of the current version, you will almost always want to enable ScrollY along with ScrollX, "
            "because the container window won't automatically extend vertically to fix contents "
            "(this may be improved in future versions).");
        static ImGuiTableFlags flags = ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
        static int freeze_cols = 1;
        static int freeze_rows = 1;

        PushStyleCompact();
        HvkGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags, ImGuiTableFlags_Resizable);
        HvkGui::CheckboxFlags("ImGuiTableFlags_ScrollX", &flags, ImGuiTableFlags_ScrollX);
        HvkGui::CheckboxFlags("ImGuiTableFlags_ScrollY", &flags, ImGuiTableFlags_ScrollY);
        HvkGui::SetNextItemWidth(HvkGui::GetFrameHeight());
        HvkGui::DragInt("freeze_cols", &freeze_cols, 0.2f, 0, 9, NULL, ImGuiSliderFlags_NoInput);
        HvkGui::SetNextItemWidth(HvkGui::GetFrameHeight());
        HvkGui::DragInt("freeze_rows", &freeze_rows, 0.2f, 0, 9, NULL, ImGuiSliderFlags_NoInput);
        PopStyleCompact();

        // When using ScrollX or ScrollY we need to specify a size for our table container!
        // Otherwise by default the table will fit all available space, like a BeginChild() call.
        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
        if (HvkGui::BeginTable("table_scrollx", 7, flags, outer_size))
        {
            HvkGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
            HvkGui::TableSetupColumn("Line #", ImGuiTableColumnFlags_NoHide); // Make the first column not hideable to match our use of TableSetupScrollFreeze()
            HvkGui::TableSetupColumn("One");
            HvkGui::TableSetupColumn("Two");
            HvkGui::TableSetupColumn("Three");
            HvkGui::TableSetupColumn("Four");
            HvkGui::TableSetupColumn("Five");
            HvkGui::TableSetupColumn("Six");
            HvkGui::TableHeadersRow();
            for (int row = 0; row < 20; row++)
            {
                HvkGui::TableNextRow();
                for (int column = 0; column < 7; column++)
                {
                    // Both TableNextColumn() and TableSetColumnIndex() return true when a column is visible or performing width measurement.
                    // Because here we know that:
                    // - A) all our columns are contributing the same to row height
                    // - B) column 0 is always visible,
                    // We only always submit this one column and can skip others.
                    // More advanced per-column clipping behaviors may benefit from polling the status flags via TableGetColumnFlags().
                    if (!HvkGui::TableSetColumnIndex(column) && column > 0)
                        continue;
                    if (column == 0)
                        HvkGui::Text("Line %d", row);
                    else
                        HvkGui::Text("Hello world %d,%d", column, row);
                }
            }
            HvkGui::EndTable();
        }

        HvkGui::Spacing();
        HvkGui::TextUnformatted("Stretch + ScrollX");
        HvkGui::SameLine();
        HelpMarker(
            "Showcase using Stretch columns + ScrollX together: "
            "this is rather unusual and only makes sense when specifying an 'inner_width' for the table!\n"
            "Without an explicit value, inner_width is == outer_size.x and therefore using Stretch columns "
            "along with ScrollX doesn't make sense.");
        static ImGuiTableFlags flags2 = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_RowBg | ImGuiTableFlags_ContextMenuInBody;
        static float inner_width = 1000.0f;
        PushStyleCompact();
        HvkGui::PushID("flags3");
        HvkGui::PushItemWidth(TEXT_BASE_WIDTH * 30);
        HvkGui::CheckboxFlags("ImGuiTableFlags_ScrollX", &flags2, ImGuiTableFlags_ScrollX);
        HvkGui::DragFloat("inner_width", &inner_width, 1.0f, 0.0f, FLT_MAX, "%.1f");
        HvkGui::PopItemWidth();
        HvkGui::PopID();
        PopStyleCompact();
        if (HvkGui::BeginTable("table2", 7, flags2, outer_size, inner_width))
        {
            for (int cell = 0; cell < 20 * 7; cell++)
            {
                HvkGui::TableNextColumn();
                HvkGui::Text("Hello world %d,%d", HvkGui::TableGetColumnIndex(), HvkGui::TableGetRowIndex());
            }
            HvkGui::EndTable();
        }
        HvkGui::TreePop();
    }

    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Columns flags");
    if (HvkGui::TreeNode("Columns flags"))
    {
        // Create a first table just to show all the options/flags we want to make visible in our example!
        const int column_count = 3;
        const char* column_names[column_count] = { "One", "Two", "Three" };
        static ImGuiTableColumnFlags column_flags[column_count] = { ImGuiTableColumnFlags_DefaultSort, ImGuiTableColumnFlags_None, ImGuiTableColumnFlags_DefaultHide };
        static ImGuiTableColumnFlags column_flags_out[column_count] = { 0, 0, 0 }; // Output from TableGetColumnFlags()

        if (HvkGui::BeginTable("table_columns_flags_checkboxes", column_count, ImGuiTableFlags_None))
        {
            PushStyleCompact();
            for (int column = 0; column < column_count; column++)
            {
                HvkGui::TableNextColumn();
                HvkGui::PushID(column);
                HvkGui::AlignTextToFramePadding(); // FIXME-TABLE: Workaround for wrong text baseline propagation across columns
                HvkGui::Text("'%s'", column_names[column]);
                HvkGui::Spacing();
                HvkGui::Text("Input flags:");
                EditTableColumnsFlags(&column_flags[column]);
                HvkGui::Spacing();
                HvkGui::Text("Output flags:");
                HvkGui::BeginDisabled();
                ShowTableColumnsStatusFlags(column_flags_out[column]);
                HvkGui::EndDisabled();
                HvkGui::PopID();
            }
            PopStyleCompact();
            HvkGui::EndTable();
        }

        // Create the real table we care about for the example!
        // We use a scrolling table to be able to showcase the difference between the _IsEnabled and _IsVisible flags above,
        // otherwise in a non-scrolling table columns are always visible (unless using ImGuiTableFlags_NoKeepColumnsVisible
        // + resizing the parent window down).
        const ImGuiTableFlags flags
            = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY
            | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV
            | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable;
        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 9);
        if (HvkGui::BeginTable("table_columns_flags", column_count, flags, outer_size))
        {
            bool has_angled_header = false;
            for (int column = 0; column < column_count; column++)
            {
                has_angled_header |= (column_flags[column] & ImGuiTableColumnFlags_AngledHeader) != 0;
                HvkGui::TableSetupColumn(column_names[column], column_flags[column]);
            }
            if (has_angled_header)
                HvkGui::TableAngledHeadersRow();
            HvkGui::TableHeadersRow();
            for (int column = 0; column < column_count; column++)
                column_flags_out[column] = HvkGui::TableGetColumnFlags(column);
            float indent_step = (float)((int)TEXT_BASE_WIDTH / 2);
            for (int row = 0; row < 8; row++)
            {
                // Add some indentation to demonstrate usage of per-column IndentEnable/IndentDisable flags.
                HvkGui::Indent(indent_step);
                HvkGui::TableNextRow();
                for (int column = 0; column < column_count; column++)
                {
                    HvkGui::TableSetColumnIndex(column);
                    HvkGui::Text("%s %s", (column == 0) ? "Indented" : "Hello", HvkGui::TableGetColumnName(column));
                }
            }
            HvkGui::Unindent(indent_step * 8.0f);

            HvkGui::EndTable();
        }
        HvkGui::TreePop();
    }

    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Columns widths");
    if (HvkGui::TreeNode("Columns widths"))
    {
        HelpMarker("Using TableSetupColumn() to setup default width.");

        static ImGuiTableFlags flags1 = ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBodyUntilResize;
        PushStyleCompact();
        HvkGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags1, ImGuiTableFlags_Resizable);
        HvkGui::CheckboxFlags("ImGuiTableFlags_NoBordersInBodyUntilResize", &flags1, ImGuiTableFlags_NoBordersInBodyUntilResize);
        PopStyleCompact();
        if (HvkGui::BeginTable("table1", 3, flags1))
        {
            // We could also set ImGuiTableFlags_SizingFixedFit on the table and all columns will default to ImGuiTableColumnFlags_WidthFixed.
            HvkGui::TableSetupColumn("one", ImGuiTableColumnFlags_WidthFixed, 100.0f); // Default to 100.0f
            HvkGui::TableSetupColumn("two", ImGuiTableColumnFlags_WidthFixed, 200.0f); // Default to 200.0f
            HvkGui::TableSetupColumn("three", ImGuiTableColumnFlags_WidthFixed);       // Default to auto
            HvkGui::TableHeadersRow();
            for (int row = 0; row < 4; row++)
            {
                HvkGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    HvkGui::TableSetColumnIndex(column);
                    if (row == 0)
                        HvkGui::Text("(w: %5.1f)", HvkGui::GetContentRegionAvail().x);
                    else
                        HvkGui::Text("Hello %d,%d", column, row);
                }
            }
            HvkGui::EndTable();
        }

        HelpMarker(
            "Using TableSetupColumn() to setup explicit width.\n\nUnless _NoKeepColumnsVisible is set, "
            "fixed columns with set width may still be shrunk down if there's not enough space in the host.");

        static ImGuiTableFlags flags2 = ImGuiTableFlags_None;
        PushStyleCompact();
        HvkGui::CheckboxFlags("ImGuiTableFlags_NoKeepColumnsVisible", &flags2, ImGuiTableFlags_NoKeepColumnsVisible);
        HvkGui::CheckboxFlags("ImGuiTableFlags_BordersInnerV", &flags2, ImGuiTableFlags_BordersInnerV);
        HvkGui::CheckboxFlags("ImGuiTableFlags_BordersOuterV", &flags2, ImGuiTableFlags_BordersOuterV);
        PopStyleCompact();
        if (HvkGui::BeginTable("table2", 4, flags2))
        {
            // We could also set ImGuiTableFlags_SizingFixedFit on the table and then all columns
            // will default to ImGuiTableColumnFlags_WidthFixed.
            HvkGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 100.0f);
            HvkGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 15.0f);
            HvkGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 30.0f);
            HvkGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 15.0f);
            for (int row = 0; row < 5; row++)
            {
                HvkGui::TableNextRow();
                for (int column = 0; column < 4; column++)
                {
                    HvkGui::TableSetColumnIndex(column);
                    if (row == 0)
                        HvkGui::Text("(w: %5.1f)", HvkGui::GetContentRegionAvail().x);
                    else
                        HvkGui::Text("Hello %d,%d", column, row);
                }
            }
            HvkGui::EndTable();
        }
        HvkGui::TreePop();
    }

    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Nested tables");
    if (HvkGui::TreeNode("Nested tables"))
    {
        HelpMarker("This demonstrates embedding a table into another table cell.");

        if (HvkGui::BeginTable("table_nested1", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable))
        {
            HvkGui::TableSetupColumn("A0");
            HvkGui::TableSetupColumn("A1");
            HvkGui::TableHeadersRow();

            HvkGui::TableNextColumn();
            HvkGui::Text("A0 Row 0");
            {
                float rows_height = (TEXT_BASE_HEIGHT * 2.0f) + (HvkGui::GetStyle().CellPadding.y * 2.0f);
                if (HvkGui::BeginTable("table_nested2", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable))
                {
                    HvkGui::TableSetupColumn("B0");
                    HvkGui::TableSetupColumn("B1");
                    HvkGui::TableHeadersRow();

                    HvkGui::TableNextRow(ImGuiTableRowFlags_None, rows_height);
                    HvkGui::TableNextColumn();
                    HvkGui::Text("B0 Row 0");
                    HvkGui::TableNextColumn();
                    HvkGui::Text("B1 Row 0");
                    HvkGui::TableNextRow(ImGuiTableRowFlags_None, rows_height);
                    HvkGui::TableNextColumn();
                    HvkGui::Text("B0 Row 1");
                    HvkGui::TableNextColumn();
                    HvkGui::Text("B1 Row 1");

                    HvkGui::EndTable();
                }
            }
            HvkGui::TableNextColumn(); HvkGui::Text("A1 Row 0");
            HvkGui::TableNextColumn(); HvkGui::Text("A0 Row 1");
            HvkGui::TableNextColumn(); HvkGui::Text("A1 Row 1");
            HvkGui::EndTable();
        }
        HvkGui::TreePop();
    }

    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Row height");
    if (HvkGui::TreeNode("Row height"))
    {
        HelpMarker(
            "You can pass a 'min_row_height' to TableNextRow().\n\nRows are padded with 'style.CellPadding.y' on top and bottom, "
            "so effectively the minimum row height will always be >= 'style.CellPadding.y * 2.0f'.\n\n"
            "We cannot honor a _maximum_ row height as that would require a unique clipping rectangle per row.");
        if (HvkGui::BeginTable("table_row_height", 1, ImGuiTableFlags_Borders))
        {
            for (int row = 0; row < 8; row++)
            {
                float min_row_height = (float)(int)(TEXT_BASE_HEIGHT * 0.30f * row + HvkGui::GetStyle().CellPadding.y * 2.0f);
                HvkGui::TableNextRow(ImGuiTableRowFlags_None, min_row_height);
                HvkGui::TableNextColumn();
                HvkGui::Text("min_row_height = %.2f", min_row_height);
            }
            HvkGui::EndTable();
        }

        HelpMarker(
            "Showcase using SameLine(0,0) to share Current Line Height between cells.\n\n"
            "Please note that Tables Row Height is not the same thing as Current Line Height, "
            "as a table cell may contains multiple lines.");
        if (HvkGui::BeginTable("table_share_lineheight", 2, ImGuiTableFlags_Borders))
        {
            HvkGui::TableNextRow();
            HvkGui::TableNextColumn();
            HvkGui::ColorButton("##1", ImVec4(0.13f, 0.26f, 0.40f, 1.0f), ImGuiColorEditFlags_None, ImVec2(40, 40));
            HvkGui::TableNextColumn();
            HvkGui::Text("Line 1");
            HvkGui::Text("Line 2");

            HvkGui::TableNextRow();
            HvkGui::TableNextColumn();
            HvkGui::ColorButton("##2", ImVec4(0.13f, 0.26f, 0.40f, 1.0f), ImGuiColorEditFlags_None, ImVec2(40, 40));
            HvkGui::TableNextColumn();
            HvkGui::SameLine(0.0f, 0.0f); // Reuse line height from previous column
            HvkGui::Text("Line 1, with SameLine(0,0)");
            HvkGui::Text("Line 2");

            HvkGui::EndTable();
        }

        HelpMarker("Showcase altering CellPadding.y between rows. Note that CellPadding.x is locked for the entire table.");
        if (HvkGui::BeginTable("table_changing_cellpadding_y", 1, ImGuiTableFlags_Borders))
        {
            ImGuiStyle& style = HvkGui::GetStyle();
            for (int row = 0; row < 8; row++)
            {
                if ((row % 3) == 2)
                    HvkGui::PushStyleVarY(ImGuiStyleVar_CellPadding, 20.0f);
                HvkGui::TableNextRow(ImGuiTableRowFlags_None);
                HvkGui::TableNextColumn();
                HvkGui::Text("CellPadding.y = %.2f", style.CellPadding.y);
                if ((row % 3) == 2)
                    HvkGui::PopStyleVar();
            }
            HvkGui::EndTable();
        }

        HvkGui::TreePop();
    }

    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Outer size");
    if (HvkGui::TreeNode("Outer size"))
    {
        // Showcasing use of ImGuiTableFlags_NoHostExtendX and ImGuiTableFlags_NoHostExtendY
        // Important to that note how the two flags have slightly different behaviors!
        HvkGui::Text("Using NoHostExtendX and NoHostExtendY:");
        PushStyleCompact();
        static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX;
        HvkGui::CheckboxFlags("ImGuiTableFlags_NoHostExtendX", &flags, ImGuiTableFlags_NoHostExtendX);
        HvkGui::SameLine(); HelpMarker("Make outer width auto-fit to columns, overriding outer_size.x value.\n\nOnly available when ScrollX/ScrollY are disabled and Stretch columns are not used.");
        HvkGui::CheckboxFlags("ImGuiTableFlags_NoHostExtendY", &flags, ImGuiTableFlags_NoHostExtendY);
        HvkGui::SameLine(); HelpMarker("Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit).\n\nOnly available when ScrollX/ScrollY are disabled. Data below the limit will be clipped and not visible.");
        PopStyleCompact();

        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 5.5f);
        if (HvkGui::BeginTable("table1", 3, flags, outer_size))
        {
            for (int row = 0; row < 10; row++)
            {
                HvkGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    HvkGui::TableNextColumn();
                    HvkGui::Text("Cell %d,%d", column, row);
                }
            }
            HvkGui::EndTable();
        }
        HvkGui::SameLine();
        HvkGui::Text("Hello!");

        HvkGui::Spacing();

        HvkGui::Text("Using explicit size:");
        if (HvkGui::BeginTable("table2", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg, ImVec2(TEXT_BASE_WIDTH * 30, 0.0f)))
        {
            for (int row = 0; row < 5; row++)
            {
                HvkGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    HvkGui::TableNextColumn();
                    HvkGui::Text("Cell %d,%d", column, row);
                }
            }
            HvkGui::EndTable();
        }
        HvkGui::SameLine();
        if (HvkGui::BeginTable("table3", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg, ImVec2(TEXT_BASE_WIDTH * 30, 0.0f)))
        {
            const float rows_height = TEXT_BASE_HEIGHT * 1.5f + HvkGui::GetStyle().CellPadding.y * 2.0f;
            for (int row = 0; row < 3; row++)
            {
                HvkGui::TableNextRow(0, rows_height);
                for (int column = 0; column < 3; column++)
                {
                    HvkGui::TableNextColumn();
                    HvkGui::Text("Cell %d,%d", column, row);
                }
            }
            HvkGui::EndTable();
        }

        HvkGui::TreePop();
    }

    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Background color");
    if (HvkGui::TreeNode("Background color"))
    {
        static ImGuiTableFlags flags = ImGuiTableFlags_RowBg;
        static int row_bg_type = 1;
        static int row_bg_target = 1;
        static int cell_bg_type = 1;

        PushStyleCompact();
        HvkGui::CheckboxFlags("ImGuiTableFlags_Borders", &flags, ImGuiTableFlags_Borders);
        HvkGui::CheckboxFlags("ImGuiTableFlags_RowBg", &flags, ImGuiTableFlags_RowBg);
        HvkGui::SameLine(); HelpMarker("ImGuiTableFlags_RowBg automatically sets RowBg0 to alternative colors pulled from the Style.");
        HvkGui::Combo("row bg type", (int*)&row_bg_type, "None\0Red\0Gradient\0");
        HvkGui::Combo("row bg target", (int*)&row_bg_target, "RowBg0\0RowBg1\0"); HvkGui::SameLine(); HelpMarker("Target RowBg0 to override the alternating odd/even colors,\nTarget RowBg1 to blend with them.");
        HvkGui::Combo("cell bg type", (int*)&cell_bg_type, "None\0Blue\0"); HvkGui::SameLine(); HelpMarker("We are colorizing cells to B1->C2 here.");
        IM_ASSERT(row_bg_type >= 0 && row_bg_type <= 2);
        IM_ASSERT(row_bg_target >= 0 && row_bg_target <= 1);
        IM_ASSERT(cell_bg_type >= 0 && cell_bg_type <= 1);
        PopStyleCompact();

        if (HvkGui::BeginTable("table1", 5, flags))
        {
            for (int row = 0; row < 6; row++)
            {
                HvkGui::TableNextRow();

                // Demonstrate setting a row background color with 'HvkGui::TableSetBgColor(ImGuiTableBgTarget_RowBgX, ...)'
                // We use a transparent color so we can see the one behind in case our target is RowBg1 and RowBg0 was already targeted by the ImGuiTableFlags_RowBg flag.
                if (row_bg_type != 0)
                {
                    ImU32 row_bg_color = HvkGui::GetColorU32(row_bg_type == 1 ? ImVec4(0.7f, 0.3f, 0.3f, 0.65f) : ImVec4(0.2f + row * 0.1f, 0.2f, 0.2f, 0.65f)); // Flat or Gradient?
                    HvkGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0 + row_bg_target, row_bg_color);
                }

                // Fill cells
                for (int column = 0; column < 5; column++)
                {
                    HvkGui::TableSetColumnIndex(column);
                    HvkGui::Text("%c%c", 'A' + row, '0' + column);

                    // Change background of Cells B1->C2
                    // Demonstrate setting a cell background color with 'HvkGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ...)'
                    // (the CellBg color will be blended over the RowBg and ColumnBg colors)
                    // We can also pass a column number as a third parameter to TableSetBgColor() and do this outside the column loop.
                    if (row >= 1 && row <= 2 && column >= 1 && column <= 2 && cell_bg_type == 1)
                    {
                        ImU32 cell_bg_color = HvkGui::GetColorU32(ImVec4(0.3f, 0.3f, 0.7f, 0.65f));
                        HvkGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                    }
                }
            }
            HvkGui::EndTable();
        }
        HvkGui::TreePop();
    }

    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Tree view");
    if (HvkGui::TreeNode("Tree view"))
    {
        static ImGuiTableFlags table_flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

        static ImGuiTreeNodeFlags tree_node_flags_base = ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_DrawLinesFull;
        HvkGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanFullWidth",  &tree_node_flags_base, ImGuiTreeNodeFlags_SpanFullWidth);
        HvkGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanLabelWidth",  &tree_node_flags_base, ImGuiTreeNodeFlags_SpanLabelWidth);
        HvkGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanAllColumns", &tree_node_flags_base, ImGuiTreeNodeFlags_SpanAllColumns);
        HvkGui::CheckboxFlags("ImGuiTreeNodeFlags_LabelSpanAllColumns", &tree_node_flags_base, ImGuiTreeNodeFlags_LabelSpanAllColumns);
        HvkGui::SameLine(); HelpMarker("Useful if you know that you aren't displaying contents in other columns");

        HelpMarker("See \"Columns flags\" section to configure how indentation is applied to individual columns.");
        if (HvkGui::BeginTable("3ways", 3, table_flags))
        {
            // The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
            HvkGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
            HvkGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
            HvkGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
            HvkGui::TableHeadersRow();

            // Simple storage to output a dummy file-system.
            struct MyTreeNode
            {
                const char*     Name;
                const char*     Type;
                int             Size;
                int             ChildIdx;
                int             ChildCount;
                static void DisplayNode(const MyTreeNode* node, const MyTreeNode* all_nodes)
                {
                    HvkGui::TableNextRow();
                    HvkGui::TableNextColumn();
                    const bool is_folder = (node->ChildCount > 0);

                    ImGuiTreeNodeFlags node_flags = tree_node_flags_base;
                    if (node != &all_nodes[0])
                        node_flags &= ~ImGuiTreeNodeFlags_LabelSpanAllColumns; // Only demonstrate this on the root node.

                    if (is_folder)
                    {
                        bool open = HvkGui::TreeNodeEx(node->Name, node_flags);
                        if ((node_flags & ImGuiTreeNodeFlags_LabelSpanAllColumns) == 0)
                        {
                            HvkGui::TableNextColumn();
                            HvkGui::TextDisabled("--");
                            HvkGui::TableNextColumn();
                            HvkGui::TextUnformatted(node->Type);
                        }
                        if (open)
                        {
                            for (int child_n = 0; child_n < node->ChildCount; child_n++)
                                DisplayNode(&all_nodes[node->ChildIdx + child_n], all_nodes);
                            HvkGui::TreePop();
                        }
                    }
                    else
                    {
                        HvkGui::TreeNodeEx(node->Name, node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen);
                        HvkGui::TableNextColumn();
                        HvkGui::Text("%d", node->Size);
                        HvkGui::TableNextColumn();
                        HvkGui::TextUnformatted(node->Type);
                    }
                }
            };
            static const MyTreeNode nodes[] =
            {
                { "Root with Long Name",          "Folder",       -1,       1, 3    }, // 0
                { "Music",                        "Folder",       -1,       4, 2    }, // 1
                { "Textures",                     "Folder",       -1,       6, 3    }, // 2
                { "desktop.ini",                  "System file",  1024,    -1,-1    }, // 3
                { "File1_a.wav",                  "Audio file",   123000,  -1,-1    }, // 4
                { "File1_b.wav",                  "Audio file",   456000,  -1,-1    }, // 5
                { "Image001.png",                 "Image file",   203128,  -1,-1    }, // 6
                { "Copy of Image001.png",         "Image file",   203256,  -1,-1    }, // 7
                { "Copy of Image001 (Final2).png","Image file",   203512,  -1,-1    }, // 8
            };

            MyTreeNode::DisplayNode(&nodes[0], nodes);

            HvkGui::EndTable();
        }
        HvkGui::TreePop();
    }

    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Item width");
    if (HvkGui::TreeNode("Item width"))
    {
        HelpMarker(
            "Showcase using PushItemWidth() and how it is preserved on a per-column basis.\n\n"
            "Note that on auto-resizing non-resizable fixed columns, querying the content width for "
            "e.g. right-alignment doesn't make sense.");
        if (HvkGui::BeginTable("table_item_width", 3, ImGuiTableFlags_Borders))
        {
            HvkGui::TableSetupColumn("small");
            HvkGui::TableSetupColumn("half");
            HvkGui::TableSetupColumn("right-align");
            HvkGui::TableHeadersRow();

            for (int row = 0; row < 3; row++)
            {
                HvkGui::TableNextRow();
                if (row == 0)
                {
                    // Setup ItemWidth once (instead of setting up every time, which is also possible but less efficient)
                    HvkGui::TableSetColumnIndex(0);
                    HvkGui::PushItemWidth(TEXT_BASE_WIDTH * 3.0f); // Small
                    HvkGui::TableSetColumnIndex(1);
                    HvkGui::PushItemWidth(-HvkGui::GetContentRegionAvail().x * 0.5f);
                    HvkGui::TableSetColumnIndex(2);
                    HvkGui::PushItemWidth(-FLT_MIN); // Right-aligned
                }

                // Draw our contents
                static float dummy_f = 0.0f;
                HvkGui::PushID(row);
                HvkGui::TableSetColumnIndex(0);
                HvkGui::SliderFloat("float0", &dummy_f, 0.0f, 1.0f);
                HvkGui::TableSetColumnIndex(1);
                HvkGui::SliderFloat("float1", &dummy_f, 0.0f, 1.0f);
                HvkGui::TableSetColumnIndex(2);
                HvkGui::SliderFloat("##float2", &dummy_f, 0.0f, 1.0f); // No visible label since right-aligned
                HvkGui::PopID();
            }
            HvkGui::EndTable();
        }
        HvkGui::TreePop();
    }

    // Demonstrate using TableHeader() calls instead of TableHeadersRow()
    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Custom headers");
    if (HvkGui::TreeNode("Custom headers"))
    {
        const int COLUMNS_COUNT = 3;
        if (HvkGui::BeginTable("table_custom_headers", COLUMNS_COUNT, ImGuiTableFlags_Borders | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable))
        {
            HvkGui::TableSetupColumn("Apricot");
            HvkGui::TableSetupColumn("Banana");
            HvkGui::TableSetupColumn("Cherry");

            // Dummy entire-column selection storage
            // FIXME: It would be nice to actually demonstrate full-featured selection using those checkbox.
            static bool column_selected[3] = {};

            // Instead of calling TableHeadersRow() we'll submit custom headers ourselves.
            // (A different approach is also possible:
            //    - Specify ImGuiTableColumnFlags_NoHeaderLabel in some TableSetupColumn() call.
            //    - Call TableHeadersRow() normally. This will submit TableHeader() with no name.
            //    - Then call TableSetColumnIndex() to position yourself in the column and submit your stuff e.g. Checkbox().)
            HvkGui::TableNextRow(ImGuiTableRowFlags_Headers);
            for (int column = 0; column < COLUMNS_COUNT; column++)
            {
                HvkGui::TableSetColumnIndex(column);
                const char* column_name = HvkGui::TableGetColumnName(column); // Retrieve name passed to TableSetupColumn()
                HvkGui::PushID(column);
                HvkGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                HvkGui::Checkbox("##checkall", &column_selected[column]);
                HvkGui::PopStyleVar();
                HvkGui::SameLine(0.0f, HvkGui::GetStyle().ItemInnerSpacing.x);
                HvkGui::TableHeader(column_name);
                HvkGui::PopID();
            }

            // Submit table contents
            for (int row = 0; row < 5; row++)
            {
                HvkGui::TableNextRow();
                for (int column = 0; column < 3; column++)
                {
                    char buf[32];
                    sprintf(buf, "Cell %d,%d", column, row);
                    HvkGui::TableSetColumnIndex(column);
                    HvkGui::Selectable(buf, column_selected[column]);
                }
            }
            HvkGui::EndTable();
        }
        HvkGui::TreePop();
    }

    // Demonstrate using ImGuiTableColumnFlags_AngledHeader flag to create angled headers
    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Angled headers");
    if (HvkGui::TreeNode("Angled headers"))
    {
        const char* column_names[] = { "Track", "cabasa", "ride", "smash", "tom-hi", "tom-mid", "tom-low", "hihat-o", "hihat-c", "snare-s", "snare-c", "clap", "rim", "kick" };
        const int columns_count = IM_ARRAYSIZE(column_names);
        const int rows_count = 12;

        static ImGuiTableFlags table_flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_Hideable | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_HighlightHoveredColumn;
        static ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_AngledHeader | ImGuiTableColumnFlags_WidthFixed;
        static bool bools[columns_count * rows_count] = {}; // Dummy storage selection storage
        static int frozen_cols = 1;
        static int frozen_rows = 2;
        HvkGui::CheckboxFlags("_ScrollX", &table_flags, ImGuiTableFlags_ScrollX);
        HvkGui::CheckboxFlags("_ScrollY", &table_flags, ImGuiTableFlags_ScrollY);
        HvkGui::CheckboxFlags("_Resizable", &table_flags, ImGuiTableFlags_Resizable);
        HvkGui::CheckboxFlags("_Sortable", &table_flags, ImGuiTableFlags_Sortable);
        HvkGui::CheckboxFlags("_NoBordersInBody", &table_flags, ImGuiTableFlags_NoBordersInBody);
        HvkGui::CheckboxFlags("_HighlightHoveredColumn", &table_flags, ImGuiTableFlags_HighlightHoveredColumn);
        HvkGui::SetNextItemWidth(HvkGui::GetFontSize() * 8);
        HvkGui::SliderInt("Frozen columns", &frozen_cols, 0, 2);
        HvkGui::SetNextItemWidth(HvkGui::GetFontSize() * 8);
        HvkGui::SliderInt("Frozen rows", &frozen_rows, 0, 2);
        HvkGui::CheckboxFlags("Disable header contributing to column width", &column_flags, ImGuiTableColumnFlags_NoHeaderWidth);

        if (HvkGui::TreeNode("Style settings"))
        {
            HvkGui::SameLine();
            HelpMarker("Giving access to some ImGuiStyle value in this demo for convenience.");
            HvkGui::SetNextItemWidth(HvkGui::GetFontSize() * 8);
            HvkGui::SliderAngle("style.TableAngledHeadersAngle", &HvkGui::GetStyle().TableAngledHeadersAngle, -50.0f, +50.0f);
            HvkGui::SetNextItemWidth(HvkGui::GetFontSize() * 8);
            HvkGui::SliderFloat2("style.TableAngledHeadersTextAlign", (float*)&HvkGui::GetStyle().TableAngledHeadersTextAlign, 0.0f, 1.0f, "%.2f");
            HvkGui::TreePop();
        }

        if (HvkGui::BeginTable("table_angled_headers", columns_count, table_flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * 12)))
        {
            HvkGui::TableSetupColumn(column_names[0], ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoReorder);
            for (int n = 1; n < columns_count; n++)
                HvkGui::TableSetupColumn(column_names[n], column_flags);
            HvkGui::TableSetupScrollFreeze(frozen_cols, frozen_rows);

            HvkGui::TableAngledHeadersRow(); // Draw angled headers for all columns with the ImGuiTableColumnFlags_AngledHeader flag.
            HvkGui::TableHeadersRow();       // Draw remaining headers and allow access to context-menu and other functions.
            for (int row = 0; row < rows_count; row++)
            {
                HvkGui::PushID(row);
                HvkGui::TableNextRow();
                HvkGui::TableSetColumnIndex(0);
                HvkGui::AlignTextToFramePadding();
                HvkGui::Text("Track %d", row);
                for (int column = 1; column < columns_count; column++)
                    if (HvkGui::TableSetColumnIndex(column))
                    {
                        HvkGui::PushID(column);
                        HvkGui::Checkbox("", &bools[row * columns_count + column]);
                        HvkGui::PopID();
                    }
                HvkGui::PopID();
            }
            HvkGui::EndTable();
        }
        HvkGui::TreePop();
    }

    // Demonstrate creating custom context menus inside columns,
    // while playing it nice with context menus provided by TableHeadersRow()/TableHeader()
    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Context menus");
    if (HvkGui::TreeNode("Context menus"))
    {
        HelpMarker(
            "By default, right-clicking over a TableHeadersRow()/TableHeader() line will open the default context-menu.\n"
            "Using ImGuiTableFlags_ContextMenuInBody we also allow right-clicking over columns body.");
        static ImGuiTableFlags flags1 = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders | ImGuiTableFlags_ContextMenuInBody;

        PushStyleCompact();
        HvkGui::CheckboxFlags("ImGuiTableFlags_ContextMenuInBody", &flags1, ImGuiTableFlags_ContextMenuInBody);
        PopStyleCompact();

        // Context Menus: first example
        // [1.1] Right-click on the TableHeadersRow() line to open the default table context menu.
        // [1.2] Right-click in columns also open the default table context menu (if ImGuiTableFlags_ContextMenuInBody is set)
        const int COLUMNS_COUNT = 3;
        if (HvkGui::BeginTable("table_context_menu", COLUMNS_COUNT, flags1))
        {
            HvkGui::TableSetupColumn("One");
            HvkGui::TableSetupColumn("Two");
            HvkGui::TableSetupColumn("Three");

            // [1.1]] Right-click on the TableHeadersRow() line to open the default table context menu.
            HvkGui::TableHeadersRow();

            // Submit dummy contents
            for (int row = 0; row < 4; row++)
            {
                HvkGui::TableNextRow();
                for (int column = 0; column < COLUMNS_COUNT; column++)
                {
                    HvkGui::TableSetColumnIndex(column);
                    HvkGui::Text("Cell %d,%d", column, row);
                }
            }
            HvkGui::EndTable();
        }

        // Context Menus: second example
        // [2.1] Right-click on the TableHeadersRow() line to open the default table context menu.
        // [2.2] Right-click on the ".." to open a custom popup
        // [2.3] Right-click in columns to open another custom popup
        HelpMarker(
            "Demonstrate mixing table context menu (over header), item context button (over button) "
            "and custom per-column context menu (over column body).");
        ImGuiTableFlags flags2 = ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;
        if (HvkGui::BeginTable("table_context_menu_2", COLUMNS_COUNT, flags2))
        {
            HvkGui::TableSetupColumn("One");
            HvkGui::TableSetupColumn("Two");
            HvkGui::TableSetupColumn("Three");

            // [2.1] Right-click on the TableHeadersRow() line to open the default table context menu.
            HvkGui::TableHeadersRow();
            for (int row = 0; row < 4; row++)
            {
                HvkGui::TableNextRow();
                for (int column = 0; column < COLUMNS_COUNT; column++)
                {
                    // Submit dummy contents
                    HvkGui::TableSetColumnIndex(column);
                    HvkGui::Text("Cell %d,%d", column, row);
                    HvkGui::SameLine();

                    // [2.2] Right-click on the ".." to open a custom popup
                    HvkGui::PushID(row * COLUMNS_COUNT + column);
                    HvkGui::SmallButton("..");
                    if (HvkGui::BeginPopupContextItem())
                    {
                        HvkGui::Text("This is the popup for Button(\"..\") in Cell %d,%d", column, row);
                        if (HvkGui::Button("Close"))
                            HvkGui::CloseCurrentPopup();
                        HvkGui::EndPopup();
                    }
                    HvkGui::PopID();
                }
            }

            // [2.3] Right-click anywhere in columns to open another custom popup
            // (instead of testing for !IsAnyItemHovered() we could also call OpenPopup() with ImGuiPopupFlags_NoOpenOverExistingPopup
            // to manage popup priority as the popups triggers, here "are we hovering a column" are overlapping)
            int hovered_column = -1;
            for (int column = 0; column < COLUMNS_COUNT + 1; column++)
            {
                HvkGui::PushID(column);
                if (HvkGui::TableGetColumnFlags(column) & ImGuiTableColumnFlags_IsHovered)
                    hovered_column = column;
                if (hovered_column == column && !HvkGui::IsAnyItemHovered() && HvkGui::IsMouseReleased(1))
                    HvkGui::OpenPopup("MyPopup");
                if (HvkGui::BeginPopup("MyPopup"))
                {
                    if (column == COLUMNS_COUNT)
                        HvkGui::Text("This is a custom popup for unused space after the last column.");
                    else
                        HvkGui::Text("This is a custom popup for Column %d", column);
                    if (HvkGui::Button("Close"))
                        HvkGui::CloseCurrentPopup();
                    HvkGui::EndPopup();
                }
                HvkGui::PopID();
            }

            HvkGui::EndTable();
            HvkGui::Text("Hovered column: %d", hovered_column);
        }
        HvkGui::TreePop();
    }

    // Demonstrate creating multiple tables with the same ID
    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Synced instances");
    if (HvkGui::TreeNode("Synced instances"))
    {
        HelpMarker("Multiple tables with the same identifier will share their settings, width, visibility, order etc.");

        static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoSavedSettings;
        HvkGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags, ImGuiTableFlags_Resizable);
        HvkGui::CheckboxFlags("ImGuiTableFlags_ScrollY", &flags, ImGuiTableFlags_ScrollY);
        HvkGui::CheckboxFlags("ImGuiTableFlags_SizingFixedFit", &flags, ImGuiTableFlags_SizingFixedFit);
        HvkGui::CheckboxFlags("ImGuiTableFlags_HighlightHoveredColumn", &flags, ImGuiTableFlags_HighlightHoveredColumn);
        for (int n = 0; n < 3; n++)
        {
            char buf[32];
            sprintf(buf, "Synced Table %d", n);
            bool open = HvkGui::CollapsingHeader(buf, ImGuiTreeNodeFlags_DefaultOpen);
            if (open && HvkGui::BeginTable("Table", 3, flags, ImVec2(0.0f, HvkGui::GetTextLineHeightWithSpacing() * 5)))
            {
                HvkGui::TableSetupColumn("One");
                HvkGui::TableSetupColumn("Two");
                HvkGui::TableSetupColumn("Three");
                HvkGui::TableHeadersRow();
                const int cell_count = (n == 1) ? 27 : 9; // Make second table have a scrollbar to verify that additional decoration is not affecting column positions.
                for (int cell = 0; cell < cell_count; cell++)
                {
                    HvkGui::TableNextColumn();
                    HvkGui::Text("this cell %d", cell);
                }
                HvkGui::EndTable();
            }
        }
        HvkGui::TreePop();
    }

    // Demonstrate using Sorting facilities
    // This is a simplified version of the "Advanced" example, where we mostly focus on the code necessary to handle sorting.
    // Note that the "Advanced" example also showcase manually triggering a sort (e.g. if item quantities have been modified)
    static const char* template_items_names[] =
    {
        "Banana", "Apple", "Cherry", "Watermelon", "Grapefruit", "Strawberry", "Mango",
        "Kiwi", "Orange", "Pineapple", "Blueberry", "Plum", "Coconut", "Pear", "Apricot"
    };
    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Sorting");
    if (HvkGui::TreeNode("Sorting"))
    {
        // Create item list
        static ImVector<MyItem> items;
        if (items.Size == 0)
        {
            items.resize(50, MyItem());
            for (int n = 0; n < items.Size; n++)
            {
                const int template_n = n % IM_ARRAYSIZE(template_items_names);
                MyItem& item = items[n];
                item.ID = n;
                item.Name = template_items_names[template_n];
                item.Quantity = (n * n - n) % 20; // Assign default quantities
            }
        }

        // Options
        static ImGuiTableFlags flags =
            ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
            | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody
            | ImGuiTableFlags_ScrollY;
        PushStyleCompact();
        HvkGui::CheckboxFlags("ImGuiTableFlags_SortMulti", &flags, ImGuiTableFlags_SortMulti);
        HvkGui::SameLine(); HelpMarker("When sorting is enabled: hold shift when clicking headers to sort on multiple column. TableGetSortSpecs() may return specs where (SpecsCount > 1).");
        HvkGui::CheckboxFlags("ImGuiTableFlags_SortTristate", &flags, ImGuiTableFlags_SortTristate);
        HvkGui::SameLine(); HelpMarker("When sorting is enabled: allow no sorting, disable default sorting. TableGetSortSpecs() may return specs where (SpecsCount == 0).");
        PopStyleCompact();

        if (HvkGui::BeginTable("table_sorting", 4, flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * 15), 0.0f))
        {
            // Declare columns
            // We use the "user_id" parameter of TableSetupColumn() to specify a user id that will be stored in the sort specifications.
            // This is so our sort function can identify a column given our own identifier. We could also identify them based on their index!
            // Demonstrate using a mixture of flags among available sort-related flags:
            // - ImGuiTableColumnFlags_DefaultSort
            // - ImGuiTableColumnFlags_NoSort / ImGuiTableColumnFlags_NoSortAscending / ImGuiTableColumnFlags_NoSortDescending
            // - ImGuiTableColumnFlags_PreferSortAscending / ImGuiTableColumnFlags_PreferSortDescending
            HvkGui::TableSetupColumn("ID",       ImGuiTableColumnFlags_DefaultSort          | ImGuiTableColumnFlags_WidthFixed,   0.0f, MyItemColumnID_ID);
            HvkGui::TableSetupColumn("Name",                                                  ImGuiTableColumnFlags_WidthFixed,   0.0f, MyItemColumnID_Name);
            HvkGui::TableSetupColumn("Action",   ImGuiTableColumnFlags_NoSort               | ImGuiTableColumnFlags_WidthFixed,   0.0f, MyItemColumnID_Action);
            HvkGui::TableSetupColumn("Quantity", ImGuiTableColumnFlags_PreferSortDescending | ImGuiTableColumnFlags_WidthStretch, 0.0f, MyItemColumnID_Quantity);
            HvkGui::TableSetupScrollFreeze(0, 1); // Make row always visible
            HvkGui::TableHeadersRow();

            // Sort our data if sort specs have been changed!
            if (ImGuiTableSortSpecs* sort_specs = HvkGui::TableGetSortSpecs())
                if (sort_specs->SpecsDirty)
                {
                    MyItem::SortWithSortSpecs(sort_specs, items.Data, items.Size);
                    sort_specs->SpecsDirty = false;
                }

            // Demonstrate using clipper for large vertical lists
            ImGuiListClipper clipper;
            clipper.Begin(items.Size);
            while (clipper.Step())
                for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
                {
                    // Display a data item
                    MyItem* item = &items[row_n];
                    HvkGui::PushID(item->ID);
                    HvkGui::TableNextRow();
                    HvkGui::TableNextColumn();
                    HvkGui::Text("%04d", item->ID);
                    HvkGui::TableNextColumn();
                    HvkGui::TextUnformatted(item->Name);
                    HvkGui::TableNextColumn();
                    HvkGui::SmallButton("None");
                    HvkGui::TableNextColumn();
                    HvkGui::Text("%d", item->Quantity);
                    HvkGui::PopID();
                }
            HvkGui::EndTable();
        }
        HvkGui::TreePop();
    }

    // In this example we'll expose most table flags and settings.
    // For specific flags and settings refer to the corresponding section for more detailed explanation.
    // This section is mostly useful to experiment with combining certain flags or settings with each others.
    //HvkGui::SetNextItemOpen(true, ImGuiCond_Once); // [DEBUG]
    if (open_action != -1)
        HvkGui::SetNextItemOpen(open_action != 0);
    IMGUI_DEMO_MARKER("Tables/Advanced");
    if (HvkGui::TreeNode("Advanced"))
    {
        static ImGuiTableFlags flags =
            ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable
            | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
            | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBody
            | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY
            | ImGuiTableFlags_SizingFixedFit;
        static ImGuiTableColumnFlags columns_base_flags = ImGuiTableColumnFlags_None;

        enum ContentsType { CT_Text, CT_Button, CT_SmallButton, CT_FillButton, CT_Selectable, CT_SelectableSpanRow };
        static int contents_type = CT_SelectableSpanRow;
        const char* contents_type_names[] = { "Text", "Button", "SmallButton", "FillButton", "Selectable", "Selectable (span row)" };
        static int freeze_cols = 1;
        static int freeze_rows = 1;
        static int items_count = IM_ARRAYSIZE(template_items_names) * 2;
        static ImVec2 outer_size_value = ImVec2(0.0f, TEXT_BASE_HEIGHT * 12);
        static float row_min_height = 0.0f; // Auto
        static float inner_width_with_scroll = 0.0f; // Auto-extend
        static bool outer_size_enabled = true;
        static bool show_headers = true;
        static bool show_wrapped_text = false;
        //static ImGuiTextFilter filter;
        //HvkGui::SetNextItemOpen(true, ImGuiCond_Once); // FIXME-TABLE: Enabling this results in initial clipped first pass on table which tend to affect column sizing
        if (HvkGui::TreeNode("Options"))
        {
            // Make the UI compact because there are so many fields
            PushStyleCompact();
            HvkGui::PushItemWidth(TEXT_BASE_WIDTH * 28.0f);

            if (HvkGui::TreeNodeEx("Features:", ImGuiTreeNodeFlags_DefaultOpen))
            {
                HvkGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags, ImGuiTableFlags_Resizable);
                HvkGui::CheckboxFlags("ImGuiTableFlags_Reorderable", &flags, ImGuiTableFlags_Reorderable);
                HvkGui::CheckboxFlags("ImGuiTableFlags_Hideable", &flags, ImGuiTableFlags_Hideable);
                HvkGui::CheckboxFlags("ImGuiTableFlags_Sortable", &flags, ImGuiTableFlags_Sortable);
                HvkGui::CheckboxFlags("ImGuiTableFlags_NoSavedSettings", &flags, ImGuiTableFlags_NoSavedSettings);
                HvkGui::CheckboxFlags("ImGuiTableFlags_ContextMenuInBody", &flags, ImGuiTableFlags_ContextMenuInBody);
                HvkGui::TreePop();
            }

            if (HvkGui::TreeNodeEx("Decorations:", ImGuiTreeNodeFlags_DefaultOpen))
            {
                HvkGui::CheckboxFlags("ImGuiTableFlags_RowBg", &flags, ImGuiTableFlags_RowBg);
                HvkGui::CheckboxFlags("ImGuiTableFlags_BordersV", &flags, ImGuiTableFlags_BordersV);
                HvkGui::CheckboxFlags("ImGuiTableFlags_BordersOuterV", &flags, ImGuiTableFlags_BordersOuterV);
                HvkGui::CheckboxFlags("ImGuiTableFlags_BordersInnerV", &flags, ImGuiTableFlags_BordersInnerV);
                HvkGui::CheckboxFlags("ImGuiTableFlags_BordersH", &flags, ImGuiTableFlags_BordersH);
                HvkGui::CheckboxFlags("ImGuiTableFlags_BordersOuterH", &flags, ImGuiTableFlags_BordersOuterH);
                HvkGui::CheckboxFlags("ImGuiTableFlags_BordersInnerH", &flags, ImGuiTableFlags_BordersInnerH);
                HvkGui::CheckboxFlags("ImGuiTableFlags_NoBordersInBody", &flags, ImGuiTableFlags_NoBordersInBody); HvkGui::SameLine(); HelpMarker("Disable vertical borders in columns Body (borders will always appear in Headers)");
                HvkGui::CheckboxFlags("ImGuiTableFlags_NoBordersInBodyUntilResize", &flags, ImGuiTableFlags_NoBordersInBodyUntilResize); HvkGui::SameLine(); HelpMarker("Disable vertical borders in columns Body until hovered for resize (borders will always appear in Headers)");
                HvkGui::TreePop();
            }

            if (HvkGui::TreeNodeEx("Sizing:", ImGuiTreeNodeFlags_DefaultOpen))
            {
                EditTableSizingFlags(&flags);
                HvkGui::SameLine(); HelpMarker("In the Advanced demo we override the policy of each column so those table-wide settings have less effect that typical.");
                HvkGui::CheckboxFlags("ImGuiTableFlags_NoHostExtendX", &flags, ImGuiTableFlags_NoHostExtendX);
                HvkGui::SameLine(); HelpMarker("Make outer width auto-fit to columns, overriding outer_size.x value.\n\nOnly available when ScrollX/ScrollY are disabled and Stretch columns are not used.");
                HvkGui::CheckboxFlags("ImGuiTableFlags_NoHostExtendY", &flags, ImGuiTableFlags_NoHostExtendY);
                HvkGui::SameLine(); HelpMarker("Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit).\n\nOnly available when ScrollX/ScrollY are disabled. Data below the limit will be clipped and not visible.");
                HvkGui::CheckboxFlags("ImGuiTableFlags_NoKeepColumnsVisible", &flags, ImGuiTableFlags_NoKeepColumnsVisible);
                HvkGui::SameLine(); HelpMarker("Only available if ScrollX is disabled.");
                HvkGui::CheckboxFlags("ImGuiTableFlags_PreciseWidths", &flags, ImGuiTableFlags_PreciseWidths);
                HvkGui::SameLine(); HelpMarker("Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: Without this flag: 33,33,34. With this flag: 33,33,33). With larger number of columns, resizing will appear to be less smooth.");
                HvkGui::CheckboxFlags("ImGuiTableFlags_NoClip", &flags, ImGuiTableFlags_NoClip);
                HvkGui::SameLine(); HelpMarker("Disable clipping rectangle for every individual columns (reduce draw command count, items will be able to overflow into other columns). Generally incompatible with ScrollFreeze options.");
                HvkGui::TreePop();
            }

            if (HvkGui::TreeNodeEx("Padding:", ImGuiTreeNodeFlags_DefaultOpen))
            {
                HvkGui::CheckboxFlags("ImGuiTableFlags_PadOuterX", &flags, ImGuiTableFlags_PadOuterX);
                HvkGui::CheckboxFlags("ImGuiTableFlags_NoPadOuterX", &flags, ImGuiTableFlags_NoPadOuterX);
                HvkGui::CheckboxFlags("ImGuiTableFlags_NoPadInnerX", &flags, ImGuiTableFlags_NoPadInnerX);
                HvkGui::TreePop();
            }

            if (HvkGui::TreeNodeEx("Scrolling:", ImGuiTreeNodeFlags_DefaultOpen))
            {
                HvkGui::CheckboxFlags("ImGuiTableFlags_ScrollX", &flags, ImGuiTableFlags_ScrollX);
                HvkGui::SameLine();
                HvkGui::SetNextItemWidth(HvkGui::GetFrameHeight());
                HvkGui::DragInt("freeze_cols", &freeze_cols, 0.2f, 0, 9, NULL, ImGuiSliderFlags_NoInput);
                HvkGui::CheckboxFlags("ImGuiTableFlags_ScrollY", &flags, ImGuiTableFlags_ScrollY);
                HvkGui::SameLine();
                HvkGui::SetNextItemWidth(HvkGui::GetFrameHeight());
                HvkGui::DragInt("freeze_rows", &freeze_rows, 0.2f, 0, 9, NULL, ImGuiSliderFlags_NoInput);
                HvkGui::TreePop();
            }

            if (HvkGui::TreeNodeEx("Sorting:", ImGuiTreeNodeFlags_DefaultOpen))
            {
                HvkGui::CheckboxFlags("ImGuiTableFlags_SortMulti", &flags, ImGuiTableFlags_SortMulti);
                HvkGui::SameLine(); HelpMarker("When sorting is enabled: hold shift when clicking headers to sort on multiple column. TableGetSortSpecs() may return specs where (SpecsCount > 1).");
                HvkGui::CheckboxFlags("ImGuiTableFlags_SortTristate", &flags, ImGuiTableFlags_SortTristate);
                HvkGui::SameLine(); HelpMarker("When sorting is enabled: allow no sorting, disable default sorting. TableGetSortSpecs() may return specs where (SpecsCount == 0).");
                HvkGui::TreePop();
            }

            if (HvkGui::TreeNodeEx("Headers:", ImGuiTreeNodeFlags_DefaultOpen))
            {
                HvkGui::Checkbox("show_headers", &show_headers);
                HvkGui::CheckboxFlags("ImGuiTableFlags_HighlightHoveredColumn", &flags, ImGuiTableFlags_HighlightHoveredColumn);
                HvkGui::CheckboxFlags("ImGuiTableColumnFlags_AngledHeader", &columns_base_flags, ImGuiTableColumnFlags_AngledHeader);
                HvkGui::SameLine(); HelpMarker("Enable AngledHeader on all columns. Best enabled on selected narrow columns (see \"Angled headers\" section of the demo).");
                HvkGui::TreePop();
            }

            if (HvkGui::TreeNodeEx("Other:", ImGuiTreeNodeFlags_DefaultOpen))
            {
                HvkGui::Checkbox("show_wrapped_text", &show_wrapped_text);

                HvkGui::DragFloat2("##OuterSize", &outer_size_value.x);
                HvkGui::SameLine(0.0f, HvkGui::GetStyle().ItemInnerSpacing.x);
                HvkGui::Checkbox("outer_size", &outer_size_enabled);
                HvkGui::SameLine();
                HelpMarker("If scrolling is disabled (ScrollX and ScrollY not set):\n"
                    "- The table is output directly in the parent window.\n"
                    "- OuterSize.x < 0.0f will right-align the table.\n"
                    "- OuterSize.x = 0.0f will narrow fit the table unless there are any Stretch columns.\n"
                    "- OuterSize.y then becomes the minimum size for the table, which will extend vertically if there are more rows (unless NoHostExtendY is set).");

                // From a user point of view we will tend to use 'inner_width' differently depending on whether our table is embedding scrolling.
                // To facilitate toying with this demo we will actually pass 0.0f to the BeginTable() when ScrollX is disabled.
                HvkGui::DragFloat("inner_width (when ScrollX active)", &inner_width_with_scroll, 1.0f, 0.0f, FLT_MAX);

                HvkGui::DragFloat("row_min_height", &row_min_height, 1.0f, 0.0f, FLT_MAX);
                HvkGui::SameLine(); HelpMarker("Specify height of the Selectable item.");

                HvkGui::DragInt("items_count", &items_count, 0.1f, 0, 9999);
                HvkGui::Combo("items_type (first column)", &contents_type, contents_type_names, IM_ARRAYSIZE(contents_type_names));
                //filter.Draw("filter");
                HvkGui::TreePop();
            }

            HvkGui::PopItemWidth();
            PopStyleCompact();
            HvkGui::Spacing();
            HvkGui::TreePop();
        }

        // Update item list if we changed the number of items
        static ImVector<MyItem> items;
        static ImVector<int> selection;
        static bool items_need_sort = false;
        if (items.Size != items_count)
        {
            items.resize(items_count, MyItem());
            for (int n = 0; n < items_count; n++)
            {
                const int template_n = n % IM_ARRAYSIZE(template_items_names);
                MyItem& item = items[n];
                item.ID = n;
                item.Name = template_items_names[template_n];
                item.Quantity = (template_n == 3) ? 10 : (template_n == 4) ? 20 : 0; // Assign default quantities
            }
        }

        const ImDrawList* parent_draw_list = HvkGui::GetWindowDrawList();
        const int parent_draw_list_draw_cmd_count = parent_draw_list->CmdBuffer.Size;
        ImVec2 table_scroll_cur, table_scroll_max; // For debug display
        const ImDrawList* table_draw_list = NULL;  // "

        // Submit table
        const float inner_width_to_use = (flags & ImGuiTableFlags_ScrollX) ? inner_width_with_scroll : 0.0f;
        if (HvkGui::BeginTable("table_advanced", 6, flags, outer_size_enabled ? outer_size_value : ImVec2(0, 0), inner_width_to_use))
        {
            // Declare columns
            // We use the "user_id" parameter of TableSetupColumn() to specify a user id that will be stored in the sort specifications.
            // This is so our sort function can identify a column given our own identifier. We could also identify them based on their index!
            HvkGui::TableSetupColumn("ID",           columns_base_flags | ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide, 0.0f, MyItemColumnID_ID);
            HvkGui::TableSetupColumn("Name",         columns_base_flags | ImGuiTableColumnFlags_WidthFixed, 0.0f, MyItemColumnID_Name);
            HvkGui::TableSetupColumn("Action",       columns_base_flags | ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, MyItemColumnID_Action);
            HvkGui::TableSetupColumn("Quantity",     columns_base_flags | ImGuiTableColumnFlags_PreferSortDescending, 0.0f, MyItemColumnID_Quantity);
            HvkGui::TableSetupColumn("Description",  columns_base_flags | ((flags & ImGuiTableFlags_NoHostExtendX) ? 0 : ImGuiTableColumnFlags_WidthStretch), 0.0f, MyItemColumnID_Description);
            HvkGui::TableSetupColumn("Hidden",       columns_base_flags |  ImGuiTableColumnFlags_DefaultHide | ImGuiTableColumnFlags_NoSort);
            HvkGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);

            // Sort our data if sort specs have been changed!
            ImGuiTableSortSpecs* sort_specs = HvkGui::TableGetSortSpecs();
            if (sort_specs && sort_specs->SpecsDirty)
                items_need_sort = true;
            if (sort_specs && items_need_sort && items.Size > 1)
            {
                MyItem::SortWithSortSpecs(sort_specs, items.Data, items.Size);
                sort_specs->SpecsDirty = false;
            }
            items_need_sort = false;

            // Take note of whether we are currently sorting based on the Quantity field,
            // we will use this to trigger sorting when we know the data of this column has been modified.
            const bool sorts_specs_using_quantity = (HvkGui::TableGetColumnFlags(3) & ImGuiTableColumnFlags_IsSorted) != 0;

            // Show headers
            if (show_headers && (columns_base_flags & ImGuiTableColumnFlags_AngledHeader) != 0)
                HvkGui::TableAngledHeadersRow();
            if (show_headers)
                HvkGui::TableHeadersRow();

            // Show data
            // FIXME-TABLE FIXME-NAV: How we can get decent up/down even though we have the buttons here?
#if 1
            // Demonstrate using clipper for large vertical lists
            ImGuiListClipper clipper;
            clipper.Begin(items.Size);
            while (clipper.Step())
            {
                for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
#else
            // Without clipper
            {
                for (int row_n = 0; row_n < items.Size; row_n++)
#endif
                {
                    MyItem* item = &items[row_n];
                    //if (!filter.PassFilter(item->Name))
                    //    continue;

                    const bool item_is_selected = selection.contains(item->ID);
                    HvkGui::PushID(item->ID);
                    HvkGui::TableNextRow(ImGuiTableRowFlags_None, row_min_height);

                    // For the demo purpose we can select among different type of items submitted in the first column
                    HvkGui::TableSetColumnIndex(0);
                    char label[32];
                    sprintf(label, "%04d", item->ID);
                    if (contents_type == CT_Text)
                        HvkGui::TextUnformatted(label);
                    else if (contents_type == CT_Button)
                        HvkGui::Button(label);
                    else if (contents_type == CT_SmallButton)
                        HvkGui::SmallButton(label);
                    else if (contents_type == CT_FillButton)
                        HvkGui::Button(label, ImVec2(-FLT_MIN, 0.0f));
                    else if (contents_type == CT_Selectable || contents_type == CT_SelectableSpanRow)
                    {
                        ImGuiSelectableFlags selectable_flags = (contents_type == CT_SelectableSpanRow) ? ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap : ImGuiSelectableFlags_None;
                        if (HvkGui::Selectable(label, item_is_selected, selectable_flags, ImVec2(0, row_min_height)))
                        {
                            if (HvkGui::GetIO().KeyCtrl)
                            {
                                if (item_is_selected)
                                    selection.find_erase_unsorted(item->ID);
                                else
                                    selection.push_back(item->ID);
                            }
                            else
                            {
                                selection.clear();
                                selection.push_back(item->ID);
                            }
                        }
                    }

                    if (HvkGui::TableSetColumnIndex(1))
                        HvkGui::TextUnformatted(item->Name);

                    // Here we demonstrate marking our data set as needing to be sorted again if we modified a quantity,
                    // and we are currently sorting on the column showing the Quantity.
                    // To avoid triggering a sort while holding the button, we only trigger it when the button has been released.
                    // You will probably need some extra logic if you want to automatically sort when a specific entry changes.
                    if (HvkGui::TableSetColumnIndex(2))
                    {
                        if (HvkGui::SmallButton("Chop")) { item->Quantity += 1; }
                        if (sorts_specs_using_quantity && HvkGui::IsItemDeactivated()) { items_need_sort = true; }
                        HvkGui::SameLine();
                        if (HvkGui::SmallButton("Eat")) { item->Quantity -= 1; }
                        if (sorts_specs_using_quantity && HvkGui::IsItemDeactivated()) { items_need_sort = true; }
                    }

                    if (HvkGui::TableSetColumnIndex(3))
                        HvkGui::Text("%d", item->Quantity);

                    HvkGui::TableSetColumnIndex(4);
                    if (show_wrapped_text)
                        HvkGui::TextWrapped("Lorem ipsum dolor sit amet");
                    else
                        HvkGui::Text("Lorem ipsum dolor sit amet");

                    if (HvkGui::TableSetColumnIndex(5))
                        HvkGui::Text("1234");

                    HvkGui::PopID();
                }
            }

            // Store some info to display debug details below
            table_scroll_cur = ImVec2(HvkGui::GetScrollX(), HvkGui::GetScrollY());
            table_scroll_max = ImVec2(HvkGui::GetScrollMaxX(), HvkGui::GetScrollMaxY());
            table_draw_list = HvkGui::GetWindowDrawList();
            HvkGui::EndTable();
        }
        static bool show_debug_details = false;
        HvkGui::Checkbox("Debug details", &show_debug_details);
        if (show_debug_details && table_draw_list)
        {
            HvkGui::SameLine(0.0f, 0.0f);
            const int table_draw_list_draw_cmd_count = table_draw_list->CmdBuffer.Size;
            if (table_draw_list == parent_draw_list)
                HvkGui::Text(": DrawCmd: +%d (in same window)",
                    table_draw_list_draw_cmd_count - parent_draw_list_draw_cmd_count);
            else
                HvkGui::Text(": DrawCmd: +%d (in child window), Scroll: (%.f/%.f) (%.f/%.f)",
                    table_draw_list_draw_cmd_count - 1, table_scroll_cur.x, table_scroll_max.x, table_scroll_cur.y, table_scroll_max.y);
        }
        HvkGui::TreePop();
    }

    HvkGui::PopID();

    DemoWindowColumns();

    if (disable_indent)
        HvkGui::PopStyleVar();
}

// Demonstrate old/legacy Columns API!
// [2020: Columns are under-featured and not maintained. Prefer using the more flexible and powerful BeginTable() API!]
static void DemoWindowColumns()
{
    IMGUI_DEMO_MARKER("Columns (legacy API)");
    bool open = HvkGui::TreeNode("Legacy Columns API");
    HvkGui::SameLine();
    HelpMarker("Columns() is an old API! Prefer using the more flexible and powerful BeginTable() API!");
    if (!open)
        return;

    // Basic columns
    IMGUI_DEMO_MARKER("Columns (legacy API)/Basic");
    if (HvkGui::TreeNode("Basic"))
    {
        HvkGui::Text("Without border:");
        HvkGui::Columns(3, "mycolumns3", false);  // 3-ways, no border
        HvkGui::Separator();
        for (int n = 0; n < 14; n++)
        {
            char label[32];
            sprintf(label, "Item %d", n);
            if (HvkGui::Selectable(label)) {}
            //if (HvkGui::Button(label, ImVec2(-FLT_MIN,0.0f))) {}
            HvkGui::NextColumn();
        }
        HvkGui::Columns(1);
        HvkGui::Separator();

        HvkGui::Text("With border:");
        HvkGui::Columns(4, "mycolumns"); // 4-ways, with border
        HvkGui::Separator();
        HvkGui::Text("ID"); HvkGui::NextColumn();
        HvkGui::Text("Name"); HvkGui::NextColumn();
        HvkGui::Text("Path"); HvkGui::NextColumn();
        HvkGui::Text("Hovered"); HvkGui::NextColumn();
        HvkGui::Separator();
        const char* names[3] = { "One", "Two", "Three" };
        const char* paths[3] = { "/path/one", "/path/two", "/path/three" };
        static int selected = -1;
        for (int i = 0; i < 3; i++)
        {
            char label[32];
            sprintf(label, "%04d", i);
            if (HvkGui::Selectable(label, selected == i, ImGuiSelectableFlags_SpanAllColumns))
                selected = i;
            bool hovered = HvkGui::IsItemHovered();
            HvkGui::NextColumn();
            HvkGui::Text(names[i]); HvkGui::NextColumn();
            HvkGui::Text(paths[i]); HvkGui::NextColumn();
            HvkGui::Text("%d", hovered); HvkGui::NextColumn();
        }
        HvkGui::Columns(1);
        HvkGui::Separator();
        HvkGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Columns (legacy API)/Borders");
    if (HvkGui::TreeNode("Borders"))
    {
        // NB: Future columns API should allow automatic horizontal borders.
        static bool h_borders = true;
        static bool v_borders = true;
        static int columns_count = 4;
        const int lines_count = 3;
        HvkGui::SetNextItemWidth(HvkGui::GetFontSize() * 8);
        HvkGui::DragInt("##columns_count", &columns_count, 0.1f, 2, 10, "%d columns");
        if (columns_count < 2)
            columns_count = 2;
        HvkGui::SameLine();
        HvkGui::Checkbox("horizontal", &h_borders);
        HvkGui::SameLine();
        HvkGui::Checkbox("vertical", &v_borders);
        HvkGui::Columns(columns_count, NULL, v_borders);
        for (int i = 0; i < columns_count * lines_count; i++)
        {
            if (h_borders && HvkGui::GetColumnIndex() == 0)
                HvkGui::Separator();
            HvkGui::PushID(i);
            HvkGui::Text("%c%c%c", 'a' + i, 'a' + i, 'a' + i);
            HvkGui::Text("Width %.2f", HvkGui::GetColumnWidth());
            HvkGui::Text("Avail %.2f", HvkGui::GetContentRegionAvail().x);
            HvkGui::Text("Offset %.2f", HvkGui::GetColumnOffset());
            HvkGui::Text("Long text that is likely to clip");
            HvkGui::Button("Button", ImVec2(-FLT_MIN, 0.0f));
            HvkGui::PopID();
            HvkGui::NextColumn();
        }
        HvkGui::Columns(1);
        if (h_borders)
            HvkGui::Separator();
        HvkGui::TreePop();
    }

    // Create multiple items in a same cell before switching to next column
    IMGUI_DEMO_MARKER("Columns (legacy API)/Mixed items");
    if (HvkGui::TreeNode("Mixed items"))
    {
        HvkGui::Columns(3, "mixed");
        HvkGui::Separator();

        HvkGui::Text("Hello");
        HvkGui::Button("Banana");
        HvkGui::NextColumn();

        HvkGui::Text("ImGui");
        HvkGui::Button("Apple");
        static float foo = 1.0f;
        HvkGui::InputFloat("red", &foo, 0.05f, 0, "%.3f");
        HvkGui::Text("An extra line here.");
        HvkGui::NextColumn();

        HvkGui::Text("Sailor");
        HvkGui::Button("Corniflower");
        static float bar = 1.0f;
        HvkGui::InputFloat("blue", &bar, 0.05f, 0, "%.3f");
        HvkGui::NextColumn();

        if (HvkGui::CollapsingHeader("Category A")) { HvkGui::Text("Blah blah blah"); } HvkGui::NextColumn();
        if (HvkGui::CollapsingHeader("Category B")) { HvkGui::Text("Blah blah blah"); } HvkGui::NextColumn();
        if (HvkGui::CollapsingHeader("Category C")) { HvkGui::Text("Blah blah blah"); } HvkGui::NextColumn();
        HvkGui::Columns(1);
        HvkGui::Separator();
        HvkGui::TreePop();
    }

    // Word wrapping
    IMGUI_DEMO_MARKER("Columns (legacy API)/Word-wrapping");
    if (HvkGui::TreeNode("Word-wrapping"))
    {
        HvkGui::Columns(2, "word-wrapping");
        HvkGui::Separator();
        HvkGui::TextWrapped("The quick brown fox jumps over the lazy dog.");
        HvkGui::TextWrapped("Hello Left");
        HvkGui::NextColumn();
        HvkGui::TextWrapped("The quick brown fox jumps over the lazy dog.");
        HvkGui::TextWrapped("Hello Right");
        HvkGui::Columns(1);
        HvkGui::Separator();
        HvkGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Columns (legacy API)/Horizontal Scrolling");
    if (HvkGui::TreeNode("Horizontal Scrolling"))
    {
        HvkGui::SetNextWindowContentSize(ImVec2(1500.0f, 0.0f));
        ImVec2 child_size = ImVec2(0, HvkGui::GetFontSize() * 20.0f);
        HvkGui::BeginChild("##ScrollingRegion", child_size, ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);
        HvkGui::Columns(10);

        // Also demonstrate using clipper for large vertical lists
        int ITEMS_COUNT = 2000;
        ImGuiListClipper clipper;
        clipper.Begin(ITEMS_COUNT);
        while (clipper.Step())
        {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                for (int j = 0; j < 10; j++)
                {
                    HvkGui::Text("Line %d Column %d...", i, j);
                    HvkGui::NextColumn();
                }
        }
        HvkGui::Columns(1);
        HvkGui::EndChild();
        HvkGui::TreePop();
    }

    IMGUI_DEMO_MARKER("Columns (legacy API)/Tree");
    if (HvkGui::TreeNode("Tree"))
    {
        HvkGui::Columns(2, "tree", true);
        for (int x = 0; x < 3; x++)
        {
            bool open1 = HvkGui::TreeNode((void*)(intptr_t)x, "Node%d", x);
            HvkGui::NextColumn();
            HvkGui::Text("Node contents");
            HvkGui::NextColumn();
            if (open1)
            {
                for (int y = 0; y < 3; y++)
                {
                    bool open2 = HvkGui::TreeNode((void*)(intptr_t)y, "Node%d.%d", x, y);
                    HvkGui::NextColumn();
                    HvkGui::Text("Node contents");
                    if (open2)
                    {
                        HvkGui::Text("Even more contents");
                        if (HvkGui::TreeNode("Tree in column"))
                        {
                            HvkGui::Text("The quick brown fox jumps over the lazy dog");
                            HvkGui::TreePop();
                        }
                    }
                    HvkGui::NextColumn();
                    if (open2)
                        HvkGui::TreePop();
                }
                HvkGui::TreePop();
            }
        }
        HvkGui::Columns(1);
        HvkGui::TreePop();
    }

    HvkGui::TreePop();
}

//-----------------------------------------------------------------------------
// [SECTION] DemoWindowInputs()
//-----------------------------------------------------------------------------

static void DemoWindowInputs()
{
    IMGUI_DEMO_MARKER("Inputs & Focus");
    if (HvkGui::CollapsingHeader("Inputs & Focus"))
    {
        ImGuiIO& io = HvkGui::GetIO();

        // Display inputs submitted to ImGuiIO
        IMGUI_DEMO_MARKER("Inputs & Focus/Inputs");
        HvkGui::SetNextItemOpen(true, ImGuiCond_Once);
        bool inputs_opened = HvkGui::TreeNode("Inputs");
        HvkGui::SameLine();
        HelpMarker(
            "This is a simplified view. See more detailed input state:\n"
            "- in 'Tools->Metrics/Debugger->Inputs'.\n"
            "- in 'Tools->Debug Log->IO'.");
        if (inputs_opened)
        {
            if (HvkGui::IsMousePosValid())
                HvkGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
            else
                HvkGui::Text("Mouse pos: <INVALID>");
            HvkGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
            HvkGui::Text("Mouse down:");
            for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (HvkGui::IsMouseDown(i)) { HvkGui::SameLine(); HvkGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
            HvkGui::Text("Mouse wheel: %.1f", io.MouseWheel);
            HvkGui::Text("Mouse clicked count:");
            for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (io.MouseClickedCount[i] > 0) { HvkGui::SameLine(); HvkGui::Text("b%d: %d", i, io.MouseClickedCount[i]); }

            // We iterate both legacy native range and named ImGuiKey ranges. This is a little unusual/odd but this allows
            // displaying the data for old/new backends.
            // User code should never have to go through such hoops!
            // You can generally iterate between ImGuiKey_NamedKey_BEGIN and ImGuiKey_NamedKey_END.
            struct funcs { static bool IsLegacyNativeDupe(ImGuiKey) { return false; } };
            ImGuiKey start_key = ImGuiKey_NamedKey_BEGIN;
            HvkGui::Text("Keys down:");         for (ImGuiKey key = start_key; key < ImGuiKey_NamedKey_END; key = (ImGuiKey)(key + 1)) { if (funcs::IsLegacyNativeDupe(key) || !HvkGui::IsKeyDown(key)) continue; HvkGui::SameLine(); HvkGui::Text((key < ImGuiKey_NamedKey_BEGIN) ? "\"%s\"" : "\"%s\" %d", HvkGui::GetKeyName(key), key); }
            HvkGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
            HvkGui::Text("Chars queue:");       for (int i = 0; i < io.InputQueueCharacters.Size; i++) { ImWchar c = io.InputQueueCharacters[i]; HvkGui::SameLine();  HvkGui::Text("\'%c\' (0x%04X)", (c > ' ' && c <= 255) ? (char)c : '?', c); } // FIXME: We should convert 'c' to UTF-8 here but the functions are not public.

            HvkGui::TreePop();
        }

        // Display ImGuiIO output flags
        IMGUI_DEMO_MARKER("Inputs & Focus/Outputs");
        HvkGui::SetNextItemOpen(true, ImGuiCond_Once);
        bool outputs_opened = HvkGui::TreeNode("Outputs");
        HvkGui::SameLine();
        HelpMarker(
            "The value of io.WantCaptureMouse and io.WantCaptureKeyboard are normally set by Dear ImGui "
            "to instruct your application of how to route inputs. Typically, when a value is true, it means "
            "Dear ImGui wants the corresponding inputs and we expect the underlying application to ignore them.\n\n"
            "The most typical case is: when hovering a window, Dear ImGui set io.WantCaptureMouse to true, "
            "and underlying application should ignore mouse inputs (in practice there are many and more subtle "
            "rules leading to how those flags are set).");
        if (outputs_opened)
        {
            HvkGui::Text("io.WantCaptureMouse: %d", io.WantCaptureMouse);
            HvkGui::Text("io.WantCaptureMouseUnlessPopupClose: %d", io.WantCaptureMouseUnlessPopupClose);
            HvkGui::Text("io.WantCaptureKeyboard: %d", io.WantCaptureKeyboard);
            HvkGui::Text("io.WantTextInput: %d", io.WantTextInput);
            HvkGui::Text("io.WantSetMousePos: %d", io.WantSetMousePos);
            HvkGui::Text("io.NavActive: %d, io.NavVisible: %d", io.NavActive, io.NavVisible);

            IMGUI_DEMO_MARKER("Inputs & Focus/Outputs/WantCapture override");
            if (HvkGui::TreeNode("WantCapture override"))
            {
                HelpMarker(
                    "Hovering the colored canvas will override io.WantCaptureXXX fields.\n"
                    "Notice how normally (when set to none), the value of io.WantCaptureKeyboard would be false when hovering "
                    "and true when clicking.");
                static int capture_override_mouse = -1;
                static int capture_override_keyboard = -1;
                const char* capture_override_desc[] = { "None", "Set to false", "Set to true" };
                HvkGui::SetNextItemWidth(HvkGui::GetFontSize() * 15);
                HvkGui::SliderInt("SetNextFrameWantCaptureMouse() on hover", &capture_override_mouse, -1, +1, capture_override_desc[capture_override_mouse + 1], ImGuiSliderFlags_AlwaysClamp);
                HvkGui::SetNextItemWidth(HvkGui::GetFontSize() * 15);
                HvkGui::SliderInt("SetNextFrameWantCaptureKeyboard() on hover", &capture_override_keyboard, -1, +1, capture_override_desc[capture_override_keyboard + 1], ImGuiSliderFlags_AlwaysClamp);

                HvkGui::ColorButton("##panel", ImVec4(0.7f, 0.1f, 0.7f, 1.0f), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, ImVec2(128.0f, 96.0f)); // Dummy item
                if (HvkGui::IsItemHovered() && capture_override_mouse != -1)
                    HvkGui::SetNextFrameWantCaptureMouse(capture_override_mouse == 1);
                if (HvkGui::IsItemHovered() && capture_override_keyboard != -1)
                    HvkGui::SetNextFrameWantCaptureKeyboard(capture_override_keyboard == 1);

                HvkGui::TreePop();
            }
            HvkGui::TreePop();
        }

        // Demonstrate using Shortcut() and Routing Policies.
        // The general flow is:
        // - Code interested in a chord (e.g. "Ctrl+A") declares their intent.
        // - Multiple locations may be interested in same chord! Routing helps find a winner.
        // - Every frame, we resolve all claims and assign one owner if the modifiers are matching.
        // - The lower-level function is 'bool SetShortcutRouting()', returns true when caller got the route.
        // - Most of the times, SetShortcutRouting() is not called directly. User mostly calls Shortcut() with routing flags.
        // - If you call Shortcut() WITHOUT any routing option, it uses ImGuiInputFlags_RouteFocused.
        // TL;DR: Most uses will simply be:
        // - Shortcut(ImGuiMod_Ctrl | ImGuiKey_A); // Use ImGuiInputFlags_RouteFocused policy.
        IMGUI_DEMO_MARKER("Inputs & Focus/Shortcuts");
        if (HvkGui::TreeNode("Shortcuts"))
        {
            static ImGuiInputFlags route_options = ImGuiInputFlags_Repeat;
            static ImGuiInputFlags route_type = ImGuiInputFlags_RouteFocused;
            HvkGui::CheckboxFlags("ImGuiInputFlags_Repeat", &route_options, ImGuiInputFlags_Repeat);
            HvkGui::RadioButton("ImGuiInputFlags_RouteActive", &route_type, ImGuiInputFlags_RouteActive);
            HvkGui::RadioButton("ImGuiInputFlags_RouteFocused (default)", &route_type, ImGuiInputFlags_RouteFocused);
            HvkGui::RadioButton("ImGuiInputFlags_RouteGlobal", &route_type, ImGuiInputFlags_RouteGlobal);
            HvkGui::Indent();
            HvkGui::BeginDisabled(route_type != ImGuiInputFlags_RouteGlobal);
            HvkGui::CheckboxFlags("ImGuiInputFlags_RouteOverFocused", &route_options, ImGuiInputFlags_RouteOverFocused);
            HvkGui::CheckboxFlags("ImGuiInputFlags_RouteOverActive", &route_options, ImGuiInputFlags_RouteOverActive);
            HvkGui::CheckboxFlags("ImGuiInputFlags_RouteUnlessBgFocused", &route_options, ImGuiInputFlags_RouteUnlessBgFocused);
            HvkGui::EndDisabled();
            HvkGui::Unindent();
            HvkGui::RadioButton("ImGuiInputFlags_RouteAlways", &route_type, ImGuiInputFlags_RouteAlways);
            ImGuiInputFlags flags = route_type | route_options; // Merged flags
            if (route_type != ImGuiInputFlags_RouteGlobal)
                flags &= ~(ImGuiInputFlags_RouteOverFocused | ImGuiInputFlags_RouteOverActive | ImGuiInputFlags_RouteUnlessBgFocused);

            HvkGui::SeparatorText("Using SetNextItemShortcut()");
            HvkGui::Text("Ctrl+S");
            HvkGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_S, flags | ImGuiInputFlags_Tooltip);
            HvkGui::Button("Save");
            HvkGui::Text("Alt+F");
            HvkGui::SetNextItemShortcut(ImGuiMod_Alt | ImGuiKey_F, flags | ImGuiInputFlags_Tooltip);
            static float f = 0.5f;
            HvkGui::SliderFloat("Factor", &f, 0.0f, 1.0f);

            HvkGui::SeparatorText("Using Shortcut()");
            const float line_height = HvkGui::GetTextLineHeightWithSpacing();
            const ImGuiKeyChord key_chord = ImGuiMod_Ctrl | ImGuiKey_A;

            HvkGui::Text("Ctrl+A");
            HvkGui::Text("IsWindowFocused: %d, Shortcut: %s", HvkGui::IsWindowFocused(), HvkGui::Shortcut(key_chord, flags) ? "PRESSED" : "...");

            HvkGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 0.0f, 1.0f, 0.1f));

            HvkGui::BeginChild("WindowA", ImVec2(-FLT_MIN, line_height * 14), true);
            HvkGui::Text("Press Ctrl+A and see who receives it!");
            HvkGui::Separator();

            // 1: Window polling for Ctrl+A
            HvkGui::Text("(in WindowA)");
            HvkGui::Text("IsWindowFocused: %d, Shortcut: %s", HvkGui::IsWindowFocused(), HvkGui::Shortcut(key_chord, flags) ? "PRESSED" : "...");

            // 2: InputText also polling for Ctrl+A: it always uses _RouteFocused internally (gets priority when active)
            // (Commented because the owner-aware version of Shortcut() is still in imgui_internal.h)
            //char str[16] = "Press Ctrl+A";
            //HvkGui::Spacing();
            //HvkGui::InputText("InputTextB", str, IM_ARRAYSIZE(str), ImGuiInputTextFlags_ReadOnly);
            //ImGuiID item_id = HvkGui::GetItemID();
            //HvkGui::SameLine(); HelpMarker("Internal widgets always use _RouteFocused");
            //HvkGui::Text("IsWindowFocused: %d, Shortcut: %s", HvkGui::IsWindowFocused(), HvkGui::Shortcut(key_chord, flags, item_id) ? "PRESSED" : "...");

            // 3: Dummy child is not claiming the route: focusing them shouldn't steal route away from WindowA
            HvkGui::BeginChild("ChildD", ImVec2(-FLT_MIN, line_height * 4), true);
            HvkGui::Text("(in ChildD: not using same Shortcut)");
            HvkGui::Text("IsWindowFocused: %d", HvkGui::IsWindowFocused());
            HvkGui::EndChild();

            // 4: Child window polling for Ctrl+A. It is deeper than WindowA and gets priority when focused.
            HvkGui::BeginChild("ChildE", ImVec2(-FLT_MIN, line_height * 4), true);
            HvkGui::Text("(in ChildE: using same Shortcut)");
            HvkGui::Text("IsWindowFocused: %d, Shortcut: %s", HvkGui::IsWindowFocused(), HvkGui::Shortcut(key_chord, flags) ? "PRESSED" : "...");
            HvkGui::EndChild();

            // 5: In a popup
            if (HvkGui::Button("Open Popup"))
                HvkGui::OpenPopup("PopupF");
            if (HvkGui::BeginPopup("PopupF"))
            {
                HvkGui::Text("(in PopupF)");
                HvkGui::Text("IsWindowFocused: %d, Shortcut: %s", HvkGui::IsWindowFocused(), HvkGui::Shortcut(key_chord, flags) ? "PRESSED" : "...");
                // (Commented because the owner-aware version of Shortcut() is still in imgui_internal.h)
                //HvkGui::InputText("InputTextG", str, IM_ARRAYSIZE(str), ImGuiInputTextFlags_ReadOnly);
                //HvkGui::Text("IsWindowFocused: %d, Shortcut: %s", HvkGui::IsWindowFocused(), HvkGui::Shortcut(key_chord, flags, HvkGui::GetItemID()) ? "PRESSED" : "...");
                HvkGui::EndPopup();
            }
            HvkGui::EndChild();
            HvkGui::PopStyleColor();

            HvkGui::TreePop();
        }

        // Display mouse cursors
        IMGUI_DEMO_MARKER("Inputs & Focus/Mouse Cursors");
        if (HvkGui::TreeNode("Mouse Cursors"))
        {
            const char* mouse_cursors_names[] = { "Arrow", "TextInput", "ResizeAll", "ResizeNS", "ResizeEW", "ResizeNESW", "ResizeNWSE", "Hand", "Wait", "Progress", "NotAllowed" };
            IM_ASSERT(IM_ARRAYSIZE(mouse_cursors_names) == ImGuiMouseCursor_COUNT);

            ImGuiMouseCursor current = HvkGui::GetMouseCursor();
            const char* cursor_name = (current >= ImGuiMouseCursor_Arrow) && (current < ImGuiMouseCursor_COUNT) ? mouse_cursors_names[current] : "N/A";
            HvkGui::Text("Current mouse cursor = %d: %s", current, cursor_name);
            HvkGui::BeginDisabled(true);
            HvkGui::CheckboxFlags("io.BackendFlags: HasMouseCursors", &io.BackendFlags, ImGuiBackendFlags_HasMouseCursors);
            HvkGui::EndDisabled();

            HvkGui::Text("Hover to see mouse cursors:");
            HvkGui::SameLine(); HelpMarker(
                "Your application can render a different mouse cursor based on what HvkGui::GetMouseCursor() returns. "
                "If software cursor rendering (io.MouseDrawCursor) is set ImGui will draw the right cursor for you, "
                "otherwise your backend needs to handle it.");
            for (int i = 0; i < ImGuiMouseCursor_COUNT; i++)
            {
                char label[32];
                sprintf(label, "Mouse cursor %d: %s", i, mouse_cursors_names[i]);
                HvkGui::Bullet(); HvkGui::Selectable(label, false);
                if (HvkGui::IsItemHovered())
                    HvkGui::SetMouseCursor(i);
            }
            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Inputs & Focus/Tabbing");
        if (HvkGui::TreeNode("Tabbing"))
        {
            HvkGui::Text("Use Tab/Shift+Tab to cycle through keyboard editable fields.");
            static char buf[32] = "hello";
            HvkGui::InputText("1", buf, IM_ARRAYSIZE(buf));
            HvkGui::InputText("2", buf, IM_ARRAYSIZE(buf));
            HvkGui::InputText("3", buf, IM_ARRAYSIZE(buf));
            HvkGui::PushItemFlag(ImGuiItemFlags_NoTabStop, true);
            HvkGui::InputText("4 (tab skip)", buf, IM_ARRAYSIZE(buf));
            HvkGui::SameLine(); HelpMarker("Item won't be cycled through when using TAB or Shift+Tab.");
            HvkGui::PopItemFlag();
            HvkGui::InputText("5", buf, IM_ARRAYSIZE(buf));
            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Inputs & Focus/Focus from code");
        if (HvkGui::TreeNode("Focus from code"))
        {
            bool focus_1 = HvkGui::Button("Focus on 1"); HvkGui::SameLine();
            bool focus_2 = HvkGui::Button("Focus on 2"); HvkGui::SameLine();
            bool focus_3 = HvkGui::Button("Focus on 3");
            int has_focus = 0;
            static char buf[128] = "click on a button to set focus";

            if (focus_1) HvkGui::SetKeyboardFocusHere();
            HvkGui::InputText("1", buf, IM_ARRAYSIZE(buf));
            if (HvkGui::IsItemActive()) has_focus = 1;

            if (focus_2) HvkGui::SetKeyboardFocusHere();
            HvkGui::InputText("2", buf, IM_ARRAYSIZE(buf));
            if (HvkGui::IsItemActive()) has_focus = 2;

            HvkGui::PushItemFlag(ImGuiItemFlags_NoTabStop, true);
            if (focus_3) HvkGui::SetKeyboardFocusHere();
            HvkGui::InputText("3 (tab skip)", buf, IM_ARRAYSIZE(buf));
            if (HvkGui::IsItemActive()) has_focus = 3;
            HvkGui::SameLine(); HelpMarker("Item won't be cycled through when using TAB or Shift+Tab.");
            HvkGui::PopItemFlag();

            if (has_focus)
                HvkGui::Text("Item with focus: %d", has_focus);
            else
                HvkGui::Text("Item with focus: <none>");

            // Use >= 0 parameter to SetKeyboardFocusHere() to focus an upcoming item
            static float f3[3] = { 0.0f, 0.0f, 0.0f };
            int focus_ahead = -1;
            if (HvkGui::Button("Focus on X")) { focus_ahead = 0; } HvkGui::SameLine();
            if (HvkGui::Button("Focus on Y")) { focus_ahead = 1; } HvkGui::SameLine();
            if (HvkGui::Button("Focus on Z")) { focus_ahead = 2; }
            if (focus_ahead != -1) HvkGui::SetKeyboardFocusHere(focus_ahead);
            HvkGui::SliderFloat3("Float3", &f3[0], 0.0f, 1.0f);

            HvkGui::TextWrapped("NB: Cursor & selection are preserved when refocusing last used item in code.");
            HvkGui::TreePop();
        }

        IMGUI_DEMO_MARKER("Inputs & Focus/Dragging");
        if (HvkGui::TreeNode("Dragging"))
        {
            HvkGui::TextWrapped("You can use HvkGui::GetMouseDragDelta(0) to query for the dragged amount on any widget.");
            for (int button = 0; button < 3; button++)
            {
                HvkGui::Text("IsMouseDragging(%d):", button);
                HvkGui::Text("  w/ default threshold: %d,", HvkGui::IsMouseDragging(button));
                HvkGui::Text("  w/ zero threshold: %d,", HvkGui::IsMouseDragging(button, 0.0f));
                HvkGui::Text("  w/ large threshold: %d,", HvkGui::IsMouseDragging(button, 20.0f));
            }

            HvkGui::Button("Drag Me");
            if (HvkGui::IsItemActive())
                HvkGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, HvkGui::GetColorU32(ImGuiCol_Button), 4.0f); // Draw a line between the button and the mouse cursor

            // Drag operations gets "unlocked" when the mouse has moved past a certain threshold
            // (the default threshold is stored in io.MouseDragThreshold). You can request a lower or higher
            // threshold using the second parameter of IsMouseDragging() and GetMouseDragDelta().
            ImVec2 value_raw = HvkGui::GetMouseDragDelta(0, 0.0f);
            ImVec2 value_with_lock_threshold = HvkGui::GetMouseDragDelta(0);
            ImVec2 mouse_delta = io.MouseDelta;
            HvkGui::Text("GetMouseDragDelta(0):");
            HvkGui::Text("  w/ default threshold: (%.1f, %.1f)", value_with_lock_threshold.x, value_with_lock_threshold.y);
            HvkGui::Text("  w/ zero threshold: (%.1f, %.1f)", value_raw.x, value_raw.y);
            HvkGui::Text("io.MouseDelta: (%.1f, %.1f)", mouse_delta.x, mouse_delta.y);
            HvkGui::TreePop();
        }
    }
}

//-----------------------------------------------------------------------------
// [SECTION] About Window / ShowAboutWindow()
// Access from Dear ImGui Demo -> Tools -> About
//-----------------------------------------------------------------------------

void HvkGui::ShowAboutWindow(bool* p_open)
{
    if (!HvkGui::Begin("About Dear ImGui", p_open, ImGuiWindowFlags_AlwaysAutoResize))
    {
        HvkGui::End();
        return;
    }
    IMGUI_DEMO_MARKER("Tools/About Dear ImGui");
    HvkGui::Text("Dear ImGui %s (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);

    HvkGui::TextLinkOpenURL("Homepage", "https://github.com/ocornut/imgui");
    HvkGui::SameLine();
    HvkGui::TextLinkOpenURL("FAQ", "https://github.com/ocornut/imgui/blob/master/docs/FAQ.md");
    HvkGui::SameLine();
    HvkGui::TextLinkOpenURL("Wiki", "https://github.com/ocornut/imgui/wiki");
    HvkGui::SameLine();
    HvkGui::TextLinkOpenURL("Extensions", "https://github.com/ocornut/imgui/wiki/Useful-Extensions");
    HvkGui::SameLine();
    HvkGui::TextLinkOpenURL("Releases", "https://github.com/ocornut/imgui/releases");
    HvkGui::SameLine();
    HvkGui::TextLinkOpenURL("Funding", "https://github.com/ocornut/imgui/wiki/Funding");

    HvkGui::Separator();
    HvkGui::Text("(c) 2014-2025 Omar Cornut");
    HvkGui::Text("Developed by Omar Cornut and all Dear ImGui contributors.");
    HvkGui::Text("Dear ImGui is licensed under the MIT License, see LICENSE for more information.");
    HvkGui::Text("If your company uses this, please consider funding the project.");

    static bool show_config_info = false;
    HvkGui::Checkbox("Config/Build Information", &show_config_info);
    if (show_config_info)
    {
        ImGuiIO& io = HvkGui::GetIO();
        ImGuiStyle& style = HvkGui::GetStyle();

        bool copy_to_clipboard = HvkGui::Button("Copy to clipboard");
        ImVec2 child_size = ImVec2(0, HvkGui::GetTextLineHeightWithSpacing() * 18);
        HvkGui::BeginChild(HvkGui::GetID("cfg_infos"), child_size, ImGuiChildFlags_FrameStyle);
        if (copy_to_clipboard)
        {
            HvkGui::LogToClipboard();
            HvkGui::LogText("```cpp\n"); // Back quotes will make text appears without formatting when pasting on GitHub
        }

        HvkGui::Text("Dear ImGui %s (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
        HvkGui::Separator();
        HvkGui::Text("sizeof(size_t): %d, sizeof(ImDrawIdx): %d, sizeof(ImDrawVert): %d", (int)sizeof(size_t), (int)sizeof(ImDrawIdx), (int)sizeof(ImDrawVert));
        HvkGui::Text("define: __cplusplus=%d", (int)__cplusplus);
#ifdef IMGUI_ENABLE_TEST_ENGINE
        HvkGui::Text("define: IMGUI_ENABLE_TEST_ENGINE");
#endif
#ifdef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
        HvkGui::Text("define: IMGUI_DISABLE_OBSOLETE_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS
        HvkGui::Text("define: IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS
        HvkGui::Text("define: IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_WIN32_FUNCTIONS
        HvkGui::Text("define: IMGUI_DISABLE_WIN32_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_DEFAULT_SHELL_FUNCTIONS
        HvkGui::Text("define: IMGUI_DISABLE_DEFAULT_SHELL_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_DEFAULT_FORMAT_FUNCTIONS
        HvkGui::Text("define: IMGUI_DISABLE_DEFAULT_FORMAT_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_DEFAULT_MATH_FUNCTIONS
        HvkGui::Text("define: IMGUI_DISABLE_DEFAULT_MATH_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_DEFAULT_FILE_FUNCTIONS
        HvkGui::Text("define: IMGUI_DISABLE_DEFAULT_FILE_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_FILE_FUNCTIONS
        HvkGui::Text("define: IMGUI_DISABLE_FILE_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_DEFAULT_ALLOCATORS
        HvkGui::Text("define: IMGUI_DISABLE_DEFAULT_ALLOCATORS");
#endif
#ifdef IMGUI_USE_BGRA_PACKED_COLOR
        HvkGui::Text("define: IMGUI_USE_BGRA_PACKED_COLOR");
#endif
#ifdef _WIN32
        HvkGui::Text("define: _WIN32");
#endif
#ifdef _WIN64
        HvkGui::Text("define: _WIN64");
#endif
#ifdef __linux__
        HvkGui::Text("define: __linux__");
#endif
#ifdef __APPLE__
        HvkGui::Text("define: __APPLE__");
#endif
#ifdef _MSC_VER
        HvkGui::Text("define: _MSC_VER=%d", _MSC_VER);
#endif
#ifdef _MSVC_LANG
        HvkGui::Text("define: _MSVC_LANG=%d", (int)_MSVC_LANG);
#endif
#ifdef __MINGW32__
        HvkGui::Text("define: __MINGW32__");
#endif
#ifdef __MINGW64__
        HvkGui::Text("define: __MINGW64__");
#endif
#ifdef __GNUC__
        HvkGui::Text("define: __GNUC__=%d", (int)__GNUC__);
#endif
#ifdef __clang_version__
        HvkGui::Text("define: __clang_version__=%s", __clang_version__);
#endif
#ifdef __EMSCRIPTEN__
        HvkGui::Text("define: __EMSCRIPTEN__");
        HvkGui::Text("Emscripten: %d.%d.%d", __EMSCRIPTEN_major__, __EMSCRIPTEN_minor__, __EMSCRIPTEN_tiny__);
#endif
#ifdef NDEBUG
        HvkGui::Text("define: NDEBUG");
#endif

        // Heuristic to detect no-op IM_ASSERT() macros
        // - This is designed so people opening bug reports would convey and notice that they have disabled asserts for Dear ImGui code.
        // - 16 is > strlen("((void)(_EXPR))") which we suggested in our imconfig.h template as a possible way to disable.
        int assert_runs_expression = 0;
        IM_ASSERT(++assert_runs_expression);
        int assert_expand_len = (int)strlen(IM_STRINGIFY((IM_ASSERT(true))));
        bool assert_maybe_disabled = (!assert_runs_expression || assert_expand_len <= 16);
        HvkGui::Text("IM_ASSERT: runs expression: %s. expand size: %s%s",
            assert_runs_expression ? "OK" : "KO", (assert_expand_len > 16) ? "OK" : "KO", assert_maybe_disabled ? " (MAYBE DISABLED?!)" : "");
        if (assert_maybe_disabled)
        {
            HvkGui::SameLine();
            HelpMarker("IM_ASSERT() calls assert() by default. Compiling with NDEBUG will usually strip out assert() to nothing, which is NOT recommended because we use asserts to notify of programmer mistakes!");
        }

        HvkGui::Separator();
        HvkGui::Text("io.BackendPlatformName: %s", io.BackendPlatformName ? io.BackendPlatformName : "NULL");
        HvkGui::Text("io.BackendRendererName: %s", io.BackendRendererName ? io.BackendRendererName : "NULL");
        HvkGui::Text("io.ConfigFlags: 0x%08X", io.ConfigFlags);
        if (io.ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard)        HvkGui::Text(" NavEnableKeyboard");
        if (io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad)         HvkGui::Text(" NavEnableGamepad");
        if (io.ConfigFlags & ImGuiConfigFlags_NoMouse)                  HvkGui::Text(" NoMouse");
        if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)      HvkGui::Text(" NoMouseCursorChange");
        if (io.ConfigFlags & ImGuiConfigFlags_NoKeyboard)               HvkGui::Text(" NoKeyboard");
        if (io.MouseDrawCursor)                                         HvkGui::Text("io.MouseDrawCursor");
        if (io.ConfigMacOSXBehaviors)                                   HvkGui::Text("io.ConfigMacOSXBehaviors");
        if (io.ConfigNavMoveSetMousePos)                                HvkGui::Text("io.ConfigNavMoveSetMousePos");
        if (io.ConfigNavCaptureKeyboard)                                HvkGui::Text("io.ConfigNavCaptureKeyboard");
        if (io.ConfigInputTextCursorBlink)                              HvkGui::Text("io.ConfigInputTextCursorBlink");
        if (io.ConfigWindowsResizeFromEdges)                            HvkGui::Text("io.ConfigWindowsResizeFromEdges");
        if (io.ConfigWindowsMoveFromTitleBarOnly)                       HvkGui::Text("io.ConfigWindowsMoveFromTitleBarOnly");
        if (io.ConfigMemoryCompactTimer >= 0.0f)                        HvkGui::Text("io.ConfigMemoryCompactTimer = %.1f", io.ConfigMemoryCompactTimer);
        HvkGui::Text("io.BackendFlags: 0x%08X", io.BackendFlags);
        if (io.BackendFlags & ImGuiBackendFlags_HasGamepad)             HvkGui::Text(" HasGamepad");
        if (io.BackendFlags & ImGuiBackendFlags_HasMouseCursors)        HvkGui::Text(" HasMouseCursors");
        if (io.BackendFlags & ImGuiBackendFlags_HasSetMousePos)         HvkGui::Text(" HasSetMousePos");
        if (io.BackendFlags & ImGuiBackendFlags_RendererHasVtxOffset)   HvkGui::Text(" RendererHasVtxOffset");
        if (io.BackendFlags & ImGuiBackendFlags_RendererHasTextures)    HvkGui::Text(" RendererHasTextures");
        HvkGui::Separator();
        HvkGui::Text("io.Fonts: %d fonts, Flags: 0x%08X, TexSize: %d,%d", io.Fonts->Fonts.Size, io.Fonts->Flags, io.Fonts->TexData->Width, io.Fonts->TexData->Height);
        HvkGui::Text("io.Fonts->FontLoaderName: %s", io.Fonts->FontLoaderName ? io.Fonts->FontLoaderName : "NULL");
        HvkGui::Text("io.DisplaySize: %.2f,%.2f", io.DisplaySize.x, io.DisplaySize.y);
        HvkGui::Text("io.DisplayFramebufferScale: %.2f,%.2f", io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        HvkGui::Separator();
        HvkGui::Text("style.WindowPadding: %.2f,%.2f", style.WindowPadding.x, style.WindowPadding.y);
        HvkGui::Text("style.WindowBorderSize: %.2f", style.WindowBorderSize);
        HvkGui::Text("style.FramePadding: %.2f,%.2f", style.FramePadding.x, style.FramePadding.y);
        HvkGui::Text("style.FrameRounding: %.2f", style.FrameRounding);
        HvkGui::Text("style.FrameBorderSize: %.2f", style.FrameBorderSize);
        HvkGui::Text("style.ItemSpacing: %.2f,%.2f", style.ItemSpacing.x, style.ItemSpacing.y);
        HvkGui::Text("style.ItemInnerSpacing: %.2f,%.2f", style.ItemInnerSpacing.x, style.ItemInnerSpacing.y);

        if (copy_to_clipboard)
        {
            HvkGui::LogText("\n```\n");
            HvkGui::LogFinish();
        }
        HvkGui::EndChild();
    }
    HvkGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Style Editor / ShowStyleEditor()
//-----------------------------------------------------------------------------
// - ShowStyleSelector()
// - ShowStyleEditor()
//-----------------------------------------------------------------------------

// Demo helper function to select among default colors. See ShowStyleEditor() for more advanced options.
bool HvkGui::ShowStyleSelector(const char* label)
{
    // FIXME: This is a bit tricky to get right as style are functions, they don't register a name nor the fact that one is active.
    // So we keep track of last active one among our limited selection.
    static int style_idx = -1;
    const char* style_names[] = { "Dark", "Light", "Classic" };
    bool ret = false;
    if (HvkGui::BeginCombo(label, (style_idx >= 0 && style_idx < IM_ARRAYSIZE(style_names)) ? style_names[style_idx] : ""))
    {
        for (int n = 0; n < IM_ARRAYSIZE(style_names); n++)
        {
            if (HvkGui::Selectable(style_names[n], style_idx == n, ImGuiSelectableFlags_SelectOnNav))
            {
                style_idx = n;
                ret = true;
                switch (style_idx)
                {
                case 0: HvkGui::StyleColorsDark(); break;
                case 1: HvkGui::StyleColorsLight(); break;
                case 2: HvkGui::StyleColorsClassic(); break;
                }
            }
            else if (style_idx == n)
                HvkGui::SetItemDefaultFocus();
        }
        HvkGui::EndCombo();
    }
    return ret;
}

static const char* GetTreeLinesFlagsName(ImGuiTreeNodeFlags flags)
{
    if (flags == ImGuiTreeNodeFlags_DrawLinesNone) return "DrawLinesNone";
    if (flags == ImGuiTreeNodeFlags_DrawLinesFull) return "DrawLinesFull";
    if (flags == ImGuiTreeNodeFlags_DrawLinesToNodes) return "DrawLinesToNodes";
    return "";
}

// We omit the HvkGui:: prefix in this function, as we don't expect user to be copy and pasting this code.
void HvkGui::ShowStyleEditor(ImGuiStyle* ref)
{
    IMGUI_DEMO_MARKER("Tools/Style Editor");
    // You can pass in a reference ImGuiStyle structure to compare to, revert to and save to
    // (without a reference style pointer, we will use one compared locally as a reference)
    ImGuiStyle& style = GetStyle();
    static ImGuiStyle ref_saved_style;

    // Default to using internal storage as reference
    static bool init = true;
    if (init && ref == NULL)
        ref_saved_style = style;
    init = false;
    if (ref == NULL)
        ref = &ref_saved_style;

    PushItemWidth(GetWindowWidth() * 0.50f);

    {
        // General
        SeparatorText("General");
        if ((GetIO().BackendFlags & ImGuiBackendFlags_RendererHasTextures) == 0)
        {
            BulletText("Warning: Font scaling will NOT be smooth, because\nImGuiBackendFlags_RendererHasTextures is not set!");
            BulletText("For instructions, see:");
            SameLine();
            TextLinkOpenURL("docs/BACKENDS.md", "https://github.com/ocornut/imgui/blob/master/docs/BACKENDS.md");
        }

        if (ShowStyleSelector("Colors##Selector"))
            ref_saved_style = style;
        ShowFontSelector("Fonts##Selector");
        if (DragFloat("FontSizeBase", &style.FontSizeBase, 0.20f, 5.0f, 100.0f, "%.0f"))
            style._NextFrameFontSizeBase = style.FontSizeBase; // FIXME: Temporary hack until we finish remaining work.
        SameLine(0.0f, 0.0f); Text(" (out %.2f)", GetFontSize());
        DragFloat("FontScaleMain", &style.FontScaleMain, 0.02f, 0.5f, 4.0f);
        //BeginDisabled(GetIO().ConfigDpiScaleFonts);
        DragFloat("FontScaleDpi", &style.FontScaleDpi, 0.02f, 0.5f, 4.0f);
        //SetItemTooltip("When io.ConfigDpiScaleFonts is set, this value is automatically overwritten.");
        //EndDisabled();

        // Simplified Settings (expose floating-pointer border sizes as boolean representing 0.0f or 1.0f)
        if (SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f"))
            style.GrabRounding = style.FrameRounding; // Make GrabRounding always the same value as FrameRounding
        { bool border = (style.WindowBorderSize > 0.0f); if (Checkbox("WindowBorder", &border)) { style.WindowBorderSize = border ? 1.0f : 0.0f; } }
        SameLine();
        { bool border = (style.FrameBorderSize > 0.0f);  if (Checkbox("FrameBorder", &border)) { style.FrameBorderSize = border ? 1.0f : 0.0f; } }
        SameLine();
        { bool border = (style.PopupBorderSize > 0.0f);  if (Checkbox("PopupBorder", &border)) { style.PopupBorderSize = border ? 1.0f : 0.0f; } }
    }

    // Save/Revert button
    if (Button("Save Ref"))
        *ref = ref_saved_style = style;
    SameLine();
    if (Button("Revert Ref"))
        style = *ref;
    SameLine();
    HelpMarker(
        "Save/Revert in local non-persistent storage. Default Colors definition are not affected. "
        "Use \"Export\" below to save them somewhere.");

    SeparatorText("Details");
    if (BeginTabBar("##tabs", ImGuiTabBarFlags_None))
    {
        if (BeginTabItem("Sizes"))
        {
            SeparatorText("Main");
            SliderFloat2("WindowPadding", (float*)&style.WindowPadding, 0.0f, 20.0f, "%.0f");
            SliderFloat2("FramePadding", (float*)&style.FramePadding, 0.0f, 20.0f, "%.0f");
            SliderFloat2("ItemSpacing", (float*)&style.ItemSpacing, 0.0f, 20.0f, "%.0f");
            SliderFloat2("ItemInnerSpacing", (float*)&style.ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
            SliderFloat2("TouchExtraPadding", (float*)&style.TouchExtraPadding, 0.0f, 10.0f, "%.0f");
            SliderFloat("IndentSpacing", &style.IndentSpacing, 0.0f, 30.0f, "%.0f");
            SliderFloat("GrabMinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");

            SeparatorText("Borders");
            SliderFloat("WindowBorderSize", &style.WindowBorderSize, 0.0f, 1.0f, "%.0f");
            SliderFloat("ChildBorderSize", &style.ChildBorderSize, 0.0f, 1.0f, "%.0f");
            SliderFloat("PopupBorderSize", &style.PopupBorderSize, 0.0f, 1.0f, "%.0f");
            SliderFloat("FrameBorderSize", &style.FrameBorderSize, 0.0f, 1.0f, "%.0f");

            SeparatorText("Rounding");
            SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 12.0f, "%.0f");
            SliderFloat("ChildRounding", &style.ChildRounding, 0.0f, 12.0f, "%.0f");
            SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f");
            SliderFloat("PopupRounding", &style.PopupRounding, 0.0f, 12.0f, "%.0f");
            SliderFloat("GrabRounding", &style.GrabRounding, 0.0f, 12.0f, "%.0f");

            SeparatorText("Scrollbar");
            SliderFloat("ScrollbarSize", &style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
            SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 12.0f, "%.0f");
            SliderFloat("ScrollbarPadding", &style.ScrollbarPadding, 0.0f, 10.0f, "%.0f");

            SeparatorText("Tabs");
            SliderFloat("TabBorderSize", &style.TabBorderSize, 0.0f, 1.0f, "%.0f");
            SliderFloat("TabBarBorderSize", &style.TabBarBorderSize, 0.0f, 2.0f, "%.0f");
            SliderFloat("TabBarOverlineSize", &style.TabBarOverlineSize, 0.0f, 3.0f, "%.0f");
            SameLine(); HelpMarker("Overline is only drawn over the selected tab when ImGuiTabBarFlags_DrawSelectedOverline is set.");
            DragFloat("TabMinWidthBase", &style.TabMinWidthBase, 0.5f, 1.0f, 500.0f, "%.0f");
            DragFloat("TabMinWidthShrink", &style.TabMinWidthShrink, 0.5f, 1.0f, 500.0f, "%0.f");
            DragFloat("TabCloseButtonMinWidthSelected", &style.TabCloseButtonMinWidthSelected, 0.5f, -1.0f, 100.0f, (style.TabCloseButtonMinWidthSelected < 0.0f) ? "%.0f (Always)" : "%.0f");
            DragFloat("TabCloseButtonMinWidthUnselected", &style.TabCloseButtonMinWidthUnselected, 0.5f, -1.0f, 100.0f, (style.TabCloseButtonMinWidthUnselected < 0.0f) ? "%.0f (Always)" : "%.0f");
            SliderFloat("TabRounding", &style.TabRounding, 0.0f, 12.0f, "%.0f");

            SeparatorText("Tables");
            SliderFloat2("CellPadding", (float*)&style.CellPadding, 0.0f, 20.0f, "%.0f");
            SliderAngle("TableAngledHeadersAngle", &style.TableAngledHeadersAngle, -50.0f, +50.0f);
            SliderFloat2("TableAngledHeadersTextAlign", (float*)&style.TableAngledHeadersTextAlign, 0.0f, 1.0f, "%.2f");

            SeparatorText("Trees");
            bool combo_open = BeginCombo("TreeLinesFlags", GetTreeLinesFlagsName(style.TreeLinesFlags));
            SameLine();
            HelpMarker("[Experimental] Tree lines may not work in all situations (e.g. using a clipper) and may incurs slight traversal overhead.\n\nImGuiTreeNodeFlags_DrawLinesFull is faster than ImGuiTreeNodeFlags_DrawLinesToNode.");
            if (combo_open)
            {
                const ImGuiTreeNodeFlags options[] = { ImGuiTreeNodeFlags_DrawLinesNone, ImGuiTreeNodeFlags_DrawLinesFull, ImGuiTreeNodeFlags_DrawLinesToNodes };
                for (ImGuiTreeNodeFlags option : options)
                    if (Selectable(GetTreeLinesFlagsName(option), style.TreeLinesFlags == option))
                        style.TreeLinesFlags = option;
                EndCombo();
            }
            SliderFloat("TreeLinesSize", &style.TreeLinesSize, 0.0f, 2.0f, "%.0f");
            SliderFloat("TreeLinesRounding", &style.TreeLinesRounding, 0.0f, 12.0f, "%.0f");

            SeparatorText("Windows");
            SliderFloat2("WindowTitleAlign", (float*)&style.WindowTitleAlign, 0.0f, 1.0f, "%.2f");
            SliderFloat("WindowBorderHoverPadding", &style.WindowBorderHoverPadding, 1.0f, 20.0f, "%.0f");
            int window_menu_button_position = style.WindowMenuButtonPosition + 1;
            if (Combo("WindowMenuButtonPosition", (int*)&window_menu_button_position, "None\0Left\0Right\0"))
                style.WindowMenuButtonPosition = (ImGuiDir)(window_menu_button_position - 1);

            SeparatorText("Widgets");
            SliderFloat("ColorMarkerSize", &style.ColorMarkerSize, 0.0f, 8.0f, "%.0f");
            Combo("ColorButtonPosition", (int*)&style.ColorButtonPosition, "Left\0Right\0");
            SliderFloat2("ButtonTextAlign", (float*)&style.ButtonTextAlign, 0.0f, 1.0f, "%.2f");
            SameLine(); HelpMarker("Alignment applies when a button is larger than its text content.");
            SliderFloat2("SelectableTextAlign", (float*)&style.SelectableTextAlign, 0.0f, 1.0f, "%.2f");
            SameLine(); HelpMarker("Alignment applies when a selectable is larger than its text content.");
            SliderFloat("SeparatorTextBorderSize", &style.SeparatorTextBorderSize, 0.0f, 10.0f, "%.0f");
            SliderFloat2("SeparatorTextAlign", (float*)&style.SeparatorTextAlign, 0.0f, 1.0f, "%.2f");
            SliderFloat2("SeparatorTextPadding", (float*)&style.SeparatorTextPadding, 0.0f, 40.0f, "%.0f");
            SliderFloat("LogSliderDeadzone", &style.LogSliderDeadzone, 0.0f, 12.0f, "%.0f");
            SliderFloat("ImageBorderSize", &style.ImageBorderSize, 0.0f, 1.0f, "%.0f");

            SeparatorText("Tooltips");
            for (int n = 0; n < 2; n++)
                if (TreeNodeEx(n == 0 ? "HoverFlagsForTooltipMouse" : "HoverFlagsForTooltipNav"))
                {
                    ImGuiHoveredFlags* p = (n == 0) ? &style.HoverFlagsForTooltipMouse : &style.HoverFlagsForTooltipNav;
                    CheckboxFlags("ImGuiHoveredFlags_DelayNone", p, ImGuiHoveredFlags_DelayNone);
                    CheckboxFlags("ImGuiHoveredFlags_DelayShort", p, ImGuiHoveredFlags_DelayShort);
                    CheckboxFlags("ImGuiHoveredFlags_DelayNormal", p, ImGuiHoveredFlags_DelayNormal);
                    CheckboxFlags("ImGuiHoveredFlags_Stationary", p, ImGuiHoveredFlags_Stationary);
                    CheckboxFlags("ImGuiHoveredFlags_NoSharedDelay", p, ImGuiHoveredFlags_NoSharedDelay);
                    TreePop();
                }

            SeparatorText("Misc");
            SliderFloat2("DisplayWindowPadding", (float*)&style.DisplayWindowPadding, 0.0f, 30.0f, "%.0f"); SameLine(); HelpMarker("Apply to regular windows: amount which we enforce to keep visible when moving near edges of your screen.");
            SliderFloat2("DisplaySafeAreaPadding", (float*)&style.DisplaySafeAreaPadding, 0.0f, 30.0f, "%.0f"); SameLine(); HelpMarker("Apply to every windows, menus, popups, tooltips: amount where we avoid displaying contents. Adjust if you cannot see the edges of your screen (e.g. on a TV where scaling has not been configured).");

            EndTabItem();
        }

        if (BeginTabItem("Colors"))
        {
            static int output_dest = 0;
            static bool output_only_modified = true;
            if (Button("Export"))
            {
                if (output_dest == 0)
                    LogToClipboard();
                else
                    LogToTTY();
                LogText("ImVec4* colors = GetStyle().Colors;" IM_NEWLINE);
                for (int i = 0; i < ImGuiCol_COUNT; i++)
                {
                    const ImVec4& col = style.Colors[i];
                    const char* name = GetStyleColorName(i);
                    if (!output_only_modified || memcmp(&col, &ref->Colors[i], sizeof(ImVec4)) != 0)
                        LogText("colors[ImGuiCol_%s]%*s= ImVec4(%.2ff, %.2ff, %.2ff, %.2ff);" IM_NEWLINE,
                            name, 23 - (int)strlen(name), "", col.x, col.y, col.z, col.w);
                }
                LogFinish();
            }
            SameLine(); SetNextItemWidth(GetFontSize() * 10); Combo("##output_type", &output_dest, "To Clipboard\0To TTY\0");
            SameLine(); Checkbox("Only Modified Colors", &output_only_modified);

            static ImGuiTextFilter filter;
            filter.Draw("Filter colors", GetFontSize() * 16);

            static ImGuiColorEditFlags alpha_flags = 0;
            if (RadioButton("Opaque", alpha_flags == ImGuiColorEditFlags_AlphaOpaque))       { alpha_flags = ImGuiColorEditFlags_AlphaOpaque; } SameLine();
            if (RadioButton("Alpha",  alpha_flags == ImGuiColorEditFlags_None))              { alpha_flags = ImGuiColorEditFlags_None; } SameLine();
            if (RadioButton("Both",   alpha_flags == ImGuiColorEditFlags_AlphaPreviewHalf))  { alpha_flags = ImGuiColorEditFlags_AlphaPreviewHalf; } SameLine();
            HelpMarker(
                "In the color list:\n"
                "Left-click on color square to open color picker,\n"
                "Right-click to open edit options menu.");

            SetNextWindowSizeConstraints(ImVec2(0.0f, GetTextLineHeightWithSpacing() * 10), ImVec2(FLT_MAX, FLT_MAX));
            BeginChild("##colors", ImVec2(0, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);
            PushItemWidth(GetFontSize() * -12);
            for (int i = 0; i < ImGuiCol_COUNT; i++)
            {
                const char* name = GetStyleColorName(i);
                if (!filter.PassFilter(name))
                    continue;
                PushID(i);
#ifndef IMGUI_DISABLE_DEBUG_TOOLS
                if (Button("?"))
                    DebugFlashStyleColor((ImGuiCol)i);
                SetItemTooltip("Flash given color to identify places where it is used.");
                SameLine();
#endif
                ColorEdit4("##color", (float*)&style.Colors[i], ImGuiColorEditFlags_AlphaBar | alpha_flags);
                if (memcmp(&style.Colors[i], &ref->Colors[i], sizeof(ImVec4)) != 0)
                {
                    // Tips: in a real user application, you may want to merge and use an icon font into the main font,
                    // so instead of "Save"/"Revert" you'd use icons!
                    // Read the FAQ and docs/FONTS.md about using icon fonts. It's really easy and super convenient!
                    SameLine(0.0f, style.ItemInnerSpacing.x); if (Button("Save")) { ref->Colors[i] = style.Colors[i]; }
                    SameLine(0.0f, style.ItemInnerSpacing.x); if (Button("Revert")) { style.Colors[i] = ref->Colors[i]; }
                }
                SameLine(0.0f, style.ItemInnerSpacing.x);
                TextUnformatted(name);
                PopID();
            }
            PopItemWidth();
            EndChild();

            EndTabItem();
        }

        if (BeginTabItem("Fonts"))
        {
            ImGuiIO& io = GetIO();
            ImFontAtlas* atlas = io.Fonts;
            ShowFontAtlas(atlas);

            // Post-baking font scaling. Note that this is NOT the nice way of scaling fonts, read below.
            // (we enforce hard clamping manually as by default DragFloat/SliderFloat allows Ctrl+Click text to get out of bounds).
            /*
            SeparatorText("Legacy Scaling");
            const float MIN_SCALE = 0.3f;
            const float MAX_SCALE = 2.0f;
            HelpMarker(
                "Those are old settings provided for convenience.\n"
                "However, the _correct_ way of scaling your UI is currently to reload your font at the designed size, "
                "rebuild the font atlas, and call style.ScaleAllSizes() on a reference ImGuiStyle structure.\n"
                "Using those settings here will give you poor quality results.");
            PushItemWidth(GetFontSize() * 8);
            DragFloat("global scale", &io.FontGlobalScale, 0.005f, MIN_SCALE, MAX_SCALE, "%.2f", ImGuiSliderFlags_AlwaysClamp); // Scale everything
            //static float window_scale = 1.0f;
            //if (DragFloat("window scale", &window_scale, 0.005f, MIN_SCALE, MAX_SCALE, "%.2f", ImGuiSliderFlags_AlwaysClamp)) // Scale only this window
            //    SetWindowFontScale(window_scale);
            PopItemWidth();
            */

            EndTabItem();
        }

        if (BeginTabItem("Rendering"))
        {
            Checkbox("Anti-aliased lines", &style.AntiAliasedLines);
            SameLine();
            HelpMarker("When disabling anti-aliasing lines, you'll probably want to disable borders in your style as well.");

            Checkbox("Anti-aliased lines use texture", &style.AntiAliasedLinesUseTex);
            SameLine();
            HelpMarker("Faster lines using texture data. Require backend to render with bilinear filtering (not point/nearest filtering).");

            Checkbox("Anti-aliased fill", &style.AntiAliasedFill);
            PushItemWidth(GetFontSize() * 8);
            DragFloat("Curve Tessellation Tolerance", &style.CurveTessellationTol, 0.02f, 0.10f, 10.0f, "%.2f");
            if (style.CurveTessellationTol < 0.10f) style.CurveTessellationTol = 0.10f;

            // When editing the "Circle Segment Max Error" value, draw a preview of its effect on auto-tessellated circles.
            DragFloat("Circle Tessellation Max Error", &style.CircleTessellationMaxError , 0.005f, 0.10f, 5.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
            const bool show_samples = IsItemActive();
            if (show_samples)
                SetNextWindowPos(GetCursorScreenPos());
            if (show_samples && BeginTooltip())
            {
                TextUnformatted("(R = radius, N = approx number of segments)");
                Spacing();
                ImDrawList* draw_list = GetWindowDrawList();
                const float min_widget_width = CalcTextSize("R: MMM\nN: MMM").x;
                for (int n = 0; n < 8; n++)
                {
                    const float RAD_MIN = 5.0f;
                    const float RAD_MAX = 70.0f;
                    const float rad = RAD_MIN + (RAD_MAX - RAD_MIN) * (float)n / (8.0f - 1.0f);

                    BeginGroup();

                    // N is not always exact here due to how PathArcTo() function work internally
                    Text("R: %.f\nN: %d", rad, draw_list->_CalcCircleAutoSegmentCount(rad));

                    const float canvas_width = IM_MAX(min_widget_width, rad * 2.0f);
                    const float offset_x     = floorf(canvas_width * 0.5f);
                    const float offset_y     = floorf(RAD_MAX);

                    const ImVec2 p1 = GetCursorScreenPos();
                    draw_list->AddCircle(ImVec2(p1.x + offset_x, p1.y + offset_y), rad, GetColorU32(ImGuiCol_Text));
                    Dummy(ImVec2(canvas_width, RAD_MAX * 2));

                    /*
                    const ImVec2 p2 = GetCursorScreenPos();
                    draw_list->AddCircleFilled(ImVec2(p2.x + offset_x, p2.y + offset_y), rad, GetColorU32(ImGuiCol_Text));
                    Dummy(ImVec2(canvas_width, RAD_MAX * 2));
                    */

                    EndGroup();
                    SameLine();
                }
                EndTooltip();
            }
            SameLine();
            HelpMarker("When drawing circle primitives with \"num_segments == 0\" tessellation will be calculated automatically.");

            DragFloat("Global Alpha", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f"); // Not exposing zero here so user doesn't "lose" the UI (zero alpha clips all widgets). But application code could have a toggle to switch between zero and non-zero.
            DragFloat("Disabled Alpha", &style.DisabledAlpha, 0.005f, 0.0f, 1.0f, "%.2f"); SameLine(); HelpMarker("Additional alpha multiplier for disabled items (multiply over current value of Alpha).");
            PopItemWidth();

            EndTabItem();
        }

        EndTabBar();
    }
    PopItemWidth();
}

//-----------------------------------------------------------------------------
// [SECTION] User Guide / ShowUserGuide()
//-----------------------------------------------------------------------------

// We omit the HvkGui:: prefix in this function, as we don't expect user to be copy and pasting this code.
void HvkGui::ShowUserGuide()
{
    ImGuiIO& io = GetIO();
    BulletText("Double-click on title bar to collapse window.");
    BulletText(
        "Click and drag on lower corner or border to resize window.\n"
        "(double-click to auto fit window to its contents)");
    BulletText("Ctrl+Click on a slider or drag box to input value as text.");
    BulletText("Tab/Shift+Tab to cycle through keyboard editable fields.");
    BulletText("Ctrl+Tab/Ctrl+Shift+Tab to focus windows.");
    if (io.FontAllowUserScaling)
        BulletText("Ctrl+Mouse Wheel to zoom window contents.");
    BulletText("While inputting text:\n");
    Indent();
    BulletText("Ctrl+Left/Right to word jump.");
    BulletText("Ctrl+A or double-click to select all.");
    BulletText("Ctrl+X/C/V to use clipboard cut/copy/paste.");
    BulletText("Ctrl+Z to undo, Ctrl+Y/Ctrl+Shift+Z to redo.");
    BulletText("Escape to revert.");
    Unindent();
    BulletText("With keyboard navigation enabled:");
    Indent();
    BulletText("Arrow keys or Home/End/PageUp/PageDown to navigate.");
    BulletText("Space to activate a widget.");
    BulletText("Return to input text into a widget.");
    BulletText("Escape to deactivate a widget, close popup,\nexit a child window or the menu layer, clear focus.");
    BulletText("Alt to jump to the menu layer of a window.");
    Unindent();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Main Menu Bar / ShowExampleAppMainMenuBar()
//-----------------------------------------------------------------------------
// - ShowExampleAppMainMenuBar()
// - ShowExampleMenuFile()
//-----------------------------------------------------------------------------

// Demonstrate creating a "main" fullscreen menu bar and populating it.
// Note the difference between BeginMainMenuBar() and BeginMenuBar():
// - BeginMenuBar() = menu-bar inside current window (which needs the ImGuiWindowFlags_MenuBar flag!)
// - BeginMainMenuBar() = helper to create menu-bar-sized window at the top of the main viewport + call BeginMenuBar() into it.
static void ShowExampleAppMainMenuBar()
{
    if (HvkGui::BeginMainMenuBar())
    {
        if (HvkGui::BeginMenu("File"))
        {
            ShowExampleMenuFile();
            HvkGui::EndMenu();
        }
        if (HvkGui::BeginMenu("Edit"))
        {
            if (HvkGui::MenuItem("Undo", "Ctrl+Z")) {}
            if (HvkGui::MenuItem("Redo", "Ctrl+Y", false, false)) {} // Disabled item
            HvkGui::Separator();
            if (HvkGui::MenuItem("Cut", "Ctrl+X")) {}
            if (HvkGui::MenuItem("Copy", "Ctrl+C")) {}
            if (HvkGui::MenuItem("Paste", "Ctrl+V")) {}
            HvkGui::EndMenu();
        }
        HvkGui::EndMainMenuBar();
    }
}

// Note that shortcuts are currently provided for display only
// (future version will add explicit flags to BeginMenu() to request processing shortcuts)
static void ShowExampleMenuFile()
{
    IMGUI_DEMO_MARKER("Examples/Menu");
    HvkGui::MenuItem("(demo menu)", NULL, false, false);
    if (HvkGui::MenuItem("New")) {}
    if (HvkGui::MenuItem("Open", "Ctrl+O")) {}
    if (HvkGui::BeginMenu("Open Recent"))
    {
        HvkGui::MenuItem("fish_hat.c");
        HvkGui::MenuItem("fish_hat.inl");
        HvkGui::MenuItem("fish_hat.h");
        if (HvkGui::BeginMenu("More.."))
        {
            HvkGui::MenuItem("Hello");
            HvkGui::MenuItem("Sailor");
            if (HvkGui::BeginMenu("Recurse.."))
            {
                ShowExampleMenuFile();
                HvkGui::EndMenu();
            }
            HvkGui::EndMenu();
        }
        HvkGui::EndMenu();
    }
    if (HvkGui::MenuItem("Save", "Ctrl+S")) {}
    if (HvkGui::MenuItem("Save As..")) {}

    HvkGui::Separator();
    IMGUI_DEMO_MARKER("Examples/Menu/Options");
    if (HvkGui::BeginMenu("Options"))
    {
        static bool enabled = true;
        HvkGui::MenuItem("Enabled", "", &enabled);
        HvkGui::BeginChild("child", ImVec2(0, 60), ImGuiChildFlags_Borders);
        for (int i = 0; i < 10; i++)
            HvkGui::Text("Scrolling Text %d", i);
        HvkGui::EndChild();
        static float f = 0.5f;
        static int n = 0;
        HvkGui::SliderFloat("Value", &f, 0.0f, 1.0f);
        HvkGui::InputFloat("Input", &f, 0.1f);
        HvkGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
        HvkGui::EndMenu();
    }

    IMGUI_DEMO_MARKER("Examples/Menu/Colors");
    if (HvkGui::BeginMenu("Colors"))
    {
        float sz = HvkGui::GetTextLineHeight();
        for (int i = 0; i < ImGuiCol_COUNT; i++)
        {
            const char* name = HvkGui::GetStyleColorName((ImGuiCol)i);
            ImVec2 p = HvkGui::GetCursorScreenPos();
            HvkGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), HvkGui::GetColorU32((ImGuiCol)i));
            HvkGui::Dummy(ImVec2(sz, sz));
            HvkGui::SameLine();
            HvkGui::MenuItem(name);
        }
        HvkGui::EndMenu();
    }

    // Here we demonstrate appending again to the "Options" menu (which we already created above)
    // Of course in this demo it is a little bit silly that this function calls BeginMenu("Options") twice.
    // In a real code-base using it would make senses to use this feature from very different code locations.
    if (HvkGui::BeginMenu("Options")) // <-- Append!
    {
        IMGUI_DEMO_MARKER("Examples/Menu/Append to an existing menu");
        static bool b = true;
        HvkGui::Checkbox("SomeOption", &b);
        HvkGui::EndMenu();
    }

    if (HvkGui::BeginMenu("Disabled", false)) // Disabled
    {
        IM_ASSERT(0);
    }
    if (HvkGui::MenuItem("Checked", NULL, true)) {}
    HvkGui::Separator();
    if (HvkGui::MenuItem("Quit", "Alt+F4")) {}
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Debug Console / ShowExampleAppConsole()
//-----------------------------------------------------------------------------

// Demonstrate creating a simple console window, with scrolling, filtering, completion and history.
// For the console example, we are using a more C++ like approach of declaring a class to hold both data and functions.
struct ExampleAppConsole
{
    char                  InputBuf[256];
    ImVector<char*>       Items;
    ImVector<const char*> Commands;
    ImVector<char*>       History;
    int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
    ImGuiTextFilter       Filter;
    bool                  AutoScroll;
    bool                  ScrollToBottom;

    ExampleAppConsole()
    {
        IMGUI_DEMO_MARKER("Examples/Console");
        ClearLog();
        memset(InputBuf, 0, sizeof(InputBuf));
        HistoryPos = -1;

        // "CLASSIFY" is here to provide the test case where "C"+[tab] completes to "CL" and display multiple matches.
        Commands.push_back("HELP");
        Commands.push_back("HISTORY");
        Commands.push_back("CLEAR");
        Commands.push_back("CLASSIFY");
        AutoScroll = true;
        ScrollToBottom = false;
        AddLog("Welcome to Dear ImGui!");
    }
    ~ExampleAppConsole()
    {
        ClearLog();
        for (int i = 0; i < History.Size; i++)
            HvkGui::MemFree(History[i]);
    }

    // Portable helpers
    static int   Stricmp(const char* s1, const char* s2)         { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
    static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
    static char* Strdup(const char* s)                           { IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = HvkGui::MemAlloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
    static void  Strtrim(char* s)                                { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

    void    ClearLog()
    {
        for (int i = 0; i < Items.Size; i++)
            HvkGui::MemFree(Items[i]);
        Items.clear();
    }

    void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        // FIXME-OPT
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf)-1] = 0;
        va_end(args);
        Items.push_back(Strdup(buf));
    }

    void    Draw(const char* title, bool* p_open)
    {
        HvkGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        if (!HvkGui::Begin(title, p_open))
        {
            HvkGui::End();
            return;
        }

        // As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar.
        // So e.g. IsItemHovered() will return true when hovering the title bar.
        // Here we create a context menu only available from the title bar.
        if (HvkGui::BeginPopupContextItem())
        {
            if (HvkGui::MenuItem("Close Console"))
                *p_open = false;
            HvkGui::EndPopup();
        }

        HvkGui::TextWrapped(
            "This example implements a console with basic coloring, completion (TAB key) and history (Up/Down keys). A more elaborate "
            "implementation may want to store entries along with extra data such as timestamp, emitter, etc.");
        HvkGui::TextWrapped("Enter 'HELP' for help.");

        // TODO: display items starting from the bottom

        if (HvkGui::SmallButton("Add Debug Text"))  { AddLog("%d some text", Items.Size); AddLog("some more text"); AddLog("display very important message here!"); }
        HvkGui::SameLine();
        if (HvkGui::SmallButton("Add Debug Error")) { AddLog("[error] something went wrong"); }
        HvkGui::SameLine();
        if (HvkGui::SmallButton("Clear"))           { ClearLog(); }
        HvkGui::SameLine();
        bool copy_to_clipboard = HvkGui::SmallButton("Copy");
        //static float t = 0.0f; if (HvkGui::GetTime() - t > 0.02f) { t = HvkGui::GetTime(); AddLog("Spam %f", t); }

        HvkGui::Separator();

        // Options menu
        if (HvkGui::BeginPopup("Options"))
        {
            HvkGui::Checkbox("Auto-scroll", &AutoScroll);
            HvkGui::EndPopup();
        }

        // Options, Filter
        HvkGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_O, ImGuiInputFlags_Tooltip);
        if (HvkGui::Button("Options"))
            HvkGui::OpenPopup("Options");
        HvkGui::SameLine();
        Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
        HvkGui::Separator();

        // Reserve enough left-over height for 1 separator + 1 input text
        const float footer_height_to_reserve = HvkGui::GetStyle().ItemSpacing.y + HvkGui::GetFrameHeightWithSpacing();
        if (HvkGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (HvkGui::BeginPopupContextWindow())
            {
                if (HvkGui::Selectable("Clear")) ClearLog();
                HvkGui::EndPopup();
            }

            // Display every line as a separate entry so we can change their color or add custom widgets.
            // If you only want raw text you can use HvkGui::TextUnformatted(log.begin(), log.end());
            // NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping
            // to only process visible items. The clipper will automatically measure the height of your first item and then
            // "seek" to display only items in the visible area.
            // To use the clipper we can replace your standard loop:
            //      for (int i = 0; i < Items.Size; i++)
            //   With:
            //      ImGuiListClipper clipper;
            //      clipper.Begin(Items.Size);
            //      while (clipper.Step())
            //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
            // - That your items are evenly spaced (same height)
            // - That you have cheap random access to your elements (you can access them given their index,
            //   without processing all the ones before)
            // You cannot this code as-is if a filter is active because it breaks the 'cheap random-access' property.
            // We would need random-access on the post-filtered list.
            // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices
            // or offsets of items that passed the filtering test, recomputing this array when user changes the filter,
            // and appending newly elements as they are inserted. This is left as a task to the user until we can manage
            // to improve this example code!
            // If your items are of variable height:
            // - Split them into same height items would be simpler and facilitate random-seeking into your list.
            // - Consider using manual call to IsRectVisible() and skipping extraneous decoration from your items.
            HvkGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
            if (copy_to_clipboard)
                HvkGui::LogToClipboard();
            for (const char* item : Items)
            {
                if (!Filter.PassFilter(item))
                    continue;

                // Normally you would store more information in your item than just a string.
                // (e.g. make Items[] an array of structure, store color/type etc.)
                ImVec4 color;
                bool has_color = false;
                if (strstr(item, "[error]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
                else if (strncmp(item, "# ", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
                if (has_color)
                    HvkGui::PushStyleColor(ImGuiCol_Text, color);
                HvkGui::TextUnformatted(item);
                if (has_color)
                    HvkGui::PopStyleColor();
            }
            if (copy_to_clipboard)
                HvkGui::LogFinish();

            // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
            // Using a scrollbar or mouse-wheel will take away from the bottom edge.
            if (ScrollToBottom || (AutoScroll && HvkGui::GetScrollY() >= HvkGui::GetScrollMaxY()))
                HvkGui::SetScrollHereY(1.0f);
            ScrollToBottom = false;

            HvkGui::PopStyleVar();
        }
        HvkGui::EndChild();
        HvkGui::Separator();

        // Command-line
        bool reclaim_focus = false;
        ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
        if (HvkGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &TextEditCallbackStub, (void*)this))
        {
            char* s = InputBuf;
            Strtrim(s);
            if (s[0])
                ExecCommand(s);
            strcpy(s, "");
            reclaim_focus = true;
        }

        // Auto-focus on window apparition
        HvkGui::SetItemDefaultFocus();
        if (reclaim_focus)
            HvkGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

        HvkGui::End();
    }

    void    ExecCommand(const char* command_line)
    {
        AddLog("# %s\n", command_line);

        // Insert into history. First find match and delete it so it can be pushed to the back.
        // This isn't trying to be smart or optimal.
        HistoryPos = -1;
        for (int i = History.Size - 1; i >= 0; i--)
            if (Stricmp(History[i], command_line) == 0)
            {
                HvkGui::MemFree(History[i]);
                History.erase(History.begin() + i);
                break;
            }
        History.push_back(Strdup(command_line));

        // Process command
        if (Stricmp(command_line, "CLEAR") == 0)
        {
            ClearLog();
        }
        else if (Stricmp(command_line, "HELP") == 0)
        {
            AddLog("Commands:");
            for (int i = 0; i < Commands.Size; i++)
                AddLog("- %s", Commands[i]);
        }
        else if (Stricmp(command_line, "HISTORY") == 0)
        {
            int first = History.Size - 10;
            for (int i = first > 0 ? first : 0; i < History.Size; i++)
                AddLog("%3d: %s\n", i, History[i]);
        }
        else
        {
            AddLog("Unknown command: '%s'\n", command_line);
        }

        // On command input, we scroll to bottom even if AutoScroll==false
        ScrollToBottom = true;
    }

    // In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
    static int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
    {
        ExampleAppConsole* console = (ExampleAppConsole*)data->UserData;
        return console->TextEditCallback(data);
    }

    int     TextEditCallback(ImGuiInputTextCallbackData* data)
    {
        //AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
        switch (data->EventFlag)
        {
        case ImGuiInputTextFlags_CallbackCompletion:
            {
                // Example of TEXT COMPLETION

                // Locate beginning of current word
                const char* word_end = data->Buf + data->CursorPos;
                const char* word_start = word_end;
                while (word_start > data->Buf)
                {
                    const char c = word_start[-1];
                    if (c == ' ' || c == '\t' || c == ',' || c == ';')
                        break;
                    word_start--;
                }

                // Build a list of candidates
                ImVector<const char*> candidates;
                for (int i = 0; i < Commands.Size; i++)
                    if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) == 0)
                        candidates.push_back(Commands[i]);

                if (candidates.Size == 0)
                {
                    // No match
                    AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
                }
                else if (candidates.Size == 1)
                {
                    // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
                    data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                    data->InsertChars(data->CursorPos, candidates[0]);
                    data->InsertChars(data->CursorPos, " ");
                }
                else
                {
                    // Multiple matches. Complete as much as we can..
                    // So inputting "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
                    int match_len = (int)(word_end - word_start);
                    for (;;)
                    {
                        int c = 0;
                        bool all_candidates_matches = true;
                        for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                            if (i == 0)
                                c = toupper(candidates[i][match_len]);
                            else if (c == 0 || c != toupper(candidates[i][match_len]))
                                all_candidates_matches = false;
                        if (!all_candidates_matches)
                            break;
                        match_len++;
                    }

                    if (match_len > 0)
                    {
                        data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                        data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                    }

                    // List matches
                    AddLog("Possible matches:\n");
                    for (int i = 0; i < candidates.Size; i++)
                        AddLog("- %s\n", candidates[i]);
                }

                break;
            }
        case ImGuiInputTextFlags_CallbackHistory:
            {
                // Example of HISTORY
                const int prev_history_pos = HistoryPos;
                if (data->EventKey == ImGuiKey_UpArrow)
                {
                    if (HistoryPos == -1)
                        HistoryPos = History.Size - 1;
                    else if (HistoryPos > 0)
                        HistoryPos--;
                }
                else if (data->EventKey == ImGuiKey_DownArrow)
                {
                    if (HistoryPos != -1)
                        if (++HistoryPos >= History.Size)
                            HistoryPos = -1;
                }

                // A better implementation would preserve the data on the current input line along with cursor position.
                if (prev_history_pos != HistoryPos)
                {
                    const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
                    data->DeleteChars(0, data->BufTextLen);
                    data->InsertChars(0, history_str);
                }
            }
        }
        return 0;
    }
};

static void ShowExampleAppConsole(bool* p_open)
{
    static ExampleAppConsole console;
    console.Draw("Example: Console", p_open);
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Debug Log / ShowExampleAppLog()
//-----------------------------------------------------------------------------

// Usage:
//  static ExampleAppLog my_log;
//  my_log.AddLog("Hello %d world\n", 123);
//  my_log.Draw("title");
struct ExampleAppLog
{
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
    bool                AutoScroll;  // Keep scrolling if already at the bottom.

    ExampleAppLog()
    {
        AutoScroll = true;
        Clear();
    }

    void    Clear()
    {
        Buf.clear();
        LineOffsets.clear();
        LineOffsets.push_back(0);
    }

    void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        int old_size = Buf.size();
        va_list args;
        va_start(args, fmt);
        Buf.appendfv(fmt, args);
        va_end(args);
        for (int new_size = Buf.size(); old_size < new_size; old_size++)
            if (Buf[old_size] == '\n')
                LineOffsets.push_back(old_size + 1);
    }

    void    Draw(const char* title, bool* p_open = NULL)
    {
        if (!HvkGui::Begin(title, p_open))
        {
            HvkGui::End();
            return;
        }

        // Options menu
        if (HvkGui::BeginPopup("Options"))
        {
            HvkGui::Checkbox("Auto-scroll", &AutoScroll);
            HvkGui::EndPopup();
        }

        // Main window
        if (HvkGui::Button("Options"))
            HvkGui::OpenPopup("Options");
        HvkGui::SameLine();
        bool clear = HvkGui::Button("Clear");
        HvkGui::SameLine();
        bool copy = HvkGui::Button("Copy");
        HvkGui::SameLine();
        Filter.Draw("Filter", -100.0f);

        HvkGui::Separator();

        if (HvkGui::BeginChild("scrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (clear)
                Clear();
            if (copy)
                HvkGui::LogToClipboard();

            HvkGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            const char* buf = Buf.begin();
            const char* buf_end = Buf.end();
            if (Filter.IsActive())
            {
                // In this example we don't use the clipper when Filter is enabled.
                // This is because we don't have random access to the result of our filter.
                // A real application processing logs with ten of thousands of entries may want to store the result of
                // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
                for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
                {
                    const char* line_start = buf + LineOffsets[line_no];
                    const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                    if (Filter.PassFilter(line_start, line_end))
                        HvkGui::TextUnformatted(line_start, line_end);
                }
            }
            else
            {
                // The simplest and easy way to display the entire buffer:
                //   HvkGui::TextUnformatted(buf_begin, buf_end);
                // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
                // to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
                // within the visible area.
                // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
                // on your side is recommended. Using ImGuiListClipper requires
                // - A) random access into your data
                // - B) items all being the  same height,
                // both of which we can handle since we have an array pointing to the beginning of each line of text.
                // When using the filter (in the block of code above) we don't have random access into the data to display
                // anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
                // it possible (and would be recommended if you want to search through tens of thousands of entries).
                ImGuiListClipper clipper;
                clipper.Begin(LineOffsets.Size);
                while (clipper.Step())
                {
                    for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                    {
                        const char* line_start = buf + LineOffsets[line_no];
                        const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                        HvkGui::TextUnformatted(line_start, line_end);
                    }
                }
                clipper.End();
            }
            HvkGui::PopStyleVar();

            // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
            // Using a scrollbar or mouse-wheel will take away from the bottom edge.
            if (AutoScroll && HvkGui::GetScrollY() >= HvkGui::GetScrollMaxY())
                HvkGui::SetScrollHereY(1.0f);
        }
        HvkGui::EndChild();
        HvkGui::End();
    }
};

// Demonstrate creating a simple log window with basic filtering.
static void ShowExampleAppLog(bool* p_open)
{
    static ExampleAppLog log;

    // For the demo: add a debug button _BEFORE_ the normal log window contents
    // We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
    // Most of the contents of the window will be added by the log.Draw() call.
    HvkGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    HvkGui::Begin("Example: Log", p_open);
    IMGUI_DEMO_MARKER("Examples/Log");
    if (HvkGui::SmallButton("[Debug] Add 5 entries"))
    {
        static int counter = 0;
        const char* categories[3] = { "info", "warn", "error" };
        const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
        for (int n = 0; n < 5; n++)
        {
            const char* category = categories[counter % IM_ARRAYSIZE(categories)];
            const char* word = words[counter % IM_ARRAYSIZE(words)];
            log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
                HvkGui::GetFrameCount(), category, HvkGui::GetTime(), word);
            counter++;
        }
    }
    HvkGui::End();

    // Actually call in the regular Log helper (which will Begin() into the same window as we just did)
    log.Draw("Example: Log", p_open);
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Simple Layout / ShowExampleAppLayout()
//-----------------------------------------------------------------------------

// Demonstrate create a window with multiple child windows.
static void ShowExampleAppLayout(bool* p_open)
{
    HvkGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (HvkGui::Begin("Example: Simple layout", p_open, ImGuiWindowFlags_MenuBar))
    {
        IMGUI_DEMO_MARKER("Examples/Simple layout");
        if (HvkGui::BeginMenuBar())
        {
            if (HvkGui::BeginMenu("File"))
            {
                if (HvkGui::MenuItem("Close", "Ctrl+W")) { *p_open = false; }
                HvkGui::EndMenu();
            }
            HvkGui::EndMenuBar();
        }

        // Left
        static int selected = 0;
        {
            HvkGui::BeginChild("left pane", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
            for (int i = 0; i < 100; i++)
            {
                char label[128];
                sprintf(label, "MyObject %d", i);
                if (HvkGui::Selectable(label, selected == i, ImGuiSelectableFlags_SelectOnNav))
                    selected = i;
            }
            HvkGui::EndChild();
        }
        HvkGui::SameLine();

        // Right
        {
            HvkGui::BeginGroup();
            HvkGui::BeginChild("item view", ImVec2(0, -HvkGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
            HvkGui::Text("MyObject: %d", selected);
            HvkGui::Separator();
            if (HvkGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
            {
                if (HvkGui::BeginTabItem("Description"))
                {
                    HvkGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
                    HvkGui::EndTabItem();
                }
                if (HvkGui::BeginTabItem("Details"))
                {
                    HvkGui::Text("ID: 0123456789");
                    HvkGui::EndTabItem();
                }
                HvkGui::EndTabBar();
            }
            HvkGui::EndChild();
            if (HvkGui::Button("Revert")) {}
            HvkGui::SameLine();
            if (HvkGui::Button("Save")) {}
            HvkGui::EndGroup();
        }
    }
    HvkGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Property Editor / ShowExampleAppPropertyEditor()
//-----------------------------------------------------------------------------
// Some of the interactions are a bit lack-luster:
// - We would want pressing validating or leaving the filter to somehow restore focus.
// - We may want more advanced filtering (child nodes) and clipper support: both will need extra work.
// - We would want to customize some keyboard interactions to easily keyboard navigate between the tree and the properties.
//-----------------------------------------------------------------------------

struct ExampleAppPropertyEditor
{
    ImGuiTextFilter     Filter;
    ExampleTreeNode*    VisibleNode = NULL;

    void Draw(ExampleTreeNode* root_node)
    {
        // Left side: draw tree
        // - Currently using a table to benefit from RowBg feature
        if (HvkGui::BeginChild("##tree", ImVec2(300, 0), ImGuiChildFlags_ResizeX | ImGuiChildFlags_Borders | ImGuiChildFlags_NavFlattened))
        {
            HvkGui::SetNextItemWidth(-FLT_MIN);
            HvkGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_F, ImGuiInputFlags_Tooltip);
            HvkGui::PushItemFlag(ImGuiItemFlags_NoNavDefaultFocus, true);
            if (HvkGui::InputTextWithHint("##Filter", "incl,-excl", Filter.InputBuf, IM_ARRAYSIZE(Filter.InputBuf), ImGuiInputTextFlags_EscapeClearsAll))
                Filter.Build();
            HvkGui::PopItemFlag();

            if (HvkGui::BeginTable("##bg", 1, ImGuiTableFlags_RowBg))
            {
                for (ExampleTreeNode* node : root_node->Childs)
                    if (Filter.PassFilter(node->Name)) // Filter root node
                        DrawTreeNode(node);
                HvkGui::EndTable();
            }
        }
        HvkGui::EndChild();

        // Right side: draw properties
        HvkGui::SameLine();

        HvkGui::BeginGroup(); // Lock X position
        if (ExampleTreeNode* node = VisibleNode)
        {
            HvkGui::Text("%s", node->Name);
            HvkGui::TextDisabled("UID: 0x%08X", node->UID);
            HvkGui::Separator();
            if (HvkGui::BeginTable("##properties", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY))
            {
                // Push object ID after we entered the table, so table is shared for all objects
                HvkGui::PushID((int)node->UID);
                HvkGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
                HvkGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 2.0f); // Default twice larger
                if (node->HasData)
                {
                    // In a typical application, the structure description would be derived from a data-driven system.
                    // - We try to mimic this with our ExampleMemberInfo structure and the ExampleTreeNodeMemberInfos[] array.
                    // - Limits and some details are hard-coded to simplify the demo.
                    for (const ExampleMemberInfo& field_desc : ExampleTreeNodeMemberInfos)
                    {
                        HvkGui::TableNextRow();
                        HvkGui::PushID(field_desc.Name);
                        HvkGui::TableNextColumn();
                        HvkGui::AlignTextToFramePadding();
                        HvkGui::TextUnformatted(field_desc.Name);
                        HvkGui::TableNextColumn();
                        void* field_ptr = (void*)(((unsigned char*)node) + field_desc.Offset);
                        switch (field_desc.DataType)
                        {
                        case ImGuiDataType_Bool:
                        {
                            IM_ASSERT(field_desc.DataCount == 1);
                            HvkGui::Checkbox("##Editor", (bool*)field_ptr);
                            break;
                        }
                        case ImGuiDataType_S32:
                        {
                            int v_min = INT_MIN, v_max = INT_MAX;
                            HvkGui::SetNextItemWidth(-FLT_MIN);
                            HvkGui::DragScalarN("##Editor", field_desc.DataType, field_ptr, field_desc.DataCount, 1.0f, &v_min, &v_max);
                            break;
                        }
                        case ImGuiDataType_Float:
                        {
                            float v_min = 0.0f, v_max = 1.0f;
                            HvkGui::SetNextItemWidth(-FLT_MIN);
                            HvkGui::SliderScalarN("##Editor", field_desc.DataType, field_ptr, field_desc.DataCount, &v_min, &v_max);
                            break;
                        }
                        case ImGuiDataType_String:
                        {
                            HvkGui::InputText("##Editor", reinterpret_cast<char*>(field_ptr), 28);
                            break;
                        }
                        }
                        HvkGui::PopID();
                    }
                }
                HvkGui::PopID();
                HvkGui::EndTable();
            }
        }
        HvkGui::EndGroup();
    }

    void DrawTreeNode(ExampleTreeNode* node)
    {
        HvkGui::TableNextRow();
        HvkGui::TableNextColumn();
        HvkGui::PushID(node->UID);
        ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_None;
        tree_flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;// Standard opening mode as we are likely to want to add selection afterwards
        tree_flags |= ImGuiTreeNodeFlags_NavLeftJumpsToParent;  // Left arrow support
        tree_flags |= ImGuiTreeNodeFlags_SpanFullWidth;         // Span full width for easier mouse reach
        tree_flags |= ImGuiTreeNodeFlags_DrawLinesToNodes;      // Always draw hierarchy outlines
        if (node == VisibleNode)
            tree_flags |= ImGuiTreeNodeFlags_Selected;
        if (node->Childs.Size == 0)
            tree_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
        if (node->DataMyBool == false)
            HvkGui::PushStyleColor(ImGuiCol_Text, HvkGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
        bool node_open = HvkGui::TreeNodeEx("", tree_flags, "%s", node->Name);
        if (node->DataMyBool == false)
            HvkGui::PopStyleColor();
        if (HvkGui::IsItemFocused())
            VisibleNode = node;
        if (node_open)
        {
            for (ExampleTreeNode* child : node->Childs)
                DrawTreeNode(child);
            HvkGui::TreePop();
        }
        HvkGui::PopID();
    }
};

// Demonstrate creating a simple property editor.
static void ShowExampleAppPropertyEditor(bool* p_open, ImGuiDemoWindowData* demo_data)
{
    HvkGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
    if (!HvkGui::Begin("Example: Property editor", p_open))
    {
        HvkGui::End();
        return;
    }

    IMGUI_DEMO_MARKER("Examples/Property Editor");
    static ExampleAppPropertyEditor property_editor;
    if (demo_data->DemoTree == NULL)
        demo_data->DemoTree = ExampleTree_CreateDemoTree();
    property_editor.Draw(demo_data->DemoTree);

    HvkGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Long Text / ShowExampleAppLongText()
//-----------------------------------------------------------------------------

// Demonstrate/test rendering huge amount of text, and the incidence of clipping.
static void ShowExampleAppLongText(bool* p_open)
{
    HvkGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    if (!HvkGui::Begin("Example: Long text display", p_open))
    {
        HvkGui::End();
        return;
    }
    IMGUI_DEMO_MARKER("Examples/Long text display");

    static int test_type = 0;
    static ImGuiTextBuffer log;
    static int lines = 0;
    HvkGui::Text("Printing unusually long amount of text.");
    HvkGui::Combo("Test type", &test_type,
        "Single call to TextUnformatted()\0"
        "Multiple calls to Text(), clipped\0"
        "Multiple calls to Text(), not clipped (slow)\0");
    HvkGui::Text("Buffer contents: %d lines, %d bytes", lines, log.size());
    if (HvkGui::Button("Clear")) { log.clear(); lines = 0; }
    HvkGui::SameLine();
    if (HvkGui::Button("Add 1000 lines"))
    {
        for (int i = 0; i < 1000; i++)
            log.appendf("%i The quick brown fox jumps over the lazy dog\n", lines + i);
        lines += 1000;
    }
    HvkGui::BeginChild("Log");
    switch (test_type)
    {
    case 0:
        // Single call to TextUnformatted() with a big buffer
        HvkGui::TextUnformatted(log.begin(), log.end());
        break;
    case 1:
        {
            // Multiple calls to Text(), manually coarsely clipped - demonstrate how to use the ImGuiListClipper helper.
            HvkGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            ImGuiListClipper clipper;
            clipper.Begin(lines);
            while (clipper.Step())
                for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                    HvkGui::Text("%i The quick brown fox jumps over the lazy dog", i);
            HvkGui::PopStyleVar();
            break;
        }
    case 2:
        // Multiple calls to Text(), not clipped (slow)
        HvkGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        for (int i = 0; i < lines; i++)
            HvkGui::Text("%i The quick brown fox jumps over the lazy dog", i);
        HvkGui::PopStyleVar();
        break;
    }
    HvkGui::EndChild();
    HvkGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Auto Resize / ShowExampleAppAutoResize()
//-----------------------------------------------------------------------------

// Demonstrate creating a window which gets auto-resized according to its content.
static void ShowExampleAppAutoResize(bool* p_open)
{
    if (!HvkGui::Begin("Example: Auto-resizing window", p_open, ImGuiWindowFlags_AlwaysAutoResize))
    {
        HvkGui::End();
        return;
    }
    IMGUI_DEMO_MARKER("Examples/Auto-resizing window");

    static int lines = 10;
    HvkGui::TextUnformatted(
        "Window will resize every-frame to the size of its content.\n"
        "Note that you probably don't want to query the window size to\n"
        "output your content because that would create a feedback loop.");
    HvkGui::SliderInt("Number of lines", &lines, 1, 20);
    for (int i = 0; i < lines; i++)
        HvkGui::Text("%*sThis is line %d", i * 4, "", i); // Pad with space to extend size horizontally
    HvkGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Constrained Resize / ShowExampleAppConstrainedResize()
//-----------------------------------------------------------------------------

// Demonstrate creating a window with custom resize constraints.
// Note that size constraints currently don't work on a docked window (when in 'docking' branch)
static void ShowExampleAppConstrainedResize(bool* p_open)
{
    struct CustomConstraints
    {
        // Helper functions to demonstrate programmatic constraints
        // FIXME: This doesn't take account of decoration size (e.g. title bar), library should make this easier.
        // FIXME: None of the three demos works consistently when resizing from borders.
        static void AspectRatio(ImGuiSizeCallbackData* data)
        {
            float aspect_ratio = *(float*)data->UserData;
            data->DesiredSize.y = (float)(int)(data->DesiredSize.x / aspect_ratio);
        }
        static void Square(ImGuiSizeCallbackData* data)
        {
            data->DesiredSize.x = data->DesiredSize.y = IM_MAX(data->DesiredSize.x, data->DesiredSize.y);
        }
        static void Step(ImGuiSizeCallbackData* data)
        {
            float step = *(float*)data->UserData;
            data->DesiredSize = ImVec2((int)(data->DesiredSize.x / step + 0.5f) * step, (int)(data->DesiredSize.y / step + 0.5f) * step);
        }
    };

    const char* test_desc[] =
    {
        "Between 100x100 and 500x500",
        "At least 100x100",
        "Resize vertical + lock current width",
        "Resize horizontal + lock current height",
        "Width Between 400 and 500",
        "Height at least 400",
        "Custom: Aspect Ratio 16:9",
        "Custom: Always Square",
        "Custom: Fixed Steps (100)",
    };

    // Options
    static bool auto_resize = false;
    static bool window_padding = true;
    static int type = 6; // Aspect Ratio
    static int display_lines = 10;

    // Submit constraint
    float aspect_ratio = 16.0f / 9.0f;
    float fixed_step = 100.0f;
    if (type == 0) HvkGui::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(500, 500));         // Between 100x100 and 500x500
    if (type == 1) HvkGui::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(FLT_MAX, FLT_MAX)); // Width > 100, Height > 100
    if (type == 2) HvkGui::SetNextWindowSizeConstraints(ImVec2(-1, 0),    ImVec2(-1, FLT_MAX));      // Resize vertical + lock current width
    if (type == 3) HvkGui::SetNextWindowSizeConstraints(ImVec2(0, -1),    ImVec2(FLT_MAX, -1));      // Resize horizontal + lock current height
    if (type == 4) HvkGui::SetNextWindowSizeConstraints(ImVec2(400, -1),  ImVec2(500, -1));          // Width Between and 400 and 500
    if (type == 5) HvkGui::SetNextWindowSizeConstraints(ImVec2(-1, 400),  ImVec2(-1, FLT_MAX));      // Height at least 400
    if (type == 6) HvkGui::SetNextWindowSizeConstraints(ImVec2(0, 0),     ImVec2(FLT_MAX, FLT_MAX), CustomConstraints::AspectRatio, (void*)&aspect_ratio);   // Aspect ratio
    if (type == 7) HvkGui::SetNextWindowSizeConstraints(ImVec2(0, 0),     ImVec2(FLT_MAX, FLT_MAX), CustomConstraints::Square);                              // Always Square
    if (type == 8) HvkGui::SetNextWindowSizeConstraints(ImVec2(0, 0),     ImVec2(FLT_MAX, FLT_MAX), CustomConstraints::Step, (void*)&fixed_step);            // Fixed Step

    // Submit window
    if (!window_padding)
        HvkGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    const ImGuiWindowFlags window_flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;
    const bool window_open = HvkGui::Begin("Example: Constrained Resize", p_open, window_flags);
    if (!window_padding)
        HvkGui::PopStyleVar();
    if (window_open)
    {
        IMGUI_DEMO_MARKER("Examples/Constrained Resizing window");
        if (HvkGui::GetIO().KeyShift)
        {
            // Display a dummy viewport (in your real app you would likely use ImageButton() to display a texture)
            ImVec2 avail_size = HvkGui::GetContentRegionAvail();
            ImVec2 pos = HvkGui::GetCursorScreenPos();
            HvkGui::ColorButton("viewport", ImVec4(0.5f, 0.2f, 0.5f, 1.0f), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, avail_size);
            HvkGui::SetCursorScreenPos(ImVec2(pos.x + 10, pos.y + 10));
            HvkGui::Text("%.2f x %.2f", avail_size.x, avail_size.y);
        }
        else
        {
            HvkGui::Text("(Hold Shift to display a dummy viewport)");
            if (HvkGui::Button("Set 200x200")) { HvkGui::SetWindowSize(ImVec2(200, 200)); } HvkGui::SameLine();
            if (HvkGui::Button("Set 500x500")) { HvkGui::SetWindowSize(ImVec2(500, 500)); } HvkGui::SameLine();
            if (HvkGui::Button("Set 800x200")) { HvkGui::SetWindowSize(ImVec2(800, 200)); }
            HvkGui::SetNextItemWidth(HvkGui::GetFontSize() * 20);
            HvkGui::Combo("Constraint", &type, test_desc, IM_ARRAYSIZE(test_desc));
            HvkGui::SetNextItemWidth(HvkGui::GetFontSize() * 20);
            HvkGui::DragInt("Lines", &display_lines, 0.2f, 1, 100);
            HvkGui::Checkbox("Auto-resize", &auto_resize);
            HvkGui::Checkbox("Window padding", &window_padding);
            for (int i = 0; i < display_lines; i++)
                HvkGui::Text("%*sHello, sailor! Making this line long enough for the example.", i * 4, "");
        }
    }
    HvkGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Simple overlay / ShowExampleAppSimpleOverlay()
//-----------------------------------------------------------------------------

// Demonstrate creating a simple static window with no decoration
// + a context-menu to choose which corner of the screen to use.
static void ShowExampleAppSimpleOverlay(bool* p_open)
{
    static int location = 0;
    ImGuiIO& io = HvkGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (location >= 0)
    {
        const float PAD = 10.0f;
        const ImGuiViewport* viewport = HvkGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
        HvkGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    else if (location == -2)
    {
        // Center window
        HvkGui::SetNextWindowPos(HvkGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    HvkGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (HvkGui::Begin("Example: Simple overlay", p_open, window_flags))
    {
        IMGUI_DEMO_MARKER("Examples/Simple Overlay");
        HvkGui::Text("Simple overlay\n" "(right-click to change position)");
        HvkGui::Separator();
        if (HvkGui::IsMousePosValid())
            HvkGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
        else
            HvkGui::Text("Mouse Position: <invalid>");
        if (HvkGui::BeginPopupContextWindow())
        {
            if (HvkGui::MenuItem("Custom",       NULL, location == -1)) location = -1;
            if (HvkGui::MenuItem("Center",       NULL, location == -2)) location = -2;
            if (HvkGui::MenuItem("Top-left",     NULL, location == 0)) location = 0;
            if (HvkGui::MenuItem("Top-right",    NULL, location == 1)) location = 1;
            if (HvkGui::MenuItem("Bottom-left",  NULL, location == 2)) location = 2;
            if (HvkGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
            if (p_open && HvkGui::MenuItem("Close")) *p_open = false;
            HvkGui::EndPopup();
        }
    }
    HvkGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Fullscreen window / ShowExampleAppFullscreen()
//-----------------------------------------------------------------------------

// Demonstrate creating a window covering the entire screen/viewport
static void ShowExampleAppFullscreen(bool* p_open)
{
    static bool use_work_area = true;
    static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

    // We demonstrate using the full viewport area or the work area (without menu-bars, task-bars etc.)
    // Based on your use case you may want one or the other.
    const ImGuiViewport* viewport = HvkGui::GetMainViewport();
    HvkGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
    HvkGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

    if (HvkGui::Begin("Example: Fullscreen window", p_open, flags))
    {
        HvkGui::Checkbox("Use work area instead of main area", &use_work_area);
        HvkGui::SameLine();
        HelpMarker("Main Area = entire viewport,\nWork Area = entire viewport minus sections used by the main menu bars, task bars etc.\n\nEnable the main-menu bar in Examples menu to see the difference.");

        HvkGui::CheckboxFlags("ImGuiWindowFlags_NoBackground", &flags, ImGuiWindowFlags_NoBackground);
        HvkGui::CheckboxFlags("ImGuiWindowFlags_NoDecoration", &flags, ImGuiWindowFlags_NoDecoration);
        HvkGui::Indent();
        HvkGui::CheckboxFlags("ImGuiWindowFlags_NoTitleBar", &flags, ImGuiWindowFlags_NoTitleBar);
        HvkGui::CheckboxFlags("ImGuiWindowFlags_NoCollapse", &flags, ImGuiWindowFlags_NoCollapse);
        HvkGui::CheckboxFlags("ImGuiWindowFlags_NoScrollbar", &flags, ImGuiWindowFlags_NoScrollbar);
        HvkGui::Unindent();

        if (p_open && HvkGui::Button("Close this window"))
            *p_open = false;
    }
    HvkGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Manipulating Window Titles / ShowExampleAppWindowTitles()
//-----------------------------------------------------------------------------

// Demonstrate the use of "##" and "###" in identifiers to manipulate ID generation.
// This applies to all regular items as well.
// Read FAQ section "How can I have multiple widgets with the same label?" for details.
static void ShowExampleAppWindowTitles(bool*)
{
    const ImGuiViewport* viewport = HvkGui::GetMainViewport();
    const ImVec2 base_pos = viewport->Pos;

    // By default, Windows are uniquely identified by their title.
    // You can use the "##" and "###" markers to manipulate the display/ID.

    // Using "##" to display same title but have unique identifier.
    HvkGui::SetNextWindowPos(ImVec2(base_pos.x + 100, base_pos.y + 100), ImGuiCond_FirstUseEver);
    HvkGui::Begin("Same title as another window##1");
    IMGUI_DEMO_MARKER("Examples/Manipulating window titles");
    HvkGui::Text("This is window 1.\nMy title is the same as window 2, but my identifier is unique.");
    HvkGui::End();

    HvkGui::SetNextWindowPos(ImVec2(base_pos.x + 100, base_pos.y + 200), ImGuiCond_FirstUseEver);
    HvkGui::Begin("Same title as another window##2");
    HvkGui::Text("This is window 2.\nMy title is the same as window 1, but my identifier is unique.");
    HvkGui::End();

    // Using "###" to display a changing title but keep a static identifier "AnimatedTitle"
    char buf[128];
    sprintf(buf, "Animated title %c %d###AnimatedTitle", "|/-\\"[(int)(HvkGui::GetTime() / 0.25f) & 3], HvkGui::GetFrameCount());
    HvkGui::SetNextWindowPos(ImVec2(base_pos.x + 100, base_pos.y + 300), ImGuiCond_FirstUseEver);
    HvkGui::Begin(buf);
    HvkGui::Text("This window has a changing title.");
    HvkGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Custom Rendering using ImDrawList API / ShowExampleAppCustomRendering()
//-----------------------------------------------------------------------------

// Add a |_| looking shape
static void PathConcaveShape(ImDrawList* draw_list, float x, float y, float sz)
{
    const ImVec2 pos_norms[] = { { 0.0f, 0.0f }, { 0.3f, 0.0f }, { 0.3f, 0.7f }, { 0.7f, 0.7f }, { 0.7f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    for (const ImVec2& p : pos_norms)
        draw_list->PathLineTo(ImVec2(x + 0.5f + (int)(sz * p.x), y + 0.5f + (int)(sz * p.y)));
}

// Demonstrate using the low-level ImDrawList to draw custom shapes.
static void ShowExampleAppCustomRendering(bool* p_open)
{
    if (!HvkGui::Begin("Example: Custom rendering", p_open))
    {
        HvkGui::End();
        return;
    }
    IMGUI_DEMO_MARKER("Examples/Custom Rendering");

    // Tip: If you do a lot of custom rendering, you probably want to use your own geometrical types and benefit of
    // overloaded operators, etc. Define IM_VEC2_CLASS_EXTRA in imconfig.h to create implicit conversions between your
    // types and ImVec2/ImVec4. Dear ImGui defines overloaded operators but they are internal to imgui.cpp and not
    // exposed outside (to avoid messing with your types) In this example we are not using the maths operators!

    if (HvkGui::BeginTabBar("##TabBar"))
    {
        if (HvkGui::BeginTabItem("Primitives"))
        {
            HvkGui::PushItemWidth(-HvkGui::GetFontSize() * 15);
            ImDrawList* draw_list = HvkGui::GetWindowDrawList();

            // Draw gradients
            // (note that those are currently exacerbating our sRGB/Linear issues)
            // Calling HvkGui::GetColorU32() multiplies the given colors by the current Style Alpha, but you may pass the IM_COL32() directly as well..
            HvkGui::Text("Gradients");
            ImVec2 gradient_size = ImVec2(HvkGui::CalcItemWidth(), HvkGui::GetFrameHeight());
            {
                ImVec2 p0 = HvkGui::GetCursorScreenPos();
                ImVec2 p1 = ImVec2(p0.x + gradient_size.x, p0.y + gradient_size.y);
                ImU32 col_a = HvkGui::GetColorU32(IM_COL32(0, 0, 0, 255));
                ImU32 col_b = HvkGui::GetColorU32(IM_COL32(255, 255, 255, 255));
                draw_list->AddRectFilledMultiColor(p0, p1, col_a, col_b, col_b, col_a);
                HvkGui::InvisibleButton("##gradient1", gradient_size);
            }
            {
                ImVec2 p0 = HvkGui::GetCursorScreenPos();
                ImVec2 p1 = ImVec2(p0.x + gradient_size.x, p0.y + gradient_size.y);
                ImU32 col_a = HvkGui::GetColorU32(IM_COL32(0, 255, 0, 255));
                ImU32 col_b = HvkGui::GetColorU32(IM_COL32(255, 0, 0, 255));
                draw_list->AddRectFilledMultiColor(p0, p1, col_a, col_b, col_b, col_a);
                HvkGui::InvisibleButton("##gradient2", gradient_size);
            }

            // Draw a bunch of primitives
            HvkGui::Text("All primitives");
            static float sz = 36.0f;
            static float thickness = 3.0f;
            static int ngon_sides = 6;
            static bool circle_segments_override = false;
            static int circle_segments_override_v = 12;
            static bool curve_segments_override = false;
            static int curve_segments_override_v = 8;
            static ImVec4 colf = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
            HvkGui::DragFloat("Size", &sz, 0.2f, 2.0f, 100.0f, "%.0f");
            HvkGui::DragFloat("Thickness", &thickness, 0.05f, 1.0f, 8.0f, "%.02f");
            HvkGui::SliderInt("N-gon sides", &ngon_sides, 3, 12);
            HvkGui::Checkbox("##circlesegmentoverride", &circle_segments_override);
            HvkGui::SameLine(0.0f, HvkGui::GetStyle().ItemInnerSpacing.x);
            circle_segments_override |= HvkGui::SliderInt("Circle segments override", &circle_segments_override_v, 3, 40);
            HvkGui::Checkbox("##curvessegmentoverride", &curve_segments_override);
            HvkGui::SameLine(0.0f, HvkGui::GetStyle().ItemInnerSpacing.x);
            curve_segments_override |= HvkGui::SliderInt("Curves segments override", &curve_segments_override_v, 3, 40);
            HvkGui::ColorEdit4("Color", &colf.x);

            const ImVec2 p = HvkGui::GetCursorScreenPos();
            const ImU32 col = ImColor(colf);
            const float spacing = 10.0f;
            const ImDrawFlags corners_tl_br = ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersBottomRight;
            const float rounding = sz / 5.0f;
            const int circle_segments = circle_segments_override ? circle_segments_override_v : 0;
            const int curve_segments = curve_segments_override ? curve_segments_override_v : 0;
            const ImVec2 cp3[3] = { ImVec2(0.0f, sz * 0.6f), ImVec2(sz * 0.5f, -sz * 0.4f), ImVec2(sz, sz) }; // Control points for curves
            const ImVec2 cp4[4] = { ImVec2(0.0f, 0.0f), ImVec2(sz * 1.3f, sz * 0.3f), ImVec2(sz - sz * 1.3f, sz - sz * 0.3f), ImVec2(sz, sz) };

            float x = p.x + 4.0f;
            float y = p.y + 4.0f;
            for (int n = 0; n < 2; n++)
            {
                // First line uses a thickness of 1.0f, second line uses the configurable thickness
                float th = (n == 0) ? 1.0f : thickness;
                draw_list->AddNgon(ImVec2(x + sz*0.5f, y + sz*0.5f), sz*0.5f, col, ngon_sides, th);                 x += sz + spacing;  // N-gon
                draw_list->AddCircle(ImVec2(x + sz*0.5f, y + sz*0.5f), sz*0.5f, col, circle_segments, th);          x += sz + spacing;  // Circle
                draw_list->AddEllipse(ImVec2(x + sz*0.5f, y + sz*0.5f), ImVec2(sz*0.5f, sz*0.3f), col, -0.3f, circle_segments, th); x += sz + spacing;  // Ellipse
                draw_list->AddRect(ImVec2(x, y), ImVec2(x + sz, y + sz), col, 0.0f, ImDrawFlags_None, th);          x += sz + spacing;  // Square
                draw_list->AddRect(ImVec2(x, y), ImVec2(x + sz, y + sz), col, rounding, ImDrawFlags_None, th);      x += sz + spacing;  // Square with all rounded corners
                draw_list->AddRect(ImVec2(x, y), ImVec2(x + sz, y + sz), col, rounding, corners_tl_br, th);         x += sz + spacing;  // Square with two rounded corners
                draw_list->AddTriangle(ImVec2(x+sz*0.5f,y), ImVec2(x+sz, y+sz-0.5f), ImVec2(x, y+sz-0.5f), col, th);x += sz + spacing;  // Triangle
                //draw_list->AddTriangle(ImVec2(x+sz*0.2f,y), ImVec2(x, y+sz-0.5f), ImVec2(x+sz*0.4f, y+sz-0.5f), col, th);x+= sz*0.4f + spacing; // Thin triangle
                PathConcaveShape(draw_list, x, y, sz); draw_list->PathStroke(col, ImDrawFlags_Closed, th);          x += sz + spacing;  // Concave Shape
                //draw_list->AddPolyline(concave_shape, IM_ARRAYSIZE(concave_shape), col, ImDrawFlags_Closed, th);
                draw_list->AddLine(ImVec2(x, y), ImVec2(x + sz, y), col, th);                                       x += sz + spacing;  // Horizontal line (note: drawing a filled rectangle will be faster!)
                draw_list->AddLine(ImVec2(x, y), ImVec2(x, y + sz), col, th);                                       x += spacing;       // Vertical line (note: drawing a filled rectangle will be faster!)
                draw_list->AddLine(ImVec2(x, y), ImVec2(x + sz, y + sz), col, th);                                  x += sz + spacing;  // Diagonal line

                // Path
                draw_list->PathArcTo(ImVec2(x + sz*0.5f, y + sz*0.5f), sz*0.5f, 3.141592f, 3.141592f * -0.5f);
                draw_list->PathStroke(col, ImDrawFlags_None, th);
                x += sz + spacing;

                // Quadratic Bezier Curve (3 control points)
                draw_list->AddBezierQuadratic(ImVec2(x + cp3[0].x, y + cp3[0].y), ImVec2(x + cp3[1].x, y + cp3[1].y), ImVec2(x + cp3[2].x, y + cp3[2].y), col, th, curve_segments);
                x += sz + spacing;

                // Cubic Bezier Curve (4 control points)
                draw_list->AddBezierCubic(ImVec2(x + cp4[0].x, y + cp4[0].y), ImVec2(x + cp4[1].x, y + cp4[1].y), ImVec2(x + cp4[2].x, y + cp4[2].y), ImVec2(x + cp4[3].x, y + cp4[3].y), col, th, curve_segments);

                x = p.x + 4;
                y += sz + spacing;
            }

            // Filled shapes
            draw_list->AddNgonFilled(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, col, ngon_sides);             x += sz + spacing;  // N-gon
            draw_list->AddCircleFilled(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, col, circle_segments);      x += sz + spacing;  // Circle
            draw_list->AddEllipseFilled(ImVec2(x + sz * 0.5f, y + sz * 0.5f), ImVec2(sz * 0.5f, sz * 0.3f), col, -0.3f, circle_segments); x += sz + spacing;// Ellipse
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col);                                    x += sz + spacing;  // Square
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col, 10.0f);                             x += sz + spacing;  // Square with all rounded corners
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col, 10.0f, corners_tl_br);              x += sz + spacing;  // Square with two rounded corners
            draw_list->AddTriangleFilled(ImVec2(x+sz*0.5f,y), ImVec2(x+sz, y+sz-0.5f), ImVec2(x, y+sz-0.5f), col);  x += sz + spacing;  // Triangle
            //draw_list->AddTriangleFilled(ImVec2(x+sz*0.2f,y), ImVec2(x, y+sz-0.5f), ImVec2(x+sz*0.4f, y+sz-0.5f), col); x += sz*0.4f + spacing; // Thin triangle
            PathConcaveShape(draw_list, x, y, sz); draw_list->PathFillConcave(col);                                 x += sz + spacing;  // Concave shape
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + thickness), col);                             x += sz + spacing;  // Horizontal line (faster than AddLine, but only handle integer thickness)
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + thickness, y + sz), col);                             x += spacing * 2.0f;// Vertical line (faster than AddLine, but only handle integer thickness)
            draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + 1, y + 1), col);                                      x += sz;            // Pixel (faster than AddLine)

            // Path
            draw_list->PathArcTo(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, 3.141592f * -0.5f, 3.141592f);
            draw_list->PathFillConvex(col);
            x += sz + spacing;

            // Quadratic Bezier Curve (3 control points)
            draw_list->PathLineTo(ImVec2(x + cp3[0].x, y + cp3[0].y));
            draw_list->PathBezierQuadraticCurveTo(ImVec2(x + cp3[1].x, y + cp3[1].y), ImVec2(x + cp3[2].x, y + cp3[2].y), curve_segments);
            draw_list->PathFillConvex(col);
            x += sz + spacing;

            draw_list->AddRectFilledMultiColor(ImVec2(x, y), ImVec2(x + sz, y + sz), IM_COL32(0, 0, 0, 255), IM_COL32(255, 0, 0, 255), IM_COL32(255, 255, 0, 255), IM_COL32(0, 255, 0, 255));
            x += sz + spacing;

            HvkGui::Dummy(ImVec2((sz + spacing) * 13.2f, (sz + spacing) * 3.0f));
            HvkGui::PopItemWidth();
            HvkGui::EndTabItem();
        }

        if (HvkGui::BeginTabItem("Canvas"))
        {
            static ImVector<ImVec2> points;
            static ImVec2 scrolling(0.0f, 0.0f);
            static bool opt_enable_grid = true;
            static bool opt_enable_context_menu = true;
            static bool adding_line = false;

            HvkGui::Checkbox("Enable grid", &opt_enable_grid);
            HvkGui::Checkbox("Enable context menu", &opt_enable_context_menu);
            HvkGui::Text("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.");

            // Typically you would use a BeginChild()/EndChild() pair to benefit from a clipping region + own scrolling.
            // Here we demonstrate that this can be replaced by simple offsetting + custom drawing + PushClipRect/PopClipRect() calls.
            // To use a child window instead we could use, e.g:
            //      HvkGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));      // Disable padding
            //      HvkGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));  // Set a background color
            //      HvkGui::BeginChild("canvas", ImVec2(0.0f, 0.0f), ImGuiChildFlags_Borders, ImGuiWindowFlags_NoMove);
            //      HvkGui::PopStyleColor();
            //      HvkGui::PopStyleVar();
            //      [...]
            //      HvkGui::EndChild();

            // Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
            ImVec2 canvas_p0 = HvkGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
            ImVec2 canvas_sz = HvkGui::GetContentRegionAvail();   // Resize canvas to what's available
            if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
            if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
            ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

            // Draw border and background color
            ImGuiIO& io = HvkGui::GetIO();
            ImDrawList* draw_list = HvkGui::GetWindowDrawList();
            draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
            draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

            // This will catch our interactions
            HvkGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
            const bool is_hovered = HvkGui::IsItemHovered(); // Hovered
            const bool is_active = HvkGui::IsItemActive();   // Held
            const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
            const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

            // Add first and second point
            if (is_hovered && !adding_line && HvkGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                points.push_back(mouse_pos_in_canvas);
                points.push_back(mouse_pos_in_canvas);
                adding_line = true;
            }
            if (adding_line)
            {
                points.back() = mouse_pos_in_canvas;
                if (!HvkGui::IsMouseDown(ImGuiMouseButton_Left))
                    adding_line = false;
            }

            // Pan (we use a zero mouse threshold when there's no context menu)
            // You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.
            const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
            if (is_active && HvkGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
            {
                scrolling.x += io.MouseDelta.x;
                scrolling.y += io.MouseDelta.y;
            }

            // Context menu (under default mouse threshold)
            ImVec2 drag_delta = HvkGui::GetMouseDragDelta(ImGuiMouseButton_Right);
            if (opt_enable_context_menu && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
                HvkGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
            if (HvkGui::BeginPopup("context"))
            {
                if (adding_line)
                    points.resize(points.size() - 2);
                adding_line = false;
                if (HvkGui::MenuItem("Remove one", NULL, false, points.Size > 0)) { points.resize(points.size() - 2); }
                if (HvkGui::MenuItem("Remove all", NULL, false, points.Size > 0)) { points.clear(); }
                HvkGui::EndPopup();
            }

            // Draw grid + all lines in the canvas
            draw_list->PushClipRect(canvas_p0, canvas_p1, true);
            if (opt_enable_grid)
            {
                const float GRID_STEP = 64.0f;
                for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
                    draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
                for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
                    draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
            }
            for (int n = 0; n < points.Size; n += 2)
                draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y), ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
            draw_list->PopClipRect();

            HvkGui::EndTabItem();
        }

        if (HvkGui::BeginTabItem("BG/FG draw lists"))
        {
            static bool draw_bg = true;
            static bool draw_fg = true;
            HvkGui::Checkbox("Draw in Background draw list", &draw_bg);
            HvkGui::SameLine(); HelpMarker("The Background draw list will be rendered below every Dear ImGui windows.");
            HvkGui::Checkbox("Draw in Foreground draw list", &draw_fg);
            HvkGui::SameLine(); HelpMarker("The Foreground draw list will be rendered over every Dear ImGui windows.");
            ImVec2 window_pos = HvkGui::GetWindowPos();
            ImVec2 window_size = HvkGui::GetWindowSize();
            ImVec2 window_center = ImVec2(window_pos.x + window_size.x * 0.5f, window_pos.y + window_size.y * 0.5f);
            if (draw_bg)
                HvkGui::GetBackgroundDrawList()->AddCircle(window_center, window_size.x * 0.6f, IM_COL32(255, 0, 0, 200), 0, 10 + 4);
            if (draw_fg)
                HvkGui::GetForegroundDrawList()->AddCircle(window_center, window_size.y * 0.6f, IM_COL32(0, 255, 0, 200), 0, 10);
            HvkGui::EndTabItem();
        }

        // Demonstrate out-of-order rendering via channels splitting
        // We use functions in ImDrawList as each draw list contains a convenience splitter,
        // but you can also instantiate your own ImDrawListSplitter if you need to nest them.
        if (HvkGui::BeginTabItem("Draw Channels"))
        {
            ImDrawList* draw_list = HvkGui::GetWindowDrawList();
            {
                HvkGui::Text("Blue shape is drawn first: appears in back");
                HvkGui::Text("Red shape is drawn after: appears in front");
                ImVec2 p0 = HvkGui::GetCursorScreenPos();
                draw_list->AddRectFilled(ImVec2(p0.x, p0.y), ImVec2(p0.x + 50, p0.y + 50), IM_COL32(0, 0, 255, 255)); // Blue
                draw_list->AddRectFilled(ImVec2(p0.x + 25, p0.y + 25), ImVec2(p0.x + 75, p0.y + 75), IM_COL32(255, 0, 0, 255)); // Red
                HvkGui::Dummy(ImVec2(75, 75));
            }
            HvkGui::Separator();
            {
                HvkGui::Text("Blue shape is drawn first, into channel 1: appears in front");
                HvkGui::Text("Red shape is drawn after, into channel 0: appears in back");
                ImVec2 p1 = HvkGui::GetCursorScreenPos();

                // Create 2 channels and draw a Blue shape THEN a Red shape.
                // You can create any number of channels. Tables API use 1 channel per column in order to better batch draw calls.
                draw_list->ChannelsSplit(2);
                draw_list->ChannelsSetCurrent(1);
                draw_list->AddRectFilled(ImVec2(p1.x, p1.y), ImVec2(p1.x + 50, p1.y + 50), IM_COL32(0, 0, 255, 255)); // Blue
                draw_list->ChannelsSetCurrent(0);
                draw_list->AddRectFilled(ImVec2(p1.x + 25, p1.y + 25), ImVec2(p1.x + 75, p1.y + 75), IM_COL32(255, 0, 0, 255)); // Red

                // Flatten/reorder channels. Red shape is in channel 0 and it appears below the Blue shape in channel 1.
                // This works by copying draw indices only (vertices are not copied).
                draw_list->ChannelsMerge();
                HvkGui::Dummy(ImVec2(75, 75));
                HvkGui::Text("After reordering, contents of channel 0 appears below channel 1.");
            }
            HvkGui::EndTabItem();
        }

        HvkGui::EndTabBar();
    }

    HvkGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Documents Handling / ShowExampleAppDocuments()
//-----------------------------------------------------------------------------

// Simplified structure to mimic a Document model
struct MyDocument
{
    char        Name[32];   // Document title
    int         UID;        // Unique ID (necessary as we can change title)
    bool        Open;       // Set when open (we keep an array of all available documents to simplify demo code!)
    bool        OpenPrev;   // Copy of Open from last update.
    bool        Dirty;      // Set when the document has been modified
    ImVec4      Color;      // An arbitrary variable associated to the document

    MyDocument(int uid, const char* name, bool open = true, const ImVec4& color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f))
    {
        UID = uid;
        snprintf(Name, sizeof(Name), "%s", name);
        Open = OpenPrev = open;
        Dirty = false;
        Color = color;
    }
    void DoOpen()       { Open = true; }
    void DoForceClose() { Open = false; Dirty = false; }
    void DoSave()       { Dirty = false; }
};

struct ExampleAppDocuments
{
    ImVector<MyDocument>    Documents;
    ImVector<MyDocument*>   CloseQueue;
    MyDocument*             RenamingDoc = NULL;
    bool                    RenamingStarted = false;

    ExampleAppDocuments()
    {
        Documents.push_back(MyDocument(0, "Lettuce",             true,  ImVec4(0.4f, 0.8f, 0.4f, 1.0f)));
        Documents.push_back(MyDocument(1, "Eggplant",            true,  ImVec4(0.8f, 0.5f, 1.0f, 1.0f)));
        Documents.push_back(MyDocument(2, "Carrot",              true,  ImVec4(1.0f, 0.8f, 0.5f, 1.0f)));
        Documents.push_back(MyDocument(3, "Tomato",              false, ImVec4(1.0f, 0.3f, 0.4f, 1.0f)));
        Documents.push_back(MyDocument(4, "A Rather Long Title", false, ImVec4(0.4f, 0.8f, 0.8f, 1.0f)));
        Documents.push_back(MyDocument(5, "Some Document",       false, ImVec4(0.8f, 0.8f, 1.0f, 1.0f)));
    }

    // As we allow to change document name, we append a never-changing document ID so tabs are stable
    void GetTabName(MyDocument* doc, char* out_buf, size_t out_buf_size)
    {
        snprintf(out_buf, out_buf_size, "%s###doc%d", doc->Name, doc->UID);
    }

    // Display placeholder contents for the Document
    void DisplayDocContents(MyDocument* doc)
    {
        HvkGui::PushID(doc);
        HvkGui::Text("Document \"%s\"", doc->Name);
        HvkGui::PushStyleColor(ImGuiCol_Text, doc->Color);
        HvkGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
        HvkGui::PopStyleColor();

        HvkGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_R, ImGuiInputFlags_Tooltip);
        if (HvkGui::Button("Rename.."))
        {
            RenamingDoc = doc;
            RenamingStarted = true;
        }
        HvkGui::SameLine();

        HvkGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_M, ImGuiInputFlags_Tooltip);
        if (HvkGui::Button("Modify"))
            doc->Dirty = true;

        HvkGui::SameLine();
        HvkGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_S, ImGuiInputFlags_Tooltip);
        if (HvkGui::Button("Save"))
            doc->DoSave();

        HvkGui::SameLine();
        HvkGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_W, ImGuiInputFlags_Tooltip);
        if (HvkGui::Button("Close"))
            CloseQueue.push_back(doc);
        HvkGui::ColorEdit3("color", &doc->Color.x);  // Useful to test drag and drop and hold-dragged-to-open-tab behavior.
        HvkGui::PopID();
    }

    // Display context menu for the Document
    void DisplayDocContextMenu(MyDocument* doc)
    {
        if (!HvkGui::BeginPopupContextItem())
            return;

        char buf[256];
        sprintf(buf, "Save %s", doc->Name);
        if (HvkGui::MenuItem(buf, "Ctrl+S", false, doc->Open))
            doc->DoSave();
        if (HvkGui::MenuItem("Rename...", "Ctrl+R", false, doc->Open))
            RenamingDoc = doc;
        if (HvkGui::MenuItem("Close", "Ctrl+W", false, doc->Open))
            CloseQueue.push_back(doc);
        HvkGui::EndPopup();
    }

    // [Optional] Notify the system of Tabs/Windows closure that happened outside the regular tab interface.
    // If a tab has been closed programmatically (aka closed from another source such as the Checkbox() in the demo,
    // as opposed to clicking on the regular tab closing button) and stops being submitted, it will take a frame for
    // the tab bar to notice its absence. During this frame there will be a gap in the tab bar, and if the tab that has
    // disappeared was the selected one, the tab bar will report no selected tab during the frame. This will effectively
    // give the impression of a flicker for one frame.
    // We call SetTabItemClosed() to manually notify the Tab Bar or Docking system of removed tabs to avoid this glitch.
    // Note that this completely optional, and only affect tab bars with the ImGuiTabBarFlags_Reorderable flag.
    void NotifyOfDocumentsClosedElsewhere()
    {
        for (MyDocument& doc : Documents)
        {
            if (!doc.Open && doc.OpenPrev)
                HvkGui::SetTabItemClosed(doc.Name);
            doc.OpenPrev = doc.Open;
        }
    }
};

void ShowExampleAppDocuments(bool* p_open)
{
    static ExampleAppDocuments app;

    // Options
    static bool opt_reorderable = true;
    static ImGuiTabBarFlags opt_fitting_flags = ImGuiTabBarFlags_FittingPolicyDefault_;

    bool window_contents_visible = HvkGui::Begin("Example: Documents", p_open, ImGuiWindowFlags_MenuBar);
    if (!window_contents_visible)
    {
        HvkGui::End();
        return;
    }

    // Menu
    if (HvkGui::BeginMenuBar())
    {
        if (HvkGui::BeginMenu("File"))
        {
            int open_count = 0;
            for (MyDocument& doc : app.Documents)
                open_count += doc.Open ? 1 : 0;

            if (HvkGui::BeginMenu("Open", open_count < app.Documents.Size))
            {
                for (MyDocument& doc : app.Documents)
                    if (!doc.Open && HvkGui::MenuItem(doc.Name))
                        doc.DoOpen();
                HvkGui::EndMenu();
            }
            if (HvkGui::MenuItem("Close All Documents", NULL, false, open_count > 0))
                for (MyDocument& doc : app.Documents)
                    app.CloseQueue.push_back(&doc);
            if (HvkGui::MenuItem("Exit") && p_open)
                *p_open = false;
            HvkGui::EndMenu();
        }
        HvkGui::EndMenuBar();
    }

    // [Debug] List documents with one checkbox for each
    for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
    {
        MyDocument& doc = app.Documents[doc_n];
        if (doc_n > 0)
            HvkGui::SameLine();
        HvkGui::PushID(&doc);
        if (HvkGui::Checkbox(doc.Name, &doc.Open))
            if (!doc.Open)
                doc.DoForceClose();
        HvkGui::PopID();
    }

    HvkGui::Separator();

    // About the ImGuiWindowFlags_UnsavedDocument / ImGuiTabItemFlags_UnsavedDocument flags.
    // They have multiple effects:
    // - Display a dot next to the title.
    // - Tab is selected when clicking the X close button.
    // - Closure is not assumed (will wait for user to stop submitting the tab).
    //   Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
    //   We need to assume closure by default otherwise waiting for "lack of submission" on the next frame would leave an empty
    //   hole for one-frame, both in the tab-bar and in tab-contents when closing a tab/window.
    //   The rarely used SetTabItemClosed() function is a way to notify of programmatic closure to avoid the one-frame hole.

    // Submit Tab Bar and Tabs
    {
        ImGuiTabBarFlags tab_bar_flags = (opt_fitting_flags) | (opt_reorderable ? ImGuiTabBarFlags_Reorderable : 0);
        tab_bar_flags |= ImGuiTabBarFlags_DrawSelectedOverline;
        if (HvkGui::BeginTabBar("##tabs", tab_bar_flags))
        {
            if (opt_reorderable)
                app.NotifyOfDocumentsClosedElsewhere();

            // [DEBUG] Stress tests
            //if ((HvkGui::GetFrameCount() % 30) == 0) docs[1].Open ^= 1;            // [DEBUG] Automatically show/hide a tab. Test various interactions e.g. dragging with this on.
            //if (HvkGui::GetIO().KeyCtrl) HvkGui::SetTabItemSelected(docs[1].Name);  // [DEBUG] Test SetTabItemSelected(), probably not very useful as-is anyway..

            // Submit Tabs
            for (MyDocument& doc : app.Documents)
            {
                if (!doc.Open)
                    continue;

                // As we allow to change document name, we append a never-changing document id so tabs are stable
                char doc_name_buf[64];
                app.GetTabName(&doc, doc_name_buf, sizeof(doc_name_buf));
                ImGuiTabItemFlags tab_flags = (doc.Dirty ? ImGuiTabItemFlags_UnsavedDocument : 0);
                bool visible = HvkGui::BeginTabItem(doc_name_buf, &doc.Open, tab_flags);

                // Cancel attempt to close when unsaved add to save queue so we can display a popup.
                if (!doc.Open && doc.Dirty)
                {
                    doc.Open = true;
                    app.CloseQueue.push_back(&doc);
                }

                app.DisplayDocContextMenu(&doc);
                if (visible)
                {
                    app.DisplayDocContents(&doc);
                    HvkGui::EndTabItem();
                }
            }

            HvkGui::EndTabBar();
        }
    }

    // Display renaming UI
    if (app.RenamingDoc != NULL)
    {
        if (app.RenamingStarted)
            HvkGui::OpenPopup("Rename");
        if (HvkGui::BeginPopup("Rename"))
        {
            HvkGui::SetNextItemWidth(HvkGui::GetFontSize() * 30);
            if (HvkGui::InputText("###Name", app.RenamingDoc->Name, IM_ARRAYSIZE(app.RenamingDoc->Name), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                HvkGui::CloseCurrentPopup();
                app.RenamingDoc = NULL;
            }
            if (app.RenamingStarted)
                HvkGui::SetKeyboardFocusHere(-1);
            HvkGui::EndPopup();
        }
        else
        {
            app.RenamingDoc = NULL;
        }
        app.RenamingStarted = false;
    }

    // Display closing confirmation UI
    if (!app.CloseQueue.empty())
    {
        int close_queue_unsaved_documents = 0;
        for (int n = 0; n < app.CloseQueue.Size; n++)
            if (app.CloseQueue[n]->Dirty)
                close_queue_unsaved_documents++;

        if (close_queue_unsaved_documents == 0)
        {
            // Close documents when all are unsaved
            for (int n = 0; n < app.CloseQueue.Size; n++)
                app.CloseQueue[n]->DoForceClose();
            app.CloseQueue.clear();
        }
        else
        {
            if (!HvkGui::IsPopupOpen("Save?"))
                HvkGui::OpenPopup("Save?");
            if (HvkGui::BeginPopupModal("Save?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                HvkGui::Text("Save change to the following items?");
                float item_height = HvkGui::GetTextLineHeightWithSpacing();
                if (HvkGui::BeginChild(HvkGui::GetID("frame"), ImVec2(-FLT_MIN, 6.25f * item_height), ImGuiChildFlags_FrameStyle))
                    for (MyDocument* doc : app.CloseQueue)
                        if (doc->Dirty)
                            HvkGui::Text("%s", doc->Name);
                HvkGui::EndChild();

                ImVec2 button_size(HvkGui::GetFontSize() * 7.0f, 0.0f);
                if (HvkGui::Button("Yes", button_size))
                {
                    for (MyDocument* doc : app.CloseQueue)
                    {
                        if (doc->Dirty)
                            doc->DoSave();
                        doc->DoForceClose();
                    }
                    app.CloseQueue.clear();
                    HvkGui::CloseCurrentPopup();
                }
                HvkGui::SameLine();
                if (HvkGui::Button("No", button_size))
                {
                    for (MyDocument* doc : app.CloseQueue)
                        doc->DoForceClose();
                    app.CloseQueue.clear();
                    HvkGui::CloseCurrentPopup();
                }
                HvkGui::SameLine();
                if (HvkGui::Button("Cancel", button_size))
                {
                    app.CloseQueue.clear();
                    HvkGui::CloseCurrentPopup();
                }
                HvkGui::EndPopup();
            }
        }
    }

    HvkGui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Assets Browser / ShowExampleAppAssetsBrowser()
//-----------------------------------------------------------------------------

//#include "imgui_internal.h" // NavMoveRequestTryWrapping()

struct ExampleAsset
{
    ImGuiID ID;
    int     Type;

    ExampleAsset(ImGuiID id, int type) { ID = id; Type = type; }

    static const ImGuiTableSortSpecs* s_current_sort_specs;

    static void SortWithSortSpecs(ImGuiTableSortSpecs* sort_specs, ExampleAsset* items, int items_count)
    {
        s_current_sort_specs = sort_specs; // Store in variable accessible by the sort function.
        if (items_count > 1)
            qsort(items, (size_t)items_count, sizeof(items[0]), ExampleAsset::CompareWithSortSpecs);
        s_current_sort_specs = NULL;
    }

    // Compare function to be used by qsort()
    static int IMGUI_CDECL CompareWithSortSpecs(const void* lhs, const void* rhs)
    {
        const ExampleAsset* a = (const ExampleAsset*)lhs;
        const ExampleAsset* b = (const ExampleAsset*)rhs;
        for (int n = 0; n < s_current_sort_specs->SpecsCount; n++)
        {
            const ImGuiTableColumnSortSpecs* sort_spec = &s_current_sort_specs->Specs[n];
            int delta = 0;
            if (sort_spec->ColumnIndex == 0)
                delta = ((int)a->ID - (int)b->ID);
            else if (sort_spec->ColumnIndex == 1)
                delta = (a->Type - b->Type);
            if (delta > 0)
                return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? +1 : -1;
            if (delta < 0)
                return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? -1 : +1;
        }
        return (int)a->ID - (int)b->ID;
    }
};
const ImGuiTableSortSpecs* ExampleAsset::s_current_sort_specs = NULL;

struct ExampleAssetsBrowser
{
    // Options
    bool            ShowTypeOverlay = true;
    bool            AllowSorting = true;
    bool            AllowDragUnselected = false;
    bool            AllowBoxSelect = true;
    float           IconSize = 32.0f;
    int             IconSpacing = 10;
    int             IconHitSpacing = 4;         // Increase hit-spacing if you want to make it possible to clear or box-select from gaps. Some spacing is required to able to amend with Shift+box-select. Value is small in Explorer.
    bool            StretchSpacing = true;

    // State
    ImVector<ExampleAsset> Items;               // Our items
    ExampleSelectionWithDeletion Selection;     // Our selection (ImGuiSelectionBasicStorage + helper funcs to handle deletion)
    ImGuiID         NextItemId = 0;             // Unique identifier when creating new items
    bool            RequestDelete = false;      // Deferred deletion request
    bool            RequestSort = false;        // Deferred sort request
    float           ZoomWheelAccum = 0.0f;      // Mouse wheel accumulator to handle smooth wheels better

    // Calculated sizes for layout, output of UpdateLayoutSizes(). Could be locals but our code is simpler this way.
    ImVec2          LayoutItemSize;
    ImVec2          LayoutItemStep;             // == LayoutItemSize + LayoutItemSpacing
    float           LayoutItemSpacing = 0.0f;
    float           LayoutSelectableSpacing = 0.0f;
    float           LayoutOuterPadding = 0.0f;
    int             LayoutColumnCount = 0;
    int             LayoutLineCount = 0;

    // Functions
    ExampleAssetsBrowser()
    {
        AddItems(10000);
    }
    void AddItems(int count)
    {
        if (Items.Size == 0)
            NextItemId = 0;
        Items.reserve(Items.Size + count);
        for (int n = 0; n < count; n++, NextItemId++)
            Items.push_back(ExampleAsset(NextItemId, (NextItemId % 20) < 15 ? 0 : (NextItemId % 20) < 18 ? 1 : 2));
        RequestSort = true;
    }
    void ClearItems()
    {
        Items.clear();
        Selection.Clear();
    }

    // Logic would be written in the main code BeginChild() and outputting to local variables.
    // We extracted it into a function so we can call it easily from multiple places.
    void UpdateLayoutSizes(float avail_width)
    {
        // Layout: when not stretching: allow extending into right-most spacing.
        LayoutItemSpacing = (float)IconSpacing;
        if (StretchSpacing == false)
            avail_width += floorf(LayoutItemSpacing * 0.5f);

        // Layout: calculate number of icon per line and number of lines
        LayoutItemSize = ImVec2(floorf(IconSize), floorf(IconSize));
        LayoutColumnCount = IM_MAX((int)(avail_width / (LayoutItemSize.x + LayoutItemSpacing)), 1);
        LayoutLineCount = (Items.Size + LayoutColumnCount - 1) / LayoutColumnCount;

        // Layout: when stretching: allocate remaining space to more spacing. Round before division, so item_spacing may be non-integer.
        if (StretchSpacing && LayoutColumnCount > 1)
            LayoutItemSpacing = floorf(avail_width - LayoutItemSize.x * LayoutColumnCount) / LayoutColumnCount;

        LayoutItemStep = ImVec2(LayoutItemSize.x + LayoutItemSpacing, LayoutItemSize.y + LayoutItemSpacing);
        LayoutSelectableSpacing = IM_MAX(floorf(LayoutItemSpacing) - IconHitSpacing, 0.0f);
        LayoutOuterPadding = floorf(LayoutItemSpacing * 0.5f);
    }

    void Draw(const char* title, bool* p_open)
    {
        HvkGui::SetNextWindowSize(ImVec2(IconSize * 25, IconSize * 15), ImGuiCond_FirstUseEver);
        if (!HvkGui::Begin(title, p_open, ImGuiWindowFlags_MenuBar))
        {
            HvkGui::End();
            return;
        }

        // Menu bar
        if (HvkGui::BeginMenuBar())
        {
            if (HvkGui::BeginMenu("File"))
            {
                if (HvkGui::MenuItem("Add 10000 items"))
                    AddItems(10000);
                if (HvkGui::MenuItem("Clear items"))
                    ClearItems();
                HvkGui::Separator();
                if (HvkGui::MenuItem("Close", NULL, false, p_open != NULL))
                    *p_open = false;
                HvkGui::EndMenu();
            }
            if (HvkGui::BeginMenu("Edit"))
            {
                if (HvkGui::MenuItem("Delete", "Del", false, Selection.Size > 0))
                    RequestDelete = true;
                HvkGui::EndMenu();
            }
            if (HvkGui::BeginMenu("Options"))
            {
                HvkGui::PushItemWidth(HvkGui::GetFontSize() * 10);

                HvkGui::SeparatorText("Contents");
                HvkGui::Checkbox("Show Type Overlay", &ShowTypeOverlay);
                HvkGui::Checkbox("Allow Sorting", &AllowSorting);

                HvkGui::SeparatorText("Selection Behavior");
                HvkGui::Checkbox("Allow dragging unselected item", &AllowDragUnselected);
                HvkGui::Checkbox("Allow box-selection", &AllowBoxSelect);

                HvkGui::SeparatorText("Layout");
                HvkGui::SliderFloat("Icon Size", &IconSize, 16.0f, 128.0f, "%.0f");
                HvkGui::SameLine(); HelpMarker("Use Ctrl+Wheel to zoom");
                HvkGui::SliderInt("Icon Spacing", &IconSpacing, 0, 32);
                HvkGui::SliderInt("Icon Hit Spacing", &IconHitSpacing, 0, 32);
                HvkGui::Checkbox("Stretch Spacing", &StretchSpacing);
                HvkGui::PopItemWidth();
                HvkGui::EndMenu();
            }
            HvkGui::EndMenuBar();
        }

        // Show a table with ONLY one header row to showcase the idea/possibility of using this to provide a sorting UI
        if (AllowSorting)
        {
            HvkGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            ImGuiTableFlags table_flags_for_sort_specs = ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Borders;
            if (HvkGui::BeginTable("for_sort_specs_only", 2, table_flags_for_sort_specs, ImVec2(0.0f, HvkGui::GetFrameHeight())))
            {
                HvkGui::TableSetupColumn("Index");
                HvkGui::TableSetupColumn("Type");
                HvkGui::TableHeadersRow();
                if (ImGuiTableSortSpecs* sort_specs = HvkGui::TableGetSortSpecs())
                    if (sort_specs->SpecsDirty || RequestSort)
                    {
                        ExampleAsset::SortWithSortSpecs(sort_specs, Items.Data, Items.Size);
                        sort_specs->SpecsDirty = RequestSort = false;
                    }
                HvkGui::EndTable();
            }
            HvkGui::PopStyleVar();
        }

        ImGuiIO& io = HvkGui::GetIO();
        HvkGui::SetNextWindowContentSize(ImVec2(0.0f, LayoutOuterPadding + LayoutLineCount * (LayoutItemSize.y + LayoutItemSpacing)));
        if (HvkGui::BeginChild("Assets", ImVec2(0.0f, -HvkGui::GetTextLineHeightWithSpacing()), ImGuiChildFlags_Borders, ImGuiWindowFlags_NoMove))
        {
            ImDrawList* draw_list = HvkGui::GetWindowDrawList();

            const float avail_width = HvkGui::GetContentRegionAvail().x;
            UpdateLayoutSizes(avail_width);

            // Calculate and store start position.
            ImVec2 start_pos = HvkGui::GetCursorScreenPos();
            start_pos = ImVec2(start_pos.x + LayoutOuterPadding, start_pos.y + LayoutOuterPadding);
            HvkGui::SetCursorScreenPos(start_pos);

            // Multi-select
            ImGuiMultiSelectFlags ms_flags = ImGuiMultiSelectFlags_ClearOnEscape | ImGuiMultiSelectFlags_ClearOnClickVoid;

            // - Enable box-select (in 2D mode, so that changing box-select rectangle X1/X2 boundaries will affect clipped items)
            if (AllowBoxSelect)
                ms_flags |= ImGuiMultiSelectFlags_BoxSelect2d;

            // - This feature allows dragging an unselected item without selecting it (rarely used)
            if (AllowDragUnselected)
                ms_flags |= ImGuiMultiSelectFlags_SelectOnClickRelease;

            // - Enable keyboard wrapping on X axis
            // (FIXME-MULTISELECT: We haven't designed/exposed a general nav wrapping api yet, so this flag is provided as a courtesy to avoid doing:
            //    HvkGui::NavMoveRequestTryWrapping(HvkGui::GetCurrentWindow(), ImGuiNavMoveFlags_WrapX);
            // When we finish implementing a more general API for this, we will obsolete this flag in favor of the new system)
            ms_flags |= ImGuiMultiSelectFlags_NavWrapX;

            ImGuiMultiSelectIO* ms_io = HvkGui::BeginMultiSelect(ms_flags, Selection.Size, Items.Size);

            // Use custom selection adapter: store ID in selection (recommended)
            Selection.UserData = this;
            Selection.AdapterIndexToStorageId = [](ImGuiSelectionBasicStorage* self_, int idx) { ExampleAssetsBrowser* self = (ExampleAssetsBrowser*)self_->UserData; return self->Items[idx].ID; };
            Selection.ApplyRequests(ms_io);

            const bool want_delete = (HvkGui::Shortcut(ImGuiKey_Delete, ImGuiInputFlags_Repeat) && (Selection.Size > 0)) || RequestDelete;
            const int item_curr_idx_to_focus = want_delete ? Selection.ApplyDeletionPreLoop(ms_io, Items.Size) : -1;
            RequestDelete = false;

            // Push LayoutSelectableSpacing (which is LayoutItemSpacing minus hit-spacing, if we decide to have hit gaps between items)
            // Altering style ItemSpacing may seem unnecessary as we position every items using SetCursorScreenPos()...
            // But it is necessary for two reasons:
            // - Selectables uses it by default to visually fill the space between two items.
            // - The vertical spacing would be measured by Clipper to calculate line height if we didn't provide it explicitly (here we do).
            HvkGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(LayoutSelectableSpacing, LayoutSelectableSpacing));

            // Rendering parameters
            const ImU32 icon_type_overlay_colors[3] = { 0, IM_COL32(200, 70, 70, 255), IM_COL32(70, 170, 70, 255) };
            const ImU32 icon_bg_color = HvkGui::GetColorU32(IM_COL32(35, 35, 35, 220));
            const ImVec2 icon_type_overlay_size = ImVec2(4.0f, 4.0f);
            const bool display_label = (LayoutItemSize.x >= HvkGui::CalcTextSize("999").x);

            const int column_count = LayoutColumnCount;
            ImGuiListClipper clipper;
            clipper.Begin(LayoutLineCount, LayoutItemStep.y);
            if (item_curr_idx_to_focus != -1)
                clipper.IncludeItemByIndex(item_curr_idx_to_focus / column_count); // Ensure focused item line is not clipped.
            if (ms_io->RangeSrcItem != -1)
                clipper.IncludeItemByIndex((int)ms_io->RangeSrcItem / column_count); // Ensure RangeSrc item line is not clipped.
            while (clipper.Step())
            {
                for (int line_idx = clipper.DisplayStart; line_idx < clipper.DisplayEnd; line_idx++)
                {
                    const int item_min_idx_for_current_line = line_idx * column_count;
                    const int item_max_idx_for_current_line = IM_MIN((line_idx + 1) * column_count, Items.Size);
                    for (int item_idx = item_min_idx_for_current_line; item_idx < item_max_idx_for_current_line; ++item_idx)
                    {
                        ExampleAsset* item_data = &Items[item_idx];
                        HvkGui::PushID((int)item_data->ID);

                        // Position item
                        ImVec2 pos = ImVec2(start_pos.x + (item_idx % column_count) * LayoutItemStep.x, start_pos.y + line_idx * LayoutItemStep.y);
                        HvkGui::SetCursorScreenPos(pos);

                        HvkGui::SetNextItemSelectionUserData(item_idx);
                        bool item_is_selected = Selection.Contains((ImGuiID)item_data->ID);
                        bool item_is_visible = HvkGui::IsRectVisible(LayoutItemSize);
                        HvkGui::Selectable("", item_is_selected, ImGuiSelectableFlags_None, LayoutItemSize);

                        // Update our selection state immediately (without waiting for EndMultiSelect() requests)
                        // because we use this to alter the color of our text/icon.
                        if (HvkGui::IsItemToggledSelection())
                            item_is_selected = !item_is_selected;

                        // Focus (for after deletion)
                        if (item_curr_idx_to_focus == item_idx)
                            HvkGui::SetKeyboardFocusHere(-1);

                        // Drag and drop
                        if (HvkGui::BeginDragDropSource())
                        {
                            // Create payload with full selection OR single unselected item.
                            // (the later is only possible when using ImGuiMultiSelectFlags_SelectOnClickRelease)
                            if (HvkGui::GetDragDropPayload() == NULL)
                            {
                                ImVector<ImGuiID> payload_items;
                                void* it = NULL;
                                ImGuiID id = 0;
                                if (!item_is_selected)
                                    payload_items.push_back(item_data->ID);
                                else
                                    while (Selection.GetNextSelectedItem(&it, &id))
                                        payload_items.push_back(id);
                                HvkGui::SetDragDropPayload("ASSETS_BROWSER_ITEMS", payload_items.Data, (size_t)payload_items.size_in_bytes());
                            }

                            // Display payload content in tooltip, by extracting it from the payload data
                            // (we could read from selection, but it is more correct and reusable to read from payload)
                            const ImGuiPayload* payload = HvkGui::GetDragDropPayload();
                            const int payload_count = (int)payload->DataSize / (int)sizeof(ImGuiID);
                            HvkGui::Text("%d assets", payload_count);

                            HvkGui::EndDragDropSource();
                        }

                        // Render icon (a real app would likely display an image/thumbnail here)
                        // Because we use ImGuiMultiSelectFlags_BoxSelect2d, clipping vertical may occasionally be larger, so we coarse-clip our rendering as well.
                        if (item_is_visible)
                        {
                            ImVec2 box_min(pos.x - 1, pos.y - 1);
                            ImVec2 box_max(box_min.x + LayoutItemSize.x + 2, box_min.y + LayoutItemSize.y + 2); // Dubious
                            draw_list->AddRectFilled(box_min, box_max, icon_bg_color); // Background color
                            if (ShowTypeOverlay && item_data->Type != 0)
                            {
                                ImU32 type_col = icon_type_overlay_colors[item_data->Type % IM_ARRAYSIZE(icon_type_overlay_colors)];
                                draw_list->AddRectFilled(ImVec2(box_max.x - 2 - icon_type_overlay_size.x, box_min.y + 2), ImVec2(box_max.x - 2, box_min.y + 2 + icon_type_overlay_size.y), type_col);
                            }
                            if (display_label)
                            {
                                ImU32 label_col = HvkGui::GetColorU32(item_is_selected ? ImGuiCol_Text : ImGuiCol_TextDisabled);
                                char label[32];
                                sprintf(label, "%d", item_data->ID);
                                draw_list->AddText(ImVec2(box_min.x, box_max.y - HvkGui::GetFontSize()), label_col, label);
                            }
                        }

                        HvkGui::PopID();
                    }
                }
            }
            clipper.End();
            HvkGui::PopStyleVar(); // ImGuiStyleVar_ItemSpacing

            // Context menu
            if (HvkGui::BeginPopupContextWindow())
            {
                HvkGui::Text("Selection: %d items", Selection.Size);
                HvkGui::Separator();
                if (HvkGui::MenuItem("Delete", "Del", false, Selection.Size > 0))
                    RequestDelete = true;
                HvkGui::EndPopup();
            }

            ms_io = HvkGui::EndMultiSelect();
            Selection.ApplyRequests(ms_io);
            if (want_delete)
                Selection.ApplyDeletionPostLoop(ms_io, Items, item_curr_idx_to_focus);

            // Zooming with Ctrl+Wheel
            if (HvkGui::IsWindowAppearing())
                ZoomWheelAccum = 0.0f;
            if (HvkGui::IsWindowHovered() && io.MouseWheel != 0.0f && HvkGui::IsKeyDown(ImGuiMod_Ctrl) && HvkGui::IsAnyItemActive() == false)
            {
                ZoomWheelAccum += io.MouseWheel;
                if (fabsf(ZoomWheelAccum) >= 1.0f)
                {
                    // Calculate hovered item index from mouse location
                    // FIXME: Locking aiming on 'hovered_item_idx' (with a cool-down timer) would ensure zoom keeps on it.
                    const float hovered_item_nx = (io.MousePos.x - start_pos.x + LayoutItemSpacing * 0.5f) / LayoutItemStep.x;
                    const float hovered_item_ny = (io.MousePos.y - start_pos.y + LayoutItemSpacing * 0.5f) / LayoutItemStep.y;
                    const int hovered_item_idx = ((int)hovered_item_ny * LayoutColumnCount) + (int)hovered_item_nx;
                    //HvkGui::SetTooltip("%f,%f -> item %d", hovered_item_nx, hovered_item_ny, hovered_item_idx); // Move those 4 lines in block above for easy debugging

                    // Zoom
                    IconSize *= powf(1.1f, (float)(int)ZoomWheelAccum);
                    IconSize = IM_CLAMP(IconSize, 16.0f, 128.0f);
                    ZoomWheelAccum -= (int)ZoomWheelAccum;
                    UpdateLayoutSizes(avail_width);

                    // Manipulate scroll to that we will land at the same Y location of currently hovered item.
                    // - Calculate next frame position of item under mouse
                    // - Set new scroll position to be used in next HvkGui::BeginChild() call.
                    float hovered_item_rel_pos_y = ((float)(hovered_item_idx / LayoutColumnCount) + fmodf(hovered_item_ny, 1.0f)) * LayoutItemStep.y;
                    hovered_item_rel_pos_y += HvkGui::GetStyle().WindowPadding.y;
                    float mouse_local_y = io.MousePos.y - HvkGui::GetWindowPos().y;
                    HvkGui::SetScrollY(hovered_item_rel_pos_y - mouse_local_y);
                }
            }
        }
        HvkGui::EndChild();

        HvkGui::Text("Selected: %d/%d items", Selection.Size, Items.Size);
        HvkGui::End();
    }
};

void ShowExampleAppAssetsBrowser(bool* p_open)
{
    IMGUI_DEMO_MARKER("Examples/Assets Browser");
    static ExampleAssetsBrowser assets_browser;
    assets_browser.Draw("Example: Assets Browser", p_open);
}

// End of Demo code
#else

void HvkGui::ShowAboutWindow(bool*) {}
void HvkGui::ShowDemoWindow(bool*) {}
void HvkGui::ShowUserGuide() {}
void HvkGui::ShowStyleEditor(ImGuiStyle*) {}
bool HvkGui::ShowStyleSelector(const char*) { return false; }

#endif // #ifndef IMGUI_DISABLE_DEMO_WINDOWS

#endif // #ifndef IMGUI_DISABLE
