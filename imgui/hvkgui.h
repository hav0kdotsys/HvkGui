// dear HvkGui, v1.92.6 WIP
// (headers)

// Help:
// - Call and read HvkGui::ShowDemoWindow() in HvkGui_demo.cpp. All applications in examples/ are doing that.
// - Read top of HvkGui.cpp for more details, links and comments.
// - Add '#define HvkGui_DEFINE_MATH_OPERATORS' before including HvkGui.h (or in Hvkconfig.h) to access courtesy maths operators for HvkVec2 and HvkVec4.

// Resources:
// - FAQ ........................ https://dearHvkGui.com/faq (in repository as docs/FAQ.md)
// - Homepage ................... https://github.com/ocornut/HvkGui
// - Releases & Changelog ....... https://github.com/ocornut/HvkGui/releases
// - Gallery .................... https://github.com/ocornut/HvkGui/issues?q=label%3Agallery (please post your screenshots/video there!)
// - Wiki ....................... https://github.com/ocornut/HvkGui/wiki (lots of good stuff there)
//   - Getting Started            https://github.com/ocornut/HvkGui/wiki/Getting-Started (how to integrate in an existing app by adding ~25 lines of code)
//   - Third-party Extensions     https://github.com/ocornut/HvkGui/wiki/Useful-Extensions (HvkPlot & many more)
//   - Bindings/Backends          https://github.com/ocornut/HvkGui/wiki/Bindings (language bindings + backends for various tech/engines)
//   - Debug Tools                https://github.com/ocornut/HvkGui/wiki/Debug-Tools
//   - Glossary                   https://github.com/ocornut/HvkGui/wiki/Glossary
//   - Software using Dear HvkGui  https://github.com/ocornut/HvkGui/wiki/Software-using-dear-HvkGui
// - Issues & support ........... https://github.com/ocornut/HvkGui/issues
// - Test Engine & Automation ... https://github.com/ocornut/HvkGui_test_engine (test suite, test engine to automate your apps)
// - Web version of the Demo .... https://pthom.github.io/HvkGui_manual_online/manual/HvkGui_manual.html (w/ source code browser)

// For FIRST-TIME users having issues compiling/linking/running:
// please post in https://github.com/ocornut/HvkGui/discussions if you cannot find a solution in resources above.
// EVERYTHING ELSE should be asked in 'Issues'! We are building a database of cross-linked knowledge there.
// Since 1.92, we encourage font loading questions to also be posted in 'Issues'.

// Library Version
// (Integer encoded as XYYZZ for use in #if preprocessor conditionals, e.g. '#if HvkGui_VERSION_NUM >= 12345')
#define HvkGui_VERSION       "1.92.6 WIP"
#define HvkGui_VERSION_NUM   19254
#define HvkGui_HAS_TABLE             // Added BeginTable() - from HvkGui_VERSION_NUM >= 18000
#define HvkGui_HAS_TEXTURES          // Added HvkGuiBackendFlags_RendererHasTextures - from HvkGui_VERSION_NUM >= 19198

/*

Index of this file:
// [SECTION] Header mess
// [SECTION] Forward declarations and basic types
// [SECTION] Texture identifiers (HvkTextureID, HvkTextureRef)
// [SECTION] Dear HvkGui end-user API functions
// [SECTION] Flags & Enumerations
// [SECTION] Tables API flags and structures (HvkGuiTableFlags, HvkGuiTableColumnFlags, HvkGuiTableRowFlags, HvkGuiTableBgTarget, HvkGuiTableSortSpecs, HvkGuiTableColumnSortSpecs)
// [SECTION] Helpers: Debug log, Memory allocations macros, HvkVector<>
// [SECTION] HvkGuiStyle
// [SECTION] HvkGuiIO
// [SECTION] Misc data structures (HvkGuiInputTextCallbackData, HvkGuiSizeCallbackData, HvkGuiPayload)
// [SECTION] Helpers (HvkGuiOnceUponAFrame, HvkGuiTextFilter, HvkGuiTextBuffer, HvkGuiStorage, HvkGuiListClipper, Math Operators, HvkColor)
// [SECTION] Multi-Select API flags and structures (HvkGuiMultiSelectFlags, HvkGuiMultiSelectIO, HvkGuiSelectionRequest, HvkGuiSelectionBasicStorage, HvkGuiSelectionExternalStorage)
// [SECTION] Drawing API (HvkDrawCallback, HvkDrawCmd, HvkDrawIdx, HvkDrawVert, HvkDrawChannel, HvkDrawListSplitter, HvkDrawFlags, HvkDrawListFlags, HvkDrawList, HvkDrawData)
// [SECTION] Texture API (HvkTextureFormat, HvkTextureStatus, HvkTextureRect, HvkTextureData)
// [SECTION] Font API (HvkFontConfig, HvkFontGlyph, HvkFontGlyphRangesBuilder, HvkFontAtlasFlags, HvkFontAtlas, HvkFontBaked, HvkFont)
// [SECTION] Viewports (HvkGuiViewportFlags, HvkGuiViewport)
// [SECTION] HvkGuiPlatformIO + other Platform Dependent Interfaces (HvkGuiPlatformImeData)
// [SECTION] Obsolete functions and types

*/

#pragma once

// Configuration file with compile-time options
// (edit Hvkconfig.h or '#define HvkGui_USER_CONFIG "myfilename.h" from your build system)
#ifdef HvkGui_USER_CONFIG
#include HvkGui_USER_CONFIG
#endif
#include "hvkconfig.h"

#ifndef HvkGui_DISABLE

//-----------------------------------------------------------------------------
// [SECTION] Header mess
//-----------------------------------------------------------------------------

// Includes
#include <float.h>                  // FLT_MIN, FLT_MAX
#include <stdarg.h>                 // va_list, va_start, va_end
#include <stddef.h>                 // ptrdiff_t, NULL
#include <string.h>                 // memset, memmove, memcpy, strlen, strchr, strcpy, strcmp

// Define attributes of all API symbols declarations (e.g. for DLL under Windows)
// HvkGui_API is used for core HvkGui functions, HvkGui_IMPL_API is used for the default backends files (HvkGui_impl_xxx.h)
// Using dear HvkGui via a shared library is not recommended: we don't guarantee backward nor forward ABI compatibility + this is a call-heavy library and function call overhead adds up.
#ifndef HvkGui_API
#define HvkGui_API
#endif
#ifndef HvkGui_IMPL_API
#define HvkGui_IMPL_API              HvkGui_API
#endif

// Helper Macros
// (note: compiling with NDEBUG will usually strip out assert() to nothing, which is NOT recommended because we use asserts to notify of programmer mistakes.)
#ifndef Hvk_ASSERT
#include <assert.h>
#define Hvk_ASSERT(_EXPR)            assert(_EXPR)                               // You can override the default assert handler by editing Hvkconfig.h
#endif
#define Hvk_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*(_ARR))))     // Size of a static C-style array. Don't use on pointers!
#define Hvk_UNUSED(_VAR)             ((void)(_VAR))                              // Used to silence "unused variable warnings". Often useful as asserts may be stripped out from final builds.
#define Hvk_STRINGIFY_HELPER(_EXPR)  #_EXPR
#define Hvk_STRINGIFY(_EXPR)         Hvk_STRINGIFY_HELPER(_EXPR)                  // Preprocessor idiom to stringify e.g. an integer or a macro.

// Check that version and structures layouts are matching between compiled HvkGui code and caller. Read comments above DebugCheckVersionAndDataLayout() for details.
#define HvkGui_CHECKVERSION()        HvkGui::DebugCheckVersionAndDataLayout(HvkGui_VERSION, sizeof(HvkGuiIO), sizeof(HvkGuiStyle), sizeof(HvkVec2), sizeof(HvkVec4), sizeof(HvkDrawVert), sizeof(HvkDrawIdx))

// Helper Macros - Hvk_FMTARGS, Hvk_FMTLIST: Apply printf-style warnings to our formatting functions.
// (MSVC provides an equivalent mechanism via SAL Annotations but it requires the macros in a different
//  location. e.g. #include <sal.h> + void myprintf(_Printf_format_string_ const char* format, ...),
//  and only works when using Code Analysis, rather than just normal compiling).
// (see https://github.com/ocornut/HvkGui/issues/8871 for a patch to enable this for MSVC's Code Analysis)
#if !defined(HvkGui_USE_STB_SPRINTF) && defined(__MINGW32__) && !defined(__clang__)
#define Hvk_FMTARGS(FMT)             __attribute__((format(gnu_printf, FMT, FMT+1)))
#define Hvk_FMTLIST(FMT)             __attribute__((format(gnu_printf, FMT, 0)))
#elif !defined(HvkGui_USE_STB_SPRINTF) && (defined(__clang__) || defined(__GNUC__))
#define Hvk_FMTARGS(FMT)             __attribute__((format(printf, FMT, FMT+1)))
#define Hvk_FMTLIST(FMT)             __attribute__((format(printf, FMT, 0)))
#else
#define Hvk_FMTARGS(FMT)
#define Hvk_FMTLIST(FMT)
#endif

// Disable some of MSVC most aggressive Debug runtime checks in function header/footer (used in some simple/low-level functions)
#if defined(_MSC_VER) && !defined(__clang__)  && !defined(__INTEL_COMPILER) && !defined(HvkGui_DEBUG_PARANOID)
#define Hvk_MSVC_RUNTIME_CHECKS_OFF      __pragma(runtime_checks("",off))     __pragma(check_stack(off)) __pragma(strict_gs_check(push,off))
#define Hvk_MSVC_RUNTIME_CHECKS_RESTORE  __pragma(runtime_checks("",restore)) __pragma(check_stack())    __pragma(strict_gs_check(pop))
#else
#define Hvk_MSVC_RUNTIME_CHECKS_OFF
#define Hvk_MSVC_RUNTIME_CHECKS_RESTORE
#endif

// Warnings
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 26495)    // [Static Analyzer] Variable 'XXX' is uninitialized. Always initialize a member variable (type.6).
#endif
#if defined(__clang__)
#pragma clang diagnostic push
#if __has_warning("-Wunknown-warning-option")
#pragma clang diagnostic ignored "-Wunknown-warning-option"         // warning: unknown warning group 'xxx'
#endif
#pragma clang diagnostic ignored "-Wunknown-pragmas"                // warning: unknown warning group 'xxx'
#pragma clang diagnostic ignored "-Wold-style-cast"                 // warning: use of old-style cast
#pragma clang diagnostic ignored "-Wfloat-equal"                    // warning: comparing floating point with == or != is unsafe
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"  // warning: zero as null pointer constant
#pragma clang diagnostic ignored "-Wreserved-identifier"            // warning: identifier '_Xxx' is reserved because it starts with '_' followed by a capital letter
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"            // warning: 'xxx' is an unsafe pointer used for buffer access
#pragma clang diagnostic ignored "-Wnontrivial-memaccess"           // warning: first argument in call to 'memset' is a pointer to non-trivially copyable type
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"                          // warning: unknown option after '#pragma GCC diagnostic' kind
#pragma GCC diagnostic ignored "-Wfloat-equal"                      // warning: comparing floating-point with '==' or '!=' is unsafe
#pragma GCC diagnostic ignored "-Wclass-memaccess"                  // [__GNUC__ >= 8] warning: 'memset/memcpy' clearing/writing an object of type 'xxxx' with no trivial copy-assignment; use assignment or value-initialization instead
#endif

//-----------------------------------------------------------------------------
// [SECTION] Forward declarations and basic types
//-----------------------------------------------------------------------------

// Scalar data types
typedef unsigned int        HvkGuiID;// A unique ID used by widgets (typically the result of hashing a stack of string)
typedef signed char         HvkS8;   // 8-bit signed integer
typedef unsigned char       HvkU8;   // 8-bit unsigned integer
typedef signed short        HvkS16;  // 16-bit signed integer
typedef unsigned short      HvkU16;  // 16-bit unsigned integer
typedef signed int          HvkS32;  // 32-bit signed integer == int
typedef unsigned int        HvkU32;  // 32-bit unsigned integer (often used to store packed colors)
typedef signed   long long  HvkS64;  // 64-bit signed integer
typedef unsigned long long  HvkU64;  // 64-bit unsigned integer

// Forward declarations: HvkDrawList, HvkFontAtlas layer
struct HvkDrawChannel;               // Temporary storage to output draw commands out of order, used by HvkDrawListSplitter and HvkDrawList::ChannelsSplit()
struct HvkDrawCmd;                   // A single draw command within a parent HvkDrawList (generally maps to 1 GPU draw call, unless it is a callback)
struct HvkDrawData;                  // All draw command lists required to render the frame + pos/size coordinates to use for the projection matrix.
struct HvkDrawList;                  // A single draw command list (generally one per window, conceptually you may see this as a dynamic "mesh" builder)
struct HvkDrawListSharedData;        // Data shared among multiple draw lists (typically owned by parent HvkGui context, but you may create one yourself)
struct HvkDrawListSplitter;          // Helper to split a draw list into different layers which can be drawn into out of order, then flattened back.
struct HvkDrawVert;                  // A single vertex (pos + uv + col = 20 bytes by default. Override layout with HvkGui_OVERRIDE_DRAWVERT_STRUCT_LAYOUT)
struct HvkFont;                      // Runtime data for a single font within a parent HvkFontAtlas
struct HvkFontAtlas;                 // Runtime data for multiple fonts, bake multiple fonts into a single texture, TTF/OTF font loader
struct HvkFontAtlasBuilder;          // Opaque storage for building a HvkFontAtlas
struct HvkFontAtlasRect;             // Output of HvkFontAtlas::GetCustomRect() when using custom rectangles.
struct HvkFontBaked;                 // Baked data for a HvkFont at a given size.
struct HvkFontConfig;                // Configuration data when adding a font or merging fonts
struct HvkFontGlyph;                 // A single font glyph (code point + coordinates within in HvkFontAtlas + offset)
struct HvkFontGlyphRangesBuilder;    // Helper to build glyph ranges from text/string data
struct HvkFontLoader;                // Opaque interface to a font loading backend (stb_truetype, FreeType etc.).
struct HvkTextureData;               // Specs and pixel storage for a texture used by Dear HvkGui.
struct HvkTextureRect;               // Coordinates of a rectangle within a texture.
struct HvkColor;                     // Helper functions to create a color that can be converted to either u32 or float4 (*OBSOLETE* please avoid using)

// Forward declarations: HvkGui layer
struct HvkGuiContext;                // Dear HvkGui context (opaque structure, unless including HvkGui_internal.h)
struct HvkGuiIO;                     // Main configuration and I/O between your application and HvkGui (also see: HvkGuiPlatformIO)
struct HvkGuiInputTextCallbackData;  // Shared state of InputText() when using custom HvkGuiInputTextCallback (rare/advanced use)
struct HvkGuiKeyData;                // Storage for HvkGuiIO and IsKeyDown(), IsKeyPressed() etc functions.
struct HvkGuiListClipper;            // Helper to manually clip large list of items
struct HvkGuiMultiSelectIO;          // Structure to interact with a BeginMultiSelect()/EndMultiSelect() block
struct HvkGuiOnceUponAFrame;         // Helper for running a block of code not more than once a frame
struct HvkGuiPayload;                // User data payload for drag and drop operations
struct HvkGuiPlatformIO;             // Interface between platform/renderer backends and HvkGui (e.g. Clipboard, IME hooks). Extends HvkGuiIO. In docking branch, this gets extended to support multi-viewports.
struct HvkGuiPlatformImeData;        // Platform IME data for io.PlatformSetImeDataFn() function.
struct HvkGuiSelectionBasicStorage;  // Optional helper to store multi-selection state + apply multi-selection requests.
struct HvkGuiSelectionExternalStorage;//Optional helper to apply multi-selection requests to existing randomly accessible storage.
struct HvkGuiSelectionRequest;       // A selection request (stored in HvkGuiMultiSelectIO)
struct HvkGuiSizeCallbackData;       // Callback data when using SetNextWindowSizeConstraints() (rare/advanced use)
struct HvkGuiStorage;                // Helper for key->value storage (container sorted by key)
struct HvkGuiStoragePair;            // Helper for key->value storage (pair)
struct HvkGuiStyle;                  // Runtime data for styling/colors
struct HvkGuiTableSortSpecs;         // Sorting specifications for a table (often handling sort specs for a single column, occasionally more)
struct HvkGuiTableColumnSortSpecs;   // Sorting specification for one column of a table
struct HvkGuiTextBuffer;             // Helper to hold and append into a text buffer (~string builder)
struct HvkGuiTextFilter;             // Helper to parse and apply text filters (e.g. "aaaaa[,bbbbb][,ccccc]")
struct HvkGuiViewport;               // A Platform Window (always only one in 'master' branch), in the future may represent Platform Monitor

// Enumerations
// - We don't use strongly typed enums much because they add constraints (can't extend in private code, can't store typed in bit fields, extra casting on iteration)
// - Tip: Use your programming IDE navigation facilities on the names in the _central column_ below to find the actual flags/enum lists!
//   - In Visual Studio: Ctrl+Comma ("Edit.GoToAll") can follow symbols inside comments, whereas Ctrl+F12 ("Edit.GoToImplementation") cannot.
//   - In Visual Studio w/ Visual Assist installed: Alt+G ("VAssistX.GoToImplementation") can also follow symbols inside comments.
//   - In VS Code, CLion, etc.: Ctrl+Click can follow symbols inside comments.
enum HvkGuiDir : int;                // -> enum HvkGuiDir              // Enum: A cardinal direction (Left, Right, Up, Down)
enum HvkGuiKey : int;                // -> enum HvkGuiKey              // Enum: A key identifier (HvkGuiKey_XXX or HvkGuiMod_XXX value)
enum HvkGuiMouseSource : int;        // -> enum HvkGuiMouseSource      // Enum; A mouse input source identifier (Mouse, TouchScreen, Pen)
enum HvkGuiSortDirection : HvkU8;     // -> enum HvkGuiSortDirection    // Enum: A sorting direction (ascending or descending)
typedef int HvkGuiCol;               // -> enum HvkGuiCol_             // Enum: A color identifier for styling
typedef int HvkGuiCond;              // -> enum HvkGuiCond_            // Enum: A condition for many Set*() functions
typedef int HvkGuiDataType;          // -> enum HvkGuiDataType_        // Enum: A primary data type
typedef int HvkGuiMouseButton;       // -> enum HvkGuiMouseButton_     // Enum: A mouse button identifier (0=left, 1=right, 2=middle)
typedef int HvkGuiMouseCursor;       // -> enum HvkGuiMouseCursor_     // Enum: A mouse cursor shape
typedef int HvkGuiStyleVar;          // -> enum HvkGuiStyleVar_        // Enum: A variable identifier for styling
typedef int HvkGuiTableBgTarget;     // -> enum HvkGuiTableBgTarget_   // Enum: A color target for TableSetBgColor()

// Flags (declared as int to allow using as flags without overhead, and to not pollute the top of this file)
// - Tip: Use your programming IDE navigation facilities on the names in the _central column_ below to find the actual flags/enum lists!
//   - In Visual Studio: Ctrl+Comma ("Edit.GoToAll") can follow symbols inside comments, whereas Ctrl+F12 ("Edit.GoToImplementation") cannot.
//   - In Visual Studio w/ Visual Assist installed: Alt+G ("VAssistX.GoToImplementation") can also follow symbols inside comments.
//   - In VS Code, CLion, etc.: Ctrl+Click can follow symbols inside comments.
typedef int HvkDrawFlags;            // -> enum HvkDrawFlags_          // Flags: for HvkDrawList functions
typedef int HvkDrawListFlags;        // -> enum HvkDrawListFlags_      // Flags: for HvkDrawList instance
typedef int HvkDrawTextFlags;        // -> enum HvkDrawTextFlags_      // Internal, do not use!
typedef int HvkFontFlags;            // -> enum HvkFontFlags_          // Flags: for HvkFont
typedef int HvkFontAtlasFlags;       // -> enum HvkFontAtlasFlags_     // Flags: for HvkFontAtlas
typedef int HvkGuiBackendFlags;      // -> enum HvkGuiBackendFlags_    // Flags: for io.BackendFlags
typedef int HvkGuiButtonFlags;       // -> enum HvkGuiButtonFlags_     // Flags: for InvisibleButton()
typedef int HvkGuiChildFlags;        // -> enum HvkGuiChildFlags_      // Flags: for BeginChild()
typedef int HvkGuiColorEditFlags;    // -> enum HvkGuiColorEditFlags_  // Flags: for ColorEdit4(), ColorPicker4() etc.
typedef int HvkGuiConfigFlags;       // -> enum HvkGuiConfigFlags_     // Flags: for io.ConfigFlags
typedef int HvkGuiComboFlags;        // -> enum HvkGuiComboFlags_      // Flags: for BeginCombo()
typedef int HvkGuiDragDropFlags;     // -> enum HvkGuiDragDropFlags_   // Flags: for BeginDragDropSource(), AcceptDragDropPayload()
typedef int HvkGuiFocusedFlags;      // -> enum HvkGuiFocusedFlags_    // Flags: for IsWindowFocused()
typedef int HvkGuiHoveredFlags;      // -> enum HvkGuiHoveredFlags_    // Flags: for IsItemHovered(), IsWindowHovered() etc.
typedef int HvkGuiInputFlags;        // -> enum HvkGuiInputFlags_      // Flags: for Shortcut(), SetNextItemShortcut()
typedef int HvkGuiInputTextFlags;    // -> enum HvkGuiInputTextFlags_  // Flags: for InputText(), InputTextMultiline()
typedef int HvkGuiItemFlags;         // -> enum HvkGuiItemFlags_       // Flags: for PushItemFlag(), shared by all items
typedef int HvkGuiKeyChord;          // -> HvkGuiKey | HvkGuiMod_XXX    // Flags: for IsKeyChordPressed(), Shortcut() etc. an HvkGuiKey optionally OR-ed with one or more HvkGuiMod_XXX values.
typedef int HvkGuiListClipperFlags;  // -> enum HvkGuiListClipperFlags_// Flags: for HvkGuiListClipper
typedef int HvkGuiPopupFlags;        // -> enum HvkGuiPopupFlags_      // Flags: for OpenPopup*(), BeginPopupContext*(), IsPopupOpen()
typedef int HvkGuiMultiSelectFlags;  // -> enum HvkGuiMultiSelectFlags_// Flags: for BeginMultiSelect()
typedef int HvkGuiSelectableFlags;   // -> enum HvkGuiSelectableFlags_ // Flags: for Selectable()
typedef int HvkGuiSliderFlags;       // -> enum HvkGuiSliderFlags_     // Flags: for DragFloat(), DragInt(), SliderFloat(), SliderInt() etc.
typedef int HvkGuiTabBarFlags;       // -> enum HvkGuiTabBarFlags_     // Flags: for BeginTabBar()
typedef int HvkGuiTabItemFlags;      // -> enum HvkGuiTabItemFlags_    // Flags: for BeginTabItem()
typedef int HvkGuiTableFlags;        // -> enum HvkGuiTableFlags_      // Flags: For BeginTable()
typedef int HvkGuiTableColumnFlags;  // -> enum HvkGuiTableColumnFlags_// Flags: For TableSetupColumn()
typedef int HvkGuiTableRowFlags;     // -> enum HvkGuiTableRowFlags_   // Flags: For TableNextRow()
typedef int HvkGuiTreeNodeFlags;     // -> enum HvkGuiTreeNodeFlags_   // Flags: for TreeNode(), TreeNodeEx(), CollapsingHeader()
typedef int HvkGuiViewportFlags;     // -> enum HvkGuiViewportFlags_   // Flags: for HvkGuiViewport
typedef int HvkGuiWindowFlags;       // -> enum HvkGuiWindowFlags_     // Flags: for Begin(), BeginChild()

// Character types
// (we generally use UTF-8 encoded string in the API. This is storage specifically for a decoded character used for keyboard input and display)
typedef unsigned int HvkWchar32;     // A single decoded U32 character/code point. We encode them as multi bytes UTF-8 when used in strings.
typedef unsigned short HvkWchar16;   // A single decoded U16 character/code point. We encode them as multi bytes UTF-8 when used in strings.
#ifdef HvkGui_USE_WCHAR32            // HvkWchar [configurable type: override in Hvkconfig.h with '#define HvkGui_USE_WCHAR32' to support Unicode planes 1-16]
typedef HvkWchar32 HvkWchar;
#else
typedef HvkWchar16 HvkWchar;
#endif

// Multi-Selection item index or identifier when using BeginMultiSelect()
// - Used by SetNextItemSelectionUserData() + and inside HvkGuiMultiSelectIO structure.
// - Most users are likely to use this store an item INDEX but this may be used to store a POINTER/ID as well. Read comments near HvkGuiMultiSelectIO for details.
typedef HvkS64 HvkGuiSelectionUserData;

// Callback and functions types
typedef int     (*HvkGuiInputTextCallback)(HvkGuiInputTextCallbackData* data);    // Callback function for HvkGui::InputText()
typedef void    (*HvkGuiSizeCallback)(HvkGuiSizeCallbackData* data);              // Callback function for HvkGui::SetNextWindowSizeConstraints()
typedef void*   (*HvkGuiMemAllocFunc)(size_t sz, void* user_data);               // Function signature for HvkGui::SetAllocatorFunctions()
typedef void    (*HvkGuiMemFreeFunc)(void* ptr, void* user_data);                // Function signature for HvkGui::SetAllocatorFunctions()

// HvkVec2: 2D vector used to store positions, sizes etc. [Compile-time configurable type]
// - This is a frequently used type in the API. Consider using Hvk_VEC2_CLASS_EXTRA to create Hvkplicit cast from/to our preferred type.
// - Add '#define HvkGui_DEFINE_MATH_OPERATORS' before including this file (or in Hvkconfig.h) to access courtesy maths operators for HvkVec2 and HvkVec4.
Hvk_MSVC_RUNTIME_CHECKS_OFF
struct HvkVec2
{
    float                                   x, y;
    constexpr HvkVec2()                      : x(0.0f), y(0.0f) { }
    constexpr HvkVec2(float _x, float _y)    : x(_x), y(_y) { }
    float& operator[] (size_t idx)          { Hvk_ASSERT(idx == 0 || idx == 1); return ((float*)(void*)(char*)this)[idx]; } // We very rarely use this [] operator, so the assert overhead is fine.
    float  operator[] (size_t idx) const    { Hvk_ASSERT(idx == 0 || idx == 1); return ((const float*)(const void*)(const char*)this)[idx]; }
#ifdef Hvk_VEC2_CLASS_EXTRA
    Hvk_VEC2_CLASS_EXTRA     // Define additional constructors and Hvkplicit cast operators in Hvkconfig.h to convert back and forth between your math types and HvkVec2.
#endif
};

// HvkVec4: 4D vector used to store clipping rectangles, colors etc. [Compile-time configurable type]
struct HvkVec4
{
    float                                                     x, y, z, w;
    constexpr HvkVec4()                                        : x(0.0f), y(0.0f), z(0.0f), w(0.0f) { }
    constexpr HvkVec4(float _x, float _y, float _z, float _w)  : x(_x), y(_y), z(_z), w(_w) { }
#ifdef Hvk_VEC4_CLASS_EXTRA
    Hvk_VEC4_CLASS_EXTRA     // Define additional constructors and Hvkplicit cast operators in Hvkconfig.h to convert back and forth between your math types and HvkVec4.
#endif
};
Hvk_MSVC_RUNTIME_CHECKS_RESTORE

//-----------------------------------------------------------------------------
// [SECTION] Texture identifiers (HvkTextureID, HvkTextureRef)
//-----------------------------------------------------------------------------

// HvkTextureID = backend specific, low-level identifier for a texture uploaded in GPU/graphics system.
// [Compile-time configurable type]
// - When a Rendered Backend creates a texture, it store its native identifier into a HvkTextureID value.
//   (e.g. Used by DX11 backend to a `ID3D11ShaderResourceView*`; Used by OpenGL backends to store `GLuint`;
//         Used by SDLGPU backend to store a `SDL_GPUTextureSamplerBinding*`, etc.).
// - User may submit their own textures to e.g. HvkGui::Hvkage() function by passing this value.
// - During the rendering loop, the Renderer Backend retrieve the HvkTextureID, which stored inside a
//   HvkTextureRef, which is stored inside a HvkDrawCmd.
// - Compile-time type configuration:
//   - To use something other than a 64-bit value: add '#define HvkTextureID MyTextureType*' in your Hvkconfig.h file.
//   - This can be whatever to you want it to be! read the FAQ entry about textures for details.
//   - You may decide to store a higher-level structure containing texture, sampler, shader etc. with various
//     constructors if you like. You will need to implement ==/!= operators.
// History:
// - In v1.91.4 (2024/10/08): the default type for HvkTextureID was changed from 'void*' to 'HvkU64'. This allowed backends requiring 64-bit worth of data to build on 32-bit architectures. Use intermediary intptr_t cast and read FAQ if you have casting warnings.
// - In v1.92.0 (2025/06/11): added HvkTextureRef which carry either a HvkTextureID either a pointer to internal texture atlas. All user facing functions taking HvkTextureID changed to HvkTextureRef
#ifndef HvkTextureID
typedef HvkU64 HvkTextureID;      // Default: store up to 64-bits (any pointer or integer). A majority of backends are ok with that.
#endif

// Define this if you need 0 to be a valid HvkTextureID for your backend.
#ifndef HvkTextureID_Invalid
#define HvkTextureID_Invalid     ((HvkTextureID)0)
#endif

// HvkTextureRef = higher-level identifier for a texture. Store a HvkTextureID _or_ a HvkTextureData*.
// The identifier is valid even before the texture has been uploaded to the GPU/graphics system.
// This is what gets passed to functions such as `HvkGui::Hvkage()`, `HvkDrawList::AddImage()`.
// This is what gets stored in draw commands (`HvkDrawCmd`) to identify a texture during rendering.
// - When a texture is created by user code (e.g. custom Hvkages), we directly stores the low-level HvkTextureID.
//   Because of this, when displaying your own texture you are likely to ever only manage HvkTextureID values on your side.
// - When a texture is created by the backend, we stores a HvkTextureData* which becomes an indirection
//   to extract the HvkTextureID value during rendering, after texture upload has happened.
// - To create a HvkTextureRef from a HvkTextureData you can use HvkTextureData::GetTexRef().
//   We intentionally do not provide an HvkTextureRef constructor for this: we don't expect this
//   to be frequently useful to the end-user, and it would be erroneously called by many legacy code.
// - If you want to bind the current atlas when using custom rectangle, you can use io.Fonts->TexRef.
// - Binding generators for languages such as C (which don't have constructors), should provide a helper, e.g.
//      inline HvkTextureRef HvkTextureRefFromID(HvkTextureID tex_id) { HvkTextureRef tex_ref = { ._TexData = NULL, .TexID = tex_id }; return tex_ref; }
// In 1.92 we changed most drawing functions using HvkTextureID to use HvkTextureRef.
// We intentionally do not provide an Hvkplicit HvkTextureRef -> HvkTextureID cast operator because it is technically lossy to convert HvkTextureRef to HvkTextureID before rendering.
Hvk_MSVC_RUNTIME_CHECKS_OFF
struct HvkTextureRef
{
    HvkTextureRef()                          { _TexData = NULL; _TexID = HvkTextureID_Invalid; }
    HvkTextureRef(HvkTextureID tex_id)        { _TexData = NULL; _TexID = tex_id; }
#if !defined(HvkGui_DISABLE_OBSOLETE_FUNCTIONS) && !defined(HvkTextureID)
    HvkTextureRef(void* tex_id)              { _TexData = NULL; _TexID = (HvkTextureID)(size_t)tex_id; }  // For legacy backends casting to HvkTextureID
#endif

    inline HvkTextureID  GetTexID() const;   // == (_TexData ? _TexData->TexID : _TexID) // implemented below in the file.

    // Members (either are set, never both!)
    HvkTextureData*      _TexData;           //      A texture, generally owned by a HvkFontAtlas. Will convert to HvkTextureID during render loop, after texture has been uploaded.
    HvkTextureID         _TexID;             // _OR_ Low-level backend texture identifier, if already uploaded or created by user/app. Generally provided to e.g. HvkGui::Hvkage() calls.
};
Hvk_MSVC_RUNTIME_CHECKS_RESTORE

//-----------------------------------------------------------------------------
// [SECTION] Dear HvkGui end-user API functions
// (Note that HvkGui:: being a namespace, you can add extra HvkGui:: functions in your own separate file. Please don't modify HvkGui source files!)
//-----------------------------------------------------------------------------

namespace HvkGui
{
    // Context creation and access
    // - Each context create its own HvkFontAtlas by default. You may instance one yourself and pass it to CreateContext() to share a font atlas between contexts.
    // - DLL users: heaps and globals are not shared across DLL boundaries! You will need to call SetCurrentContext() + SetAllocatorFunctions()
    //   for each static/DLL boundary you are calling from. Read "Context and Memory Allocators" section of HvkGui.cpp for details.
    HvkGui_API HvkGuiContext* CreateContext(HvkFontAtlas* shared_font_atlas = NULL);
    HvkGui_API void          DestroyContext(HvkGuiContext* ctx = NULL);   // NULL = destroy current context
    HvkGui_API HvkGuiContext* GetCurrentContext();
    HvkGui_API void          SetCurrentContext(HvkGuiContext* ctx);

    // Main
    HvkGui_API HvkGuiIO&      GetIO();                                    // access the HvkGuiIO structure (mouse/keyboard/gamepad inputs, time, various configuration options/flags)
    HvkGui_API HvkGuiPlatformIO& GetPlatformIO();                         // access the HvkGuiPlatformIO structure (mostly hooks/functions to connect to platform/renderer and OS Clipboard, IME etc.)
    HvkGui_API HvkGuiStyle&   GetStyle();                                 // access the Style structure (colors, sizes). Always use PushStyleColor(), PushStyleVar() to modify style mid-frame!
    HvkGui_API void          NewFrame();                                 // start a new Dear HvkGui frame, you can submit any command from this point until Render()/EndFrame().
    HvkGui_API void          EndFrame();                                 // ends the Dear HvkGui frame. automatically called by Render(). If you don't need to render data (skipping rendering) you may call EndFrame() without Render()... but you'll have wasted CPU already! If you don't need to render, better to not create any windows and not call NewFrame() at all!
    HvkGui_API void          Render();                                   // ends the Dear HvkGui frame, finalize the draw data. You can then get call GetDrawData().
    HvkGui_API HvkDrawData*   GetDrawData();                              // valid after Render() and until the next call to NewFrame(). Call HvkGui_ImplXXXX_RenderDrawData() function in your Renderer Backend to render.

    // Demo, Debug, Information
    HvkGui_API void          ShowDemoWindow(bool* p_open = NULL);        // create Demo window. demonstrate most HvkGui features. call this to learn about the library! try to make it always available in your application!
    HvkGui_API void          ShowMetricsWindow(bool* p_open = NULL);     // create Metrics/Debugger window. display Dear HvkGui internals: windows, draw commands, various internal state, etc.
    HvkGui_API void          ShowDebugLogWindow(bool* p_open = NULL);    // create Debug Log window. display a simplified log of Hvkportant dear HvkGui events.
    HvkGui_API void          ShowIDStackToolWindow(bool* p_open = NULL); // create Stack Tool window. hover items with mouse to query information about the source of their unique ID.
    HvkGui_API void          ShowAboutWindow(bool* p_open = NULL);       // create About window. display Dear HvkGui version, credits and build/system information.
    HvkGui_API void          ShowStyleEditor(HvkGuiStyle* ref = NULL);    // add style editor block (not a window). you can pass in a reference HvkGuiStyle structure to compare to, revert to and save to (else it uses the default style)
    HvkGui_API bool          ShowStyleSelector(const char* label);       // add style selector block (not a window), essentially a combo listing the default styles.
    HvkGui_API void          ShowFontSelector(const char* label);        // add font selector block (not a window), essentially a combo listing the loaded fonts.
    HvkGui_API void          ShowUserGuide();                            // add basic help/info block (not a window): how to manipulate HvkGui as an end-user (mouse/keyboard controls).
    HvkGui_API const char*   GetVersion();                               // get the compiled version string e.g. "1.80 WIP" (essentially the value for HvkGui_VERSION from the compiled version of HvkGui.cpp)

    // Styles
    HvkGui_API void          StyleColorsDark(HvkGuiStyle* dst = NULL);    // new, recommended style (default)
    HvkGui_API void          StyleColorsLight(HvkGuiStyle* dst = NULL);   // best used with borders and a custom, thicker font
    HvkGui_API void          StyleColorsClassic(HvkGuiStyle* dst = NULL); // classic HvkGui style

    // Windows
    // - Begin() = push window to the stack and start appending to it. End() = pop window from the stack.
    // - Passing 'bool* p_open != NULL' shows a window-closing widget in the upper-right corner of the window,
    //   which clicking will set the boolean to false when clicked.
    // - You may append multiple times to the same window during the same frame by calling Begin()/End() pairs multiple times.
    //   Some information such as 'flags' or 'p_open' will only be considered by the first call to Begin().
    // - Begin() return false to indicate the window is collapsed or fully clipped, so you may early out and omit submitting
    //   anything to the window. Always call a matching End() for each Begin() call, regardless of its return value!
    //   [Hvkportant: due to legacy reason, Begin/End and BeginChild/EndChild are inconsistent with all other functions
    //    such as BeginMenu/EndMenu, BeginPopup/EndPopup, etc. where the EndXXX call should only be called if the corresponding
    //    BeginXXX function returned true. Begin and BeginChild are the only odd ones out. Will be fixed in a future update.]
    // - Note that the bottom of window stack always contains a window called "Debug".
    HvkGui_API bool          Begin(const char* name, bool* p_open = NULL, HvkGuiWindowFlags flags = 0);
    HvkGui_API void          End();

    // Child Windows
    // - Use child windows to begin into a self-contained independent scrolling/clipping regions within a host window. Child windows can embed their own child.
    // - Before 1.90 (November 2023), the "HvkGuiChildFlags child_flags = 0" parameter was "bool border = false".
    //   This API is backward compatible with old code, as we guarantee that HvkGuiChildFlags_Borders == true.
    //   Consider updating your old code:
    //      BeginChild("Name", size, false)   -> Begin("Name", size, 0); or Begin("Name", size, HvkGuiChildFlags_None);
    //      BeginChild("Name", size, true)    -> Begin("Name", size, HvkGuiChildFlags_Borders);
    // - Manual sizing (each axis can use a different setting e.g. HvkVec2(0.0f, 400.0f)):
    //     == 0.0f: use remaining parent window size for this axis.
    //      > 0.0f: use specified size for this axis.
    //      < 0.0f: right/bottom-align to specified distance from available content boundaries.
    // - Specifying HvkGuiChildFlags_AutoResizeX or HvkGuiChildFlags_AutoResizeY makes the sizing automatic based on child contents.
    //   Combining both HvkGuiChildFlags_AutoResizeX _and_ HvkGuiChildFlags_AutoResizeY defeats purpose of a scrolling region and is NOT recommended.
    // - BeginChild() returns false to indicate the window is collapsed or fully clipped, so you may early out and omit submitting
    //   anything to the window. Always call a matching EndChild() for each BeginChild() call, regardless of its return value.
    //   [Hvkportant: due to legacy reason, Begin/End and BeginChild/EndChild are inconsistent with all other functions
    //    such as BeginMenu/EndMenu, BeginPopup/EndPopup, etc. where the EndXXX call should only be called if the corresponding
    //    BeginXXX function returned true. Begin and BeginChild are the only odd ones out. Will be fixed in a future update.]
    HvkGui_API bool          BeginChild(const char* str_id, const HvkVec2& size = HvkVec2(0, 0), HvkGuiChildFlags child_flags = 0, HvkGuiWindowFlags window_flags = 0);
    HvkGui_API bool          BeginChild(HvkGuiID id, const HvkVec2& size = HvkVec2(0, 0), HvkGuiChildFlags child_flags = 0, HvkGuiWindowFlags window_flags = 0);
    HvkGui_API void          EndChild();

    // Windows Utilities
    // - 'current window' = the window we are appending into while inside a Begin()/End() block. 'next window' = next window we will Begin() into.
    HvkGui_API bool          IsWindowAppearing();
    HvkGui_API bool          IsWindowCollapsed();
    HvkGui_API bool          IsWindowFocused(HvkGuiFocusedFlags flags=0); // is current window focused? or its root/child, depending on flags. see flags for options.
    HvkGui_API bool          IsWindowHovered(HvkGuiHoveredFlags flags=0); // is current window hovered and hoverable (e.g. not blocked by a popup/modal)? See HvkGuiHoveredFlags_ for options. HvkPORTANT: If you are trying to check whether your mouse should be dispatched to Dear HvkGui or to your underlying app, you should not use this function! Use the 'io.WantCaptureMouse' boolean for that! Refer to FAQ entry "How can I tell whether to dispatch mouse/keyboard to Dear HvkGui or my application?" for details.
    HvkGui_API HvkDrawList*   GetWindowDrawList();                        // get draw list associated to the current window, to append your own drawing primitives
    HvkGui_API HvkVec2        GetWindowPos();                             // get current window position in screen space (IT IS UNLIKELY YOU EVER NEED TO USE THIS. Consider always using GetCursorScreenPos() and GetContentRegionAvail() instead)
    HvkGui_API HvkVec2        GetWindowSize();                            // get current window size (IT IS UNLIKELY YOU EVER NEED TO USE THIS. Consider always using GetCursorScreenPos() and GetContentRegionAvail() instead)
    HvkGui_API float         GetWindowWidth();                           // get current window width (IT IS UNLIKELY YOU EVER NEED TO USE THIS). Shortcut for GetWindowSize().x.
    HvkGui_API float         GetWindowHeight();                          // get current window height (IT IS UNLIKELY YOU EVER NEED TO USE THIS). Shortcut for GetWindowSize().y.

    // Window manipulation
    // - Prefer using SetNextXXX functions (before Begin) rather that SetXXX functions (after Begin).
    HvkGui_API void          SetNextWindowPos(const HvkVec2& pos, HvkGuiCond cond = 0, const HvkVec2& pivot = HvkVec2(0, 0)); // set next window position. call before Begin(). use pivot=(0.5f,0.5f) to center on given point, etc.
    HvkGui_API void          SetNextWindowSize(const HvkVec2& size, HvkGuiCond cond = 0);                  // set next window size. set axis to 0.0f to force an auto-fit on this axis. call before Begin()
    HvkGui_API void          SetNextWindowSizeConstraints(const HvkVec2& size_min, const HvkVec2& size_max, HvkGuiSizeCallback custom_callback = NULL, void* custom_callback_data = NULL); // set next window size limits. use 0.0f or FLT_MAX if you don't want limits. Use -1 for both min and max of same axis to preserve current size (which itself is a constraint). Use callback to apply non-trivial programmatic constraints.
    HvkGui_API void          SetNextWindowContentSize(const HvkVec2& size);                               // set next window content size (~ scrollable client area, which enforce the range of scrollbars). Not including window decorations (title bar, menu bar, etc.) nor WindowPadding. set an axis to 0.0f to leave it automatic. call before Begin()
    HvkGui_API void          SetNextWindowCollapsed(bool collapsed, HvkGuiCond cond = 0);                 // set next window collapsed state. call before Begin()
    HvkGui_API void          SetNextWindowFocus();                                                       // set next window to be focused / top-most. call before Begin()
    HvkGui_API void          SetNextWindowScroll(const HvkVec2& scroll);                                  // set next window scrolling value (use < 0.0f to not affect a given axis).
    HvkGui_API void          SetNextWindowBgAlpha(float alpha);                                          // set next window background color alpha. helper to easily override the Alpha component of HvkGuiCol_WindowBg/ChildBg/PopupBg. you may also use HvkGuiWindowFlags_NoBackground.
    HvkGui_API void          SetWindowPos(const HvkVec2& pos, HvkGuiCond cond = 0);                        // (not recommended) set current window position - call within Begin()/End(). prefer using SetNextWindowPos(), as this may incur tearing and side-effects.
    HvkGui_API void          SetWindowSize(const HvkVec2& size, HvkGuiCond cond = 0);                      // (not recommended) set current window size - call within Begin()/End(). set to HvkVec2(0, 0) to force an auto-fit. prefer using SetNextWindowSize(), as this may incur tearing and minor side-effects.
    HvkGui_API void          SetWindowCollapsed(bool collapsed, HvkGuiCond cond = 0);                     // (not recommended) set current window collapsed state. prefer using SetNextWindowCollapsed().
    HvkGui_API void          SetWindowFocus();                                                           // (not recommended) set current window to be focused / top-most. prefer using SetNextWindowFocus().
    HvkGui_API void          SetWindowPos(const char* name, const HvkVec2& pos, HvkGuiCond cond = 0);      // set named window position.
    HvkGui_API void          SetWindowSize(const char* name, const HvkVec2& size, HvkGuiCond cond = 0);    // set named window size. set axis to 0.0f to force an auto-fit on this axis.
    HvkGui_API void          SetWindowCollapsed(const char* name, bool collapsed, HvkGuiCond cond = 0);   // set named window collapsed state
    HvkGui_API void          SetWindowFocus(const char* name);                                           // set named window to be focused / top-most. use NULL to remove focus.

    // Windows Scrolling
    // - Any change of Scroll will be applied at the beginning of next frame in the first call to Begin().
    // - You may instead use SetNextWindowScroll() prior to calling Begin() to avoid this delay, as an alternative to using SetScrollX()/SetScrollY().
    HvkGui_API float         GetScrollX();                                                   // get scrolling amount [0 .. GetScrollMaxX()]
    HvkGui_API float         GetScrollY();                                                   // get scrolling amount [0 .. GetScrollMaxY()]
    HvkGui_API void          SetScrollX(float scroll_x);                                     // set scrolling amount [0 .. GetScrollMaxX()]
    HvkGui_API void          SetScrollY(float scroll_y);                                     // set scrolling amount [0 .. GetScrollMaxY()]
    HvkGui_API float         GetScrollMaxX();                                                // get maximum scrolling amount ~~ ContentSize.x - WindowSize.x - DecorationsSize.x
    HvkGui_API float         GetScrollMaxY();                                                // get maximum scrolling amount ~~ ContentSize.y - WindowSize.y - DecorationsSize.y
    HvkGui_API void          SetScrollHereX(float center_x_ratio = 0.5f);                    // adjust scrolling amount to make current cursor position visible. center_x_ratio=0.0: left, 0.5: center, 1.0: right. When using to make a "default/current item" visible, consider using SetItemDefaultFocus() instead.
    HvkGui_API void          SetScrollHereY(float center_y_ratio = 0.5f);                    // adjust scrolling amount to make current cursor position visible. center_y_ratio=0.0: top, 0.5: center, 1.0: bottom. When using to make a "default/current item" visible, consider using SetItemDefaultFocus() instead.
    HvkGui_API void          SetScrollFromPosX(float local_x, float center_x_ratio = 0.5f);  // adjust scrolling amount to make given position visible. Generally GetCursorStartPos() + offset to compute a valid position.
    HvkGui_API void          SetScrollFromPosY(float local_y, float center_y_ratio = 0.5f);  // adjust scrolling amount to make given position visible. Generally GetCursorStartPos() + offset to compute a valid position.

    // Parameters stacks (font)
    //  - PushFont(font, 0.0f)                       // Change font and keep current size
    //  - PushFont(NULL, 20.0f)                      // Keep font and change current size
    //  - PushFont(font, 20.0f)                      // Change font and set size to 20.0f
    //  - PushFont(font, style.FontSizeBase * 2.0f)  // Change font and set size to be twice bigger than current size.
    //  - PushFont(font, font->LegacySize)           // Change font and set size to size passed to AddFontXXX() function. Same as pre-1.92 behavior.
    // *HvkPORTANT* before 1.92, fonts had a single size. They can now be dynamically be adjusted.
    //  - In 1.92 we have REMOVED the single parameter version of PushFont() because it seems like the easiest way to provide an error-proof transition.
    //  - PushFont(font) before 1.92 = PushFont(font, font->LegacySize) after 1.92          // Use default font size as passed to AddFontXXX() function.
    // *HvkPORTANT* global scale factors are applied over the provided size.
    //  - Global scale factors are: 'style.FontScaleMain', 'style.FontScaleDpi' and maybe more.
    // -  If you want to apply a factor to the _current_ font size:
    //  - CORRECT:   PushFont(NULL, style.FontSizeBase)         // use current unscaled size    == does nothing
    //  - CORRECT:   PushFont(NULL, style.FontSizeBase * 2.0f)  // use current unscaled size x2 == make text twice bigger
    //  - INCORRECT: PushFont(NULL, GetFontSize())              // INCORRECT! using size after global factors already applied == GLOBAL SCALING FACTORS WILL APPLY TWICE!
    //  - INCORRECT: PushFont(NULL, GetFontSize() * 2.0f)       // INCORRECT! using size after global factors already applied == GLOBAL SCALING FACTORS WILL APPLY TWICE!
    HvkGui_API void          PushFont(HvkFont* font, float font_size_base_unscaled);          // Use NULL as a shortcut to keep current font. Use 0.0f to keep current size.
    HvkGui_API void          PopFont();
    HvkGui_API HvkFont*       GetFont();                                                      // get current font
    HvkGui_API float         GetFontSize();                                                  // get current scaled font size (= height in pixels). AFTER global scale factors applied. *HvkPORTANT* DO NOT PASS THIS VALUE TO PushFont()! Use HvkGui::GetStyle().FontSizeBase to get value before global scale factors.
    HvkGui_API HvkFontBaked*  GetFontBaked();                                                 // get current font bound at current size // == GetFont()->GetFontBaked(GetFontSize())

    // Parameters stacks (shared)
    HvkGui_API void          PushStyleColor(HvkGuiCol idx, HvkU32 col);                        // modify a style color. always use this if you modify the style after NewFrame().
    HvkGui_API void          PushStyleColor(HvkGuiCol idx, const HvkVec4& col);
    HvkGui_API void          PopStyleColor(int count = 1);
    HvkGui_API void          PushStyleVar(HvkGuiStyleVar idx, float val);                     // modify a style float variable. always use this if you modify the style after NewFrame()!
    HvkGui_API void          PushStyleVar(HvkGuiStyleVar idx, const HvkVec2& val);             // modify a style HvkVec2 variable. "
    HvkGui_API void          PushStyleVarX(HvkGuiStyleVar idx, float val_x);                  // modify X component of a style HvkVec2 variable. "
    HvkGui_API void          PushStyleVarY(HvkGuiStyleVar idx, float val_y);                  // modify Y component of a style HvkVec2 variable. "
    HvkGui_API void          PopStyleVar(int count = 1);
    HvkGui_API void          PushItemFlag(HvkGuiItemFlags option, bool enabled);              // modify specified shared item flag, e.g. PushItemFlag(HvkGuiItemFlags_NoTabStop, true)
    HvkGui_API void          PopItemFlag();

    // Parameters stacks (current window)
    HvkGui_API void          PushItemWidth(float item_width);                                // push width of items for common large "item+label" widgets. >0.0f: width in pixels, <0.0f align xx pixels to the right of window (so -FLT_MIN always align width to the right side).
    HvkGui_API void          PopItemWidth();
    HvkGui_API void          SetNextItemWidth(float item_width);                             // set width of the _next_ common large "item+label" widget. >0.0f: width in pixels, <0.0f align xx pixels to the right of window (so -FLT_MIN always align width to the right side)
    HvkGui_API float         CalcItemWidth();                                                // width of item given pushed settings and current cursor position. NOT necessarily the width of last item unlike most 'Item' functions.
    HvkGui_API void          PushTextWrapPos(float wrap_local_pos_x = 0.0f);                 // push word-wrapping position for Text*() commands. < 0.0f: no wrapping; 0.0f: wrap to end of window (or column); > 0.0f: wrap at 'wrap_pos_x' position in window local space
    HvkGui_API void          PopTextWrapPos();

    // Style read access
    // - Use the ShowStyleEditor() function to interactively see/edit the colors.
    HvkGui_API HvkVec2        GetFontTexUvWhitePixel();                                       // get UV coordinate for a white pixel, useful to draw custom shapes via the HvkDrawList API
    HvkGui_API HvkU32         GetColorU32(HvkGuiCol idx, float alpha_mul = 1.0f);              // retrieve given style color with style alpha applied and optional extra alpha multiplier, packed as a 32-bit value suitable for HvkDrawList
    HvkGui_API HvkU32         GetColorU32(const HvkVec4& col);                                 // retrieve given color with style alpha applied, packed as a 32-bit value suitable for HvkDrawList
    HvkGui_API HvkU32         GetColorU32(HvkU32 col, float alpha_mul = 1.0f);                 // retrieve given color with style alpha applied, packed as a 32-bit value suitable for HvkDrawList
    HvkGui_API const HvkVec4& GetStyleColorVec4(HvkGuiCol idx);                                // retrieve style color as stored in HvkGuiStyle structure. use to feed back into PushStyleColor(), otherwise use GetColorU32() to get style color with style alpha baked in.

    // Layout cursor positioning
    // - By "cursor" we mean the current output position.
    // - The typical widget behavior is to output themselves at the current cursor position, then move the cursor one line down.
    // - You can call SameLine() between widgets to undo the last carriage return and output at the right of the preceding widget.
    // - YOU CAN DO 99% OF WHAT YOU NEED WITH ONLY GetCursorScreenPos() and GetContentRegionAvail().
    // - Attention! We currently have inconsistencies between window-local and absolute positions we will aim to fix with future API:
    //    - Absolute coordinate:        GetCursorScreenPos(), SetCursorScreenPos(), all HvkDrawList:: functions. -> this is the preferred way forward.
    //    - Window-local coordinates:   SameLine(offset), GetCursorPos(), SetCursorPos(), GetCursorStartPos(), PushTextWrapPos()
    //    - Window-local coordinates:   GetContentRegionMax(), GetWindowContentRegionMin(), GetWindowContentRegionMax() --> all obsoleted. YOU DON'T NEED THEM.
    // - GetCursorScreenPos() = GetCursorPos() + GetWindowPos(). GetWindowPos() is almost only ever useful to convert from window-local to absolute coordinates. Try not to use it.
    HvkGui_API HvkVec2        GetCursorScreenPos();                                           // cursor position, absolute coordinates. THIS IS YOUR BEST FRIEND (prefer using this rather than GetCursorPos(), also more useful to work with HvkDrawList API).
    HvkGui_API void          SetCursorScreenPos(const HvkVec2& pos);                          // cursor position, absolute coordinates. THIS IS YOUR BEST FRIEND.
    HvkGui_API HvkVec2        GetContentRegionAvail();                                        // available space from current position. THIS IS YOUR BEST FRIEND.
    HvkGui_API HvkVec2        GetCursorPos();                                                 // [window-local] cursor position in window-local coordinates. This is not your best friend.
    HvkGui_API float         GetCursorPosX();                                                // [window-local] "
    HvkGui_API float         GetCursorPosY();                                                // [window-local] "
    HvkGui_API void          SetCursorPos(const HvkVec2& local_pos);                          // [window-local] "
    HvkGui_API void          SetCursorPosX(float local_x);                                   // [window-local] "
    HvkGui_API void          SetCursorPosY(float local_y);                                   // [window-local] "
    HvkGui_API HvkVec2        GetCursorStartPos();                                            // [window-local] initial cursor position, in window-local coordinates. Call GetCursorScreenPos() after Begin() to get the absolute coordinates version.

    // Other layout functions
    HvkGui_API void          Separator();                                                    // separator, generally horizontal. inside a menu bar or in horizontal layout mode, this becomes a vertical separator.
    HvkGui_API void          SameLine(float offset_from_start_x=0.0f, float spacing=-1.0f);  // call between widgets or groups to layout them horizontally. X position given in window coordinates.
    HvkGui_API void          NewLine();                                                      // undo a SameLine() or force a new line when in a horizontal-layout context.
    HvkGui_API void          Spacing();                                                      // add vertical spacing.
    HvkGui_API void          Dummy(const HvkVec2& size);                                      // add a dummy item of given size. unlike InvisibleButton(), Dummy() won't take the mouse click or be navigable into.
    HvkGui_API void          Indent(float indent_w = 0.0f);                                  // move content position toward the right, by indent_w, or style.IndentSpacing if indent_w <= 0
    HvkGui_API void          Unindent(float indent_w = 0.0f);                                // move content position back to the left, by indent_w, or style.IndentSpacing if indent_w <= 0
    HvkGui_API void          BeginGroup();                                                   // lock horizontal starting position
    HvkGui_API void          EndGroup();                                                     // unlock horizontal starting position + capture the whole group bounding box into one "item" (so you can use IsItemHovered() or layout primitives such as SameLine() on whole group, etc.)
    HvkGui_API void          AlignTextToFramePadding();                                      // vertically align upcoming text baseline to FramePadding.y so that it will align properly to regularly framed items (call if you have text on a line before a framed item)
    HvkGui_API float         GetTextLineHeight();                                            // ~ FontSize
    HvkGui_API float         GetTextLineHeightWithSpacing();                                 // ~ FontSize + style.ItemSpacing.y (distance in pixels between 2 consecutive lines of text)
    HvkGui_API float         GetFrameHeight();                                               // ~ FontSize + style.FramePadding.y * 2
    HvkGui_API float         GetFrameHeightWithSpacing();                                    // ~ FontSize + style.FramePadding.y * 2 + style.ItemSpacing.y (distance in pixels between 2 consecutive lines of framed widgets)

    // ID stack/scopes
    // Read the FAQ (docs/FAQ.md or http://dearHvkGui.com/faq) for more details about how ID are handled in dear HvkGui.
    // - Those questions are answered and Hvkpacted by understanding of the ID stack system:
    //   - "Q: Why is my widget not reacting when I click on it?"
    //   - "Q: How can I have widgets with an empty label?"
    //   - "Q: How can I have multiple widgets with the same label?"
    // - Short version: ID are hashes of the entire ID stack. If you are creating widgets in a loop you most likely
    //   want to push a unique identifier (e.g. object pointer, loop index) to uniquely differentiate them.
    // - You can also use the "Label##foobar" syntax within widget label to distinguish them from each others.
    // - In this header file we use the "label"/"name" terminology to denote a string that will be displayed + used as an ID,
    //   whereas "str_id" denote a string that is only used as an ID and not normally displayed.
    HvkGui_API void          PushID(const char* str_id);                                     // push string into the ID stack (will hash string).
    HvkGui_API void          PushID(const char* str_id_begin, const char* str_id_end);       // push string into the ID stack (will hash string).
    HvkGui_API void          PushID(const void* ptr_id);                                     // push pointer into the ID stack (will hash pointer).
    HvkGui_API void          PushID(int int_id);                                             // push integer into the ID stack (will hash integer).
    HvkGui_API void          PopID();                                                        // pop from the ID stack.
    HvkGui_API HvkGuiID       GetID(const char* str_id);                                      // calculate unique ID (hash of whole ID stack + given parameter). e.g. if you want to query into HvkGuiStorage yourself
    HvkGui_API HvkGuiID       GetID(const char* str_id_begin, const char* str_id_end);
    HvkGui_API HvkGuiID       GetID(const void* ptr_id);
    HvkGui_API HvkGuiID       GetID(int int_id);

    // Widgets: Text
    HvkGui_API void          TextUnformatted(const char* text, const char* text_end = NULL); // raw text without formatting. Roughly equivalent to Text("%s", text) but: A) doesn't require null terminated string if 'text_end' is specified, B) it's faster, no memory copy is done, no buffer size limits, recommended for long chunks of text.
    HvkGui_API void          Text(const char* fmt, ...)                                      Hvk_FMTARGS(1); // formatted text
    HvkGui_API void          TextV(const char* fmt, va_list args)                            Hvk_FMTLIST(1);
    HvkGui_API void          TextColored(const HvkVec4& col, const char* fmt, ...)            Hvk_FMTARGS(2); // shortcut for PushStyleColor(HvkGuiCol_Text, col); Text(fmt, ...); PopStyleColor();
    HvkGui_API void          TextColoredV(const HvkVec4& col, const char* fmt, va_list args)  Hvk_FMTLIST(2);
    HvkGui_API void          TextDisabled(const char* fmt, ...)                              Hvk_FMTARGS(1); // shortcut for PushStyleColor(HvkGuiCol_Text, style.Colors[HvkGuiCol_TextDisabled]); Text(fmt, ...); PopStyleColor();
    HvkGui_API void          TextDisabledV(const char* fmt, va_list args)                    Hvk_FMTLIST(1);
    HvkGui_API void          TextWrapped(const char* fmt, ...)                               Hvk_FMTARGS(1); // shortcut for PushTextWrapPos(0.0f); Text(fmt, ...); PopTextWrapPos();. Note that this won't work on an auto-resizing window if there's no other widgets to extend the window width, yoy may need to set a size using SetNextWindowSize().
    HvkGui_API void          TextWrappedV(const char* fmt, va_list args)                     Hvk_FMTLIST(1);
    HvkGui_API void          LabelText(const char* label, const char* fmt, ...)              Hvk_FMTARGS(2); // display text+label aligned the same way as value+label widgets
    HvkGui_API void          LabelTextV(const char* label, const char* fmt, va_list args)    Hvk_FMTLIST(2);
    HvkGui_API void          BulletText(const char* fmt, ...)                                Hvk_FMTARGS(1); // shortcut for Bullet()+Text()
    HvkGui_API void          BulletTextV(const char* fmt, va_list args)                      Hvk_FMTLIST(1);
    HvkGui_API void          SeparatorText(const char* label);                               // currently: formatted text with a horizontal line

    // Widgets: Main
    // - Most widgets return true when the value has been changed or when pressed/selected
    // - You may also use one of the many IsItemXXX functions (e.g. IsItemActive, IsItemHovered, etc.) to query widget state.
    HvkGui_API bool          Button(const char* label, const HvkVec2& size = HvkVec2(0, 0));   // button
    HvkGui_API bool          SmallButton(const char* label);                                 // button with (FramePadding.y == 0) to easily embed within text
    HvkGui_API bool          InvisibleButton(const char* str_id, const HvkVec2& size, HvkGuiButtonFlags flags = 0); // flexible button behavior without the visuals, frequently useful to build custom behaviors using the public api (along with IsItemActive, IsItemHovered, etc.)
    HvkGui_API bool          ArrowButton(const char* str_id, HvkGuiDir dir);                  // square button with an arrow shape
    HvkGui_API bool          Checkbox(const char* label, bool* v);
    HvkGui_API bool          CheckboxFlags(const char* label, int* flags, int flags_value);
    HvkGui_API bool          CheckboxFlags(const char* label, unsigned int* flags, unsigned int flags_value);
    HvkGui_API bool          RadioButton(const char* label, bool active);                    // use with e.g. if (RadioButton("one", my_value==1)) { my_value = 1; }
    HvkGui_API bool          RadioButton(const char* label, int* v, int v_button);           // shortcut to handle the above pattern when value is an integer
    HvkGui_API void          ProgressBar(float fraction, const HvkVec2& size_arg = HvkVec2(-FLT_MIN, 0), const char* overlay = NULL);
    HvkGui_API void          Bullet();                                                       // draw a small circle + keep the cursor on the same line. advance cursor x position by GetTreeNodeToLabelSpacing(), same distance that TreeNode() uses
    HvkGui_API bool          TextLink(const char* label);                                    // hyperlink text button, return true when clicked
    HvkGui_API bool          TextLinkOpenURL(const char* label, const char* url = NULL);     // hyperlink text button, automatically open file/url when clicked

    // Widgets: Hvkages
    // - Read about HvkTextureID/HvkTextureRef  here: https://github.com/ocornut/HvkGui/wiki/Hvkage-Loading-and-Displaying-Examples
    // - 'uv0' and 'uv1' are texture coordinates. Read about them from the same link above.
    // - Hvkage() pads adds style.HvkageBorderSize on each side, HvkageButton() adds style.FramePadding on each side.
    // - HvkageButton() draws a background based on regular Button() color + optionally an inner background if specified.
    // - An obsolete version of Hvkage(), before 1.91.9 (March 2025), had a 'tint_col' parameter which is now supported by the HvkageWithBg() function.
    HvkGui_API void          Hvkage(HvkTextureRef tex_ref, const HvkVec2& Hvkage_size, const HvkVec2& uv0 = HvkVec2(0, 0), const HvkVec2& uv1 = HvkVec2(1, 1));
    HvkGui_API void          HvkageWithBg(HvkTextureRef tex_ref, const HvkVec2& Hvkage_size, const HvkVec2& uv0 = HvkVec2(0, 0), const HvkVec2& uv1 = HvkVec2(1, 1), const HvkVec4& bg_col = HvkVec4(0, 0, 0, 0), const HvkVec4& tint_col = HvkVec4(1, 1, 1, 1));
    HvkGui_API bool          HvkageButton(const char* str_id, HvkTextureRef tex_ref, const HvkVec2& Hvkage_size, const HvkVec2& uv0 = HvkVec2(0, 0), const HvkVec2& uv1 = HvkVec2(1, 1), const HvkVec4& bg_col = HvkVec4(0, 0, 0, 0), const HvkVec4& tint_col = HvkVec4(1, 1, 1, 1));

    // Widgets: Combo Box (Dropdown)
    // - The BeginCombo()/EndCombo() api allows you to manage your contents and selection state however you want it, by creating e.g. Selectable() items.
    // - The old Combo() api are helpers over BeginCombo()/EndCombo() which are kept available for convenience purpose. This is analogous to how ListBox are created.
    HvkGui_API bool          BeginCombo(const char* label, const char* preview_value, HvkGuiComboFlags flags = 0);
    HvkGui_API void          EndCombo(); // only call EndCombo() if BeginCombo() returns true!
    HvkGui_API bool          Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
    HvkGui_API bool          Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);      // Separate items with \0 within a string, end item-list with \0\0. e.g. "One\0Two\0Three\0"
    HvkGui_API bool          Combo(const char* label, int* current_item, const char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int popup_max_height_in_items = -1);

    // Widgets: Drag Sliders
    // - Ctrl+Click on any drag box to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use HvkGuiSliderFlags_AlwaysClamp to always clamp.
    // - For all the Float2/Float3/Float4/Int2/Int3/Int4 versions of every function, note that a 'float v[X]' function argument is the same as 'float* v',
    //   the array syntax is just a way to document the number of elements that are expected to be accessible. You can pass address of your first element out of a contiguous set, e.g. &myvector.x
    // - Adjust format string to decorate the value with a prefix, a suffix, or adapt the editing and display precision e.g. "%.3f" -> 1.234; "%5.2f secs" -> 01.23 secs; "Biscuit: %.0f" -> Biscuit: 1; etc.
    // - Format string may also be set to NULL or use the default format ("%f" or "%d").
    // - Speed are per-pixel of mouse movement (v_speed=0.2f: mouse needs to move by 5 pixels to increase value by 1). For keyboard/gamepad navigation, minimum speed is Max(v_speed, minimum_step_at_given_precision).
    // - Use v_min < v_max to clamp edits to given limits. Note that Ctrl+Click manual input can override those limits if HvkGuiSliderFlags_AlwaysClamp is not used.
    // - Use v_max = FLT_MAX / INT_MAX etc to avoid clamping to a maximum, same with v_min = -FLT_MAX / INT_MIN to avoid clamping to a minimum.
    // - We use the same sets of flags for DragXXX() and SliderXXX() functions as the features are the same and it makes it easier to swap them.
    // - Legacy: Pre-1.78 there are DragXXX() function signatures that take a final `float power=1.0f' argument instead of the `HvkGuiSliderFlags flags=0' argument.
    //   If you get a warning converting a float to HvkGuiSliderFlags, read https://github.com/ocornut/HvkGui/issues/3361
    HvkGui_API bool          DragFloat(const char* label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", HvkGuiSliderFlags flags = 0);     // If v_min >= v_max we have no bound
    HvkGui_API bool          DragFloat2(const char* label, float v[2], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", HvkGuiSliderFlags flags = 0);
    HvkGui_API bool          DragFloat3(const char* label, float v[3], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", HvkGuiSliderFlags flags = 0);
    HvkGui_API bool          DragFloat4(const char* label, float v[4], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", HvkGuiSliderFlags flags = 0);
    HvkGui_API bool          DragFloatRange2(const char* label, float* v_current_min, float* v_current_max, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", const char* format_max = NULL, HvkGuiSliderFlags flags = 0);
    HvkGui_API bool          DragInt(const char* label, int* v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", HvkGuiSliderFlags flags = 0);  // If v_min >= v_max we have no bound
    HvkGui_API bool          DragInt2(const char* label, int v[2], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", HvkGuiSliderFlags flags = 0);
    HvkGui_API bool          DragInt3(const char* label, int v[3], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", HvkGuiSliderFlags flags = 0);
    HvkGui_API bool          DragInt4(const char* label, int v[4], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", HvkGuiSliderFlags flags = 0);
    HvkGui_API bool          DragIntRange2(const char* label, int* v_current_min, int* v_current_max, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", const char* format_max = NULL, HvkGuiSliderFlags flags = 0);
    HvkGui_API bool          DragScalar(const char* label, HvkGuiDataType data_type, void* p_data, float v_speed = 1.0f, const void* p_min = NULL, const void* p_max = NULL, const char* format = NULL, HvkGuiSliderFlags flags = 0);
    HvkGui_API bool          DragScalarN(const char* label, HvkGuiDataType data_type, void* p_data, int components, float v_speed = 1.0f, const void* p_min = NULL, const void* p_max = NULL, const char* format = NULL, HvkGuiSliderFlags flags = 0);

    // Widgets: Regular Sliders
    // - Ctrl+Click on any slider to turn them into an input box. Manually input values aren't clamped by default and can go off-bounds. Use HvkGuiSliderFlags_AlwaysClamp to always clamp.
    // - Adjust format string to decorate the value with a prefix, a suffix, or adapt the editing and display precision e.g. "%.3f" -> 1.234; "%5.2f secs" -> 01.23 secs; "Biscuit: %.0f" -> Biscuit: 1; etc.
    // - Format string may also be set to NULL or use the default format ("%f" or "%d").
    // - Legacy: Pre-1.78 there are SliderXXX() function signatures that take a final `float power=1.0f' argument instead of the `HvkGuiSliderFlags flags=0' argument.
    //   If you get a warning converting a float to HvkGuiSliderFlags, read https://github.com/ocornut/HvkGui/issues/3361
    HvkGui_API bool          SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", HvkGuiSliderFlags flags = 0);     // adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display.
    HvkGui_API bool          SliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* format = "%.3f", HvkGuiSliderFlags flags = 0);
    HvkGui_API bool          SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format = "%.3f", HvkGuiSliderFlags flags = 0);
    HvkGui_API bool          SliderFloat4(const char* label, float v[4], float v_min, float v_max, const char* format = "%.3f", HvkGuiSliderFlags flags = 0);
    HvkGui_API bool          SliderAngle(const char* label, float* v_rad, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f, const char* format = "%.0f deg", HvkGuiSliderFlags flags = 0);
    HvkGui_API bool          SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d", HvkGuiSliderFlags flags = 0);
    HvkGui_API bool          SliderInt2(const char* label, int v[2], int v_min, int v_max, const char* format = "%d", HvkGuiSliderFlags flags = 0);
    HvkGui_API bool          SliderInt3(const char* label, int v[3], int v_min, int v_max, const char* format = "%d", HvkGuiSliderFlags flags = 0);
    HvkGui_API bool          SliderInt4(const char* label, int v[4], int v_min, int v_max, const char* format = "%d", HvkGuiSliderFlags flags = 0);
    HvkGui_API bool          SliderScalar(const char* label, HvkGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, HvkGuiSliderFlags flags = 0);
    HvkGui_API bool          SliderScalarN(const char* label, HvkGuiDataType data_type, void* p_data, int components, const void* p_min, const void* p_max, const char* format = NULL, HvkGuiSliderFlags flags = 0);
    HvkGui_API bool          VSliderFloat(const char* label, const HvkVec2& size, float* v, float v_min, float v_max, const char* format = "%.3f", HvkGuiSliderFlags flags = 0);
    HvkGui_API bool          VSliderInt(const char* label, const HvkVec2& size, int* v, int v_min, int v_max, const char* format = "%d", HvkGuiSliderFlags flags = 0);
    HvkGui_API bool          VSliderScalar(const char* label, const HvkVec2& size, HvkGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, HvkGuiSliderFlags flags = 0);

    // Widgets: Input with Keyboard
    // - If you want to use InputText() with std::string or any custom dynamic string type, use the wrapper in misc/cpp/HvkGui_stdlib.h/.cpp!
    // - Most of the HvkGuiInputTextFlags flags are only useful for InputText() and not for InputFloatX, InputIntX, InputDouble etc.
    HvkGui_API bool          InputText(const char* label, char* buf, size_t buf_size, HvkGuiInputTextFlags flags = 0, HvkGuiInputTextCallback callback = NULL, void* user_data = NULL);
    HvkGui_API bool          InputTextMultiline(const char* label, char* buf, size_t buf_size, const HvkVec2& size = HvkVec2(0, 0), HvkGuiInputTextFlags flags = 0, HvkGuiInputTextCallback callback = NULL, void* user_data = NULL);
    HvkGui_API bool          InputTextWithHint(const char* label, const char* hint, char* buf, size_t buf_size, HvkGuiInputTextFlags flags = 0, HvkGuiInputTextCallback callback = NULL, void* user_data = NULL);
    HvkGui_API bool          InputFloat(const char* label, float* v, float step = 0.0f, float step_fast = 0.0f, const char* format = "%.3f", HvkGuiInputTextFlags flags = 0);
    HvkGui_API bool          InputFloat2(const char* label, float v[2], const char* format = "%.3f", HvkGuiInputTextFlags flags = 0);
    HvkGui_API bool          InputFloat3(const char* label, float v[3], const char* format = "%.3f", HvkGuiInputTextFlags flags = 0);
    HvkGui_API bool          InputFloat4(const char* label, float v[4], const char* format = "%.3f", HvkGuiInputTextFlags flags = 0);
    HvkGui_API bool          InputInt(const char* label, int* v, int step = 1, int step_fast = 100, HvkGuiInputTextFlags flags = 0);
    HvkGui_API bool          InputInt2(const char* label, int v[2], HvkGuiInputTextFlags flags = 0);
    HvkGui_API bool          InputInt3(const char* label, int v[3], HvkGuiInputTextFlags flags = 0);
    HvkGui_API bool          InputInt4(const char* label, int v[4], HvkGuiInputTextFlags flags = 0);
    HvkGui_API bool          InputDouble(const char* label, double* v, double step = 0.0, double step_fast = 0.0, const char* format = "%.6f", HvkGuiInputTextFlags flags = 0);
    HvkGui_API bool          InputScalar(const char* label, HvkGuiDataType data_type, void* p_data, const void* p_step = NULL, const void* p_step_fast = NULL, const char* format = NULL, HvkGuiInputTextFlags flags = 0);
    HvkGui_API bool          InputScalarN(const char* label, HvkGuiDataType data_type, void* p_data, int components, const void* p_step = NULL, const void* p_step_fast = NULL, const char* format = NULL, HvkGuiInputTextFlags flags = 0);

    // Widgets: Color Editor/Picker (tip: the ColorEdit* functions have a little color square that can be left-clicked to open a picker, and right-clicked to open an option menu.)
    // - Note that in C++ a 'float v[X]' function argument is the _same_ as 'float* v', the array syntax is just a way to document the number of elements that are expected to be accessible.
    // - You can pass the address of a first float element out of a contiguous structure, e.g. &myvector.x
    HvkGui_API bool          ColorEdit3(const char* label, float col[3], HvkGuiColorEditFlags flags = 0);
    HvkGui_API bool          ColorEdit4(const char* label, float col[4], HvkGuiColorEditFlags flags = 0);
    HvkGui_API bool          ColorPicker3(const char* label, float col[3], HvkGuiColorEditFlags flags = 0);
    HvkGui_API bool          ColorPicker4(const char* label, float col[4], HvkGuiColorEditFlags flags = 0, const float* ref_col = NULL);
    HvkGui_API bool          ColorButton(const char* desc_id, const HvkVec4& col, HvkGuiColorEditFlags flags = 0, const HvkVec2& size = HvkVec2(0, 0)); // display a color square/button, hover for details, return true when pressed.
    HvkGui_API void          SetColorEditOptions(HvkGuiColorEditFlags flags);                     // initialize current options (generally on application startup) if you want to select a default format, picker type, etc. User will be able to change many settings, unless you pass the _NoOptions flag to your calls.

    // Widgets: Trees
    // - TreeNode functions return true when the node is open, in which case you need to also call TreePop() when you are finished displaying the tree node contents.
    HvkGui_API bool          TreeNode(const char* label);
    HvkGui_API bool          TreeNode(const char* str_id, const char* fmt, ...) Hvk_FMTARGS(2);   // helper variation to easily decorrelate the id from the displayed string. Read the FAQ about why and how to use ID. to align arbitrary text at the same level as a TreeNode() you can use Bullet().
    HvkGui_API bool          TreeNode(const void* ptr_id, const char* fmt, ...) Hvk_FMTARGS(2);   // "
    HvkGui_API bool          TreeNodeV(const char* str_id, const char* fmt, va_list args) Hvk_FMTLIST(2);
    HvkGui_API bool          TreeNodeV(const void* ptr_id, const char* fmt, va_list args) Hvk_FMTLIST(2);
    HvkGui_API bool          TreeNodeEx(const char* label, HvkGuiTreeNodeFlags flags = 0);
    HvkGui_API bool          TreeNodeEx(const char* str_id, HvkGuiTreeNodeFlags flags, const char* fmt, ...) Hvk_FMTARGS(3);
    HvkGui_API bool          TreeNodeEx(const void* ptr_id, HvkGuiTreeNodeFlags flags, const char* fmt, ...) Hvk_FMTARGS(3);
    HvkGui_API bool          TreeNodeExV(const char* str_id, HvkGuiTreeNodeFlags flags, const char* fmt, va_list args) Hvk_FMTLIST(3);
    HvkGui_API bool          TreeNodeExV(const void* ptr_id, HvkGuiTreeNodeFlags flags, const char* fmt, va_list args) Hvk_FMTLIST(3);
    HvkGui_API void          TreePush(const char* str_id);                                       // ~ Indent()+PushID(). Already called by TreeNode() when returning true, but you can call TreePush/TreePop yourself if desired.
    HvkGui_API void          TreePush(const void* ptr_id);                                       // "
    HvkGui_API void          TreePop();                                                          // ~ Unindent()+PopID()
    HvkGui_API float         GetTreeNodeToLabelSpacing();                                        // horizontal distance preceding label when using TreeNode*() or Bullet() == (g.FontSize + style.FramePadding.x*2) for a regular unframed TreeNode
    HvkGui_API bool          CollapsingHeader(const char* label, HvkGuiTreeNodeFlags flags = 0);  // if returning 'true' the header is open. doesn't indent nor push on ID stack. user doesn't have to call TreePop().
    HvkGui_API bool          CollapsingHeader(const char* label, bool* p_visible, HvkGuiTreeNodeFlags flags = 0); // when 'p_visible != NULL': if '*p_visible==true' display an additional small close button on upper right of the header which will set the bool to false when clicked, if '*p_visible==false' don't display the header.
    HvkGui_API void          SetNextItemOpen(bool is_open, HvkGuiCond cond = 0);                  // set next TreeNode/CollapsingHeader open state.
    HvkGui_API void          SetNextItemStorageID(HvkGuiID storage_id);                           // set id to use for open/close storage (default to same as item id).

    // Widgets: Selectables
    // - A selectable highlights when hovered, and can display another color when selected.
    // - Neighbors selectable extend their highlight bounds in order to leave no gap between them. This is so a series of selected Selectable appear contiguous.
    HvkGui_API bool          Selectable(const char* label, bool selected = false, HvkGuiSelectableFlags flags = 0, const HvkVec2& size = HvkVec2(0, 0)); // "bool selected" carry the selection state (read-only). Selectable() is clicked is returns true so you can modify your selection state. size.x==0.0: use remaining width, size.x>0.0: specify width. size.y==0.0: use label height, size.y>0.0: specify height
    HvkGui_API bool          Selectable(const char* label, bool* p_selected, HvkGuiSelectableFlags flags = 0, const HvkVec2& size = HvkVec2(0, 0));      // "bool* p_selected" point to the selection state (read-write), as a convenient helper.

    // Multi-selection system for Selectable(), Checkbox(), TreeNode() functions [BETA]
    // - This enables standard multi-selection/range-selection idioms (Ctrl+Mouse/Keyboard, Shift+Mouse/Keyboard, etc.) in a way that also allow a clipper to be used.
    // - HvkGuiSelectionUserData is often used to store your item index within the current view (but may store something else).
    // - Read comments near HvkGuiMultiSelectIO for instructions/details and see 'Demo->Widgets->Selection State & Multi-Select' for demo.
    // - TreeNode() is technically supported but... using this correctly is more complicated. You need some sort of linear/random access to your tree,
    //   which is suited to advanced trees setups already implementing filters and clipper. We will work simplifying the current demo.
    // - 'selection_size' and 'items_count' parameters are optional and used by a few features. If they are costly for you to compute, you may avoid them.
    HvkGui_API HvkGuiMultiSelectIO*   BeginMultiSelect(HvkGuiMultiSelectFlags flags, int selection_size = -1, int items_count = -1);
    HvkGui_API HvkGuiMultiSelectIO*   EndMultiSelect();
    HvkGui_API void                  SetNextItemSelectionUserData(HvkGuiSelectionUserData selection_user_data);
    HvkGui_API bool                  IsItemToggledSelection();                                   // Was the last item selection state toggled? Useful if you need the per-item information _before_ reaching EndMultiSelect(). We only returns toggle _event_ in order to handle clipping correctly.

    // Widgets: List Boxes
    // - This is essentially a thin wrapper to using BeginChild/EndChild with the HvkGuiChildFlags_FrameStyle flag for stylistic changes + displaying a label.
    // - If you don't need a label you can probably simply use BeginChild() with the HvkGuiChildFlags_FrameStyle flag for the same result.
    // - You can submit contents and manage your selection state however you want it, by creating e.g. Selectable() or any other items.
    // - The simplified/old ListBox() api are helpers over BeginListBox()/EndListBox() which are kept available for convenience purpose. This is analogous to how Combos are created.
    // - Choose frame width:   size.x > 0.0f: custom  /  size.x < 0.0f or -FLT_MIN: right-align   /  size.x = 0.0f (default): use current ItemWidth
    // - Choose frame height:  size.y > 0.0f: custom  /  size.y < 0.0f or -FLT_MIN: bottom-align  /  size.y = 0.0f (default): arbitrary default height which can fit ~7 items
    HvkGui_API bool          BeginListBox(const char* label, const HvkVec2& size = HvkVec2(0, 0)); // open a framed scrolling region
    HvkGui_API void          EndListBox();                                                       // only call EndListBox() if BeginListBox() returned true!
    HvkGui_API bool          ListBox(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items = -1);
    HvkGui_API bool          ListBox(const char* label, int* current_item, const char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int height_in_items = -1);

    // Widgets: Data Plotting
    // - Consider using HvkPlot (https://github.com/epezent/Hvkplot) which is much better!
    HvkGui_API void          PlotLines(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, HvkVec2 graph_size = HvkVec2(0, 0), int stride = sizeof(float));
    HvkGui_API void          PlotLines(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, HvkVec2 graph_size = HvkVec2(0, 0));
    HvkGui_API void          PlotHistogram(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, HvkVec2 graph_size = HvkVec2(0, 0), int stride = sizeof(float));
    HvkGui_API void          PlotHistogram(const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, HvkVec2 graph_size = HvkVec2(0, 0));

    // Widgets: Value() Helpers.
    // - Those are merely shortcut to calling Text() with a format string. Output single value in "name: value" format (tip: freely declare more in your code to handle your types. you can add functions to the HvkGui namespace)
    HvkGui_API void          Value(const char* prefix, bool b);
    HvkGui_API void          Value(const char* prefix, int v);
    HvkGui_API void          Value(const char* prefix, unsigned int v);
    HvkGui_API void          Value(const char* prefix, float v, const char* float_format = NULL);

    // Widgets: Menus
    // - Use BeginMenuBar() on a window HvkGuiWindowFlags_MenuBar to append to its menu bar.
    // - Use BeginMainMenuBar() to create a menu bar at the top of the screen and append to it.
    // - Use BeginMenu() to create a menu. You can call BeginMenu() multiple time with the same identifier to append more items to it.
    // - Not that MenuItem() keyboardshortcuts are displayed as a convenience but _not processed_ by Dear HvkGui at the moment.
    HvkGui_API bool          BeginMenuBar();                                                     // append to menu-bar of current window (requires HvkGuiWindowFlags_MenuBar flag set on parent window).
    HvkGui_API void          EndMenuBar();                                                       // only call EndMenuBar() if BeginMenuBar() returns true!
    HvkGui_API bool          BeginMainMenuBar();                                                 // create and append to a full screen menu-bar.
    HvkGui_API void          EndMainMenuBar();                                                   // only call EndMainMenuBar() if BeginMainMenuBar() returns true!
    HvkGui_API bool          BeginMenu(const char* label, bool enabled = true);                  // create a sub-menu entry. only call EndMenu() if this returns true!
    HvkGui_API void          EndMenu();                                                          // only call EndMenu() if BeginMenu() returns true!
    HvkGui_API bool          MenuItem(const char* label, const char* shortcut = NULL, bool selected = false, bool enabled = true);  // return true when activated.
    HvkGui_API bool          MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled = true);              // return true when activated + toggle (*p_selected) if p_selected != NULL

    // Tooltips
    // - Tooltips are windows following the mouse. They do not take focus away.
    // - A tooltip window can contain items of any types.
    // - SetTooltip() is more or less a shortcut for the 'if (BeginTooltip()) { Text(...); EndTooltip(); }' idiom (with a subtlety that it discard any previously submitted tooltip)
    HvkGui_API bool          BeginTooltip();                                                     // begin/append a tooltip window.
    HvkGui_API void          EndTooltip();                                                       // only call EndTooltip() if BeginTooltip()/BeginItemTooltip() returns true!
    HvkGui_API void          SetTooltip(const char* fmt, ...) Hvk_FMTARGS(1);                     // set a text-only tooltip. Often used after a HvkGui::IsItemHovered() check. Override any previous call to SetTooltip().
    HvkGui_API void          SetTooltipV(const char* fmt, va_list args) Hvk_FMTLIST(1);

    // Tooltips: helpers for showing a tooltip when hovering an item
    // - BeginItemTooltip() is a shortcut for the 'if (IsItemHovered(HvkGuiHoveredFlags_ForTooltip) && BeginTooltip())' idiom.
    // - SetItemTooltip() is a shortcut for the 'if (IsItemHovered(HvkGuiHoveredFlags_ForTooltip)) { SetTooltip(...); }' idiom.
    // - Where 'HvkGuiHoveredFlags_ForTooltip' itself is a shortcut to use 'style.HoverFlagsForTooltipMouse' or 'style.HoverFlagsForTooltipNav' depending on active input type. For mouse it defaults to 'HvkGuiHoveredFlags_Stationary | HvkGuiHoveredFlags_DelayShort'.
    HvkGui_API bool          BeginItemTooltip();                                                 // begin/append a tooltip window if preceding item was hovered.
    HvkGui_API void          SetItemTooltip(const char* fmt, ...) Hvk_FMTARGS(1);                 // set a text-only tooltip if preceding item was hovered. override any previous call to SetTooltip().
    HvkGui_API void          SetItemTooltipV(const char* fmt, va_list args) Hvk_FMTLIST(1);

    // Popups, Modals
    //  - They block normal mouse hovering detection (and therefore most mouse interactions) behind them.
    //  - If not modal: they can be closed by clicking anywhere outside them, or by pressing ESCAPE.
    //  - Their visibility state (~bool) is held internally instead of being held by the programmer as we are used to with regular Begin*() calls.
    //  - The 3 properties above are related: we need to retain popup visibility state in the library because popups may be closed as any time.
    //  - You can bypass the hovering restriction by using HvkGuiHoveredFlags_AllowWhenBlockedByPopup when calling IsItemHovered() or IsWindowHovered().
    //  - HvkPORTANT: Popup identifiers are relative to the current ID stack, so OpenPopup and BeginPopup generally needs to be at the same level of the stack.
    //    This is sometimes leading to confusing mistakes. May rework this in the future.
    //  - BeginPopup(): query popup state, if open start appending into the window. Call EndPopup() afterwards if returned true. HvkGuiWindowFlags are forwarded to the window.
    //  - BeginPopupModal(): block every interaction behind the window, cannot be closed by user, add a dimming background, has a title bar.
    HvkGui_API bool          BeginPopup(const char* str_id, HvkGuiWindowFlags flags = 0);                         // return true if the popup is open, and you can start outputting to it.
    HvkGui_API bool          BeginPopupModal(const char* name, bool* p_open = NULL, HvkGuiWindowFlags flags = 0); // return true if the modal is open, and you can start outputting to it.
    HvkGui_API void          EndPopup();                                                                         // only call EndPopup() if BeginPopupXXX() returns true!

    // Popups: open/close functions
    //  - OpenPopup(): set popup state to open. HvkGuiPopupFlags are available for opening options.
    //  - If not modal: they can be closed by clicking anywhere outside them, or by pressing ESCAPE.
    //  - CloseCurrentPopup(): use inside the BeginPopup()/EndPopup() scope to close manually.
    //  - CloseCurrentPopup() is called by default by Selectable()/MenuItem() when activated (FIXME: need some options).
    //  - Use HvkGuiPopupFlags_NoOpenOverExistingPopup to avoid opening a popup if there's already one at the same level. This is equivalent to e.g. testing for !IsAnyPopupOpen() prior to OpenPopup().
    //  - Use IsWindowAppearing() after BeginPopup() to tell if a window just opened.
    //  - HvkPORTANT: Notice that for OpenPopupOnItemClick() we exceptionally default flags to 1 (== HvkGuiPopupFlags_MouseButtonRight) for backward compatibility with older API taking 'int mouse_button = 1' parameter
    HvkGui_API void          OpenPopup(const char* str_id, HvkGuiPopupFlags popup_flags = 0);                     // call to mark popup as open (don't call every frame!).
    HvkGui_API void          OpenPopup(HvkGuiID id, HvkGuiPopupFlags popup_flags = 0);                             // id overload to facilitate calling from nested stacks
    HvkGui_API void          OpenPopupOnItemClick(const char* str_id = NULL, HvkGuiPopupFlags popup_flags = 1);   // helper to open popup when clicked on last item. Default to HvkGuiPopupFlags_MouseButtonRight == 1. (note: actually triggers on the mouse _released_ event to be consistent with popup behaviors)
    HvkGui_API void          CloseCurrentPopup();                                                                // manually close the popup we have begin-ed into.

    // Popups: open+begin combined functions helpers
    //  - Helpers to do OpenPopup+BeginPopup where the Open action is triggered by e.g. hovering an item and right-clicking.
    //  - They are convenient to easily create context menus, hence the name.
    //  - HvkPORTANT: Notice that BeginPopupContextXXX takes HvkGuiPopupFlags just like OpenPopup() and unlike BeginPopup(). For full consistency, we may add HvkGuiWindowFlags to the BeginPopupContextXXX functions in the future.
    //  - HvkPORTANT: Notice that we exceptionally default their flags to 1 (== HvkGuiPopupFlags_MouseButtonRight) for backward compatibility with older API taking 'int mouse_button = 1' parameter, so if you add other flags remember to re-add the HvkGuiPopupFlags_MouseButtonRight.
    HvkGui_API bool          BeginPopupContextItem(const char* str_id = NULL, HvkGuiPopupFlags popup_flags = 1);  // open+begin popup when clicked on last item. Use str_id==NULL to associate the popup to previous item. If you want to use that on a non-interactive item such as Text() you need to pass in an explicit ID here. read comments in .cpp!
    HvkGui_API bool          BeginPopupContextWindow(const char* str_id = NULL, HvkGuiPopupFlags popup_flags = 1);// open+begin popup when clicked on current window.
    HvkGui_API bool          BeginPopupContextVoid(const char* str_id = NULL, HvkGuiPopupFlags popup_flags = 1);  // open+begin popup when clicked in void (where there are no windows).

    // Popups: query functions
    //  - IsPopupOpen(): return true if the popup is open at the current BeginPopup() level of the popup stack.
    //  - IsPopupOpen() with HvkGuiPopupFlags_AnyPopupId: return true if any popup is open at the current BeginPopup() level of the popup stack.
    //  - IsPopupOpen() with HvkGuiPopupFlags_AnyPopupId + HvkGuiPopupFlags_AnyPopupLevel: return true if any popup is open.
    HvkGui_API bool          IsPopupOpen(const char* str_id, HvkGuiPopupFlags flags = 0);                         // return true if the popup is open.

    // Tables
    // - Full-featured replacement for old Columns API.
    // - See Demo->Tables for demo code. See top of HvkGui_tables.cpp for general commentary.
    // - See HvkGuiTableFlags_ and HvkGuiTableColumnFlags_ enums for a description of available flags.
    // The typical call flow is:
    // - 1. Call BeginTable(), early out if returning false.
    // - 2. Optionally call TableSetupColumn() to submit column name/flags/defaults.
    // - 3. Optionally call TableSetupScrollFreeze() to request scroll freezing of columns/rows.
    // - 4. Optionally call TableHeadersRow() to submit a header row. Names are pulled from TableSetupColumn() data.
    // - 5. Populate contents:
    //    - In most situations you can use TableNextRow() + TableSetColumnIndex(N) to start appending into a column.
    //    - If you are using tables as a sort of grid, where every column is holding the same type of contents,
    //      you may prefer using TableNextColumn() instead of TableNextRow() + TableSetColumnIndex().
    //      TableNextColumn() will automatically wrap-around into the next row if needed.
    //    - HvkPORTANT: Comparatively to the old Columns() API, we need to call TableNextColumn() for the first column!
    //    - Summary of possible call flow:
    //        - TableNextRow() -> TableSetColumnIndex(0) -> Text("Hello 0") -> TableSetColumnIndex(1) -> Text("Hello 1")  // OK
    //        - TableNextRow() -> TableNextColumn()      -> Text("Hello 0") -> TableNextColumn()      -> Text("Hello 1")  // OK
    //        -                   TableNextColumn()      -> Text("Hello 0") -> TableNextColumn()      -> Text("Hello 1")  // OK: TableNextColumn() automatically gets to next row!
    //        - TableNextRow()                           -> Text("Hello 0")                                               // Not OK! Missing TableSetColumnIndex() or TableNextColumn()! Text will not appear!
    // - 5. Call EndTable()
    HvkGui_API bool          BeginTable(const char* str_id, int columns, HvkGuiTableFlags flags = 0, const HvkVec2& outer_size = HvkVec2(0.0f, 0.0f), float inner_width = 0.0f);
    HvkGui_API void          EndTable();                                         // only call EndTable() if BeginTable() returns true!
    HvkGui_API void          TableNextRow(HvkGuiTableRowFlags row_flags = 0, float min_row_height = 0.0f); // append into the first cell of a new row. 'min_row_height' include the minimum top and bottom padding aka CellPadding.y * 2.0f.
    HvkGui_API bool          TableNextColumn();                                  // append into the next column (or first column of next row if currently in last column). Return true when column is visible.
    HvkGui_API bool          TableSetColumnIndex(int column_n);                  // append into the specified column. Return true when column is visible.

    // Tables: Headers & Columns declaration
    // - Use TableSetupColumn() to specify label, resizing policy, default width/weight, id, various other flags etc.
    // - Use TableHeadersRow() to create a header row and automatically submit a TableHeader() for each column.
    //   Headers are required to perform: reordering, sorting, and opening the context menu.
    //   The context menu can also be made available in columns body using HvkGuiTableFlags_ContextMenuInBody.
    // - You may manually submit headers using TableNextRow() + TableHeader() calls, but this is only useful in
    //   some advanced use cases (e.g. adding custom widgets in header row).
    // - Use TableSetupScrollFreeze() to lock columns/rows so they stay visible when scrolled.
    HvkGui_API void          TableSetupColumn(const char* label, HvkGuiTableColumnFlags flags = 0, float init_width_or_weight = 0.0f, HvkGuiID user_id = 0);
    HvkGui_API void          TableSetupScrollFreeze(int cols, int rows);         // lock columns/rows so they stay visible when scrolled.
    HvkGui_API void          TableHeader(const char* label);                     // submit one header cell manually (rarely used)
    HvkGui_API void          TableHeadersRow();                                  // submit a row with headers cells based on data provided to TableSetupColumn() + submit context menu
    HvkGui_API void          TableAngledHeadersRow();                            // submit a row with angled headers for every column with the HvkGuiTableColumnFlags_AngledHeader flag. MUST BE FIRST ROW.

    // Tables: Sorting & Miscellaneous functions
    // - Sorting: call TableGetSortSpecs() to retrieve latest sort specs for the table. NULL when not sorting.
    //   When 'sort_specs->SpecsDirty == true' you should sort your data. It will be true when sorting specs have
    //   changed since last call, or the first time. Make sure to set 'SpecsDirty = false' after sorting,
    //   else you may wastefully sort your data every frame!
    // - Functions args 'int column_n' treat the default value of -1 as the same as passing the current column index.
    HvkGui_API HvkGuiTableSortSpecs*  TableGetSortSpecs();                        // get latest sort specs for the table (NULL if not sorting).  Lifetime: don't hold on this pointer over multiple frames or past any subsequent call to BeginTable().
    HvkGui_API int                   TableGetColumnCount();                      // return number of columns (value passed to BeginTable)
    HvkGui_API int                   TableGetColumnIndex();                      // return current column index.
    HvkGui_API int                   TableGetRowIndex();                         // return current row index (header rows are accounted for)
    HvkGui_API const char*           TableGetColumnName(int column_n = -1);      // return "" if column didn't have a name declared by TableSetupColumn(). Pass -1 to use current column.
    HvkGui_API HvkGuiTableColumnFlags TableGetColumnFlags(int column_n = -1);     // return column flags so you can query their Enabled/Visible/Sorted/Hovered status flags. Pass -1 to use current column.
    HvkGui_API void                  TableSetColumnEnabled(int column_n, bool v);// change user accessible enabled/disabled state of a column. Set to false to hide the column. User can use the context menu to change this themselves (right-click in headers, or right-click in columns body with HvkGuiTableFlags_ContextMenuInBody)
    HvkGui_API int                   TableGetHoveredColumn();                    // return hovered column. return -1 when table is not hovered. return columns_count if the unused space at the right of visible columns is hovered. Can also use (TableGetColumnFlags() & HvkGuiTableColumnFlags_IsHovered) instead.
    HvkGui_API void                  TableSetBgColor(HvkGuiTableBgTarget target, HvkU32 color, int column_n = -1);  // change the color of a cell, row, or column. See HvkGuiTableBgTarget_ flags for details.

    // Legacy Columns API (prefer using Tables!)
    // - You can also use SameLine(pos_x) to mimic simplified columns.
    HvkGui_API void          Columns(int count = 1, const char* id = NULL, bool borders = true);
    HvkGui_API void          NextColumn();                                                       // next column, defaults to current row or next row if the current row is finished
    HvkGui_API int           GetColumnIndex();                                                   // get current column index
    HvkGui_API float         GetColumnWidth(int column_index = -1);                              // get column width (in pixels). pass -1 to use current column
    HvkGui_API void          SetColumnWidth(int column_index, float width);                      // set column width (in pixels). pass -1 to use current column
    HvkGui_API float         GetColumnOffset(int column_index = -1);                             // get position of column line (in pixels, from the left side of the contents region). pass -1 to use current column, otherwise 0..GetColumnsCount() inclusive. column 0 is typically 0.0f
    HvkGui_API void          SetColumnOffset(int column_index, float offset_x);                  // set position of column line (in pixels, from the left side of the contents region). pass -1 to use current column
    HvkGui_API int           GetColumnsCount();

    // Tab Bars, Tabs
    // - Note: Tabs are automatically created by the docking system (when in 'docking' branch). Use this to create tab bars/tabs yourself.
    HvkGui_API bool          BeginTabBar(const char* str_id, HvkGuiTabBarFlags flags = 0);        // create and append into a TabBar
    HvkGui_API void          EndTabBar();                                                        // only call EndTabBar() if BeginTabBar() returns true!
    HvkGui_API bool          BeginTabItem(const char* label, bool* p_open = NULL, HvkGuiTabItemFlags flags = 0); // create a Tab. Returns true if the Tab is selected.
    HvkGui_API void          EndTabItem();                                                       // only call EndTabItem() if BeginTabItem() returns true!
    HvkGui_API bool          TabItemButton(const char* label, HvkGuiTabItemFlags flags = 0);      // create a Tab behaving like a button. return true when clicked. cannot be selected in the tab bar.
    HvkGui_API void          SetTabItemClosed(const char* tab_or_docked_window_label);           // notify TabBar or Docking system of a closed tab/window ahead (useful to reduce visual flicker on reorderable tab bars). For tab-bar: call after BeginTabBar() and before Tab submissions. Otherwise call with a window name.

    // Logging/Capture
    // - All text output from the interface can be captured into tty/file/clipboard. By default, tree nodes are automatically opened during logging.
    HvkGui_API void          LogToTTY(int auto_open_depth = -1);                                 // start logging to tty (stdout)
    HvkGui_API void          LogToFile(int auto_open_depth = -1, const char* filename = NULL);   // start logging to file
    HvkGui_API void          LogToClipboard(int auto_open_depth = -1);                           // start logging to OS clipboard
    HvkGui_API void          LogFinish();                                                        // stop logging (close file, etc.)
    HvkGui_API void          LogButtons();                                                       // helper to display buttons for logging to tty/file/clipboard
    HvkGui_API void          LogText(const char* fmt, ...) Hvk_FMTARGS(1);                        // pass text data straight to log (without being displayed)
    HvkGui_API void          LogTextV(const char* fmt, va_list args) Hvk_FMTLIST(1);

    // Drag and Drop
    // - On source items, call BeginDragDropSource(), if it returns true also call SetDragDropPayload() + EndDragDropSource().
    // - On target candidates, call BeginDragDropTarget(), if it returns true also call AcceptDragDropPayload() + EndDragDropTarget().
    // - If you stop calling BeginDragDropSource() the payload is preserved however it won't have a preview tooltip (we currently display a fallback "..." tooltip, see #1725)
    // - An item can be both drag source and drop target.
    HvkGui_API bool          BeginDragDropSource(HvkGuiDragDropFlags flags = 0);                                      // call after submitting an item which may be dragged. when this return true, you can call SetDragDropPayload() + EndDragDropSource()
    HvkGui_API bool          SetDragDropPayload(const char* type, const void* data, size_t sz, HvkGuiCond cond = 0);  // type is a user defined string of maximum 32 characters. Strings starting with '_' are reserved for dear HvkGui internal types. Data is copied and held by HvkGui. Return true when payload has been accepted.
    HvkGui_API void          EndDragDropSource();                                                                    // only call EndDragDropSource() if BeginDragDropSource() returns true!
    HvkGui_API bool                  BeginDragDropTarget();                                                          // call after submitting an item that may receive a payload. If this returns true, you can call AcceptDragDropPayload() + EndDragDropTarget()
    HvkGui_API const HvkGuiPayload*   AcceptDragDropPayload(const char* type, HvkGuiDragDropFlags flags = 0);          // accept contents of a given type. If HvkGuiDragDropFlags_AcceptBeforeDelivery is set you can peek into the payload before the mouse button is released.
    HvkGui_API void                  EndDragDropTarget();                                                            // only call EndDragDropTarget() if BeginDragDropTarget() returns true!
    HvkGui_API const HvkGuiPayload*   GetDragDropPayload();                                                           // peek directly into the current payload from anywhere. returns NULL when drag and drop is finished or inactive. use HvkGuiPayload::IsDataType() to test for the payload type.

    // Disabling [BETA API]
    // - Disable all user interactions and dim items visuals (applying style.DisabledAlpha over current colors)
    // - Those can be nested but it cannot be used to enable an already disabled section (a single BeginDisabled(true) in the stack is enough to keep everything disabled)
    // - Tooltips windows are automatically opted out of disabling. Note that IsItemHovered() by default returns false on disabled items, unless using HvkGuiHoveredFlags_AllowWhenDisabled. 
    // - BeginDisabled(false)/EndDisabled() essentially does nothing but is provided to facilitate use of boolean expressions (as a micro-optimization: if you have tens of thousands of BeginDisabled(false)/EndDisabled() pairs, you might want to reformulate your code to avoid making those calls)
    HvkGui_API void          BeginDisabled(bool disabled = true);
    HvkGui_API void          EndDisabled();

    // Clipping
    // - Mouse hovering is affected by HvkGui::PushClipRect() calls, unlike direct calls to HvkDrawList::PushClipRect() which are render only.
    HvkGui_API void          PushClipRect(const HvkVec2& clip_rect_min, const HvkVec2& clip_rect_max, bool intersect_with_current_clip_rect);
    HvkGui_API void          PopClipRect();

    // Focus, Activation
    HvkGui_API void          SetItemDefaultFocus();                                              // make last item the default focused item of a newly appearing window.
    HvkGui_API void          SetKeyboardFocusHere(int offset = 0);                               // focus keyboard on the next widget. Use positive 'offset' to access sub components of a multiple component widget. Use -1 to access previous widget.

    // Keyboard/Gamepad Navigation
    HvkGui_API void          SetNavCursorVisible(bool visible);                                  // alter visibility of keyboard/gamepad cursor. by default: show when using an arrow key, hide when clicking with mouse.

    // Overlapping mode
    HvkGui_API void          SetNextItemAllowOverlap();                                          // allow next item to be overlapped by a subsequent item. Useful with invisible buttons, selectable, treenode covering an area where subsequent items may need to be added. Note that both Selectable() and TreeNode() have dedicated flags doing this.

    // Item/Widgets Utilities and Query Functions
    // - Most of the functions are referring to the previous Item that has been submitted.
    // - See Demo Window under "Widgets->Querying Status" for an interactive visualization of most of those functions.
    HvkGui_API bool          IsItemHovered(HvkGuiHoveredFlags flags = 0);                         // is the last item hovered? (and usable, aka not blocked by a popup, etc.). See HvkGuiHoveredFlags for more options.
    HvkGui_API bool          IsItemActive();                                                     // is the last item active? (e.g. button being held, text field being edited. This will continuously return true while holding mouse button on an item. Items that don't interact will always return false)
    HvkGui_API bool          IsItemFocused();                                                    // is the last item focused for keyboard/gamepad navigation?
    HvkGui_API bool          IsItemClicked(HvkGuiMouseButton mouse_button = 0);                   // is the last item hovered and mouse clicked on? (**)  == IsMouseClicked(mouse_button) && IsItemHovered()Hvkportant. (**) this is NOT equivalent to the behavior of e.g. Button(). Read comments in function definition.
    HvkGui_API bool          IsItemVisible();                                                    // is the last item visible? (items may be out of sight because of clipping/scrolling)
    HvkGui_API bool          IsItemEdited();                                                     // did the last item modify its underlying value this frame? or was pressed? This is generally the same as the "bool" return value of many widgets.
    HvkGui_API bool          IsItemActivated();                                                  // was the last item just made active (item was previously inactive).
    HvkGui_API bool          IsItemDeactivated();                                                // was the last item just made inactive (item was previously active). Useful for Undo/Redo patterns with widgets that require continuous editing.
    HvkGui_API bool          IsItemDeactivatedAfterEdit();                                       // was the last item just made inactive and made a value change when it was active? (e.g. Slider/Drag moved). Useful for Undo/Redo patterns with widgets that require continuous editing. Note that you may get false positives (some widgets such as Combo()/ListBox()/Selectable() will return true even when clicking an already selected item).
    HvkGui_API bool          IsItemToggledOpen();                                                // was the last item open state toggled? set by TreeNode().
    HvkGui_API bool          IsAnyItemHovered();                                                 // is any item hovered?
    HvkGui_API bool          IsAnyItemActive();                                                  // is any item active?
    HvkGui_API bool          IsAnyItemFocused();                                                 // is any item focused?
    HvkGui_API HvkGuiID       GetItemID();                                                        // get ID of last item (~~ often same HvkGui::GetID(label) beforehand)
    HvkGui_API HvkVec2        GetItemRectMin();                                                   // get upper-left bounding rectangle of the last item (screen space)
    HvkGui_API HvkVec2        GetItemRectMax();                                                   // get lower-right bounding rectangle of the last item (screen space)
    HvkGui_API HvkVec2        GetItemRectSize();                                                  // get size of last item

    // Viewports
    // - Currently represents the Platform Window created by the application which is hosting our Dear HvkGui windows.
    // - In 'docking' branch with multi-viewport enabled, we extend this concept to have multiple active viewports.
    // - In the future we will extend this concept further to also represent Platform Monitor and support a "no main platform window" operation mode.
    HvkGui_API HvkGuiViewport* GetMainViewport();                                                 // return primary/default viewport. This can never be NULL.

    // Background/Foreground Draw Lists
    HvkGui_API HvkDrawList*   GetBackgroundDrawList();                                            // this draw list will be the first rendered one. Useful to quickly draw shapes/text behind dear HvkGui contents.
    HvkGui_API HvkDrawList*   GetForegroundDrawList();                                            // this draw list will be the last rendered one. Useful to quickly draw shapes/text over dear HvkGui contents.

    // Miscellaneous Utilities
    HvkGui_API bool          IsRectVisible(const HvkVec2& size);                                  // test if rectangle (of given size, starting from cursor position) is visible / not clipped.
    HvkGui_API bool          IsRectVisible(const HvkVec2& rect_min, const HvkVec2& rect_max);      // test if rectangle (in screen space) is visible / not clipped. to perform coarse clipping on user's side.
    HvkGui_API double        GetTime();                                                          // get global HvkGui time. incremented by io.DeltaTime every frame.
    HvkGui_API int           GetFrameCount();                                                    // get global HvkGui frame count. incremented by 1 every frame.
    HvkGui_API HvkDrawListSharedData* GetDrawListSharedData();                                    // you may use this when creating your own HvkDrawList instances.
    HvkGui_API const char*   GetStyleColorName(HvkGuiCol idx);                                    // get a string corresponding to the enum value (for display, saving, etc.).
    HvkGui_API void          SetStateStorage(HvkGuiStorage* storage);                             // replace current window storage with our own (if you want to manipulate it yourself, typically clear subsection of it)
    HvkGui_API HvkGuiStorage* GetStateStorage();

    // Text Utilities
    HvkGui_API HvkVec2        CalcTextSize(const char* text, const char* text_end = NULL, bool hide_text_after_double_hash = false, float wrap_width = -1.0f);

    // Color Utilities
    HvkGui_API HvkVec4        ColorConvertU32ToFloat4(HvkU32 in);
    HvkGui_API HvkU32         ColorConvertFloat4ToU32(const HvkVec4& in);
    HvkGui_API void          ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v);
    HvkGui_API void          ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b);

    // Inputs Utilities: Keyboard/Mouse/Gamepad
    // - the HvkGuiKey enum contains all possible keyboard, mouse and gamepad inputs (e.g. HvkGuiKey_A, HvkGuiKey_MouseLeft, HvkGuiKey_GamepadDpadUp...).
    // - (legacy: before v1.87, we used HvkGuiKey to carry native/user indices as defined by each backends. This was obsoleted in 1.87 (2022-02) and completely removed in 1.91.5 (2024-11). See https://github.com/ocornut/HvkGui/issues/4921)
    // - (legacy: any use of HvkGuiKey will assert when key < 512 to detect passing legacy native/user indices)
    HvkGui_API bool          IsKeyDown(HvkGuiKey key);                                            // is key being held.
    HvkGui_API bool          IsKeyPressed(HvkGuiKey key, bool repeat = true);                     // was key pressed (went from !Down to Down)? if repeat=true, uses io.KeyRepeatDelay / KeyRepeatRate
    HvkGui_API bool          IsKeyReleased(HvkGuiKey key);                                        // was key released (went from Down to !Down)?
    HvkGui_API bool          IsKeyChordPressed(HvkGuiKeyChord key_chord);                         // was key chord (mods + key) pressed, e.g. you can pass 'HvkGuiMod_Ctrl | HvkGuiKey_S' as a key-chord. This doesn't do any routing or focus check, please consider using Shortcut() function instead.
    HvkGui_API int           GetKeyPressedAmount(HvkGuiKey key, float repeat_delay, float rate);  // uses provided repeat rate/delay. return a count, most often 0 or 1 but might be >1 if RepeatRate is small enough that DeltaTime > RepeatRate
    HvkGui_API const char*   GetKeyName(HvkGuiKey key);                                           // [DEBUG] returns English name of the key. Those names are provided for debugging purpose and are not meant to be saved persistently nor compared.
    HvkGui_API void          SetNextFrameWantCaptureKeyboard(bool want_capture_keyboard);        // Override io.WantCaptureKeyboard flag next frame (said flag is left for your application to handle, typically when true it instructs your app to ignore inputs). e.g. force capture keyboard when your widget is being hovered. This is equivalent to setting "io.WantCaptureKeyboard = want_capture_keyboard"; after the next NewFrame() call.

    // Inputs Utilities: Shortcut Testing & Routing [BETA]
    // - HvkGuiKeyChord = a HvkGuiKey + optional HvkGuiMod_Alt/HvkGuiMod_Ctrl/HvkGuiMod_Shift/HvkGuiMod_Super.
    //       HvkGuiKey_C                          // Accepted by functions taking HvkGuiKey or HvkGuiKeyChord arguments
    //       HvkGuiMod_Ctrl | HvkGuiKey_C          // Accepted by functions taking HvkGuiKeyChord arguments
    //   only HvkGuiMod_XXX values are legal to combine with an HvkGuiKey. You CANNOT combine two HvkGuiKey values.
    // - The general idea is that several callers may register interest in a shortcut, and only one owner gets it.
    //      Parent   -> call Shortcut(Ctrl+S)    // When Parent is focused, Parent gets the shortcut.
    //        Child1 -> call Shortcut(Ctrl+S)    // When Child1 is focused, Child1 gets the shortcut (Child1 overrides Parent shortcuts)
    //        Child2 -> no call                  // When Child2 is focused, Parent gets the shortcut.
    //   The whole system is order independent, so if Child1 makes its calls before Parent, results will be identical.
    //   This is an Hvkportant property as it facilitate working with foreign code or larger codebase.
    // - To understand the difference:
    //   - IsKeyChordPressed() compares mods and call IsKeyPressed() -> function has no side-effect.
    //   - Shortcut() submits a route, routes are resolved, if it currently can be routed it calls IsKeyChordPressed() -> function has (desirable) side-effects as it can prevents another call from getting the route.
    // - Visualize registered routes in 'Metrics/Debugger->Inputs'.
    HvkGui_API bool          Shortcut(HvkGuiKeyChord key_chord, HvkGuiInputFlags flags = 0);
    HvkGui_API void          SetNextItemShortcut(HvkGuiKeyChord key_chord, HvkGuiInputFlags flags = 0);

    // Inputs Utilities: Key/Input Ownership [BETA]
    // - One common use case would be to allow your items to disable standard inputs behaviors such
    //   as Tab or Alt key handling, Mouse Wheel scrolling, etc.
    //   e.g. Button(...); SetItemKeyOwner(HvkGuiKey_MouseWheelY); to make hovering/activating a button disable wheel for scrolling.
    // - Reminder HvkGuiKey enum include access to mouse buttons and gamepad, so key ownership can apply to them.
    // - Many related features are still in HvkGui_internal.h. For instance, most IsKeyXXX()/IsMouseXXX() functions have an owner-id-aware version.
    HvkGui_API void          SetItemKeyOwner(HvkGuiKey key);                                      // Set key owner to last item ID if it is hovered or active. Equivalent to 'if (IsItemHovered() || IsItemActive()) { SetKeyOwner(key, GetItemID());'.

    // Inputs Utilities: Mouse
    // - To refer to a mouse button, you may use named enums in your code e.g. HvkGuiMouseButton_Left, HvkGuiMouseButton_Right.
    // - You can also use regular integer: it is forever guaranteed that 0=Left, 1=Right, 2=Middle.
    // - Dragging operations are only reported after mouse has moved a certain distance away from the initial clicking position (see 'lock_threshold' and 'io.MouseDraggingThreshold')
    HvkGui_API bool          IsMouseDown(HvkGuiMouseButton button);                               // is mouse button held?
    HvkGui_API bool          IsMouseClicked(HvkGuiMouseButton button, bool repeat = false);       // did mouse button clicked? (went from !Down to Down). Same as GetMouseClickedCount() == 1.
    HvkGui_API bool          IsMouseReleased(HvkGuiMouseButton button);                           // did mouse button released? (went from Down to !Down)
    HvkGui_API bool          IsMouseDoubleClicked(HvkGuiMouseButton button);                      // did mouse button double-clicked? Same as GetMouseClickedCount() == 2. (note that a double-click will also report IsMouseClicked() == true)
    HvkGui_API bool          IsMouseReleasedWithDelay(HvkGuiMouseButton button, float delay);     // delayed mouse release (use very sparingly!). Generally used with 'delay >= io.MouseDoubleClickTime' + combined with a 'io.MouseClickedLastCount==1' test. This is a very rarely used UI idiom, but some apps use this: e.g. MS Explorer single click on an icon to rename.
    HvkGui_API int           GetMouseClickedCount(HvkGuiMouseButton button);                      // return the number of successive mouse-clicks at the time where a click happen (otherwise 0).
    HvkGui_API bool          IsMouseHoveringRect(const HvkVec2& r_min, const HvkVec2& r_max, bool clip = true);// is mouse hovering given bounding rect (in screen space). clipped by current clipping settings, but disregarding of other consideration of focus/window ordering/popup-block.
    HvkGui_API bool          IsMousePosValid(const HvkVec2* mouse_pos = NULL);                    // by convention we use (-FLT_MAX,-FLT_MAX) to denote that there is no mouse available
    HvkGui_API bool          IsAnyMouseDown();                                                   // [WILL OBSOLETE] is any mouse button held? This was designed for backends, but prefer having backend maintain a mask of held mouse buttons, because upcoming input queue system will make this invalid.
    HvkGui_API HvkVec2        GetMousePos();                                                      // shortcut to HvkGui::GetIO().MousePos provided by user, to be consistent with other calls
    HvkGui_API HvkVec2        GetMousePosOnOpeningCurrentPopup();                                 // retrieve mouse position at the time of opening popup we have BeginPopup() into (helper to avoid user backing that value themselves)
    HvkGui_API bool          IsMouseDragging(HvkGuiMouseButton button, float lock_threshold = -1.0f);         // is mouse dragging? (uses io.MouseDraggingThreshold if lock_threshold < 0.0f)
    HvkGui_API HvkVec2        GetMouseDragDelta(HvkGuiMouseButton button = 0, float lock_threshold = -1.0f);   // return the delta from the initial clicking position while the mouse button is pressed or was just released. This is locked and return 0.0f until the mouse moves past a distance threshold at least once (uses io.MouseDraggingThreshold if lock_threshold < 0.0f)
    HvkGui_API void          ResetMouseDragDelta(HvkGuiMouseButton button = 0);                   //
    HvkGui_API HvkGuiMouseCursor GetMouseCursor();                                                // get desired mouse cursor shape. Hvkportant: reset in HvkGui::NewFrame(), this is updated during the frame. valid before Render(). If you use software rendering by setting io.MouseDrawCursor HvkGui will render those for you
    HvkGui_API void          SetMouseCursor(HvkGuiMouseCursor cursor_type);                       // set desired mouse cursor shape
    HvkGui_API void          SetNextFrameWantCaptureMouse(bool want_capture_mouse);              // Override io.WantCaptureMouse flag next frame (said flag is left for your application to handle, typical when true it instructs your app to ignore inputs). This is equivalent to setting "io.WantCaptureMouse = want_capture_mouse;" after the next NewFrame() call.

    // Clipboard Utilities
    // - Also see the LogToClipboard() function to capture GUI into clipboard, or easily output text data to the clipboard.
    HvkGui_API const char*   GetClipboardText();
    HvkGui_API void          SetClipboardText(const char* text);

    // Settings/.Ini Utilities
    // - The disk functions are automatically called if io.IniFilename != NULL (default is "HvkGui.ini").
    // - Set io.IniFilename to NULL to load/save manually. Read io.WantSaveIniSettings description about handling .ini saving manually.
    // - Hvkportant: default value "HvkGui.ini" is relative to current working dir! Most apps will want to lock this to an absolute path (e.g. same path as executables).
    HvkGui_API void          LoadIniSettingsFromDisk(const char* ini_filename);                  // call after CreateContext() and before the first call to NewFrame(). NewFrame() automatically calls LoadIniSettingsFromDisk(io.IniFilename).
    HvkGui_API void          LoadIniSettingsFromMemory(const char* ini_data, size_t ini_size=0); // call after CreateContext() and before the first call to NewFrame() to provide .ini data from your own data source.
    HvkGui_API void          SaveIniSettingsToDisk(const char* ini_filename);                    // this is automatically called (if io.IniFilename is not empty) a few seconds after any modification that should be reflected in the .ini file (and also by DestroyContext).
    HvkGui_API const char*   SaveIniSettingsToMemory(size_t* out_ini_size = NULL);               // return a zero-terminated string with the .ini data which you can save by your own mean. call when io.WantSaveIniSettings is set, then save data by your own mean and clear io.WantSaveIniSettings.

    // Debug Utilities
    // - Your main debugging friend is the ShowMetricsWindow() function, which is also accessible from Demo->Tools->Metrics Debugger
    HvkGui_API void          DebugTextEncoding(const char* text);
    HvkGui_API void          DebugFlashStyleColor(HvkGuiCol idx);
    HvkGui_API void          DebugStartItemPicker();
    HvkGui_API bool          DebugCheckVersionAndDataLayout(const char* version_str, size_t sz_io, size_t sz_style, size_t sz_vec2, size_t sz_vec4, size_t sz_drawvert, size_t sz_drawidx); // This is called by HvkGui_CHECKVERSION() macro.
#ifndef HvkGui_DISABLE_DEBUG_TOOLS
    HvkGui_API void          DebugLog(const char* fmt, ...)           Hvk_FMTARGS(1); // Call via HvkGui_DEBUG_LOG() for maximum stripping in caller code!
    HvkGui_API void          DebugLogV(const char* fmt, va_list args) Hvk_FMTLIST(1);
#endif

    // Memory Allocators
    // - Those functions are not reliant on the current context.
    // - DLL users: heaps and globals are not shared across DLL boundaries! You will need to call SetCurrentContext() + SetAllocatorFunctions()
    //   for each static/DLL boundary you are calling from. Read "Context and Memory Allocators" section of HvkGui.cpp for more details.
    HvkGui_API void          SetAllocatorFunctions(HvkGuiMemAllocFunc alloc_func, HvkGuiMemFreeFunc free_func, void* user_data = NULL);
    HvkGui_API void          GetAllocatorFunctions(HvkGuiMemAllocFunc* p_alloc_func, HvkGuiMemFreeFunc* p_free_func, void** p_user_data);
    HvkGui_API void*         MemAlloc(size_t size);
    HvkGui_API void          MemFree(void* ptr);

} // namespace HvkGui

//-----------------------------------------------------------------------------
// [SECTION] Flags & Enumerations
//-----------------------------------------------------------------------------

// Flags for HvkGui::Begin()
// (Those are per-window flags. There are shared flags in HvkGuiIO: io.ConfigWindowsResizeFromEdges and io.ConfigWindowsMoveFromTitleBarOnly)
enum HvkGuiWindowFlags_
{
    HvkGuiWindowFlags_None                   = 0,
    HvkGuiWindowFlags_NoTitleBar             = 1 << 0,   // Disable title-bar
    HvkGuiWindowFlags_NoResize               = 1 << 1,   // Disable user resizing with the lower-right grip
    HvkGuiWindowFlags_NoMove                 = 1 << 2,   // Disable user moving the window
    HvkGuiWindowFlags_NoScrollbar            = 1 << 3,   // Disable scrollbars (window can still scroll with mouse or programmatically)
    HvkGuiWindowFlags_NoScrollWithMouse      = 1 << 4,   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
    HvkGuiWindowFlags_NoCollapse             = 1 << 5,   // Disable user collapsing window by double-clicking on it. Also referred to as Window Menu Button (e.g. within a docking node).
    HvkGuiWindowFlags_AlwaysAutoResize       = 1 << 6,   // Resize every window to its content every frame
    HvkGuiWindowFlags_NoBackground           = 1 << 7,   // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
    HvkGuiWindowFlags_NoSavedSettings        = 1 << 8,   // Never load/save settings in .ini file
    HvkGuiWindowFlags_NoMouseInputs          = 1 << 9,   // Disable catching mouse, hovering test with pass through.
    HvkGuiWindowFlags_MenuBar                = 1 << 10,  // Has a menu-bar
    HvkGuiWindowFlags_HorizontalScrollbar    = 1 << 11,  // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(HvkVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in HvkGui_demo in the "Horizontal Scrolling" section.
    HvkGuiWindowFlags_NoFocusOnAppearing     = 1 << 12,  // Disable taking focus when transitioning from hidden to visible state
    HvkGuiWindowFlags_NoBringToFrontOnFocus  = 1 << 13,  // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
    HvkGuiWindowFlags_AlwaysVerticalScrollbar= 1 << 14,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
    HvkGuiWindowFlags_AlwaysHorizontalScrollbar=1<< 15,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
    HvkGuiWindowFlags_NoNavInputs            = 1 << 16,  // No keyboard/gamepad navigation within the window
    HvkGuiWindowFlags_NoNavFocus             = 1 << 17,  // No focusing toward this window with keyboard/gamepad navigation (e.g. skipped by Ctrl+Tab)
    HvkGuiWindowFlags_UnsavedDocument        = 1 << 18,  // Display a dot next to the title. When used in a tab/docking context, tab is selected when clicking the X + closure is not assumed (will wait for user to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
    HvkGuiWindowFlags_NoNav                  = HvkGuiWindowFlags_NoNavInputs | HvkGuiWindowFlags_NoNavFocus,
    HvkGuiWindowFlags_NoDecoration           = HvkGuiWindowFlags_NoTitleBar | HvkGuiWindowFlags_NoResize | HvkGuiWindowFlags_NoScrollbar | HvkGuiWindowFlags_NoCollapse,
    HvkGuiWindowFlags_NoInputs               = HvkGuiWindowFlags_NoMouseInputs | HvkGuiWindowFlags_NoNavInputs | HvkGuiWindowFlags_NoNavFocus,

    // [Internal]
    HvkGuiWindowFlags_ChildWindow            = 1 << 24,  // Don't use! For internal use by BeginChild()
    HvkGuiWindowFlags_Tooltip                = 1 << 25,  // Don't use! For internal use by BeginTooltip()
    HvkGuiWindowFlags_Popup                  = 1 << 26,  // Don't use! For internal use by BeginPopup()
    HvkGuiWindowFlags_Modal                  = 1 << 27,  // Don't use! For internal use by BeginPopupModal()
    HvkGuiWindowFlags_ChildMenu              = 1 << 28,  // Don't use! For internal use by BeginMenu()

    // Obsolete names
#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    //HvkGuiWindowFlags_NavFlattened           = 1 << 29,  // Obsoleted in 1.90.9: moved to HvkGuiChildFlags. BeginChild(name, size, 0, HvkGuiWindowFlags_NavFlattened)           --> BeginChild(name, size, HvkGuiChildFlags_NavFlattened, 0)
    //HvkGuiWindowFlags_AlwaysUseWindowPadding = 1 << 30,  // Obsoleted in 1.90.0: moved to HvkGuiChildFlags. BeginChild(name, size, 0, HvkGuiWindowFlags_AlwaysUseWindowPadding) --> BeginChild(name, size, HvkGuiChildFlags_AlwaysUseWindowPadding, 0)
#endif
};

// Flags for HvkGui::BeginChild()
// (Legacy: bit 0 must always correspond to HvkGuiChildFlags_Borders to be backward compatible with old API using 'bool border = false'.)
// About using AutoResizeX/AutoResizeY flags:
// - May be combined with SetNextWindowSizeConstraints() to set a min/max size for each axis (see "Demo->Child->Auto-resize with Constraints").
// - Size measurement for a given axis is only performed when the child window is within visible boundaries, or is just appearing.
//   - This allows BeginChild() to return false when not within boundaries (e.g. when scrolling), which is more optimal. BUT it won't update its auto-size while clipped.
//     While not perfect, it is a better default behavior as the always-on performance gain is more valuable than the occasional "resizing after becoming visible again" glitch.
//   - You may also use HvkGuiChildFlags_AlwaysAutoResize to force an update even when child window is not in view.
//     HOWEVER PLEASE UNDERSTAND THAT DOING SO WILL PREVENT BeginChild() FROM EVER RETURNING FALSE, disabling benefits of coarse clipping.
enum HvkGuiChildFlags_
{
    HvkGuiChildFlags_None                    = 0,
    HvkGuiChildFlags_Borders                 = 1 << 0,   // Show an outer border and enable WindowPadding. (HvkPORTANT: this is always == 1 == true for legacy reason)
    HvkGuiChildFlags_AlwaysUseWindowPadding  = 1 << 1,   // Pad with style.WindowPadding even if no border are drawn (no padding by default for non-bordered child windows because it makes more sense)
    HvkGuiChildFlags_ResizeX                 = 1 << 2,   // Allow resize from right border (layout direction). Enable .ini saving (unless HvkGuiWindowFlags_NoSavedSettings passed to window flags)
    HvkGuiChildFlags_ResizeY                 = 1 << 3,   // Allow resize from bottom border (layout direction). "
    HvkGuiChildFlags_AutoResizeX             = 1 << 4,   // Enable auto-resizing width. Read "HvkPORTANT: Size measurement" details above.
    HvkGuiChildFlags_AutoResizeY             = 1 << 5,   // Enable auto-resizing height. Read "HvkPORTANT: Size measurement" details above.
    HvkGuiChildFlags_AlwaysAutoResize        = 1 << 6,   // Combined with AutoResizeX/AutoResizeY. Always measure size even when child is hidden, always return true, always disable clipping optimization! NOT RECOMMENDED.
    HvkGuiChildFlags_FrameStyle              = 1 << 7,   // Style the child window like a framed item: use FrameBg, FrameRounding, FrameBorderSize, FramePadding instead of ChildBg, ChildRounding, ChildBorderSize, WindowPadding.
    HvkGuiChildFlags_NavFlattened            = 1 << 8,   // [BETA] Share focus scope, allow keyboard/gamepad navigation to cross over parent border to this child or between sibling child windows.

    // Obsolete names
#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    //HvkGuiChildFlags_Border                = HvkGuiChildFlags_Borders,  // Renamed in 1.91.1 (August 2024) for consistency.
#endif
};

// Flags for HvkGui::PushItemFlag()
// (Those are shared by all items)
enum HvkGuiItemFlags_
{
    HvkGuiItemFlags_None                     = 0,        // (Default)
    HvkGuiItemFlags_NoTabStop                = 1 << 0,   // false    // Disable keyboard tabbing. This is a "lighter" version of HvkGuiItemFlags_NoNav.
    HvkGuiItemFlags_NoNav                    = 1 << 1,   // false    // Disable any form of focusing (keyboard/gamepad directional navigation and SetKeyboardFocusHere() calls).
    HvkGuiItemFlags_NoNavDefaultFocus        = 1 << 2,   // false    // Disable item being a candidate for default focus (e.g. used by title bar items).
    HvkGuiItemFlags_ButtonRepeat             = 1 << 3,   // false    // Any button-like behavior will have repeat mode enabled (based on io.KeyRepeatDelay and io.KeyRepeatRate values). Note that you can also call IsItemActive() after any button to tell if it is being held.
    HvkGuiItemFlags_AutoClosePopups          = 1 << 4,   // true     // MenuItem()/Selectable() automatically close their parent popup window.
    HvkGuiItemFlags_AllowDuplicateId         = 1 << 5,   // false    // Allow submitting an item with the same identifier as an item already submitted this frame without triggering a warning tooltip if io.ConfigDebugHighlightIdConflicts is set.
};

// Flags for HvkGui::InputText()
// (Those are per-item flags. There are shared flags in HvkGuiIO: io.ConfigInputTextCursorBlink and io.ConfigInputTextEnterKeepActive)
enum HvkGuiInputTextFlags_
{
    // Basic filters (also see HvkGuiInputTextFlags_CallbackCharFilter)
    HvkGuiInputTextFlags_None                = 0,
    HvkGuiInputTextFlags_CharsDecimal        = 1 << 0,   // Allow 0123456789.+-*/
    HvkGuiInputTextFlags_CharsHexadecimal    = 1 << 1,   // Allow 0123456789ABCDEFabcdef
    HvkGuiInputTextFlags_CharsScientific     = 1 << 2,   // Allow 0123456789.+-*/eE (Scientific notation input)
    HvkGuiInputTextFlags_CharsUppercase      = 1 << 3,   // Turn a..z into A..Z
    HvkGuiInputTextFlags_CharsNoBlank        = 1 << 4,   // Filter out spaces, tabs

    // Inputs
    HvkGuiInputTextFlags_AllowTabInput       = 1 << 5,   // Pressing TAB input a '\t' character into the text field
    HvkGuiInputTextFlags_EnterReturnsTrue    = 1 << 6,   // Return 'true' when Enter is pressed (as opposed to every time the value was modified). Consider using IsItemDeactivatedAfterEdit() instead!
    HvkGuiInputTextFlags_EscapeClearsAll     = 1 << 7,   // Escape key clears content if not empty, and deactivate otherwise (contrast to default behavior of Escape to revert)
    HvkGuiInputTextFlags_CtrlEnterForNewLine = 1 << 8,   // In multi-line mode, validate with Enter, add new line with Ctrl+Enter (default is opposite: validate with Ctrl+Enter, add line with Enter).

    // Other options
    HvkGuiInputTextFlags_ReadOnly            = 1 << 9,   // Read-only mode
    HvkGuiInputTextFlags_Password            = 1 << 10,  // Password mode, display all characters as '*', disable copy
    HvkGuiInputTextFlags_AlwaysOverwrite     = 1 << 11,  // Overwrite mode
    HvkGuiInputTextFlags_AutoSelectAll       = 1 << 12,  // Select entire text when first taking mouse focus
    HvkGuiInputTextFlags_ParseEmptyRefVal    = 1 << 13,  // InputFloat(), InputInt(), InputScalar() etc. only: parse empty string as zero value.
    HvkGuiInputTextFlags_DisplayEmptyRefVal  = 1 << 14,  // InputFloat(), InputInt(), InputScalar() etc. only: when value is zero, do not display it. Generally used with HvkGuiInputTextFlags_ParseEmptyRefVal.
    HvkGuiInputTextFlags_NoHorizontalScroll  = 1 << 15,  // Disable following the cursor horizontally
    HvkGuiInputTextFlags_NoUndoRedo          = 1 << 16,  // Disable undo/redo. Note that input text owns the text data while active, if you want to provide your own undo/redo stack you need e.g. to call ClearActiveID().

    // Elide display / Alignment
    HvkGuiInputTextFlags_ElideLeft           = 1 << 17,  // When text doesn't fit, elide left side to ensure right side stays visible. Useful for path/filenames. Single-line only!

    // Callback features
    HvkGuiInputTextFlags_CallbackCompletion  = 1 << 18,  // Callback on pressing TAB (for completion handling)
    HvkGuiInputTextFlags_CallbackHistory     = 1 << 19,  // Callback on pressing Up/Down arrows (for history handling)
    HvkGuiInputTextFlags_CallbackAlways      = 1 << 20,  // Callback on each iteration. User code may query cursor position, modify text buffer.
    HvkGuiInputTextFlags_CallbackCharFilter  = 1 << 21,  // Callback on character inputs to replace or discard them. Modify 'EventChar' to replace or discard, or return 1 in callback to discard.
    HvkGuiInputTextFlags_CallbackResize      = 1 << 22,  // Callback on buffer capacity changes request (beyond 'buf_size' parameter value), allowing the string to grow. Notify when the string wants to be resized (for string types which hold a cache of their Size). You will be provided a new BufSize in the callback and NEED to honor it. (see misc/cpp/HvkGui_stdlib.h for an example of using this)
    HvkGuiInputTextFlags_CallbackEdit        = 1 << 23,  // Callback on any edit. Note that InputText() already returns true on edit + you can always use IsItemEdited(). The callback is useful to manipulate the underlying buffer while focus is active.

    // Multi-line Word-Wrapping [BETA]
    // - Not well tested yet. Please report any incorrect cursor movement, selection behavior etc. bug to https://github.com/ocornut/HvkGui/issues/3237.
    // - Wrapping style is not ideal. Wrapping of long words/sections (e.g. words larger than total available width) may be particularly unpleasing.
    // - Wrapping width needs to always account for the possibility of a vertical scrollbar.
    // - It is much slower than regular text fields.
    //   Ballpark estimate of cost on my 2019 desktop PC: for a 100 KB text buffer: +~0.3 ms (Optimized) / +~1.0 ms (Debug build).
    //   The CPU cost is very roughly proportional to text length, so a 10 KB buffer should cost about ten times less.
    HvkGuiInputTextFlags_WordWrap            = 1 << 24,  // InputTextMultiline(): word-wrap lines that are too long.

    // Obsolete names
    //HvkGuiInputTextFlags_AlwaysInsertMode  = HvkGuiInputTextFlags_AlwaysOverwrite   // [renamed in 1.82] name was not matching behavior
};

// Flags for HvkGui::TreeNodeEx(), HvkGui::CollapsingHeader*()
enum HvkGuiTreeNodeFlags_
{
    HvkGuiTreeNodeFlags_None                 = 0,
    HvkGuiTreeNodeFlags_Selected             = 1 << 0,   // Draw as selected
    HvkGuiTreeNodeFlags_Framed               = 1 << 1,   // Draw frame with background (e.g. for CollapsingHeader)
    HvkGuiTreeNodeFlags_AllowOverlap         = 1 << 2,   // Hit testing to allow subsequent widgets to overlap this one
    HvkGuiTreeNodeFlags_NoTreePushOnOpen     = 1 << 3,   // Don't do a TreePush() when open (e.g. for CollapsingHeader) = no extra indent nor pushing on ID stack
    HvkGuiTreeNodeFlags_NoAutoOpenOnLog      = 1 << 4,   // Don't automatically and temporarily open node when Logging is active (by default logging will automatically open tree nodes)
    HvkGuiTreeNodeFlags_DefaultOpen          = 1 << 5,   // Default node to be open
    HvkGuiTreeNodeFlags_OpenOnDoubleClick    = 1 << 6,   // Open on double-click instead of simple click (default for multi-select unless any _OpenOnXXX behavior is set explicitly). Both behaviors may be combined.
    HvkGuiTreeNodeFlags_OpenOnArrow          = 1 << 7,   // Open when clicking on the arrow part (default for multi-select unless any _OpenOnXXX behavior is set explicitly). Both behaviors may be combined.
    HvkGuiTreeNodeFlags_Leaf                 = 1 << 8,   // No collapsing, no arrow (use as a convenience for leaf nodes).
    HvkGuiTreeNodeFlags_Bullet               = 1 << 9,   // Display a bullet instead of arrow. HvkPORTANT: node can still be marked open/close if you don't set the _Leaf flag!
    HvkGuiTreeNodeFlags_FramePadding         = 1 << 10,  // Use FramePadding (even for an unframed text node) to vertically align text baseline to regular widget height. Equivalent to calling AlignTextToFramePadding() before the node.
    HvkGuiTreeNodeFlags_SpanAvailWidth       = 1 << 11,  // Extend hit box to the right-most edge, even if not framed. This is not the default in order to allow adding other items on the same line without using AllowOverlap mode.
    HvkGuiTreeNodeFlags_SpanFullWidth        = 1 << 12,  // Extend hit box to the left-most and right-most edges (cover the indent area).
    HvkGuiTreeNodeFlags_SpanLabelWidth       = 1 << 13,  // Narrow hit box + narrow hovering highlight, will only cover the label text.
    HvkGuiTreeNodeFlags_SpanAllColumns       = 1 << 14,  // Frame will span all columns of its container table (label will still fit in current column)
    HvkGuiTreeNodeFlags_LabelSpanAllColumns  = 1 << 15,  // Label will span all columns of its container table
    //HvkGuiTreeNodeFlags_NoScrollOnOpen     = 1 << 16,  // FIXME: TODO: Disable automatic scroll on TreePop() if node got just open and contents is not visible
    HvkGuiTreeNodeFlags_NavLeftJumpsToParent = 1 << 17,  // Nav: left arrow moves back to parent. This is processed in TreePop() when there's an unfulfilled Left nav request remaining.
    HvkGuiTreeNodeFlags_CollapsingHeader     = HvkGuiTreeNodeFlags_Framed | HvkGuiTreeNodeFlags_NoTreePushOnOpen | HvkGuiTreeNodeFlags_NoAutoOpenOnLog,

    // [EXPERIMENTAL] Draw lines connecting TreeNode hierarchy. Discuss in GitHub issue #2920.
    // Default value is pulled from style.TreeLinesFlags. May be overridden in TreeNode calls.
    HvkGuiTreeNodeFlags_DrawLinesNone        = 1 << 18,  // No lines drawn
    HvkGuiTreeNodeFlags_DrawLinesFull        = 1 << 19,  // Horizontal lines to child nodes. Vertical line drawn down to TreePop() position: cover full contents. Faster (for large trees).
    HvkGuiTreeNodeFlags_DrawLinesToNodes     = 1 << 20,  // Horizontal lines to child nodes. Vertical line drawn down to bottom-most child node. Slower (for large trees).

#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    HvkGuiTreeNodeFlags_NavLeftJumpsBackHere = HvkGuiTreeNodeFlags_NavLeftJumpsToParent,  // Renamed in 1.92.0
    HvkGuiTreeNodeFlags_SpanTextWidth        = HvkGuiTreeNodeFlags_SpanLabelWidth,        // Renamed in 1.90.7
    //HvkGuiTreeNodeFlags_AllowItemOverlap   = HvkGuiTreeNodeFlags_AllowOverlap,          // Renamed in 1.89.7
#endif
};

// Flags for OpenPopup*(), BeginPopupContext*(), IsPopupOpen() functions.
// - To be backward compatible with older API which took an 'int mouse_button = 1' argument instead of 'HvkGuiPopupFlags flags',
//   we need to treat small flags values as a mouse button index, so we encode the mouse button in the first few bits of the flags.
//   It is therefore guaranteed to be legal to pass a mouse button index in HvkGuiPopupFlags.
// - For the same reason, we exceptionally default the HvkGuiPopupFlags argument of BeginPopupContextXXX functions to 1 instead of 0.
//   HvkPORTANT: because the default parameter is 1 (==HvkGuiPopupFlags_MouseButtonRight), if you rely on the default parameter
//   and want to use another flag, you need to pass in the HvkGuiPopupFlags_MouseButtonRight flag explicitly.
// - Multiple buttons currently cannot be combined/or-ed in those functions (we could allow it later).
enum HvkGuiPopupFlags_
{
    HvkGuiPopupFlags_None                    = 0,
    HvkGuiPopupFlags_MouseButtonLeft         = 0,        // For BeginPopupContext*(): open on Left Mouse release. Guaranteed to always be == 0 (same as HvkGuiMouseButton_Left)
    HvkGuiPopupFlags_MouseButtonRight        = 1,        // For BeginPopupContext*(): open on Right Mouse release. Guaranteed to always be == 1 (same as HvkGuiMouseButton_Right)
    HvkGuiPopupFlags_MouseButtonMiddle       = 2,        // For BeginPopupContext*(): open on Middle Mouse release. Guaranteed to always be == 2 (same as HvkGuiMouseButton_Middle)
    HvkGuiPopupFlags_MouseButtonMask_        = 0x1F,
    HvkGuiPopupFlags_MouseButtonDefault_     = 1,
    HvkGuiPopupFlags_NoReopen                = 1 << 5,   // For OpenPopup*(), BeginPopupContext*(): don't reopen same popup if already open (won't reposition, won't reinitialize navigation)
    //HvkGuiPopupFlags_NoReopenAlwaysNavInit = 1 << 6,   // For OpenPopup*(), BeginPopupContext*(): focus and initialize navigation even when not reopening.
    HvkGuiPopupFlags_NoOpenOverExistingPopup = 1 << 7,   // For OpenPopup*(), BeginPopupContext*(): don't open if there's already a popup at the same level of the popup stack
    HvkGuiPopupFlags_NoOpenOverItems         = 1 << 8,   // For BeginPopupContextWindow(): don't return true when hovering items, only when hovering empty space
    HvkGuiPopupFlags_AnyPopupId              = 1 << 10,  // For IsPopupOpen(): ignore the HvkGuiID parameter and test for any popup.
    HvkGuiPopupFlags_AnyPopupLevel           = 1 << 11,  // For IsPopupOpen(): search/test at any level of the popup stack (default test in the current level)
    HvkGuiPopupFlags_AnyPopup                = HvkGuiPopupFlags_AnyPopupId | HvkGuiPopupFlags_AnyPopupLevel,
};

// Flags for HvkGui::Selectable()
enum HvkGuiSelectableFlags_
{
    HvkGuiSelectableFlags_None               = 0,
    HvkGuiSelectableFlags_NoAutoClosePopups  = 1 << 0,   // Clicking this doesn't close parent popup window (overrides HvkGuiItemFlags_AutoClosePopups)
    HvkGuiSelectableFlags_SpanAllColumns     = 1 << 1,   // Frame will span all columns of its container table (text will still fit in current column)
    HvkGuiSelectableFlags_AllowDoubleClick   = 1 << 2,   // Generate press events on double clicks too
    HvkGuiSelectableFlags_Disabled           = 1 << 3,   // Cannot be selected, display grayed out text
    HvkGuiSelectableFlags_AllowOverlap       = 1 << 4,   // (WIP) Hit testing to allow subsequent widgets to overlap this one
    HvkGuiSelectableFlags_Highlight          = 1 << 5,   // Make the item be displayed as if it is hovered
    HvkGuiSelectableFlags_SelectOnNav        = 1 << 6,   // Auto-select when moved into, unless Ctrl is held. Automatic when in a BeginMultiSelect() block.

#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    HvkGuiSelectableFlags_DontClosePopups    = HvkGuiSelectableFlags_NoAutoClosePopups,   // Renamed in 1.91.0
    //HvkGuiSelectableFlags_AllowItemOverlap = HvkGuiSelectableFlags_AllowOverlap,        // Renamed in 1.89.7
#endif
};

// Flags for HvkGui::BeginCombo()
enum HvkGuiComboFlags_
{
    HvkGuiComboFlags_None                    = 0,
    HvkGuiComboFlags_PopupAlignLeft          = 1 << 0,   // Align the popup toward the left by default
    HvkGuiComboFlags_HeightSmall             = 1 << 1,   // Max ~4 items visible. Tip: If you want your combo popup to be a specific size you can use SetNextWindowSizeConstraints() prior to calling BeginCombo()
    HvkGuiComboFlags_HeightRegular           = 1 << 2,   // Max ~8 items visible (default)
    HvkGuiComboFlags_HeightLarge             = 1 << 3,   // Max ~20 items visible
    HvkGuiComboFlags_HeightLargest           = 1 << 4,   // As many fitting items as possible
    HvkGuiComboFlags_NoArrowButton           = 1 << 5,   // Display on the preview box without the square arrow button
    HvkGuiComboFlags_NoPreview               = 1 << 6,   // Display only a square arrow button
    HvkGuiComboFlags_WidthFitPreview         = 1 << 7,   // Width dynamically calculated from preview contents
    HvkGuiComboFlags_HeightMask_             = HvkGuiComboFlags_HeightSmall | HvkGuiComboFlags_HeightRegular | HvkGuiComboFlags_HeightLarge | HvkGuiComboFlags_HeightLargest,
};

// Flags for HvkGui::BeginTabBar()
enum HvkGuiTabBarFlags_
{
    HvkGuiTabBarFlags_None                           = 0,
    HvkGuiTabBarFlags_Reorderable                    = 1 << 0,   // Allow manually dragging tabs to re-order them + New tabs are appended at the end of list
    HvkGuiTabBarFlags_AutoSelectNewTabs              = 1 << 1,   // Automatically select new tabs when they appear
    HvkGuiTabBarFlags_TabListPopupButton             = 1 << 2,   // Disable buttons to open the tab list popup
    HvkGuiTabBarFlags_NoCloseWithMiddleMouseButton   = 1 << 3,   // Disable behavior of closing tabs (that are submitted with p_open != NULL) with middle mouse button. You may handle this behavior manually on user's side with if (IsItemHovered() && IsMouseClicked(2)) *p_open = false.
    HvkGuiTabBarFlags_NoTabListScrollingButtons      = 1 << 4,   // Disable scrolling buttons (apply when fitting policy is HvkGuiTabBarFlags_FittingPolicyScroll)
    HvkGuiTabBarFlags_NoTooltip                      = 1 << 5,   // Disable tooltips when hovering a tab
    HvkGuiTabBarFlags_DrawSelectedOverline           = 1 << 6,   // Draw selected overline markers over selected tab

    // Fitting/Resize policy
    HvkGuiTabBarFlags_FittingPolicyMixed             = 1 << 7,   // Shrink down tabs when they don't fit, until width is style.TabMinWidthShrink, then enable scrolling buttons.
    HvkGuiTabBarFlags_FittingPolicyShrink            = 1 << 8,   // Shrink down tabs when they don't fit
    HvkGuiTabBarFlags_FittingPolicyScroll            = 1 << 9,   // Enable scrolling buttons when tabs don't fit
    HvkGuiTabBarFlags_FittingPolicyMask_             = HvkGuiTabBarFlags_FittingPolicyMixed | HvkGuiTabBarFlags_FittingPolicyShrink | HvkGuiTabBarFlags_FittingPolicyScroll,
    HvkGuiTabBarFlags_FittingPolicyDefault_          = HvkGuiTabBarFlags_FittingPolicyMixed,

#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    HvkGuiTabBarFlags_FittingPolicyResizeDown        = HvkGuiTabBarFlags_FittingPolicyShrink, // Renamed in 1.92.2
#endif
};

// Flags for HvkGui::BeginTabItem()
enum HvkGuiTabItemFlags_
{
    HvkGuiTabItemFlags_None                          = 0,
    HvkGuiTabItemFlags_UnsavedDocument               = 1 << 0,   // Display a dot next to the title + set HvkGuiTabItemFlags_NoAssumedClosure.
    HvkGuiTabItemFlags_SetSelected                   = 1 << 1,   // Trigger flag to programmatically make the tab selected when calling BeginTabItem()
    HvkGuiTabItemFlags_NoCloseWithMiddleMouseButton  = 1 << 2,   // Disable behavior of closing tabs (that are submitted with p_open != NULL) with middle mouse button. You may handle this behavior manually on user's side with if (IsItemHovered() && IsMouseClicked(2)) *p_open = false.
    HvkGuiTabItemFlags_NoPushId                      = 1 << 3,   // Don't call PushID()/PopID() on BeginTabItem()/EndTabItem()
    HvkGuiTabItemFlags_NoTooltip                     = 1 << 4,   // Disable tooltip for the given tab
    HvkGuiTabItemFlags_NoReorder                     = 1 << 5,   // Disable reordering this tab or having another tab cross over this tab
    HvkGuiTabItemFlags_Leading                       = 1 << 6,   // Enforce the tab position to the left of the tab bar (after the tab list popup button)
    HvkGuiTabItemFlags_Trailing                      = 1 << 7,   // Enforce the tab position to the right of the tab bar (before the scrolling buttons)
    HvkGuiTabItemFlags_NoAssumedClosure              = 1 << 8,   // Tab is selected when trying to close + closure is not immediately assumed (will wait for user to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
};

// Flags for HvkGui::IsWindowFocused()
enum HvkGuiFocusedFlags_
{
    HvkGuiFocusedFlags_None                          = 0,
    HvkGuiFocusedFlags_ChildWindows                  = 1 << 0,   // Return true if any children of the window is focused
    HvkGuiFocusedFlags_RootWindow                    = 1 << 1,   // Test from root window (top most parent of the current hierarchy)
    HvkGuiFocusedFlags_AnyWindow                     = 1 << 2,   // Return true if any window is focused. Hvkportant: If you are trying to tell how to dispatch your low-level inputs, do NOT use this. Use 'io.WantCaptureMouse' instead! Please read the FAQ!
    HvkGuiFocusedFlags_NoPopupHierarchy              = 1 << 3,   // Do not consider popup hierarchy (do not treat popup emitter as parent of popup) (when used with _ChildWindows or _RootWindow)
    //HvkGuiFocusedFlags_DockHierarchy               = 1 << 4,   // Consider docking hierarchy (treat dockspace host as parent of docked window) (when used with _ChildWindows or _RootWindow)
    HvkGuiFocusedFlags_RootAndChildWindows           = HvkGuiFocusedFlags_RootWindow | HvkGuiFocusedFlags_ChildWindows,
};

// Flags for HvkGui::IsItemHovered(), HvkGui::IsWindowHovered()
// Note: if you are trying to check whether your mouse should be dispatched to Dear HvkGui or to your app, you should use 'io.WantCaptureMouse' instead! Please read the FAQ!
// Note: windows with the HvkGuiWindowFlags_NoInputs flag are ignored by IsWindowHovered() calls.
enum HvkGuiHoveredFlags_
{
    HvkGuiHoveredFlags_None                          = 0,        // Return true if directly over the item/window, not obstructed by another window, not obstructed by an active popup or modal blocking inputs under them.
    HvkGuiHoveredFlags_ChildWindows                  = 1 << 0,   // IsWindowHovered() only: Return true if any children of the window is hovered
    HvkGuiHoveredFlags_RootWindow                    = 1 << 1,   // IsWindowHovered() only: Test from root window (top most parent of the current hierarchy)
    HvkGuiHoveredFlags_AnyWindow                     = 1 << 2,   // IsWindowHovered() only: Return true if any window is hovered
    HvkGuiHoveredFlags_NoPopupHierarchy              = 1 << 3,   // IsWindowHovered() only: Do not consider popup hierarchy (do not treat popup emitter as parent of popup) (when used with _ChildWindows or _RootWindow)
    //HvkGuiHoveredFlags_DockHierarchy               = 1 << 4,   // IsWindowHovered() only: Consider docking hierarchy (treat dockspace host as parent of docked window) (when used with _ChildWindows or _RootWindow)
    HvkGuiHoveredFlags_AllowWhenBlockedByPopup       = 1 << 5,   // Return true even if a popup window is normally blocking access to this item/window
    //HvkGuiHoveredFlags_AllowWhenBlockedByModal     = 1 << 6,   // Return true even if a modal popup window is normally blocking access to this item/window. FIXME-TODO: Unavailable yet.
    HvkGuiHoveredFlags_AllowWhenBlockedByActiveItem  = 1 << 7,   // Return true even if an active item is blocking access to this item/window. Useful for Drag and Drop patterns.
    HvkGuiHoveredFlags_AllowWhenOverlappedByItem     = 1 << 8,   // IsItemHovered() only: Return true even if the item uses AllowOverlap mode and is overlapped by another hoverable item.
    HvkGuiHoveredFlags_AllowWhenOverlappedByWindow   = 1 << 9,   // IsItemHovered() only: Return true even if the position is obstructed or overlapped by another window.
    HvkGuiHoveredFlags_AllowWhenDisabled             = 1 << 10,  // IsItemHovered() only: Return true even if the item is disabled
    HvkGuiHoveredFlags_NoNavOverride                 = 1 << 11,  // IsItemHovered() only: Disable using keyboard/gamepad navigation state when active, always query mouse
    HvkGuiHoveredFlags_AllowWhenOverlapped           = HvkGuiHoveredFlags_AllowWhenOverlappedByItem | HvkGuiHoveredFlags_AllowWhenOverlappedByWindow,
    HvkGuiHoveredFlags_RectOnly                      = HvkGuiHoveredFlags_AllowWhenBlockedByPopup | HvkGuiHoveredFlags_AllowWhenBlockedByActiveItem | HvkGuiHoveredFlags_AllowWhenOverlapped,
    HvkGuiHoveredFlags_RootAndChildWindows           = HvkGuiHoveredFlags_RootWindow | HvkGuiHoveredFlags_ChildWindows,

    // Tooltips mode
    // - typically used in IsItemHovered() + SetTooltip() sequence.
    // - this is a shortcut to pull flags from 'style.HoverFlagsForTooltipMouse' or 'style.HoverFlagsForTooltipNav' where you can reconfigure desired behavior.
    //   e.g. 'HoverFlagsForTooltipMouse' defaults to 'HvkGuiHoveredFlags_Stationary | HvkGuiHoveredFlags_DelayShort | HvkGuiHoveredFlags_AllowWhenDisabled'.
    // - for frequently actioned or hovered items providing a tooltip, you want may to use HvkGuiHoveredFlags_ForTooltip (stationary + delay) so the tooltip doesn't show too often.
    // - for items which main purpose is to be hovered, or items with low affordance, or in less consistent apps, prefer no delay or shorter delay.
    HvkGuiHoveredFlags_ForTooltip                    = 1 << 12,  // Shortcut for standard flags when using IsItemHovered() + SetTooltip() sequence.

    // (Advanced) Mouse Hovering delays.
    // - generally you can use HvkGuiHoveredFlags_ForTooltip to use application-standardized flags.
    // - use those if you need specific overrides.
    HvkGuiHoveredFlags_Stationary                    = 1 << 13,  // Require mouse to be stationary for style.HoverStationaryDelay (~0.15 sec) _at least one time_. After this, can move on same item/window. Using the stationary test tends to reduces the need for a long delay.
    HvkGuiHoveredFlags_DelayNone                     = 1 << 14,  // IsItemHovered() only: Return true immediately (default). As this is the default you generally ignore this.
    HvkGuiHoveredFlags_DelayShort                    = 1 << 15,  // IsItemHovered() only: Return true after style.HoverDelayShort elapsed (~0.15 sec) (shared between items) + requires mouse to be stationary for style.HoverStationaryDelay (once per item).
    HvkGuiHoveredFlags_DelayNormal                   = 1 << 16,  // IsItemHovered() only: Return true after style.HoverDelayNormal elapsed (~0.40 sec) (shared between items) + requires mouse to be stationary for style.HoverStationaryDelay (once per item).
    HvkGuiHoveredFlags_NoSharedDelay                 = 1 << 17,  // IsItemHovered() only: Disable shared delay system where moving from one item to the next keeps the previous timer for a short time (standard for tooltips with long delays)
};

// Flags for HvkGui::BeginDragDropSource(), HvkGui::AcceptDragDropPayload()
enum HvkGuiDragDropFlags_
{
    HvkGuiDragDropFlags_None                         = 0,
    // BeginDragDropSource() flags
    HvkGuiDragDropFlags_SourceNoPreviewTooltip       = 1 << 0,   // Disable preview tooltip. By default, a successful call to BeginDragDropSource opens a tooltip so you can display a preview or description of the source contents. This flag disables this behavior.
    HvkGuiDragDropFlags_SourceNoDisableHover         = 1 << 1,   // By default, when dragging we clear data so that IsItemHovered() will return false, to avoid subsequent user code submitting tooltips. This flag disables this behavior so you can still call IsItemHovered() on the source item.
    HvkGuiDragDropFlags_SourceNoHoldToOpenOthers     = 1 << 2,   // Disable the behavior that allows to open tree nodes and collapsing header by holding over them while dragging a source item.
    HvkGuiDragDropFlags_SourceAllowNullID            = 1 << 3,   // Allow items such as Text(), Hvkage() that have no unique identifier to be used as drag source, by manufacturing a temporary identifier based on their window-relative position. This is extremely unusual within the dear HvkGui ecosystem and so we made it explicit.
    HvkGuiDragDropFlags_SourceExtern                 = 1 << 4,   // External source (from outside of dear HvkGui), won't attempt to read current item/window info. Will always return true. Only one Extern source can be active simultaneously.
    HvkGuiDragDropFlags_PayloadAutoExpire            = 1 << 5,   // Automatically expire the payload if the source cease to be submitted (otherwise payloads are persisting while being dragged)
    HvkGuiDragDropFlags_PayloadNoCrossContext        = 1 << 6,   // Hint to specify that the payload may not be copied outside current dear HvkGui context.
    HvkGuiDragDropFlags_PayloadNoCrossProcess        = 1 << 7,   // Hint to specify that the payload may not be copied outside current process.
    // AcceptDragDropPayload() flags
    HvkGuiDragDropFlags_AcceptBeforeDelivery         = 1 << 10,  // AcceptDragDropPayload() will returns true even before the mouse button is released. You can then call IsDelivery() to test if the payload needs to be delivered.
    HvkGuiDragDropFlags_AcceptNoDrawDefaultRect      = 1 << 11,  // Do not draw the default highlight rectangle when hovering over target.
    HvkGuiDragDropFlags_AcceptNoPreviewTooltip       = 1 << 12,  // Request hiding the BeginDragDropSource tooltip from the BeginDragDropTarget site.
    HvkGuiDragDropFlags_AcceptDrawAsHovered          = 1 << 13,  // Accepting item will render as if hovered. Useful for e.g. a Button() used as a drop target.
    HvkGuiDragDropFlags_AcceptPeekOnly               = HvkGuiDragDropFlags_AcceptBeforeDelivery | HvkGuiDragDropFlags_AcceptNoDrawDefaultRect, // For peeking ahead and inspecting the payload before delivery.

#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    HvkGuiDragDropFlags_SourceAutoExpirePayload = HvkGuiDragDropFlags_PayloadAutoExpire, // Renamed in 1.90.9
#endif
};

// Standard Drag and Drop payload types. You can define you own payload types using short strings. Types starting with '_' are defined by Dear HvkGui.
#define HvkGui_PAYLOAD_TYPE_COLOR_3F     "_COL3F"    // float[3]: Standard type for colors, without alpha. User code may use this type.
#define HvkGui_PAYLOAD_TYPE_COLOR_4F     "_COL4F"    // float[4]: Standard type for colors. User code may use this type.

// A primary data type
enum HvkGuiDataType_
{
    HvkGuiDataType_S8,       // signed char / char (with sensible compilers)
    HvkGuiDataType_U8,       // unsigned char
    HvkGuiDataType_S16,      // short
    HvkGuiDataType_U16,      // unsigned short
    HvkGuiDataType_S32,      // int
    HvkGuiDataType_U32,      // unsigned int
    HvkGuiDataType_S64,      // long long / __int64
    HvkGuiDataType_U64,      // unsigned long long / unsigned __int64
    HvkGuiDataType_Float,    // float
    HvkGuiDataType_Double,   // double
    HvkGuiDataType_Bool,     // bool (provided for user convenience, not supported by scalar widgets)
    HvkGuiDataType_String,   // char* (provided for user convenience, not supported by scalar widgets)
    HvkGuiDataType_COUNT
};

// A cardinal direction
enum HvkGuiDir : int
{
    HvkGuiDir_None    = -1,
    HvkGuiDir_Left    = 0,
    HvkGuiDir_Right   = 1,
    HvkGuiDir_Up      = 2,
    HvkGuiDir_Down    = 3,
    HvkGuiDir_COUNT
};

// A sorting direction
enum HvkGuiSortDirection : HvkU8
{
    HvkGuiSortDirection_None         = 0,
    HvkGuiSortDirection_Ascending    = 1,    // Ascending = 0->9, A->Z etc.
    HvkGuiSortDirection_Descending   = 2     // Descending = 9->0, Z->A etc.
};

// A key identifier (HvkGuiKey_XXX or HvkGuiMod_XXX value): can represent Keyboard, Mouse and Gamepad values.
// All our named keys are >= 512. Keys value 0 to 511 are left unused and were legacy native/opaque key values (< 1.87).
// Support for legacy keys was completely removed in 1.91.5.
// Read details about the 1.87+ transition : https://github.com/ocornut/HvkGui/issues/4921
// Note that "Keys" related to physical keys and are not the same concept as input "Characters", the later are submitted via io.AddInputCharacter().
// The keyboard key enum values are named after the keys on a standard US keyboard, and on other keyboard types the keys reported may not match the keycaps.
enum HvkGuiKey : int
{
    // Keyboard
    HvkGuiKey_None = 0,
    HvkGuiKey_NamedKey_BEGIN = 512,  // First valid key value (other than 0)

    HvkGuiKey_Tab = 512,             // == HvkGuiKey_NamedKey_BEGIN
    HvkGuiKey_LeftArrow,
    HvkGuiKey_RightArrow,
    HvkGuiKey_UpArrow,
    HvkGuiKey_DownArrow,
    HvkGuiKey_PageUp,
    HvkGuiKey_PageDown,
    HvkGuiKey_Home,
    HvkGuiKey_End,
    HvkGuiKey_Insert,
    HvkGuiKey_Delete,
    HvkGuiKey_Backspace,
    HvkGuiKey_Space,
    HvkGuiKey_Enter,
    HvkGuiKey_Escape,
    HvkGuiKey_LeftCtrl, HvkGuiKey_LeftShift, HvkGuiKey_LeftAlt, HvkGuiKey_LeftSuper,     // Also see HvkGuiMod_Ctrl, HvkGuiMod_Shift, HvkGuiMod_Alt, HvkGuiMod_Super below!
    HvkGuiKey_RightCtrl, HvkGuiKey_RightShift, HvkGuiKey_RightAlt, HvkGuiKey_RightSuper,
    HvkGuiKey_Menu,
    HvkGuiKey_0, HvkGuiKey_1, HvkGuiKey_2, HvkGuiKey_3, HvkGuiKey_4, HvkGuiKey_5, HvkGuiKey_6, HvkGuiKey_7, HvkGuiKey_8, HvkGuiKey_9,
    HvkGuiKey_A, HvkGuiKey_B, HvkGuiKey_C, HvkGuiKey_D, HvkGuiKey_E, HvkGuiKey_F, HvkGuiKey_G, HvkGuiKey_H, HvkGuiKey_I, HvkGuiKey_J,
    HvkGuiKey_K, HvkGuiKey_L, HvkGuiKey_M, HvkGuiKey_N, HvkGuiKey_O, HvkGuiKey_P, HvkGuiKey_Q, HvkGuiKey_R, HvkGuiKey_S, HvkGuiKey_T,
    HvkGuiKey_U, HvkGuiKey_V, HvkGuiKey_W, HvkGuiKey_X, HvkGuiKey_Y, HvkGuiKey_Z,
    HvkGuiKey_F1, HvkGuiKey_F2, HvkGuiKey_F3, HvkGuiKey_F4, HvkGuiKey_F5, HvkGuiKey_F6,
    HvkGuiKey_F7, HvkGuiKey_F8, HvkGuiKey_F9, HvkGuiKey_F10, HvkGuiKey_F11, HvkGuiKey_F12,
    HvkGuiKey_F13, HvkGuiKey_F14, HvkGuiKey_F15, HvkGuiKey_F16, HvkGuiKey_F17, HvkGuiKey_F18,
    HvkGuiKey_F19, HvkGuiKey_F20, HvkGuiKey_F21, HvkGuiKey_F22, HvkGuiKey_F23, HvkGuiKey_F24,
    HvkGuiKey_Apostrophe,        // '
    HvkGuiKey_Comma,             // ,
    HvkGuiKey_Minus,             // -
    HvkGuiKey_Period,            // .
    HvkGuiKey_Slash,             // /
    HvkGuiKey_Semicolon,         // ;
    HvkGuiKey_Equal,             // =
    HvkGuiKey_LeftBracket,       // [
    HvkGuiKey_Backslash,         // \ (this text inhibit multiline comment caused by backslash)
    HvkGuiKey_RightBracket,      // ]
    HvkGuiKey_GraveAccent,       // `
    HvkGuiKey_CapsLock,
    HvkGuiKey_ScrollLock,
    HvkGuiKey_NumLock,
    HvkGuiKey_PrintScreen,
    HvkGuiKey_Pause,
    HvkGuiKey_Keypad0, HvkGuiKey_Keypad1, HvkGuiKey_Keypad2, HvkGuiKey_Keypad3, HvkGuiKey_Keypad4,
    HvkGuiKey_Keypad5, HvkGuiKey_Keypad6, HvkGuiKey_Keypad7, HvkGuiKey_Keypad8, HvkGuiKey_Keypad9,
    HvkGuiKey_KeypadDecimal,
    HvkGuiKey_KeypadDivide,
    HvkGuiKey_KeypadMultiply,
    HvkGuiKey_KeypadSubtract,
    HvkGuiKey_KeypadAdd,
    HvkGuiKey_KeypadEnter,
    HvkGuiKey_KeypadEqual,
    HvkGuiKey_AppBack,               // Available on some keyboard/mouses. Often referred as "Browser Back"
    HvkGuiKey_AppForward,
    HvkGuiKey_Oem102,                // Non-US backslash.

    // Gamepad
    // (analog values are 0.0f to 1.0f)
    // (download controller mapping PNG/PSD at http://dearHvkGui.com/controls_sheets)
    //                              // XBOX        | SWITCH  | PLAYSTA. | -> ACTION
    HvkGuiKey_GamepadStart,          // Menu        | +       | Options  |
    HvkGuiKey_GamepadBack,           // View        | -       | Share    |
    HvkGuiKey_GamepadFaceLeft,       // X           | Y       | Square   | Tap: Toggle Menu. Hold: Windowing mode (Focus/Move/Resize windows)
    HvkGuiKey_GamepadFaceRight,      // B           | A       | Circle   | Cancel / Close / Exit
    HvkGuiKey_GamepadFaceUp,         // Y           | X       | Triangle | Text Input / On-screen Keyboard
    HvkGuiKey_GamepadFaceDown,       // A           | B       | Cross    | Activate / Open / Toggle / Tweak
    HvkGuiKey_GamepadDpadLeft,       // D-pad Left  | "       | "        | Move / Tweak / Resize Window (in Windowing mode)
    HvkGuiKey_GamepadDpadRight,      // D-pad Right | "       | "        | Move / Tweak / Resize Window (in Windowing mode)
    HvkGuiKey_GamepadDpadUp,         // D-pad Up    | "       | "        | Move / Tweak / Resize Window (in Windowing mode)
    HvkGuiKey_GamepadDpadDown,       // D-pad Down  | "       | "        | Move / Tweak / Resize Window (in Windowing mode)
    HvkGuiKey_GamepadL1,             // L Bumper    | L       | L1       | Tweak Slower / Focus Previous (in Windowing mode)
    HvkGuiKey_GamepadR1,             // R Bumper    | R       | R1       | Tweak Faster / Focus Next (in Windowing mode)
    HvkGuiKey_GamepadL2,             // L Trigger   | ZL      | L2       | [Analog]
    HvkGuiKey_GamepadR2,             // R Trigger   | ZR      | R2       | [Analog]
    HvkGuiKey_GamepadL3,             // L Stick     | L3      | L3       |
    HvkGuiKey_GamepadR3,             // R Stick     | R3      | R3       |
    HvkGuiKey_GamepadLStickLeft,     //             |         |          | [Analog] Move Window (in Windowing mode)
    HvkGuiKey_GamepadLStickRight,    //             |         |          | [Analog] Move Window (in Windowing mode)
    HvkGuiKey_GamepadLStickUp,       //             |         |          | [Analog] Move Window (in Windowing mode)
    HvkGuiKey_GamepadLStickDown,     //             |         |          | [Analog] Move Window (in Windowing mode)
    HvkGuiKey_GamepadRStickLeft,     //             |         |          | [Analog]
    HvkGuiKey_GamepadRStickRight,    //             |         |          | [Analog]
    HvkGuiKey_GamepadRStickUp,       //             |         |          | [Analog]
    HvkGuiKey_GamepadRStickDown,     //             |         |          | [Analog]

    // Aliases: Mouse Buttons (auto-submitted from AddMouseButtonEvent() calls)
    // - This is mirroring the data also written to io.MouseDown[], io.MouseWheel, in a format allowing them to be accessed via standard key API.
    HvkGuiKey_MouseLeft, HvkGuiKey_MouseRight, HvkGuiKey_MouseMiddle, HvkGuiKey_MouseX1, HvkGuiKey_MouseX2, HvkGuiKey_MouseWheelX, HvkGuiKey_MouseWheelY,

    // [Internal] Reserved for mod storage
    HvkGuiKey_ReservedForModCtrl, HvkGuiKey_ReservedForModShift, HvkGuiKey_ReservedForModAlt, HvkGuiKey_ReservedForModSuper,

    // [Internal] If you need to iterate all keys (for e.g. an input mapper) you may use HvkGuiKey_NamedKey_BEGIN..HvkGuiKey_NamedKey_END.
    HvkGuiKey_NamedKey_END,
    HvkGuiKey_NamedKey_COUNT = HvkGuiKey_NamedKey_END - HvkGuiKey_NamedKey_BEGIN,

    // Keyboard Modifiers (explicitly submitted by backend via AddKeyEvent() calls)
    // - Any functions taking a HvkGuiKeyChord parameter can binary-or those with regular keys, e.g. Shortcut(HvkGuiMod_Ctrl | HvkGuiKey_S).
    // - Those are written back into io.KeyCtrl, io.KeyShift, io.KeyAlt, io.KeySuper for convenience,
    //   but may be accessed via standard key API such as IsKeyPressed(), IsKeyReleased(), querying duration etc.
    // - Code polling every key (e.g. an interface to detect a key press for input mapping) might want to ignore those
    //   and prefer using the real keys (e.g. HvkGuiKey_LeftCtrl, HvkGuiKey_RightCtrl instead of HvkGuiMod_Ctrl).
    // - In theory the value of keyboard modifiers should be roughly equivalent to a logical or of the equivalent left/right keys.
    //   In practice: it's complicated; mods are often provided from different sources. Keyboard layout, IME, sticky keys and
    //   backends tend to interfere and break that equivalence. The safer decision is to relay that ambiguity down to the end-user...
    // - On macOS, we swap Cmd(Super) and Ctrl keys at the time of the io.AddKeyEvent() call.
    HvkGuiMod_None                   = 0,
    HvkGuiMod_Ctrl                   = 1 << 12, // Ctrl (non-macOS), Cmd (macOS)
    HvkGuiMod_Shift                  = 1 << 13, // Shift
    HvkGuiMod_Alt                    = 1 << 14, // Option/Menu
    HvkGuiMod_Super                  = 1 << 15, // Windows/Super (non-macOS), Ctrl (macOS)
    HvkGuiMod_Mask_                  = 0xF000,  // 4-bits

#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    HvkGuiKey_COUNT                  = HvkGuiKey_NamedKey_END,    // Obsoleted in 1.91.5 because it was misleading (since named keys don't start at 0 anymore)
    HvkGuiMod_Shortcut               = HvkGuiMod_Ctrl,            // Removed in 1.90.7, you can now simply use HvkGuiMod_Ctrl
    //HvkGuiKey_ModCtrl = HvkGuiMod_Ctrl, HvkGuiKey_ModShift = HvkGuiMod_Shift, HvkGuiKey_ModAlt = HvkGuiMod_Alt, HvkGuiKey_ModSuper = HvkGuiMod_Super, // Renamed in 1.89
    //HvkGuiKey_KeyPadEnter = HvkGuiKey_KeypadEnter,              // Renamed in 1.87
#endif
};

// Flags for Shortcut(), SetNextItemShortcut(),
// (and for upcoming extended versions of IsKeyPressed(), IsMouseClicked(), Shortcut(), SetKeyOwner(), SetItemKeyOwner() that are still in HvkGui_internal.h)
// Don't mistake with HvkGuiInputTextFlags! (which is for HvkGui::InputText() function)
enum HvkGuiInputFlags_
{
    HvkGuiInputFlags_None                    = 0,
    HvkGuiInputFlags_Repeat                  = 1 << 0,   // Enable repeat. Return true on successive repeats. Default for legacy IsKeyPressed(). NOT Default for legacy IsMouseClicked(). MUST BE == 1.

    // Flags for Shortcut(), SetNextItemShortcut()
    // - Routing policies: RouteGlobal+OverActive >> RouteActive or RouteFocused (if owner is active item) >> RouteGlobal+OverFocused >> RouteFocused (if in focused window stack) >> RouteGlobal.
    // - Default policy is RouteFocused. Can select only 1 policy among all available.
    HvkGuiInputFlags_RouteActive             = 1 << 10,  // Route to active item only.
    HvkGuiInputFlags_RouteFocused            = 1 << 11,  // Route to windows in the focus stack (DEFAULT). Deep-most focused window takes inputs. Active item takes inputs over deep-most focused window.
    HvkGuiInputFlags_RouteGlobal             = 1 << 12,  // Global route (unless a focused window or active item registered the route).
    HvkGuiInputFlags_RouteAlways             = 1 << 13,  // Do not register route, poll keys directly.
    // - Routing options
    HvkGuiInputFlags_RouteOverFocused        = 1 << 14,  // Option: global route: higher priority than focused route (unless active item in focused route).
    HvkGuiInputFlags_RouteOverActive         = 1 << 15,  // Option: global route: higher priority than active item. Unlikely you need to use that: will interfere with every active items, e.g. Ctrl+A registered by InputText will be overridden by this. May not be fully honored as user/internal code is likely to always assume they can access keys when active.
    HvkGuiInputFlags_RouteUnlessBgFocused    = 1 << 16,  // Option: global route: will not be applied if underlying background/void is focused (== no Dear HvkGui windows are focused). Useful for overlay applications.
    HvkGuiInputFlags_RouteFromRootWindow     = 1 << 17,  // Option: route evaluated from the point of view of root window rather than current window.

    // Flags for SetNextItemShortcut()
    HvkGuiInputFlags_Tooltip                 = 1 << 18,  // Automatically display a tooltip when hovering item [BETA] Unsure of right api (opt-in/opt-out)
};

// Configuration flags stored in io.ConfigFlags. Set by user/application.
enum HvkGuiConfigFlags_
{
    HvkGuiConfigFlags_None                   = 0,
    HvkGuiConfigFlags_NavEnableKeyboard      = 1 << 0,   // Master keyboard navigation enable flag. Enable full Tabbing + directional arrows + space/enter to activate.
    HvkGuiConfigFlags_NavEnableGamepad       = 1 << 1,   // Master gamepad navigation enable flag. Backend also needs to set HvkGuiBackendFlags_HasGamepad.
    HvkGuiConfigFlags_NoMouse                = 1 << 4,   // Instruct dear HvkGui to disable mouse inputs and interactions.
    HvkGuiConfigFlags_NoMouseCursorChange    = 1 << 5,   // Instruct backend to not alter mouse cursor shape and visibility. Use if the backend cursor changes are interfering with yours and you don't want to use SetMouseCursor() to change mouse cursor. You may want to honor requests from HvkGui by reading GetMouseCursor() yourself instead.
    HvkGuiConfigFlags_NoKeyboard             = 1 << 6,   // Instruct dear HvkGui to disable keyboard inputs and interactions. This is done by ignoring keyboard events and clearing existing states.

    // User storage (to allow your backend/engine to communicate to code that may be shared between multiple projects. Those flags are NOT used by core Dear HvkGui)
    HvkGuiConfigFlags_IsSRGB                 = 1 << 20,  // Application is SRGB-aware.
    HvkGuiConfigFlags_IsTouchScreen          = 1 << 21,  // Application is using a touch screen instead of a mouse.

#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    HvkGuiConfigFlags_NavEnableSetMousePos   = 1 << 2,   // [moved/renamed in 1.91.4] -> use bool io.ConfigNavMoveSetMousePos
    HvkGuiConfigFlags_NavNoCaptureKeyboard   = 1 << 3,   // [moved/renamed in 1.91.4] -> use bool io.ConfigNavCaptureKeyboard
#endif
};

// Backend capabilities flags stored in io.BackendFlags. Set by HvkGui_impl_xxx or custom backend.
enum HvkGuiBackendFlags_
{
    HvkGuiBackendFlags_None                  = 0,
    HvkGuiBackendFlags_HasGamepad            = 1 << 0,   // Backend Platform supports gamepad and currently has one connected.
    HvkGuiBackendFlags_HasMouseCursors       = 1 << 1,   // Backend Platform supports honoring GetMouseCursor() value to change the OS cursor shape.
    HvkGuiBackendFlags_HasSetMousePos        = 1 << 2,   // Backend Platform supports io.WantSetMousePos requests to reposition the OS mouse position (only used if io.ConfigNavMoveSetMousePos is set).
    HvkGuiBackendFlags_RendererHasVtxOffset  = 1 << 3,   // Backend Renderer supports HvkDrawCmd::VtxOffset. This enables output of large meshes (64K+ vertices) while still using 16-bit indices.
    HvkGuiBackendFlags_RendererHasTextures   = 1 << 4,   // Backend Renderer supports HvkTextureData requests to create/update/destroy textures. This enables incremental texture updates and texture reloads. See https://github.com/ocornut/HvkGui/blob/master/docs/BACKENDS.md for instructions on how to upgrade your custom backend.
};

// Enumeration for PushStyleColor() / PopStyleColor()
enum HvkGuiCol_
{
    HvkGuiCol_Text,
    HvkGuiCol_TextDisabled,
    HvkGuiCol_WindowBg,              // Background of normal windows
    HvkGuiCol_ChildBg,               // Background of child windows
    HvkGuiCol_PopupBg,               // Background of popups, menus, tooltips windows
    HvkGuiCol_Border,
    HvkGuiCol_BorderShadow,
    HvkGuiCol_FrameBg,               // Background of checkbox, radio button, plot, slider, text input
    HvkGuiCol_FrameBgHovered,
    HvkGuiCol_FrameBgActive,
    HvkGuiCol_TitleBg,               // Title bar
    HvkGuiCol_TitleBgActive,         // Title bar when focused
    HvkGuiCol_TitleBgCollapsed,      // Title bar when collapsed
    HvkGuiCol_MenuBarBg,
    HvkGuiCol_ScrollbarBg,
    HvkGuiCol_ScrollbarGrab,
    HvkGuiCol_ScrollbarGrabHovered,
    HvkGuiCol_ScrollbarGrabActive,
    HvkGuiCol_CheckMark,             // Checkbox tick and RadioButton circle
    HvkGuiCol_SliderGrab,
    HvkGuiCol_SliderGrabActive,
    HvkGuiCol_Button,
    HvkGuiCol_ButtonHovered,
    HvkGuiCol_ButtonActive,
    HvkGuiCol_Header,                // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
    HvkGuiCol_HeaderHovered,
    HvkGuiCol_HeaderActive,
    HvkGuiCol_Separator,
    HvkGuiCol_SeparatorHovered,
    HvkGuiCol_SeparatorActive,
    HvkGuiCol_ResizeGrip,            // Resize grip in lower-right and lower-left corners of windows.
    HvkGuiCol_ResizeGripHovered,
    HvkGuiCol_ResizeGripActive,
    HvkGuiCol_InputTextCursor,       // InputText cursor/caret
    HvkGuiCol_TabHovered,            // Tab background, when hovered
    HvkGuiCol_Tab,                   // Tab background, when tab-bar is focused & tab is unselected
    HvkGuiCol_TabSelected,           // Tab background, when tab-bar is focused & tab is selected
    HvkGuiCol_TabSelectedOverline,   // Tab horizontal overline, when tab-bar is focused & tab is selected
    HvkGuiCol_TabDimmed,             // Tab background, when tab-bar is unfocused & tab is unselected
    HvkGuiCol_TabDimmedSelected,     // Tab background, when tab-bar is unfocused & tab is selected
    HvkGuiCol_TabDimmedSelectedOverline,//..horizontal overline, when tab-bar is unfocused & tab is selected
    HvkGuiCol_PlotLines,
    HvkGuiCol_PlotLinesHovered,
    HvkGuiCol_PlotHistogram,
    HvkGuiCol_PlotHistogramHovered,
    HvkGuiCol_TableHeaderBg,         // Table header background
    HvkGuiCol_TableBorderStrong,     // Table outer and header borders (prefer using Alpha=1.0 here)
    HvkGuiCol_TableBorderLight,      // Table inner borders (prefer using Alpha=1.0 here)
    HvkGuiCol_TableRowBg,            // Table row background (even rows)
    HvkGuiCol_TableRowBgAlt,         // Table row background (odd rows)
    HvkGuiCol_TextLink,              // Hyperlink color
    HvkGuiCol_TextSelectedBg,        // Selected text inside an InputText
    HvkGuiCol_TreeLines,             // Tree node hierarchy outlines when using HvkGuiTreeNodeFlags_DrawLines
    HvkGuiCol_DragDropTarget,        // Rectangle border highlighting a drop target
    HvkGuiCol_DragDropTargetBg,      // Rectangle background highlighting a drop target
    HvkGuiCol_UnsavedMarker,         // Unsaved Document marker (in window title and tabs)
    HvkGuiCol_NavCursor,             // Color of keyboard/gamepad navigation cursor/rectangle, when visible
    HvkGuiCol_NavWindowingHighlight, // Highlight window when using Ctrl+Tab
    HvkGuiCol_NavWindowingDimBg,     // Darken/colorize entire screen behind the Ctrl+Tab window list, when active
    HvkGuiCol_ModalWindowDimBg,      // Darken/colorize entire screen behind a modal window, when one is active
    HvkGuiCol_COUNT,

#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    HvkGuiCol_TabActive = HvkGuiCol_TabSelected,                  // [renamed in 1.90.9]
    HvkGuiCol_TabUnfocused = HvkGuiCol_TabDimmed,                 // [renamed in 1.90.9]
    HvkGuiCol_TabUnfocusedActive = HvkGuiCol_TabDimmedSelected,   // [renamed in 1.90.9]
    HvkGuiCol_NavHighlight = HvkGuiCol_NavCursor,                 // [renamed in 1.91.4]
#endif
};

// Enumeration for PushStyleVar() / PopStyleVar() to temporarily modify the HvkGuiStyle structure.
// - The enum only refers to fields of HvkGuiStyle which makes sense to be pushed/popped inside UI code.
//   During initialization or between frames, feel free to just poke into HvkGuiStyle directly.
// - Tip: Use your programming IDE navigation facilities on the names in the _second column_ below to find the actual members and their description.
//   - In Visual Studio: Ctrl+Comma ("Edit.GoToAll") can follow symbols inside comments, whereas Ctrl+F12 ("Edit.GoToImplementation") cannot.
//   - In Visual Studio w/ Visual Assist installed: Alt+G ("VAssistX.GoToImplementation") can also follow symbols inside comments.
//   - In VS Code, CLion, etc.: Ctrl+Click can follow symbols inside comments.
// - When changing this enum, you need to update the associated internal table GStyleVarInfo[] accordingly. This is where we link enum values to members offset/type.
enum HvkGuiStyleVar_
{
    // Enum name -------------------------- // Member in HvkGuiStyle structure (see HvkGuiStyle for descriptions)
    HvkGuiStyleVar_Alpha,                    // float     Alpha
    HvkGuiStyleVar_DisabledAlpha,            // float     DisabledAlpha
    HvkGuiStyleVar_WindowPadding,            // HvkVec2    WindowPadding
    HvkGuiStyleVar_WindowRounding,           // float     WindowRounding
    HvkGuiStyleVar_WindowBorderSize,         // float     WindowBorderSize
    HvkGuiStyleVar_WindowMinSize,            // HvkVec2    WindowMinSize
    HvkGuiStyleVar_WindowTitleAlign,         // HvkVec2    WindowTitleAlign
    HvkGuiStyleVar_ChildRounding,            // float     ChildRounding
    HvkGuiStyleVar_ChildBorderSize,          // float     ChildBorderSize
    HvkGuiStyleVar_PopupRounding,            // float     PopupRounding
    HvkGuiStyleVar_PopupBorderSize,          // float     PopupBorderSize
    HvkGuiStyleVar_FramePadding,             // HvkVec2    FramePadding
    HvkGuiStyleVar_FrameRounding,            // float     FrameRounding
    HvkGuiStyleVar_FrameBorderSize,          // float     FrameBorderSize
    HvkGuiStyleVar_ItemSpacing,              // HvkVec2    ItemSpacing
    HvkGuiStyleVar_ItemInnerSpacing,         // HvkVec2    ItemInnerSpacing
    HvkGuiStyleVar_IndentSpacing,            // float     IndentSpacing
    HvkGuiStyleVar_CellPadding,              // HvkVec2    CellPadding
    HvkGuiStyleVar_ScrollbarSize,            // float     ScrollbarSize
    HvkGuiStyleVar_ScrollbarRounding,        // float     ScrollbarRounding
    HvkGuiStyleVar_ScrollbarPadding,         // float     ScrollbarPadding
    HvkGuiStyleVar_GrabMinSize,              // float     GrabMinSize
    HvkGuiStyleVar_GrabRounding,             // float     GrabRounding
    HvkGuiStyleVar_ImageBorderSize,          // float     HvkageBorderSize
    HvkGuiStyleVar_TabRounding,              // float     TabRounding
    HvkGuiStyleVar_TabBorderSize,            // float     TabBorderSize
    HvkGuiStyleVar_TabMinWidthBase,          // float     TabMinWidthBase
    HvkGuiStyleVar_TabMinWidthShrink,        // float     TabMinWidthShrink
    HvkGuiStyleVar_TabBarBorderSize,         // float     TabBarBorderSize
    HvkGuiStyleVar_TabBarOverlineSize,       // float     TabBarOverlineSize
    HvkGuiStyleVar_TableAngledHeadersAngle,  // float     TableAngledHeadersAngle
    HvkGuiStyleVar_TableAngledHeadersTextAlign,// HvkVec2  TableAngledHeadersTextAlign
    HvkGuiStyleVar_TreeLinesSize,            // float     TreeLinesSize
    HvkGuiStyleVar_TreeLinesRounding,        // float     TreeLinesRounding
    HvkGuiStyleVar_ButtonTextAlign,          // HvkVec2    ButtonTextAlign
    HvkGuiStyleVar_SelectableTextAlign,      // HvkVec2    SelectableTextAlign
    HvkGuiStyleVar_SeparatorTextBorderSize,  // float     SeparatorTextBorderSize
    HvkGuiStyleVar_SeparatorTextAlign,       // HvkVec2    SeparatorTextAlign
    HvkGuiStyleVar_SeparatorTextPadding,     // HvkVec2    SeparatorTextPadding
    HvkGuiStyleVar_COUNT
};

// Flags for InvisibleButton() [extended in HvkGui_internal.h]
enum HvkGuiButtonFlags_
{
    HvkGuiButtonFlags_None                   = 0,
    HvkGuiButtonFlags_MouseButtonLeft        = 1 << 0,   // React on left mouse button (default)
    HvkGuiButtonFlags_MouseButtonRight       = 1 << 1,   // React on right mouse button
    HvkGuiButtonFlags_MouseButtonMiddle      = 1 << 2,   // React on center mouse button
    HvkGuiButtonFlags_MouseButtonMask_       = HvkGuiButtonFlags_MouseButtonLeft | HvkGuiButtonFlags_MouseButtonRight | HvkGuiButtonFlags_MouseButtonMiddle, // [Internal]
    HvkGuiButtonFlags_EnableNav              = 1 << 3,   // InvisibleButton(): do not disable navigation/tabbing. Otherwise disabled by default.
};

// Flags for ColorEdit3() / ColorEdit4() / ColorPicker3() / ColorPicker4() / ColorButton()
enum HvkGuiColorEditFlags_
{
    HvkGuiColorEditFlags_None            = 0,
    HvkGuiColorEditFlags_NoAlpha         = 1 << 1,   //              // ColorEdit, ColorPicker, ColorButton: ignore Alpha component (will only read 3 components from the input pointer).
    HvkGuiColorEditFlags_NoPicker        = 1 << 2,   //              // ColorEdit: disable picker when clicking on color square.
    HvkGuiColorEditFlags_NoOptions       = 1 << 3,   //              // ColorEdit: disable toggling options menu when right-clicking on inputs/small preview.
    HvkGuiColorEditFlags_NoSmallPreview  = 1 << 4,   //              // ColorEdit, ColorPicker: disable color square preview next to the inputs. (e.g. to show only the inputs)
    HvkGuiColorEditFlags_NoInputs        = 1 << 5,   //              // ColorEdit, ColorPicker: disable inputs sliders/text widgets (e.g. to show only the small preview color square).
    HvkGuiColorEditFlags_NoTooltip       = 1 << 6,   //              // ColorEdit, ColorPicker, ColorButton: disable tooltip when hovering the preview.
    HvkGuiColorEditFlags_NoLabel         = 1 << 7,   //              // ColorEdit, ColorPicker: disable display of inline text label (the label is still forwarded to the tooltip and picker).
    HvkGuiColorEditFlags_NoSidePreview   = 1 << 8,   //              // ColorPicker: disable bigger color preview on right side of the picker, use small color square preview instead.
    HvkGuiColorEditFlags_NoDragDrop      = 1 << 9,   //              // ColorEdit: disable drag and drop target. ColorButton: disable drag and drop source.
    HvkGuiColorEditFlags_NoBorder        = 1 << 10,  //              // ColorButton: disable border (which is enforced by default)
    HvkGuiColorEditFlags_NoColorMarkers  = 1 << 11,  //              // ColorEdit: disable rendering R/G/B/A color marker. May also be disabled globally by setting style.ColorMarkerSize = 0.

    // Alpha preview
    // - Prior to 1.91.8 (2025/01/21): alpha was made opaque in the preview by default using old name HvkGuiColorEditFlags_AlphaPreview.
    // - We now display the preview as transparent by default. You can use HvkGuiColorEditFlags_AlphaOpaque to use old behavior.
    // - The new flags may be combined better and allow finer controls.
    HvkGuiColorEditFlags_AlphaOpaque     = 1 << 12,  //              // ColorEdit, ColorPicker, ColorButton: disable alpha in the preview,. Contrary to _NoAlpha it may still be edited when calling ColorEdit4()/ColorPicker4(). For ColorButton() this does the same as _NoAlpha.
    HvkGuiColorEditFlags_AlphaNoBg       = 1 << 13,  //              // ColorEdit, ColorPicker, ColorButton: disable rendering a checkerboard background behind transparent color.
    HvkGuiColorEditFlags_AlphaPreviewHalf= 1 << 14,  //              // ColorEdit, ColorPicker, ColorButton: display half opaque / half transparent preview.

    // User Options (right-click on widget to change some of them).
    HvkGuiColorEditFlags_AlphaBar        = 1 << 18,  //              // ColorEdit, ColorPicker: show vertical alpha bar/gradient in picker.
    HvkGuiColorEditFlags_HDR             = 1 << 19,  //              // (WIP) ColorEdit: Currently only disable 0.0f..1.0f limits in RGBA edition (note: you probably want to use HvkGuiColorEditFlags_Float flag as well).
    HvkGuiColorEditFlags_DisplayRGB      = 1 << 20,  // [Display]    // ColorEdit: override _display_ type among RGB/HSV/Hex. ColorPicker: select any combination using one or more of RGB/HSV/Hex.
    HvkGuiColorEditFlags_DisplayHSV      = 1 << 21,  // [Display]    // "
    HvkGuiColorEditFlags_DisplayHex      = 1 << 22,  // [Display]    // "
    HvkGuiColorEditFlags_Uint8           = 1 << 23,  // [DataType]   // ColorEdit, ColorPicker, ColorButton: _display_ values formatted as 0..255.
    HvkGuiColorEditFlags_Float           = 1 << 24,  // [DataType]   // ColorEdit, ColorPicker, ColorButton: _display_ values formatted as 0.0f..1.0f floats instead of 0..255 integers. No round-trip of value via integers.
    HvkGuiColorEditFlags_PickerHueBar    = 1 << 25,  // [Picker]     // ColorPicker: bar for Hue, rectangle for Sat/Value.
    HvkGuiColorEditFlags_PickerHueWheel  = 1 << 26,  // [Picker]     // ColorPicker: wheel for Hue, triangle for Sat/Value.
    HvkGuiColorEditFlags_InputRGB        = 1 << 27,  // [Input]      // ColorEdit, ColorPicker: input and output data in RGB format.
    HvkGuiColorEditFlags_InputHSV        = 1 << 28,  // [Input]      // ColorEdit, ColorPicker: input and output data in HSV format.

    // Defaults Options. You can set application defaults using SetColorEditOptions(). The intent is that you probably don't want to
    // override them in most of your calls. Let the user choose via the option menu and/or call SetColorEditOptions() once during startup.
    HvkGuiColorEditFlags_DefaultOptions_ = HvkGuiColorEditFlags_Uint8 | HvkGuiColorEditFlags_DisplayRGB | HvkGuiColorEditFlags_InputRGB | HvkGuiColorEditFlags_PickerHueBar,

    // [Internal] Masks
    HvkGuiColorEditFlags_AlphaMask_      = HvkGuiColorEditFlags_NoAlpha | HvkGuiColorEditFlags_AlphaOpaque | HvkGuiColorEditFlags_AlphaNoBg | HvkGuiColorEditFlags_AlphaPreviewHalf,
    HvkGuiColorEditFlags_DisplayMask_    = HvkGuiColorEditFlags_DisplayRGB | HvkGuiColorEditFlags_DisplayHSV | HvkGuiColorEditFlags_DisplayHex,
    HvkGuiColorEditFlags_DataTypeMask_   = HvkGuiColorEditFlags_Uint8 | HvkGuiColorEditFlags_Float,
    HvkGuiColorEditFlags_PickerMask_     = HvkGuiColorEditFlags_PickerHueWheel | HvkGuiColorEditFlags_PickerHueBar,
    HvkGuiColorEditFlags_InputMask_      = HvkGuiColorEditFlags_InputRGB | HvkGuiColorEditFlags_InputHSV,

    // Obsolete names
#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    HvkGuiColorEditFlags_AlphaPreview = 0, // Removed in 1.91.8. This is the default now. Will display a checkerboard unless HvkGuiColorEditFlags_AlphaNoBg is set.
#endif
    //HvkGuiColorEditFlags_RGB = HvkGuiColorEditFlags_DisplayRGB, HvkGuiColorEditFlags_HSV = HvkGuiColorEditFlags_DisplayHSV, HvkGuiColorEditFlags_HEX = HvkGuiColorEditFlags_DisplayHex  // [renamed in 1.69]
};

// Flags for DragFloat(), DragInt(), SliderFloat(), SliderInt() etc.
// We use the same sets of flags for DragXXX() and SliderXXX() functions as the features are the same and it makes it easier to swap them.
// (Those are per-item flags. There is shared behavior flag too: HvkGuiIO: io.ConfigDragClickToInputText)
enum HvkGuiSliderFlags_
{
    HvkGuiSliderFlags_None               = 0,
    HvkGuiSliderFlags_Logarithmic        = 1 << 5,       // Make the widget logarithmic (linear otherwise). Consider using HvkGuiSliderFlags_NoRoundToFormat with this if using a format-string with small amount of digits.
    HvkGuiSliderFlags_NoRoundToFormat    = 1 << 6,       // Disable rounding underlying value to match precision of the display format string (e.g. %.3f values are rounded to those 3 digits).
    HvkGuiSliderFlags_NoInput            = 1 << 7,       // Disable Ctrl+Click or Enter key allowing to input text directly into the widget.
    HvkGuiSliderFlags_WrapAround         = 1 << 8,       // Enable wrapping around from max to min and from min to max. Only supported by DragXXX() functions for now.
    HvkGuiSliderFlags_ClampOnInput       = 1 << 9,       // Clamp value to min/max bounds when input manually with Ctrl+Click. By default Ctrl+Click allows going out of bounds.
    HvkGuiSliderFlags_ClampZeroRange     = 1 << 10,      // Clamp even if min==max==0.0f. Otherwise due to legacy reason DragXXX functions don't clamp with those values. When your clamping limits are dynamic you almost always want to use it.
    HvkGuiSliderFlags_NoSpeedTweaks      = 1 << 11,      // Disable keyboard modifiers altering tweak speed. Useful if you want to alter tweak speed yourself based on your own logic.
    HvkGuiSliderFlags_ColorMarkers       = 1 << 12,      // DragScalarN(), SliderScalarN(): Draw R/G/B/A color markers on each component.
    HvkGuiSliderFlags_AlwaysClamp        = HvkGuiSliderFlags_ClampOnInput | HvkGuiSliderFlags_ClampZeroRange,
    HvkGuiSliderFlags_InvalidMask_       = 0x7000000F,   // [Internal] We treat using those bits as being potentially a 'float power' argument from legacy API (obsoleted 2020-08) that has got miscast to this enum, and will trigger an assert if needed.
};

// Identify a mouse button.
// Those values are guaranteed to be stable and we frequently use 0/1 directly. Named enums provided for convenience.
enum HvkGuiMouseButton_
{
    HvkGuiMouseButton_Left = 0,
    HvkGuiMouseButton_Right = 1,
    HvkGuiMouseButton_Middle = 2,
    HvkGuiMouseButton_COUNT = 5
};

// Enumeration for GetMouseCursor()
// User code may request backend to display given cursor by calling SetMouseCursor(), which is why we have some cursors that are marked unused here
enum HvkGuiMouseCursor_
{
    HvkGuiMouseCursor_None = -1,
    HvkGuiMouseCursor_Arrow = 0,
    HvkGuiMouseCursor_TextInput,         // When hovering over InputText, etc.
    HvkGuiMouseCursor_ResizeAll,         // (Unused by Dear HvkGui functions)
    HvkGuiMouseCursor_ResizeNS,          // When hovering over a horizontal border
    HvkGuiMouseCursor_ResizeEW,          // When hovering over a vertical border or a column
    HvkGuiMouseCursor_ResizeNESW,        // When hovering over the bottom-left corner of a window
    HvkGuiMouseCursor_ResizeNWSE,        // When hovering over the bottom-right corner of a window
    HvkGuiMouseCursor_Hand,              // (Unused by Dear HvkGui functions. Use for e.g. hyperlinks)
    HvkGuiMouseCursor_Wait,              // When waiting for something to process/load.
    HvkGuiMouseCursor_Progress,          // When waiting for something to process/load, but application is still interactive.
    HvkGuiMouseCursor_NotAllowed,        // When hovering something with disallowed interaction. Usually a crossed circle.
    HvkGuiMouseCursor_COUNT
};

// Enumeration for AddMouseSourceEvent() actual source of Mouse Input data.
// Historically we use "Mouse" terminology everywhere to indicate pointer data, e.g. MousePos, IsMousePressed(), io.AddMousePosEvent()
// But that "Mouse" data can come from different source which occasionally may be useful for application to know about.
// You can submit a change of pointer type using io.AddMouseSourceEvent().
enum HvkGuiMouseSource : int
{
    HvkGuiMouseSource_Mouse = 0,         // Input is coming from an actual mouse.
    HvkGuiMouseSource_TouchScreen,       // Input is coming from a touch screen (no hovering prior to initial press, less precise initial press aiming, dual-axis wheeling possible).
    HvkGuiMouseSource_Pen,               // Input is coming from a pressure/magnetic pen (often used in conjunction with high-sampling rates).
    HvkGuiMouseSource_COUNT
};

// Enumeration for HvkGui::SetNextWindow***(), SetWindow***(), SetNextItem***() functions
// Represent a condition.
// Hvkportant: Treat as a regular enum! Do NOT combine multiple values using binary operators! All the functions above treat 0 as a shortcut to HvkGuiCond_Always.
enum HvkGuiCond_
{
    HvkGuiCond_None          = 0,        // No condition (always set the variable), same as _Always
    HvkGuiCond_Always        = 1 << 0,   // No condition (always set the variable), same as _None
    HvkGuiCond_Once          = 1 << 1,   // Set the variable once per runtime session (only the first call will succeed)
    HvkGuiCond_FirstUseEver  = 1 << 2,   // Set the variable if the object/window has no persistently saved data (no entry in .ini file)
    HvkGuiCond_Appearing     = 1 << 3,   // Set the variable if the object/window is appearing after being hidden/inactive (or the first time)
};

//-----------------------------------------------------------------------------
// [SECTION] Tables API flags and structures (HvkGuiTableFlags, HvkGuiTableColumnFlags, HvkGuiTableRowFlags, HvkGuiTableBgTarget, HvkGuiTableSortSpecs, HvkGuiTableColumnSortSpecs)
//-----------------------------------------------------------------------------

// Flags for HvkGui::BeginTable()
// - Hvkportant! Sizing policies have complex and subtle side effects, much more so than you would expect.
//   Read comments/demos carefully + experiment with live demos to get acquainted with them.
// - The DEFAULT sizing policies are:
//    - Default to HvkGuiTableFlags_SizingFixedFit    if ScrollX is on, or if host window has HvkGuiWindowFlags_AlwaysAutoResize.
//    - Default to HvkGuiTableFlags_SizingStretchSame if ScrollX is off.
// - When ScrollX is off:
//    - Table defaults to HvkGuiTableFlags_SizingStretchSame -> all Columns defaults to HvkGuiTableColumnFlags_WidthStretch with same weight.
//    - Columns sizing policy allowed: Stretch (default), Fixed/Auto.
//    - Fixed Columns (if any) will generally obtain their requested width (unless the table cannot fit them all).
//    - Stretch Columns will share the remaining width according to their respective weight.
//    - Mixed Fixed/Stretch columns is possible but has various side-effects on resizing behaviors.
//      The typical use of mixing sizing policies is: any number of LEADING Fixed columns, followed by one or two TRAILING Stretch columns.
//      (this is because the visible order of columns have subtle but necessary effects on how they react to manual resizing).
// - When ScrollX is on:
//    - Table defaults to HvkGuiTableFlags_SizingFixedFit -> all Columns defaults to HvkGuiTableColumnFlags_WidthFixed
//    - Columns sizing policy allowed: Fixed/Auto mostly.
//    - Fixed Columns can be enlarged as needed. Table will show a horizontal scrollbar if needed.
//    - When using auto-resizing (non-resizable) fixed columns, querying the content width to use item right-alignment e.g. SetNextItemWidth(-FLT_MIN) doesn't make sense, would create a feedback loop.
//    - Using Stretch columns OFTEN DOES NOT MAKE SENSE if ScrollX is on, UNLESS you have specified a value for 'inner_width' in BeginTable().
//      If you specify a value for 'inner_width' then effectively the scrolling space is known and Stretch or mixed Fixed/Stretch columns become meaningful again.
// - Read on documentation at the top of HvkGui_tables.cpp for details.
enum HvkGuiTableFlags_
{
    // Features
    HvkGuiTableFlags_None                       = 0,
    HvkGuiTableFlags_Resizable                  = 1 << 0,   // Enable resizing columns.
    HvkGuiTableFlags_Reorderable                = 1 << 1,   // Enable reordering columns in header row (need calling TableSetupColumn() + TableHeadersRow() to display headers)
    HvkGuiTableFlags_Hideable                   = 1 << 2,   // Enable hiding/disabling columns in context menu.
    HvkGuiTableFlags_Sortable                   = 1 << 3,   // Enable sorting. Call TableGetSortSpecs() to obtain sort specs. Also see HvkGuiTableFlags_SortMulti and HvkGuiTableFlags_SortTristate.
    HvkGuiTableFlags_NoSavedSettings            = 1 << 4,   // Disable persisting columns order, width, visibility and sort settings in the .ini file.
    HvkGuiTableFlags_ContextMenuInBody          = 1 << 5,   // Right-click on columns body/contents will display table context menu. By default it is available in TableHeadersRow().
    // Decorations
    HvkGuiTableFlags_RowBg                      = 1 << 6,   // Set each RowBg color with HvkGuiCol_TableRowBg or HvkGuiCol_TableRowBgAlt (equivalent of calling TableSetBgColor with HvkGuiTableBgFlags_RowBg0 on each row manually)
    HvkGuiTableFlags_BordersInnerH              = 1 << 7,   // Draw horizontal borders between rows.
    HvkGuiTableFlags_BordersOuterH              = 1 << 8,   // Draw horizontal borders at the top and bottom.
    HvkGuiTableFlags_BordersInnerV              = 1 << 9,   // Draw vertical borders between columns.
    HvkGuiTableFlags_BordersOuterV              = 1 << 10,  // Draw vertical borders on the left and right sides.
    HvkGuiTableFlags_BordersH                   = HvkGuiTableFlags_BordersInnerH | HvkGuiTableFlags_BordersOuterH, // Draw horizontal borders.
    HvkGuiTableFlags_BordersV                   = HvkGuiTableFlags_BordersInnerV | HvkGuiTableFlags_BordersOuterV, // Draw vertical borders.
    HvkGuiTableFlags_BordersInner               = HvkGuiTableFlags_BordersInnerV | HvkGuiTableFlags_BordersInnerH, // Draw inner borders.
    HvkGuiTableFlags_BordersOuter               = HvkGuiTableFlags_BordersOuterV | HvkGuiTableFlags_BordersOuterH, // Draw outer borders.
    HvkGuiTableFlags_Borders                    = HvkGuiTableFlags_BordersInner | HvkGuiTableFlags_BordersOuter,   // Draw all borders.
    HvkGuiTableFlags_NoBordersInBody            = 1 << 11,  // [ALPHA] Disable vertical borders in columns Body (borders will always appear in Headers). -> May move to style
    HvkGuiTableFlags_NoBordersInBodyUntilResize = 1 << 12,  // [ALPHA] Disable vertical borders in columns Body until hovered for resize (borders will always appear in Headers). -> May move to style
    // Sizing Policy (read above for defaults)
    HvkGuiTableFlags_SizingFixedFit             = 1 << 13,  // Columns default to _WidthFixed or _WidthAuto (if resizable or not resizable), matching contents width.
    HvkGuiTableFlags_SizingFixedSame            = 2 << 13,  // Columns default to _WidthFixed or _WidthAuto (if resizable or not resizable), matching the maximum contents width of all columns. Hvkplicitly enable HvkGuiTableFlags_NoKeepColumnsVisible.
    HvkGuiTableFlags_SizingStretchProp          = 3 << 13,  // Columns default to _WidthStretch with default weights proportional to each columns contents widths.
    HvkGuiTableFlags_SizingStretchSame          = 4 << 13,  // Columns default to _WidthStretch with default weights all equal, unless overridden by TableSetupColumn().
    // Sizing Extra Options
    HvkGuiTableFlags_NoHostExtendX              = 1 << 16,  // Make outer width auto-fit to columns, overriding outer_size.x value. Only available when ScrollX/ScrollY are disabled and Stretch columns are not used.
    HvkGuiTableFlags_NoHostExtendY              = 1 << 17,  // Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit). Only available when ScrollX/ScrollY are disabled. Data below the limit will be clipped and not visible.
    HvkGuiTableFlags_NoKeepColumnsVisible       = 1 << 18,  // Disable keeping column always minimally visible when ScrollX is off and table gets too small. Not recommended if columns are resizable.
    HvkGuiTableFlags_PreciseWidths              = 1 << 19,  // Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: Without this flag: 33,33,34. With this flag: 33,33,33). With larger number of columns, resizing will appear to be less smooth.
    // Clipping
    HvkGuiTableFlags_NoClip                     = 1 << 20,  // Disable clipping rectangle for every individual columns (reduce draw command count, items will be able to overflow into other columns). Generally incompatible with TableSetupScrollFreeze().
    // Padding
    HvkGuiTableFlags_PadOuterX                  = 1 << 21,  // Default if BordersOuterV is on. Enable outermost padding. Generally desirable if you have headers.
    HvkGuiTableFlags_NoPadOuterX                = 1 << 22,  // Default if BordersOuterV is off. Disable outermost padding.
    HvkGuiTableFlags_NoPadInnerX                = 1 << 23,  // Disable inner padding between columns (double inner padding if BordersOuterV is on, single inner padding if BordersOuterV is off).
    // Scrolling
    HvkGuiTableFlags_ScrollX                    = 1 << 24,  // Enable horizontal scrolling. Require 'outer_size' parameter of BeginTable() to specify the container size. Changes default sizing policy. Because this creates a child window, ScrollY is currently generally recommended when using ScrollX.
    HvkGuiTableFlags_ScrollY                    = 1 << 25,  // Enable vertical scrolling. Require 'outer_size' parameter of BeginTable() to specify the container size.
    // Sorting
    HvkGuiTableFlags_SortMulti                  = 1 << 26,  // Hold shift when clicking headers to sort on multiple column. TableGetSortSpecs() may return specs where (SpecsCount > 1).
    HvkGuiTableFlags_SortTristate               = 1 << 27,  // Allow no sorting, disable default sorting. TableGetSortSpecs() may return specs where (SpecsCount == 0).
    // Miscellaneous
    HvkGuiTableFlags_HighlightHoveredColumn     = 1 << 28,  // Highlight column headers when hovered (may evolve into a fuller highlight)

    // [Internal] Combinations and masks
    HvkGuiTableFlags_SizingMask_                = HvkGuiTableFlags_SizingFixedFit | HvkGuiTableFlags_SizingFixedSame | HvkGuiTableFlags_SizingStretchProp | HvkGuiTableFlags_SizingStretchSame,
};

// Flags for HvkGui::TableSetupColumn()
enum HvkGuiTableColumnFlags_
{
    // Input configuration flags
    HvkGuiTableColumnFlags_None                  = 0,
    HvkGuiTableColumnFlags_Disabled              = 1 << 0,   // Overriding/master disable flag: hide column, won't show in context menu (unlike calling TableSetColumnEnabled() which manipulates the user accessible state)
    HvkGuiTableColumnFlags_DefaultHide           = 1 << 1,   // Default as a hidden/disabled column.
    HvkGuiTableColumnFlags_DefaultSort           = 1 << 2,   // Default as a sorting column.
    HvkGuiTableColumnFlags_WidthStretch          = 1 << 3,   // Column will stretch. Preferable with horizontal scrolling disabled (default if table sizing policy is _SizingStretchSame or _SizingStretchProp).
    HvkGuiTableColumnFlags_WidthFixed            = 1 << 4,   // Column will not stretch. Preferable with horizontal scrolling enabled (default if table sizing policy is _SizingFixedFit and table is resizable).
    HvkGuiTableColumnFlags_NoResize              = 1 << 5,   // Disable manual resizing.
    HvkGuiTableColumnFlags_NoReorder             = 1 << 6,   // Disable manual reordering this column, this will also prevent other columns from crossing over this column.
    HvkGuiTableColumnFlags_NoHide                = 1 << 7,   // Disable ability to hide/disable this column.
    HvkGuiTableColumnFlags_NoClip                = 1 << 8,   // Disable clipping for this column (all NoClip columns will render in a same draw command).
    HvkGuiTableColumnFlags_NoSort                = 1 << 9,   // Disable ability to sort on this field (even if HvkGuiTableFlags_Sortable is set on the table).
    HvkGuiTableColumnFlags_NoSortAscending       = 1 << 10,  // Disable ability to sort in the ascending direction.
    HvkGuiTableColumnFlags_NoSortDescending      = 1 << 11,  // Disable ability to sort in the descending direction.
    HvkGuiTableColumnFlags_NoHeaderLabel         = 1 << 12,  // TableHeadersRow() will submit an empty label for this column. Convenient for some small columns. Name will still appear in context menu or in angled headers. You may append into this cell by calling TableSetColumnIndex() right after the TableHeadersRow() call.
    HvkGuiTableColumnFlags_NoHeaderWidth         = 1 << 13,  // Disable header text width contribution to automatic column width.
    HvkGuiTableColumnFlags_PreferSortAscending   = 1 << 14,  // Make the initial sort direction Ascending when first sorting on this column (default).
    HvkGuiTableColumnFlags_PreferSortDescending  = 1 << 15,  // Make the initial sort direction Descending when first sorting on this column.
    HvkGuiTableColumnFlags_IndentEnable          = 1 << 16,  // Use current Indent value when entering cell (default for column 0).
    HvkGuiTableColumnFlags_IndentDisable         = 1 << 17,  // Ignore current Indent value when entering cell (default for columns > 0). Indentation changes _within_ the cell will still be honored.
    HvkGuiTableColumnFlags_AngledHeader          = 1 << 18,  // TableHeadersRow() will submit an angled header row for this column. Note this will add an extra row.

    // Output status flags, read-only via TableGetColumnFlags()
    HvkGuiTableColumnFlags_IsEnabled             = 1 << 24,  // Status: is enabled == not hidden by user/api (referred to as "Hide" in _DefaultHide and _NoHide) flags.
    HvkGuiTableColumnFlags_IsVisible             = 1 << 25,  // Status: is visible == is enabled AND not clipped by scrolling.
    HvkGuiTableColumnFlags_IsSorted              = 1 << 26,  // Status: is currently part of the sort specs
    HvkGuiTableColumnFlags_IsHovered             = 1 << 27,  // Status: is hovered by mouse

    // [Internal] Combinations and masks
    HvkGuiTableColumnFlags_WidthMask_            = HvkGuiTableColumnFlags_WidthStretch | HvkGuiTableColumnFlags_WidthFixed,
    HvkGuiTableColumnFlags_IndentMask_           = HvkGuiTableColumnFlags_IndentEnable | HvkGuiTableColumnFlags_IndentDisable,
    HvkGuiTableColumnFlags_StatusMask_           = HvkGuiTableColumnFlags_IsEnabled | HvkGuiTableColumnFlags_IsVisible | HvkGuiTableColumnFlags_IsSorted | HvkGuiTableColumnFlags_IsHovered,
    HvkGuiTableColumnFlags_NoDirectResize_       = 1 << 30,  // [Internal] Disable user resizing this column directly (it may however we resized indirectly from its left edge)
};

// Flags for HvkGui::TableNextRow()
enum HvkGuiTableRowFlags_
{
    HvkGuiTableRowFlags_None                     = 0,
    HvkGuiTableRowFlags_Headers                  = 1 << 0,   // Identify header row (set default background color + width of its contents accounted differently for auto column width)
};

// Enum for HvkGui::TableSetBgColor()
// Background colors are rendering in 3 layers:
//  - Layer 0: draw with RowBg0 color if set, otherwise draw with ColumnBg0 if set.
//  - Layer 1: draw with RowBg1 color if set, otherwise draw with ColumnBg1 if set.
//  - Layer 2: draw with CellBg color if set.
// The purpose of the two row/columns layers is to let you decide if a background color change should override or blend with the existing color.
// When using HvkGuiTableFlags_RowBg on the table, each row has the RowBg0 color automatically set for odd/even rows.
// If you set the color of RowBg0 target, your color will override the existing RowBg0 color.
// If you set the color of RowBg1 or ColumnBg1 target, your color will blend over the RowBg0 color.
enum HvkGuiTableBgTarget_
{
    HvkGuiTableBgTarget_None                     = 0,
    HvkGuiTableBgTarget_RowBg0                   = 1,        // Set row background color 0 (generally used for background, automatically set when HvkGuiTableFlags_RowBg is used)
    HvkGuiTableBgTarget_RowBg1                   = 2,        // Set row background color 1 (generally used for selection marking)
    HvkGuiTableBgTarget_CellBg                   = 3,        // Set cell background color (top-most color)
};

// Sorting specifications for a table (often handling sort specs for a single column, occasionally more)
// Obtained by calling TableGetSortSpecs().
// When 'SpecsDirty == true' you can sort your data. It will be true with sorting specs have changed since last call, or the first time.
// Make sure to set 'SpecsDirty = false' after sorting, else you may wastefully sort your data every frame!
struct HvkGuiTableSortSpecs
{
    const HvkGuiTableColumnSortSpecs* Specs;     // Pointer to sort spec array.
    int                         SpecsCount;     // Sort spec count. Most often 1. May be > 1 when HvkGuiTableFlags_SortMulti is enabled. May be == 0 when HvkGuiTableFlags_SortTristate is enabled.
    bool                        SpecsDirty;     // Set to true when specs have changed since last time! Use this to sort again, then clear the flag.

    HvkGuiTableSortSpecs()       { memset(this, 0, sizeof(*this)); }
};

// Sorting specification for one column of a table (sizeof == 12 bytes)
struct HvkGuiTableColumnSortSpecs
{
    HvkGuiID                     ColumnUserID;       // User id of the column (if specified by a TableSetupColumn() call)
    HvkS16                       ColumnIndex;        // Index of the column
    HvkS16                       SortOrder;          // Index within parent HvkGuiTableSortSpecs (always stored in order starting from 0, tables sorted on a single criteria will always have a 0 here)
    HvkGuiSortDirection          SortDirection;      // HvkGuiSortDirection_Ascending or HvkGuiSortDirection_Descending

    HvkGuiTableColumnSortSpecs() { memset(this, 0, sizeof(*this)); }
};

//-----------------------------------------------------------------------------
// [SECTION] Helpers: Debug log, memory allocations macros, HvkVector<>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Debug Logging into ShowDebugLogWindow(), tty and more.
//-----------------------------------------------------------------------------

#ifndef HvkGui_DISABLE_DEBUG_TOOLS
#define HvkGui_DEBUG_LOG(...)        HvkGui::DebugLog(__VA_ARGS__)
#else
#define HvkGui_DEBUG_LOG(...)        ((void)0)
#endif

//-----------------------------------------------------------------------------
// Hvk_MALLOC(), Hvk_FREE(), Hvk_NEW(), Hvk_PLACEMENT_NEW(), Hvk_DELETE()
// We call C++ constructor on own allocated memory via the placement "new(ptr) Type()" syntax.
// Defining a custom placement new() with a custom parameter allows us to bypass including <new> which on some platforms complains when user has disabled exceptions.
//-----------------------------------------------------------------------------

struct HvkNewWrapper {};
inline void* operator new(size_t, HvkNewWrapper, void* ptr) { return ptr; }
inline void  operator delete(void*, HvkNewWrapper, void*)   {} // This is only required so we can use the symmetrical new()
#define Hvk_ALLOC(_SIZE)                     HvkGui::MemAlloc(_SIZE)
#define Hvk_FREE(_PTR)                       HvkGui::MemFree(_PTR)
#define Hvk_PLACEMENT_NEW(_PTR)              new(HvkNewWrapper(), _PTR)
#define Hvk_NEW(_TYPE)                       new(HvkNewWrapper(), HvkGui::MemAlloc(sizeof(_TYPE))) _TYPE
template<typename T> void Hvk_DELETE(T* p)   { if (p) { p->~T(); HvkGui::MemFree(p); } }

//-----------------------------------------------------------------------------
// HvkVector<>
// Lightweight std::vector<>-like class to avoid dragging dependencies (also, some implementations of STL with debug enabled are absurdly slow, we bypass it so our code runs fast in debug).
//-----------------------------------------------------------------------------
// - You generally do NOT need to care or use this ever. But we need to make it available in HvkGui.h because some of our public structures are relying on it.
// - We use std-like naming convention here, which is a little unusual for this codebase.
// - Hvkportant: clear() frees memory, resize(0) keep the allocated buffer. We use resize(0) a lot to intentionally recycle allocated buffers across frames and amortize our costs.
// - Hvkportant: our implementation does NOT call C++ constructors/destructors, we treat everything as raw data! This is intentional but be extra mindful of that,
//   Do NOT use this class as a std::vector replacement in your own code! Many of the structures used by dear HvkGui can be safely initialized by a zero-memset.
//-----------------------------------------------------------------------------

Hvk_MSVC_RUNTIME_CHECKS_OFF
template<typename T>
struct HvkVector
{
    int                 Size;
    int                 Capacity;
    T*                  Data;

    // Provide standard typedefs but we don't use them ourselves.
    typedef T                   value_type;
    typedef value_type*         iterator;
    typedef const value_type*   const_iterator;

    // Constructors, destructor
    inline HvkVector()                                       { Size = Capacity = 0; Data = NULL; }
    inline HvkVector(const HvkVector<T>& src)                 { Size = Capacity = 0; Data = NULL; operator=(src); }
    inline HvkVector<T>& operator=(const HvkVector<T>& src)   { clear(); resize(src.Size); if (Data && src.Data) memcpy(Data, src.Data, (size_t)Size * sizeof(T)); return *this; }
    inline ~HvkVector()                                      { if (Data) Hvk_FREE(Data); } // Hvkportant: does not destruct anything

    inline void         clear()                             { if (Data) { Size = Capacity = 0; Hvk_FREE(Data); Data = NULL; } }  // Hvkportant: does not destruct anything
    inline void         clear_delete()                      { for (int n = 0; n < Size; n++) Hvk_DELETE(Data[n]); clear(); }     // Hvkportant: never called automatically! always explicit.
    inline void         clear_destruct()                    { for (int n = 0; n < Size; n++) Data[n].~T(); clear(); }           // Hvkportant: never called automatically! always explicit.

    inline bool         empty() const                       { return Size == 0; }
    inline int          size() const                        { return Size; }
    inline int          size_in_bytes() const               { return Size * (int)sizeof(T); }
    inline int          max_size() const                    { return 0x7FFFFFFF / (int)sizeof(T); }
    inline int          capacity() const                    { return Capacity; }
    inline T&           operator[](int i)                   { Hvk_ASSERT(i >= 0 && i < Size); return Data[i]; }
    inline const T&     operator[](int i) const             { Hvk_ASSERT(i >= 0 && i < Size); return Data[i]; }

    inline T*           begin()                             { return Data; }
    inline const T*     begin() const                       { return Data; }
    inline T*           end()                               { return Data + Size; }
    inline const T*     end() const                         { return Data + Size; }
    inline T&           front()                             { Hvk_ASSERT(Size > 0); return Data[0]; }
    inline const T&     front() const                       { Hvk_ASSERT(Size > 0); return Data[0]; }
    inline T&           back()                              { Hvk_ASSERT(Size > 0); return Data[Size - 1]; }
    inline const T&     back() const                        { Hvk_ASSERT(Size > 0); return Data[Size - 1]; }
    inline void         swap(HvkVector<T>& rhs)              { int rhs_size = rhs.Size; rhs.Size = Size; Size = rhs_size; int rhs_cap = rhs.Capacity; rhs.Capacity = Capacity; Capacity = rhs_cap; T* rhs_data = rhs.Data; rhs.Data = Data; Data = rhs_data; }

    inline int          _grow_capacity(int sz) const        { int new_capacity = Capacity ? (Capacity + Capacity / 2) : 8; return new_capacity > sz ? new_capacity : sz; }
    inline void         resize(int new_size)                { if (new_size > Capacity) reserve(_grow_capacity(new_size)); Size = new_size; }
    inline void         resize(int new_size, const T& v)    { if (new_size > Capacity) reserve(_grow_capacity(new_size)); if (new_size > Size) for (int n = Size; n < new_size; n++) memcpy(&Data[n], &v, sizeof(v)); Size = new_size; }
    inline void         shrink(int new_size)                { Hvk_ASSERT(new_size <= Size); Size = new_size; } // Resize a vector to a smaller size, guaranteed not to cause a reallocation
    inline void         reserve(int new_capacity)           { if (new_capacity <= Capacity) return; T* new_data = (T*)Hvk_ALLOC((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); Hvk_FREE(Data); } Data = new_data; Capacity = new_capacity; }
    inline void         reserve_discard(int new_capacity)   { if (new_capacity <= Capacity) return; if (Data) Hvk_FREE(Data); Data = (T*)Hvk_ALLOC((size_t)new_capacity * sizeof(T)); Capacity = new_capacity; }

    // NB: It is illegal to call push_back/push_front/insert with a reference pointing inside the HvkVector data itself! e.g. v.push_back(v[10]) is forbidden.
    inline void         push_back(const T& v)               { if (Size == Capacity) reserve(_grow_capacity(Size + 1)); memcpy(&Data[Size], &v, sizeof(v)); Size++; }
    inline void         pop_back()                          { Hvk_ASSERT(Size > 0); Size--; }
    inline void         push_front(const T& v)              { if (Size == 0) push_back(v); else insert(Data, v); }
    inline T*           erase(const T* it)                  { Hvk_ASSERT(it >= Data && it < Data + Size); const ptrdiff_t off = it - Data; memmove(Data + off, Data + off + 1, ((size_t)Size - (size_t)off - 1) * sizeof(T)); Size--; return Data + off; }
    inline T*           erase(const T* it, const T* it_last){ Hvk_ASSERT(it >= Data && it < Data + Size && it_last >= it && it_last <= Data + Size); const ptrdiff_t count = it_last - it; const ptrdiff_t off = it - Data; memmove(Data + off, Data + off + count, ((size_t)Size - (size_t)off - (size_t)count) * sizeof(T)); Size -= (int)count; return Data + off; }
    inline T*           erase_unsorted(const T* it)         { Hvk_ASSERT(it >= Data && it < Data + Size);  const ptrdiff_t off = it - Data; if (it < Data + Size - 1) memcpy(Data + off, Data + Size - 1, sizeof(T)); Size--; return Data + off; }
    inline T*           insert(const T* it, const T& v)     { Hvk_ASSERT(it >= Data && it <= Data + Size); const ptrdiff_t off = it - Data; if (Size == Capacity) reserve(_grow_capacity(Size + 1)); if (off < (int)Size) memmove(Data + off + 1, Data + off, ((size_t)Size - (size_t)off) * sizeof(T)); memcpy(&Data[off], &v, sizeof(v)); Size++; return Data + off; }
    inline bool         contains(const T& v) const          { const T* data = Data;  const T* data_end = Data + Size; while (data < data_end) if (*data++ == v) return true; return false; }
    inline T*           find(const T& v)                    { T* data = Data;  const T* data_end = Data + Size; while (data < data_end) if (*data == v) break; else ++data; return data; }
    inline const T*     find(const T& v) const              { const T* data = Data;  const T* data_end = Data + Size; while (data < data_end) if (*data == v) break; else ++data; return data; }
    inline int          find_index(const T& v) const        { const T* data_end = Data + Size; const T* it = find(v); if (it == data_end) return -1; const ptrdiff_t off = it - Data; return (int)off; }
    inline bool         find_erase(const T& v)              { const T* it = find(v); if (it < Data + Size) { erase(it); return true; } return false; }
    inline bool         find_erase_unsorted(const T& v)     { const T* it = find(v); if (it < Data + Size) { erase_unsorted(it); return true; } return false; }
    inline int          index_from_ptr(const T* it) const   { Hvk_ASSERT(it >= Data && it < Data + Size); const ptrdiff_t off = it - Data; return (int)off; }
};
Hvk_MSVC_RUNTIME_CHECKS_RESTORE

//-----------------------------------------------------------------------------
// [SECTION] HvkGuiStyle
//-----------------------------------------------------------------------------
// You may modify the HvkGui::GetStyle() main instance during initialization and before NewFrame().
// During the frame, use HvkGui::PushStyleVar(HvkGuiStyleVar_XXXX)/PopStyleVar() to alter the main style values,
// and HvkGui::PushStyleColor(HvkGuiCol_XXX)/PopStyleColor() for colors.
//-----------------------------------------------------------------------------

struct HvkGuiStyle
{
    // Font scaling
    // - recap: HvkGui::GetFontSize() == FontSizeBase * (FontScaleMain * FontScaleDpi * other_scaling_factors)
    float       FontSizeBase;               // Current base font size before external global factors are applied. Use PushFont(NULL, size) to modify. Use HvkGui::GetFontSize() to obtain scaled value.
    float       FontScaleMain;              // Main global scale factor. May be set by application once, or exposed to end-user.
    float       FontScaleDpi;               // Additional global scale factor from viewport/monitor contents scale. When io.ConfigDpiScaleFonts is enabled, this is automatically overwritten when changing monitor DPI.

    float       Alpha;                      // Global alpha applies to everything in Dear HvkGui.
    float       DisabledAlpha;              // Additional alpha multiplier applied by BeginDisabled(). Multiply over current value of Alpha.
    HvkVec2      WindowPadding;              // Padding within a window.
    float       WindowRounding;             // Radius of window corners rounding. Set to 0.0f to have rectangular windows. Large values tend to lead to variety of artifacts and are not recommended.
    float       WindowBorderSize;           // Thickness of border around windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    float       WindowBorderHoverPadding;   // Hit-testing extent outside/inside resizing border. Also extend determination of hovered window. Generally meaningfully larger than WindowBorderSize to make it easy to reach borders.
    HvkVec2      WindowMinSize;              // Minimum window size. This is a global setting. If you want to constrain individual windows, use SetNextWindowSizeConstraints().
    HvkVec2      WindowTitleAlign;           // Alignment for title bar text. Defaults to (0.0f,0.5f) for left-aligned,vertically centered.
    HvkGuiDir    WindowMenuButtonPosition;   // Side of the collapsing/docking button in the title bar (None/Left/Right). Defaults to HvkGuiDir_Left.
    float       ChildRounding;              // Radius of child window corners rounding. Set to 0.0f to have rectangular windows.
    float       ChildBorderSize;            // Thickness of border around child windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    float       PopupRounding;              // Radius of popup window corners rounding. (Note that tooltip windows use WindowRounding)
    float       PopupBorderSize;            // Thickness of border around popup/tooltip windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    HvkVec2      FramePadding;               // Padding within a framed rectangle (used by most widgets).
    float       FrameRounding;              // Radius of frame corners rounding. Set to 0.0f to have rectangular frame (used by most widgets).
    float       FrameBorderSize;            // Thickness of border around frames. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    HvkVec2      ItemSpacing;                // Horizontal and vertical spacing between widgets/lines.
    HvkVec2      ItemInnerSpacing;           // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label).
    HvkVec2      CellPadding;                // Padding within a table cell. Cellpadding.x is locked for entire table. CellPadding.y may be altered between different rows.
    HvkVec2      TouchExtraPadding;          // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
    float       IndentSpacing;              // Horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
    float       ColumnsMinSpacing;          // Minimum horizontal spacing between two columns. Preferably > (FramePadding.x + 1).
    float       ScrollbarSize;              // Width of the vertical scrollbar, Height of the horizontal scrollbar.
    float       ScrollbarRounding;          // Radius of grab corners for scrollbar.
    float       ScrollbarPadding;           // Padding of scrollbar grab within its frame (same for both axes).
    float       GrabMinSize;                // Minimum width/height of a grab box for slider/scrollbar.
    float       GrabRounding;               // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
    float       LogSliderDeadzone;          // The size in pixels of the dead-zone around zero on logarithmic sliders that cross zero.
    float       HvkageBorderSize;            // Thickness of border around Hvkage() calls.
    float       TabRounding;                // Radius of upper corners of a tab. Set to 0.0f to have rectangular tabs.
    float       TabBorderSize;              // Thickness of border around tabs.
    float       TabMinWidthBase;            // Minimum tab width, to make tabs larger than their contents. TabBar buttons are not affected.
    float       TabMinWidthShrink;          // Minimum tab width after shrinking, when using HvkGuiTabBarFlags_FittingPolicyMixed policy.
    float       TabCloseButtonMinWidthSelected;     // -1: always visible. 0.0f: visible when hovered. >0.0f: visible when hovered if minimum width.
    float       TabCloseButtonMinWidthUnselected;   // -1: always visible. 0.0f: visible when hovered. >0.0f: visible when hovered if minimum width. FLT_MAX: never show close button when unselected.
    float       TabBarBorderSize;           // Thickness of tab-bar separator, which takes on the tab active color to denote focus.
    float       TabBarOverlineSize;         // Thickness of tab-bar overline, which highlights the selected tab-bar.
    float       TableAngledHeadersAngle;    // Angle of angled headers (supported values range from -50.0f degrees to +50.0f degrees).
    HvkVec2      TableAngledHeadersTextAlign;// Alignment of angled headers within the cell
    HvkGuiTreeNodeFlags TreeLinesFlags;      // Default way to draw lines connecting TreeNode hierarchy. HvkGuiTreeNodeFlags_DrawLinesNone or HvkGuiTreeNodeFlags_DrawLinesFull or HvkGuiTreeNodeFlags_DrawLinesToNodes.
    float       TreeLinesSize;              // Thickness of outlines when using HvkGuiTreeNodeFlags_DrawLines.
    float       TreeLinesRounding;          // Radius of lines connecting child nodes to the vertical line.
    float       DragDropTargetRounding;     // Radius of the drag and drop target frame.
    float       DragDropTargetBorderSize;   // Thickness of the drag and drop target border.
    float       DragDropTargetPadding;      // Size to expand the drag and drop target from actual target item size.
    float       ColorMarkerSize;            // Size of R/G/B/A color markers for ColorEdit4() and for Drags/Sliders when using HvkGuiSliderFlags_ColorMarkers.
    HvkGuiDir    ColorButtonPosition;        // Side of the color button in the ColorEdit4 widget (left/right). Defaults to HvkGuiDir_Right.
    HvkVec2      ButtonTextAlign;            // Alignment of button text when button is larger than text. Defaults to (0.5f, 0.5f) (centered).
    HvkVec2      SelectableTextAlign;        // Alignment of selectable text. Defaults to (0.0f, 0.0f) (top-left aligned). It's generally Hvkportant to keep this left-aligned if you want to lay multiple items on a same line.
    float       SeparatorTextBorderSize;    // Thickness of border in SeparatorText()
    HvkVec2      SeparatorTextAlign;         // Alignment of text within the separator. Defaults to (0.0f, 0.5f) (left aligned, center).
    HvkVec2      SeparatorTextPadding;       // Horizontal offset of text from each edge of the separator + spacing on other axis. Generally small values. .y is recommended to be == FramePadding.y.
    HvkVec2      DisplayWindowPadding;       // Apply to regular windows: amount which we enforce to keep visible when moving near edges of your screen.
    HvkVec2      DisplaySafeAreaPadding;     // Apply to every windows, menus, popups, tooltips: amount where we avoid displaying contents. Adjust if you cannot see the edges of your screen (e.g. on a TV where scaling has not been configured).
    float       MouseCursorScale;           // Scale software rendered mouse cursor (when io.MouseDrawCursor is enabled). We apply per-monitor DPI scaling over this scale. May be removed later.
    bool        AntiAliasedLines;           // Enable anti-aliased lines/borders. Disable if you are really tight on CPU/GPU. Latched at the beginning of the frame (copied to HvkDrawList).
    bool        AntiAliasedLinesUseTex;     // Enable anti-aliased lines/borders using textures where possible. Require backend to render with bilinear filtering (NOT point/nearest filtering). Latched at the beginning of the frame (copied to HvkDrawList).
    bool        AntiAliasedFill;            // Enable anti-aliased edges around filled shapes (rounded rectangles, circles, etc.). Disable if you are really tight on CPU/GPU. Latched at the beginning of the frame (copied to HvkDrawList).
    float       CurveTessellationTol;       // Tessellation tolerance when using PathBezierCurveTo() without a specific number of segments. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.
    float       CircleTessellationMaxError; // Maximum error (in pixels) allowed when using AddCircle()/AddCircleFilled() or drawing rounded corner rectangles with no explicit segment count specified. Decrease for higher quality but more geometry.

    // Colors
    HvkVec4      Colors[HvkGuiCol_COUNT];

    // Behaviors
    // (It is possible to modify those fields mid-frame if specific behavior need it, unlike e.g. configuration fields in HvkGuiIO)
    float             HoverStationaryDelay;     // Delay for IsItemHovered(HvkGuiHoveredFlags_Stationary). Time required to consider mouse stationary.
    float             HoverDelayShort;          // Delay for IsItemHovered(HvkGuiHoveredFlags_DelayShort). Usually used along with HoverStationaryDelay.
    float             HoverDelayNormal;         // Delay for IsItemHovered(HvkGuiHoveredFlags_DelayNormal). "
    HvkGuiHoveredFlags HoverFlagsForTooltipMouse;// Default flags when using IsItemHovered(HvkGuiHoveredFlags_ForTooltip) or BeginItemTooltip()/SetItemTooltip() while using mouse.
    HvkGuiHoveredFlags HoverFlagsForTooltipNav;  // Default flags when using IsItemHovered(HvkGuiHoveredFlags_ForTooltip) or BeginItemTooltip()/SetItemTooltip() while using keyboard/gamepad.

    // [Internal]
    float       _MainScale;                 // FIXME-WIP: Reference scale, as applied by ScaleAllSizes().
    float       _NextFrameFontSizeBase;     // FIXME: Temporary hack until we finish remaining work.

    // Functions
    HvkGui_API   HvkGuiStyle();
    HvkGui_API   void ScaleAllSizes(float scale_factor); // Scale all spacing/padding/thickness values. Do not scale fonts.

    // Obsolete names
#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    // TabMinWidthForCloseButton = TabCloseButtonMinWidthUnselected // Renamed in 1.91.9.
#endif
};

//-----------------------------------------------------------------------------
// [SECTION] HvkGuiIO
//-----------------------------------------------------------------------------
// Communicate most settings and inputs/outputs to Dear HvkGui using this structure.
// Access via HvkGui::GetIO(). Read 'Programmer guide' section in .cpp file for general usage.
// It is generally expected that:
// - initialization: backends and user code writes to HvkGuiIO.
// - main loop: backends writes to HvkGuiIO, user code and HvkGui code reads from HvkGuiIO.
//-----------------------------------------------------------------------------
// Also see HvkGui::GetPlatformIO() and HvkGuiPlatformIO struct for OS/platform related functions: clipboard, IME etc.
//-----------------------------------------------------------------------------

// [Internal] Storage used by IsKeyDown(), IsKeyPressed() etc functions.
// If prior to 1.87 you used io.KeysDownDuration[] (which was marked as internal), you should use GetKeyData(key)->DownDuration and *NOT* io.KeysData[key]->DownDuration.
struct HvkGuiKeyData
{
    bool        Down;               // True for if key is down
    float       DownDuration;       // Duration the key has been down (<0.0f: not pressed, 0.0f: just pressed, >0.0f: time held)
    float       DownDurationPrev;   // Last frame duration the key has been down
    float       AnalogValue;        // 0.0f..1.0f for gamepad values
};

struct HvkGuiIO
{
    //------------------------------------------------------------------
    // Configuration                            // Default value
    //------------------------------------------------------------------

    HvkGuiConfigFlags   ConfigFlags;             // = 0              // See HvkGuiConfigFlags_ enum. Set by user/application. Keyboard/Gamepad navigation options, etc.
    HvkGuiBackendFlags  BackendFlags;            // = 0              // See HvkGuiBackendFlags_ enum. Set by backend (HvkGui_impl_xxx files or custom backend) to communicate features supported by the backend.
    HvkVec2      DisplaySize;                    // <unset>          // Main display size, in pixels (== GetMainViewport()->Size). May change every frame.
    HvkVec2      DisplayFramebufferScale;        // = (1, 1)         // Main display density. For retina display where window coordinates are different from framebuffer coordinates. This will affect font density + will end up in HvkDrawData::FramebufferScale.
    float       DeltaTime;                      // = 1.0f/60.0f     // Time elapsed since last frame, in seconds. May change every frame.
    float       IniSavingRate;                  // = 5.0f           // Minimum time between saving positions/sizes to .ini file, in seconds.
    const char* IniFilename;                    // = "HvkGui.ini"    // Path to .ini file (Hvkportant: default "HvkGui.ini" is relative to current working dir!). Set NULL to disable automatic .ini loading/saving or if you want to manually call LoadIniSettingsXXX() / SaveIniSettingsXXX() functions.
    const char* LogFilename;                    // = "HvkGui_log.txt"// Path to .log file (default parameter to HvkGui::LogToFile when no file is specified).
    void*       UserData;                       // = NULL           // Store your own data.

    // Font system
    HvkFontAtlas*Fonts;                          // <auto>           // Font atlas: load, rasterize and pack one or more fonts into a single texture.
    HvkFont*     FontDefault;                    // = NULL           // Font to use on NewFrame(). Use NULL to uses Fonts->Fonts[0].
    bool        FontAllowUserScaling;           // = false          // Allow user scaling text of individual window with Ctrl+Wheel.

    // Keyboard/Gamepad Navigation options
    bool        ConfigNavSwapGamepadButtons;    // = false          // Swap Activate<>Cancel (A<>B) buttons, matching typical "Nintendo/Japanese style" gamepad layout.
    bool        ConfigNavMoveSetMousePos;       // = false          // Directional/tabbing navigation teleports the mouse cursor. May be useful on TV/console systems where moving a virtual mouse is difficult. Will update io.MousePos and set io.WantSetMousePos=true.
    bool        ConfigNavCaptureKeyboard;       // = true           // Sets io.WantCaptureKeyboard when io.NavActive is set.
    bool        ConfigNavEscapeClearFocusItem;  // = true           // Pressing Escape can clear focused item + navigation id/highlight. Set to false if you want to always keep highlight on.
    bool        ConfigNavEscapeClearFocusWindow;// = false          // Pressing Escape can clear focused window as well (super set of io.ConfigNavEscapeClearFocusItem).
    bool        ConfigNavCursorVisibleAuto;     // = true           // Using directional navigation key makes the cursor visible. Mouse click hides the cursor.
    bool        ConfigNavCursorVisibleAlways;   // = false          // Navigation cursor is always visible.

    // Miscellaneous options
    // (you can visualize and interact with all options in 'Demo->Configuration')
    bool        MouseDrawCursor;                // = false          // Request HvkGui to draw a mouse cursor for you (if you are on a platform without a mouse cursor). Cannot be easily renamed to 'io.ConfigXXX' because this is frequently used by backend implementations.
    bool        ConfigMacOSXBehaviors;          // = defined(__APPLE__) // Swap Cmd<>Ctrl keys + OS X style text editing cursor movement using Alt instead of Ctrl, Shortcuts using Cmd/Super instead of Ctrl, Line/Text Start and End using Cmd+Arrows instead of Home/End, Double click selects by word instead of selecting whole text, Multi-selection in lists uses Cmd/Super instead of Ctrl.
    bool        ConfigInputTrickleEventQueue;   // = true           // Enable input queue trickling: some types of events submitted during the same frame (e.g. button down + up) will be spread over multiple frames, Hvkproving interactions with low framerates.
    bool        ConfigInputTextCursorBlink;     // = true           // Enable blinking cursor (optional as some users consider it to be distracting).
    bool        ConfigInputTextEnterKeepActive; // = false          // [BETA] Pressing Enter will keep item active and select contents (single-line only).
    bool        ConfigDragClickToInputText;     // = false          // [BETA] Enable turning DragXXX widgets into text input with a simple mouse click-release (without moving). Not desirable on devices without a keyboard.
    bool        ConfigWindowsResizeFromEdges;   // = true           // Enable resizing of windows from their edges and from the lower-left corner. This requires HvkGuiBackendFlags_HasMouseCursors for better mouse cursor feedback. (This used to be a per-window HvkGuiWindowFlags_ResizeFromAnySide flag)
    bool        ConfigWindowsMoveFromTitleBarOnly;  // = false      // Enable allowing to move windows only when clicking on their title bar. Does not apply to windows without a title bar.
    bool        ConfigWindowsCopyContentsWithCtrlC; // = false      // [EXPERIMENTAL] Ctrl+C copy the contents of focused window into the clipboard. Experimental because: (1) has known issues with nested Begin/End pairs (2) text output quality varies (3) text output is in submission order rather than spatial order.
    bool        ConfigScrollbarScrollByPage;    // = true           // Enable scrolling page by page when clicking outside the scrollbar grab. When disabled, always scroll to clicked location. When enabled, Shift+Click scrolls to clicked location.
    float       ConfigMemoryCompactTimer;       // = 60.0f          // Timer (in seconds) to free transient windows/tables memory buffers when unused. Set to -1.0f to disable.

    // Inputs Behaviors
    // (other variables, ones which are expected to be tweaked within UI code, are exposed in HvkGuiStyle)
    float       MouseDoubleClickTime;           // = 0.30f          // Time for a double-click, in seconds.
    float       MouseDoubleClickMaxDist;        // = 6.0f           // Distance threshold to stay in to validate a double-click, in pixels.
    float       MouseDragThreshold;             // = 6.0f           // Distance threshold before considering we are dragging.
    float       KeyRepeatDelay;                 // = 0.275f         // When holding a key/button, time before it starts repeating, in seconds (for buttons in Repeat mode, etc.).
    float       KeyRepeatRate;                  // = 0.050f         // When holding a key/button, rate at which it repeats, in seconds.

    //------------------------------------------------------------------
    // Debug options
    //------------------------------------------------------------------

    // Options to configure Error Handling and how we handle recoverable errors [EXPERIMENTAL]
    // - Error recovery is provided as a way to facilitate:
    //    - Recovery after a programming error (native code or scripting language - the later tends to facilitate iterating on code while running).
    //    - Recovery after running an exception handler or any error processing which may skip code after an error has been detected.
    // - Error recovery is not perfect nor guaranteed! It is a feature to ease development.
    //   You not are not supposed to rely on it in the course of a normal application run.
    // - Functions that support error recovery are using Hvk_ASSERT_USER_ERROR() instead of Hvk_ASSERT().
    // - By design, we do NOT allow error recovery to be 100% silent. One of the three options needs to be checked!
    // - Always ensure that on programmers seats you have at minimum Asserts or Tooltips enabled when making direct HvkGui API calls!
    //   Otherwise it would severely hinder your ability to catch and correct mistakes!
    // Read https://github.com/ocornut/HvkGui/wiki/Error-Handling for details.
    // - Programmer seats: keep asserts (default), or disable asserts and keep error tooltips (new and nice!)
    // - Non-programmer seats: maybe disable asserts, but make sure errors are resurfaced (tooltips, visible log entries, use callback etc.)
    // - Recovery after error/exception: record stack sizes with ErrorRecoveryStoreState(), disable assert, set log callback (to e.g. trigger high-level breakpoint), recover with ErrorRecoveryTryToRecoverState(), restore settings.
    bool        ConfigErrorRecovery;                // = true       // Enable error recovery support. Some errors won't be detected and lead to direct crashes if recovery is disabled.
    bool        ConfigErrorRecoveryEnableAssert;    // = true       // Enable asserts on recoverable error. By default call Hvk_ASSERT() when returning from a failing Hvk_ASSERT_USER_ERROR()
    bool        ConfigErrorRecoveryEnableDebugLog;  // = true       // Enable debug log output on recoverable errors.
    bool        ConfigErrorRecoveryEnableTooltip;   // = true       // Enable tooltip on recoverable errors. The tooltip include a way to enable asserts if they were disabled.

    // Option to enable various debug tools showing buttons that will call the Hvk_DEBUG_BREAK() macro.
    // - The Item Picker tool will be available regardless of this being enabled, in order to maximize its discoverability.
    // - Requires a debugger being attached, otherwise Hvk_DEBUG_BREAK() options will appear to crash your application.
    //   e.g. io.ConfigDebugIsDebuggerPresent = ::IsDebuggerPresent() on Win32, or refer to HvkOsIsDebuggerPresent() HvkGui_test_engine/HvkGui_te_utils.cpp for a Unix compatible version.
    bool        ConfigDebugIsDebuggerPresent;   // = false          // Enable various tools calling Hvk_DEBUG_BREAK().

    // Tools to detect code submitting items with conflicting/duplicate IDs
    // - Code should use PushID()/PopID() in loops, or append "##xx" to same-label identifiers.
    // - Empty label e.g. Button("") == same ID as parent widget/node. Use Button("##xx") instead!
    // - See FAQ https://github.com/ocornut/HvkGui/blob/master/docs/FAQ.md#q-about-the-id-stack-system
    bool        ConfigDebugHighlightIdConflicts;// = true           // Highlight and show an error message popup when multiple items have conflicting identifiers.
    bool        ConfigDebugHighlightIdConflictsShowItemPicker;//=true // Show "Item Picker" button in aforementioned popup.

    // Tools to test correct Begin/End and BeginChild/EndChild behaviors.
    // - Presently Begin()/End() and BeginChild()/EndChild() needs to ALWAYS be called in tandem, regardless of return value of BeginXXX()
    // - This is inconsistent with other BeginXXX functions and create confusion for many users.
    // - We expect to update the API eventually. In the meanwhile we provide tools to facilitate checking user-code behavior.
    bool        ConfigDebugBeginReturnValueOnce;// = false          // First-time calls to Begin()/BeginChild() will return false. NEEDS TO BE SET AT APPLICATION BOOT TIME if you don't want to miss windows.
    bool        ConfigDebugBeginReturnValueLoop;// = false          // Some calls to Begin()/BeginChild() will return false. Will cycle through window depths then repeat. Suggested use: add "io.ConfigDebugBeginReturnValue = io.KeyShift" in your main loop then occasionally press SHIFT. Windows should be flickering while running.

    // Option to deactivate io.AddFocusEvent(false) handling.
    // - May facilitate interactions with a debugger when focus loss leads to clearing inputs data.
    // - Backends may have other side-effects on focus loss, so this will reduce side-effects but not necessary remove all of them.
    bool        ConfigDebugIgnoreFocusLoss;     // = false          // Ignore io.AddFocusEvent(false), consequently not calling io.ClearInputKeys()/io.ClearInputMouse() in input processing.

    // Option to audit .ini data
    bool        ConfigDebugIniSettings;         // = false          // Save .ini data with extra comments (particularly helpful for Docking, but makes saving slower)

    //------------------------------------------------------------------
    // Platform Identifiers
    // (the HvkGui_impl_xxxx backend files are setting those up for you)
    //------------------------------------------------------------------

    // Nowadays those would be stored in HvkGuiPlatformIO but we are leaving them here for legacy reasons.
    // Optional: Platform/Renderer backend name (informational only! will be displayed in About Window) + User data for backend/wrappers to store their own stuff.
    const char* BackendPlatformName;            // = NULL
    const char* BackendRendererName;            // = NULL
    void*       BackendPlatformUserData;        // = NULL           // User data for platform backend
    void*       BackendRendererUserData;        // = NULL           // User data for renderer backend
    void*       BackendLanguageUserData;        // = NULL           // User data for non C++ programming language backend

    //------------------------------------------------------------------
    // Input - Call before calling NewFrame()
    //------------------------------------------------------------------

    // Input Functions
    HvkGui_API void  AddKeyEvent(HvkGuiKey key, bool down);                   // Queue a new key down/up event. Key should be "translated" (as in, generally HvkGuiKey_A matches the key end-user would use to emit an 'A' character)
    HvkGui_API void  AddKeyAnalogEvent(HvkGuiKey key, bool down, float v);    // Queue a new key down/up event for analog values (e.g. HvkGuiKey_Gamepad_ values). Dead-zones should be handled by the backend.
    HvkGui_API void  AddMousePosEvent(float x, float y);                     // Queue a mouse position update. Use -FLT_MAX,-FLT_MAX to signify no mouse (e.g. app not focused and not hovered)
    HvkGui_API void  AddMouseButtonEvent(int button, bool down);             // Queue a mouse button change
    HvkGui_API void  AddMouseWheelEvent(float wheel_x, float wheel_y);       // Queue a mouse wheel update. wheel_y<0: scroll down, wheel_y>0: scroll up, wheel_x<0: scroll right, wheel_x>0: scroll left.
    HvkGui_API void  AddMouseSourceEvent(HvkGuiMouseSource source);           // Queue a mouse source change (Mouse/TouchScreen/Pen)
    HvkGui_API void  AddFocusEvent(bool focused);                            // Queue a gain/loss of focus for the application (generally based on OS/platform focus of your window)
    HvkGui_API void  AddInputCharacter(unsigned int c);                      // Queue a new character input
    HvkGui_API void  AddInputCharacterUTF16(HvkWchar16 c);                    // Queue a new character input from a UTF-16 character, it can be a surrogate
    HvkGui_API void  AddInputCharactersUTF8(const char* str);                // Queue a new characters input from a UTF-8 string

    HvkGui_API void  SetKeyEventNativeData(HvkGuiKey key, int native_keycode, int native_scancode, int native_legacy_index = -1); // [Optional] Specify index for legacy <1.87 IsKeyXXX() functions with native indices + specify native keycode, scancode.
    HvkGui_API void  SetAppAcceptingEvents(bool accepting_events);           // Set master flag for accepting key/mouse/text events (default to true). Useful if you have native dialog boxes that are interrupting your application loop/refresh, and you want to disable events being queued while your app is frozen.
    HvkGui_API void  ClearEventsQueue();                                     // Clear all incoming events.
    HvkGui_API void  ClearInputKeys();                                       // Clear current keyboard/gamepad state + current frame text input buffer. Equivalent to releasing all keys/buttons.
    HvkGui_API void  ClearInputMouse();                                      // Clear current mouse state.

    //------------------------------------------------------------------
    // Output - Updated by NewFrame() or EndFrame()/Render()
    // (when reading from the io.WantCaptureMouse, io.WantCaptureKeyboard flags to dispatch your inputs, it is
    //  generally easier and more correct to use their state BEFORE calling NewFrame(). See FAQ for details!)
    //------------------------------------------------------------------

    bool        WantCaptureMouse;                   // Set when Dear HvkGui will use mouse inputs, in this case do not dispatch them to your main game/application (either way, always pass on mouse inputs to HvkGui). (e.g. unclicked mouse is hovering over an HvkGui window, widget is active, mouse was clicked over an HvkGui window, etc.).
    bool        WantCaptureKeyboard;                // Set when Dear HvkGui will use keyboard inputs, in this case do not dispatch them to your main game/application (either way, always pass keyboard inputs to HvkGui). (e.g. InputText active, or an HvkGui window is focused and navigation is enabled, etc.).
    bool        WantTextInput;                      // Mobile/console: when set, you may display an on-screen keyboard. This is set by Dear HvkGui when it wants textual keyboard input to happen (e.g. when a InputText widget is active).
    bool        WantSetMousePos;                    // MousePos has been altered, backend should reposition mouse on next frame. Rarely used! Set only when io.ConfigNavMoveSetMousePos is enabled.
    bool        WantSaveIniSettings;                // When manual .ini load/save is active (io.IniFilename == NULL), this will be set to notify your application that you can call SaveIniSettingsToMemory() and save yourself. Hvkportant: clear io.WantSaveIniSettings yourself after saving!
    bool        NavActive;                          // Keyboard/Gamepad navigation is currently allowed (will handle HvkGuiKey_NavXXX events) = a window is focused and it doesn't use the HvkGuiWindowFlags_NoNavInputs flag.
    bool        NavVisible;                         // Keyboard/Gamepad navigation highlight is visible and allowed (will handle HvkGuiKey_NavXXX events).
    float       Framerate;                          // Estimate of application framerate (rolling average over 60 frames, based on io.DeltaTime), in frame per second. Solely for convenience. Slow applications may not want to use a moving average or may want to reset underlying buffers occasionally.
    int         MetricsRenderVertices;              // Vertices output during last call to Render()
    int         MetricsRenderIndices;               // Indices output during last call to Render() = number of triangles * 3
    int         MetricsRenderWindows;               // Number of visible windows
    int         MetricsActiveWindows;               // Number of active windows
    HvkVec2      MouseDelta;                         // Mouse delta. Note that this is zero if either current or previous position are invalid (-FLT_MAX,-FLT_MAX), so a disappearing/reappearing mouse won't have a huge delta.

    //------------------------------------------------------------------
    // [Internal] Dear HvkGui will maintain those fields. Forward compatibility not guaranteed!
    //------------------------------------------------------------------

    HvkGuiContext* Ctx;                              // Parent UI context (needs to be set explicitly by parent).

    // Main Input State
    // (this block used to be written by backend, since 1.87 it is best to NOT write to those directly, call the AddXXX functions above instead)
    // (reading from those variables is fair game, as they are extremely unlikely to be moving anywhere)
    HvkVec2      MousePos;                           // Mouse position, in pixels. Set to HvkVec2(-FLT_MAX, -FLT_MAX) if mouse is unavailable (on another screen, etc.)
    bool        MouseDown[5];                       // Mouse buttons: 0=left, 1=right, 2=middle + extras (HvkGuiMouseButton_COUNT == 5). Dear HvkGui mostly uses left and right buttons. Other buttons allow us to track if the mouse is being used by your application + available to user as a convenience via IsMouse** API.
    float       MouseWheel;                         // Mouse wheel Vertical: 1 unit scrolls about 5 lines text. >0 scrolls Up, <0 scrolls Down. Hold Shift to turn vertical scroll into horizontal scroll.
    float       MouseWheelH;                        // Mouse wheel Horizontal. >0 scrolls Left, <0 scrolls Right. Most users don't have a mouse with a horizontal wheel, may not be filled by all backends.
    HvkGuiMouseSource MouseSource;                   // Mouse actual input peripheral (Mouse/TouchScreen/Pen).
    bool        KeyCtrl;                            // Keyboard modifier down: Ctrl (non-macOS), Cmd (macOS)
    bool        KeyShift;                           // Keyboard modifier down: Shift
    bool        KeyAlt;                             // Keyboard modifier down: Alt
    bool        KeySuper;                           // Keyboard modifier down: Windows/Super (non-macOS), Ctrl (macOS)

    // Other state maintained from data above + IO function calls
    HvkGuiKeyChord KeyMods;                          // Key mods flags (any of HvkGuiMod_Ctrl/HvkGuiMod_Shift/HvkGuiMod_Alt/HvkGuiMod_Super flags, same as io.KeyCtrl/KeyShift/KeyAlt/KeySuper but merged into flags). Read-only, updated by NewFrame()
    HvkGuiKeyData  KeysData[HvkGuiKey_NamedKey_COUNT];// Key state for all known keys. MUST use 'key - HvkGuiKey_NamedKey_BEGIN' as index. Use IsKeyXXX() functions to access this.
    bool        WantCaptureMouseUnlessPopupClose;   // Alternative to WantCaptureMouse: (WantCaptureMouse == true && WantCaptureMouseUnlessPopupClose == false) when a click over void is expected to close a popup.
    HvkVec2      MousePosPrev;                       // Previous mouse position (note that MouseDelta is not necessary == MousePos-MousePosPrev, in case either position is invalid)
    HvkVec2      MouseClickedPos[5];                 // Position at time of clicking
    double      MouseClickedTime[5];                // Time of last click (used to figure out double-click)
    bool        MouseClicked[5];                    // Mouse button went from !Down to Down (same as MouseClickedCount[x] != 0)
    bool        MouseDoubleClicked[5];              // Has mouse button been double-clicked? (same as MouseClickedCount[x] == 2)
    HvkU16       MouseClickedCount[5];               // == 0 (not clicked), == 1 (same as MouseClicked[]), == 2 (double-clicked), == 3 (triple-clicked) etc. when going from !Down to Down
    HvkU16       MouseClickedLastCount[5];           // Count successive number of clicks. Stays valid after mouse release. Reset after another click is done.
    bool        MouseReleased[5];                   // Mouse button went from Down to !Down
    double      MouseReleasedTime[5];               // Time of last released (rarely used! but useful to handle delayed single-click when trying to disambiguate them from double-click).
    bool        MouseDownOwned[5];                  // Track if button was clicked inside a dear HvkGui window or over void blocked by a popup. We don't request mouse capture from the application if click started outside HvkGui bounds.
    bool        MouseDownOwnedUnlessPopupClose[5];  // Track if button was clicked inside a dear HvkGui window.
    bool        MouseWheelRequestAxisSwap;          // On a non-Mac system, holding Shift requests WheelY to perform the equivalent of a WheelX event. On a Mac system this is already enforced by the system.
    bool        MouseCtrlLeftAsRightClick;          // (OSX) Set to true when the current click was a Ctrl+Click that spawned a simulated right click
    float       MouseDownDuration[5];               // Duration the mouse button has been down (0.0f == just clicked)
    float       MouseDownDurationPrev[5];           // Previous time the mouse button has been down
    float       MouseDragMaxDistanceSqr[5];         // Squared maximum distance of how much mouse has traveled from the clicking point (used for moving thresholds)
    float       PenPressure;                        // Touch/Pen pressure (0.0f to 1.0f, should be >0.0f only when MouseDown[0] == true). Helper storage currently unused by Dear HvkGui.
    bool        AppFocusLost;                       // Only modify via AddFocusEvent()
    bool        AppAcceptingEvents;                 // Only modify via SetAppAcceptingEvents()
    HvkWchar16   InputQueueSurrogate;                // For AddInputCharacterUTF16()
    HvkVector<HvkWchar> InputQueueCharacters;         // Queue of _characters_ input (obtained by platform backend). Fill using AddInputCharacter() helper.

    // Legacy: before 1.87, we required backend to fill io.KeyMap[] (HvkGui->native map) during initialization and io.KeysDown[] (native indices) every frame.
    // This is still temporarily supported as a legacy feature. However the new preferred scheme is for backend to call io.AddKeyEvent().
    //   Old (<1.87):  HvkGui::IsKeyPressed(HvkGui::GetIO().KeyMap[HvkGuiKey_Space]) --> New (1.87+) HvkGui::IsKeyPressed(HvkGuiKey_Space)
    //   Old (<1.87):  HvkGui::IsKeyPressed(MYPLATFORM_KEY_SPACE)                  --> New (1.87+) HvkGui::IsKeyPressed(HvkGuiKey_Space)
    // Read https://github.com/ocornut/HvkGui/issues/4921 for details.
    //int       KeyMap[HvkGuiKey_COUNT];             // [LEGACY] Input: map of indices into the KeysDown[512] entries array which represent your "native" keyboard state. The first 512 are now unused and should be kept zero. Legacy backend will write into KeyMap[] using HvkGuiKey_ indices which are always >512.
    //bool      KeysDown[HvkGuiKey_COUNT];           // [LEGACY] Input: Keyboard keys that are pressed (ideally left in the "native" order your engine has access to keyboard keys, so you can use your own defines/enums for keys). This used to be [512] sized. It is now HvkGuiKey_COUNT to allow legacy io.KeysDown[GetKeyIndex(...)] to work without an overflow.
    //float     NavInputs[HvkGuiNavInput_COUNT];     // [LEGACY] Since 1.88, NavInputs[] was removed. Backends from 1.60 to 1.86 won't build. Feed gamepad inputs via io.AddKeyEvent() and HvkGuiKey_GamepadXXX enums.
    //void*     IMEWindowHandle;                    // [Obsoleted in 1.87] Set HvkGuiViewport::PlatformHandleRaw instead. Set this to your HWND to get automatic IME cursor positioning.

#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    float       FontGlobalScale;                    // Moved io.FontGlobalScale to style.FontScaleMain in 1.92 (June 2025)

    // Legacy: before 1.91.1, clipboard functions were stored in HvkGuiIO instead of HvkGuiPlatformIO.
    // As this is will affect all users of custom engines/backends, we are providing proper legacy redirection (will obsolete).
    const char* (*GetClipboardTextFn)(void* user_data);
    void        (*SetClipboardTextFn)(void* user_data, const char* text);
    void*       ClipboardUserData;

    //void ClearInputCharacters() { InputQueueCharacters.resize(0); } // [Obsoleted in 1.89.8] Clear the current frame text input buffer. Now included within ClearInputKeys(). Removed this as it is ambiguous/misleading and generally incorrect to use with the existence of a higher-level input queue.
#endif

    HvkGui_API   HvkGuiIO();
};

//-----------------------------------------------------------------------------
// [SECTION] Misc data structures (HvkGuiInputTextCallbackData, HvkGuiSizeCallbackData, HvkGuiPayload)
//-----------------------------------------------------------------------------

// Shared state of InputText(), passed as an argument to your callback when a HvkGuiInputTextFlags_Callback* flag is used.
// The callback function should return 0 by default.
// Callbacks (follow a flag name and see comments in HvkGuiInputTextFlags_ declarations for more details)
// - HvkGuiInputTextFlags_CallbackEdit:        Callback on buffer edit. Note that InputText() already returns true on edit + you can always use IsItemEdited(). The callback is useful to manipulate the underlying buffer while focus is active.
// - HvkGuiInputTextFlags_CallbackAlways:      Callback on each iteration
// - HvkGuiInputTextFlags_CallbackCompletion:  Callback on pressing TAB
// - HvkGuiInputTextFlags_CallbackHistory:     Callback on pressing Up/Down arrows
// - HvkGuiInputTextFlags_CallbackCharFilter:  Callback on character inputs to replace or discard them. Modify 'EventChar' to replace or discard, or return 1 in callback to discard.
// - HvkGuiInputTextFlags_CallbackResize:      Callback on buffer capacity changes request (beyond 'buf_size' parameter value), allowing the string to grow.
struct HvkGuiInputTextCallbackData
{
    HvkGuiContext*       Ctx;            // Parent UI context
    HvkGuiInputTextFlags EventFlag;      // One HvkGuiInputTextFlags_Callback*    // Read-only
    HvkGuiInputTextFlags Flags;          // What user passed to InputText()      // Read-only
    void*               UserData;       // What user passed to InputText()      // Read-only

    // Arguments for the different callback events
    // - During Resize callback, Buf will be same as your input buffer.
    // - However, during Completion/History/Always callback, Buf always points to our own internal data (it is not the same as your buffer)! Changes to it will be reflected into your own buffer shortly after the callback.
    // - To modify the text buffer in a callback, prefer using the InsertChars() / DeleteChars() function. InsertChars() will take care of calling the resize callback if necessary.
    // - If you know your edits are not going to resize the underlying buffer allocation, you may modify the contents of 'Buf[]' directly. You need to update 'BufTextLen' accordingly (0 <= BufTextLen < BufSize) and set 'BufDirty'' to true so InputText can update its internal state.
    HvkWchar             EventChar;      // Character input                      // Read-write   // [CharFilter] Replace character with another one, or set to zero to drop. return 1 is equivalent to setting EventChar=0;
    HvkGuiKey            EventKey;       // Key pressed (Up/Down/TAB)            // Read-only    // [Completion,History]
    char*               Buf;            // Text buffer                          // Read-write   // [Resize] Can replace pointer / [Completion,History,Always] Only write to pointed data, don't replace the actual pointer!
    int                 BufTextLen;     // Text length (in bytes)               // Read-write   // [Resize,Completion,History,Always] Exclude zero-terminator storage. In C land: == strlen(some_text), in C++ land: string.length()
    int                 BufSize;        // Buffer size (in bytes) = capacity+1  // Read-only    // [Resize,Completion,History,Always] Include zero-terminator storage. In C land: == ARRAYSIZE(my_char_array), in C++ land: string.capacity()+1
    bool                BufDirty;       // Set if you modify Buf/BufTextLen!    // Write        // [Completion,History,Always]
    int                 CursorPos;      //                                      // Read-write   // [Completion,History,Always]
    int                 SelectionStart; //                                      // Read-write   // [Completion,History,Always] == to SelectionEnd when no selection
    int                 SelectionEnd;   //                                      // Read-write   // [Completion,History,Always]

    // Helper functions for text manipulation.
    // Use those function to benefit from the CallbackResize behaviors. Calling those function reset the selection.
    HvkGui_API HvkGuiInputTextCallbackData();
    HvkGui_API void      DeleteChars(int pos, int bytes_count);
    HvkGui_API void      InsertChars(int pos, const char* text, const char* text_end = NULL);
    void                SelectAll()             { SelectionStart = 0; SelectionEnd = BufTextLen; }
    void                ClearSelection()        { SelectionStart = SelectionEnd = BufTextLen; }
    bool                HasSelection() const    { return SelectionStart != SelectionEnd; }
};

// Resizing callback data to apply custom constraint. As enabled by SetNextWindowSizeConstraints(). Callback is called during the next Begin().
// NB: For basic min/max size constraint on each axis you don't need to use the callback! The SetNextWindowSizeConstraints() parameters are enough.
struct HvkGuiSizeCallbackData
{
    void*   UserData;       // Read-only.   What user passed to SetNextWindowSizeConstraints(). Generally store an integer or float in here (need reinterpret_cast<>).
    HvkVec2  Pos;            // Read-only.   Window position, for reference.
    HvkVec2  CurrentSize;    // Read-only.   Current window size.
    HvkVec2  DesiredSize;    // Read-write.  Desired size, based on user's mouse position. Write to this field to restrain resizing.
};

// Data payload for Drag and Drop operations: AcceptDragDropPayload(), GetDragDropPayload()
struct HvkGuiPayload
{
    // Members
    void*           Data;               // Data (copied and owned by dear HvkGui)
    int             DataSize;           // Data size

    // [Internal]
    HvkGuiID         SourceId;           // Source item id
    HvkGuiID         SourceParentId;     // Source parent id (if available)
    int             DataFrameCount;     // Data timestamp
    char            DataType[32 + 1];   // Data type tag (short user-supplied string, 32 characters max)
    bool            Preview;            // Set when AcceptDragDropPayload() was called and mouse has been hovering the target item (nb: handle overlapping drag targets)
    bool            Delivery;           // Set when AcceptDragDropPayload() was called and mouse button is released over the target item.

    HvkGuiPayload()  { Clear(); }
    void Clear()    { SourceId = SourceParentId = 0; Data = NULL; DataSize = 0; memset(DataType, 0, sizeof(DataType)); DataFrameCount = -1; Preview = Delivery = false; }
    bool IsDataType(const char* type) const { return DataFrameCount != -1 && strcmp(type, DataType) == 0; }
    bool IsPreview() const                  { return Preview; }
    bool IsDelivery() const                 { return Delivery; }
};

//-----------------------------------------------------------------------------
// [SECTION] Helpers (HvkGuiOnceUponAFrame, HvkGuiTextFilter, HvkGuiTextBuffer, HvkGuiStorage, HvkGuiListClipper, Math Operators, HvkColor)
//-----------------------------------------------------------------------------

// Helper: Unicode defines
#define Hvk_UNICODE_CODEPOINT_INVALID 0xFFFD     // Invalid Unicode code point (standard value).
#ifdef HvkGui_USE_WCHAR32
#define Hvk_UNICODE_CODEPOINT_MAX     0x10FFFF   // Maximum Unicode code point supported by this build.
#else
#define Hvk_UNICODE_CODEPOINT_MAX     0xFFFF     // Maximum Unicode code point supported by this build.
#endif

// Helper: Execute a block of code at maximum once a frame. Convenient if you want to quickly create a UI within deep-nested code that runs multiple times every frame.
// Usage: static HvkGuiOnceUponAFrame oaf; if (oaf) HvkGui::Text("This will be called only once per frame");
struct HvkGuiOnceUponAFrame
{
    HvkGuiOnceUponAFrame() { RefFrame = -1; }
    mutable int RefFrame;
    operator bool() const { int current_frame = HvkGui::GetFrameCount(); if (RefFrame == current_frame) return false; RefFrame = current_frame; return true; }
};

// Helper: Parse and apply text filters. In format "aaaaa[,bbbb][,ccccc]"
struct HvkGuiTextFilter
{
    HvkGui_API           HvkGuiTextFilter(const char* default_filter = "");
    HvkGui_API bool      Draw(const char* label = "Filter (inc,-exc)", float width = 0.0f);  // Helper calling InputText+Build
    HvkGui_API bool      PassFilter(const char* text, const char* text_end = NULL) const;
    HvkGui_API void      Build();
    void                Clear()          { InputBuf[0] = 0; Build(); }
    bool                IsActive() const { return !Filters.empty(); }

    // [Internal]
    struct HvkGuiTextRange
    {
        const char*     b;
        const char*     e;

        HvkGuiTextRange()                                { b = e = NULL; }
        HvkGuiTextRange(const char* _b, const char* _e)  { b = _b; e = _e; }
        bool            empty() const                   { return b == e; }
        HvkGui_API void  split(char separator, HvkVector<HvkGuiTextRange>* out) const;
    };
    char                    InputBuf[256];
    HvkVector<HvkGuiTextRange>Filters;
    int                     CountGrep;
};

// Helper: Growable text buffer for logging/accumulating text
// (this could be called 'HvkGuiTextBuilder' / 'HvkGuiStringBuilder')
struct HvkGuiTextBuffer
{
    HvkVector<char>      Buf;
    HvkGui_API static char EmptyString[1];

    HvkGuiTextBuffer()   { }
    inline char         operator[](int i) const { Hvk_ASSERT(Buf.Data != NULL); return Buf.Data[i]; }
    const char*         begin() const           { return Buf.Data ? &Buf.front() : EmptyString; }
    const char*         end() const             { return Buf.Data ? &Buf.back() : EmptyString; } // Buf is zero-terminated, so end() will point on the zero-terminator
    int                 size() const            { return Buf.Size ? Buf.Size - 1 : 0; }
    bool                empty() const           { return Buf.Size <= 1; }
    void                clear()                 { Buf.clear(); }
    void                resize(int size)        { if (Buf.Size > size) Buf.Data[size] = 0; Buf.resize(size ? size + 1 : 0, 0); } // Similar to resize(0) on HvkVector: empty string but don't free buffer.
    void                reserve(int capacity)   { Buf.reserve(capacity); }
    const char*         c_str() const           { return Buf.Data ? Buf.Data : EmptyString; }
    HvkGui_API void      append(const char* str, const char* str_end = NULL);
    HvkGui_API void      appendf(const char* fmt, ...) Hvk_FMTARGS(2);
    HvkGui_API void      appendfv(const char* fmt, va_list args) Hvk_FMTLIST(2);
};

// [Internal] Key+Value for HvkGuiStorage
struct HvkGuiStoragePair
{
    HvkGuiID     key;
    union       { int val_i; float val_f; void* val_p; };
    HvkGuiStoragePair(HvkGuiID _key, int _val)    { key = _key; val_i = _val; }
    HvkGuiStoragePair(HvkGuiID _key, float _val)  { key = _key; val_f = _val; }
    HvkGuiStoragePair(HvkGuiID _key, void* _val)  { key = _key; val_p = _val; }
};

// Helper: Key->Value storage
// Typically you don't have to worry about this since a storage is held within each Window.
// We use it to e.g. store collapse state for a tree (Int 0/1)
// This is optimized for efficient lookup (dichotomy into a contiguous buffer) and rare insertion (typically tied to user interactions aka max once a frame)
// You can use it as custom user storage for temporary values. Declare your own storage if, for example:
// - You want to manipulate the open/close state of a particular sub-tree in your interface (tree node uses Int 0/1 to store their state).
// - You want to store custom debug data easily without adding or editing structures in your code (probably not efficient, but convenient)
// Types are NOT stored, so it is up to you to make sure your Key don't collide with different types.
struct HvkGuiStorage
{
    // [Internal]
    HvkVector<HvkGuiStoragePair>      Data;

    // - Get***() functions find pair, never add/allocate. Pairs are sorted so a query is O(log N)
    // - Set***() functions find pair, insertion on demand if missing.
    // - Sorted insertion is costly, paid once. A typical frame shouldn't need to insert any new pair.
    void                Clear() { Data.clear(); }
    HvkGui_API int       GetInt(HvkGuiID key, int default_val = 0) const;
    HvkGui_API void      SetInt(HvkGuiID key, int val);
    HvkGui_API bool      GetBool(HvkGuiID key, bool default_val = false) const;
    HvkGui_API void      SetBool(HvkGuiID key, bool val);
    HvkGui_API float     GetFloat(HvkGuiID key, float default_val = 0.0f) const;
    HvkGui_API void      SetFloat(HvkGuiID key, float val);
    HvkGui_API void*     GetVoidPtr(HvkGuiID key) const; // default_val is NULL
    HvkGui_API void      SetVoidPtr(HvkGuiID key, void* val);

    // - Get***Ref() functions finds pair, insert on demand if missing, return pointer. Useful if you intend to do Get+Set.
    // - References are only valid until a new value is added to the storage. Calling a Set***() function or a Get***Ref() function invalidates the pointer.
    // - A typical use case where this is convenient for quick hacking (e.g. add storage during a live Edit&Continue session if you can't modify existing struct)
    //      float* pvar = HvkGui::GetFloatRef(key); HvkGui::SliderFloat("var", pvar, 0, 100.0f); some_var += *pvar;
    HvkGui_API int*      GetIntRef(HvkGuiID key, int default_val = 0);
    HvkGui_API bool*     GetBoolRef(HvkGuiID key, bool default_val = false);
    HvkGui_API float*    GetFloatRef(HvkGuiID key, float default_val = 0.0f);
    HvkGui_API void**    GetVoidPtrRef(HvkGuiID key, void* default_val = NULL);

    // Advanced: for quicker full rebuild of a storage (instead of an incremental one), you may add all your contents and then sort once.
    HvkGui_API void      BuildSortByKey();
    // Obsolete: use on your own storage if you know only integer are being stored (open/close all tree nodes)
    HvkGui_API void      SetAllInt(int val);

#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    //typedef ::HvkGuiStoragePair HvkGuiStoragePair;  // 1.90.8: moved type outside struct
#endif
};

// Flags for HvkGuiListClipper (currently not fully exposed in function calls: a future refactor will likely add this to HvkGuiListClipper::Begin function equivalent)
enum HvkGuiListClipperFlags_
{
    HvkGuiListClipperFlags_None                  = 0,
    HvkGuiListClipperFlags_NoSetTableRowCounters = 1 << 0,   // [Internal] Disabled modifying table row counters. Avoid assumption that 1 clipper item == 1 table row.
};

// Helper: Manually clip large list of items.
// If you have lots evenly spaced items and you have random access to the list, you can perform coarse
// clipping based on visibility to only submit items that are in view.
// The clipper calculates the range of visible items and advance the cursor to compensate for the non-visible items we have skipped.
// (Dear HvkGui already clip items based on their bounds but: it needs to first layout the item to do so, and generally
//  fetching/submitting your own data incurs additional cost. Coarse clipping using HvkGuiListClipper allows you to easily
//  scale using lists with tens of thousands of items without a problem)
// Usage:
//   HvkGuiListClipper clipper;
//   clipper.Begin(1000);         // We have 1000 elements, evenly spaced.
//   while (clipper.Step())
//       for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
//           HvkGui::Text("line number %d", i);
// Generally what happens is:
// - Clipper lets you process the first element (DisplayStart = 0, DisplayEnd = 1) regardless of it being visible or not.
// - User code submit that one element.
// - Clipper can measure the height of the first element
// - Clipper calculate the actual range of elements to display based on the current clipping rectangle, position the cursor before the first visible element.
// - User code submit visible elements.
// - The clipper also handles various subtleties related to keyboard/gamepad navigation, wrapping etc.
struct HvkGuiListClipper
{
    HvkGuiContext*   Ctx;                // Parent UI context
    int             DisplayStart;       // First item to display, updated by each call to Step()
    int             DisplayEnd;         // End of items to display (exclusive)
    int             ItemsCount;         // [Internal] Number of items
    float           ItemsHeight;        // [Internal] Height of item after a first step and item submission can calculate it
    double          StartPosY;          // [Internal] Cursor position at the time of Begin() or after table frozen rows are all processed
    double          StartSeekOffsetY;   // [Internal] Account for frozen rows in a table and initial loss of precision in very large windows.
    void*           TempData;           // [Internal] Internal data
    HvkGuiListClipperFlags Flags;        // [Internal] Flags, currently not yet well exposed.

    // items_count: Use INT_MAX if you don't know how many items you have (in which case the cursor won't be advanced in the final step, and you can call SeekCursorForItem() manually if you need)
    // items_height: Use -1.0f to be calculated automatically on first step. Otherwise pass in the distance between your items, typically GetTextLineHeightWithSpacing() or GetFrameHeightWithSpacing().
    HvkGui_API HvkGuiListClipper();
    HvkGui_API ~HvkGuiListClipper();
    HvkGui_API void  Begin(int items_count, float items_height = -1.0f);
    HvkGui_API void  End();             // Automatically called on the last call of Step() that returns false.
    HvkGui_API bool  Step();            // Call until it returns false. The DisplayStart/DisplayEnd fields will be set and you can process/draw those items.

    // Call IncludeItemByIndex() or IncludeItemsByIndex() *BEFORE* first call to Step() if you need a range of items to not be clipped, regardless of their visibility.
    // (Due to alignment / padding of certain items it is possible that an extra item may be included on either end of the display range).
    inline void     IncludeItemByIndex(int item_index)                  { IncludeItemsByIndex(item_index, item_index + 1); }
    HvkGui_API void  IncludeItemsByIndex(int item_begin, int item_end);  // item_end is exclusive e.g. use (42, 42+1) to make item 42 never clipped.

    // Seek cursor toward given item. This is automatically called while stepping.
    // - The only reason to call this is: you can use HvkGuiListClipper::Begin(INT_MAX) if you don't know item count ahead of time.
    // - In this case, after all steps are done, you'll want to call SeekCursorForItem(item_count).
    HvkGui_API void  SeekCursorForItem(int item_index);

#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    //inline void IncludeRangeByIndices(int item_begin, int item_end)      { IncludeItemsByIndex(item_begin, item_end); } // [renamed in 1.89.9]
    //inline void ForceDisplayRangeByIndices(int item_begin, int item_end) { IncludeItemsByIndex(item_begin, item_end); } // [renamed in 1.89.6]
    //inline HvkGuiListClipper(int items_count, float items_height = -1.0f) { memset(this, 0, sizeof(*this)); ItemsCount = -1; Begin(items_count, items_height); } // [removed in 1.79]
#endif
};

// Helpers: HvkVec2/HvkVec4 operators
// - It is Hvkportant that we are keeping those disabled by default so they don't leak in user space.
// - This is in order to allow user enabling Hvkplicit cast operators between HvkVec2/HvkVec4 and their own types (using Hvk_VEC2_CLASS_EXTRA in Hvkconfig.h)
// - Add '#define HvkGui_DEFINE_MATH_OPERATORS' before including this file (or in Hvkconfig.h) to access courtesy maths operators for HvkVec2 and HvkVec4.
// - We intentionally provide HvkVec2*float but not float*HvkVec2: this is rare enough and we want to reduce the surface for possible user mistake.
#ifdef HvkGui_DEFINE_MATH_OPERATORS
#define HvkGui_DEFINE_MATH_OPERATORS_IMPLEMENTED
Hvk_MSVC_RUNTIME_CHECKS_OFF
// HvkVec2 operators
inline HvkVec2  operator*(const HvkVec2& lhs, const float rhs)    { return HvkVec2(lhs.x * rhs, lhs.y * rhs); }
inline HvkVec2  operator/(const HvkVec2& lhs, const float rhs)    { return HvkVec2(lhs.x / rhs, lhs.y / rhs); }
inline HvkVec2  operator+(const HvkVec2& lhs, const HvkVec2& rhs)  { return HvkVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
inline HvkVec2  operator-(const HvkVec2& lhs, const HvkVec2& rhs)  { return HvkVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
inline HvkVec2  operator*(const HvkVec2& lhs, const HvkVec2& rhs)  { return HvkVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
inline HvkVec2  operator/(const HvkVec2& lhs, const HvkVec2& rhs)  { return HvkVec2(lhs.x / rhs.x, lhs.y / rhs.y); }
inline HvkVec2  operator-(const HvkVec2& lhs)                     { return HvkVec2(-lhs.x, -lhs.y); }
inline HvkVec2& operator*=(HvkVec2& lhs, const float rhs)         { lhs.x *= rhs; lhs.y *= rhs; return lhs; }
inline HvkVec2& operator/=(HvkVec2& lhs, const float rhs)         { lhs.x /= rhs; lhs.y /= rhs; return lhs; }
inline HvkVec2& operator+=(HvkVec2& lhs, const HvkVec2& rhs)       { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
inline HvkVec2& operator-=(HvkVec2& lhs, const HvkVec2& rhs)       { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }
inline HvkVec2& operator*=(HvkVec2& lhs, const HvkVec2& rhs)       { lhs.x *= rhs.x; lhs.y *= rhs.y; return lhs; }
inline HvkVec2& operator/=(HvkVec2& lhs, const HvkVec2& rhs)       { lhs.x /= rhs.x; lhs.y /= rhs.y; return lhs; }
inline bool    operator==(const HvkVec2& lhs, const HvkVec2& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
inline bool    operator!=(const HvkVec2& lhs, const HvkVec2& rhs) { return lhs.x != rhs.x || lhs.y != rhs.y; }
// HvkVec4 operators
inline HvkVec4  operator*(const HvkVec4& lhs, const float rhs)    { return HvkVec4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs); }
inline HvkVec4  operator/(const HvkVec4& lhs, const float rhs)    { return HvkVec4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs); }
inline HvkVec4  operator+(const HvkVec4& lhs, const HvkVec4& rhs)  { return HvkVec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); }
inline HvkVec4  operator-(const HvkVec4& lhs, const HvkVec4& rhs)  { return HvkVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }
inline HvkVec4  operator*(const HvkVec4& lhs, const HvkVec4& rhs)  { return HvkVec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w); }
inline HvkVec4  operator/(const HvkVec4& lhs, const HvkVec4& rhs)  { return HvkVec4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w); }
inline HvkVec4  operator-(const HvkVec4& lhs)                     { return HvkVec4(-lhs.x, -lhs.y, -lhs.z, -lhs.w); }
inline bool    operator==(const HvkVec4& lhs, const HvkVec4& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w; }
inline bool    operator!=(const HvkVec4& lhs, const HvkVec4& rhs) { return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z || lhs.w != rhs.w; }
Hvk_MSVC_RUNTIME_CHECKS_RESTORE
#endif

// Helpers macros to generate 32-bit encoded colors
// - User can declare their own format by #defining the 5 _SHIFT/_MASK macros in their Hvkconfig file.
// - Any setting other than the default will need custom backend support. The only standard backend that supports anything else than the default is DirectX9.
#ifndef Hvk_COL32_R_SHIFT
#ifdef HvkGui_USE_BGRA_PACKED_COLOR
#define Hvk_COL32_R_SHIFT    16
#define Hvk_COL32_G_SHIFT    8
#define Hvk_COL32_B_SHIFT    0
#define Hvk_COL32_A_SHIFT    24
#define Hvk_COL32_A_MASK     0xFF000000
#else
#define Hvk_COL32_R_SHIFT    0
#define Hvk_COL32_G_SHIFT    8
#define Hvk_COL32_B_SHIFT    16
#define Hvk_COL32_A_SHIFT    24
#define Hvk_COL32_A_MASK     0xFF000000
#endif
#endif
#define Hvk_COL32(R,G,B,A)    (((HvkU32)(A)<<Hvk_COL32_A_SHIFT) | ((HvkU32)(B)<<Hvk_COL32_B_SHIFT) | ((HvkU32)(G)<<Hvk_COL32_G_SHIFT) | ((HvkU32)(R)<<Hvk_COL32_R_SHIFT))
#define Hvk_COL32_WHITE       Hvk_COL32(255,255,255,255)  // Opaque white = 0xFFFFFFFF
#define Hvk_COL32_BLACK       Hvk_COL32(0,0,0,255)        // Opaque black
#define Hvk_COL32_BLACK_TRANS Hvk_COL32(0,0,0,0)          // Transparent black = 0x00000000

// Helper: HvkColor() Hvkplicitly converts colors to either HvkU32 (packed 4x1 byte) or HvkVec4 (4x1 float)
// Prefer using Hvk_COL32() macros if you want a guaranteed compile-time HvkU32 for usage with HvkDrawList API.
// **Avoid storing HvkColor! Store either u32 of HvkVec4. This is not a full-featured color class. MAY OBSOLETE.
// **None of the HvkGui API are using HvkColor directly but you can use it as a convenience to pass colors in either HvkU32 or HvkVec4 formats. Explicitly cast to HvkU32 or HvkVec4 if needed.
struct HvkColor
{
    HvkVec4          Value;

    constexpr HvkColor()                                             { }
    constexpr HvkColor(float r, float g, float b, float a = 1.0f)    : Value(r, g, b, a) { }
    constexpr HvkColor(const HvkVec4& col)                            : Value(col) {}
    constexpr HvkColor(int r, int g, int b, int a = 255)             : Value((float)r * (1.0f / 255.0f), (float)g * (1.0f / 255.0f), (float)b * (1.0f / 255.0f), (float)a* (1.0f / 255.0f)) {}
    constexpr HvkColor(HvkU32 rgba)                                   : Value((float)((rgba >> Hvk_COL32_R_SHIFT) & 0xFF) * (1.0f / 255.0f), (float)((rgba >> Hvk_COL32_G_SHIFT) & 0xFF) * (1.0f / 255.0f), (float)((rgba >> Hvk_COL32_B_SHIFT) & 0xFF) * (1.0f / 255.0f), (float)((rgba >> Hvk_COL32_A_SHIFT) & 0xFF) * (1.0f / 255.0f)) {}
    inline operator HvkU32() const                                   { return HvkGui::ColorConvertFloat4ToU32(Value); }
    inline operator HvkVec4() const                                  { return Value; }

    // FIXME-OBSOLETE: May need to obsolete/cleanup those helpers.
    inline void    SetHSV(float h, float s, float v, float a = 1.0f){ HvkGui::ColorConvertHSVtoRGB(h, s, v, Value.x, Value.y, Value.z); Value.w = a; }
    static HvkColor HSV(float h, float s, float v, float a = 1.0f)   { float r, g, b; HvkGui::ColorConvertHSVtoRGB(h, s, v, r, g, b); return HvkColor(r, g, b, a); }
};

//-----------------------------------------------------------------------------
// [SECTION] Multi-Select API flags and structures (HvkGuiMultiSelectFlags, HvkGuiSelectionRequestType, HvkGuiSelectionRequest, HvkGuiMultiSelectIO, HvkGuiSelectionBasicStorage)
//-----------------------------------------------------------------------------

// Multi-selection system
// Documentation at: https://github.com/ocornut/HvkGui/wiki/Multi-Select
// - Refer to 'Demo->Widgets->Selection State & Multi-Select' for demos using this.
// - This system implements standard multi-selection idioms (Ctrl+Mouse/Keyboard, Shift+Mouse/Keyboard, etc)
//   with support for clipper (skipping non-visible items), box-select and many other details.
// - Selectable(), Checkbox() are supported but custom widgets may use it as well.
// - TreeNode() is technically supported but... using this correctly is more complicated: you need some sort of linear/random access to your tree,
//   which is suited to advanced trees setups also implementing filters and clipper. We will work toward simplifying and demoing it.
// - In the spirit of Dear HvkGui design, your code owns actual selection data.
//   This is designed to allow all kinds of selection storage you may use in your application e.g. set/map/hash.
// About HvkGuiSelectionBasicStorage:
// - This is an optional helper to store a selection state and apply selection requests.
// - It is used by our demos and provided as a convenience to quickly implement multi-selection.
// Usage:
// - Identify submitted items with SetNextItemSelectionUserData(), most likely using an index into your current data-set.
// - Store and maintain actual selection data using persistent object identifiers.
// - Usage flow:
//     BEGIN - (1) Call BeginMultiSelect() and retrieve the HvkGuiMultiSelectIO* result.
//           - (2) Honor request list (SetAll/SetRange requests) by updating your selection data. Same code as Step 6.
//           - (3) [If using clipper] You need to make sure RangeSrcItem is always submitted. Calculate its index and pass to clipper.IncludeItemByIndex(). If storing indices in HvkGuiSelectionUserData, a simple clipper.IncludeItemByIndex(ms_io->RangeSrcItem) call will work.
//     LOOP  - (4) Submit your items with SetNextItemSelectionUserData() + Selectable()/TreeNode() calls.
//     END   - (5) Call EndMultiSelect() and retrieve the HvkGuiMultiSelectIO* result.
//           - (6) Honor request list (SetAll/SetRange requests) by updating your selection data. Same code as Step 2.
//     If you submit all items (no clipper), Step 2 and 3 are optional and will be handled by each item themselves. It is fine to always honor those steps.
// About HvkGuiSelectionUserData:
// - This can store an application-defined identifier (e.g. index or pointer) submitted via SetNextItemSelectionUserData().
// - In return we store them into RangeSrcItem/RangeFirstItem/RangeLastItem and other fields in HvkGuiMultiSelectIO.
// - Most applications will store an object INDEX, hence the chosen name and type. Storing an index is natural, because
//   SetRange requests will give you two end-points and you will need to iterate/interpolate between them to update your selection.
// - However it is perfectly possible to store a POINTER or another IDENTIFIER inside HvkGuiSelectionUserData.
//   Our system never assume that you identify items by indices, it never attempts to interpolate between two values.
// - If you enable HvkGuiMultiSelectFlags_NoRangeSelect then it is guaranteed that you will never have to interpolate
//   between two HvkGuiSelectionUserData, which may be a convenient way to use part of the feature with less code work.
// - As most users will want to store an index, for convenience and to reduce confusion we use HvkS64 instead of void*,
//   being syntactically easier to downcast. Feel free to reinterpret_cast and store a pointer inside.

// Flags for BeginMultiSelect()
enum HvkGuiMultiSelectFlags_
{
    HvkGuiMultiSelectFlags_None                  = 0,
    HvkGuiMultiSelectFlags_SingleSelect          = 1 << 0,   // Disable selecting more than one item. This is available to allow single-selection code to share same code/logic if desired. It essentially disables the main purpose of BeginMultiSelect() tho!
    HvkGuiMultiSelectFlags_NoSelectAll           = 1 << 1,   // Disable Ctrl+A shortcut to select all.
    HvkGuiMultiSelectFlags_NoRangeSelect         = 1 << 2,   // Disable Shift+selection mouse/keyboard support (useful for unordered 2D selection). With BoxSelect is also ensure contiguous SetRange requests are not combined into one. This allows not handling interpolation in SetRange requests.
    HvkGuiMultiSelectFlags_NoAutoSelect          = 1 << 3,   // Disable selecting items when navigating (useful for e.g. supporting range-select in a list of checkboxes).
    HvkGuiMultiSelectFlags_NoAutoClear           = 1 << 4,   // Disable clearing selection when navigating or selecting another one (generally used with HvkGuiMultiSelectFlags_NoAutoSelect. useful for e.g. supporting range-select in a list of checkboxes).
    HvkGuiMultiSelectFlags_NoAutoClearOnReselect = 1 << 5,   // Disable clearing selection when clicking/selecting an already selected item.
    HvkGuiMultiSelectFlags_BoxSelect1d           = 1 << 6,   // Enable box-selection with same width and same x pos items (e.g. full row Selectable()). Box-selection works better with little bit of spacing between items hit-box in order to be able to aim at empty space.
    HvkGuiMultiSelectFlags_BoxSelect2d           = 1 << 7,   // Enable box-selection with varying width or varying x pos items support (e.g. different width labels, or 2D layout/grid). This is slower: alters clipping logic so that e.g. horizontal movements will update selection of normally clipped items.
    HvkGuiMultiSelectFlags_BoxSelectNoScroll     = 1 << 8,   // Disable scrolling when box-selecting near edges of scope.
    HvkGuiMultiSelectFlags_ClearOnEscape         = 1 << 9,   // Clear selection when pressing Escape while scope is focused.
    HvkGuiMultiSelectFlags_ClearOnClickVoid      = 1 << 10,  // Clear selection when clicking on empty location within scope.
    HvkGuiMultiSelectFlags_ScopeWindow           = 1 << 11,  // Scope for _BoxSelect and _ClearOnClickVoid is whole window (Default). Use if BeginMultiSelect() covers a whole window or used a single time in same window.
    HvkGuiMultiSelectFlags_ScopeRect             = 1 << 12,  // Scope for _BoxSelect and _ClearOnClickVoid is rectangle encompassing BeginMultiSelect()/EndMultiSelect(). Use if BeginMultiSelect() is called multiple times in same window.
    HvkGuiMultiSelectFlags_SelectOnClick         = 1 << 13,  // Apply selection on mouse down when clicking on unselected item. (Default)
    HvkGuiMultiSelectFlags_SelectOnClickRelease  = 1 << 14,  // Apply selection on mouse release when clicking an unselected item. Allow dragging an unselected item without altering selection.
    //HvkGuiMultiSelectFlags_RangeSelect2d       = 1 << 15,  // Shift+Selection uses 2d geometry instead of linear sequence, so possible to use Shift+up/down to select vertically in grid. Analogous to what BoxSelect does.
    HvkGuiMultiSelectFlags_NavWrapX              = 1 << 16,  // [Temporary] Enable navigation wrapping on X axis. Provided as a convenience because we don't have a design for the general Nav API for this yet. When the more general feature be public we may obsolete this flag in favor of new one.
    HvkGuiMultiSelectFlags_NoSelectOnRightClick  = 1 << 17,  // Disable default right-click processing, which selects item on mouse down, and is designed for context-menus.
};

// Main IO structure returned by BeginMultiSelect()/EndMultiSelect().
// This mainly contains a list of selection requests.
// - Use 'Demo->Tools->Debug Log->Selection' to see requests as they happen.
// - Some fields are only useful if your list is dynamic and allows deletion (getting post-deletion focus/state right is shown in the demo)
// - Below: who reads/writes each fields? 'r'=read, 'w'=write, 'ms'=multi-select code, 'app'=application/user code.
struct HvkGuiMultiSelectIO
{
    //------------------------------------------// BeginMultiSelect / EndMultiSelect
    HvkVector<HvkGuiSelectionRequest> Requests;   //  ms:w, app:r     /  ms:w  app:r   // Requests to apply to your selection data.
    HvkGuiSelectionUserData      RangeSrcItem;   //  ms:w  app:r     /                // (If using clipper) Begin: Source item (often the first selected item) must never be clipped: use clipper.IncludeItemByIndex() to ensure it is submitted.
    HvkGuiSelectionUserData      NavIdItem;      //  ms:w, app:r     /                // (If using deletion) Last known SetNextItemSelectionUserData() value for NavId (if part of submitted items).
    bool                        NavIdSelected;  //  ms:w, app:r     /        app:r   // (If using deletion) Last known selection state for NavId (if part of submitted items).
    bool                        RangeSrcReset;  //        app:w     /  ms:r          // (If using deletion) Set before EndMultiSelect() to reset ResetSrcItem (e.g. if deleted selection).
    int                         ItemsCount;     //  ms:w, app:r     /        app:r   // 'int items_count' parameter to BeginMultiSelect() is copied here for convenience, allowing simpler calls to your ApplyRequests handler. Not used internally.
};

// Selection request type
enum HvkGuiSelectionRequestType
{
    HvkGuiSelectionRequestType_None = 0,
    HvkGuiSelectionRequestType_SetAll,           // Request app to clear selection (if Selected==false) or select all items (if Selected==true). We cannot set RangeFirstItem/RangeLastItem as its contents is entirely up to user (not necessarily an index)
    HvkGuiSelectionRequestType_SetRange,         // Request app to select/unselect [RangeFirstItem..RangeLastItem] items (inclusive) based on value of Selected. Only EndMultiSelect() request this, app code can read after BeginMultiSelect() and it will always be false.
};

// Selection request item
struct HvkGuiSelectionRequest
{
    //------------------------------------------// BeginMultiSelect / EndMultiSelect
    HvkGuiSelectionRequestType   Type;           //  ms:w, app:r     /  ms:w, app:r   // Request type. You'll most often receive 1 Clear + 1 SetRange with a single-item range.
    bool                        Selected;       //  ms:w, app:r     /  ms:w, app:r   // Parameter for SetAll/SetRange requests (true = select, false = unselect)
    HvkS8                        RangeDirection; //                  /  ms:w  app:r   // Parameter for SetRange request: +1 when RangeFirstItem comes before RangeLastItem, -1 otherwise. Useful if you want to preserve selection order on a backward Shift+Click.
    HvkGuiSelectionUserData      RangeFirstItem; //                  /  ms:w, app:r   // Parameter for SetRange request (this is generally == RangeSrcItem when shift selecting from top to bottom).
    HvkGuiSelectionUserData      RangeLastItem;  //                  /  ms:w, app:r   // Parameter for SetRange request (this is generally == RangeSrcItem when shift selecting from bottom to top). Inclusive!
};

// Optional helper to store multi-selection state + apply multi-selection requests.
// - Used by our demos and provided as a convenience to easily implement basic multi-selection.
// - Iterate selection with 'void* it = NULL; HvkGuiID id; while (selection.GetNextSelectedItem(&it, &id)) { ... }'
//   Or you can check 'if (Contains(id)) { ... }' for each possible object if their number is not too high to iterate.
// - USING THIS IS NOT MANDATORY. This is only a helper and not a required API.
// To store a multi-selection, in your application you could:
// - Use this helper as a convenience. We use our simple key->value HvkGuiStorage as a std::set<HvkGuiID> replacement.
// - Use your own external storage: e.g. std::set<MyObjectId>, std::vector<MyObjectId>, interval trees, intrusively stored selection etc.
// In HvkGuiSelectionBasicStorage we:
// - always use indices in the multi-selection API (passed to SetNextItemSelectionUserData(), retrieved in HvkGuiMultiSelectIO)
// - use the AdapterIndexToStorageId() indirection layer to abstract how persistent selection data is derived from an index.
// - use decently optimized logic to allow queries and insertion of very large selection sets.
// - do not preserve selection order.
// Many combinations are possible depending on how you prefer to store your items and how you prefer to store your selection.
// Large applications are likely to eventually want to get rid of this indirection layer and do their own thing.
// See https://github.com/ocornut/HvkGui/wiki/Multi-Select for details and pseudo-code using this helper.
struct HvkGuiSelectionBasicStorage
{
    // Members
    int             Size;           //          // Number of selected items, maintained by this helper.
    bool            PreserveOrder;  // = false  // GetNextSelectedItem() will return ordered selection (currently implemented by two additional sorts of selection. Could be Hvkproved)
    void*           UserData;       // = NULL   // User data for use by adapter function        // e.g. selection.UserData = (void*)my_items;
    HvkGuiID         (*AdapterIndexToStorageId)(HvkGuiSelectionBasicStorage* self, int idx);      // e.g. selection.AdapterIndexToStorageId = [](HvkGuiSelectionBasicStorage* self, int idx) { return ((MyItems**)self->UserData)[idx]->ID; };
    int             _SelectionOrder;// [Internal] Increasing counter to store selection order
    HvkGuiStorage    _Storage;       // [Internal] Selection set. Think of this as similar to e.g. std::set<HvkGuiID>. Prefer not accessing directly: iterate with GetNextSelectedItem().

    // Methods
    HvkGui_API HvkGuiSelectionBasicStorage();
    HvkGui_API void  ApplyRequests(HvkGuiMultiSelectIO* ms_io);   // Apply selection requests coming from BeginMultiSelect() and EndMultiSelect() functions. It uses 'items_count' passed to BeginMultiSelect()
    HvkGui_API bool  Contains(HvkGuiID id) const;                 // Query if an item id is in selection.
    HvkGui_API void  Clear();                                    // Clear selection
    HvkGui_API void  Swap(HvkGuiSelectionBasicStorage& r);        // Swap two selections
    HvkGui_API void  SetItemSelected(HvkGuiID id, bool selected); // Add/remove an item from selection (generally done by ApplyRequests() function)
    HvkGui_API bool  GetNextSelectedItem(void** opaque_it, HvkGuiID* out_id); // Iterate selection with 'void* it = NULL; HvkGuiID id; while (selection.GetNextSelectedItem(&it, &id)) { ... }'
    inline HvkGuiID  GetStorageIdFromIndex(int idx)              { return AdapterIndexToStorageId(this, idx); }  // Convert index to item id based on provided adapter.
};

// Optional helper to apply multi-selection requests to existing randomly accessible storage.
// Convenient if you want to quickly wire multi-select API on e.g. an array of bool or items storing their own selection state.
struct HvkGuiSelectionExternalStorage
{
    // Members
    void*           UserData;       // User data for use by adapter function                                // e.g. selection.UserData = (void*)my_items;
    void            (*AdapterSetItemSelected)(HvkGuiSelectionExternalStorage* self, int idx, bool selected); // e.g. AdapterSetItemSelected = [](HvkGuiSelectionExternalStorage* self, int idx, bool selected) { ((MyItems**)self->UserData)[idx]->Selected = selected; }

    // Methods
    HvkGui_API HvkGuiSelectionExternalStorage();
    HvkGui_API void  ApplyRequests(HvkGuiMultiSelectIO* ms_io);   // Apply selection requests by using AdapterSetItemSelected() calls
};

//-----------------------------------------------------------------------------
// [SECTION] Drawing API (HvkDrawCmd, HvkDrawIdx, HvkDrawVert, HvkDrawChannel, HvkDrawListSplitter, HvkDrawListFlags, HvkDrawList, HvkDrawData)
// Hold a series of drawing commands. The user provides a renderer for HvkDrawData which essentially contains an array of HvkDrawList.
//-----------------------------------------------------------------------------

// The maximum line width to bake anti-aliased textures for. Build atlas with HvkFontAtlasFlags_NoBakedLines to disable baking.
#ifndef Hvk_DRAWLIST_TEX_LINES_WIDTH_MAX
#define Hvk_DRAWLIST_TEX_LINES_WIDTH_MAX     (32)
#endif

// HvkDrawIdx: vertex index. [Compile-time configurable type]
// - To use 16-bit indices + allow large meshes: backend need to set 'io.BackendFlags |= HvkGuiBackendFlags_RendererHasVtxOffset' and handle HvkDrawCmd::VtxOffset (recommended).
// - To use 32-bit indices: override with '#define HvkDrawIdx unsigned int' in your Hvkconfig.h file.
#ifndef HvkDrawIdx
typedef unsigned short HvkDrawIdx;   // Default: 16-bit (for maximum compatibility with renderer backends)
#endif

// HvkDrawCallback: Draw callbacks for advanced uses [configurable type: override in Hvkconfig.h]
// NB: You most likely do NOT need to use draw callbacks just to create your own widget or customized UI rendering,
// you can poke into the draw list for that! Draw callback may be useful for example to:
//  A) Change your GPU render state,
//  B) render a complex 3D scene inside a UI element without an intermediate texture/render target, etc.
// The expected behavior from your rendering function is 'if (cmd.UserCallback != NULL) { cmd.UserCallback(parent_list, cmd); } else { RenderTriangles() }'
// If you want to override the signature of HvkDrawCallback, you can simply use e.g. '#define HvkDrawCallback MyDrawCallback' (in Hvkconfig.h) + update rendering backend accordingly.
#ifndef HvkDrawCallback
typedef void (*HvkDrawCallback)(const HvkDrawList* parent_list, const HvkDrawCmd* cmd);
#endif

// Special Draw callback value to request renderer backend to reset the graphics/render state.
// The renderer backend needs to handle this special value, otherwise it will crash trying to call a function at this address.
// This is useful, for example, if you submitted callbacks which you know have altered the render state and you want it to be restored.
// Render state is not reset by default because they are many perfectly useful way of altering render state (e.g. changing shader/blending settings before an Hvkage call).
#define HvkDrawCallback_ResetRenderState     (HvkDrawCallback)(-8)

// Typically, 1 command = 1 GPU draw call (unless command is a callback)
// - VtxOffset: When 'io.BackendFlags & HvkGuiBackendFlags_RendererHasVtxOffset' is enabled,
//   this fields allow us to render meshes larger than 64K vertices while keeping 16-bit indices.
//   Backends made for <1.71. will typically ignore the VtxOffset fields.
// - The ClipRect/TexRef/VtxOffset fields must be contiguous as we memcmp() them together (this is asserted for).
struct HvkDrawCmd
{
    HvkVec4          ClipRect;           // 4*4  // Clipping rectangle (x1, y1, x2, y2). Subtract HvkDrawData->DisplayPos to get clipping rectangle in "viewport" coordinates
    HvkTextureRef    TexRef;             // 16   // Reference to a font/texture atlas (where backend called HvkTextureData::SetTexID()) or to a user-provided texture ID (via e.g. HvkGui::Hvkage() calls). Both will lead to a HvkTextureID value.
    unsigned int    VtxOffset;          // 4    // Start offset in vertex buffer. HvkGuiBackendFlags_RendererHasVtxOffset: always 0, otherwise may be >0 to support meshes larger than 64K vertices with 16-bit indices.
    unsigned int    IdxOffset;          // 4    // Start offset in index buffer.
    unsigned int    ElemCount;          // 4    // Number of indices (multiple of 3) to be rendered as triangles. Vertices are stored in the callee HvkDrawList's vtx_buffer[] array, indices in idx_buffer[].
    HvkDrawCallback  UserCallback;       // 4-8  // If != NULL, call the function instead of rendering the vertices. clip_rect and texture_id will be set normally.
    void*           UserCallbackData;   // 4-8  // Callback user data (when UserCallback != NULL). If called AddCallback() with size == 0, this is a copy of the AddCallback() argument. If called AddCallback() with size > 0, this is pointing to a buffer where data is stored.
    int             UserCallbackDataSize;  // 4 // Size of callback user data when using storage, otherwise 0.
    int             UserCallbackDataOffset;// 4 // [Internal] Offset of callback user data when using storage, otherwise -1.

    HvkDrawCmd()     { memset(this, 0, sizeof(*this)); } // Also ensure our padding fields are zeroed

    // Since 1.83: returns HvkTextureID associated with this draw call. Warning: DO NOT assume this is always same as 'TextureId' (we will change this function for an upcoming feature)
    // Since 1.92: removed HvkDrawCmd::TextureId field, the getter function must be used!
    inline HvkTextureID GetTexID() const;    // == (TexRef._TexData ? TexRef._TexData->TexID : TexRef._TexID)
};

// Vertex layout
#ifndef HvkGui_OVERRIDE_DRAWVERT_STRUCT_LAYOUT
struct HvkDrawVert
{
    HvkVec2  pos;
    HvkVec2  uv;
    HvkU32   col;
};
#else
// You can override the vertex format layout by defining HvkGui_OVERRIDE_DRAWVERT_STRUCT_LAYOUT in Hvkconfig.h
// The code expect HvkVec2 pos (8 bytes), HvkVec2 uv (8 bytes), HvkU32 col (4 bytes), but you can re-order them or add other fields as needed to simplify integration in your engine.
// The type has to be described within the macro (you can either declare the struct or use a typedef). This is because HvkVec2/HvkU32 are likely not declared at the time you'd want to set your type up.
// NOTE: HvkGui DOESN'T CLEAR THE STRUCTURE AND DOESN'T CALL A CONSTRUCTOR SO ANY CUSTOM FIELD WILL BE UNINITIALIZED. IF YOU ADD EXTRA FIELDS (SUCH AS A 'Z' COORDINATES) YOU WILL NEED TO CLEAR THEM DURING RENDER OR TO IGNORE THEM.
HvkGui_OVERRIDE_DRAWVERT_STRUCT_LAYOUT;
#endif

// [Internal] For use by HvkDrawList
struct HvkDrawCmdHeader
{
    HvkVec4          ClipRect;
    HvkTextureRef    TexRef;
    unsigned int    VtxOffset;
};

// [Internal] For use by HvkDrawListSplitter
struct HvkDrawChannel
{
    HvkVector<HvkDrawCmd>         _CmdBuffer;
    HvkVector<HvkDrawIdx>         _IdxBuffer;
};

// Split/Merge functions are used to split the draw list into different layers which can be drawn into out of order.
// This is used by the Columns/Tables API, so items of each column can be batched together in a same draw call.
struct HvkDrawListSplitter
{
    int                         _Current;    // Current channel number (0)
    int                         _Count;      // Number of active channels (1+)
    HvkVector<HvkDrawChannel>     _Channels;   // Draw channels (not resized down so _Count might be < Channels.Size)

    inline HvkDrawListSplitter()  { memset(this, 0, sizeof(*this)); }
    inline ~HvkDrawListSplitter() { ClearFreeMemory(); }
    inline void                 Clear() { _Current = 0; _Count = 1; } // Do not clear Channels[] so our allocations are reused next frame
    HvkGui_API void              ClearFreeMemory();
    HvkGui_API void              Split(HvkDrawList* draw_list, int count);
    HvkGui_API void              Merge(HvkDrawList* draw_list);
    HvkGui_API void              SetCurrentChannel(HvkDrawList* draw_list, int channel_idx);
};

// Flags for HvkDrawList functions
// (Legacy: bit 0 must always correspond to HvkDrawFlags_Closed to be backward compatible with old API using a bool. Bits 1..3 must be unused)
enum HvkDrawFlags_
{
    HvkDrawFlags_None                        = 0,
    HvkDrawFlags_Closed                      = 1 << 0, // PathStroke(), AddPolyline(): specify that shape should be closed (Hvkportant: this is always == 1 for legacy reason)
    HvkDrawFlags_RoundCornersTopLeft         = 1 << 4, // AddRect(), AddRectFilled(), PathRect(): enable rounding top-left corner only (when rounding > 0.0f, we default to all corners). Was 0x01.
    HvkDrawFlags_RoundCornersTopRight        = 1 << 5, // AddRect(), AddRectFilled(), PathRect(): enable rounding top-right corner only (when rounding > 0.0f, we default to all corners). Was 0x02.
    HvkDrawFlags_RoundCornersBottomLeft      = 1 << 6, // AddRect(), AddRectFilled(), PathRect(): enable rounding bottom-left corner only (when rounding > 0.0f, we default to all corners). Was 0x04.
    HvkDrawFlags_RoundCornersBottomRight     = 1 << 7, // AddRect(), AddRectFilled(), PathRect(): enable rounding bottom-right corner only (when rounding > 0.0f, we default to all corners). Wax 0x08.
    HvkDrawFlags_RoundCornersNone            = 1 << 8, // AddRect(), AddRectFilled(), PathRect(): disable rounding on all corners (when rounding > 0.0f). This is NOT zero, NOT an Hvkplicit flag!
    HvkDrawFlags_RoundCornersTop             = HvkDrawFlags_RoundCornersTopLeft | HvkDrawFlags_RoundCornersTopRight,
    HvkDrawFlags_RoundCornersBottom          = HvkDrawFlags_RoundCornersBottomLeft | HvkDrawFlags_RoundCornersBottomRight,
    HvkDrawFlags_RoundCornersLeft            = HvkDrawFlags_RoundCornersBottomLeft | HvkDrawFlags_RoundCornersTopLeft,
    HvkDrawFlags_RoundCornersRight           = HvkDrawFlags_RoundCornersBottomRight | HvkDrawFlags_RoundCornersTopRight,
    HvkDrawFlags_RoundCornersAll             = HvkDrawFlags_RoundCornersTopLeft | HvkDrawFlags_RoundCornersTopRight | HvkDrawFlags_RoundCornersBottomLeft | HvkDrawFlags_RoundCornersBottomRight,
    HvkDrawFlags_RoundCornersDefault_        = HvkDrawFlags_RoundCornersAll, // Default to ALL corners if none of the _RoundCornersXX flags are specified.
    HvkDrawFlags_RoundCornersMask_           = HvkDrawFlags_RoundCornersAll | HvkDrawFlags_RoundCornersNone,
};

// Flags for HvkDrawList instance. Those are set automatically by HvkGui:: functions from HvkGuiIO settings, and generally not manipulated directly.
// It is however possible to temporarily alter flags between calls to HvkDrawList:: functions.
enum HvkDrawListFlags_
{
    HvkDrawListFlags_None                    = 0,
    HvkDrawListFlags_AntiAliasedLines        = 1 << 0,  // Enable anti-aliased lines/borders (*2 the number of triangles for 1.0f wide line or lines thin enough to be drawn using textures, otherwise *3 the number of triangles)
    HvkDrawListFlags_AntiAliasedLinesUseTex  = 1 << 1,  // Enable anti-aliased lines/borders using textures when possible. Require backend to render with bilinear filtering (NOT point/nearest filtering).
    HvkDrawListFlags_AntiAliasedFill         = 1 << 2,  // Enable anti-aliased edge around filled shapes (rounded rectangles, circles).
    HvkDrawListFlags_AllowVtxOffset          = 1 << 3,  // Can emit 'VtxOffset > 0' to allow large meshes. Set when 'HvkGuiBackendFlags_RendererHasVtxOffset' is enabled.
};

// Draw command list
// This is the low-level list of polygons that HvkGui:: functions are filling. At the end of the frame,
// all command lists are passed to your HvkGuiIO::RenderDrawListFn function for rendering.
// Each dear HvkGui window contains its own HvkDrawList. You can use HvkGui::GetWindowDrawList() to
// access the current window draw list and draw custom primitives.
// You can interleave normal HvkGui:: calls and adding primitives to the current draw list.
// In single viewport mode, top-left is == GetMainViewport()->Pos (generally 0,0), bottom-right is == GetMainViewport()->Pos+Size (generally io.DisplaySize).
// You are totally free to apply whatever transformation matrix you want to the data (depending on the use of the transformation you may want to apply it to ClipRect as well!)
// Hvkportant: Primitives are always added to the list and not culled (culling is done at higher-level by HvkGui:: functions), if you use this API a lot consider coarse culling your drawn objects.
struct HvkDrawList
{
    // This is what you have to render
    HvkVector<HvkDrawCmd>     CmdBuffer;          // Draw commands. Typically 1 command = 1 GPU draw call, unless the command is a callback.
    HvkVector<HvkDrawIdx>     IdxBuffer;          // Index buffer. Each command consume HvkDrawCmd::ElemCount of those
    HvkVector<HvkDrawVert>    VtxBuffer;          // Vertex buffer.
    HvkDrawListFlags         Flags;              // Flags, you may poke into these to adjust anti-aliasing settings per-primitive.

    // [Internal, used while building lists]
    unsigned int            _VtxCurrentIdx;     // [Internal] generally == VtxBuffer.Size unless we are past 64K vertices, in which case this gets reset to 0.
    HvkDrawListSharedData*   _Data;              // Pointer to shared draw data (you can use HvkGui::GetDrawListSharedData() to get the one from current HvkGui context)
    HvkDrawVert*             _VtxWritePtr;       // [Internal] point within VtxBuffer.Data after each add command (to avoid using the HvkVector<> operators too much)
    HvkDrawIdx*              _IdxWritePtr;       // [Internal] point within IdxBuffer.Data after each add command (to avoid using the HvkVector<> operators too much)
    HvkVector<HvkVec2>        _Path;              // [Internal] current path building
    HvkDrawCmdHeader         _CmdHeader;         // [Internal] template of active commands. Fields should match those of CmdBuffer.back().
    HvkDrawListSplitter      _Splitter;          // [Internal] for channels api (note: prefer using your own persistent instance of HvkDrawListSplitter!)
    HvkVector<HvkVec4>        _ClipRectStack;     // [Internal]
    HvkVector<HvkTextureRef>  _TextureStack;      // [Internal]
    HvkVector<HvkU8>          _CallbacksDataBuf;  // [Internal]
    float                   _FringeScale;       // [Internal] anti-alias fringe is scaled by this value, this helps to keep things sharp while zooming at vertex buffer content
    const char*             _OwnerName;         // Pointer to owner window's name for debugging

    // If you want to create HvkDrawList instances, pass them HvkGui::GetDrawListSharedData().
    // (advanced: you may create and use your own HvkDrawListSharedData so you can use HvkDrawList without HvkGui, but that's more involved)
    HvkGui_API HvkDrawList(HvkDrawListSharedData* shared_data);
    HvkGui_API ~HvkDrawList();

    HvkGui_API void  PushClipRect(const HvkVec2& clip_rect_min, const HvkVec2& clip_rect_max, bool intersect_with_current_clip_rect = false);  // Render-level scissoring. This is passed down to your render function but not used for CPU-side coarse clipping. Prefer using higher-level HvkGui::PushClipRect() to affect logic (hit-testing and widget culling)
    HvkGui_API void  PushClipRectFullScreen();
    HvkGui_API void  PopClipRect();
    HvkGui_API void  PushTexture(HvkTextureRef tex_ref);
    HvkGui_API void  PopTexture();
    inline HvkVec2   GetClipRectMin() const { const HvkVec4& cr = _ClipRectStack.back(); return HvkVec2(cr.x, cr.y); }
    inline HvkVec2   GetClipRectMax() const { const HvkVec4& cr = _ClipRectStack.back(); return HvkVec2(cr.z, cr.w); }

    // Primitives
    // - Filled shapes must always use clockwise winding order. The anti-aliasing fringe depends on it. Counter-clockwise shapes will have "inward" anti-aliasing.
    // - For rectangular primitives, "p_min" and "p_max" represent the upper-left and lower-right corners.
    // - For circle primitives, use "num_segments == 0" to automatically calculate tessellation (preferred).
    //   In older versions (until Dear HvkGui 1.77) the AddCircle functions defaulted to num_segments == 12.
    //   In future versions we will use textures to provide cheaper and higher-quality circles.
    //   Use AddNgon() and AddNgonFilled() functions if you need to guarantee a specific number of sides.
    HvkGui_API void  AddLine(const HvkVec2& p1, const HvkVec2& p2, HvkU32 col, float thickness = 1.0f);
    HvkGui_API void  AddRect(const HvkVec2& p_min, const HvkVec2& p_max, HvkU32 col, float rounding = 0.0f, HvkDrawFlags flags = 0, float thickness = 1.0f);   // a: upper-left, b: lower-right (== upper-left + size)
    HvkGui_API void  AddRectFilled(const HvkVec2& p_min, const HvkVec2& p_max, HvkU32 col, float rounding = 0.0f, HvkDrawFlags flags = 0);                     // a: upper-left, b: lower-right (== upper-left + size)
    HvkGui_API void  AddRectFilledMultiColor(const HvkVec2& p_min, const HvkVec2& p_max, HvkU32 col_upr_left, HvkU32 col_upr_right, HvkU32 col_bot_right, HvkU32 col_bot_left);
    HvkGui_API void  AddQuad(const HvkVec2& p1, const HvkVec2& p2, const HvkVec2& p3, const HvkVec2& p4, HvkU32 col, float thickness = 1.0f);
    HvkGui_API void  AddQuadFilled(const HvkVec2& p1, const HvkVec2& p2, const HvkVec2& p3, const HvkVec2& p4, HvkU32 col);
    HvkGui_API void  AddTriangle(const HvkVec2& p1, const HvkVec2& p2, const HvkVec2& p3, HvkU32 col, float thickness = 1.0f);
    HvkGui_API void  AddTriangleFilled(const HvkVec2& p1, const HvkVec2& p2, const HvkVec2& p3, HvkU32 col);
    HvkGui_API void  AddCircle(const HvkVec2& center, float radius, HvkU32 col, int num_segments = 0, float thickness = 1.0f);
    HvkGui_API void  AddCircleFilled(const HvkVec2& center, float radius, HvkU32 col, int num_segments = 0);
    HvkGui_API void  AddNgon(const HvkVec2& center, float radius, HvkU32 col, int num_segments, float thickness = 1.0f);
    HvkGui_API void  AddNgonFilled(const HvkVec2& center, float radius, HvkU32 col, int num_segments);
    HvkGui_API void  AddEllipse(const HvkVec2& center, const HvkVec2& radius, HvkU32 col, float rot = 0.0f, int num_segments = 0, float thickness = 1.0f);
    HvkGui_API void  AddEllipseFilled(const HvkVec2& center, const HvkVec2& radius, HvkU32 col, float rot = 0.0f, int num_segments = 0);
    HvkGui_API void  AddText(const HvkVec2& pos, HvkU32 col, const char* text_begin, const char* text_end = NULL);
    HvkGui_API void  AddText(HvkFont* font, float font_size, const HvkVec2& pos, HvkU32 col, const char* text_begin, const char* text_end = NULL, float wrap_width = 0.0f, const HvkVec4* cpu_fine_clip_rect = NULL);
    HvkGui_API void  AddBezierCubic(const HvkVec2& p1, const HvkVec2& p2, const HvkVec2& p3, const HvkVec2& p4, HvkU32 col, float thickness, int num_segments = 0); // Cubic Bezier (4 control points)
    HvkGui_API void  AddBezierQuadratic(const HvkVec2& p1, const HvkVec2& p2, const HvkVec2& p3, HvkU32 col, float thickness, int num_segments = 0);               // Quadratic Bezier (3 control points)

    // General polygon
    // - Only simple polygons are supported by filling functions (no self-intersections, no holes).
    // - Concave polygon fill is more expensive than convex one: it has O(N^2) complexity. Provided as a convenience for the user but not used by the main library.
    HvkGui_API void  AddPolyline(const HvkVec2* points, int num_points, HvkU32 col, HvkDrawFlags flags, float thickness);
    HvkGui_API void  AddConvexPolyFilled(const HvkVec2* points, int num_points, HvkU32 col);
    HvkGui_API void  AddConcavePolyFilled(const HvkVec2* points, int num_points, HvkU32 col);

    // Hvkage primitives
    // - Read FAQ to understand what HvkTextureID/HvkTextureRef are.
    // - "p_min" and "p_max" represent the upper-left and lower-right corners of the rectangle.
    // - "uv_min" and "uv_max" represent the normalized texture coordinates to use for those corners. Using (0,0)->(1,1) texture coordinates will generally display the entire texture.
    HvkGui_API void  AddImage(HvkTextureRef tex_ref, const HvkVec2& p_min, const HvkVec2& p_max, const HvkVec2& uv_min = HvkVec2(0, 0), const HvkVec2& uv_max = HvkVec2(1, 1), HvkU32 col = Hvk_COL32_WHITE);
    HvkGui_API void  AddImageQuad(HvkTextureRef tex_ref, const HvkVec2& p1, const HvkVec2& p2, const HvkVec2& p3, const HvkVec2& p4, const HvkVec2& uv1 = HvkVec2(0, 0), const HvkVec2& uv2 = HvkVec2(1, 0), const HvkVec2& uv3 = HvkVec2(1, 1), const HvkVec2& uv4 = HvkVec2(0, 1), HvkU32 col = Hvk_COL32_WHITE);
    HvkGui_API void  AddImageRounded(HvkTextureRef tex_ref, const HvkVec2& p_min, const HvkVec2& p_max, const HvkVec2& uv_min, const HvkVec2& uv_max, HvkU32 col, float rounding, HvkDrawFlags flags = 0);

    // Stateful path API, add points then finish with PathFillConvex() or PathStroke()
    // - Hvkportant: filled shapes must always use clockwise winding order! The anti-aliasing fringe depends on it. Counter-clockwise shapes will have "inward" anti-aliasing.
    //   so e.g. 'PathArcTo(center, radius, PI * -0.5f, PI)' is ok, whereas 'PathArcTo(center, radius, PI, PI * -0.5f)' won't have correct anti-aliasing when followed by PathFillConvex().
    inline    void  PathClear()                                                 { _Path.Size = 0; }
    inline    void  PathLineTo(const HvkVec2& pos)                               { _Path.push_back(pos); }
    inline    void  PathLineToMergeDuplicate(const HvkVec2& pos)                 { if (_Path.Size == 0 || memcmp(&_Path.Data[_Path.Size - 1], &pos, 8) != 0) _Path.push_back(pos); }
    inline    void  PathFillConvex(HvkU32 col)                                   { AddConvexPolyFilled(_Path.Data, _Path.Size, col); _Path.Size = 0; }
    inline    void  PathFillConcave(HvkU32 col)                                  { AddConcavePolyFilled(_Path.Data, _Path.Size, col); _Path.Size = 0; }
    inline    void  PathStroke(HvkU32 col, HvkDrawFlags flags = 0, float thickness = 1.0f) { AddPolyline(_Path.Data, _Path.Size, col, flags, thickness); _Path.Size = 0; }
    HvkGui_API void  PathArcTo(const HvkVec2& center, float radius, float a_min, float a_max, int num_segments = 0);
    HvkGui_API void  PathArcToFast(const HvkVec2& center, float radius, int a_min_of_12, int a_max_of_12);                // Use precomputed angles for a 12 steps circle
    HvkGui_API void  PathEllipticalArcTo(const HvkVec2& center, const HvkVec2& radius, float rot, float a_min, float a_max, int num_segments = 0); // Ellipse
    HvkGui_API void  PathBezierCubicCurveTo(const HvkVec2& p2, const HvkVec2& p3, const HvkVec2& p4, int num_segments = 0); // Cubic Bezier (4 control points)
    HvkGui_API void  PathBezierQuadraticCurveTo(const HvkVec2& p2, const HvkVec2& p3, int num_segments = 0);               // Quadratic Bezier (3 control points)
    HvkGui_API void  PathRect(const HvkVec2& rect_min, const HvkVec2& rect_max, float rounding = 0.0f, HvkDrawFlags flags = 0);

    // Advanced: Draw Callbacks
    // - May be used to alter render state (change sampler, blending, current shader). May be used to emit custom rendering commands (difficult to do correctly, but possible).
    // - Use special HvkDrawCallback_ResetRenderState callback to instruct backend to reset its render state to the default.
    // - Your rendering loop must check for 'UserCallback' in HvkDrawCmd and call the function instead of rendering triangles. All standard backends are honoring this.
    // - For some backends, the callback may access selected render-states exposed by the backend in a HvkGui_ImplXXXX_RenderState structure pointed to by platform_io.Renderer_RenderState.
    // - HvkPORTANT: please be mindful of the different level of indirection between using size==0 (copying argument) and using size>0 (copying pointed data into a buffer).
    //   - If userdata_size == 0: we copy/store the 'userdata' argument as-is. It will be available unmodified in HvkDrawCmd::UserCallbackData during render.
    //   - If userdata_size > 0,  we copy/store 'userdata_size' bytes pointed to by 'userdata'. We store them in a buffer stored inside the drawlist. HvkDrawCmd::UserCallbackData will point inside that buffer so you have to retrieve data from there. Your callback may need to use HvkDrawCmd::UserCallbackDataSize if you expect dynamically-sized data.
    //   - Support for userdata_size > 0 was added in v1.91.4, October 2024. So earlier code always only allowed to copy/store a simple void*.
    HvkGui_API void  AddCallback(HvkDrawCallback callback, void* userdata, size_t userdata_size = 0);

    // Advanced: Miscellaneous
    HvkGui_API void  AddDrawCmd();                                               // This is useful if you need to forcefully create a new draw call (to allow for dependent rendering / blending). Otherwise primitives are merged into the same draw-call as much as possible
    HvkGui_API HvkDrawList* CloneOutput() const;                                  // Create a clone of the CmdBuffer/IdxBuffer/VtxBuffer. For multi-threaded rendering, consider using `HvkGui_threaded_rendering` from https://github.com/ocornut/HvkGui_club instead.

    // Advanced: Channels
    // - Use to split render into layers. By switching channels to can render out-of-order (e.g. submit FG primitives before BG primitives)
    // - Use to minimize draw calls (e.g. if going back-and-forth between multiple clipping rectangles, prefer to append into separate channels then merge at the end)
    // - This API shouldn't have been in HvkDrawList in the first place!
    //   Prefer using your own persistent instance of HvkDrawListSplitter as you can stack them.
    //   Using the HvkDrawList::ChannelsXXXX you cannot stack a split over another.
    inline void     ChannelsSplit(int count)    { _Splitter.Split(this, count); }
    inline void     ChannelsMerge()             { _Splitter.Merge(this); }
    inline void     ChannelsSetCurrent(int n)   { _Splitter.SetCurrentChannel(this, n); }

    // Advanced: Primitives allocations
    // - We render triangles (three vertices)
    // - All primitives needs to be reserved via PrimReserve() beforehand.
    HvkGui_API void  PrimReserve(int idx_count, int vtx_count);
    HvkGui_API void  PrimUnreserve(int idx_count, int vtx_count);
    HvkGui_API void  PrimRect(const HvkVec2& a, const HvkVec2& b, HvkU32 col);      // Axis aligned rectangle (composed of two triangles)
    HvkGui_API void  PrimRectUV(const HvkVec2& a, const HvkVec2& b, const HvkVec2& uv_a, const HvkVec2& uv_b, HvkU32 col);
    HvkGui_API void  PrimQuadUV(const HvkVec2& a, const HvkVec2& b, const HvkVec2& c, const HvkVec2& d, const HvkVec2& uv_a, const HvkVec2& uv_b, const HvkVec2& uv_c, const HvkVec2& uv_d, HvkU32 col);
    inline    void  PrimWriteVtx(const HvkVec2& pos, const HvkVec2& uv, HvkU32 col)    { _VtxWritePtr->pos = pos; _VtxWritePtr->uv = uv; _VtxWritePtr->col = col; _VtxWritePtr++; _VtxCurrentIdx++; }
    inline    void  PrimWriteIdx(HvkDrawIdx idx)                                     { *_IdxWritePtr = idx; _IdxWritePtr++; }
    inline    void  PrimVtx(const HvkVec2& pos, const HvkVec2& uv, HvkU32 col)         { PrimWriteIdx((HvkDrawIdx)_VtxCurrentIdx); PrimWriteVtx(pos, uv, col); } // Write vertex with unique index

    // Obsolete names
#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    inline    void  PushTextureID(HvkTextureRef tex_ref) { PushTexture(tex_ref); }   // RENAMED in 1.92.0
    inline    void  PopTextureID()                      { PopTexture(); }           // RENAMED in 1.92.0
#endif
    //inline  void  AddEllipse(const HvkVec2& center, float radius_x, float radius_y, HvkU32 col, float rot = 0.0f, int num_segments = 0, float thickness = 1.0f) { AddEllipse(center, HvkVec2(radius_x, radius_y), col, rot, num_segments, thickness); } // OBSOLETED in 1.90.5 (Mar 2024)
    //inline  void  AddEllipseFilled(const HvkVec2& center, float radius_x, float radius_y, HvkU32 col, float rot = 0.0f, int num_segments = 0) { AddEllipseFilled(center, HvkVec2(radius_x, radius_y), col, rot, num_segments); }                        // OBSOLETED in 1.90.5 (Mar 2024)
    //inline  void  PathEllipticalArcTo(const HvkVec2& center, float radius_x, float radius_y, float rot, float a_min, float a_max, int num_segments = 0) { PathEllipticalArcTo(center, HvkVec2(radius_x, radius_y), rot, a_min, a_max, num_segments); } // OBSOLETED in 1.90.5 (Mar 2024)
    //inline  void  AddBezierCurve(const HvkVec2& p1, const HvkVec2& p2, const HvkVec2& p3, const HvkVec2& p4, HvkU32 col, float thickness, int num_segments = 0) { AddBezierCubic(p1, p2, p3, p4, col, thickness, num_segments); }                         // OBSOLETED in 1.80 (Jan 2021)
    //inline  void  PathBezierCurveTo(const HvkVec2& p2, const HvkVec2& p3, const HvkVec2& p4, int num_segments = 0) { PathBezierCubicCurveTo(p2, p3, p4, num_segments); }                                                                                // OBSOLETED in 1.80 (Jan 2021)

    // [Internal helpers]
    HvkGui_API void  _SetDrawListSharedData(HvkDrawListSharedData* data);
    HvkGui_API void  _ResetForNewFrame();
    HvkGui_API void  _ClearFreeMemory();
    HvkGui_API void  _PopUnusedDrawCmd();
    HvkGui_API void  _TryMergeDrawCmds();
    HvkGui_API void  _OnChangedClipRect();
    HvkGui_API void  _OnChangedTexture();
    HvkGui_API void  _OnChangedVtxOffset();
    HvkGui_API void  _SetTexture(HvkTextureRef tex_ref);
    HvkGui_API int   _CalcCircleAutoSegmentCount(float radius) const;
    HvkGui_API void  _PathArcToFastEx(const HvkVec2& center, float radius, int a_min_sample, int a_max_sample, int a_step);
    HvkGui_API void  _PathArcToN(const HvkVec2& center, float radius, float a_min, float a_max, int num_segments);
};

// All draw data to render a Dear HvkGui frame
// (NB: the style and the naming convention here is a little inconsistent, we currently preserve them for backward compatibility purpose,
// as this is one of the oldest structure exposed by the library! Basically, HvkDrawList == CmdList)
struct HvkDrawData
{
    bool                Valid;              // Only valid after Render() is called and before the next NewFrame() is called.
    int                 CmdListsCount;      // == CmdLists.Size. (OBSOLETE: exists for legacy reasons). Number of HvkDrawList* to render.
    int                 TotalIdxCount;      // For convenience, sum of all HvkDrawList's IdxBuffer.Size
    int                 TotalVtxCount;      // For convenience, sum of all HvkDrawList's VtxBuffer.Size
    HvkVector<HvkDrawList*> CmdLists;         // Array of HvkDrawList* to render. The HvkDrawLists are owned by HvkGuiContext and only pointed to from here.
    HvkVec2              DisplayPos;         // Top-left position of the viewport to render (== top-left of the orthogonal projection matrix to use) (== GetMainViewport()->Pos for the main viewport, == (0.0) in most single-viewport applications)
    HvkVec2              DisplaySize;        // Size of the viewport to render (== GetMainViewport()->Size for the main viewport, == io.DisplaySize in most single-viewport applications)
    HvkVec2              FramebufferScale;   // Amount of pixels for each unit of DisplaySize. Copied from viewport->FramebufferScale (== io.DisplayFramebufferScale for main viewport). Generally (1,1) on normal display, (2,2) on OSX with Retina display.
    HvkGuiViewport*      OwnerViewport;      // Viewport carrying the HvkDrawData instance, might be of use to the renderer (generally not).
    HvkVector<HvkTextureData*>* Textures;     // List of textures to update. Most of the times the list is shared by all HvkDrawData, has only 1 texture and it doesn't need any update. This almost always points to HvkGui::GetPlatformIO().Textures[]. May be overridden or set to NULL if you want to manually update textures.

    // Functions
    HvkDrawData()    { Clear(); }
    HvkGui_API void  Clear();
    HvkGui_API void  AddDrawList(HvkDrawList* draw_list);     // Helper to add an external draw list into an existing HvkDrawData.
    HvkGui_API void  DeIndexAllBuffers();                    // Helper to convert all buffers from indexed to non-indexed, in case you cannot render indexed. Note: this is slow and most likely a waste of resources. Always prefer indexed rendering!
    HvkGui_API void  ScaleClipRects(const HvkVec2& fb_scale); // Helper to scale the ClipRect field of each HvkDrawCmd. Use if your final output buffer is at a different scale than Dear HvkGui expects, or if there is a difference between your window resolution and framebuffer resolution.
};

//-----------------------------------------------------------------------------
// [SECTION] Texture API (HvkTextureFormat, HvkTextureStatus, HvkTextureRect, HvkTextureData)
//-----------------------------------------------------------------------------
// In principle, the only data types that user/application code should care about are 'HvkTextureRef' and 'HvkTextureID'.
// They are defined above in this header file. Read their description to the difference between HvkTextureRef and HvkTextureID.
// FOR ALL OTHER HvkTextureXXXX TYPES: ONLY CORE LIBRARY AND RENDERER BACKENDS NEED TO KNOW AND CARE ABOUT THEM.
//-----------------------------------------------------------------------------

#undef Status // X11 headers are leaking this.

// We intentionally support a limited amount of texture formats to limit burden on CPU-side code and extension.
// Most standard backends only support RGBA32 but we provide a single channel option for low-resource/embedded systems.
enum HvkTextureFormat
{
    HvkTextureFormat_RGBA32,         // 4 components per pixel, each is unsigned 8-bit. Total size = TexWidth * TexHeight * 4
    HvkTextureFormat_Alpha8,         // 1 component per pixel, each is unsigned 8-bit. Total size = TexWidth * TexHeight
};

// Status of a texture to communicate with Renderer Backend.
enum HvkTextureStatus
{
    HvkTextureStatus_OK,
    HvkTextureStatus_Destroyed,      // Backend destroyed the texture.
    HvkTextureStatus_WantCreate,     // Requesting backend to create the texture. Set status OK when done.
    HvkTextureStatus_WantUpdates,    // Requesting backend to update specific blocks of pixels (write to texture portions which have never been used before). Set status OK when done.
    HvkTextureStatus_WantDestroy,    // Requesting backend to destroy the texture. Set status to Destroyed when done.
};

// Coordinates of a rectangle within a texture.
// When a texture is in HvkTextureStatus_WantUpdates state, we provide a list of individual rectangles to copy to the graphics system.
// You may use HvkTextureData::Updates[] for the list, or HvkTextureData::UpdateBox for a single bounding box.
struct HvkTextureRect
{
    unsigned short      x, y;       // Upper-left coordinates of rectangle to update
    unsigned short      w, h;       // Size of rectangle to update (in pixels)
};

// Specs and pixel storage for a texture used by Dear HvkGui.
// This is only useful for (1) core library and (2) backends. End-user/applications do not need to care about this.
// Renderer Backends will create a GPU-side version of this.
// Why does we store two identifiers: TexID and BackendUserData?
// - HvkTextureID    TexID           = lower-level identifier stored in HvkDrawCmd. HvkDrawCmd can refer to textures not created by the backend, and for which there's no HvkTextureData.
// - void*          BackendUserData = higher-level opaque storage for backend own book-keeping. Some backends may have enough with TexID and not need both.
 // In columns below: who reads/writes each fields? 'r'=read, 'w'=write, 'core'=main library, 'backend'=renderer backend
struct HvkTextureData
{
    //------------------------------------------ core / backend ---------------------------------------
    int                 UniqueID;               // w    -   // [DEBUG] Sequential index to facilitate identifying a texture when debugging/printing. Unique per atlas.
    HvkTextureStatus     Status;                 // rw   rw  // HvkTextureStatus_OK/_WantCreate/_WantUpdates/_WantDestroy. Always use SetStatus() to modify!
    void*               BackendUserData;        // -    rw  // Convenience storage for backend. Some backends may have enough with TexID.
    HvkTextureID         TexID;                  // r    w   // Backend-specific texture identifier. Always use SetTexID() to modify! The identifier will stored in HvkDrawCmd::GetTexID() and passed to backend's RenderDrawData function.
    HvkTextureFormat     Format;                 // w    r   // HvkTextureFormat_RGBA32 (default) or HvkTextureFormat_Alpha8
    int                 Width;                  // w    r   // Texture width
    int                 Height;                 // w    r   // Texture height
    int                 BytesPerPixel;          // w    r   // 4 or 1
    unsigned char*      Pixels;                 // w    r   // Pointer to buffer holding 'Width*Height' pixels and 'Width*Height*BytesPerPixels' bytes.
    HvkTextureRect       UsedRect;               // w    r   // Bounding box encompassing all past and queued Updates[].
    HvkTextureRect       UpdateRect;             // w    r   // Bounding box encompassing all queued Updates[].
    HvkVector<HvkTextureRect> Updates;            // w    r   // Array of individual updates.
    int                 UnusedFrames;           // w    r   // In order to facilitate handling Status==WantDestroy in some backend: this is a count successive frames where the texture was not used. Always >0 when Status==WantDestroy.
    unsigned short      RefCount;               // w    r   // Number of contexts using this texture. Used during backend shutdown.
    bool                UseColors;              // w    r   // Tell whether our texture data is known to use colors (rather than just white + alpha).
    bool                WantDestroyNextFrame;   // rw   -   // [Internal] Queued to set HvkTextureStatus_WantDestroy next frame. May still be used in the current frame.

    // Functions
    HvkTextureData()     { memset(this, 0, sizeof(*this)); Status = HvkTextureStatus_Destroyed; TexID = HvkTextureID_Invalid; }
    ~HvkTextureData()    { DestroyPixels(); }
    HvkGui_API void      Create(HvkTextureFormat format, int w, int h);
    HvkGui_API void      DestroyPixels();
    void*               GetPixels()                 { Hvk_ASSERT(Pixels != NULL); return Pixels; }
    void*               GetPixelsAt(int x, int y)   { Hvk_ASSERT(Pixels != NULL); return Pixels + (x + y * Width) * BytesPerPixel; }
    int                 GetSizeInBytes() const      { return Width * Height * BytesPerPixel; }
    int                 GetPitch() const            { return Width * BytesPerPixel; }
    HvkTextureRef        GetTexRef()                 { HvkTextureRef tex_ref; tex_ref._TexData = this; tex_ref._TexID = HvkTextureID_Invalid; return tex_ref; }
    HvkTextureID         GetTexID() const            { return TexID; }

    // Called by Renderer backend
    // - Call SetTexID() and SetStatus() after honoring texture requests. Never modify TexID and Status directly!
    // - A backend may decide to destroy a texture that we did not request to destroy, which is fine (e.g. freeing resources), but we immediately set the texture back in _WantCreate mode.
    void    SetTexID(HvkTextureID tex_id)            { TexID = tex_id; }
    void    SetStatus(HvkTextureStatus status)       { Status = status; if (status == HvkTextureStatus_Destroyed && !WantDestroyNextFrame) Status = HvkTextureStatus_WantCreate; }
};

//-----------------------------------------------------------------------------
// [SECTION] Font API (HvkFontConfig, HvkFontGlyph, HvkFontAtlasFlags, HvkFontAtlas, HvkFontGlyphRangesBuilder, HvkFont)
//-----------------------------------------------------------------------------

// A font input/source (we may rename this to HvkFontSource in the future)
struct HvkFontConfig
{
    // Data Source
    char            Name[40];               // <auto>   // Name (strictly to ease debugging, hence limited size buffer)
    void*           FontData;               //          // TTF/OTF data
    int             FontDataSize;           //          // TTF/OTF data size
    bool            FontDataOwnedByAtlas;   // true     // TTF/OTF data ownership taken by the owner HvkFontAtlas (will delete memory itself).

    // Options
    bool            MergeMode;              // false    // Merge into previous HvkFont, so you can combine multiple inputs font into one HvkFont (e.g. ASCII font + icons + Japanese glyphs). You may want to use GlyphOffset.y when merge font of different heights.
    bool            PixelSnapH;             // false    // Align every glyph AdvanceX to pixel boundaries. Useful e.g. if you are merging a non-pixel aligned font with the default font. If enabled, you can set OversampleH/V to 1.
    bool            PixelSnapV;             // true     // Align Scaled GlyphOffset.y to pixel boundaries.
    HvkS8            OversampleH;            // 0 (2)    // Rasterize at higher quality for sub-pixel positioning. 0 == auto == 1 or 2 depending on size. Note the difference between 2 and 3 is minimal. You can reduce this to 1 for large glyphs save memory. Read https://github.com/nothings/stb/blob/master/tests/oversample/README.md for details.
    HvkS8            OversampleV;            // 0 (1)    // Rasterize at higher quality for sub-pixel positioning. 0 == auto == 1. This is not really useful as we don't use sub-pixel positions on the Y axis.
    HvkWchar         EllipsisChar;           // 0        // Explicitly specify Unicode codepoint of ellipsis character. When fonts are being merged first specified ellipsis will be used.
    float           SizePixels;             //          // Size in pixels for rasterizer (more or less maps to the resulting font height).
    const HvkWchar*  GlyphRanges;            // NULL     // *LEGACY* THE ARRAY DATA NEEDS TO PERSIST AS LONG AS THE FONT IS ALIVE. Pointer to a user-provided list of Unicode range (2 value per range, values are inclusive, zero-terminated list).
    const HvkWchar*  GlyphExcludeRanges;     // NULL     // Pointer to a small user-provided list of Unicode ranges (2 value per range, values are inclusive, zero-terminated list). This is very close to GlyphRanges[] but designed to exclude ranges from a font source, when merging fonts with overlapping glyphs. Use "Input Glyphs Overlap Detection Tool" to find about your overlapping ranges.
    //HvkVec2        GlyphExtraSpacing;      // 0, 0     // (REMOVED AT IT SEEMS LARGELY OBSOLETE. PLEASE REPORT IF YOU WERE USING THIS). Extra spacing (in pixels) between glyphs when rendered: essentially add to glyph->AdvanceX. Only X axis is supported for now.
    HvkVec2          GlyphOffset;            // 0, 0     // Offset (in pixels) all glyphs from this font input. Absolute value for default size, other sizes will scale this value.
    float           GlyphMinAdvanceX;       // 0        // Minimum AdvanceX for glyphs, set Min to align font icons, set both Min/Max to enforce mono-space font. Absolute value for default size, other sizes will scale this value.
    float           GlyphMaxAdvanceX;       // FLT_MAX  // Maximum AdvanceX for glyphs
    float           GlyphExtraAdvanceX;     // 0        // Extra spacing (in pixels) between glyphs. Please contact us if you are using this. // FIXME-NEWATLAS: Intentionally unscaled
    HvkU32           FontNo;                 // 0        // Index of font within TTF/OTF file
    unsigned int    FontLoaderFlags;        // 0        // Settings for custom font builder. THIS IS BUILDER implementation DEPENDENT. Leave as zero if unsure.
    //unsigned int  FontBuilderFlags;       // --       // [Renamed in 1.92] Ue FontLoaderFlags.
    float           RasterizerMultiply;     // 1.0f     // Linearly brighten (>1.0f) or darken (<1.0f) font output. Brightening small fonts may be a good workaround to make them more readable. This is a silly thing we may remove in the future.
    float           RasterizerDensity;      // 1.0f     // [LEGACY: this only makes sense when HvkGuiBackendFlags_RendererHasTextures is not supported] DPI scale multiplier for rasterization. Not altering other font metrics: makes it easy to swap between e.g. a 100% and a 400% fonts for a zooming display, or handle Retina screen. HvkPORTANT: If you change this it is expected that you increase/decrease font scale roughly to the inverse of this, otherwise quality may look lowered.

    // [Internal]
    HvkFontFlags     Flags;                  // Font flags (don't use just yet, will be exposed in upcoming 1.92.X updates)
    HvkFont*         DstFont;                // Target font (as we merging fonts, multiple HvkFontConfig may target the same font)
    const HvkFontLoader* FontLoader;         // Custom font backend for this source (default source is the one stored in HvkFontAtlas)
    void*           FontLoaderData;         // Font loader opaque storage (per font config)

    HvkGui_API HvkFontConfig();
};

// Hold rendering data for one glyph.
// (Note: some language parsers may fail to convert the bitfield members, in this case maybe drop store a single u32 or we can rework this)
struct HvkFontGlyph
{
    unsigned int    Colored : 1;        // Flag to indicate glyph is colored and should generally ignore tinting (make it usable with no shift on little-endian as this is used in loops)
    unsigned int    Visible : 1;        // Flag to indicate glyph has no visible pixels (e.g. space). Allow early out when rendering.
    unsigned int    SourceIdx : 4;      // Index of source in parent font
    unsigned int    Codepoint : 26;     // 0x0000..0x10FFFF
    float           AdvanceX;           // Horizontal distance to advance cursor/layout position.
    float           X0, Y0, X1, Y1;     // Glyph corners. Offsets from current cursor/layout position.
    float           U0, V0, U1, V1;     // Texture coordinates for the current value of HvkFontAtlas->TexRef. Cached equivalent of calling GetCustomRect() with PackId.
    int             PackId;             // [Internal] HvkFontAtlasRectId value (FIXME: Cold data, could be moved elsewhere?)

    HvkFontGlyph()   { memset(this, 0, sizeof(*this)); PackId = -1; }
};

// Helper to build glyph ranges from text/string data. Feed your application strings/characters to it then call BuildRanges().
// This is essentially a tightly packed of vector of 64k booleans = 8KB storage.
struct HvkFontGlyphRangesBuilder
{
    HvkVector<HvkU32> UsedChars;            // Store 1-bit per Unicode code point (0=unused, 1=used)

    HvkFontGlyphRangesBuilder()              { Clear(); }
    inline void     Clear()                 { int size_in_bytes = (Hvk_UNICODE_CODEPOINT_MAX + 1) / 8; UsedChars.resize(size_in_bytes / (int)sizeof(HvkU32)); memset(UsedChars.Data, 0, (size_t)size_in_bytes); }
    inline bool     GetBit(size_t n) const  { int off = (int)(n >> 5); HvkU32 mask = 1u << (n & 31); return (UsedChars[off] & mask) != 0; }  // Get bit n in the array
    inline void     SetBit(size_t n)        { int off = (int)(n >> 5); HvkU32 mask = 1u << (n & 31); UsedChars[off] |= mask; }               // Set bit n in the array
    inline void     AddChar(HvkWchar c)      { SetBit(c); }                      // Add character
    HvkGui_API void  AddText(const char* text, const char* text_end = NULL);     // Add string (each character of the UTF-8 string are added)
    HvkGui_API void  AddRanges(const HvkWchar* ranges);                           // Add ranges, e.g. builder.AddRanges(HvkFontAtlas::GetGlyphRangesDefault()) to force add all of ASCII/Latin+Ext
    HvkGui_API void  BuildRanges(HvkVector<HvkWchar>* out_ranges);                 // Output new ranges
};

// An opaque identifier to a rectangle in the atlas. -1 when invalid.
// The rectangle may move and UV may be invalidated, use GetCustomRect() to retrieve it.
typedef int HvkFontAtlasRectId;
#define HvkFontAtlasRectId_Invalid -1

// Output of HvkFontAtlas::GetCustomRect() when using custom rectangles.
// Those values may not be cached/stored as they are only valid for the current value of atlas->TexRef
// (this is in theory derived from HvkTextureRect but we use separate structures for reasons)
struct HvkFontAtlasRect
{
    unsigned short  x, y;               // Position (in current texture)
    unsigned short  w, h;               // Size
    HvkVec2          uv0, uv1;           // UV coordinates (in current texture)

    HvkFontAtlasRect() { memset(this, 0, sizeof(*this)); }
};

// Flags for HvkFontAtlas build
enum HvkFontAtlasFlags_
{
    HvkFontAtlasFlags_None               = 0,
    HvkFontAtlasFlags_NoPowerOfTwoHeight = 1 << 0,   // Don't round the height to next power of two
    HvkFontAtlasFlags_NoMouseCursors     = 1 << 1,   // Don't build software mouse cursors into the atlas (save a little texture memory)
    HvkFontAtlasFlags_NoBakedLines       = 1 << 2,   // Don't build thick line textures into the atlas (save a little texture memory, allow support for point/nearest filtering). The AntiAliasedLinesUseTex features uses them, otherwise they will be rendered using polygons (more expensive for CPU/GPU).
};

// Load and rasterize multiple TTF/OTF fonts into a same texture. The font atlas will build a single texture holding:
//  - One or more fonts.
//  - Custom graphics data needed to render the shapes needed by Dear HvkGui.
//  - Mouse cursor shapes for software cursor rendering (unless setting 'Flags |= HvkFontAtlasFlags_NoMouseCursors' in the font atlas).
//  - If you don't call any AddFont*** functions, the default font embedded in the code will be loaded for you.
// It is the rendering backend responsibility to upload texture into your graphics API:
//  - HvkGui_ImplXXXX_RenderDrawData() functions generally iterate platform_io->Textures[] to create/update/destroy each HvkTextureData instance.
//  - Backend then set HvkTextureData's TexID and BackendUserData.
//  - Texture id are passed back to you during rendering to identify the texture. Read FAQ entry about HvkTextureID/HvkTextureRef for more details.
// Legacy path:
//  - Call Build() + GetTexDataAsAlpha8() or GetTexDataAsRGBA32() to build and retrieve pixels data.
//  - Call SetTexID(my_tex_id); and pass the pointer/identifier to your texture in a format natural to your graphics API.
// Common pitfalls:
// - If you pass a 'glyph_ranges' array to AddFont*** functions, you need to make sure that your array persist up until the
//   atlas is build (when calling GetTexData*** or Build()). We only copy the pointer, not the data.
// - Hvkportant: By default, AddFontFromMemoryTTF() takes ownership of the data. Even though we are not writing to it, we will free the pointer on destruction.
//   You can set font_cfg->FontDataOwnedByAtlas=false to keep ownership of your data and it won't be freed,
// - Even though many functions are suffixed with "TTF", OTF data is supported just as well.
// - This is an old API and it is currently awkward for those and various other reasons! We will address them in the future!
struct HvkFontAtlas
{
    HvkGui_API HvkFontAtlas();
    HvkGui_API ~HvkFontAtlas();
    HvkGui_API HvkFont*           AddFont(const HvkFontConfig* font_cfg);
    HvkGui_API HvkFont*           AddFontDefault(const HvkFontConfig* font_cfg = NULL);
    HvkGui_API HvkFont*           AddFontFromFileTTF(const char* filename, float size_pixels = 0.0f, const HvkFontConfig* font_cfg = NULL, const HvkWchar* glyph_ranges = NULL);
    HvkGui_API HvkFont*           AddFontFromMemoryTTF(void* font_data, int font_data_size, float size_pixels = 0.0f, const HvkFontConfig* font_cfg = NULL, const HvkWchar* glyph_ranges = NULL); // Note: Transfer ownership of 'ttf_data' to HvkFontAtlas! Will be deleted after destruction of the atlas. Set font_cfg->FontDataOwnedByAtlas=false to keep ownership of your data and it won't be freed.
    HvkGui_API HvkFont*           AddFontFromMemoryCompressedTTF(const void* compressed_font_data, int compressed_font_data_size, float size_pixels = 0.0f, const HvkFontConfig* font_cfg = NULL, const HvkWchar* glyph_ranges = NULL); // 'compressed_font_data' still owned by caller. Compress with binary_to_compressed_c.cpp.
    HvkGui_API HvkFont*           AddFontFromMemoryCompressedBase85TTF(const char* compressed_font_data_base85, float size_pixels = 0.0f, const HvkFontConfig* font_cfg = NULL, const HvkWchar* glyph_ranges = NULL);              // 'compressed_font_data_base85' still owned by caller. Compress with binary_to_compressed_c.cpp with -base85 parameter.
    HvkGui_API void              RemoveFont(HvkFont* font);

    HvkGui_API void              Clear();                    // Clear everything (input fonts, output glyphs/textures).
    HvkGui_API void              CompactCache();             // Compact cached glyphs and texture.
    HvkGui_API void              SetFontLoader(const HvkFontLoader* font_loader); // Change font loader at runtime.

    // As we are transitioning toward a new font system, we expect to obsolete those soon:
    HvkGui_API void              ClearInputData();           // [OBSOLETE] Clear input data (all HvkFontConfig structures including sizes, TTF data, glyph ranges, etc.) = all the data used to build the texture and fonts.
    HvkGui_API void              ClearFonts();               // [OBSOLETE] Clear input+output font data (same as ClearInputData() + glyphs storage, UV coordinates).
    HvkGui_API void              ClearTexData();             // [OBSOLETE] Clear CPU-side copy of the texture data. Saves RAM once the texture has been copied to graphics memory.

#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    // Legacy path for build atlas + retrieving pixel data.
    // - User is in charge of copying the pixels into graphics memory (e.g. create a texture with your engine). Then store your texture handle with SetTexID().
    // - The pitch is always = Width * BytesPerPixels (1 or 4)
    // - Building in RGBA32 format is provided for convenience and compatibility, but note that unless you manually manipulate or copy color data into
    //   the texture (e.g. when using the AddCustomRect*** api), then the RGB pixels emitted will always be white (~75% of memory/bandwidth waste).
    // - From 1.92 with backends supporting HvkGuiBackendFlags_RendererHasTextures:
    //   - Calling Build(), GetTexDataAsAlpha8(), GetTexDataAsRGBA32() is not needed.
    //   - In backend: replace calls to HvkFontAtlas::SetTexID() with calls to HvkTextureData::SetTexID() after honoring texture creation.
    HvkGui_API bool  Build();                    // Build pixels data. This is called automatically for you by the GetTexData*** functions.
    HvkGui_API void  GetTexDataAsAlpha8(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel = NULL); // 1 byte per-pixel
    HvkGui_API void  GetTexDataAsRGBA32(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel = NULL); // 4 bytes-per-pixel
    void            SetTexID(HvkTextureID id)    { Hvk_ASSERT(TexRef._TexID == HvkTextureID_Invalid); TexRef._TexData->TexID = id; }                               // Called by legacy backends. May be called before texture creation.
    void            SetTexID(HvkTextureRef id)   { Hvk_ASSERT(TexRef._TexID == HvkTextureID_Invalid && id._TexData == NULL); TexRef._TexData->TexID = id._TexID; } // Called by legacy backends.
    bool            IsBuilt() const { return Fonts.Size > 0 && TexIsBuilt; } // Bit ambiguous: used to detect when user didn't build texture but effectively we should check TexID != 0 except that would be backend dependent..
#endif

    //-------------------------------------------
    // Glyph Ranges
    //-------------------------------------------

    // Since 1.92: specifying glyph ranges is only useful/necessary if your backend doesn't support HvkGuiBackendFlags_RendererHasTextures!
    HvkGui_API const HvkWchar*    GetGlyphRangesDefault();                // Basic Latin, Extended Latin
#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    // Helpers to retrieve list of common Unicode ranges (2 value per range, values are inclusive, zero-terminated list)
    // NB: Make sure that your string are UTF-8 and NOT in your local code page.
    // Read https://github.com/ocornut/HvkGui/blob/master/docs/FONTS.md/#about-utf-8-encoding for details.
    // NB: Consider using HvkFontGlyphRangesBuilder to build glyph ranges from textual data.
    HvkGui_API const HvkWchar*    GetGlyphRangesGreek();                  // Default + Greek and Coptic
    HvkGui_API const HvkWchar*    GetGlyphRangesKorean();                 // Default + Korean characters
    HvkGui_API const HvkWchar*    GetGlyphRangesJapanese();               // Default + Hiragana, Katakana, Half-Width, Selection of 2999 Ideographs
    HvkGui_API const HvkWchar*    GetGlyphRangesChineseFull();            // Default + Half-Width + Japanese Hiragana/Katakana + full set of about 21000 CJK Unified Ideographs
    HvkGui_API const HvkWchar*    GetGlyphRangesChineseSimplifiedCommon();// Default + Half-Width + Japanese Hiragana/Katakana + set of 2500 CJK Unified Ideographs for common simplified Chinese
    HvkGui_API const HvkWchar*    GetGlyphRangesCyrillic();               // Default + about 400 Cyrillic characters
    HvkGui_API const HvkWchar*    GetGlyphRangesThai();                   // Default + Thai characters
    HvkGui_API const HvkWchar*    GetGlyphRangesVietnamese();             // Default + Vietnamese characters
#endif

    //-------------------------------------------
    // [ALPHA] Custom Rectangles/Glyphs API
    //-------------------------------------------

    // Register and retrieve custom rectangles
    // - You can request arbitrary rectangles to be packed into the atlas, for your own purpose.
    // - Since 1.92.0, packing is done immediately in the function call (previously packing was done during the Build call)
    // - You can render your pixels into the texture right after calling the AddCustomRect() functions.
    // - VERY HvkPORTANT:
    //   - Texture may be created/resized at any time when calling HvkGui or HvkFontAtlas functions.
    //   - IT WILL INVALIDATE RECTANGLE DATA SUCH AS UV COORDINATES. Always use latest values from GetCustomRect().
    //   - UV coordinates are associated to the current texture identifier aka 'atlas->TexRef'. Both TexRef and UV coordinates are typically changed at the same time.
    // - If you render colored output into your custom rectangles: set 'atlas->TexPixelsUseColors = true' as this may help some backends decide of preferred texture format.
    // - Read docs/FONTS.md for more details about using colorful icons.
    // - Note: this API may be reworked further in order to facilitate supporting e.g. multi-monitor, varying DPI settings.
    // - (Pre-1.92 names) ------------> (1.92 names)
    //   - GetCustomRectByIndex()   --> Use GetCustomRect()
    //   - CalcCustomRectUV()       --> Use GetCustomRect() and read uv0, uv1 fields.
    //   - AddCustomRectRegular()   --> Renamed to AddCustomRect()
    //   - AddCustomRectFontGlyph() --> Prefer using custom HvkFontLoader inside HvkFontConfig
    //   - HvkFontAtlasCustomRect    --> Renamed to HvkFontAtlasRect
    HvkGui_API HvkFontAtlasRectId AddCustomRect(int width, int height, HvkFontAtlasRect* out_r = NULL);// Register a rectangle. Return -1 (HvkFontAtlasRectId_Invalid) on error.
    HvkGui_API void              RemoveCustomRect(HvkFontAtlasRectId id);                             // Unregister a rectangle. Existing pixels will stay in texture until resized / garbage collected.
    HvkGui_API bool              GetCustomRect(HvkFontAtlasRectId id, HvkFontAtlasRect* out_r) const;  // Get rectangle coordinates for current texture. Valid immediately, never store this (read above)!

    //-------------------------------------------
    // Members
    //-------------------------------------------

    // Input
    HvkFontAtlasFlags            Flags;              // Build flags (see HvkFontAtlasFlags_)
    HvkTextureFormat             TexDesiredFormat;   // Desired texture format (default to HvkTextureFormat_RGBA32 but may be changed to HvkTextureFormat_Alpha8).
    int                         TexGlyphPadding;    // FIXME: Should be called "TexPackPadding". Padding between glyphs within texture in pixels. Defaults to 1. If your rendering method doesn't rely on bilinear filtering you may set this to 0 (will also need to set AntiAliasedLinesUseTex = false).
    int                         TexMinWidth;        // Minimum desired texture width. Must be a power of two. Default to 512.
    int                         TexMinHeight;       // Minimum desired texture height. Must be a power of two. Default to 128.
    int                         TexMaxWidth;        // Maximum desired texture width. Must be a power of two. Default to 8192.
    int                         TexMaxHeight;       // Maximum desired texture height. Must be a power of two. Default to 8192.
    void*                       UserData;           // Store your own atlas related user-data (if e.g. you have multiple font atlas).

    // Output
    // - Because textures are dynamically created/resized, the current texture identifier may changed at *ANY TIME* during the frame.
    // - This should not affect you as you can always use the latest value. But note that any precomputed UV coordinates are only valid for the current TexRef.
#ifdef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    HvkTextureRef                TexRef;             // Latest texture identifier == TexData->GetTexRef().
#else
    union { HvkTextureRef TexRef; HvkTextureRef TexID; }; // Latest texture identifier == TexData->GetTexRef(). // RENAMED TexID to TexRef in 1.92.0.
#endif
    HvkTextureData*              TexData;            // Latest texture.

    // [Internal]
    HvkVector<HvkTextureData*>    TexList;            // Texture list (most often TexList.Size == 1). TexData is always == TexList.back(). DO NOT USE DIRECTLY, USE GetDrawData().Textures[]/GetPlatformIO().Textures[] instead!
    bool                        Locked;             // Marked as locked during HvkGui::NewFrame()..EndFrame() scope if TexUpdates are not supported. Any attempt to modify the atlas will assert.
    bool                        RendererHasTextures;// Copy of (BackendFlags & HvkGuiBackendFlags_RendererHasTextures) from supporting context.
    bool                        TexIsBuilt;         // Set when texture was built matching current font input. Mostly useful for legacy IsBuilt() call.
    bool                        TexPixelsUseColors; // Tell whether our texture data is known to use colors (rather than just alpha channel), in order to help backend select a format or conversion process.
    HvkVec2                      TexUvScale;         // = (1.0f/TexData->TexWidth, 1.0f/TexData->TexHeight). May change as new texture gets created.
    HvkVec2                      TexUvWhitePixel;    // Texture coordinates to a white pixel. May change as new texture gets created.
    HvkVector<HvkFont*>           Fonts;              // Hold all the fonts returned by AddFont*. Fonts[0] is the default font upon calling HvkGui::NewFrame(), use HvkGui::PushFont()/PopFont() to change the current font.
    HvkVector<HvkFontConfig>      Sources;            // Source/configuration data
    HvkVec4                      TexUvLines[Hvk_DRAWLIST_TEX_LINES_WIDTH_MAX + 1];  // UVs for baked anti-aliased lines
    int                         TexNextUniqueID;    // Next value to be stored in TexData->UniqueID
    int                         FontNextUniqueID;   // Next value to be stored in HvkFont->FontID
    HvkVector<HvkDrawListSharedData*> DrawListSharedDatas; // List of users for this atlas. Typically one per Dear HvkGui context.
    HvkFontAtlasBuilder*         Builder;            // Opaque interface to our data that doesn't need to be public and may be discarded when rebuilding.
    const HvkFontLoader*         FontLoader;         // Font loader opaque interface (default to use FreeType when HvkGui_ENABLE_FREETYPE is defined, otherwise default to use stb_truetype). Use SetFontLoader() to change this at runtime.
    const char*                 FontLoaderName;     // Font loader name (for display e.g. in About box) == FontLoader->Name
    void*                       FontLoaderData;     // Font backend opaque storage
    unsigned int                FontLoaderFlags;    // Shared flags (for all fonts) for font loader. THIS IS BUILD implementation DEPENDENT (e.g. Per-font override is also available in HvkFontConfig).
    int                         RefCount;           // Number of contexts using this atlas
    HvkGuiContext*               OwnerContext;       // Context which own the atlas will be in charge of updating and destroying it.

    // [Obsolete]
#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    // Legacy: You can request your rectangles to be mapped as font glyph (given a font + Unicode point), so you can render e.g. custom colorful icons and use them as regular glyphs. --> Prefer using a custom HvkFontLoader.
    HvkFontAtlasRect             TempRect;           // For old GetCustomRectByIndex() API
    inline HvkFontAtlasRectId    AddCustomRectRegular(int w, int h)                                                          { return AddCustomRect(w, h); }                             // RENAMED in 1.92.0
    inline const HvkFontAtlasRect* GetCustomRectByIndex(HvkFontAtlasRectId id)                                                { return GetCustomRect(id, &TempRect) ? &TempRect : NULL; } // OBSOLETED in 1.92.0
    inline void                 CalcCustomRectUV(const HvkFontAtlasRect* r, HvkVec2* out_uv_min, HvkVec2* out_uv_max) const    { *out_uv_min = r->uv0; *out_uv_max = r->uv1; }             // OBSOLETED in 1.92.0
    HvkGui_API HvkFontAtlasRectId AddCustomRectFontGlyph(HvkFont* font, HvkWchar codepoint, int w, int h, float advance_x, const HvkVec2& offset = HvkVec2(0, 0));                            // OBSOLETED in 1.92.0: Use custom HvkFontLoader in HvkFontConfig
    HvkGui_API HvkFontAtlasRectId AddCustomRectFontGlyphForSize(HvkFont* font, float font_size, HvkWchar codepoint, int w, int h, float advance_x, const HvkVec2& offset = HvkVec2(0, 0));    // ADDED AND OBSOLETED in 1.92.0
#endif
    //unsigned int                      FontBuilderFlags;        // OBSOLETED in 1.92.0: Renamed to FontLoaderFlags.
    //int                               TexDesiredWidth;         // OBSOLETED in 1.92.0: Force texture width before calling Build(). Must be a power-of-two. If have many glyphs your graphics API have texture size restrictions you may want to increase texture width to decrease height.
    //typedef HvkFontAtlasRect           HvkFontAtlasCustomRect;   // OBSOLETED in 1.92.0
    //typedef HvkFontAtlasCustomRect     CustomRect;              // OBSOLETED in 1.72+
    //typedef HvkFontGlyphRangesBuilder  GlyphRangesBuilder;      // OBSOLETED in 1.67+
};

// Font runtime data for a given size
// Hvkportant: pointers to HvkFontBaked are only valid for the current frame.
struct HvkFontBaked
{
    // [Internal] Members: Hot ~20/24 bytes (for CalcTextSize)
    HvkVector<float>             IndexAdvanceX;      // 12-16 // out // Sparse. Glyphs->AdvanceX in a directly indexable way (cache-friendly for CalcTextSize functions which only this info, and are often bottleneck in large UI).
    float                       FallbackAdvanceX;   // 4     // out // FindGlyph(FallbackChar)->AdvanceX
    float                       Size;               // 4     // in  // Height of characters/line, set during loading (doesn't change after loading)
    float                       RasterizerDensity;  // 4     // in  // Density this is baked at

    // [Internal] Members: Hot ~28/36 bytes (for RenderText loop)
    HvkVector<HvkU16>             IndexLookup;        // 12-16 // out // Sparse. Index glyphs by Unicode code-point.
    HvkVector<HvkFontGlyph>       Glyphs;             // 12-16 // out // All glyphs.
    int                         FallbackGlyphIndex; // 4     // out // Index of FontFallbackChar

    // [Internal] Members: Cold
    float                       Ascent, Descent;    // 4+4   // out // Ascent: distance from top to bottom of e.g. 'A' [0..FontSize] (unscaled)
    unsigned int                MetricsTotalSurface:26;// 3  // out // Total surface in pixels to get an idea of the font rasterization/texture cost (not exact, we approximate the cost of padding between glyphs)
    unsigned int                WantDestroy:1;         // 0  //     // Queued for destroy
    unsigned int                LoadNoFallback:1;      // 0  //     // Disable loading fallback in lower-level calls.
    unsigned int                LoadNoRenderOnLayout:1;// 0  //     // Enable a two-steps mode where CalcTextSize() calls will load AdvanceX *without* rendering/packing glyphs. Only advantagous if you know that the glyph is unlikely to actually be rendered, otherwise it is slower because we'd do one query on the first CalcTextSize and one query on the first Draw.
    int                         LastUsedFrame;         // 4  //     // Record of that time this was bounds
    HvkGuiID                     BakedId;            // 4     //     // Unique ID for this baked storage
    HvkFont*                     OwnerFont;          // 4-8   // in  // Parent font
    void*                       FontLoaderDatas;    // 4-8   //     // Font loader opaque storage (per baked font * sources): single contiguous buffer allocated by HvkGui, passed to loader.

    // Functions
    HvkGui_API HvkFontBaked();
    HvkGui_API void              ClearOutputData();
    HvkGui_API HvkFontGlyph*      FindGlyph(HvkWchar c);               // Return U+FFFD glyph if requested glyph doesn't exists.
    HvkGui_API HvkFontGlyph*      FindGlyphNoFallback(HvkWchar c);     // Return NULL if glyph doesn't exist
    HvkGui_API float             GetCharAdvance(HvkWchar c);
    HvkGui_API bool              IsGlyphLoaded(HvkWchar c);
};

// Font flags
// (in future versions as we redesign font loading API, this will become more Hvkportant and better documented. for now please consider this as internal/advanced use)
enum HvkFontFlags_
{
    HvkFontFlags_None                    = 0,
    HvkFontFlags_NoLoadError             = 1 << 1,   // Disable throwing an error/assert when calling AddFontXXX() with missing file/data. Calling code is expected to check AddFontXXX() return value.
    HvkFontFlags_NoLoadGlyphs            = 1 << 2,   // [Internal] Disable loading new glyphs.
    HvkFontFlags_LockBakedSizes          = 1 << 3,   // [Internal] Disable loading new baked sizes, disable garbage collecting current ones. e.g. if you want to lock a font to a single size. Hvkportant: if you use this to preload given sizes, consider the possibility of multiple font density used on Retina display.
};

// Font runtime data and rendering
// - HvkFontAtlas automatically loads a default embedded font for you if you didn't load one manually.
// - Since 1.92.0 a font may be rendered as any size! Therefore a font doesn't have one specific size.
// - Use 'font->GetFontBaked(size)' to retrieve the HvkFontBaked* corresponding to a given size.
// - If you used g.Font + g.FontSize (which is frequent from the HvkGui layer), you can use g.FontBaked as a shortcut, as g.FontBaked == g.Font->GetFontBaked(g.FontSize).
struct HvkFont
{
    // [Internal] Members: Hot ~12-20 bytes
    HvkFontBaked*                LastBaked;          // 4-8   // Cache last bound baked. NEVER USE DIRECTLY. Use GetFontBaked().
    HvkFontAtlas*                OwnerAtlas;         // 4-8   // What we have been loaded into.
    HvkFontFlags                 Flags;              // 4     // Font flags.
    float                       CurrentRasterizerDensity;    // Current rasterizer density. This is a varying state of the font.

    // [Internal] Members: Cold ~24-52 bytes
    // Conceptually Sources[] is the list of font sources merged to create this font.
    HvkGuiID                     FontId;             // Unique identifier for the font
    float                       LegacySize;         // 4     // in  // Font size passed to AddFont(). Use for old code calling PushFont() expecting to use that size. (use HvkGui::GetFontBaked() to get font baked at current bound size).
    HvkVector<HvkFontConfig*>     Sources;            // 16    // in  // List of sources. Pointers within OwnerAtlas->Sources[]
    HvkWchar                     EllipsisChar;       // 2-4   // out // Character used for ellipsis rendering ('...').
    HvkWchar                     FallbackChar;       // 2-4   // out // Character used if a glyph isn't found (U+FFFD, '?')
    HvkU8                        Used8kPagesMap[(Hvk_UNICODE_CODEPOINT_MAX+1)/8192/8]; // 1 bytes if HvkWchar=HvkWchar16, 16 bytes if HvkWchar==HvkWchar32. Store 1-bit for each block of 4K codepoints that has one active glyph. This is mainly used to facilitate iterations across all used codepoints.
    bool                        EllipsisAutoBake;   // 1     //     // Mark when the "..." glyph needs to be generated.
    HvkGuiStorage                RemapPairs;         // 16    //     // Remapping pairs when using AddRemapChar(), otherwise empty.
#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    float                       Scale;              // 4     // in  // Legacy base font scale (~1.0f), multiplied by the per-window font scale which you can adjust with SetWindowFontScale()
#endif

    // Methods
    HvkGui_API HvkFont();
    HvkGui_API ~HvkFont();
    HvkGui_API bool              IsGlyphInFont(HvkWchar c);
    bool                        IsLoaded() const                { return OwnerAtlas != NULL; }
    const char*                 GetDebugName() const            { return Sources.Size ? Sources[0]->Name : "<unknown>"; } // Fill HvkFontConfig::Name.

    // [Internal] Don't use!
    // 'max_width' stops rendering after a certain width (could be turned into a 2d size). FLT_MAX to disable.
    // 'wrap_width' enable automatic word-wrapping across multiple lines to fit into given width. 0.0f to disable.
    HvkGui_API HvkFontBaked*      GetFontBaked(float font_size, float density = -1.0f);  // Get or create baked data for given size
    HvkGui_API HvkVec2            CalcTextSizeA(float size, float max_width, float wrap_width, const char* text_begin, const char* text_end = NULL, const char** out_remaining = NULL);
    HvkGui_API const char*       CalcWordWrapPosition(float size, const char* text, const char* text_end, float wrap_width);
    HvkGui_API void              RenderChar(HvkDrawList* draw_list, float size, const HvkVec2& pos, HvkU32 col, HvkWchar c, const HvkVec4* cpu_fine_clip = NULL);
    HvkGui_API void              RenderText(HvkDrawList* draw_list, float size, const HvkVec2& pos, HvkU32 col, const HvkVec4& clip_rect, const char* text_begin, const char* text_end, float wrap_width = 0.0f, HvkDrawTextFlags flags = 0);
#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    inline const char*          CalcWordWrapPositionA(float scale, const char* text, const char* text_end, float wrap_width) { return CalcWordWrapPosition(LegacySize * scale, text, text_end, wrap_width); }
#endif

    // [Internal] Don't use!
    HvkGui_API void              ClearOutputData();
    HvkGui_API void              AddRemapChar(HvkWchar from_codepoint, HvkWchar to_codepoint); // Makes 'from_codepoint' character points to 'to_codepoint' glyph.
    HvkGui_API bool              IsGlyphRangeUnused(unsigned int c_begin, unsigned int c_last);
};

// This is provided for consistency (but we don't actually use this)
inline HvkTextureID HvkTextureRef::GetTexID() const
{
    Hvk_ASSERT(!(_TexData != NULL && _TexID != HvkTextureID_Invalid));
    return _TexData ? _TexData->TexID : _TexID;
}

// Using an indirection to avoid patching HvkDrawCmd after a SetTexID() call (but this could be an alternative solution too)
inline HvkTextureID HvkDrawCmd::GetTexID() const
{
    // If you are getting this assert: A renderer backend with support for HvkGuiBackendFlags_RendererHasTextures (1.92)
    // must iterate and handle HvkTextureData requests stored in HvkDrawData::Textures[].
    HvkTextureID tex_id = TexRef._TexData ? TexRef._TexData->TexID : TexRef._TexID; // == TexRef.GetTexID() above.
    if (TexRef._TexData != NULL)
        Hvk_ASSERT(tex_id != HvkTextureID_Invalid && "HvkDrawCmd is referring to HvkTextureData that wasn't uploaded to graphics system. Backend must call HvkTextureData::SetTexID() after handling HvkTextureStatus_WantCreate request!");
    return tex_id;
}

//-----------------------------------------------------------------------------
// [SECTION] Viewports
//-----------------------------------------------------------------------------

// Flags stored in HvkGuiViewport::Flags, giving indications to the platform backends.
enum HvkGuiViewportFlags_
{
    HvkGuiViewportFlags_None                     = 0,
    HvkGuiViewportFlags_IsPlatformWindow         = 1 << 0,   // Represent a Platform Window
    HvkGuiViewportFlags_IsPlatformMonitor        = 1 << 1,   // Represent a Platform Monitor (unused yet)
    HvkGuiViewportFlags_OwnedByApp               = 1 << 2,   // Platform Window: Is created/managed by the application (rather than a dear HvkGui backend)
};

// - Currently represents the Platform Window created by the application which is hosting our Dear HvkGui windows.
// - In 'docking' branch with multi-viewport enabled, we extend this concept to have multiple active viewports.
// - In the future we will extend this concept further to also represent Platform Monitor and support a "no main platform window" operation mode.
// - About Main Area vs Work Area:
//   - Main Area = entire viewport.
//   - Work Area = entire viewport minus sections used by main menu bars (for platform windows), or by task bar (for platform monitor).
//   - Windows are generally trying to stay within the Work Area of their host viewport.
struct HvkGuiViewport
{
    HvkGuiID             ID;                     // Unique identifier for the viewport
    HvkGuiViewportFlags  Flags;                  // See HvkGuiViewportFlags_
    HvkVec2              Pos;                    // Main Area: Position of the viewport (Dear HvkGui coordinates are the same as OS desktop/native coordinates)
    HvkVec2              Size;                   // Main Area: Size of the viewport.
    HvkVec2              FramebufferScale;       // Density of the viewport for Retina display (always 1,1 on Windows, may be 2,2 etc on macOS/iOS). This will affect font rasterizer density.
    HvkVec2              WorkPos;                // Work Area: Position of the viewport minus task bars, menus bars, status bars (>= Pos)
    HvkVec2              WorkSize;               // Work Area: Size of the viewport minus task bars, menu bars, status bars (<= Size)

    // Platform/Backend Dependent Data
    void*               PlatformHandle;         // void* to hold higher-level, platform window handle (e.g. HWND, GLFWWindow*, SDL_Window*)
    void*               PlatformHandleRaw;      // void* to hold lower-level, platform-native window handle (under Win32 this is expected to be a HWND, unused for other platforms)

    HvkGuiViewport()     { memset(this, 0, sizeof(*this)); }

    // Helpers
    HvkVec2              GetCenter() const       { return HvkVec2(Pos.x + Size.x * 0.5f, Pos.y + Size.y * 0.5f); }
    HvkVec2              GetWorkCenter() const   { return HvkVec2(WorkPos.x + WorkSize.x * 0.5f, WorkPos.y + WorkSize.y * 0.5f); }
};

//-----------------------------------------------------------------------------
// [SECTION] Platform Dependent Interfaces
//-----------------------------------------------------------------------------

// Access via HvkGui::GetPlatformIO()
struct HvkGuiPlatformIO
{
    HvkGui_API HvkGuiPlatformIO();

    //------------------------------------------------------------------
    // Input - Interface with OS and Platform backend (most common stuff)
    //------------------------------------------------------------------

    // Optional: Access OS clipboard
    // (default to use native Win32 clipboard on Windows, otherwise uses a private clipboard. Override to access OS clipboard on other architectures)
    const char* (*Platform_GetClipboardTextFn)(HvkGuiContext* ctx);
    void        (*Platform_SetClipboardTextFn)(HvkGuiContext* ctx, const char* text);
    void*       Platform_ClipboardUserData;

    // Optional: Open link/folder/file in OS Shell
    // (default to use ShellExecuteW() on Windows, system() on Linux/Mac. expected to return false on failure, but some platforms may always return true)
    bool        (*Platform_OpenInShellFn)(HvkGuiContext* ctx, const char* path);
    void*       Platform_OpenInShellUserData;

    // Optional: Notify OS Input Method Editor of the screen position of your cursor for text input position (e.g. when using Japanese/Chinese IME on Windows)
    // (default to use native imm32 api on Windows)
    void        (*Platform_SetImeDataFn)(HvkGuiContext* ctx, HvkGuiViewport* viewport, HvkGuiPlatformImeData* data);
    void*       Platform_ImeUserData;
    //void      (*SetPlatformImeDataFn)(HvkGuiViewport* viewport, HvkGuiPlatformImeData* data); // [Renamed to platform_io.PlatformSetImeDataFn in 1.91.1]

    // Optional: Platform locale
    // [Experimental] Configure decimal point e.g. '.' or ',' useful for some languages (e.g. German), generally pulled from *localeconv()->decimal_point
    HvkWchar     Platform_LocaleDecimalPoint;     // '.'

    //------------------------------------------------------------------
    // Input - Interface with Renderer Backend
    //------------------------------------------------------------------

    // Optional: Maximum texture size supported by renderer (used to adjust how we size textures). 0 if not known.
    int         Renderer_TextureMaxWidth;
    int         Renderer_TextureMaxHeight;

    // Written by some backends during HvkGui_ImplXXXX_RenderDrawData() call to point backend_specific HvkGui_ImplXXXX_RenderState* structure.
    void*       Renderer_RenderState;

    //------------------------------------------------------------------
    // Output
    //------------------------------------------------------------------

    // Textures list (the list is updated by calling HvkGui::EndFrame or HvkGui::Render)
    // The HvkGui_ImplXXXX_RenderDrawData() function of each backend generally access this via HvkDrawData::Textures which points to this. The array is available here mostly because backends will want to destroy textures on shutdown.
    HvkVector<HvkTextureData*>        Textures;           // List of textures used by Dear HvkGui (most often 1) + contents of external texture list is automatically appended into this.

    //------------------------------------------------------------------
    // Functions
    //------------------------------------------------------------------

    HvkGui_API void ClearPlatformHandlers();    // Clear all Platform_XXX fields. Typically called on Platform Backend shutdown.
    HvkGui_API void ClearRendererHandlers();    // Clear all Renderer_XXX fields. Typically called on Renderer Backend shutdown.
};

// (Optional) Support for IME (Input Method Editor) via the platform_io.Platform_SetImeDataFn() function. Handler is called during EndFrame().
struct HvkGuiPlatformImeData
{
    bool    WantVisible;            // A widget wants the IME to be visible.
    bool    WantTextInput;          // A widget wants text input, not necessarily IME to be visible. This is automatically set to the upcoming value of io.WantTextInput.
    HvkVec2  InputPos;               // Position of input cursor (for IME).
    float   InputLineHeight;        // Line height (for IME).
    HvkGuiID ViewportId;             // ID of platform window/viewport.

    HvkGuiPlatformImeData()          { memset(this, 0, sizeof(*this)); }
};

//-----------------------------------------------------------------------------
// [SECTION] Obsolete functions and types
// (Will be removed! Read 'API BREAKING CHANGES' section in HvkGui.cpp for details)
// Please keep your copy of dear HvkGui up to date! Occasionally set '#define HvkGui_DISABLE_OBSOLETE_FUNCTIONS' in Hvkconfig.h to stay ahead.
//-----------------------------------------------------------------------------

#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
namespace HvkGui
{
    // OBSOLETED in 1.92.0 (from June 2025)
    inline void         PushFont(HvkFont* font)                                  { PushFont(font, font ? font->LegacySize : 0.0f); }
    HvkGui_API void      SetWindowFontScale(float scale);                        // Set font scale factor for current window. Prefer using PushFont(NULL, style.FontSizeBase * factor) or use style.FontScaleMain to scale all windows.
    // OBSOLETED in 1.91.9 (from February 2025)
    HvkGui_API void      Hvkage(HvkTextureRef tex_ref, const HvkVec2& Hvkage_size, const HvkVec2& uv0, const HvkVec2& uv1, const HvkVec4& tint_col, const HvkVec4& border_col); // <-- 'border_col' was removed in favor of HvkGuiCol_ImageBorder. If you use 'tint_col', use HvkageWithBg() instead.
    // OBSOLETED in 1.91.0 (from July 2024)
    inline void         PushButtonRepeat(bool repeat)                           { PushItemFlag(HvkGuiItemFlags_ButtonRepeat, repeat); }
    inline void         PopButtonRepeat()                                       { PopItemFlag(); }
    inline void         PushTabStop(bool tab_stop)                              { PushItemFlag(HvkGuiItemFlags_NoTabStop, !tab_stop); }
    inline void         PopTabStop()                                            { PopItemFlag(); }
    HvkGui_API HvkVec2    GetContentRegionMax();                                  // Content boundaries max (e.g. window boundaries including scrolling, or current column boundaries). You should never need this. Always use GetCursorScreenPos() and GetContentRegionAvail()!
    HvkGui_API HvkVec2    GetWindowContentRegionMin();                            // Content boundaries min for the window (roughly (0,0)-Scroll), in window-local coordinates. You should never need this. Always use GetCursorScreenPos() and GetContentRegionAvail()!
    HvkGui_API HvkVec2    GetWindowContentRegionMax();                            // Content boundaries max for the window (roughly (0,0)+Size-Scroll), in window-local coordinates. You should never need this. Always use GetCursorScreenPos() and GetContentRegionAvail()!
    // OBSOLETED in 1.90.0 (from September 2023)
    inline bool         BeginChildFrame(HvkGuiID id, const HvkVec2& size, HvkGuiWindowFlags window_flags = 0)  { return BeginChild(id, size, HvkGuiChildFlags_FrameStyle, window_flags); }
    inline void         EndChildFrame()                                                                     { EndChild(); }
    //inline bool       BeginChild(const char* str_id, const HvkVec2& size_arg, bool borders, HvkGuiWindowFlags window_flags){ return BeginChild(str_id, size_arg, borders ? HvkGuiChildFlags_Borders : HvkGuiChildFlags_None, window_flags); } // Unnecessary as true == HvkGuiChildFlags_Borders
    //inline bool       BeginChild(HvkGuiID id, const HvkVec2& size_arg, bool borders, HvkGuiWindowFlags window_flags)        { return BeginChild(id, size_arg, borders ? HvkGuiChildFlags_Borders : HvkGuiChildFlags_None, window_flags);     } // Unnecessary as true == HvkGuiChildFlags_Borders
    inline void         ShowStackToolWindow(bool* p_open = NULL)                { ShowIDStackToolWindow(p_open); }
    HvkGui_API bool      Combo(const char* label, int* current_item, bool (*old_callback)(void* user_data, int idx, const char** out_text), void* user_data, int items_count, int popup_max_height_in_items = -1);
    HvkGui_API bool      ListBox(const char* label, int* current_item, bool (*old_callback)(void* user_data, int idx, const char** out_text), void* user_data, int items_count, int height_in_items = -1);

    // Some of the older obsolete names along with their replacement (commented out so they are not reported in IDE)
    // OBSOLETED in 1.89.7 (from June 2023)
    //HvkGui_API void      SetItemAllowOverlap();                                                      // Use SetNextItemAllowOverlap() _before_ item.
    //-- OBSOLETED in 1.89.4 (from March 2023)
    //static inline void  PushAllowKeyboardFocus(bool tab_stop)                                       { PushItemFlag(HvkGuiItemFlags_NoTabStop, !tab_stop); }
    //static inline void  PopAllowKeyboardFocus()                                                     { PopItemFlag(); }
    //-- OBSOLETED in 1.89 (from August 2022)
    //HvkGui_API bool      HvkageButton(HvkTextureID user_texture_id, const HvkVec2& size, const HvkVec2& uv0 = HvkVec2(0, 0), const HvkVec2& uv1 = HvkVec2(1, 1), int frame_padding = -1, const HvkVec4& bg_col = HvkVec4(0, 0, 0, 0), const HvkVec4& tint_col = HvkVec4(1, 1, 1, 1)); // --> Use new HvkageButton() signature (explicit item id, regular FramePadding). Refer to code in 1.91 if you want to grab a copy of this version.
    //-- OBSOLETED in 1.88 (from May 2022)
    //static inline void  CaptureKeyboardFromApp(bool want_capture_keyboard = true)                   { SetNextFrameWantCaptureKeyboard(want_capture_keyboard); } // Renamed as name was misleading + removed default value.
    //static inline void  CaptureMouseFromApp(bool want_capture_mouse = true)                         { SetNextFrameWantCaptureMouse(want_capture_mouse); }       // Renamed as name was misleading + removed default value.
    //-- OBSOLETED in 1.87 (from February 2022, more formally obsoleted April 2024)
    //HvkGui_API HvkGuiKey  GetKeyIndex(HvkGuiKey key);                                                  { Hvk_ASSERT(key >= HvkGuiKey_NamedKey_BEGIN && key < HvkGuiKey_NamedKey_END); const HvkGuiKeyData* key_data = GetKeyData(key); return (HvkGuiKey)(key_data - g.IO.KeysData); } // Map HvkGuiKey_* values into legacy native key index. == io.KeyMap[key]. When using a 1.87+ backend using io.AddKeyEvent(), calling GetKeyIndex() with ANY HvkGuiKey_XXXX values will return the same value!
    //static inline HvkGuiKey GetKeyIndex(HvkGuiKey key)                                                { Hvk_ASSERT(key >= HvkGuiKey_NamedKey_BEGIN && key < HvkGuiKey_NamedKey_END); return key; }
    //-- OBSOLETED in 1.86 (from November 2021)
    //HvkGui_API void      CalcListClipping(int items_count, float items_height, int* out_items_display_start, int* out_items_display_end); // Code removed, see 1.90 for last version of the code. Calculate range of visible items for large list of evenly sized items. Prefer using HvkGuiListClipper.
    //-- OBSOLETED in 1.85 (from August 2021)
    //static inline float GetWindowContentRegionWidth()                                               { return GetWindowContentRegionMax().x - GetWindowContentRegionMin().x; }
    //-- OBSOLETED in 1.81 (from February 2021)
    //static inline bool  ListBoxHeader(const char* label, const HvkVec2& size = HvkVec2(0, 0))         { return BeginListBox(label, size); }
    //static inline bool  ListBoxHeader(const char* label, int items_count, int height_in_items = -1) { float height = GetTextLineHeightWithSpacing() * ((height_in_items < 0 ? Immin(items_count, 7) : height_in_items) + 0.25f) + GetStyle().FramePadding.y * 2.0f; return BeginListBox(label, HvkVec2(0.0f, height)); } // Helper to calculate size from items_count and height_in_items
    //static inline void  ListBoxFooter()                                                             { EndListBox(); }
    //-- OBSOLETED in 1.79 (from August 2020)
    //static inline void  OpenPopupContextItem(const char* str_id = NULL, HvkGuiMouseButton mb = 1)    { OpenPopupOnItemClick(str_id, mb); } // Bool return value removed. Use IsWindowAppearing() in BeginPopup() instead. Renamed in 1.77, renamed back in 1.79. Sorry!
    //-- OBSOLETED in 1.78 (from June 2020): Old drag/sliders functions that took a 'float power > 1.0f' argument instead of HvkGuiSliderFlags_Logarithmic. See github.com/ocornut/HvkGui/issues/3361 for details.
    //HvkGui_API bool      DragScalar(const char* label, HvkGuiDataType data_type, void* p_data, float v_speed, const void* p_min, const void* p_max, const char* format, float power = 1.0f)                                                            // OBSOLETED in 1.78 (from June 2020)
    //HvkGui_API bool      DragScalarN(const char* label, HvkGuiDataType data_type, void* p_data, int components, float v_speed, const void* p_min, const void* p_max, const char* format, float power = 1.0f);                                          // OBSOLETED in 1.78 (from June 2020)
    //HvkGui_API bool      SliderScalar(const char* label, HvkGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, float power = 1.0f);                                                                        // OBSOLETED in 1.78 (from June 2020)
    //HvkGui_API bool      SliderScalarN(const char* label, HvkGuiDataType data_type, void* p_data, int components, const void* p_min, const void* p_max, const char* format, float power = 1.0f);                                                       // OBSOLETED in 1.78 (from June 2020)
    //static inline bool  DragFloat(const char* label, float* v, float v_speed, float v_min, float v_max, const char* format, float power = 1.0f)    { return DragScalar(label, HvkGuiDataType_Float, v, v_speed, &v_min, &v_max, format, power); }     // OBSOLETED in 1.78 (from June 2020)
    //static inline bool  DragFloat2(const char* label, float v[2], float v_speed, float v_min, float v_max, const char* format, float power = 1.0f) { return DragScalarN(label, HvkGuiDataType_Float, v, 2, v_speed, &v_min, &v_max, format, power); } // OBSOLETED in 1.78 (from June 2020)
    //static inline bool  DragFloat3(const char* label, float v[3], float v_speed, float v_min, float v_max, const char* format, float power = 1.0f) { return DragScalarN(label, HvkGuiDataType_Float, v, 3, v_speed, &v_min, &v_max, format, power); } // OBSOLETED in 1.78 (from June 2020)
    //static inline bool  DragFloat4(const char* label, float v[4], float v_speed, float v_min, float v_max, const char* format, float power = 1.0f) { return DragScalarN(label, HvkGuiDataType_Float, v, 4, v_speed, &v_min, &v_max, format, power); } // OBSOLETED in 1.78 (from June 2020)
    //static inline bool  SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, float power = 1.0f)                 { return SliderScalar(label, HvkGuiDataType_Float, v, &v_min, &v_max, format, power); }            // OBSOLETED in 1.78 (from June 2020)
    //static inline bool  SliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* format, float power = 1.0f)              { return SliderScalarN(label, HvkGuiDataType_Float, v, 2, &v_min, &v_max, format, power); }        // OBSOLETED in 1.78 (from June 2020)
    //static inline bool  SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format, float power = 1.0f)              { return SliderScalarN(label, HvkGuiDataType_Float, v, 3, &v_min, &v_max, format, power); }        // OBSOLETED in 1.78 (from June 2020)
    //static inline bool  SliderFloat4(const char* label, float v[4], float v_min, float v_max, const char* format, float power = 1.0f)              { return SliderScalarN(label, HvkGuiDataType_Float, v, 4, &v_min, &v_max, format, power); }        // OBSOLETED in 1.78 (from June 2020)
    //-- OBSOLETED in 1.77 and before
    //static inline bool  BeginPopupContextWindow(const char* str_id, HvkGuiMouseButton mb, bool over_items) { return BeginPopupContextWindow(str_id, mb | (over_items ? 0 : HvkGuiPopupFlags_NoOpenOverItems)); } // OBSOLETED in 1.77 (from June 2020)
    //static inline void  TreeAdvanceToLabelPos()               { SetCursorPosX(GetCursorPosX() + GetTreeNodeToLabelSpacing()); }   // OBSOLETED in 1.72 (from July 2019)
    //static inline void  SetNextTreeNodeOpen(bool open, HvkGuiCond cond = 0) { SetNextItemOpen(open, cond); }                       // OBSOLETED in 1.71 (from June 2019)
    //static inline float GetContentRegionAvailWidth()          { return GetContentRegionAvail().x; }                               // OBSOLETED in 1.70 (from May 2019)
    //static inline HvkDrawList* GetOverlayDrawList()            { return GetForegroundDrawList(); }                                 // OBSOLETED in 1.69 (from Mar 2019)
    //static inline void  SetScrollHere(float ratio = 0.5f)     { SetScrollHereY(ratio); }                                          // OBSOLETED in 1.66 (from Nov 2018)
    //static inline bool  IsItemDeactivatedAfterChange()        { return IsItemDeactivatedAfterEdit(); }                            // OBSOLETED in 1.63 (from Aug 2018)
    //-- OBSOLETED in 1.60 and before
    //static inline bool  IsAnyWindowFocused()                  { return IsWindowFocused(HvkGuiFocusedFlags_AnyWindow); }            // OBSOLETED in 1.60 (from Apr 2018)
    //static inline bool  IsAnyWindowHovered()                  { return IsWindowHovered(HvkGuiHoveredFlags_AnyWindow); }            // OBSOLETED in 1.60 (between Dec 2017 and Apr 2018)
    //static inline void  ShowTestWindow()                      { return ShowDemoWindow(); }                                        // OBSOLETED in 1.53 (between Oct 2017 and Dec 2017)
    //static inline bool  IsRootWindowFocused()                 { return IsWindowFocused(HvkGuiFocusedFlags_RootWindow); }           // OBSOLETED in 1.53 (between Oct 2017 and Dec 2017)
    //static inline bool  IsRootWindowOrAnyChildFocused()       { return IsWindowFocused(HvkGuiFocusedFlags_RootAndChildWindows); }  // OBSOLETED in 1.53 (between Oct 2017 and Dec 2017)
    //static inline void  SetNextWindowContentWidth(float w)    { SetNextWindowContentSize(HvkVec2(w, 0.0f)); }                      // OBSOLETED in 1.53 (between Oct 2017 and Dec 2017)
    //static inline float GetItemsLineHeightWithSpacing()       { return GetFrameHeightWithSpacing(); }                             // OBSOLETED in 1.53 (between Oct 2017 and Dec 2017)
    //HvkGui_API bool      Begin(char* name, bool* p_open, HvkVec2 size_first_use, float bg_alpha = -1.0f, HvkGuiWindowFlags flags=0); // OBSOLETED in 1.52 (between Aug 2017 and Oct 2017): Equivalent of using SetNextWindowSize(size, HvkGuiCond_FirstUseEver) and SetNextWindowBgAlpha().
    //static inline bool  IsRootWindowOrAnyChildHovered()       { return IsWindowHovered(HvkGuiHoveredFlags_RootAndChildWindows); }  // OBSOLETED in 1.52 (between Aug 2017 and Oct 2017)
    //static inline void  AlignFirstTextHeightToWidgets()       { AlignTextToFramePadding(); }                                      // OBSOLETED in 1.52 (between Aug 2017 and Oct 2017)
    //static inline void  SetNextWindowPosCenter(HvkGuiCond c=0) { SetNextWindowPos(GetMainViewport()->GetCenter(), c, HvkVec2(0.5f,0.5f)); } // OBSOLETED in 1.52 (between Aug 2017 and Oct 2017)
    //static inline bool  IsItemHoveredRect()                   { return IsItemHovered(HvkGuiHoveredFlags_RectOnly); }               // OBSOLETED in 1.51 (between Jun 2017 and Aug 2017)
    //static inline bool  IsPosHoveringAnyWindow(const HvkVec2&) { Hvk_ASSERT(0); return false; }                                     // OBSOLETED in 1.51 (between Jun 2017 and Aug 2017): This was misleading and partly broken. You probably want to use the io.WantCaptureMouse flag instead.
    //static inline bool  IsMouseHoveringAnyWindow()            { return IsWindowHovered(HvkGuiHoveredFlags_AnyWindow); }            // OBSOLETED in 1.51 (between Jun 2017 and Aug 2017)
    //static inline bool  IsMouseHoveringWindow()               { return IsWindowHovered(HvkGuiHoveredFlags_AllowWhenBlockedByPopup | HvkGuiHoveredFlags_AllowWhenBlockedByActiveItem); }       // OBSOLETED in 1.51 (between Jun 2017 and Aug 2017)
    //-- OBSOLETED in 1.50 and before
    //static inline bool  CollapsingHeader(char* label, const char* str_id, bool framed = true, bool default_open = false) { return CollapsingHeader(label, (default_open ? (1 << 5) : 0)); } // OBSOLETED in 1.49
    //static inline HvkFont*GetWindowFont()                      { return GetFont(); }                                               // OBSOLETED in 1.48
    //static inline float GetWindowFontSize()                   { return GetFontSize(); }                                           // OBSOLETED in 1.48
    //static inline void  SetScrollPosHere()                    { SetScrollHere(); }                                                // OBSOLETED in 1.42
}

//-- OBSOLETED in 1.92.0: HvkFontAtlasCustomRect becomes HvkTextureRect
// - HvkFontAtlasCustomRect::X,Y          --> HvkTextureRect::x,y
// - HvkFontAtlasCustomRect::Width,Height --> HvkTextureRect::w,h
// - HvkFontAtlasCustomRect::GlyphColored --> if you need to write to this, instead you can write to 'font->Glyphs.back()->Colored' after calling AddCustomRectFontGlyph()
// We could make HvkTextureRect an union to use old names, but 1) this would be confusing 2) the fix is easy 3) HvkFontAtlasCustomRect was always a rather esoteric api.
typedef HvkFontAtlasRect HvkFontAtlasCustomRect;
/*struct HvkFontAtlasCustomRect
{
    unsigned short  X, Y;           // Output   // Packed position in Atlas
    unsigned short  Width, Height;  // Input    // [Internal] Desired rectangle dimension
    unsigned int    GlyphID:31;     // Input    // [Internal] For custom font glyphs only (ID < 0x110000)
    unsigned int    GlyphColored:1; // Input    // [Internal] For custom font glyphs only: glyph is colored, removed tinting.
    float           GlyphAdvanceX;  // Input    // [Internal] For custom font glyphs only: glyph xadvance
    HvkVec2          GlyphOffset;    // Input    // [Internal] For custom font glyphs only: glyph display offset
    HvkFont*         Font;           // Input    // [Internal] For custom font glyphs only: target font
    HvkFontAtlasCustomRect()         { X = Y = 0xFFFF; Width = Height = 0; GlyphID = 0; GlyphColored = 0; GlyphAdvanceX = 0.0f; GlyphOffset = HvkVec2(0, 0); Font = NULL; }
    bool IsPacked() const           { return X != 0xFFFF; }
};*/

//-- OBSOLETED in 1.82 (from Mars 2021): flags for AddRect(), AddRectFilled(), AddImageRounded(), PathRect()
//typedef HvkDrawFlags HvkDrawCornerFlags;
//enum HvkDrawCornerFlags_
//{
//    HvkDrawCornerFlags_None      = HvkDrawFlags_RoundCornersNone,         // Was == 0 prior to 1.82, this is now == HvkDrawFlags_RoundCornersNone which is != 0 and not Hvkplicit
//    HvkDrawCornerFlags_TopLeft   = HvkDrawFlags_RoundCornersTopLeft,      // Was == 0x01 (1 << 0) prior to 1.82. Order matches HvkDrawFlags_NoRoundCorner* flag (we exploit this internally).
//    HvkDrawCornerFlags_TopRight  = HvkDrawFlags_RoundCornersTopRight,     // Was == 0x02 (1 << 1) prior to 1.82.
//    HvkDrawCornerFlags_BotLeft   = HvkDrawFlags_RoundCornersBottomLeft,   // Was == 0x04 (1 << 2) prior to 1.82.
//    HvkDrawCornerFlags_BotRight  = HvkDrawFlags_RoundCornersBottomRight,  // Was == 0x08 (1 << 3) prior to 1.82.
//    HvkDrawCornerFlags_All       = HvkDrawFlags_RoundCornersAll,          // Was == 0x0F prior to 1.82
//    HvkDrawCornerFlags_Top       = HvkDrawCornerFlags_TopLeft | HvkDrawCornerFlags_TopRight,
//    HvkDrawCornerFlags_Bot       = HvkDrawCornerFlags_BotLeft | HvkDrawCornerFlags_BotRight,
//    HvkDrawCornerFlags_Left      = HvkDrawCornerFlags_TopLeft | HvkDrawCornerFlags_BotLeft,
//    HvkDrawCornerFlags_Right     = HvkDrawCornerFlags_TopRight | HvkDrawCornerFlags_BotRight,
//};

// RENAMED and MERGED both HvkGuiKey_ModXXX and HvkGuiModFlags_XXX into HvkGuiMod_XXX (from September 2022)
// RENAMED HvkGuiKeyModFlags -> HvkGuiModFlags in 1.88 (from April 2022). Exceptionally commented out ahead of obsolescence schedule to reduce confusion and because they were not meant to be used in the first place.
//typedef HvkGuiKeyChord HvkGuiModFlags;      // == int. We generally use HvkGuiKeyChord to mean "a HvkGuiKey or-ed with any number of HvkGuiMod_XXX value", so you may store mods in there.
//enum HvkGuiModFlags_ { HvkGuiModFlags_None = 0, HvkGuiModFlags_Ctrl = HvkGuiMod_Ctrl, HvkGuiModFlags_Shift = HvkGuiMod_Shift, HvkGuiModFlags_Alt = HvkGuiMod_Alt, HvkGuiModFlags_Super = HvkGuiMod_Super };
//typedef HvkGuiKeyChord HvkGuiKeyModFlags; // == int
//enum HvkGuiKeyModFlags_ { HvkGuiKeyModFlags_None = 0, HvkGuiKeyModFlags_Ctrl = HvkGuiMod_Ctrl, HvkGuiKeyModFlags_Shift = HvkGuiMod_Shift, HvkGuiKeyModFlags_Alt = HvkGuiMod_Alt, HvkGuiKeyModFlags_Super = HvkGuiMod_Super };

#define Hvk_OFFSETOF(_TYPE,_MEMBER)  offsetof(_TYPE, _MEMBER)    // OBSOLETED IN 1.90 (now using C++11 standard version)

#endif // #ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS

// RENAMED HvkGui_DISABLE_METRICS_WINDOW > HvkGui_DISABLE_DEBUG_TOOLS in 1.88 (from June 2022)
#ifdef HvkGui_DISABLE_METRICS_WINDOW
#error HvkGui_DISABLE_METRICS_WINDOW was renamed to HvkGui_DISABLE_DEBUG_TOOLS, please use new name.
#endif

//-----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning (pop)
#endif

// Include HvkGui_user.h at the end of HvkGui.h
// May be convenient for some users to only explicitly include vanilla HvkGui.h and have extra stuff included.
#ifdef HvkGui_INCLUDE_HvkGui_USER_H
#ifdef HvkGui_USER_H_FILENAME
#include HvkGui_USER_H_FILENAME
#else
#include "HvkGui_user.h"
#endif
#endif

#endif // #ifndef HvkGui_DISABLE
