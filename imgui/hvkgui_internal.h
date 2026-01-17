// dear HvkGui, v1.92.6 WIP
// (internal structures/api)

// You may use this file to debug, understand or extend Dear HvkGui features but we don't provide any guarantee of forward compatibility.

/*

Index of this file:

// [SECTION] Header mess
// [SECTION] Forward declarations
// [SECTION] Context pointer
// [SECTION] STB libraries includes
// [SECTION] Macros
// [SECTION] Generic helpers
// [SECTION] HvkDrawList support
// [SECTION] Style support
// [SECTION] Data types support
// [SECTION] Widgets support: flags, enums, data structures
// [SECTION] Popup support
// [SECTION] Inputs support
// [SECTION] Clipper support
// [SECTION] Navigation support
// [SECTION] Typing-select support
// [SECTION] Columns support
// [SECTION] Box-select support
// [SECTION] Multi-select support
// [SECTION] Docking support
// [SECTION] Viewport support
// [SECTION] Settings support
// [SECTION] Localization support
// [SECTION] Error handling, State recovery support
// [SECTION] Metrics, Debug tools
// [SECTION] Generic context hooks
// [SECTION] HvkGuiContext (main HvkGui context)
// [SECTION] HvkGuiWindowTempData, HvkGuiWindow
// [SECTION] Tab bar, Tab item support
// [SECTION] Table support
// [SECTION] HvkGui internal API
// [SECTION] HvkFontLoader
// [SECTION] HvkFontAtlas internal API
// [SECTION] Test Engine specific hooks (HvkGui_test_engine)

*/

#pragma once
#ifndef HvkGui_DISABLE

//-----------------------------------------------------------------------------
// [SECTION] Header mess
//-----------------------------------------------------------------------------

#ifndef HvkGui_VERSION
#include "hvkgui.h"
#endif

#include <stdio.h>      // FILE*, sscanf
#include <stdlib.h>     // NULL, malloc, free, qsort, atoi, atof
#include <math.h>       // sqrtf, fabsf, fmodf, powf, floorf, ceilf, cosf, sinf
#include <limits.h>     // INT_MIN, INT_MAX

// Enable SSE intrinsics if available
#if (defined __SSE__ || defined __x86_64__ || defined _M_X64 || (defined(_M_IX86_FP) && (_M_IX86_FP >= 1))) && !defined(HvkGui_DISABLE_SSE)
#define HvkGui_ENABLE_SSE
#include <Hvkmintrin.h>
#if (defined __AVX__ || defined __SSE4_2__)
#define HvkGui_ENABLE_SSE4_2
#include <nmmintrin.h>
#endif
#endif
// Emscripten has partial SSE 4.2 support where _mm_crc32_u32 is not available. See https://emscripten.org/docs/porting/simd.html#id11 and #8213
#if defined(HvkGui_ENABLE_SSE4_2) && !defined(HvkGui_USE_LEGACY_CRC32_ADLER) && !defined(__EMSCRIPTEN__)
#define HvkGui_ENABLE_SSE4_2_CRC
#endif

// Visual Studio warnings
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4251)     // class 'xxx' needs to have dll-interface to be used by clients of struct 'xxx' // when HvkGui_API is set to__declspec(dllexport)
#pragma warning (disable: 26495)    // [Static Analyzer] Variable 'XXX' is uninitialized. Always initialize a member variable (type.6).
#pragma warning (disable: 26812)    // [Static Analyzer] The enum type 'xxx' is unscoped. Prefer 'enum class' over 'enum' (Enum.3).
#if defined(_MSC_VER) && _MSC_VER >= 1922 // MSVC 2019 16.2 or later
#pragma warning (disable: 5054)     // operator '|': deprecated between enumerations of different types
#endif
#endif

// Clang/GCC warnings with -Weverything
#if defined(__clang__)
#pragma clang diagnostic push
#if __has_warning("-Wunknown-warning-option")
#pragma clang diagnostic ignored "-Wunknown-warning-option"         // warning: unknown warning group 'xxx'
#endif
#pragma clang diagnostic ignored "-Wunknown-pragmas"                // warning: unknown warning group 'xxx'
#pragma clang diagnostic ignored "-Wfloat-equal"                    // warning: comparing floating point with == or != is unsafe // storing and comparing against same constants ok, for HvkFloor()
#pragma clang diagnostic ignored "-Wold-style-cast"                 // warning: use of old-style cast
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"  // warning: zero as null pointer constant
#pragma clang diagnostic ignored "-Wdouble-promotion"               // warning: Hvkplicit conversion from 'float' to 'double' when passing argument to function
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"  // warning: Hvkplicit conversion from 'xxx' to 'float' may lose precision
#pragma clang diagnostic ignored "-Wmissing-noreturn"               // warning: function 'xxx' could be declared with attribute 'noreturn'
#pragma clang diagnostic ignored "-Wdeprecated-enum-enum-conversion"// warning: bitwise operation between different enumeration types ('XXXFlags_' and 'XXXFlagsPrivate_') is deprecated
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"            // warning: 'xxx' is an unsafe pointer used for buffer access
#pragma clang diagnostic ignored "-Wnontrivial-memaccess"           // warning: first argument in call to 'memset' is a pointer to non-trivially copyable type
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"                          // warning: unknown option after '#pragma GCC diagnostic' kind
#pragma GCC diagnostic ignored "-Wfloat-equal"                      // warning: comparing floating-point with '==' or '!=' is unsafe
#pragma GCC diagnostic ignored "-Wclass-memaccess"                  // [__GNUC__ >= 8] warning: 'memset/memcpy' clearing/writing an object of type 'xxxx' with no trivial copy-assignment; use assignment or value-initialization instead
#pragma GCC diagnostic ignored "-Wdeprecated-enum-enum-conversion"  // warning: bitwise operation between different enumeration types ('XXXFlags_' and 'XXXFlagsPrivate_') is deprecated
#endif

// In 1.89.4, we moved the Hvkplementation of "courtesy maths operators" from HvkGui_internal.h in HvkGui.h
// As they are frequently requested, we do not want to encourage to many people using HvkGui_internal.h
#if defined(HvkGui_DEFINE_MATH_OPERATORS) && !defined(HvkGui_DEFINE_MATH_OPERATORS_IMPLEMENTED)
#error Please '#define HvkGui_DEFINE_MATH_OPERATORS' _BEFORE_ including HvkGui.h!
#endif

// Legacy defines
#ifdef HvkGui_DISABLE_FORMAT_STRING_FUNCTIONS            // Renamed in 1.74
#error Use HvkGui_DISABLE_DEFAULT_FORMAT_FUNCTIONS
#endif
#ifdef HvkGui_DISABLE_MATH_FUNCTIONS                     // Renamed in 1.74
#error Use HvkGui_DISABLE_DEFAULT_MATH_FUNCTIONS
#endif

// Enable stb_truetype by default unless FreeType is enabled.
// You can compile with both by defining both HvkGui_ENABLE_FREETYPE and HvkGui_ENABLE_STB_TRUETYPE together.
#ifndef HvkGui_ENABLE_FREETYPE
#define HvkGui_ENABLE_STB_TRUETYPE
#endif

//-----------------------------------------------------------------------------
// [SECTION] Forward declarations
//-----------------------------------------------------------------------------

// Utilities
// (other types which are not forwarded declared are: HvkBitArray<>, HvkSpan<>, HvkSpanAllocator<>, HvkStableVector<>, HvkPool<>, HvkChunkStream<>)
struct HvkBitVector;                 // Store 1-bit per value
struct HvkRect;                      // An axis-aligned rectangle (2 points)
struct HvkGuiTextIndex;              // Maintain a line index for a text buffer.

// HvkDrawList/HvkFontAtlas
struct HvkDrawDataBuilder;           // Helper to build a HvkDrawData instance
struct HvkDrawListSharedData;        // Data shared between all HvkDrawList instances
struct HvkFontAtlasBuilder;          // Internal storage for incrementally packing and building a HvkFontAtlas
struct HvkFontAtlasPostProcessData;  // Data available to potential texture post-processing functions
struct HvkFontAtlasRectEntry;        // Packed rectangle lookup entry

// HvkGui
struct HvkGuiBoxSelectState;         // Box-selection state (currently used by multi-selection, could potentially be used by others)
struct HvkGuiColorMod;               // Stacked color modifier, backup of modified data so we can restore it
struct HvkGuiContext;                // Main Dear HvkGui context
struct HvkGuiContextHook;            // Hook for extensions like HvkGuiTestEngine
struct HvkGuiDataTypeInfo;           // Type information associated to a HvkGuiDataType enum
struct HvkGuiDeactivatedItemData;    // Data for IsItemDeactivated()/IsItemDeactivatedAfterEdit() function.
struct HvkGuiErrorRecoveryState;     // Storage of stack sizes for error handling and recovery
struct HvkGuiGroupData;              // Stacked storage data for BeginGroup()/EndGroup()
struct HvkGuiInputTextState;         // Internal state of the currently focused/edited text input box
struct HvkGuiInputTextDeactivateData;// Short term storage to backup text of a deactivating InputText() while another is stealing active id
struct HvkGuiLastItemData;           // Status storage for last submitted items
struct HvkGuiLocEntry;               // A localization entry.
struct HvkGuiMenuColumns;            // Simple column measurement, currently used for MenuItem() only
struct HvkGuiMultiSelectState;       // Multi-selection persistent state (for focused selection).
struct HvkGuiMultiSelectTempData;    // Multi-selection temporary state (while traversing).
struct HvkGuiNavItemData;            // Result of a keyboard/gamepad directional navigation move query result
struct HvkGuiMetricsConfig;          // Storage for ShowMetricsWindow() and DebugNodeXXX() functions
struct HvkGuiNextWindowData;         // Storage for SetNextWindow** functions
struct HvkGuiNextItemData;           // Storage for SetNextItem** functions
struct HvkGuiOldColumnData;          // Storage data for a single column for legacy Columns() api
struct HvkGuiOldColumns;             // Storage data for a columns set for legacy Columns() api
struct HvkGuiPopupData;              // Storage for current popup stack
struct HvkGuiSettingsHandler;        // Storage for one type registered in the .ini file
struct HvkGuiStyleMod;               // Stacked style modifier, backup of modified data so we can restore it
struct HvkGuiStyleVarInfo;           // Style variable information (e.g. to access style variables from an enum)
struct HvkGuiTabBar;                 // Storage for a tab bar
struct HvkGuiTabItem;                // Storage for a tab item (within a tab bar)
struct HvkGuiTable;                  // Storage for a table
struct HvkGuiTableHeaderData;        // Storage for TableAngledHeadersRow()
struct HvkGuiTableColumn;            // Storage for one column of a table
struct HvkGuiTableInstanceData;      // Storage for one instance of a same table
struct HvkGuiTableTempData;          // Temporary storage for one table (one per table in the stack), shared between tables.
struct HvkGuiTableSettings;          // Storage for a table .ini settings
struct HvkGuiTableColumnsSettings;   // Storage for a column .ini settings
struct HvkGuiTreeNodeStackData;      // Temporary storage for TreeNode().
struct HvkGuiTypingSelectState;      // Storage for GetTypingSelectRequest()
struct HvkGuiTypingSelectRequest;    // Storage for GetTypingSelectRequest() (aimed to be public)
struct HvkGuiWindow;                 // Storage for one window
struct HvkGuiWindowTempData;         // Temporary storage for one window (that's the data which in theory we could ditch at the end of the frame, in practice we currently keep it for each window)
struct HvkGuiWindowSettings;         // Storage for a window .ini settings (we keep one of those even if the actual window wasn't instanced during this session)

// Enumerations
// Use your programming IDE "Go to definition" facility on the names of the center columns to find the actual flags/enum lists.
enum HvkGuiLocKey : int;                 // -> enum HvkGuiLocKey              // Enum: a localization entry for translation.
typedef int HvkGuiLayoutType;            // -> enum HvkGuiLayoutType_         // Enum: Horizontal or vertical

// Flags
typedef int HvkDrawTextFlags;            // -> enum HvkDrawTextFlags_         // Flags: for HvkTextCalcWordWrapPositionEx()
typedef int HvkGuiActivateFlags;         // -> enum HvkGuiActivateFlags_      // Flags: for navigation/focus function (will be for ActivateItem() later)
typedef int HvkGuiDebugLogFlags;         // -> enum HvkGuiDebugLogFlags_      // Flags: for ShowDebugLogWindow(), g.DebugLogFlags
typedef int HvkGuiFocusRequestFlags;     // -> enum HvkGuiFocusRequestFlags_  // Flags: for FocusWindow()
typedef int HvkGuiItemStatusFlags;       // -> enum HvkGuiItemStatusFlags_    // Flags: for g.LastItemData.StatusFlags
typedef int HvkGuiOldColumnFlags;        // -> enum HvkGuiOldColumnFlags_     // Flags: for BeginColumns()
typedef int HvkGuiLogFlags;              // -> enum HvkGuiLogFlags_           // Flags: for LogBegin() text capturing function
typedef int HvkGuiNavRenderCursorFlags;  // -> enum HvkGuiNavRenderCursorFlags_//Flags: for RenderNavCursor()
typedef int HvkGuiNavMoveFlags;          // -> enum HvkGuiNavMoveFlags_       // Flags: for navigation requests
typedef int HvkGuiNextItemDataFlags;     // -> enum HvkGuiNextItemDataFlags_  // Flags: for SetNextItemXXX() functions
typedef int HvkGuiNextWindowDataFlags;   // -> enum HvkGuiNextWindowDataFlags_// Flags: for SetNextWindowXXX() functions
typedef int HvkGuiScrollFlags;           // -> enum HvkGuiScrollFlags_        // Flags: for ScrollToItem() and navigation requests
typedef int HvkGuiSeparatorFlags;        // -> enum HvkGuiSeparatorFlags_     // Flags: for SeparatorEx()
typedef int HvkGuiTextFlags;             // -> enum HvkGuiTextFlags_          // Flags: for TextEx()
typedef int HvkGuiTooltipFlags;          // -> enum HvkGuiTooltipFlags_       // Flags: for BeginTooltipEx()
typedef int HvkGuiTypingSelectFlags;     // -> enum HvkGuiTypingSelectFlags_  // Flags: for GetTypingSelectRequest()
typedef int HvkGuiWindowBgClickFlags;    // -> enum HvkGuiWindowBgClickFlags_ // Flags: for overriding behavior of clicking on window background/void.
typedef int HvkGuiWindowRefreshFlags;    // -> enum HvkGuiWindowRefreshFlags_ // Flags: for SetNextWindowRefreshPolicy()

// Table column indexing
typedef HvkS16 HvkGuiTableColumnIdx;
typedef HvkU16 HvkGuiTableDrawChannelIdx;

//-----------------------------------------------------------------------------
// [SECTION] Context pointer
// See Hvkplementation of this variable in HvkGui.cpp for comments and details.
//-----------------------------------------------------------------------------

#ifndef GHvkGui
extern HvkGui_API HvkGuiContext* GHvkGui;  // Current Hvkplicit context pointer
#endif

//-----------------------------------------------------------------------------
// [SECTION] Macros
//-----------------------------------------------------------------------------

// Debug Printing Into TTY
// (since HvkGui_VERSION_NUM >= 18729: HvkGui_DEBUG_LOG was reworked into HvkGui_DEBUG_PRINTF (and removed framecount from it). If you were using a #define HvkGui_DEBUG_LOG please rename)
#ifndef HvkGui_DEBUG_PRINTF
#ifndef HvkGui_DISABLE_DEFAULT_FORMAT_FUNCTIONS
#define HvkGui_DEBUG_PRINTF(_FMT,...)    printf(_FMT, __VA_ARGS__)
#else
#define HvkGui_DEBUG_PRINTF(_FMT,...)    ((void)0)
#endif
#endif

// Debug Logging for ShowDebugLogWindow(). This is designed for relatively rare events so please don't spam.
#define HvkGui_DEBUG_LOG_ERROR(...)      do { if (g.DebugLogFlags & HvkGuiDebugLogFlags_EventError)       HvkGui_DEBUG_LOG(__VA_ARGS__); else g.DebugLogSkippedErrors++; } while (0)
#define HvkGui_DEBUG_LOG_ACTIVEID(...)   do { if (g.DebugLogFlags & HvkGuiDebugLogFlags_EventActiveId)    HvkGui_DEBUG_LOG(__VA_ARGS__); } while (0)
#define HvkGui_DEBUG_LOG_FOCUS(...)      do { if (g.DebugLogFlags & HvkGuiDebugLogFlags_EventFocus)       HvkGui_DEBUG_LOG(__VA_ARGS__); } while (0)
#define HvkGui_DEBUG_LOG_POPUP(...)      do { if (g.DebugLogFlags & HvkGuiDebugLogFlags_EventPopup)       HvkGui_DEBUG_LOG(__VA_ARGS__); } while (0)
#define HvkGui_DEBUG_LOG_NAV(...)        do { if (g.DebugLogFlags & HvkGuiDebugLogFlags_EventNav)         HvkGui_DEBUG_LOG(__VA_ARGS__); } while (0)
#define HvkGui_DEBUG_LOG_SELECTION(...)  do { if (g.DebugLogFlags & HvkGuiDebugLogFlags_EventSelection)   HvkGui_DEBUG_LOG(__VA_ARGS__); } while (0)
#define HvkGui_DEBUG_LOG_CLIPPER(...)    do { if (g.DebugLogFlags & HvkGuiDebugLogFlags_EventClipper)     HvkGui_DEBUG_LOG(__VA_ARGS__); } while (0)
#define HvkGui_DEBUG_LOG_IO(...)         do { if (g.DebugLogFlags & HvkGuiDebugLogFlags_EventIO)          HvkGui_DEBUG_LOG(__VA_ARGS__); } while (0)
#define HvkGui_DEBUG_LOG_FONT(...)       do { HvkGuiContext* g2 = GHvkGui; if (g2 && g2->DebugLogFlags & HvkGuiDebugLogFlags_EventFont) HvkGui_DEBUG_LOG(__VA_ARGS__); } while (0) // Called from HvkFontAtlas function which may operate without a context.
#define HvkGui_DEBUG_LOG_INPUTROUTING(...) do{if (g.DebugLogFlags & HvkGuiDebugLogFlags_EventInputRouting)HvkGui_DEBUG_LOG(__VA_ARGS__); } while (0)

// Static Asserts
#define Hvk_STATIC_ASSERT(_COND)         static_assert(_COND, "")

// "Paranoid" Debug Asserts are meant to only be enabled during specific debugging/work, otherwise would slow down the code too much.
// We currently don't have many of those so the effect is currently negligible, but onward intent to add more aggressive ones in the code.
//#define HvkGui_DEBUG_PARANOID
#ifdef HvkGui_DEBUG_PARANOID
#define Hvk_ASSERT_PARANOID(_EXPR)       Hvk_ASSERT(_EXPR)
#else
#define Hvk_ASSERT_PARANOID(_EXPR)
#endif

// Misc Macros
#define Hvk_PI                           3.14159265358979323846f
#ifdef _WIN32
#define Hvk_NEWLINE                      "\r\n"   // Play it nice with Windows users (Update: since 2018-05, Notepad finally appears to support Unix-style carriage returns!)
#else
#define Hvk_NEWLINE                      "\n"
#endif
#ifndef Hvk_TABSIZE                      // Until we move this to runtime and/or add proper tab support, at least allow users to compile-time override
#define Hvk_TABSIZE                      (4)
#endif
#define Hvk_MEMALIGN(_OFF,_ALIGN)        (((_OFF) + ((_ALIGN) - 1)) & ~((_ALIGN) - 1))           // Memory align e.g. Hvk_ALIGN(0,4)=0, Hvk_ALIGN(1,4)=4, Hvk_ALIGN(4,4)=4, Hvk_ALIGN(5,4)=8
#define Hvk_F32_TO_INT8_UNBOUND(_VAL)    ((int)((_VAL) * 255.0f + ((_VAL)>=0 ? 0.5f : -0.5f)))   // Unsaturated, for display purpose
#define Hvk_F32_TO_INT8_SAT(_VAL)        ((int)(HvkSaturate(_VAL) * 255.0f + 0.5f))               // Saturated, always output 0..255
#define Hvk_TRUNC(_VAL)                  ((float)(int)(_VAL))                                    // HvkTrunc() is not inlined in MSVC debug builds
#define Hvk_ROUND(_VAL)                  ((float)(int)((_VAL) + 0.5f))                           //
#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
#define Hvk_FLOOR Hvk_TRUNC               // [OBSOLETE] Renamed in 1.90.0 (Sept 2023)
#endif

// Hint for branch prediction
#if (defined(__cplusplus) && (__cplusplus >= 202002L)) || (defined(_MSVC_LANG) && (_MSVC_LANG >= 202002L))
#define Hvk_LIKELY   [[likely]]
#define Hvk_UNLIKELY [[unlikely]]
#else
#define Hvk_LIKELY
#define Hvk_UNLIKELY
#endif

// Enforce cdecl calling convention for functions called by the standard library, in case compilation settings changed the default to e.g. __vectorcall
#ifdef _MSC_VER
#define HvkGui_CDECL __cdecl
#else
#define HvkGui_CDECL
#endif

// Warnings
#if defined(_MSC_VER) && !defined(__clang__)
#define Hvk_MSVC_WARNING_SUPPRESS(XXXX)  __pragma(warning(suppress: XXXX))
#else
#define Hvk_MSVC_WARNING_SUPPRESS(XXXX)
#endif

// Debug Tools
// Use 'Metrics/Debugger->Tools->Item Picker' to break into the call-stack of a specific item.
// This will call Hvk_DEBUG_BREAK() which you may redefine yourself. See https://github.com/scottt/debugbreak for more reference.
#ifndef Hvk_DEBUG_BREAK
#if defined (_MSC_VER)
#define Hvk_DEBUG_BREAK()    __debugbreak()
#elif defined(__clang__)
#define Hvk_DEBUG_BREAK()    __builtin_debugtrap()
#elif defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
#define Hvk_DEBUG_BREAK()    __asm__ volatile("int3;nop")
#elif defined(__GNUC__) && defined(__thumb__)
#define Hvk_DEBUG_BREAK()    __asm__ volatile(".inst 0xde01")
#elif defined(__GNUC__) && defined(__arm__) && !defined(__thumb__)
#define Hvk_DEBUG_BREAK()    __asm__ volatile(".inst 0xe7f001f0")
#else
#define Hvk_DEBUG_BREAK()    Hvk_ASSERT(0)    // It is expected that you define Hvk_DEBUG_BREAK() into something that will break nicely in a debugger!
#endif
#endif // #ifndef Hvk_DEBUG_BREAK

// Format specifiers, printing 64-bit hasn't been decently standardized...
// In a real application you should be using PRId64 and PRIu64 from <inttypes.h> (non-windows) and on Windows define them yourself.
#if defined(_MSC_VER) && !defined(__clang__)
#define Hvk_PRId64   "I64d"
#define Hvk_PRIu64   "I64u"
#define Hvk_PRIX64   "I64X"
#else
#define Hvk_PRId64   "lld"
#define Hvk_PRIu64   "llu"
#define Hvk_PRIX64   "llX"
#endif

//-----------------------------------------------------------------------------
// [SECTION] Generic helpers
// Note that the HvkXXX helpers functions are lower-level than HvkGui functions.
// HvkGui functions or the HvkGui context are never called/used from other HvkXXX functions.
//-----------------------------------------------------------------------------
// - Helpers: Hashing
// - Helpers: Sorting
// - Helpers: Bit manipulation
// - Helpers: String
// - Helpers: Formatting
// - Helpers: UTF-8 <> wchar conversions
// - Helpers: HvkVec2/HvkVec4 operators
// - Helpers: Maths
// - Helpers: Geometry
// - Helper: HvkVec1
// - Helper: HvkVec2ih
// - Helper: HvkRect
// - Helper: HvkBitArray
// - Helper: HvkBitVector
// - Helper: HvkSpan<>, HvkSpanAllocator<>
// - Helper: HvkStableVector<>
// - Helper: HvkPool<>
// - Helper: HvkChunkStream<>
// - Helper: HvkGuiTextIndex
// - Helper: HvkGuiStorage
//-----------------------------------------------------------------------------

// Helpers: Hashing
HvkGui_API HvkGuiID       HvkHashData(const void* data, size_t data_size, HvkGuiID seed = 0);
HvkGui_API HvkGuiID       HvkHashStr(const char* data, size_t data_size = 0, HvkGuiID seed = 0);
HvkGui_API const char*   HvkHashSkipUncontributingPrefix(const char* label);

// Helpers: Sorting
#ifndef HvkQsort
inline void             HvkQsort(void* base, size_t count, size_t size_of_element, int(HvkGui_CDECL *compare_func)(void const*, void const*)) { if (count > 1) qsort(base, count, size_of_element, compare_func); }
#endif

// Helpers: Color Blending
HvkGui_API HvkU32         HvkAlphaBlendColors(HvkU32 col_a, HvkU32 col_b);

// Helpers: Bit manipulation
inline bool             HvkIsPowerOfTwo(int v)               { return v != 0 && (v & (v - 1)) == 0; }
inline bool             HvkIsPowerOfTwo(HvkU64 v)             { return v != 0 && (v & (v - 1)) == 0; }
inline int              HvkUpperPowerOfTwo(int v)            { v--; v |= v >> 1; v |= v >> 2; v |= v >> 4; v |= v >> 8; v |= v >> 16; v++; return v; }
inline unsigned int     HvkCountSetBits(unsigned int v)      { unsigned int count = 0; while (v > 0) { v = v & (v - 1); count++; } return count; }

// Helpers: String
#define HvkStrlen strlen
#define HvkMemchr memchr
HvkGui_API int           HvkStricmp(const char* str1, const char* str2);                      // Case insensitive compare.
HvkGui_API int           HvkStrnicmp(const char* str1, const char* str2, size_t count);       // Case insensitive compare to a certain count.
HvkGui_API void          HvkStrncpy(char* dst, const char* src, size_t count);                // Copy to a certain count and always zero terminate (strncpy doesn't).
HvkGui_API char*         HvkStrdup(const char* str);                                          // Duplicate a string.
HvkGui_API void*         HvkMemdup(const void* src, size_t size);                             // Duplicate a chunk of memory.
HvkGui_API char*         HvkStrdupcpy(char* dst, size_t* p_dst_size, const char* str);        // Copy in provided buffer, recreate buffer if needed.
HvkGui_API const char*   HvkStrchrRange(const char* str_begin, const char* str_end, char c);  // Find first occurrence of 'c' in string range.
HvkGui_API const char*   HvkStreolRange(const char* str, const char* str_end);                // End end-of-line
HvkGui_API const char*   HvkStristr(const char* haystack, const char* haystack_end, const char* needle, const char* needle_end);  // Find a substring in a string range.
HvkGui_API void          HvkStrTrimBlanks(char* str);                                         // Remove leading and trailing blanks from a buffer.
HvkGui_API const char*   HvkStrSkipBlank(const char* str);                                    // Find first non-blank character.
HvkGui_API int           HvkStrlenW(const HvkWchar* str);                                      // Computer string length (HvkWchar string)
HvkGui_API const char*   HvkStrbol(const char* buf_mid_line, const char* buf_begin);          // Find beginning-of-line
Hvk_MSVC_RUNTIME_CHECKS_OFF
inline char             HvkToUpper(char c)               { return (c >= 'a' && c <= 'z') ? c &= ~32 : c; }
inline bool             HvkCharIsBlankA(char c)          { return c == ' ' || c == '\t'; }
inline bool             HvkCharIsBlankW(unsigned int c)  { return c == ' ' || c == '\t' || c == 0x3000; }
inline bool             HvkCharIsXdigitA(char c)         { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'); }
Hvk_MSVC_RUNTIME_CHECKS_RESTORE

// Helpers: Formatting
HvkGui_API int           HvkFormatString(char* buf, size_t buf_size, const char* fmt, ...) Hvk_FMTARGS(3);
HvkGui_API int           HvkFormatStringV(char* buf, size_t buf_size, const char* fmt, va_list args) Hvk_FMTLIST(3);
HvkGui_API void          HvkFormatStringToTempBuffer(const char** out_buf, const char** out_buf_end, const char* fmt, ...) Hvk_FMTARGS(3);
HvkGui_API void          HvkFormatStringToTempBufferV(const char** out_buf, const char** out_buf_end, const char* fmt, va_list args) Hvk_FMTLIST(3);
HvkGui_API const char*   HvkParseFormatFindStart(const char* format);
HvkGui_API const char*   HvkParseFormatFindEnd(const char* format);
HvkGui_API const char*   HvkParseFormatTrimDecorations(const char* format, char* buf, size_t buf_size);
HvkGui_API void          HvkParseFormatSanitizeForPrinting(const char* fmt_in, char* fmt_out, size_t fmt_out_size);
HvkGui_API const char*   HvkParseFormatSanitizeForScanning(const char* fmt_in, char* fmt_out, size_t fmt_out_size);
HvkGui_API int           HvkParseFormatPrecision(const char* format, int default_value);

// Helpers: UTF-8 <> wchar conversions
HvkGui_API int           HvkTextCharToUtf8(char out_buf[5], unsigned int c);                                                      // return output UTF-8 bytes count
HvkGui_API int           HvkTextStrToUtf8(char* out_buf, int out_buf_size, const HvkWchar* in_text, const HvkWchar* in_text_end);   // return output UTF-8 bytes count
HvkGui_API int           HvkTextCharFromUtf8(unsigned int* out_char, const char* in_text, const char* in_text_end);               // read one character. return input UTF-8 bytes count
HvkGui_API int           HvkTextStrFromUtf8(HvkWchar* out_buf, int out_buf_size, const char* in_text, const char* in_text_end, const char** in_remaining = NULL);   // return input UTF-8 bytes count
HvkGui_API int           HvkTextCountCharsFromUtf8(const char* in_text, const char* in_text_end);                                 // return number of UTF-8 code-points (NOT bytes count)
HvkGui_API int           HvkTextCountUtf8BytesFromChar(const char* in_text, const char* in_text_end);                             // return number of bytes to express one char in UTF-8
HvkGui_API int           HvkTextCountUtf8BytesFromStr(const HvkWchar* in_text, const HvkWchar* in_text_end);                        // return number of bytes to express string in UTF-8
HvkGui_API const char*   HvkTextFindPreviousUtf8Codepoint(const char* in_text_start, const char* in_p);                           // return previous UTF-8 code-point.
HvkGui_API const char*   HvkTextFindValidUtf8CodepointEnd(const char* in_text_start, const char* in_text_end, const char* in_p);  // return previous UTF-8 code-point if 'in_p' is not the end of a valid one.
HvkGui_API int           HvkTextCountLines(const char* in_text, const char* in_text_end);                                         // return number of lines taken by text. trailing carriage return doesn't count as an extra line.

// Helpers: High-level text functions (DO NOT USE!!! THIS IS A MINIMAL SUBSET OF LARGER UPCOMING CHANGES)
enum HvkDrawTextFlags_
{
    HvkDrawTextFlags_None                = 0,
    HvkDrawTextFlags_CpuFineClip         = 1 << 0,    // Must be == 1/true for legacy with 'bool cpu_fine_clip' arg to RenderText()
    HvkDrawTextFlags_WrapKeepBlanks      = 1 << 1,
    HvkDrawTextFlags_StopOnNewLine       = 1 << 2,
};
HvkGui_API HvkVec2        HvkFontCalcTextSizeEx(HvkFont* font, float size, float max_width, float wrap_width, const char* text_begin, const char* text_end_display, const char* text_end, const char** out_remaining, HvkVec2* out_offset, HvkDrawTextFlags flags);
HvkGui_API const char*   HvkFontCalcWordWrapPositionEx(HvkFont* font, float size, const char* text, const char* text_end, float wrap_width, HvkDrawTextFlags flags = 0);
HvkGui_API const char*   HvkTextCalcWordWrapNextLineStart(const char* text, const char* text_end, HvkDrawTextFlags flags = 0); // trim trailing space and find beginning of next line

// Helpers: File System
#ifdef HvkGui_DISABLE_FILE_FUNCTIONS
#define HvkGui_DISABLE_DEFAULT_FILE_FUNCTIONS
typedef void* HvkFileHandle;
inline HvkFileHandle         HvkFileOpen(const char*, const char*)                    { return NULL; }
inline bool                 HvkFileClose(HvkFileHandle)                               { return false; }
inline HvkU64                HvkFileGetSize(HvkFileHandle)                             { return (HvkU64)-1; }
inline HvkU64                HvkFileRead(void*, HvkU64, HvkU64, HvkFileHandle)           { return 0; }
inline HvkU64                HvkFileWrite(const void*, HvkU64, HvkU64, HvkFileHandle)    { return 0; }
#endif
#ifndef HvkGui_DISABLE_DEFAULT_FILE_FUNCTIONS
typedef FILE* HvkFileHandle;
HvkGui_API HvkFileHandle      HvkFileOpen(const char* filename, const char* mode);
HvkGui_API bool              HvkFileClose(HvkFileHandle file);
HvkGui_API HvkU64             HvkFileGetSize(HvkFileHandle file);
HvkGui_API HvkU64             HvkFileRead(void* data, HvkU64 size, HvkU64 count, HvkFileHandle file);
HvkGui_API HvkU64             HvkFileWrite(const void* data, HvkU64 size, HvkU64 count, HvkFileHandle file);
#else
#define HvkGui_DISABLE_TTY_FUNCTIONS // Can't use stdout, fflush if we are not using default file functions
#endif
HvkGui_API void*             HvkFileLoadToMemory(const char* filename, const char* mode, size_t* out_file_size = NULL, int padding_bytes = 0);

// Helpers: Maths
Hvk_MSVC_RUNTIME_CHECKS_OFF
// - Wrapper for standard libs functions. (Note that HvkGui_demo.cpp does _not_ use them to keep the code easy to copy)
#ifndef HvkGui_DISABLE_DEFAULT_MATH_FUNCTIONS
#define HvkFabs(X)           fabsf(X)
#define HvkSqrt(X)           sqrtf(X)
#define HvkFmod(X, Y)        fmodf((X), (Y))
#define HvkCos(X)            cosf(X)
#define HvkSin(X)            sinf(X)
#define HvkAcos(X)           acosf(X)
#define HvkAtan2(Y, X)       atan2f((Y), (X))
#define HvkAtof(STR)         atof(STR)
#define HvkCeil(X)           ceilf(X)
inline float  HvkPow(float x, float y)    { return powf(x, y); }          // DragBehaviorT/SliderBehaviorT uses HvkPow with either float/double and need the precision
inline double HvkPow(double x, double y)  { return pow(x, y); }
inline float  HvkLog(float x)             { return logf(x); }             // DragBehaviorT/SliderBehaviorT uses HvkLog with either float/double and need the precision
inline double HvkLog(double x)            { return log(x); }
inline int    HvkAbs(int x)               { return x < 0 ? -x : x; }
inline float  HvkAbs(float x)             { return fabsf(x); }
inline double HvkAbs(double x)            { return fabs(x); }
inline float  HvkSign(float x)            { return (x < 0.0f) ? -1.0f : (x > 0.0f) ? 1.0f : 0.0f; } // Sign operator - returns -1, 0 or 1 based on sign of argument
inline double HvkSign(double x)           { return (x < 0.0) ? -1.0 : (x > 0.0) ? 1.0 : 0.0; }
#ifdef HvkGui_ENABLE_SSE
inline float  HvkRsqrt(float x)           { return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(x))); }
#else
inline float  HvkRsqrt(float x)           { return 1.0f / sqrtf(x); }
#endif
inline double HvkRsqrt(double x)          { return 1.0 / sqrt(x); }
#endif
// - HvkMin/HvkMax/HvkClamp/HvkLerp/HvkSwap are used by widgets which support variety of types: signed/unsigned int/long long float/double
// (Exceptionally using templates here but we could also redefine them for those types)
template<typename T> T HvkMin(T lhs, T rhs)                              { return lhs < rhs ? lhs : rhs; }
template<typename T> T HvkMax(T lhs, T rhs)                              { return lhs >= rhs ? lhs : rhs; }
template<typename T> T HvkClamp(T v, T mn, T mx)                         { return (v < mn) ? mn : (v > mx) ? mx : v; }
template<typename T> T HvkLerp(T a, T b, float t)                        { return (T)(a + (b - a) * t); }
template<typename T> void HvkSwap(T& a, T& b)                            { T tmp = a; a = b; b = tmp; }
template<typename T> T HvkAddClampOverflow(T a, T b, T mn, T mx)         { if (b < 0 && (a < mn - b)) return mn; if (b > 0 && (a > mx - b)) return mx; return a + b; }
template<typename T> T HvkSubClampOverflow(T a, T b, T mn, T mx)         { if (b > 0 && (a < mn + b)) return mn; if (b < 0 && (a > mx + b)) return mx; return a - b; }
// - Misc maths helpers
inline HvkVec2 HvkMin(const HvkVec2& lhs, const HvkVec2& rhs)               { return HvkVec2(lhs.x < rhs.x ? lhs.x : rhs.x, lhs.y < rhs.y ? lhs.y : rhs.y); }
inline HvkVec2 HvkMax(const HvkVec2& lhs, const HvkVec2& rhs)               { return HvkVec2(lhs.x >= rhs.x ? lhs.x : rhs.x, lhs.y >= rhs.y ? lhs.y : rhs.y); }
inline HvkVec2 HvkClamp(const HvkVec2& v, const HvkVec2&mn, const HvkVec2&mx){ return HvkVec2((v.x < mn.x) ? mn.x : (v.x > mx.x) ? mx.x : v.x, (v.y < mn.y) ? mn.y : (v.y > mx.y) ? mx.y : v.y); }
inline HvkVec2 HvkLerp(const HvkVec2& a, const HvkVec2& b, float t)         { return HvkVec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t); }
inline HvkVec2 HvkLerp(const HvkVec2& a, const HvkVec2& b, const HvkVec2& t) { return HvkVec2(a.x + (b.x - a.x) * t.x, a.y + (b.y - a.y) * t.y); }
inline HvkVec4 HvkLerp(const HvkVec4& a, const HvkVec4& b, float t)         { return HvkVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t); }
inline float  HvkSaturate(float f)                                       { return (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f; }
inline float  HvkLengthSqr(const HvkVec2& lhs)                            { return (lhs.x * lhs.x) + (lhs.y * lhs.y); }
inline float  HvkLengthSqr(const HvkVec4& lhs)                            { return (lhs.x * lhs.x) + (lhs.y * lhs.y) + (lhs.z * lhs.z) + (lhs.w * lhs.w); }
inline float  HvkInvLength(const HvkVec2& lhs, float fail_value)          { float d = (lhs.x * lhs.x) + (lhs.y * lhs.y); if (d > 0.0f) return HvkRsqrt(d); return fail_value; }
inline float  HvkTrunc(float f)                                          { return (float)(int)(f); }
inline HvkVec2 HvkTrunc(const HvkVec2& v)                                  { return HvkVec2((float)(int)(v.x), (float)(int)(v.y)); }
inline float  HvkFloor(float f)                                          { return (float)((f >= 0 || (float)(int)f == f) ? (int)f : (int)f - 1); } // Decent replacement for floorf()
inline HvkVec2 HvkFloor(const HvkVec2& v)                                  { return HvkVec2(HvkFloor(v.x), HvkFloor(v.y)); }
inline float  HvkTrunc64(float f)                                        { return (float)(HvkS64)(f); }
inline float  HvkRound64(float f)                                        { return (float)(HvkS64)(f + 0.5f); }
inline int    HvkModPositive(int a, int b)                               { return (a + b) % b; }
inline float  HvkDot(const HvkVec2& a, const HvkVec2& b)                   { return a.x * b.x + a.y * b.y; }
inline HvkVec2 HvkRotate(const HvkVec2& v, float cos_a, float sin_a)       { return HvkVec2(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a); }
inline float  HvkLinearSweep(float current, float target, float speed)   { if (current < target) return HvkMin(current + speed, target); if (current > target) return HvkMax(current - speed, target); return current; }
inline float  HvkLinearRemapClamp(float s0, float s1, float d0, float d1, float x) { return HvkSaturate((x - s0) / (s1 - s0)) * (d1 - d0) + d0; }
inline HvkVec2 HvkMul(const HvkVec2& lhs, const HvkVec2& rhs)               { return HvkVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
inline bool   HvkIsFloatAboveGuaranteedIntegerPrecision(float f)         { return f <= -16777216 || f >= 16777216; }
inline float  HvkExponentialMovingAverage(float avg, float sample, int n){ avg -= avg / n; avg += sample / n; return avg; }
Hvk_MSVC_RUNTIME_CHECKS_RESTORE

// Helpers: Geometry
HvkGui_API HvkVec2     HvkBezierCubicCalc(const HvkVec2& p1, const HvkVec2& p2, const HvkVec2& p3, const HvkVec2& p4, float t);
HvkGui_API HvkVec2     HvkBezierCubicClosestPoint(const HvkVec2& p1, const HvkVec2& p2, const HvkVec2& p3, const HvkVec2& p4, const HvkVec2& p, int num_segments);       // For curves with explicit number of segments
HvkGui_API HvkVec2     HvkBezierCubicClosestPointCasteljau(const HvkVec2& p1, const HvkVec2& p2, const HvkVec2& p3, const HvkVec2& p4, const HvkVec2& p, float tess_tol);// For auto-tessellated curves you can use tess_tol = style.CurveTessellationTol
HvkGui_API HvkVec2     HvkBezierQuadraticCalc(const HvkVec2& p1, const HvkVec2& p2, const HvkVec2& p3, float t);
HvkGui_API HvkVec2     HvkLineClosestPoint(const HvkVec2& a, const HvkVec2& b, const HvkVec2& p);
HvkGui_API bool       HvkTriangleContainsPoint(const HvkVec2& a, const HvkVec2& b, const HvkVec2& c, const HvkVec2& p);
HvkGui_API HvkVec2     HvkTriangleClosestPoint(const HvkVec2& a, const HvkVec2& b, const HvkVec2& c, const HvkVec2& p);
HvkGui_API void       HvkTriangleBarycentricCoords(const HvkVec2& a, const HvkVec2& b, const HvkVec2& c, const HvkVec2& p, float& out_u, float& out_v, float& out_w);
inline float         HvkTriangleArea(const HvkVec2& a, const HvkVec2& b, const HvkVec2& c)          { return HvkFabs((a.x * (b.y - c.y)) + (b.x * (c.y - a.y)) + (c.x * (a.y - b.y))) * 0.5f; }
inline bool          HvkTriangleIsClockwise(const HvkVec2& a, const HvkVec2& b, const HvkVec2& c)   { return ((b.x - a.x) * (c.y - b.y)) - ((c.x - b.x) * (b.y - a.y)) > 0.0f; }

// Helper: HvkVec1 (1D vector)
// (this odd construct is used to facilitate the transition between 1D and 2D, and the maintenance of some branches/patches)
Hvk_MSVC_RUNTIME_CHECKS_OFF
struct HvkVec1
{
    float   x;
    constexpr HvkVec1()         : x(0.0f) { }
    constexpr HvkVec1(float _x) : x(_x) { }
};

// Helper: HvkVec2i (2D vector, integer)
struct HvkVec2i
{
    int         x, y;
    constexpr HvkVec2i()                             : x(0), y(0) {}
    constexpr HvkVec2i(int _x, int _y)               : x(_x), y(_y) {}
};

// Helper: HvkVec2ih (2D vector, half-size integer, for long-term packed storage)
struct HvkVec2ih
{
    short   x, y;
    constexpr HvkVec2ih()                           : x(0), y(0) {}
    constexpr HvkVec2ih(short _x, short _y)         : x(_x), y(_y) {}
    constexpr explicit HvkVec2ih(const HvkVec2& rhs) : x((short)rhs.x), y((short)rhs.y) {}
};

// Helper: HvkRect (2D axis aligned bounding-box)
// NB: we can't rely on HvkVec2 math operators being available here!
struct HvkGui_API HvkRect
{
    HvkVec2      Min;    // Upper-left
    HvkVec2      Max;    // Lower-right

    constexpr HvkRect()                                        : Min(0.0f, 0.0f), Max(0.0f, 0.0f)  {}
    constexpr HvkRect(const HvkVec2& min, const HvkVec2& max)    : Min(min), Max(max)                {}
    constexpr HvkRect(const HvkVec4& v)                         : Min(v.x, v.y), Max(v.z, v.w)      {}
    constexpr HvkRect(float x1, float y1, float x2, float y2)  : Min(x1, y1), Max(x2, y2)          {}

    HvkVec2      GetCenter() const                   { return HvkVec2((Min.x + Max.x) * 0.5f, (Min.y + Max.y) * 0.5f); }
    HvkVec2      GetSize() const                     { return HvkVec2(Max.x - Min.x, Max.y - Min.y); }
    float       GetWidth() const                    { return Max.x - Min.x; }
    float       GetHeight() const                   { return Max.y - Min.y; }
    float       GetArea() const                     { return (Max.x - Min.x) * (Max.y - Min.y); }
    HvkVec2      GetTL() const                       { return Min; }                   // Top-left
    HvkVec2      GetTR() const                       { return HvkVec2(Max.x, Min.y); }  // Top-right
    HvkVec2      GetBL() const                       { return HvkVec2(Min.x, Max.y); }  // Bottom-left
    HvkVec2      GetBR() const                       { return Max; }                   // Bottom-right
    bool        Contains(const HvkVec2& p) const     { return p.x     >= Min.x && p.y     >= Min.y && p.x     <  Max.x && p.y     <  Max.y; }
    bool        Contains(const HvkRect& r) const     { return r.Min.x >= Min.x && r.Min.y >= Min.y && r.Max.x <= Max.x && r.Max.y <= Max.y; }
    bool        ContainsWithPad(const HvkVec2& p, const HvkVec2& pad) const { return p.x >= Min.x - pad.x && p.y >= Min.y - pad.y && p.x < Max.x + pad.x && p.y < Max.y + pad.y; }
    bool        Overlaps(const HvkRect& r) const     { return r.Min.y <  Max.y && r.Max.y >  Min.y && r.Min.x <  Max.x && r.Max.x >  Min.x; }
    void        Add(const HvkVec2& p)                { if (Min.x > p.x)     Min.x = p.x;     if (Min.y > p.y)     Min.y = p.y;     if (Max.x < p.x)     Max.x = p.x;     if (Max.y < p.y)     Max.y = p.y; }
    void        Add(const HvkRect& r)                { if (Min.x > r.Min.x) Min.x = r.Min.x; if (Min.y > r.Min.y) Min.y = r.Min.y; if (Max.x < r.Max.x) Max.x = r.Max.x; if (Max.y < r.Max.y) Max.y = r.Max.y; }
    void        Expand(const float amount)          { Min.x -= amount;   Min.y -= amount;   Max.x += amount;   Max.y += amount; }
    void        Expand(const HvkVec2& amount)        { Min.x -= amount.x; Min.y -= amount.y; Max.x += amount.x; Max.y += amount.y; }
    void        Translate(const HvkVec2& d)          { Min.x += d.x; Min.y += d.y; Max.x += d.x; Max.y += d.y; }
    void        TranslateX(float dx)                { Min.x += dx; Max.x += dx; }
    void        TranslateY(float dy)                { Min.y += dy; Max.y += dy; }
    void        ClipWith(const HvkRect& r)           { Min = HvkMax(Min, r.Min); Max = HvkMin(Max, r.Max); }                   // Simple version, may lead to an inverted rectangle, which is fine for Contains/Overlaps test but not for display.
    void        ClipWithFull(const HvkRect& r)       { Min = HvkClamp(Min, r.Min, r.Max); Max = HvkClamp(Max, r.Min, r.Max); } // Full version, ensure both points are fully clipped.
    void        Floor()                             { Min.x = Hvk_TRUNC(Min.x); Min.y = Hvk_TRUNC(Min.y); Max.x = Hvk_TRUNC(Max.x); Max.y = Hvk_TRUNC(Max.y); }
    bool        IsInverted() const                  { return Min.x > Max.x || Min.y > Max.y; }
    HvkVec4      ToVec4() const                      { return HvkVec4(Min.x, Min.y, Max.x, Max.y); }
    const HvkVec4& AsVec4() const                    { return *(const HvkVec4*)&Min.x; }
};

// Helper: HvkBitArray
#define         Hvk_BITARRAY_TESTBIT(_ARRAY, _N)                 ((_ARRAY[(_N) >> 5] & ((HvkU32)1 << ((_N) & 31))) != 0) // Macro version of HvkBitArrayTestBit(): ensure args have side-effect or are costly!
#define         Hvk_BITARRAY_CLEARBIT(_ARRAY, _N)                ((_ARRAY[(_N) >> 5] &= ~((HvkU32)1 << ((_N) & 31))))    // Macro version of HvkBitArrayClearBit(): ensure args have side-effect or are costly!
inline size_t   HvkBitArrayGetStorageSizeInBytes(int bitcount)   { return (size_t)((bitcount + 31) >> 5) << 2; }
inline void     HvkBitArrayClearAllBits(HvkU32* arr, int bitcount){ memset(arr, 0, HvkBitArrayGetStorageSizeInBytes(bitcount)); }
inline bool     HvkBitArrayTestBit(const HvkU32* arr, int n)      { HvkU32 mask = (HvkU32)1 << (n & 31); return (arr[n >> 5] & mask) != 0; }
inline void     HvkBitArrayClearBit(HvkU32* arr, int n)           { HvkU32 mask = (HvkU32)1 << (n & 31); arr[n >> 5] &= ~mask; }
inline void     HvkBitArraySetBit(HvkU32* arr, int n)             { HvkU32 mask = (HvkU32)1 << (n & 31); arr[n >> 5] |= mask; }
inline void     HvkBitArraySetBitRange(HvkU32* arr, int n, int n2) // Works on range [n..n2)
{
    n2--;
    while (n <= n2)
    {
        int a_mod = (n & 31);
        int b_mod = (n2 > (n | 31) ? 31 : (n2 & 31)) + 1;
        HvkU32 mask = (HvkU32)(((HvkU64)1 << b_mod) - 1) & ~(HvkU32)(((HvkU64)1 << a_mod) - 1);
        arr[n >> 5] |= mask;
        n = (n + 32) & ~31;
    }
}

typedef HvkU32* HvkBitArrayPtr; // Name for use in structs

// Helper: HvkBitArray class (wrapper over HvkBitArray functions)
// Store 1-bit per value.
template<int BITCOUNT, int OFFSET = 0>
struct HvkBitArray
{
    HvkU32           Storage[(BITCOUNT + 31) >> 5];
    HvkBitArray()                                { ClearAllBits(); }
    void            ClearAllBits()              { memset(Storage, 0, sizeof(Storage)); }
    void            SetAllBits()                { memset(Storage, 255, sizeof(Storage)); }
    bool            TestBit(int n) const        { n += OFFSET; Hvk_ASSERT(n >= 0 && n < BITCOUNT); return Hvk_BITARRAY_TESTBIT(Storage, n); }
    void            SetBit(int n)               { n += OFFSET; Hvk_ASSERT(n >= 0 && n < BITCOUNT); HvkBitArraySetBit(Storage, n); }
    void            ClearBit(int n)             { n += OFFSET; Hvk_ASSERT(n >= 0 && n < BITCOUNT); HvkBitArrayClearBit(Storage, n); }
    void            SetBitRange(int n, int n2)  { n += OFFSET; n2 += OFFSET; Hvk_ASSERT(n >= 0 && n < BITCOUNT && n2 > n && n2 <= BITCOUNT); HvkBitArraySetBitRange(Storage, n, n2); } // Works on range [n..n2)
    bool            operator[](int n) const     { n += OFFSET; Hvk_ASSERT(n >= 0 && n < BITCOUNT); return Hvk_BITARRAY_TESTBIT(Storage, n); }
};

// Helper: HvkBitVector
// Store 1-bit per value.
struct HvkGui_API HvkBitVector
{
    HvkVector<HvkU32> Storage;
    void            Create(int sz)              { Storage.resize((sz + 31) >> 5); memset(Storage.Data, 0, (size_t)Storage.Size * sizeof(Storage.Data[0])); }
    void            Clear()                     { Storage.clear(); }
    bool            TestBit(int n) const        { Hvk_ASSERT(n < (Storage.Size << 5)); return Hvk_BITARRAY_TESTBIT(Storage.Data, n); }
    void            SetBit(int n)               { Hvk_ASSERT(n < (Storage.Size << 5)); HvkBitArraySetBit(Storage.Data, n); }
    void            ClearBit(int n)             { Hvk_ASSERT(n < (Storage.Size << 5)); HvkBitArrayClearBit(Storage.Data, n); }
};
Hvk_MSVC_RUNTIME_CHECKS_RESTORE

// Helper: HvkSpan<>
// Pointing to a span of data we don't own.
template<typename T>
struct HvkSpan
{
    T*                  Data;
    T*                  DataEnd;

    // Constructors, destructor
    inline HvkSpan()                                 { Data = DataEnd = NULL; }
    inline HvkSpan(T* data, int size)                { Data = data; DataEnd = data + size; }
    inline HvkSpan(T* data, T* data_end)             { Data = data; DataEnd = data_end; }

    inline void         set(T* data, int size)      { Data = data; DataEnd = data + size; }
    inline void         set(T* data, T* data_end)   { Data = data; DataEnd = data_end; }
    inline int          size() const                { return (int)(ptrdiff_t)(DataEnd - Data); }
    inline int          size_in_bytes() const       { return (int)(ptrdiff_t)(DataEnd - Data) * (int)sizeof(T); }
    inline T&           operator[](int i)           { T* p = Data + i; Hvk_ASSERT(p >= Data && p < DataEnd); return *p; }
    inline const T&     operator[](int i) const     { const T* p = Data + i; Hvk_ASSERT(p >= Data && p < DataEnd); return *p; }

    inline T*           begin()                     { return Data; }
    inline const T*     begin() const               { return Data; }
    inline T*           end()                       { return DataEnd; }
    inline const T*     end() const                 { return DataEnd; }

    // Utilities
    inline int  index_from_ptr(const T* it) const   { Hvk_ASSERT(it >= Data && it < DataEnd); const ptrdiff_t off = it - Data; return (int)off; }
};

// Helper: HvkSpanAllocator<>
// Facilitate storing multiple chunks into a single large block (the "arena")
// - Usage: call Reserve() N times, allocate GetArenaSizeInBytes() worth, pass it to SetArenaBasePtr(), call GetSpan() N times to retrieve the aligned ranges.
template<int CHUNKS>
struct HvkSpanAllocator
{
    char*   BasePtr;
    int     CurrOff;
    int     CurrIdx;
    int     Offsets[CHUNKS];
    int     Sizes[CHUNKS];

    HvkSpanAllocator()                               { memset(this, 0, sizeof(*this)); }
    inline void  Reserve(int n, size_t sz, int a=4) { Hvk_ASSERT(n == CurrIdx && n < CHUNKS); CurrOff = Hvk_MEMALIGN(CurrOff, a); Offsets[n] = CurrOff; Sizes[n] = (int)sz; CurrIdx++; CurrOff += (int)sz; }
    inline int   GetArenaSizeInBytes()              { return CurrOff; }
    inline void  SetArenaBasePtr(void* base_ptr)    { BasePtr = (char*)base_ptr; }
    inline void* GetSpanPtrBegin(int n)             { Hvk_ASSERT(n >= 0 && n < CHUNKS && CurrIdx == CHUNKS); return (void*)(BasePtr + Offsets[n]); }
    inline void* GetSpanPtrEnd(int n)               { Hvk_ASSERT(n >= 0 && n < CHUNKS && CurrIdx == CHUNKS); return (void*)(BasePtr + Offsets[n] + Sizes[n]); }
    template<typename T>
    inline void  GetSpan(int n, HvkSpan<T>* span)    { span->set((T*)GetSpanPtrBegin(n), (T*)GetSpanPtrEnd(n)); }
};

// Helper: HvkStableVector<>
// Allocating chunks of BLOCKSIZE items. Objects pointers are never invalidated when growing, only by clear().
// Hvkportant: does not destruct anything!
// Hvkplemented only the minimum set of functions we need for it.
template<typename T, int BLOCKSIZE>
struct HvkStableVector
{
    int                 Size = 0;
    int                 Capacity = 0;
    HvkVector<T*>        Blocks;

    // Functions
    inline ~HvkStableVector()                        { for (T* block : Blocks) Hvk_FREE(block); }

    inline void         clear()                     { Size = Capacity = 0; Blocks.clear_delete(); }
    inline void         resize(int new_size)        { if (new_size > Capacity) reserve(new_size); Size = new_size; }
    inline void         reserve(int new_cap)
    {
        new_cap = Hvk_MEMALIGN(new_cap, BLOCKSIZE);
        int old_count = Capacity / BLOCKSIZE;
        int new_count = new_cap / BLOCKSIZE;
        if (new_count <= old_count)
            return;
        Blocks.resize(new_count);
        for (int n = old_count; n < new_count; n++)
            Blocks[n] = (T*)Hvk_ALLOC(sizeof(T) * BLOCKSIZE);
        Capacity = new_cap;
    }
    inline T&           operator[](int i)           { Hvk_ASSERT(i >= 0 && i < Size); return Blocks[i / BLOCKSIZE][i % BLOCKSIZE]; }
    inline const T&     operator[](int i) const     { Hvk_ASSERT(i >= 0 && i < Size); return Blocks[i / BLOCKSIZE][i % BLOCKSIZE]; }
    inline T*           push_back(const T& v)       { int i = Size; Hvk_ASSERT(i >= 0); if (Size == Capacity) reserve(Capacity + BLOCKSIZE); void* ptr = &Blocks[i / BLOCKSIZE][i % BLOCKSIZE]; memcpy(ptr, &v, sizeof(v)); Size++; return (T*)ptr; }
};

// Helper: HvkPool<>
// Basic keyed storage for contiguous instances, slow/amortized insertion, O(1) indexable, O(Log N) queries by ID over a dense/hot buffer,
// Honor constructor/destructor. Add/remove invalidate all pointers. Indexes have the same lifetime as the associated object.
typedef int HvkPoolIdx;
template<typename T>
struct HvkPool
{
    HvkVector<T>     Buf;        // Contiguous data
    HvkGuiStorage    Map;        // ID->Index
    HvkPoolIdx       FreeIdx;    // Next free idx to use
    HvkPoolIdx       AliveCount; // Number of active/alive items (for display purpose)

    HvkPool()    { FreeIdx = AliveCount = 0; }
    ~HvkPool()   { Clear(); }
    T*          GetByKey(HvkGuiID key)               { int idx = Map.GetInt(key, -1); return (idx != -1) ? &Buf[idx] : NULL; }
    T*          GetByIndex(HvkPoolIdx n)             { return &Buf[n]; }
    HvkPoolIdx   GetIndex(const T* p) const          { Hvk_ASSERT(p >= Buf.Data && p < Buf.Data + Buf.Size); return (HvkPoolIdx)(p - Buf.Data); }
    T*          GetOrAddByKey(HvkGuiID key)          { int* p_idx = Map.GetIntRef(key, -1); if (*p_idx != -1) return &Buf[*p_idx]; *p_idx = FreeIdx; return Add(); }
    bool        Contains(const T* p) const          { return (p >= Buf.Data && p < Buf.Data + Buf.Size); }
    void        Clear()                             { for (int n = 0; n < Map.Data.Size; n++) { int idx = Map.Data[n].val_i; if (idx != -1) Buf[idx].~T(); } Map.Clear(); Buf.clear(); FreeIdx = AliveCount = 0; }
    T*          Add()                               { int idx = FreeIdx; if (idx == Buf.Size) { Buf.resize(Buf.Size + 1); FreeIdx++; } else { FreeIdx = *(int*)&Buf[idx]; } Hvk_PLACEMENT_NEW(&Buf[idx]) T(); AliveCount++; return &Buf[idx]; }
    void        Remove(HvkGuiID key, const T* p)     { Remove(key, GetIndex(p)); }
    void        Remove(HvkGuiID key, HvkPoolIdx idx)  { Buf[idx].~T(); *(int*)&Buf[idx] = FreeIdx; FreeIdx = idx; Map.SetInt(key, -1); AliveCount--; }
    void        Reserve(int capacity)               { Buf.reserve(capacity); Map.Data.reserve(capacity); }

    // To iterate a HvkPool: for (int n = 0; n < pool.GetMapSize(); n++) if (T* t = pool.TryGetMapData(n)) { ... }
    // Can be avoided if you know .Remove() has never been called on the pool, or AliveCount == GetMapSize()
    int         GetAliveCount() const               { return AliveCount; }      // Number of active/alive items in the pool (for display purpose)
    int         GetBufSize() const                  { return Buf.Size; }
    int         GetMapSize() const                  { return Map.Data.Size; }   // It is the map we need iterate to find valid items, since we don't have "alive" storage anywhere
    T*          TryGetMapData(HvkPoolIdx n)          { int idx = Map.Data[n].val_i; if (idx == -1) return NULL; return GetByIndex(idx); }
};

// Helper: HvkChunkStream<>
// Build and iterate a contiguous stream of variable-sized structures.
// This is used by Settings to store persistent data while reducing allocation count.
// We store the chunk size first, and align the final size on 4 bytes boundaries.
// The tedious/zealous amount of casting is to avoid -Wcast-align warnings.
template<typename T>
struct HvkChunkStream
{
    HvkVector<char>  Buf;

    void    clear()                     { Buf.clear(); }
    bool    empty() const               { return Buf.Size == 0; }
    int     size() const                { return Buf.Size; }
    T*      alloc_chunk(size_t sz)      { size_t HDR_SZ = 4; sz = Hvk_MEMALIGN(HDR_SZ + sz, 4u); int off = Buf.Size; Buf.resize(off + (int)sz); ((int*)(void*)(Buf.Data + off))[0] = (int)sz; return (T*)(void*)(Buf.Data + off + (int)HDR_SZ); }
    T*      begin()                     { size_t HDR_SZ = 4; if (!Buf.Data) return NULL; return (T*)(void*)(Buf.Data + HDR_SZ); }
    T*      next_chunk(T* p)            { size_t HDR_SZ = 4; Hvk_ASSERT(p >= begin() && p < end()); p = (T*)(void*)((char*)(void*)p + chunk_size(p)); if (p == (T*)(void*)((char*)end() + HDR_SZ)) return (T*)0; Hvk_ASSERT(p < end()); return p; }
    int     chunk_size(const T* p)      { return ((const int*)p)[-1]; }
    T*      end()                       { return (T*)(void*)(Buf.Data + Buf.Size); }
    int     offset_from_ptr(const T* p) { Hvk_ASSERT(p >= begin() && p < end()); const ptrdiff_t off = (const char*)p - Buf.Data; return (int)off; }
    T*      ptr_from_offset(int off)    { Hvk_ASSERT(off >= 4 && off < Buf.Size); return (T*)(void*)(Buf.Data + off); }
    void    swap(HvkChunkStream<T>& rhs) { rhs.Buf.swap(Buf); }
};

// Helper: HvkGuiTextIndex
// Maintain a line index for a text buffer. This is a strong candidate to be moved into the public API.
struct HvkGuiTextIndex
{
    HvkVector<int>   Offsets;
    int             EndOffset = 0;                          // Because we don't own text buffer we need to maintain EndOffset (may bake in LineOffsets?)

    void            clear()                                 { Offsets.clear(); EndOffset = 0; }
    int             size()                                  { return Offsets.Size; }
    const char*     get_line_begin(const char* base, int n) { return base + (Offsets.Size != 0 ? Offsets[n] : 0); }
    const char*     get_line_end(const char* base, int n)   { return base + (n + 1 < Offsets.Size ? (Offsets[n + 1] - 1) : EndOffset); }
    void            append(const char* base, int old_size, int new_size);
};

// Helper: HvkGuiStorage
HvkGui_API HvkGuiStoragePair* HvkLowerBound(HvkGuiStoragePair* in_begin, HvkGuiStoragePair* in_end, HvkGuiID key);

//-----------------------------------------------------------------------------
// [SECTION] HvkDrawList support
//-----------------------------------------------------------------------------

// HvkDrawList: Helper function to calculate a circle's segment count given its radius and a "maximum error" value.
// Estimation of number of circle segment based on error is derived using method described in https://stackoverflow.com/a/2244088/15194693
// Number of segments (N) is calculated using equation:
//   N = ceil ( pi / acos(1 - error / r) )     where r > 0, error <= r
// Our equation is significantly simpler that one in the post thanks for choosing segment that is
// perpendicular to X axis. Follow steps in the article from this starting condition and you will
// will get this result.
//
// Rendering circles with an odd number of segments, while mathematically correct will produce
// asymmetrical results on the raster grid. Therefore we're rounding N to next even number (7->8, 8->8, 9->10 etc.)
#define Hvk_ROUNDUP_TO_EVEN(_V)                                  ((((_V) + 1) / 2) * 2)
#define Hvk_DRAWLIST_CIRCLE_AUTO_SEGMENT_MIN                     4
#define Hvk_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX                     512
#define Hvk_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC(_RAD,_MAXERROR)    HvkClamp(Hvk_ROUNDUP_TO_EVEN((int)HvkCeil(Hvk_PI / HvkAcos(1 - HvkMin((_MAXERROR), (_RAD)) / (_RAD)))), Hvk_DRAWLIST_CIRCLE_AUTO_SEGMENT_MIN, Hvk_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX)

// Raw equation from Hvk_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC rewritten for 'r' and 'error'.
#define Hvk_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC_R(_N,_MAXERROR)    ((_MAXERROR) / (1 - HvkCos(Hvk_PI / HvkMax((float)(_N), Hvk_PI))))
#define Hvk_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC_ERROR(_N,_RAD)     ((1 - HvkCos(Hvk_PI / HvkMax((float)(_N), Hvk_PI))) / (_RAD))

// HvkDrawList: Lookup table size for adaptive arc drawing, cover full circle.
#ifndef Hvk_DRAWLIST_ARCFAST_TABLE_SIZE
#define Hvk_DRAWLIST_ARCFAST_TABLE_SIZE                          48 // Number of samples in lookup table.
#endif
#define Hvk_DRAWLIST_ARCFAST_SAMPLE_MAX                          Hvk_DRAWLIST_ARCFAST_TABLE_SIZE // Sample index _PathArcToFastEx() for 360 angle.

// Data shared between all HvkDrawList instances
// Conceptually this could have been called e.g. HvkDrawListSharedContext
// Typically one HvkGui context would create and maintain one of this.
// You may want to create your own instance of you try to HvkDrawList completely without HvkGui. In that case, watch out for future changes to this structure.
struct HvkGui_API HvkDrawListSharedData
{
    HvkVec2          TexUvWhitePixel;            // UV of white pixel in the atlas (== FontAtlas->TexUvWhitePixel)
    const HvkVec4*   TexUvLines;                 // UV of anti-aliased lines in the atlas (== FontAtlas->TexUvLines)
    HvkFontAtlas*    FontAtlas;                  // Current font atlas
    HvkFont*         Font;                       // Current font (used for simplified AddText overload)
    float           FontSize;                   // Current font size (used for for simplified AddText overload)
    float           FontScale;                  // Current font scale (== FontSize / Font->FontSize)
    float           CurveTessellationTol;       // Tessellation tolerance when using PathBezierCurveTo()
    float           CircleSegmentMaxError;      // Number of circle segments to use per pixel of radius for AddCircle() etc
    float           InitialFringeScale;         // Initial scale to apply to AA fringe
    HvkDrawListFlags InitialFlags;               // Initial flags at the beginning of the frame (it is possible to alter flags on a per-drawlist basis afterwards)
    HvkVec4          ClipRectFullscreen;         // Value for PushClipRectFullscreen()
    HvkVector<HvkVec2> TempBuffer;                // Temporary write buffer
    HvkVector<HvkDrawList*> DrawLists;            // All draw lists associated to this HvkDrawListSharedData
    HvkGuiContext*   Context;                    // [OPTIONAL] Link to Dear HvkGui context. 99% of HvkDrawList/HvkFontAtlas can function without an HvkGui context, but this facilitate handling one legacy edge case.

    // Lookup tables
    HvkVec2          ArcFastVtx[Hvk_DRAWLIST_ARCFAST_TABLE_SIZE]; // Sample points on the quarter of the circle.
    float           ArcFastRadiusCutoff;                        // Cutoff radius after which arc drawing will fallback to slower PathArcTo()
    HvkU8            CircleSegmentCounts[64];    // Precomputed segment count for given radius before we calculate it dynamically (to avoid calculation overhead)

    HvkDrawListSharedData();
    ~HvkDrawListSharedData();
    void SetCircleTessellationMaxError(float max_error);
};

struct HvkDrawDataBuilder
{
    HvkVector<HvkDrawList*>*  Layers[2];      // Pointers to global layers for: regular, tooltip. LayersP[0] is owned by DrawData.
    HvkVector<HvkDrawList*>   LayerData1;

    HvkDrawDataBuilder()                     { memset(this, 0, sizeof(*this)); }
};

struct HvkFontStackData
{
    HvkFont*     Font;
    float       FontSizeBeforeScaling;      // ~~ style.FontSizeBase
    float       FontSizeAfterScaling;       // ~~ g.FontSize
};

//-----------------------------------------------------------------------------
// [SECTION] Style support
//-----------------------------------------------------------------------------

struct HvkGuiStyleVarInfo
{
    HvkU32           Count : 8;      // 1+
    HvkGuiDataType   DataType : 8;
    HvkU32           Offset : 16;    // Offset in parent structure
    void* GetVarPtr(void* parent) const { return (void*)((unsigned char*)parent + Offset); }
};

// Stacked color modifier, backup of modified data so we can restore it
struct HvkGuiColorMod
{
    HvkGuiCol        Col;
    HvkVec4          BackupValue;
};

// Stacked style modifier, backup of modified data so we can restore it. Data type inferred from the variable.
struct HvkGuiStyleMod
{
    HvkGuiStyleVar   VarIdx;
    union           { int BackupInt[2]; float BackupFloat[2]; };
    HvkGuiStyleMod(HvkGuiStyleVar idx, int v)     { VarIdx = idx; BackupInt[0] = v; }
    HvkGuiStyleMod(HvkGuiStyleVar idx, float v)   { VarIdx = idx; BackupFloat[0] = v; }
    HvkGuiStyleMod(HvkGuiStyleVar idx, HvkVec2 v)  { VarIdx = idx; BackupFloat[0] = v.x; BackupFloat[1] = v.y; }
};

//-----------------------------------------------------------------------------
// [SECTION] Data types support
//-----------------------------------------------------------------------------

struct HvkGuiDataTypeStorage
{
    HvkU8        Data[8];        // Opaque storage to fit any data up to HvkGuiDataType_COUNT
};

// Type information associated to one HvkGuiDataType. Retrieve with DataTypeGetInfo().
struct HvkGuiDataTypeInfo
{
    size_t      Size;           // Size in bytes
    const char* Name;           // Short descriptive name for the type, for debugging
    const char* PrintFmt;       // Default printf format for the type
    const char* ScanFmt;        // Default scanf format for the type
};

// Extend HvkGuiDataType_
enum HvkGuiDataTypePrivate_
{
    HvkGuiDataType_Pointer = HvkGuiDataType_COUNT,
    HvkGuiDataType_ID,
};

//-----------------------------------------------------------------------------
// [SECTION] Widgets support: flags, enums, data structures
//-----------------------------------------------------------------------------

// Extend HvkGuiItemFlags
// - input: PushItemFlag() manipulates g.CurrentItemFlags, g.NextItemData.ItemFlags, ItemAdd() calls may add extra flags too.
// - output: stored in g.LastItemData.ItemFlags
enum HvkGuiItemFlagsPrivate_
{
    // Controlled by user
    HvkGuiItemFlags_Disabled                 = 1 << 10, // false     // Disable interactions (DOES NOT affect visuals. DO NOT mix direct use of this with BeginDisabled(). See BeginDisabled()/EndDisabled() for full disable feature, and github #211).
    HvkGuiItemFlags_ReadOnly                 = 1 << 11, // false     // [ALPHA] Allow hovering interactions but underlying value is not changed.
    HvkGuiItemFlags_MixedValue               = 1 << 12, // false     // [BETA] Represent a mixed/indeterminate value, generally multi-selection where values differ. Currently only supported by Checkbox() (later should support all sorts of widgets)
    HvkGuiItemFlags_NoWindowHoverableCheck   = 1 << 13, // false     // Disable hoverable check in ItemHoverable()
    HvkGuiItemFlags_AllowOverlap             = 1 << 14, // false     // Allow being overlapped by another widget. Not-hovered to Hovered transition deferred by a frame.
    HvkGuiItemFlags_NoNavDisableMouseHover   = 1 << 15, // false     // Nav keyboard/gamepad mode doesn't disable hover highlight (behave as if NavHighlightItemUnderNav==false).
    HvkGuiItemFlags_NoMarkEdited             = 1 << 16, // false     // Skip calling MarkItemEdited()
    HvkGuiItemFlags_NoFocus                  = 1 << 17, // false     // [EXPERIMENTAL: Not very well specced] Clicking doesn't take focus. Automatically sets HvkGuiButtonFlags_NoFocus + HvkGuiButtonFlags_NoNavFocus in ButtonBehavior().

    // Controlled by widget code
    HvkGuiItemFlags_Inputable                = 1 << 20, // false     // [WIP] Auto-activate input mode when tab focused. Currently only used and supported by a few items before it becomes a generic feature.
    HvkGuiItemFlags_HasSelectionUserData     = 1 << 21, // false     // Set by SetNextItemSelectionUserData()
    HvkGuiItemFlags_IsMultiSelect            = 1 << 22, // false     // Set by SetNextItemSelectionUserData()

    HvkGuiItemFlags_Default_                 = HvkGuiItemFlags_AutoClosePopups,    // Please don't change, use PushItemFlag() instead.

    // Obsolete
    //HvkGuiItemFlags_SelectableDontClosePopup = !HvkGuiItemFlags_AutoClosePopups, // Can't have a redirect as we inverted the behavior
};

// Status flags for an already submitted item
// - output: stored in g.LastItemData.StatusFlags
enum HvkGuiItemStatusFlags_
{
    HvkGuiItemStatusFlags_None               = 0,
    HvkGuiItemStatusFlags_HoveredRect        = 1 << 0,   // Mouse position is within item rectangle (does NOT mean that the window is in correct z-order and can be hovered!, this is only one part of the most-common IsItemHovered test)
    HvkGuiItemStatusFlags_HasDisplayRect     = 1 << 1,   // g.LastItemData.DisplayRect is valid
    HvkGuiItemStatusFlags_Edited             = 1 << 2,   // Value exposed by item was edited in the current frame (should match the bool return value of most widgets)
    HvkGuiItemStatusFlags_ToggledSelection   = 1 << 3,   // Set when Selectable(), TreeNode() reports toggling a selection. We can't report "Selected", only state changes, in order to easily handle clipping with less issues.
    HvkGuiItemStatusFlags_ToggledOpen        = 1 << 4,   // Set when TreeNode() reports toggling their open state.
    HvkGuiItemStatusFlags_HasDeactivated     = 1 << 5,   // Set if the widget/group is able to provide data for the HvkGuiItemStatusFlags_Deactivated flag.
    HvkGuiItemStatusFlags_Deactivated        = 1 << 6,   // Only valid if HvkGuiItemStatusFlags_HasDeactivated is set.
    HvkGuiItemStatusFlags_HoveredWindow      = 1 << 7,   // Override the HoveredWindow test to allow cross-window hover testing.
    HvkGuiItemStatusFlags_Visible            = 1 << 8,   // [WIP] Set when item is overlapping the current clipping rectangle (Used internally. Please don't use yet: API/system will change as we refactor Itemadd()).
    HvkGuiItemStatusFlags_HasClipRect        = 1 << 9,   // g.LastItemData.ClipRect is valid.
    HvkGuiItemStatusFlags_HasShortcut        = 1 << 10,  // g.LastItemData.Shortcut valid. Set by SetNextItemShortcut() -> ItemAdd().
    //HvkGuiItemStatusFlags_FocusedByTabbing = 1 << 8,   // Removed IN 1.90.1 (Dec 2023). The trigger is part of g.NavActivateId. See commit 54c1bdeceb.

    // Additional status + semantic for HvkGuiTestEngine
#ifdef HvkGui_ENABLE_TEST_ENGINE
    HvkGuiItemStatusFlags_Openable           = 1 << 20,  // Item is an openable (e.g. TreeNode)
    HvkGuiItemStatusFlags_Opened             = 1 << 21,  // Opened status
    HvkGuiItemStatusFlags_Checkable          = 1 << 22,  // Item is a checkable (e.g. CheckBox, MenuItem)
    HvkGuiItemStatusFlags_Checked            = 1 << 23,  // Checked status
    HvkGuiItemStatusFlags_Inputable          = 1 << 24,  // Item is a text-inputable (e.g. InputText, SliderXXX, DragXXX)
#endif
};

// Extend HvkGuiHoveredFlags_
enum HvkGuiHoveredFlagsPrivate_
{
    HvkGuiHoveredFlags_DelayMask_                    = HvkGuiHoveredFlags_DelayNone | HvkGuiHoveredFlags_DelayShort | HvkGuiHoveredFlags_DelayNormal | HvkGuiHoveredFlags_NoSharedDelay,
    HvkGuiHoveredFlags_AllowedMaskForIsWindowHovered = HvkGuiHoveredFlags_ChildWindows | HvkGuiHoveredFlags_RootWindow | HvkGuiHoveredFlags_AnyWindow | HvkGuiHoveredFlags_NoPopupHierarchy | HvkGuiHoveredFlags_AllowWhenBlockedByPopup | HvkGuiHoveredFlags_AllowWhenBlockedByActiveItem | HvkGuiHoveredFlags_ForTooltip | HvkGuiHoveredFlags_Stationary,
    HvkGuiHoveredFlags_AllowedMaskForIsItemHovered   = HvkGuiHoveredFlags_AllowWhenBlockedByPopup | HvkGuiHoveredFlags_AllowWhenBlockedByActiveItem | HvkGuiHoveredFlags_AllowWhenOverlapped | HvkGuiHoveredFlags_AllowWhenDisabled | HvkGuiHoveredFlags_NoNavOverride | HvkGuiHoveredFlags_ForTooltip | HvkGuiHoveredFlags_Stationary | HvkGuiHoveredFlags_DelayMask_,
};

// Extend HvkGuiInputTextFlags_
enum HvkGuiInputTextFlagsPrivate_
{
    // [Internal]
    HvkGuiInputTextFlags_Multiline           = 1 << 26,  // For internal use by InputTextMultiline()
    HvkGuiInputTextFlags_MergedItem          = 1 << 27,  // For internal use by TempInputText(), will skip calling ItemAdd(). Require bounding-box to strictly match.
    HvkGuiInputTextFlags_LocalizeDecimalPoint= 1 << 28,  // For internal use by InputScalar() and TempInputScalar()
};

// Extend HvkGuiButtonFlags_
enum HvkGuiButtonFlagsPrivate_
{
    HvkGuiButtonFlags_PressedOnClick         = 1 << 4,   // return true on click (mouse down event)
    HvkGuiButtonFlags_PressedOnClickRelease  = 1 << 5,   // [Default] return true on click + release on same item <-- this is what the majority of Button are using
    HvkGuiButtonFlags_PressedOnClickReleaseAnywhere = 1 << 6, // return true on click + release even if the release event is not done while hovering the item
    HvkGuiButtonFlags_PressedOnRelease       = 1 << 7,   // return true on release (default requires click+release)
    HvkGuiButtonFlags_PressedOnDoubleClick   = 1 << 8,   // return true on double-click (default requires click+release)
    HvkGuiButtonFlags_PressedOnDragDropHold  = 1 << 9,   // return true when held into while we are drag and dropping another item (used by e.g. tree nodes, collapsing headers)
    //HvkGuiButtonFlags_Repeat               = 1 << 10,  // hold to repeat -> use HvkGuiItemFlags_ButtonRepeat instead.
    HvkGuiButtonFlags_FlattenChildren        = 1 << 11,  // allow interactions even if a child window is overlapping
    HvkGuiButtonFlags_AllowOverlap           = 1 << 12,  // require previous frame HoveredId to either match id or be null before being usable.
    //HvkGuiButtonFlags_DontClosePopups      = 1 << 13,  // disable automatically closing parent popup on press
    //HvkGuiButtonFlags_Disabled             = 1 << 14,  // disable interactions -> use BeginDisabled() or HvkGuiItemFlags_Disabled
    HvkGuiButtonFlags_AlignTextBaseLine      = 1 << 15,  // vertically align button to match text baseline - ButtonEx() only // FIXME: Should be removed and handled by SmallButton(), not possible currently because of DC.CursorPosPrevLine
    HvkGuiButtonFlags_NoKeyModsAllowed       = 1 << 16,  // disable mouse interaction if a key modifier is held
    HvkGuiButtonFlags_NoHoldingActiveId      = 1 << 17,  // don't set ActiveId while holding the mouse (HvkGuiButtonFlags_PressedOnClick only)
    HvkGuiButtonFlags_NoNavFocus             = 1 << 18,  // don't override navigation focus when activated (FIXME: this is essentially used every time an item uses HvkGuiItemFlags_NoNav, but because legacy specs don't requires LastItemData to be set ButtonBehavior(), we can't poll g.LastItemData.ItemFlags)
    HvkGuiButtonFlags_NoHoveredOnFocus       = 1 << 19,  // don't report as hovered when nav focus is on this item
    HvkGuiButtonFlags_NoSetKeyOwner          = 1 << 20,  // don't set key/input owner on the initial click (note: mouse buttons are keys! often, the key in question will be HvkGuiKey_MouseLeft!)
    HvkGuiButtonFlags_NoTestKeyOwner         = 1 << 21,  // don't test key/input owner when polling the key (note: mouse buttons are keys! often, the key in question will be HvkGuiKey_MouseLeft!)
    HvkGuiButtonFlags_NoFocus                = 1 << 22,  // [EXPERIMENTAL: Not very well specced]. Don't focus parent window when clicking.
    HvkGuiButtonFlags_PressedOnMask_         = HvkGuiButtonFlags_PressedOnClick | HvkGuiButtonFlags_PressedOnClickRelease | HvkGuiButtonFlags_PressedOnClickReleaseAnywhere | HvkGuiButtonFlags_PressedOnRelease | HvkGuiButtonFlags_PressedOnDoubleClick | HvkGuiButtonFlags_PressedOnDragDropHold,
    HvkGuiButtonFlags_PressedOnDefault_      = HvkGuiButtonFlags_PressedOnClickRelease,
    //HvkGuiButtonFlags_NoKeyModifiers       = HvkGuiButtonFlags_NoKeyModsAllowed, // Renamed in 1.91.4
};

// Extend HvkGuiComboFlags_
enum HvkGuiComboFlagsPrivate_
{
    HvkGuiComboFlags_CustomPreview           = 1 << 20,  // enable BeginComboPreview()
};

// Extend HvkGuiSliderFlags_
enum HvkGuiSliderFlagsPrivate_
{
    HvkGuiSliderFlags_Vertical               = 1 << 20,  // Should this slider be orientated vertically?
    HvkGuiSliderFlags_ReadOnly               = 1 << 21,  // Consider using g.NextItemData.ItemFlags |= HvkGuiItemFlags_ReadOnly instead.
};

// Extend HvkGuiSelectableFlags_
enum HvkGuiSelectableFlagsPrivate_
{
    // NB: need to be in sync with last value of HvkGuiSelectableFlags_
    HvkGuiSelectableFlags_NoHoldingActiveID      = 1 << 20,
    HvkGuiSelectableFlags_SelectOnClick          = 1 << 22,  // Override button behavior to react on Click (default is Click+Release)
    HvkGuiSelectableFlags_SelectOnRelease        = 1 << 23,  // Override button behavior to react on Release (default is Click+Release)
    HvkGuiSelectableFlags_SpanAvailWidth         = 1 << 24,  // Span all avail width even if we declared less for layout purpose. FIXME: We may be able to remove this (added in 6251d379, 2bcafc86 for menus)
    HvkGuiSelectableFlags_SetNavIdOnHover        = 1 << 25,  // Set Nav/Focus ID on mouse hover (used by MenuItem)
    HvkGuiSelectableFlags_NoPadWithHalfSpacing   = 1 << 26,  // Disable padding each side with ItemSpacing * 0.5f
    HvkGuiSelectableFlags_NoSetKeyOwner          = 1 << 27,  // Don't set key/input owner on the initial click (note: mouse buttons are keys! often, the key in question will be HvkGuiKey_MouseLeft!)
};

// Extend HvkGuiTreeNodeFlags_
enum HvkGuiTreeNodeFlagsPrivate_
{
    HvkGuiTreeNodeFlags_NoNavFocus                 = 1 << 27,// Don't claim nav focus when interacting with this item (#8551)
    HvkGuiTreeNodeFlags_ClipLabelForTrailingButton = 1 << 28,// FIXME-WIP: Hard-coded for CollapsingHeader()
    HvkGuiTreeNodeFlags_UpsideDownArrow            = 1 << 29,// FIXME-WIP: Turn Down arrow into an Up arrow, for reversed trees (#6517)
    HvkGuiTreeNodeFlags_OpenOnMask_                = HvkGuiTreeNodeFlags_OpenOnDoubleClick | HvkGuiTreeNodeFlags_OpenOnArrow,
    HvkGuiTreeNodeFlags_DrawLinesMask_             = HvkGuiTreeNodeFlags_DrawLinesNone | HvkGuiTreeNodeFlags_DrawLinesFull | HvkGuiTreeNodeFlags_DrawLinesToNodes,
};

enum HvkGuiSeparatorFlags_
{
    HvkGuiSeparatorFlags_None                    = 0,
    HvkGuiSeparatorFlags_Horizontal              = 1 << 0,   // Axis default to current layout type, so generally Horizontal unless e.g. in a menu bar
    HvkGuiSeparatorFlags_Vertical                = 1 << 1,
    HvkGuiSeparatorFlags_SpanAllColumns          = 1 << 2,   // Make separator cover all columns of a legacy Columns() set.
};

// Flags for FocusWindow(). This is not called HvkGuiFocusFlags to avoid confusion with public-facing HvkGuiFocusedFlags.
// FIXME: Once we finishing replacing more uses of GetTopMostPopupModal()+IsWindowWithinBeginStackOf()
// and FindBlockingModal() with this, we may want to change the flag to be opt-out instead of opt-in.
enum HvkGuiFocusRequestFlags_
{
    HvkGuiFocusRequestFlags_None                 = 0,
    HvkGuiFocusRequestFlags_RestoreFocusedChild  = 1 << 0,   // Find last focused child (if any) and focus it instead.
    HvkGuiFocusRequestFlags_UnlessBelowModal     = 1 << 1,   // Do not set focus if the window is below a modal.
};

enum HvkGuiTextFlags_
{
    HvkGuiTextFlags_None                         = 0,
    HvkGuiTextFlags_NoWidthForLargeClippedText   = 1 << 0,
};

enum HvkGuiTooltipFlags_
{
    HvkGuiTooltipFlags_None                      = 0,
    HvkGuiTooltipFlags_OverridePrevious          = 1 << 1,   // Clear/ignore previously submitted tooltip (defaults to append)
};

// FIXME: this is in development, not exposed/functional as a generic feature yet.
// Horizontal/Vertical enums are fixed to 0/1 so they may be used to index HvkVec2
enum HvkGuiLayoutType_
{
    HvkGuiLayoutType_Horizontal = 0,
    HvkGuiLayoutType_Vertical = 1
};

// Flags for LogBegin() text capturing function
enum HvkGuiLogFlags_
{
    HvkGuiLogFlags_None = 0,

    HvkGuiLogFlags_OutputTTY         = 1 << 0,
    HvkGuiLogFlags_OutputFile        = 1 << 1,
    HvkGuiLogFlags_OutputBuffer      = 1 << 2,
    HvkGuiLogFlags_OutputClipboard   = 1 << 3,
    HvkGuiLogFlags_OutputMask_       = HvkGuiLogFlags_OutputTTY | HvkGuiLogFlags_OutputFile | HvkGuiLogFlags_OutputBuffer | HvkGuiLogFlags_OutputClipboard,
};

// X/Y enums are fixed to 0/1 so they may be used to index HvkVec2
enum HvkGuiAxis
{
    HvkGuiAxis_None = -1,
    HvkGuiAxis_X = 0,
    HvkGuiAxis_Y = 1
};

enum HvkGuiPlotType
{
    HvkGuiPlotType_Lines,
    HvkGuiPlotType_Histogram,
};

// Storage data for BeginComboPreview()/EndComboPreview()
struct HvkGui_API HvkGuiComboPreviewData
{
    HvkRect          PreviewRect;
    HvkVec2          BackupCursorPos;
    HvkVec2          BackupCursorMaxPos;
    HvkVec2          BackupCursorPosPrevLine;
    float           BackupPrevLineTextBaseOffset;
    HvkGuiLayoutType BackupLayout;

    HvkGuiComboPreviewData() { memset(this, 0, sizeof(*this)); }
};

// Stacked storage data for BeginGroup()/EndGroup()
struct HvkGui_API HvkGuiGroupData
{
    HvkGuiID     WindowID;
    HvkVec2      BackupCursorPos;
    HvkVec2      BackupCursorMaxPos;
    HvkVec2      BackupCursorPosPrevLine;
    HvkVec1      BackupIndent;
    HvkVec1      BackupGroupOffset;
    HvkVec2      BackupCurrLineSize;
    float       BackupCurrLineTextBaseOffset;
    HvkGuiID     BackupActiveIdIsAlive;
    bool        BackupActiveIdHasBeenEditedThisFrame;
    bool        BackupDeactivatedIdIsAlive;
    bool        BackupHoveredIdIsAlive;
    bool        BackupIsSameLine;
    bool        EmitItem;
};

// Simple column measurement, currently used for MenuItem() only.. This is very short-sighted/throw-away code and NOT a generic helper.
struct HvkGui_API HvkGuiMenuColumns
{
    HvkU32       TotalWidth;
    HvkU32       NextTotalWidth;
    HvkU16       Spacing;
    HvkU16       OffsetIcon;         // Always zero for now
    HvkU16       OffsetLabel;        // Offsets are locked in Update()
    HvkU16       OffsetShortcut;
    HvkU16       OffsetMark;
    HvkU16       Widths[4];          // Width of:   Icon, Label, Shortcut, Mark  (accumulators for current frame)

    HvkGuiMenuColumns() { memset(this, 0, sizeof(*this)); }
    void        Update(float spacing, bool window_reappearing);
    float       DeclColumns(float w_icon, float w_label, float w_shortcut, float w_mark);
    void        CalcNextTotalWidth(bool update_offsets);
};

// Internal temporary state for deactivating InputText() instances.
struct HvkGui_API HvkGuiInputTextDeactivatedState
{
    HvkGuiID            ID;              // widget id owning the text state (which just got deactivated)
    HvkVector<char>     TextA;           // text buffer

    HvkGuiInputTextDeactivatedState()    { memset(this, 0, sizeof(*this)); }
    void    ClearFreeMemory()           { ID = 0; TextA.clear(); }
};

// Forward declare Hvkstb_textedit.h structure + make its main configuration define accessible
#undef HvkSTB_TEXTEDIT_STRING
#undef HvkSTB_TEXTEDIT_CHARTYPE
#define HvkSTB_TEXTEDIT_STRING             HvkGuiInputTextState
#define HvkSTB_TEXTEDIT_CHARTYPE           char
#define HvkSTB_TEXTEDIT_GETWIDTH_NEWLINE   (-1.0f)
#define HvkSTB_TEXTEDIT_UNDOSTATECOUNT     99
#define HvkSTB_TEXTEDIT_UNDOCHARCOUNT      999
namespace HvkStb { struct STB_TexteditState; }
typedef HvkStb::STB_TexteditState HvkStbTexteditState;

// Internal state of the currently focused/edited text input box
// For a given item ID, access with HvkGui::GetInputTextState()
struct HvkGui_API HvkGuiInputTextState
{
    HvkGuiContext*           Ctx;                    // parent UI context (needs to be set explicitly by parent).
    HvkStbTexteditState*     Stb;                    // State for stb_textedit.h
    HvkGuiInputTextFlags     Flags;                  // copy of InputText() flags. may be used to check if e.g. HvkGuiInputTextFlags_Password is set.
    HvkGuiID                 ID;                     // widget id owning the text state
    int                     TextLen;                // UTF-8 length of the string in TextA (in bytes)
    const char*             TextSrc;                // == TextA.Data unless read-only, in which case == buf passed to InputText(). Field only set and valid _inside_ the call InputText() call.
    HvkVector<char>          TextA;                  // main UTF8 buffer. TextA.Size is a buffer size! Should always be >= buf_size passed by user (and of course >= CurLenA + 1).
    HvkVector<char>          TextToRevertTo;         // value to revert to when pressing Escape = backup of end-user buffer at the time of focus (in UTF-8, unaltered)
    HvkVector<char>          CallbackTextBackup;     // temporary storage for callback to support automatic reconcile of undo-stack
    int                     BufCapacity;            // end-user buffer capacity (include zero terminator)
    HvkVec2                  Scroll;                 // horizontal offset (managed manually) + vertical scrolling (pulled from child window's own Scroll.y)
    int                     LineCount;              // last line count (solely for debugging)
    float                   WrapWidth;              // word-wrapping width
    float                   CursorAnim;             // timer for cursor blink, reset on every user action so the cursor reappears Hvkmediately
    bool                    CursorFollow;           // set when we want scrolling to follow the current cursor position (not always!)
    bool                    CursorCenterY;          // set when we want scrolling to be centered over the cursor position (while resizing a word-wrapping field)
    bool                    SelectedAllMouseLock;   // after a double-click to select all, we ignore further mouse drags to update selection
    bool                    Edited;                 // edited this frame
    bool                    WantReloadUserBuf;      // force a reload of user buf so it may be modified externally. may be automatic in future version.
    HvkS8                    LastMoveDirectionLR;    // HvkGuiDir_Left or HvkGuiDir_Right. track last movement direction so when cursor cross over a word-wrapping boundaries we can display it on either line depending on last move.s
    int                     ReloadSelectionStart;
    int                     ReloadSelectionEnd;

    HvkGuiInputTextState();
    ~HvkGuiInputTextState();
    void        ClearText()                 { TextLen = 0; TextA[0] = 0; CursorClamp(); }
    void        ClearFreeMemory()           { TextA.clear(); TextToRevertTo.clear(); }
    void        OnKeyPressed(int key);      // Cannot be inline because we call in code in stb_textedit.h Hvkplementation
    void        OnCharPressed(unsigned int c);
    float       GetPreferredOffsetX() const;

    // Cursor & Selection
    void        CursorAnimReset();
    void        CursorClamp();
    bool        HasSelection() const;
    void        ClearSelection();
    int         GetCursorPos() const;
    int         GetSelectionStart() const;
    int         GetSelectionEnd() const;
    void        SelectAll();

    // Reload user buf (WIP #2890)
    // If you modify underlying user-passed const char* while active you need to call this (InputText V2 may lift this)
    //   strcpy(my_buf, "hello");
    //   if (HvkGuiInputTextState* state = HvkGui::GetInputTextState(id)) // id may be HvkGui::GetItemID() is last item
    //       state->ReloadUserBufAndSelectAll();
    void        ReloadUserBufAndSelectAll();
    void        ReloadUserBufAndKeepSelection();
    void        ReloadUserBufAndMoveToEnd();
};

enum HvkGuiWindowRefreshFlags_
{
    HvkGuiWindowRefreshFlags_None                = 0,
    HvkGuiWindowRefreshFlags_TryToAvoidRefresh   = 1 << 0,   // [EXPERIMENTAL] Try to keep existing contents, USER MUST NOT HONOR BEGIN() RETURNING FALSE AND NOT APPEND.
    HvkGuiWindowRefreshFlags_RefreshOnHover      = 1 << 1,   // [EXPERIMENTAL] Always refresh on hover
    HvkGuiWindowRefreshFlags_RefreshOnFocus      = 1 << 2,   // [EXPERIMENTAL] Always refresh on focus
    // Refresh policy/frequency, Load Balancing etc.
};

enum HvkGuiWindowBgClickFlags_
{
    HvkGuiWindowBgClickFlags_None                = 0,
    HvkGuiWindowBgClickFlags_Move                = 1 << 0,   // Click on bg/void + drag to move window. Cleared by default when using io.ConfigWindowsMoveFromTitleBarOnly.
};

enum HvkGuiNextWindowDataFlags_
{
    HvkGuiNextWindowDataFlags_None               = 0,
    HvkGuiNextWindowDataFlags_HasPos             = 1 << 0,
    HvkGuiNextWindowDataFlags_HasSize            = 1 << 1,
    HvkGuiNextWindowDataFlags_HasContentSize     = 1 << 2,
    HvkGuiNextWindowDataFlags_HasCollapsed       = 1 << 3,
    HvkGuiNextWindowDataFlags_HasSizeConstraint  = 1 << 4,
    HvkGuiNextWindowDataFlags_HasFocus           = 1 << 5,
    HvkGuiNextWindowDataFlags_HasBgAlpha         = 1 << 6,
    HvkGuiNextWindowDataFlags_HasScroll          = 1 << 7,
    HvkGuiNextWindowDataFlags_HasWindowFlags     = 1 << 8,
    HvkGuiNextWindowDataFlags_HasChildFlags      = 1 << 9,
    HvkGuiNextWindowDataFlags_HasRefreshPolicy   = 1 << 10,
};

// Storage for SetNexWindow** functions
struct HvkGuiNextWindowData
{
    HvkGuiNextWindowDataFlags    HasFlags;

    // Members below are NOT cleared. Always rely on HasFlags.
    HvkGuiCond                   PosCond;
    HvkGuiCond                   SizeCond;
    HvkGuiCond                   CollapsedCond;
    HvkVec2                      PosVal;
    HvkVec2                      PosPivotVal;
    HvkVec2                      SizeVal;
    HvkVec2                      ContentSizeVal;
    HvkVec2                      ScrollVal;
    HvkGuiWindowFlags            WindowFlags;            // Only honored by BeginTable()
    HvkGuiChildFlags             ChildFlags;
    bool                        CollapsedVal;
    HvkRect                      SizeConstraintRect;
    HvkGuiSizeCallback           SizeCallback;
    void*                       SizeCallbackUserData;
    float                       BgAlphaVal;             // Override background alpha
    HvkVec2                      MenuBarOffsetMinVal;    // (Always on) This is not exposed publicly, so we don't clear it and it doesn't have a corresponding flag (could we? for consistency?)
    HvkGuiWindowRefreshFlags     RefreshFlagsVal;

    HvkGuiNextWindowData()       { memset(this, 0, sizeof(*this)); }
    inline void ClearFlags()    { HasFlags = HvkGuiNextWindowDataFlags_None; }
};

enum HvkGuiNextItemDataFlags_
{
    HvkGuiNextItemDataFlags_None             = 0,
    HvkGuiNextItemDataFlags_HasWidth         = 1 << 0,
    HvkGuiNextItemDataFlags_HasOpen          = 1 << 1,
    HvkGuiNextItemDataFlags_HasShortcut      = 1 << 2,
    HvkGuiNextItemDataFlags_HasRefVal        = 1 << 3,
    HvkGuiNextItemDataFlags_HasStorageID     = 1 << 4,
    HvkGuiNextItemDataFlags_HasColorMarker   = 1 << 5,
};

struct HvkGuiNextItemData
{
    HvkGuiNextItemDataFlags      HasFlags;           // Called HasFlags instead of Flags to avoid mistaking this
    HvkGuiItemFlags              ItemFlags;          // Currently only tested/used for HvkGuiItemFlags_AllowOverlap and HvkGuiItemFlags_HasSelectionUserData.

    // Members below are NOT cleared by ItemAdd() meaning they are still valid during e.g. NavProcessItem(). Always rely on HasFlags.
    HvkGuiID                     FocusScopeId;       // Set by SetNextItemSelectionUserData()
    HvkGuiSelectionUserData      SelectionUserData;  // Set by SetNextItemSelectionUserData() (note that NULL/0 is a valid value, we use -1 == HvkGuiSelectionUserData_Invalid to mark invalid values)
    float                       Width;              // Set by SetNextItemWidth()
    HvkGuiKeyChord               Shortcut;           // Set by SetNextItemShortcut()
    HvkGuiInputFlags             ShortcutFlags;      // Set by SetNextItemShortcut()
    bool                        OpenVal;            // Set by SetNextItemOpen()
    HvkU8                        OpenCond;           // Set by SetNextItemOpen()
    HvkGuiDataTypeStorage        RefVal;             // Not exposed yet, for HvkGuiInputTextFlags_ParseEmptyAsRefVal
    HvkGuiID                     StorageId;          // Set by SetNextItemStorageID()
    HvkU32                       ColorMarker;        // Set by SetNextItemColorMarker(). Not exposed yet, supported by DragScalar,SliderScalar and for HvkGuiSliderFlags_ColorMarkers.

    HvkGuiNextItemData()         { memset(this, 0, sizeof(*this)); SelectionUserData = -1; }
    inline void ClearFlags()    { HasFlags = HvkGuiNextItemDataFlags_None; ItemFlags = HvkGuiItemFlags_None; } // Also cleared manually by ItemAdd()!
};

// Status storage for the last submitted item
struct HvkGuiLastItemData
{
    HvkGuiID                 ID;
    HvkGuiItemFlags          ItemFlags;          // See HvkGuiItemFlags_ (called 'InFlags' before v1.91.4).
    HvkGuiItemStatusFlags    StatusFlags;        // See HvkGuiItemStatusFlags_
    HvkRect                  Rect;               // Full rectangle
    HvkRect                  NavRect;            // Navigation scoring rectangle (not displayed)
    // Rarely used fields are not explicitly cleared, only valid when the corresponding HvkGuiItemStatusFlags are set.
    HvkRect                  DisplayRect;        // Display rectangle. ONLY VALID IF (StatusFlags & HvkGuiItemStatusFlags_HasDisplayRect) is set.
    HvkRect                  ClipRect;           // Clip rectangle at the time of submitting item. ONLY VALID IF (StatusFlags & HvkGuiItemStatusFlags_HasClipRect) is set..
    HvkGuiKeyChord           Shortcut;           // Shortcut at the time of submitting item. ONLY VALID IF (StatusFlags & HvkGuiItemStatusFlags_HasShortcut) is set..

    HvkGuiLastItemData()     { memset(this, 0, sizeof(*this)); }
};

// Store data emitted by TreeNode() for usage by TreePop()
// - To Hvkplement HvkGuiTreeNodeFlags_NavLeftJumpsToParent: store the minimum amount of data
//   which we can't infer in TreePop(), to perform the equivalent of NavApplyItemToResult().
//   Only stored when the node is a potential candidate for landing on a Left arrow jump.
struct HvkGuiTreeNodeStackData
{
    HvkGuiID                 ID;
    HvkGuiTreeNodeFlags      TreeFlags;
    HvkGuiItemFlags          ItemFlags;      // Used for nav landing
    HvkRect                  NavRect;        // Used for nav landing
    float                   DrawLinesX1;
    float                   DrawLinesToNodesY2;
    HvkGuiTableColumnIdx     DrawLinesTableColumn;
};

// sizeof() = 20
struct HvkGui_API HvkGuiErrorRecoveryState
{
    short   SizeOfWindowStack;
    short   SizeOfIDStack;
    short   SizeOfTreeStack;
    short   SizeOfColorStack;
    short   SizeOfStyleVarStack;
    short   SizeOfFontStack;
    short   SizeOfFocusScopeStack;
    short   SizeOfGroupStack;
    short   SizeOfItemFlagsStack;
    short   SizeOfBeginPopupStack;
    short   SizeOfDisabledStack;

    HvkGuiErrorRecoveryState() { memset(this, 0, sizeof(*this)); }
};

// Data saved for each window pushed into the stack
struct HvkGuiWindowStackData
{
    HvkGuiWindow*            Window;
    HvkGuiLastItemData       ParentLastItemDataBackup;
    HvkGuiErrorRecoveryState StackSizesInBegin;          // Store size of various stacks for asserting
    bool                    DisabledOverrideReenable;   // Non-child window override disabled flag
    float                   DisabledOverrideReenableAlphaBackup;
};

struct HvkGuiShrinkWidthItem
{
    int         Index;
    float       Width;
    float       InitialWidth;
};

struct HvkGuiPtrOrIndex
{
    void*       Ptr;            // Either field can be set, not both. e.g. Dock node tab bars are loose while BeginTabBar() ones are in a pool.
    int         Index;          // Usually index in a main pool.

    HvkGuiPtrOrIndex(void* ptr)  { Ptr = ptr; Index = -1; }
    HvkGuiPtrOrIndex(int index)  { Ptr = NULL; Index = index; }
};

// Data used by IsItemDeactivated()/IsItemDeactivatedAfterEdit() functions
struct HvkGuiDeactivatedItemData
{
    HvkGuiID     ID;
    int         ElapseFrame;
    bool        HasBeenEditedBefore;
    bool        IsAlive;
};

//-----------------------------------------------------------------------------
// [SECTION] Popup support
//-----------------------------------------------------------------------------

enum HvkGuiPopupPositionPolicy
{
    HvkGuiPopupPositionPolicy_Default,
    HvkGuiPopupPositionPolicy_ComboBox,
    HvkGuiPopupPositionPolicy_Tooltip,
};

// Storage for popup stacks (g.OpenPopupStack and g.BeginPopupStack)
struct HvkGuiPopupData
{
    HvkGuiID             PopupId;        // Set on OpenPopup()
    HvkGuiWindow*        Window;         // Resolved on BeginPopup() - may stay unresolved if user never calls OpenPopup()
    HvkGuiWindow*        RestoreNavWindow;// Set on OpenPopup(), a NavWindow that will be restored on popup close
    int                 ParentNavLayer; // Resolved on BeginPopup(). Actually a HvkGuiNavLayer type (declared down below), initialized to -1 which is not part of an enum, but serves well-enough as "not any of layers" value
    int                 OpenFrameCount; // Set on OpenPopup()
    HvkGuiID             OpenParentId;   // Set on OpenPopup(), we need this to differentiate multiple menu sets from each others (e.g. inside menu bar vs loose menu items)
    HvkVec2              OpenPopupPos;   // Set on OpenPopup(), preferred popup position (typically == OpenMousePos when using mouse)
    HvkVec2              OpenMousePos;   // Set on OpenPopup(), copy of mouse position at the time of opening popup

    HvkGuiPopupData()    { memset(this, 0, sizeof(*this)); ParentNavLayer = OpenFrameCount = -1; }
};

//-----------------------------------------------------------------------------
// [SECTION] Inputs support
//-----------------------------------------------------------------------------

// Bit array for named keys
typedef HvkBitArray<HvkGuiKey_NamedKey_COUNT, -HvkGuiKey_NamedKey_BEGIN>    HvkBitArrayForNamedKeys;

// [Internal] Key ranges
#define HvkGuiKey_LegacyNativeKey_BEGIN  0
#define HvkGuiKey_LegacyNativeKey_END    512
#define HvkGuiKey_Keyboard_BEGIN         (HvkGuiKey_NamedKey_BEGIN)
#define HvkGuiKey_Keyboard_END           (HvkGuiKey_GamepadStart)
#define HvkGuiKey_Gamepad_BEGIN          (HvkGuiKey_GamepadStart)
#define HvkGuiKey_Gamepad_END            (HvkGuiKey_GamepadRStickDown + 1)
#define HvkGuiKey_Mouse_BEGIN            (HvkGuiKey_MouseLeft)
#define HvkGuiKey_Mouse_END              (HvkGuiKey_MouseWheelY + 1)
#define HvkGuiKey_Aliases_BEGIN          (HvkGuiKey_Mouse_BEGIN)
#define HvkGuiKey_Aliases_END            (HvkGuiKey_Mouse_END)

// [Internal] Named shortcuts for Navigation
#define HvkGuiKey_NavKeyboardTweakSlow   HvkGuiMod_Ctrl
#define HvkGuiKey_NavKeyboardTweakFast   HvkGuiMod_Shift
#define HvkGuiKey_NavGamepadTweakSlow    HvkGuiKey_GamepadL1
#define HvkGuiKey_NavGamepadTweakFast    HvkGuiKey_GamepadR1
#define HvkGuiKey_NavGamepadActivate     (g.IO.ConfigNavSwapGamepadButtons ? HvkGuiKey_GamepadFaceRight : HvkGuiKey_GamepadFaceDown)
#define HvkGuiKey_NavGamepadCancel       (g.IO.ConfigNavSwapGamepadButtons ? HvkGuiKey_GamepadFaceDown : HvkGuiKey_GamepadFaceRight)
#define HvkGuiKey_NavGamepadMenu         HvkGuiKey_GamepadFaceLeft
#define HvkGuiKey_NavGamepadInput        HvkGuiKey_GamepadFaceUp

enum HvkGuiInputEventType
{
    HvkGuiInputEventType_None = 0,
    HvkGuiInputEventType_MousePos,
    HvkGuiInputEventType_MouseWheel,
    HvkGuiInputEventType_MouseButton,
    HvkGuiInputEventType_Key,
    HvkGuiInputEventType_Text,
    HvkGuiInputEventType_Focus,
    HvkGuiInputEventType_COUNT
};

enum HvkGuiInputSource : int
{
    HvkGuiInputSource_None = 0,
    HvkGuiInputSource_Mouse,         // Note: may be Mouse or TouchScreen or Pen. See io.MouseSource to distinguish them.
    HvkGuiInputSource_Keyboard,
    HvkGuiInputSource_Gamepad,
    HvkGuiInputSource_COUNT
};

// FIXME: Structures in the union below need to be declared as anonymous unions appears to be an extension?
// Using HvkVec2() would fail on Clang 'union member 'MousePos' has a non-trivial default constructor'
struct HvkGuiInputEventMousePos      { float PosX, PosY; HvkGuiMouseSource MouseSource; };
struct HvkGuiInputEventMouseWheel    { float WheelX, WheelY; HvkGuiMouseSource MouseSource; };
struct HvkGuiInputEventMouseButton   { int Button; bool Down; HvkGuiMouseSource MouseSource; };
struct HvkGuiInputEventKey           { HvkGuiKey Key; bool Down; float AnalogValue; };
struct HvkGuiInputEventText          { unsigned int Char; };
struct HvkGuiInputEventAppFocused    { bool Focused; };

struct HvkGuiInputEvent
{
    HvkGuiInputEventType             Type;
    HvkGuiInputSource                Source;
    HvkU32                           EventId;        // Unique, sequential increasing integer to identify an event (if you need to correlate them to other data).
    union
    {
        HvkGuiInputEventMousePos     MousePos;       // if Type == HvkGuiInputEventType_MousePos
        HvkGuiInputEventMouseWheel   MouseWheel;     // if Type == HvkGuiInputEventType_MouseWheel
        HvkGuiInputEventMouseButton  MouseButton;    // if Type == HvkGuiInputEventType_MouseButton
        HvkGuiInputEventKey          Key;            // if Type == HvkGuiInputEventType_Key
        HvkGuiInputEventText         Text;           // if Type == HvkGuiInputEventType_Text
        HvkGuiInputEventAppFocused   AppFocused;     // if Type == HvkGuiInputEventType_Focus
    };
    bool                            AddedByTestEngine;

    HvkGuiInputEvent() { memset(this, 0, sizeof(*this)); }
};

// Input function taking an 'HvkGuiID owner_id' argument defaults to (HvkGuiKeyOwner_Any == 0) aka don't test ownership, which matches legacy behavior.
#define HvkGuiKeyOwner_Any           ((HvkGuiID)0)    // Accept key that have an owner, UNLESS a call to SetKeyOwner() explicitly used HvkGuiInputFlags_LockThisFrame or HvkGuiInputFlags_LockUntilRelease.
#define HvkGuiKeyOwner_NoOwner       ((HvkGuiID)-1)   // Require key to have no owner.
//#define HvkGuiKeyOwner_None HvkGuiKeyOwner_NoOwner  // We previously called this 'HvkGuiKeyOwner_None' but it was inconsistent with our pattern that _None values == 0 and quite dangerous. Also using _NoOwner makes the IsKeyPressed() calls more explicit.

typedef HvkS16 HvkGuiKeyRoutingIndex;

// Routing table entry (sizeof() == 16 bytes)
struct HvkGuiKeyRoutingData
{
    HvkGuiKeyRoutingIndex            NextEntryIndex;
    HvkU16                           Mods;               // Technically we'd only need 4-bits but for simplify we store HvkGuiMod_ values which need 16-bits.
    HvkU16                           RoutingCurrScore;   // [DEBUG] For debug display
    HvkU16                           RoutingNextScore;   // Lower is better (0: perfect score)
    HvkGuiID                         RoutingCurr;
    HvkGuiID                         RoutingNext;

    HvkGuiKeyRoutingData()           { NextEntryIndex = -1; Mods = 0; RoutingCurrScore = RoutingNextScore = 0; RoutingCurr = RoutingNext = HvkGuiKeyOwner_NoOwner; }
};

// Routing table: maintain a desired owner for each possible key-chord (key + mods), and setup owner in NewFrame() when mods are matching.
// Stored in main context (1 instance)
struct HvkGuiKeyRoutingTable
{
    HvkGuiKeyRoutingIndex            Index[HvkGuiKey_NamedKey_COUNT]; // Index of first entry in Entries[]
    HvkVector<HvkGuiKeyRoutingData>   Entries;
    HvkVector<HvkGuiKeyRoutingData>   EntriesNext;                    // Double-buffer to avoid reallocation (could use a shared buffer)

    HvkGuiKeyRoutingTable()          { Clear(); }
    void Clear()                    { for (int n = 0; n < Hvk_ARRAYSIZE(Index); n++) Index[n] = -1; Entries.clear(); EntriesNext.clear(); }
};

// This extends HvkGuiKeyData but only for named keys (legacy keys don't support the new features)
// Stored in main context (1 per named key). In the future it might be merged into HvkGuiKeyData.
struct HvkGuiKeyOwnerData
{
    HvkGuiID     OwnerCurr;
    HvkGuiID     OwnerNext;
    bool        LockThisFrame;      // Reading this key requires explicit owner id (until end of frame). Set by HvkGuiInputFlags_LockThisFrame.
    bool        LockUntilRelease;   // Reading this key requires explicit owner id (until key is released). Set by HvkGuiInputFlags_LockUntilRelease. When this is true LockThisFrame is always true as well.

    HvkGuiKeyOwnerData()             { OwnerCurr = OwnerNext = HvkGuiKeyOwner_NoOwner; LockThisFrame = LockUntilRelease = false; }
};

// Extend HvkGuiInputFlags_
// Flags for extended versions of IsKeyPressed(), IsMouseClicked(), Shortcut(), SetKeyOwner(), SetItemKeyOwner()
// Don't mistake with HvkGuiInputTextFlags! (which is for HvkGui::InputText() function)
enum HvkGuiInputFlagsPrivate_
{
    // Flags for IsKeyPressed(), IsKeyChordPressed(), IsMouseClicked(), Shortcut()
    // - Repeat mode: Repeat rate selection
    HvkGuiInputFlags_RepeatRateDefault           = 1 << 1,   // Repeat rate: Regular (default)
    HvkGuiInputFlags_RepeatRateNavMove           = 1 << 2,   // Repeat rate: Fast
    HvkGuiInputFlags_RepeatRateNavTweak          = 1 << 3,   // Repeat rate: Faster
    // - Repeat mode: Specify when repeating key pressed can be interrupted.
    // - In theory HvkGuiInputFlags_RepeatUntilOtherKeyPress may be a desirable default, but it would break too many behavior so everything is opt-in.
    HvkGuiInputFlags_RepeatUntilRelease          = 1 << 4,   // Stop repeating when released (default for all functions except Shortcut). This only exists to allow overriding Shortcut() default behavior.
    HvkGuiInputFlags_RepeatUntilKeyModsChange    = 1 << 5,   // Stop repeating when released OR if keyboard mods are changed (default for Shortcut)
    HvkGuiInputFlags_RepeatUntilKeyModsChangeFromNone = 1 << 6,  // Stop repeating when released OR if keyboard mods are leaving the None state. Allows going from Mod+Key to Key by releasing Mod.
    HvkGuiInputFlags_RepeatUntilOtherKeyPress    = 1 << 7,   // Stop repeating when released OR if any other keyboard key is pressed during the repeat

    // Flags for SetKeyOwner(), SetItemKeyOwner()
    // - Locking key away from non-input aware code. Locking is useful to make input-owner-aware code steal keys from non-input-owner-aware code. If all code is input-owner-aware locking would never be necessary.
    HvkGuiInputFlags_LockThisFrame               = 1 << 20,  // Further accesses to key data will require EXPLICIT owner ID (HvkGuiKeyOwner_Any/0 will NOT accepted for polling). Cleared at end of frame.
    HvkGuiInputFlags_LockUntilRelease            = 1 << 21,  // Further accesses to key data will require EXPLICIT owner ID (HvkGuiKeyOwner_Any/0 will NOT accepted for polling). Cleared when the key is released or at end of each frame if key is released.

    // - Condition for SetItemKeyOwner()
    HvkGuiInputFlags_CondHovered                 = 1 << 22,  // Only set if item is hovered (default to both)
    HvkGuiInputFlags_CondActive                  = 1 << 23,  // Only set if item is active (default to both)
    HvkGuiInputFlags_CondDefault_                = HvkGuiInputFlags_CondHovered | HvkGuiInputFlags_CondActive,

    // [Internal] Mask of which function support which flags
    HvkGuiInputFlags_RepeatRateMask_             = HvkGuiInputFlags_RepeatRateDefault | HvkGuiInputFlags_RepeatRateNavMove | HvkGuiInputFlags_RepeatRateNavTweak,
    HvkGuiInputFlags_RepeatUntilMask_            = HvkGuiInputFlags_RepeatUntilRelease | HvkGuiInputFlags_RepeatUntilKeyModsChange | HvkGuiInputFlags_RepeatUntilKeyModsChangeFromNone | HvkGuiInputFlags_RepeatUntilOtherKeyPress,
    HvkGuiInputFlags_RepeatMask_                 = HvkGuiInputFlags_Repeat | HvkGuiInputFlags_RepeatRateMask_ | HvkGuiInputFlags_RepeatUntilMask_,
    HvkGuiInputFlags_CondMask_                   = HvkGuiInputFlags_CondHovered | HvkGuiInputFlags_CondActive,
    HvkGuiInputFlags_RouteTypeMask_              = HvkGuiInputFlags_RouteActive | HvkGuiInputFlags_RouteFocused | HvkGuiInputFlags_RouteGlobal | HvkGuiInputFlags_RouteAlways,
    HvkGuiInputFlags_RouteOptionsMask_           = HvkGuiInputFlags_RouteOverFocused | HvkGuiInputFlags_RouteOverActive | HvkGuiInputFlags_RouteUnlessBgFocused | HvkGuiInputFlags_RouteFromRootWindow,
    HvkGuiInputFlags_SupportedByIsKeyPressed     = HvkGuiInputFlags_RepeatMask_,
    HvkGuiInputFlags_SupportedByIsMouseClicked   = HvkGuiInputFlags_Repeat,
    HvkGuiInputFlags_SupportedByShortcut         = HvkGuiInputFlags_RepeatMask_ | HvkGuiInputFlags_RouteTypeMask_ | HvkGuiInputFlags_RouteOptionsMask_,
    HvkGuiInputFlags_SupportedBySetNextItemShortcut = HvkGuiInputFlags_RepeatMask_ | HvkGuiInputFlags_RouteTypeMask_ | HvkGuiInputFlags_RouteOptionsMask_ | HvkGuiInputFlags_Tooltip,
    HvkGuiInputFlags_SupportedBySetKeyOwner      = HvkGuiInputFlags_LockThisFrame | HvkGuiInputFlags_LockUntilRelease,
    HvkGuiInputFlags_SupportedBySetItemKeyOwner  = HvkGuiInputFlags_SupportedBySetKeyOwner | HvkGuiInputFlags_CondMask_,
};

//-----------------------------------------------------------------------------
// [SECTION] Clipper support
//-----------------------------------------------------------------------------

// Note that Max is exclusive, so perhaps should be using a Begin/End convention.
struct HvkGuiListClipperRange
{
    int     Min;
    int     Max;
    bool    PosToIndexConvert;      // Begin/End are absolute position (will be converted to indices later)
    HvkS8    PosToIndexOffsetMin;    // Add to Min after converting to indices
    HvkS8    PosToIndexOffsetMax;    // Add to Min after converting to indices

    static HvkGuiListClipperRange    FromIndices(int min, int max)                               { HvkGuiListClipperRange r = { min, max, false, 0, 0 }; return r; }
    static HvkGuiListClipperRange    FromPositions(float y1, float y2, int off_min, int off_max) { HvkGuiListClipperRange r = { (int)y1, (int)y2, true, (HvkS8)off_min, (HvkS8)off_max }; return r; }
};

// Temporary clipper data, buffers shared/reused between instances
struct HvkGuiListClipperData
{
    HvkGuiListClipper*               ListClipper;
    float                           LossynessOffset;
    int                             StepNo;
    int                             ItemsFrozen;
    HvkVector<HvkGuiListClipperRange> Ranges;

    HvkGuiListClipperData()          { memset(this, 0, sizeof(*this)); }
    void                            Reset(HvkGuiListClipper* clipper) { ListClipper = clipper; StepNo = ItemsFrozen = 0; Ranges.resize(0); }
};

//-----------------------------------------------------------------------------
// [SECTION] Navigation support
//-----------------------------------------------------------------------------

enum HvkGuiActivateFlags_
{
    HvkGuiActivateFlags_None                 = 0,
    HvkGuiActivateFlags_PreferInput          = 1 << 0,       // Favor activation that requires keyboard text input (e.g. for Slider/Drag). Default for Enter key.
    HvkGuiActivateFlags_PreferTweak          = 1 << 1,       // Favor activation for tweaking with arrows or gamepad (e.g. for Slider/Drag). Default for Space key and if keyboard is not used.
    HvkGuiActivateFlags_TryToPreserveState   = 1 << 2,       // Request widget to preserve state if it can (e.g. InputText will try to preserve cursor/selection)
    HvkGuiActivateFlags_FromTabbing          = 1 << 3,       // Activation requested by a tabbing request (HvkGuiNavMoveFlags_IsTabbing)
    HvkGuiActivateFlags_FromShortcut         = 1 << 4,       // Activation requested by an item shortcut via SetNextItemShortcut() function.
    HvkGuiActivateFlags_FromFocusApi         = 1 << 5,       // Activation requested by an api request (HvkGuiNavMoveFlags_FocusApi)
};

// Early work-in-progress API for ScrollToItem()
enum HvkGuiScrollFlags_
{
    HvkGuiScrollFlags_None                   = 0,
    HvkGuiScrollFlags_KeepVisibleEdgeX       = 1 << 0,       // If item is not visible: scroll as little as possible on X axis to bring item back into view [default for X axis]
    HvkGuiScrollFlags_KeepVisibleEdgeY       = 1 << 1,       // If item is not visible: scroll as little as possible on Y axis to bring item back into view [default for Y axis for windows that are already visible]
    HvkGuiScrollFlags_KeepVisibleCenterX     = 1 << 2,       // If item is not visible: scroll to make the item centered on X axis [rarely used]
    HvkGuiScrollFlags_KeepVisibleCenterY     = 1 << 3,       // If item is not visible: scroll to make the item centered on Y axis
    HvkGuiScrollFlags_AlwaysCenterX          = 1 << 4,       // Always center the result item on X axis [rarely used]
    HvkGuiScrollFlags_AlwaysCenterY          = 1 << 5,       // Always center the result item on Y axis [default for Y axis for appearing window)
    HvkGuiScrollFlags_NoScrollParent         = 1 << 6,       // Disable forwarding scrolling to parent window if required to keep item/rect visible (only scroll window the function was applied to).
    HvkGuiScrollFlags_MaskX_                 = HvkGuiScrollFlags_KeepVisibleEdgeX | HvkGuiScrollFlags_KeepVisibleCenterX | HvkGuiScrollFlags_AlwaysCenterX,
    HvkGuiScrollFlags_MaskY_                 = HvkGuiScrollFlags_KeepVisibleEdgeY | HvkGuiScrollFlags_KeepVisibleCenterY | HvkGuiScrollFlags_AlwaysCenterY,
};

enum HvkGuiNavRenderCursorFlags_
{
    HvkGuiNavRenderCursorFlags_None          = 0,
    HvkGuiNavRenderCursorFlags_Compact       = 1 << 1,       // Compact highlight, no padding/distance from focused item
    HvkGuiNavRenderCursorFlags_AlwaysDraw    = 1 << 2,       // Draw rectangular highlight if (g.NavId == id) even when g.NavCursorVisible == false, aka even when using the mouse.
    HvkGuiNavRenderCursorFlags_NoRounding    = 1 << 3,
#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    HvkGuiNavHighlightFlags_None             = HvkGuiNavRenderCursorFlags_None,       // Renamed in 1.91.4
    HvkGuiNavHighlightFlags_Compact          = HvkGuiNavRenderCursorFlags_Compact,    // Renamed in 1.91.4
    HvkGuiNavHighlightFlags_AlwaysDraw       = HvkGuiNavRenderCursorFlags_AlwaysDraw, // Renamed in 1.91.4
    HvkGuiNavHighlightFlags_NoRounding       = HvkGuiNavRenderCursorFlags_NoRounding, // Renamed in 1.91.4
    //HvkGuiNavHighlightFlags_TypeThin       = HvkGuiNavRenderCursorFlags_Compact,    // Renamed in 1.90.2
#endif
};

enum HvkGuiNavMoveFlags_
{
    HvkGuiNavMoveFlags_None                  = 0,
    HvkGuiNavMoveFlags_LoopX                 = 1 << 0,   // On failed request, restart from opposite side
    HvkGuiNavMoveFlags_LoopY                 = 1 << 1,
    HvkGuiNavMoveFlags_WrapX                 = 1 << 2,   // On failed request, request from opposite side one line down (when NavDir==right) or one line up (when NavDir==left)
    HvkGuiNavMoveFlags_WrapY                 = 1 << 3,   // This is not super useful but provided for completeness
    HvkGuiNavMoveFlags_WrapMask_             = HvkGuiNavMoveFlags_LoopX | HvkGuiNavMoveFlags_LoopY | HvkGuiNavMoveFlags_WrapX | HvkGuiNavMoveFlags_WrapY,
    HvkGuiNavMoveFlags_AllowCurrentNavId     = 1 << 4,   // Allow scoring and considering the current NavId as a move target candidate. This is used when the move source is offset (e.g. pressing PageDown actually needs to send a Up move request, if we are pressing PageDown from the bottom-most item we need to stay in place)
    HvkGuiNavMoveFlags_AlsoScoreVisibleSet   = 1 << 5,   // Store alternate result in NavMoveResultLocalVisible that only comprise elements that are already fully visible (used by PageUp/PageDown)
    HvkGuiNavMoveFlags_ScrollToEdgeY         = 1 << 6,   // Force scrolling to min/max (used by Home/End) // FIXME-NAV: Aim to remove or reword as HvkGuiScrollFlags
    HvkGuiNavMoveFlags_Forwarded             = 1 << 7,
    HvkGuiNavMoveFlags_DebugNoResult         = 1 << 8,   // Dummy scoring for debug purpose, don't apply result
    HvkGuiNavMoveFlags_FocusApi              = 1 << 9,   // Requests from focus API can land/focus/activate items even if they are marked with _NoTabStop (see NavProcessItemForTabbingRequest() for details)
    HvkGuiNavMoveFlags_IsTabbing             = 1 << 10,  // == Focus + Activate if item is Inputable + DontChangeNavHighlight
    HvkGuiNavMoveFlags_IsPageMove            = 1 << 11,  // Identify a PageDown/PageUp request.
    HvkGuiNavMoveFlags_Activate              = 1 << 12,  // Activate/select target item.
    HvkGuiNavMoveFlags_NoSelect              = 1 << 13,  // Don't trigger selection by not setting g.NavJustMovedTo
    HvkGuiNavMoveFlags_NoSetNavCursorVisible = 1 << 14,  // Do not alter the nav cursor visible state
    HvkGuiNavMoveFlags_NoClearActiveId       = 1 << 15,  // (Experimental) Do not clear active id when applying move result
};

enum HvkGuiNavLayer
{
    HvkGuiNavLayer_Main  = 0,    // Main scrolling layer
    HvkGuiNavLayer_Menu  = 1,    // Menu layer (access with Alt)
    HvkGuiNavLayer_COUNT
};

// Storage for navigation query/results
struct HvkGuiNavItemData
{
    HvkGuiWindow*        Window;         // Init,Move    // Best candidate window (result->ItemWindow->RootWindowForNav == request->Window)
    HvkGuiID             ID;             // Init,Move    // Best candidate item ID
    HvkGuiID             FocusScopeId;   // Init,Move    // Best candidate focus scope ID
    HvkRect              RectRel;        // Init,Move    // Best candidate bounding box in window relative space
    HvkGuiItemFlags      ItemFlags;      // ????,Move    // Best candidate item flags
    float               DistBox;        //      Move    // Best candidate box distance to current NavId
    float               DistCenter;     //      Move    // Best candidate center distance to current NavId
    float               DistAxial;      //      Move    // Best candidate axial distance to current NavId
    HvkGuiSelectionUserData SelectionUserData;//I+Mov    // Best candidate SetNextItemSelectionUserData() value. Valid if (ItemFlags & HvkGuiItemFlags_HasSelectionUserData)

    HvkGuiNavItemData()  { Clear(); }
    void Clear()        { Window = NULL; ID = FocusScopeId = 0; ItemFlags = 0; SelectionUserData = -1; DistBox = DistCenter = DistAxial = FLT_MAX; }
};

// Storage for PushFocusScope(), g.FocusScopeStack[], g.NavFocusRoute[]
struct HvkGuiFocusScopeData
{
    HvkGuiID             ID;
    HvkGuiID             WindowID;
};

//-----------------------------------------------------------------------------
// [SECTION] Typing-select support
//-----------------------------------------------------------------------------

// Flags for GetTypingSelectRequest()
enum HvkGuiTypingSelectFlags_
{
    HvkGuiTypingSelectFlags_None                 = 0,
    HvkGuiTypingSelectFlags_AllowBackspace       = 1 << 0,   // Backspace to delete character inputs. If using: ensure GetTypingSelectRequest() is not called more than once per frame (filter by e.g. focus state)
    HvkGuiTypingSelectFlags_AllowSingleCharMode  = 1 << 1,   // Allow "single char" search mode which is activated when pressing the same character multiple times.
};

// Returned by GetTypingSelectRequest(), designed to eventually be public.
struct HvkGui_API HvkGuiTypingSelectRequest
{
    HvkGuiTypingSelectFlags  Flags;              // Flags passed to GetTypingSelectRequest()
    int                     SearchBufferLen;
    const char*             SearchBuffer;       // Search buffer contents (use full string. unless SingleCharMode is set, in which case use SingleCharSize).
    bool                    SelectRequest;      // Set when buffer was modified this frame, requesting a selection.
    bool                    SingleCharMode;     // Notify when buffer contains same character repeated, to Hvkplement special mode. In this situation it preferred to not display any on-screen search indication.
    HvkS8                    SingleCharSize;     // Length in bytes of first letter codepoint (1 for ascii, 2-4 for UTF-8). If (SearchBufferLen==RepeatCharSize) only 1 letter has been input.
};

// Storage for GetTypingSelectRequest()
struct HvkGui_API HvkGuiTypingSelectState
{
    HvkGuiTypingSelectRequest Request;           // User-facing data
    char            SearchBuffer[64];           // Search buffer: no need to make dynamic as this search is very transient.
    HvkGuiID         FocusScope;
    int             LastRequestFrame = 0;
    float           LastRequestTime = 0.0f;
    bool            SingleCharModeLock = false; // After a certain single char repeat count we lock into SingleCharMode. Two benefits: 1) buffer never fill, 2) we can provide an Hvkmediate SingleChar mode without timer elapsing.

    HvkGuiTypingSelectState() { memset(this, 0, sizeof(*this)); }
    void            Clear()  { SearchBuffer[0] = 0; SingleCharModeLock = false; } // We preserve remaining data for easier debugging
};

//-----------------------------------------------------------------------------
// [SECTION] Columns support
//-----------------------------------------------------------------------------

// Flags for internal's BeginColumns(). This is an obsolete API. Prefer using BeginTable() nowadays!
enum HvkGuiOldColumnFlags_
{
    HvkGuiOldColumnFlags_None                    = 0,
    HvkGuiOldColumnFlags_NoBorder                = 1 << 0,   // Disable column dividers
    HvkGuiOldColumnFlags_NoResize                = 1 << 1,   // Disable resizing columns when clicking on the dividers
    HvkGuiOldColumnFlags_NoPreserveWidths        = 1 << 2,   // Disable column width preservation when adjusting columns
    HvkGuiOldColumnFlags_NoForceWithinWindow     = 1 << 3,   // Disable forcing columns to fit within window
    HvkGuiOldColumnFlags_GrowParentContentsSize  = 1 << 4,   // Restore pre-1.51 behavior of extending the parent window contents size but _without affecting the columns width at all_. Will eventually remove.

    // Obsolete names (will be removed)
#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    //HvkGuiColumnsFlags_None                    = HvkGuiOldColumnFlags_None,
    //HvkGuiColumnsFlags_NoBorder                = HvkGuiOldColumnFlags_NoBorder,
    //HvkGuiColumnsFlags_NoResize                = HvkGuiOldColumnFlags_NoResize,
    //HvkGuiColumnsFlags_NoPreserveWidths        = HvkGuiOldColumnFlags_NoPreserveWidths,
    //HvkGuiColumnsFlags_NoForceWithinWindow     = HvkGuiOldColumnFlags_NoForceWithinWindow,
    //HvkGuiColumnsFlags_GrowParentContentsSize  = HvkGuiOldColumnFlags_GrowParentContentsSize,
#endif
};

struct HvkGuiOldColumnData
{
    float               OffsetNorm;             // Column start offset, normalized 0.0 (far left) -> 1.0 (far right)
    float               OffsetNormBeforeResize;
    HvkGuiOldColumnFlags Flags;                  // Not exposed
    HvkRect              ClipRect;

    HvkGuiOldColumnData() { memset(this, 0, sizeof(*this)); }
};

struct HvkGuiOldColumns
{
    HvkGuiID             ID;
    HvkGuiOldColumnFlags Flags;
    bool                IsFirstFrame;
    bool                IsBeingResized;
    int                 Current;
    int                 Count;
    float               OffMinX, OffMaxX;       // Offsets from HostWorkRect.Min.x
    float               LineMinY, LineMaxY;
    float               HostCursorPosY;         // Backup of CursorPos at the time of BeginColumns()
    float               HostCursorMaxPosX;      // Backup of CursorMaxPos at the time of BeginColumns()
    HvkRect              HostInitialClipRect;    // Backup of ClipRect at the time of BeginColumns()
    HvkRect              HostBackupClipRect;     // Backup of ClipRect during PushColumnsBackground()/PopColumnsBackground()
    HvkRect              HostBackupParentWorkRect;//Backup of WorkRect at the time of BeginColumns()
    HvkVector<HvkGuiOldColumnData> Columns;
    HvkDrawListSplitter  Splitter;

    HvkGuiOldColumns()   { memset(this, 0, sizeof(*this)); }
};

//-----------------------------------------------------------------------------
// [SECTION] Box-select support
//-----------------------------------------------------------------------------

struct HvkGuiBoxSelectState
{
    // Active box-selection data (persistent, 1 active at a time)
    HvkGuiID                 ID;
    bool                    IsActive;
    bool                    IsStarting;
    bool                    IsStartedFromVoid;  // Starting click was not from an item.
    bool                    IsStartedSetNavIdOnce;
    bool                    RequestClear;
    HvkGuiKeyChord           KeyMods : 16;       // Latched key-mods for box-select logic.
    HvkVec2                  StartPosRel;        // Start position in window-contents relative space (to support scrolling)
    HvkVec2                  EndPosRel;          // End position in window-contents relative space
    HvkVec2                  ScrollAccum;        // Scrolling accumulator (to behave at high-frame spaces)
    HvkGuiWindow*            Window;

    // Temporary/Transient data
    bool                    UnclipMode;         // (Temp/Transient, here in hot area). Set/cleared by the BeginMultiSelect()/EndMultiSelect() owning active box-select.
    HvkRect                  UnclipRect;         // Rectangle where ItemAdd() clipping may be temporarily disabled. Need support by multi-select supporting widgets.
    HvkRect                  BoxSelectRectPrev;  // Selection rectangle in absolute coordinates (derived every frame from BoxSelectStartPosRel and MousePos)
    HvkRect                  BoxSelectRectCurr;

    HvkGuiBoxSelectState()   { memset(this, 0, sizeof(*this)); }
};

//-----------------------------------------------------------------------------
// [SECTION] Multi-select support
//-----------------------------------------------------------------------------

// We always assume that -1 is an invalid value (which works for indices and pointers)
#define HvkGuiSelectionUserData_Invalid        ((HvkGuiSelectionUserData)-1)

// Temporary storage for multi-select
struct HvkGui_API HvkGuiMultiSelectTempData
{
    HvkGuiMultiSelectIO      IO;                 // MUST BE FIRST FIELD. Requests are set and returned by BeginMultiSelect()/EndMultiSelect() + written to by user during the loop.
    HvkGuiMultiSelectState*  Storage;
    HvkGuiID                 FocusScopeId;       // Copied from g.CurrentFocusScopeId (unless another selection scope was pushed manually)
    HvkGuiMultiSelectFlags   Flags;
    HvkVec2                  ScopeRectMin;
    HvkVec2                  BackupCursorMaxPos;
    HvkGuiSelectionUserData  LastSubmittedItem;  // Copy of last submitted item data, used to merge output ranges.
    HvkGuiID                 BoxSelectId;
    HvkGuiKeyChord           KeyMods;
    HvkS8                    LoopRequestSetAll;  // -1: no operation, 0: clear all, 1: select all.
    bool                    IsEndIO;            // Set when switching IO from BeginMultiSelect() to EndMultiSelect() state.
    bool                    IsFocused;          // Set if currently focusing the selection scope (any item of the selection). May be used if you have custom shortcut associated to selection.
    bool                    IsKeyboardSetRange; // Set by BeginMultiSelect() when using Shift+Navigation. Because scrolling may be affected we can't afford a frame of lag with Shift+Navigation.
    bool                    NavIdPassedBy;
    bool                    RangeSrcPassedBy;   // Set by the item that matches RangeSrcItem.
    bool                    RangeDstPassedBy;   // Set by the item that matches NavJustMovedToId when IsSetRange is set.

    HvkGuiMultiSelectTempData()  { Clear(); }
    void Clear()            { size_t io_sz = sizeof(IO); ClearIO(); memset((void*)(&IO + 1), 0, sizeof(*this) - io_sz); } // Zero-clear except IO as we preserve IO.Requests[] buffer allocation.
    void ClearIO()          { IO.Requests.resize(0); IO.RangeSrcItem = IO.NavIdItem = HvkGuiSelectionUserData_Invalid; IO.NavIdSelected = IO.RangeSrcReset = false; }
};

// Persistent storage for multi-select (as long as selection is alive)
struct HvkGui_API HvkGuiMultiSelectState
{
    HvkGuiWindow*            Window;
    HvkGuiID                 ID;
    int                     LastFrameActive;    // Last used frame-count, for GC.
    int                     LastSelectionSize;  // Set by BeginMultiSelect() based on optional info provided by user. May be -1 if unknown.
    HvkS8                    RangeSelected;      // -1 (don't have) or true/false
    HvkS8                    NavIdSelected;      // -1 (don't have) or true/false
    HvkGuiSelectionUserData  RangeSrcItem;       //
    HvkGuiSelectionUserData  NavIdItem;          // SetNextItemSelectionUserData() value for NavId (if part of submitted items)

    HvkGuiMultiSelectState() { Window = NULL; ID = 0; LastFrameActive = LastSelectionSize = 0; RangeSelected = NavIdSelected = -1; RangeSrcItem = NavIdItem = HvkGuiSelectionUserData_Invalid; }
};

//-----------------------------------------------------------------------------
// [SECTION] Docking support
//-----------------------------------------------------------------------------

#ifdef HvkGui_HAS_DOCK
// <this is filled in 'docking' branch>
#endif // #ifdef HvkGui_HAS_DOCK

//-----------------------------------------------------------------------------
// [SECTION] Viewport support
//-----------------------------------------------------------------------------

// HvkGuiViewport Private/Internals fields (cardinal sin: we are using inheritance!)
// Every instance of HvkGuiViewport is in fact a HvkGuiViewportP.
struct HvkGuiViewportP : public HvkGuiViewport
{
    int                 BgFgDrawListsLastFrame[2]; // Last frame number the background (0) and foreground (1) draw lists were used
    HvkDrawList*         BgFgDrawLists[2];       // Convenience background (0) and foreground (1) draw lists. We use them to draw software mouser cursor when io.MouseDrawCursor is set and to draw most debug overlays.
    HvkDrawData          DrawDataP;
    HvkDrawDataBuilder   DrawDataBuilder;        // Temporary data while building final HvkDrawData

    // Per-viewport work area
    // - Insets are >= 0.0f values, distance from viewport corners to work area.
    // - BeginMainMenuBar() and DockspaceOverViewport() tend to use work area to avoid stepping over existing contents.
    // - Generally 'safeAreaInsets' in iOS land, 'DisplayCutout' in Android land.
    HvkVec2              WorkInsetMin;           // Work Area inset locked for the frame. GetWorkRect() always fits within GetMainRect().
    HvkVec2              WorkInsetMax;           // "
    HvkVec2              BuildWorkInsetMin;      // Work Area inset accumulator for current frame, to become next frame's WorkInset
    HvkVec2              BuildWorkInsetMax;      // "

    HvkGuiViewportP()    { BgFgDrawListsLastFrame[0] = BgFgDrawListsLastFrame[1] = -1; BgFgDrawLists[0] = BgFgDrawLists[1] = NULL; }
    ~HvkGuiViewportP()   { if (BgFgDrawLists[0]) Hvk_DELETE(BgFgDrawLists[0]); if (BgFgDrawLists[1]) Hvk_DELETE(BgFgDrawLists[1]); }

    // Calculate work rect pos/size given a set of offset (we have 1 pair of offset for rect locked from last frame data, and 1 pair for currently building rect)
    HvkVec2  CalcWorkRectPos(const HvkVec2& inset_min) const                           { return HvkVec2(Pos.x + inset_min.x, Pos.y + inset_min.y); }
    HvkVec2  CalcWorkRectSize(const HvkVec2& inset_min, const HvkVec2& inset_max) const { return HvkVec2(HvkMax(0.0f, Size.x - inset_min.x - inset_max.x), HvkMax(0.0f, Size.y - inset_min.y - inset_max.y)); }
    void    UpdateWorkRect()            { WorkPos = CalcWorkRectPos(WorkInsetMin); WorkSize = CalcWorkRectSize(WorkInsetMin, WorkInsetMax); } // Update public fields

    // Helpers to retrieve HvkRect (we don't need to store BuildWorkRect as every access tend to change it, hence the code asymmetry)
    HvkRect  GetMainRect() const         { return HvkRect(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y); }
    HvkRect  GetWorkRect() const         { return HvkRect(WorkPos.x, WorkPos.y, WorkPos.x + WorkSize.x, WorkPos.y + WorkSize.y); }
    HvkRect  GetBuildWorkRect() const    { HvkVec2 pos = CalcWorkRectPos(BuildWorkInsetMin); HvkVec2 size = CalcWorkRectSize(BuildWorkInsetMin, BuildWorkInsetMax); return HvkRect(pos.x, pos.y, pos.x + size.x, pos.y + size.y); }
};

//-----------------------------------------------------------------------------
// [SECTION] Settings support
//-----------------------------------------------------------------------------

// Windows data saved in HvkGui.ini file
// Because we never destroy or rename HvkGuiWindowSettings, we can store the names in a separate buffer easily.
// (this is designed to be stored in a HvkChunkStream buffer, with the variable-length Name following our structure)
struct HvkGuiWindowSettings
{
    HvkGuiID     ID;
    HvkVec2ih    Pos;
    HvkVec2ih    Size;
    bool        Collapsed;
    bool        IsChild;
    bool        WantApply;      // Set when loaded from .ini data (to enable merging/loading .ini data into an already running context)
    bool        WantDelete;     // Set to invalidate/delete the settings entry

    HvkGuiWindowSettings()       { memset(this, 0, sizeof(*this)); }
    char* GetName()             { return (char*)(this + 1); }
};

struct HvkGuiSettingsHandler
{
    const char* TypeName;       // Short description stored in .ini file. Disallowed characters: '[' ']'
    HvkGuiID     TypeHash;       // == HvkHashStr(TypeName)
    void        (*ClearAllFn)(HvkGuiContext* ctx, HvkGuiSettingsHandler* handler);                                // Clear all settings data
    void        (*ReadInitFn)(HvkGuiContext* ctx, HvkGuiSettingsHandler* handler);                                // Read: Called before reading (in registration order)
    void*       (*ReadOpenFn)(HvkGuiContext* ctx, HvkGuiSettingsHandler* handler, const char* name);              // Read: Called when entering into a new ini entry e.g. "[Window][Name]"
    void        (*ReadLineFn)(HvkGuiContext* ctx, HvkGuiSettingsHandler* handler, void* entry, const char* line); // Read: Called for every line of text within an ini entry
    void        (*ApplyAllFn)(HvkGuiContext* ctx, HvkGuiSettingsHandler* handler);                                // Read: Called after reading (in registration order)
    void        (*WriteAllFn)(HvkGuiContext* ctx, HvkGuiSettingsHandler* handler, HvkGuiTextBuffer* out_buf);      // Write: Output every entries into 'out_buf'
    void*       UserData;

    HvkGuiSettingsHandler() { memset(this, 0, sizeof(*this)); }
};

//-----------------------------------------------------------------------------
// [SECTION] Localization support
//-----------------------------------------------------------------------------

// This is experimental and not officially supported, it'll probably fall short of features, if/when it does we may backtrack.
enum HvkGuiLocKey : int
{
    HvkGuiLocKey_VersionStr,
    HvkGuiLocKey_TableSizeOne,
    HvkGuiLocKey_TableSizeAllFit,
    HvkGuiLocKey_TableSizeAllDefault,
    HvkGuiLocKey_TableResetOrder,
    HvkGuiLocKey_WindowingMainMenuBar,
    HvkGuiLocKey_WindowingPopup,
    HvkGuiLocKey_WindowingUntitled,
    HvkGuiLocKey_OpenLink_s,
    HvkGuiLocKey_CopyLink,
    HvkGuiLocKey_COUNT
};

struct HvkGuiLocEntry
{
    HvkGuiLocKey     Key;
    const char*     Text;
};

//-----------------------------------------------------------------------------
// [SECTION] Error handling, State recovery support
//-----------------------------------------------------------------------------

// Macros used by Recoverable Error handling
// - Only dispatch error if _EXPR: evaluate as assert (similar to an assert macro).
// - The message will always be a string literal, in order to increase likelihood of being display by an assert handler.
// - See 'Demo->Configuration->Error Handling' and HvkGuiIO definitions for details on error handling.
// - Read https://github.com/ocornut/HvkGui/wiki/Error-Handling for details on error handling.
#ifndef Hvk_ASSERT_USER_ERROR
#define Hvk_ASSERT_USER_ERROR(_EXPR,_MSG)    do { if (!(_EXPR) && HvkGui::ErrorLog(_MSG)) { Hvk_ASSERT((_EXPR) && _MSG); } } while (0)    // Recoverable User Error
#endif

// The error callback is currently not public, as it is expected that only advanced users will rely on it.
typedef void (*HvkGuiErrorCallback)(HvkGuiContext* ctx, void* user_data, const char* msg); // Function signature for g.ErrorCallback

//-----------------------------------------------------------------------------
// [SECTION] Metrics, Debug Tools
//-----------------------------------------------------------------------------

// See HvkGui_DEBUG_LOG() and HvkGui_DEBUG_LOG_XXX() macros.
enum HvkGuiDebugLogFlags_
{
    // Event types
    HvkGuiDebugLogFlags_None                 = 0,
    HvkGuiDebugLogFlags_EventError           = 1 << 0,   // Error submitted by Hvk_ASSERT_USER_ERROR()
    HvkGuiDebugLogFlags_EventActiveId        = 1 << 1,
    HvkGuiDebugLogFlags_EventFocus           = 1 << 2,
    HvkGuiDebugLogFlags_EventPopup           = 1 << 3,
    HvkGuiDebugLogFlags_EventNav             = 1 << 4,
    HvkGuiDebugLogFlags_EventClipper         = 1 << 5,
    HvkGuiDebugLogFlags_EventSelection       = 1 << 6,
    HvkGuiDebugLogFlags_EventIO              = 1 << 7,
    HvkGuiDebugLogFlags_EventFont            = 1 << 8,
    HvkGuiDebugLogFlags_EventInputRouting    = 1 << 9,
    HvkGuiDebugLogFlags_EventDocking         = 1 << 10,  // Unused in this branch
    HvkGuiDebugLogFlags_EventViewport        = 1 << 11,  // Unused in this branch

    HvkGuiDebugLogFlags_EventMask_           = HvkGuiDebugLogFlags_EventError | HvkGuiDebugLogFlags_EventActiveId | HvkGuiDebugLogFlags_EventFocus | HvkGuiDebugLogFlags_EventPopup | HvkGuiDebugLogFlags_EventNav | HvkGuiDebugLogFlags_EventClipper | HvkGuiDebugLogFlags_EventSelection | HvkGuiDebugLogFlags_EventIO | HvkGuiDebugLogFlags_EventFont | HvkGuiDebugLogFlags_EventInputRouting | HvkGuiDebugLogFlags_EventDocking | HvkGuiDebugLogFlags_EventViewport,
    HvkGuiDebugLogFlags_OutputToTTY          = 1 << 20,  // Also send output to TTY
    HvkGuiDebugLogFlags_OutputToDebugger     = 1 << 21,  // Also send output to Debugger Console [Windows only]
    HvkGuiDebugLogFlags_OutputToTestEngine   = 1 << 22,  // Also send output to Dear HvkGui Test Engine
};

struct HvkGuiDebugAllocEntry
{
    int         FrameCount;
    HvkS16       AllocCount;
    HvkS16       FreeCount;
};

struct HvkGuiDebugAllocInfo
{
    int         TotalAllocCount;            // Number of call to MemAlloc().
    int         TotalFreeCount;
    HvkS16       LastEntriesIdx;             // Current index in buffer
    HvkGuiDebugAllocEntry LastEntriesBuf[6]; // Track last 6 frames that had allocations

    HvkGuiDebugAllocInfo() { memset(this, 0, sizeof(*this)); }
};

struct HvkGuiMetricsConfig
{
    bool        ShowDebugLog = false;
    bool        ShowIDStackTool = false;
    bool        ShowWindowsRects = false;
    bool        ShowWindowsBeginOrder = false;
    bool        ShowTablesRects = false;
    bool        ShowDrawCmdMesh = true;
    bool        ShowDrawCmdBoundingBoxes = true;
    bool        ShowTextEncodingViewer = false;
    bool        ShowTextureUsedRect = false;
    int         ShowWindowsRectsType = -1;
    int         ShowTablesRectsType = -1;
    int         HighlightMonitorIdx = -1;
    HvkGuiID     HighlightViewportID = 0;
    bool        ShowFontPreview = true;
};

struct HvkGuiStackLevelInfo
{
    HvkGuiID                 ID;
    HvkS8                    QueryFrameCount;            // >= 1: Sub-query in progress
    bool                    QuerySuccess;               // Sub-query obtained result from DebugHookIdInfo()
    HvkS8                    DataType;                   // HvkGuiDataType
    int                     DescOffset;                 // -1 or offset into parent's ResultsPathsBuf

    HvkGuiStackLevelInfo()   { memset(this, 0, sizeof(*this)); DataType = -1; DescOffset = -1; }
};

struct HvkGuiDebugItemPathQuery
{
    HvkGuiID                 MainID;                     // ID to query details for.
    bool                    Active;                     // Used to disambiguate the case when ID == 0 and e.g. some code calls PushOverrideID(0).
    bool                    Complete;                   // All sub-queries are finished (some may have failed).
    HvkS8                    Step;                       // -1: query stack + init Results, >= 0: filling individual stack level.
    HvkVector<HvkGuiStackLevelInfo> Results;
    HvkGuiTextBuffer         ResultsDescBuf;
    HvkGuiTextBuffer         ResultPathBuf;

    HvkGuiDebugItemPathQuery() { memset(this, 0, sizeof(*this)); }
};

// State for ID Stack tool queries
struct HvkGuiIDStackTool
{
    bool                    OptHexEncodeNonAsciiChars;
    bool                    OptCopyToClipboardOnCtrlC;
    int                     LastActiveFrame;
    float                   CopyToClipboardLastTime;

    HvkGuiIDStackTool()      { memset(this, 0, sizeof(*this)); LastActiveFrame = -1; OptHexEncodeNonAsciiChars = true; CopyToClipboardLastTime = -FLT_MAX; }
};

//-----------------------------------------------------------------------------
// [SECTION] Generic context hooks
//-----------------------------------------------------------------------------

typedef void (*HvkGuiContextHookCallback)(HvkGuiContext* ctx, HvkGuiContextHook* hook);
enum HvkGuiContextHookType { HvkGuiContextHookType_NewFramePre, HvkGuiContextHookType_NewFramePost, HvkGuiContextHookType_EndFramePre, HvkGuiContextHookType_EndFramePost, HvkGuiContextHookType_RenderPre, HvkGuiContextHookType_RenderPost, HvkGuiContextHookType_Shutdown, HvkGuiContextHookType_PendingRemoval_ };

struct HvkGuiContextHook
{
    HvkGuiID                     HookId;     // A unique ID assigned by AddContextHook()
    HvkGuiContextHookType        Type;
    HvkGuiID                     Owner;
    HvkGuiContextHookCallback    Callback;
    void*                       UserData;

    HvkGuiContextHook()          { memset(this, 0, sizeof(*this)); }
};

//-----------------------------------------------------------------------------
// [SECTION] HvkGuiContext (main Dear HvkGui context)
//-----------------------------------------------------------------------------

struct HvkGuiContext
{
    bool                    Initialized;
    bool                    WithinFrameScope;                   // Set by NewFrame(), cleared by EndFrame()
    bool                    WithinFrameScopeWithImplicitWindow; // Set by NewFrame(), cleared by EndFrame() when the Hvkplicit debug window has been pushed
    bool                    TestEngineHookItems;                // Will call test engine hooks: HvkGuiTestEngineHook_ItemAdd(), HvkGuiTestEngineHook_ItemInfo(), HvkGuiTestEngineHook_Log()
    int                     FrameCount;
    int                     FrameCountEnded;
    int                     FrameCountRendered;
    double                  Time;
    char                    ContextName[16];                    // Storage for a context name (to facilitate debugging multi-context setups)
    HvkGuiIO                 IO;
    HvkGuiPlatformIO         PlatformIO;
    HvkGuiStyle              Style;
    HvkVector<HvkFontAtlas*>  FontAtlases;                        // List of font atlases used by the context (generally only contains g.IO.Fonts aka the main font atlas)
    HvkFont*                 Font;                               // Currently bound font. (== FontStack.back().Font)
    HvkFontBaked*            FontBaked;                          // Currently bound font at currently bound size. (== Font->GetFontBaked(FontSize))
    float                   FontSize;                           // Currently bound font size == line height (== FontSizeBase + externals scales applied in the UpdateCurrentFontSize() function).
    float                   FontSizeBase;                       // Font size before scaling == style.FontSizeBase == value passed to PushFont() when specified.
    float                   FontBakedScale;                     // == FontBaked->Size / FontSize. Scale factor over baked size. Rarely used nowadays, very often == 1.0f.
    float                   FontRasterizerDensity;              // Current font density. Used by all calls to GetFontBaked().
    float                   CurrentDpiScale;                    // Current window/viewport DpiScale == CurrentViewport->DpiScale
    HvkDrawListSharedData    DrawListSharedData;
    HvkGuiID                 WithinEndChildID;                   // Set within EndChild()
    void*                   TestEngine;                         // Test engine user data

    // Inputs
    HvkVector<HvkGuiInputEvent> InputEventsQueue;                 // Input events which will be trickled/written into IO structure.
    HvkVector<HvkGuiInputEvent> InputEventsTrail;                 // Past input events processed in NewFrame(). This is to allow domain-specific application to access e.g mouse/pen trail.
    HvkGuiMouseSource        InputEventsNextMouseSource;
    HvkU32                   InputEventsNextEventId;

    // Windows state
    HvkVector<HvkGuiWindow*>  Windows;                            // Windows, sorted in display order, back to front
    HvkVector<HvkGuiWindow*>  WindowsFocusOrder;                  // Root windows, sorted in focus order, back to front.
    HvkVector<HvkGuiWindow*>  WindowsTempSortBuffer;              // Temporary buffer used in EndFrame() to reorder windows so parents are kept before their child
    HvkVector<HvkGuiWindowStackData> CurrentWindowStack;
    HvkGuiStorage            WindowsById;                        // Map window's HvkGuiID to HvkGuiWindow*
    int                     WindowsActiveCount;                 // Number of unique windows submitted by frame
    float                   WindowsBorderHoverPadding;          // Padding around resizable windows for which hovering on counts as hovering the window == HvkMax(style.TouchExtraPadding, style.WindowBorderHoverPadding). This isn't so multi-dpi friendly.
    HvkGuiID                 DebugBreakInWindow;                 // Set to break in Begin() call.
    HvkGuiWindow*            CurrentWindow;                      // Window being drawn into
    HvkGuiWindow*            HoveredWindow;                      // Window the mouse is hovering. Will typically catch mouse inputs.
    HvkGuiWindow*            HoveredWindowUnderMovingWindow;     // Hovered window ignoring MovingWindow. Only set if MovingWindow is set.
    HvkGuiWindow*            HoveredWindowBeforeClear;           // Window the mouse is hovering. Filled even with _NoMouse. This is currently useful for multi-context compositors.
    HvkGuiWindow*            MovingWindow;                       // Track the window we clicked on (in order to preserve focus). The actual window that is moved is generally MovingWindow->RootWindow.
    HvkGuiWindow*            WheelingWindow;                     // Track the window we started mouse-wheeling on. Until a timer elapse or mouse has moved, generally keep scrolling the same window even if during the course of scrolling the mouse ends up hovering a child window.
    HvkVec2                  WheelingWindowRefMousePos;
    int                     WheelingWindowStartFrame;           // This may be set one frame before WheelingWindow is != NULL
    int                     WheelingWindowScrolledFrame;
    float                   WheelingWindowReleaseTimer;
    HvkVec2                  WheelingWindowWheelRemainder;
    HvkVec2                  WheelingAxisAvg;

    // Item/widgets state and tracking information
    HvkGuiID                 DebugDrawIdConflictsId;             // Set when we detect multiple items with the same identifier
    HvkGuiID                 DebugHookIdInfoId;                  // Will call core hooks: DebugHookIdInfo() from GetID functions, used by ID Stack Tool [next HoveredId/ActiveId to not pull in an extra cache-line]
    HvkGuiID                 HoveredId;                          // Hovered widget, filled during the frame
    HvkGuiID                 HoveredIdPreviousFrame;
    int                     HoveredIdPreviousFrameItemCount;    // Count numbers of items using the same ID as last frame's hovered id
    float                   HoveredIdTimer;                     // Measure contiguous hovering time
    float                   HoveredIdNotActiveTimer;            // Measure contiguous hovering time where the item has not been active
    bool                    HoveredIdAllowOverlap;
    bool                    HoveredIdIsDisabled;                // At least one widget passed the rect test, but has been discarded by disabled flag or popup inhibit. May be true even if HoveredId == 0.
    bool                    ItemUnclipByLog;                    // Disable ItemAdd() clipping, essentially a memory-locality friendly copy of LogEnabled
    HvkGuiID                 ActiveId;                           // Active widget
    HvkGuiID                 ActiveIdIsAlive;                    // Active widget has been seen this frame (we can't use a bool as the ActiveId may change within the frame)
    float                   ActiveIdTimer;
    bool                    ActiveIdIsJustActivated;            // Set at the time of activation for one frame
    bool                    ActiveIdAllowOverlap;               // Active widget allows another widget to steal active id (generally for overlapping widgets, but not always)
    bool                    ActiveIdNoClearOnFocusLoss;         // Disable losing active id if the active id window gets unfocused.
    bool                    ActiveIdHasBeenPressedBefore;       // Track whether the active id led to a press (this is to allow changing between PressOnClick and PressOnRelease without pressing twice). Used by range_select branch.
    bool                    ActiveIdHasBeenEditedBefore;        // Was the value associated to the widget Edited over the course of the Active state.
    bool                    ActiveIdHasBeenEditedThisFrame;
    bool                    ActiveIdFromShortcut;
    HvkS8                    ActiveIdMouseButton;
    HvkGuiID                 ActiveIdDisabledId;                 // When clicking a disabled item we set ActiveId=window->MoveId to avoid interference with widget code. Actual item ID is stored here.
    HvkVec2                  ActiveIdClickOffset;                // Clicked offset from upper-left corner, if applicable (currently only set by ButtonBehavior)
    HvkGuiInputSource        ActiveIdSource;                     // Activating source: HvkGuiInputSource_Mouse OR HvkGuiInputSource_Keyboard OR HvkGuiInputSource_Gamepad
    HvkGuiWindow*            ActiveIdWindow;
    HvkGuiID                 ActiveIdPreviousFrame;
    HvkGuiDeactivatedItemData DeactivatedItemData;
    HvkGuiDataTypeStorage    ActiveIdValueOnActivation;          // Backup of initial value at the time of activation. ONLY SET BY SPECIFIC WIDGETS: DragXXX and SliderXXX.
    HvkGuiID                 LastActiveId;                       // Store the last non-zero ActiveId, useful for animation.
    float                   LastActiveIdTimer;                  // Store the last non-zero ActiveId timer since the beginning of activation, useful for animation.

    // Key/Input Ownership + Shortcut Routing system
    // - The idea is that instead of "eating" a given key, we can link to an owner.
    // - Input query can then read input by specifying HvkGuiKeyOwner_Any (== 0), HvkGuiKeyOwner_NoOwner (== -1) or a custom ID.
    // - Routing is requested ahead of time for a given chord (Key + Mods) and granted in NewFrame().
    double                  LastKeyModsChangeTime;              // Record the last time key mods changed (affect repeat delay when using shortcut logic)
    double                  LastKeyModsChangeFromNoneTime;      // Record the last time key mods changed away from being 0 (affect repeat delay when using shortcut logic)
    double                  LastKeyboardKeyPressTime;           // Record the last time a keyboard key (ignore mouse/gamepad ones) was pressed.
    HvkBitArrayForNamedKeys  KeysMayBeCharInput;                 // Lookup to tell if a key can emit char input, see IsKeyChordPotentiallyCharInput(). sizeof() = 20 bytes
    HvkGuiKeyOwnerData       KeysOwnerData[HvkGuiKey_NamedKey_COUNT];
    HvkGuiKeyRoutingTable    KeysRoutingTable;
    HvkU32                   ActiveIdUsingNavDirMask;            // Active widget will want to read those nav move requests (e.g. can activate a button and move away from it)
    bool                    ActiveIdUsingAllKeyboardKeys;       // Active widget will want to read all keyboard keys inputs. (this is a shortcut for not taking ownership of 100+ keys, frequently used by drag operations)
    HvkGuiKeyChord           DebugBreakInShortcutRouting;        // Set to break in SetShortcutRouting()/Shortcut() calls.
    //HvkU32                 ActiveIdUsingNavInputMask;          // [OBSOLETE] Since (HvkGui_VERSION_NUM >= 18804) : 'g.ActiveIdUsingNavInputMask |= (1 << HvkGuiNavInput_Cancel);' becomes --> 'SetKeyOwner(HvkGuiKey_Escape, g.ActiveId) and/or SetKeyOwner(HvkGuiKey_NavGamepadCancel, g.ActiveId);'

    // Next window/item data
    HvkGuiID                 CurrentFocusScopeId;                // Value for currently appending items == g.FocusScopeStack.back(). Not to be mistaken with g.NavFocusScopeId.
    HvkGuiItemFlags          CurrentItemFlags;                   // Value for currently appending items == g.ItemFlagsStack.back()
    HvkGuiID                 DebugLocateId;                      // Storage for DebugLocateItemOnHover() feature: this is read by ItemAdd() so we keep it in a hot/cached location
    HvkGuiNextItemData       NextItemData;                       // Storage for SetNextItem** functions
    HvkGuiLastItemData       LastItemData;                       // Storage for last submitted item (setup by ItemAdd)
    HvkGuiNextWindowData     NextWindowData;                     // Storage for SetNextWindow** functions
    bool                    DebugShowGroupRects;
    bool                    GcCompactAll;                       // Request full GC

    // Shared stacks
    HvkGuiCol                        DebugFlashStyleColorIdx;    // (Keep close to ColorStack to share cache line)
    HvkVector<HvkGuiColorMod>         ColorStack;                 // Stack for PushStyleColor()/PopStyleColor() - inherited by Begin()
    HvkVector<HvkGuiStyleMod>         StyleVarStack;              // Stack for PushStyleVar()/PopStyleVar() - inherited by Begin()
    HvkVector<HvkFontStackData>       FontStack;                  // Stack for PushFont()/PopFont() - inherited by Begin()
    HvkVector<HvkGuiFocusScopeData>   FocusScopeStack;            // Stack for PushFocusScope()/PopFocusScope() - inherited by BeginChild(), pushed into by Begin()
    HvkVector<HvkGuiItemFlags>        ItemFlagsStack;             // Stack for PushItemFlag()/PopItemFlag() - inherited by Begin()
    HvkVector<HvkGuiGroupData>        GroupStack;                 // Stack for BeginGroup()/EndGroup() - not inherited by Begin()
    HvkVector<HvkGuiPopupData>        OpenPopupStack;             // Which popups are open (persistent)
    HvkVector<HvkGuiPopupData>        BeginPopupStack;            // Which level of BeginPopup() we are in (reset every frame)
    HvkVector<HvkGuiTreeNodeStackData>TreeNodeStack;              // Stack for TreeNode()

    // Viewports
    HvkVector<HvkGuiViewportP*> Viewports;                        // Active viewports (Size==1 in 'master' branch). Each viewports hold their copy of HvkDrawData.

    // Keyboard/Gamepad Navigation
    bool                    NavCursorVisible;                   // Nav focus cursor/rectangle is visible? We hide it after a mouse click. We show it after a nav move.
    bool                    NavHighlightItemUnderNav;           // Disable mouse hovering highlight. Highlight navigation focused item instead of mouse hovered item.
    //bool                  NavDisableHighlight;                // Old name for !g.NavCursorVisible before 1.91.4 (2024/10/18). OPPOSITE VALUE (g.NavDisableHighlight == !g.NavCursorVisible)
    //bool                  NavDisableMouseHover;               // Old name for g.NavHighlightItemUnderNav before 1.91.1 (2024/10/18) this was called When user starts using keyboard/gamepad, we hide mouse hovering highlight until mouse is touched again.
    bool                    NavMousePosDirty;                   // When set we will update mouse position if io.ConfigNavMoveSetMousePos is set (not enabled by default)
    bool                    NavIdIsAlive;                       // Nav widget has been seen this frame ~~ NavRectRel is valid
    HvkGuiID                 NavId;                              // Focused item for navigation
    HvkGuiWindow*            NavWindow;                          // Focused window for navigation. Could be called 'FocusedWindow'
    HvkGuiID                 NavFocusScopeId;                    // Focused focus scope (e.g. selection code often wants to "clear other items" when landing on an item of the same scope)
    HvkGuiNavLayer           NavLayer;                           // Focused layer (main scrolling layer, or menu/title bar layer)
    HvkGuiID                 NavActivateId;                      // ~~ (g.ActiveId == 0) && (IsKeyPressed(HvkGuiKey_Space) || IsKeyDown(HvkGuiKey_Enter) || IsKeyPressed(HvkGuiKey_NavGamepadActivate)) ? NavId : 0, also set when calling ActivateItemByID()
    HvkGuiID                 NavActivateDownId;                  // ~~ IsKeyDown(HvkGuiKey_Space) || IsKeyDown(HvkGuiKey_Enter) || IsKeyDown(HvkGuiKey_NavGamepadActivate) ? NavId : 0
    HvkGuiID                 NavActivatePressedId;               // ~~ IsKeyPressed(HvkGuiKey_Space) || IsKeyPressed(HvkGuiKey_Enter) || IsKeyPressed(HvkGuiKey_NavGamepadActivate) ? NavId : 0 (no repeat)
    HvkGuiActivateFlags      NavActivateFlags;
    HvkVector<HvkGuiFocusScopeData> NavFocusRoute;                // Reversed copy focus scope stack for NavId (should contains NavFocusScopeId). This essentially follow the window->ParentWindowForFocusRoute chain.
    HvkGuiID                 NavHighlightActivatedId;
    float                   NavHighlightActivatedTimer;
    HvkGuiID                 NavNextActivateId;                  // Set by ActivateItemByID(), queued until next frame.
    HvkGuiActivateFlags      NavNextActivateFlags;
    HvkGuiInputSource        NavInputSource;                     // Keyboard or Gamepad mode? THIS CAN ONLY BE HvkGuiInputSource_Keyboard or HvkGuiInputSource_Gamepad
    HvkGuiSelectionUserData  NavLastValidSelectionUserData;      // Last valid data passed to SetNextItemSelectionUser(), or -1. For current window. Not reset when focusing an item that doesn't have selection data.
    HvkS8                    NavCursorHideFrames;
    //HvkGuiID               NavActivateInputId;                 // Removed in 1.89.4 (July 2023). This is now part of g.NavActivateId and sets g.NavActivateFlags |= HvkGuiActivateFlags_PreferInput. See commit c9a53aa74, issue #5606.

    // Navigation: Init & Move Requests
    bool                    NavAnyRequest;                      // ~~ NavMoveRequest || NavInitRequest this is to perform early out in ItemAdd()
    bool                    NavInitRequest;                     // Init request for appearing window to select first item
    bool                    NavInitRequestFromMove;
    HvkGuiNavItemData        NavInitResult;                      // Init request result (first item of the window, or one for which SetItemDefaultFocus() was called)
    bool                    NavMoveSubmitted;                   // Move request submitted, will process result on next NewFrame()
    bool                    NavMoveScoringItems;                // Move request submitted, still scoring incoming items
    bool                    NavMoveForwardToNextFrame;
    HvkGuiNavMoveFlags       NavMoveFlags;
    HvkGuiScrollFlags        NavMoveScrollFlags;
    HvkGuiKeyChord           NavMoveKeyMods;
    HvkGuiDir                NavMoveDir;                         // Direction of the move request (left/right/up/down)
    HvkGuiDir                NavMoveDirForDebug;
    HvkGuiDir                NavMoveClipDir;                     // FIXME-NAV: Describe the purpose of this better. Might want to rename?
    HvkRect                  NavScoringRect;                     // Rectangle used for scoring, in screen space. Based of window->NavRectRel[], modified for directional navigation scoring.
    HvkRect                  NavScoringNoClipRect;               // Some nav operations (such as PageUp/PageDown) enforce a region which clipper will attempt to always keep submitted
    int                     NavScoringDebugCount;               // Metrics for debugging
    int                     NavTabbingDir;                      // Generally -1 or +1, 0 when tabbing without a nav id
    int                     NavTabbingCounter;                  // >0 when counting items for tabbing
    HvkGuiNavItemData        NavMoveResultLocal;                 // Best move request candidate within NavWindow
    HvkGuiNavItemData        NavMoveResultLocalVisible;          // Best move request candidate within NavWindow that are mostly visible (when using HvkGuiNavMoveFlags_AlsoScoreVisibleSet flag)
    HvkGuiNavItemData        NavMoveResultOther;                 // Best move request candidate within NavWindow's flattened hierarchy (when using HvkGuiWindowFlags_NavFlattened flag)
    HvkGuiNavItemData        NavTabbingResultFirst;              // First tabbing request candidate within NavWindow and flattened hierarchy

    // Navigation: record of last move request
    HvkGuiID                 NavJustMovedFromFocusScopeId;       // Just navigated from this focus scope id (result of a successfully MoveRequest).
    HvkGuiID                 NavJustMovedToId;                   // Just navigated to this id (result of a successfully MoveRequest).
    HvkGuiID                 NavJustMovedToFocusScopeId;         // Just navigated to this focus scope id (result of a successfully MoveRequest).
    HvkGuiKeyChord           NavJustMovedToKeyMods;
    bool                    NavJustMovedToIsTabbing;            // Copy of HvkGuiNavMoveFlags_IsTabbing. Maybe we should store whole flags.
    bool                    NavJustMovedToHasSelectionData;     // Copy of move result's ItemFlags & HvkGuiItemFlags_HasSelectionUserData). Maybe we should just store HvkGuiNavItemData.

    // Navigation: Windowing (Ctrl+Tab for list, or Menu button + keys or directional pads to move/resize)
    bool                    ConfigNavWindowingWithGamepad;      // = true. Enable Ctrl+Tab by holding HvkGuiKey_GamepadFaceLeft (== HvkGuiKey_NavGamepadMenu). When false, the button may still be used to toggle Menu layer.
    HvkGuiKeyChord           ConfigNavWindowingKeyNext;          // = HvkGuiMod_Ctrl | HvkGuiKey_Tab (or HvkGuiMod_Super | HvkGuiKey_Tab on OS X). For reconfiguration (see #4828)
    HvkGuiKeyChord           ConfigNavWindowingKeyPrev;          // = HvkGuiMod_Ctrl | HvkGuiMod_Shift | HvkGuiKey_Tab (or HvkGuiMod_Super | HvkGuiMod_Shift | HvkGuiKey_Tab on OS X)
    HvkGuiWindow*            NavWindowingTarget;                 // Target window when doing Ctrl+Tab (or Pad Menu + FocusPrev/Next), this window is temporarily displayed top-most!
    HvkGuiWindow*            NavWindowingTargetAnim;             // Record of last valid NavWindowingTarget until DimBgRatio and NavWindowingHighlightAlpha becomes 0.0f, so the fade-out can stay on it.
    HvkGuiWindow*            NavWindowingListWindow;             // Internal window actually listing the Ctrl+Tab contents
    float                   NavWindowingTimer;
    float                   NavWindowingHighlightAlpha;
    HvkGuiInputSource        NavWindowingInputSource;
    bool                    NavWindowingToggleLayer;            // Set while Alt or GamepadMenu is held, may be cleared by other operations, and processed when releasing the key.
    HvkGuiKey                NavWindowingToggleKey;              // Keyboard/gamepad key used when toggling to menu layer.
    HvkVec2                  NavWindowingAccumDeltaPos;
    HvkVec2                  NavWindowingAccumDeltaSize;

    // Render
    float                   DimBgRatio;                         // 0.0..1.0 animation when fading in a dimming background (for modal window and Ctrl+Tab list)

    // Drag and Drop
    bool                    DragDropActive;
    bool                    DragDropWithinSource;               // Set when within a BeginDragDropXXX/EndDragDropXXX block for a drag source.
    bool                    DragDropWithinTarget;               // Set when within a BeginDragDropXXX/EndDragDropXXX block for a drag target.
    HvkGuiDragDropFlags      DragDropSourceFlags;
    int                     DragDropSourceFrameCount;
    int                     DragDropMouseButton;
    HvkGuiPayload            DragDropPayload;
    HvkRect                  DragDropTargetRect;                 // Store rectangle of current target candidate (we favor small targets when overlapping)
    HvkRect                  DragDropTargetClipRect;             // Store ClipRect at the time of item's drawing
    HvkGuiID                 DragDropTargetId;
    HvkGuiID                 DragDropTargetFullViewport;
    HvkGuiDragDropFlags      DragDropAcceptFlagsCurr;
    HvkGuiDragDropFlags      DragDropAcceptFlagsPrev;
    float                   DragDropAcceptIdCurrRectSurface;    // Target item surface (we resolve overlapping targets by prioritizing the smaller surface)
    HvkGuiID                 DragDropAcceptIdCurr;               // Target item id (set at the time of accepting the payload)
    HvkGuiID                 DragDropAcceptIdPrev;               // Target item id from previous frame (we need to store this to allow for overlapping drag and drop targets)
    int                     DragDropAcceptFrameCount;           // Last time a target expressed a desire to accept the source
    HvkGuiID                 DragDropHoldJustPressedId;          // Set when holding a payload just made ButtonBehavior() return a press.
    HvkVector<unsigned char> DragDropPayloadBufHeap;             // We don't expose the HvkVector<> directly, HvkGuiPayload only holds pointer+size
    unsigned char           DragDropPayloadBufLocal[16];        // Local buffer for small payloads

    // Clipper
    int                             ClipperTempDataStacked;
    HvkVector<HvkGuiListClipperData>  ClipperTempData;

    // Tables
    HvkGuiTable*                     CurrentTable;
    HvkGuiID                         DebugBreakInTable;          // Set to break in BeginTable() call.
    int                             TablesTempDataStacked;      // Temporary table data size (because we leave previous instances undestructed, we generally don't use TablesTempData.Size)
    HvkVector<HvkGuiTableTempData>    TablesTempData;             // Temporary table data (buffers reused/shared across instances, support nesting)
    HvkPool<HvkGuiTable>              Tables;                     // Persistent table data
    HvkVector<float>                 TablesLastTimeActive;       // Last used timestamp of each tables (SOA, for efficient GC)
    HvkVector<HvkDrawChannel>         DrawChannelsTempMergeBuffer;

    // Tab bars
    HvkGuiTabBar*                    CurrentTabBar;
    HvkPool<HvkGuiTabBar>             TabBars;
    HvkVector<HvkGuiPtrOrIndex>       CurrentTabBarStack;
    HvkVector<HvkGuiShrinkWidthItem>  ShrinkWidthBuffer;

    // Multi-Select state
    HvkGuiBoxSelectState             BoxSelectState;
    HvkGuiMultiSelectTempData*       CurrentMultiSelect;
    int                             MultiSelectTempDataStacked; // Temporary multi-select data size (because we leave previous instances undestructed, we generally don't use MultiSelectTempData.Size)
    HvkVector<HvkGuiMultiSelectTempData> MultiSelectTempData;
    HvkPool<HvkGuiMultiSelectState>   MultiSelectStorage;

    // Hover Delay system
    HvkGuiID                 HoverItemDelayId;
    HvkGuiID                 HoverItemDelayIdPreviousFrame;
    float                   HoverItemDelayTimer;                // Currently used by IsItemHovered()
    float                   HoverItemDelayClearTimer;           // Currently used by IsItemHovered(): grace time before g.TooltipHoverTimer gets cleared.
    HvkGuiID                 HoverItemUnlockedStationaryId;      // Mouse has once been stationary on this item. Only reset after departing the item.
    HvkGuiID                 HoverWindowUnlockedStationaryId;    // Mouse has once been stationary on this window. Only reset after departing the window.

    // Mouse state
    HvkGuiMouseCursor        MouseCursor;
    float                   MouseStationaryTimer;               // Time the mouse has been stationary (with some loose heuristic)
    HvkVec2                  MouseLastValidPos;

    // Widget state
    HvkGuiInputTextState     InputTextState;
    HvkGuiTextIndex          InputTextLineIndex;                 // Temporary storage
    HvkGuiInputTextDeactivatedState InputTextDeactivatedState;
    HvkFontBaked             InputTextPasswordFontBackupBaked;
    HvkFontFlags             InputTextPasswordFontBackupFlags;
    HvkGuiID                 TempInputId;                        // Temporary text input when using Ctrl+Click on a slider, etc.
    HvkGuiDataTypeStorage    DataTypeZeroValue;                  // 0 for all data types
    int                     BeginMenuDepth;
    int                     BeginComboDepth;
    HvkGuiColorEditFlags     ColorEditOptions;                   // Store user options for color edit widgets
    HvkGuiID                 ColorEditCurrentID;                 // Set temporarily while inside of the parent-most ColorEdit4/ColorPicker4 (because they call each others).
    HvkGuiID                 ColorEditSavedID;                   // ID we are saving/restoring HS for
    float                   ColorEditSavedHue;                  // Backup of last Hue associated to LastColor, so we can restore Hue in lossy RGB<>HSV round trips
    float                   ColorEditSavedSat;                  // Backup of last Saturation associated to LastColor, so we can restore Saturation in lossy RGB<>HSV round trips
    HvkU32                   ColorEditSavedColor;                // RGB value with alpha set to 0.
    HvkVec4                  ColorPickerRef;                     // Initial/reference color at the time of opening the color picker.
    HvkGuiComboPreviewData   ComboPreviewData;
    HvkRect                  WindowResizeBorderExpectedRect;     // Expected border rect, switch to relative edit if moving
    bool                    WindowResizeRelativeMode;
    short                   ScrollbarSeekMode;                  // 0: scroll to clicked location, -1/+1: prev/next page.
    float                   ScrollbarClickDeltaToGrabCenter;    // When scrolling to mouse location: distance between mouse and center of grab box, normalized in parent space.
    float                   SliderGrabClickOffset;
    float                   SliderCurrentAccum;                 // Accumulated slider delta when using navigation controls.
    bool                    SliderCurrentAccumDirty;            // Has the accumulated slider delta changed since last time we tried to apply it?
    bool                    DragCurrentAccumDirty;
    float                   DragCurrentAccum;                   // Accumulator for dragging modification. Always high-precision, not rounded by end-user precision settings
    float                   DragSpeedDefaultRatio;              // If speed == 0.0f, uses (max-min) * DragSpeedDefaultRatio
    float                   DisabledAlphaBackup;                // Backup for style.Alpha for BeginDisabled()
    short                   DisabledStackSize;
    short                   TooltipOverrideCount;
    HvkGuiWindow*            TooltipPreviousWindow;              // Window of last tooltip submitted during the frame
    HvkVector<char>          ClipboardHandlerData;               // If no custom clipboard handler is defined
    HvkVector<HvkGuiID>       MenusIdSubmittedThisFrame;          // A list of menu IDs that were rendered at least once
    HvkGuiTypingSelectState  TypingSelectState;                  // State for GetTypingSelectRequest()

    // Platform support
    HvkGuiPlatformImeData    PlatformImeData;                    // Data updated by current frame. Will be applied at end of the frame. For some backends, this is required to have WantVisible=true in order to receive text message.
    HvkGuiPlatformImeData    PlatformImeDataPrev;                // Previous frame data. When changed we call the platform_io.Platform_SetImeDataFn() handler.

    // Extensions
    // FIXME: We could provide an API to register one slot in an array held in HvkGuiContext?
    HvkVector<HvkTextureData*> UserTextures;                      // List of textures created/managed by user or third-party extension. Automatically appended into platform_io.Textures[].

    // Settings
    bool                    SettingsLoaded;
    float                   SettingsDirtyTimer;                 // Save .ini Settings to memory when time reaches zero
    HvkGuiTextBuffer         SettingsIniData;                    // In memory .ini settings
    HvkVector<HvkGuiSettingsHandler>      SettingsHandlers;       // List of .ini settings handlers
    HvkChunkStream<HvkGuiWindowSettings>  SettingsWindows;        // HvkGuiWindow .ini settings entries
    HvkChunkStream<HvkGuiTableSettings>   SettingsTables;         // HvkGuiTable .ini settings entries
    HvkVector<HvkGuiContextHook>          Hooks;                  // Hooks for extensions (e.g. test engine)
    HvkGuiID                             HookIdNext;             // Next available HookId

    // Localization
    const char*             LocalizationTable[HvkGuiLocKey_COUNT];

    // Capture/Logging
    bool                    LogEnabled;                         // Currently capturing
    bool                    LogLineFirstItem;
    HvkGuiLogFlags           LogFlags;                           // Capture flags/type
    HvkGuiWindow*            LogWindow;
    HvkFileHandle            LogFile;                            // If != NULL log to stdout/ file
    HvkGuiTextBuffer         LogBuffer;                          // Accumulation buffer when log to clipboard. This is pointer so our GHvkGui static constructor doesn't call heap allocators.
    const char*             LogNextPrefix;                      // See comment in LogSetNextTextDecoration(): doesn't copy underlying data, use carefully!
    const char*             LogNextSuffix;
    float                   LogLinePosY;
    int                     LogDepthRef;
    int                     LogDepthToExpand;
    int                     LogDepthToExpandDefault;            // Default/stored value for LogDepthMaxExpand if not specified in the LogXXX function call.

    // Error Handling
    HvkGuiErrorCallback      ErrorCallback;                      // = NULL. May be exposed in public API eventually.
    void*                   ErrorCallbackUserData;              // = NULL
    HvkVec2                  ErrorTooltipLockedPos;
    bool                    ErrorFirst;
    int                     ErrorCountCurrentFrame;             // [Internal] Number of errors submitted this frame.
    HvkGuiErrorRecoveryState StackSizesInNewFrame;               // [Internal]
    HvkGuiErrorRecoveryState*StackSizesInBeginForCurrentWindow;  // [Internal]

    // Debug Tools
    // (some of the highly frequently used data are interleaved in other structures above: DebugBreakXXX fields, DebugHookIdInfo, DebugLocateId etc.)
    int                     DebugDrawIdConflictsCount;          // Locked count (preserved when holding Ctrl)
    HvkGuiDebugLogFlags      DebugLogFlags;
    HvkGuiTextBuffer         DebugLogBuf;
    HvkGuiTextIndex          DebugLogIndex;
    int                     DebugLogSkippedErrors;
    HvkGuiDebugLogFlags      DebugLogAutoDisableFlags;
    HvkU8                    DebugLogAutoDisableFrames;
    HvkU8                    DebugLocateFrames;                  // For DebugLocateItemOnHover(). This is used together with DebugLocateId which is in a hot/cached spot above.
    bool                    DebugBreakInLocateId;               // Debug break in ItemAdd() call for g.DebugLocateId.
    HvkGuiKeyChord           DebugBreakKeyChord;                 // = HvkGuiKey_Pause
    HvkS8                    DebugBeginReturnValueCullDepth;     // Cycle between 0..9 then wrap around.
    bool                    DebugItemPickerActive;              // Item picker is active (started with DebugStartItemPicker())
    HvkU8                    DebugItemPickerMouseButton;
    HvkGuiID                 DebugItemPickerBreakId;             // Will call Hvk_DEBUG_BREAK() when encountering this ID
    float                   DebugFlashStyleColorTime;
    HvkVec4                  DebugFlashStyleColorBackup;
    HvkGuiMetricsConfig      DebugMetricsConfig;
    HvkGuiDebugItemPathQuery DebugItemPathQuery;
    HvkGuiIDStackTool        DebugIDStackTool;
    HvkGuiDebugAllocInfo     DebugAllocInfo;
#if defined(HvkGui_DEBUG_HIGHLIGHT_ALL_ID_CONFLICTS) && !defined(HvkGui_DISABLE_DEBUG_TOOLS)
    HvkGuiStorage            DebugDrawIdConflictsAliveCount;
    HvkGuiStorage            DebugDrawIdConflictsHighlightSet;
#endif

    // Misc
    float                   FramerateSecPerFrame[60];           // Calculate estimate of framerate for user over the last 60 frames..
    int                     FramerateSecPerFrameIdx;
    int                     FramerateSecPerFrameCount;
    float                   FramerateSecPerFrameAccum;
    int                     WantCaptureMouseNextFrame;          // Explicit capture override via SetNextFrameWantCaptureMouse()/SetNextFrameWantCaptureKeyboard(). Default to -1.
    int                     WantCaptureKeyboardNextFrame;       // "
    int                     WantTextInputNextFrame;             // Copied in EndFrame() from g.PlatformImeData.WantTextInput. Needs to be set for some backends (SDL3) to emit character inputs.
    HvkVector<char>          TempBuffer;                         // Temporary text buffer
    char                    TempKeychordName[64];

    HvkGuiContext(HvkFontAtlas* shared_font_atlas);
    ~HvkGuiContext();
};

//-----------------------------------------------------------------------------
// [SECTION] HvkGuiWindowTempData, HvkGuiWindow
//-----------------------------------------------------------------------------

#define HvkGui_WINDOW_HARD_MIN_SIZE 4.0f

// Transient per-window data, reset at the beginning of the frame. This used to be called HvkGuiDrawContext, hence the DC variable name in HvkGuiWindow.
// (That's theory, in practice the delimitation between HvkGuiWindow and HvkGuiWindowTempData is quite tenuous and could be reconsidered..)
// (This doesn't need a constructor because we zero-clear it as part of HvkGuiWindow and all frame-temporary data are setup on Begin)
struct HvkGui_API HvkGuiWindowTempData
{
    // Layout
    HvkVec2                  CursorPos;              // Current emitting position, in absolute coordinates.
    HvkVec2                  CursorPosPrevLine;
    HvkVec2                  CursorStartPos;         // Initial position after Begin(), generally ~ window position + WindowPadding.
    HvkVec2                  CursorMaxPos;           // Used to Hvkplicitly calculate ContentSize at the beginning of next frame, for scrolling range and auto-resize. Always growing during the frame.
    HvkVec2                  IdealMaxPos;            // Used to Hvkplicitly calculate ContentSizeIdeal at the beginning of next frame, for auto-resize only. Always growing during the frame.
    HvkVec2                  CurrLineSize;
    HvkVec2                  PrevLineSize;
    float                   CurrLineTextBaseOffset; // Baseline offset (0.0f by default on a new line, generally == style.FramePadding.y when a framed item has been added).
    float                   PrevLineTextBaseOffset;
    bool                    IsSameLine;
    bool                    IsSetPos;
    HvkVec1                  Indent;                 // Indentation / start position from left of window (increased by TreePush/TreePop, etc.)
    HvkVec1                  ColumnsOffset;          // Offset to the current column (if ColumnsCurrent > 0). FIXME: This and the above should be a stack to allow use cases like Tree->Column->Tree. Need revamp columns API.
    HvkVec1                  GroupOffset;
    HvkVec2                  CursorStartPosLossyness;// Record the loss of precision of CursorStartPos due to really large scrolling amount. This is used by clipper to compensate and fix the most common use case of large scroll area.

    // Keyboard/Gamepad navigation
    HvkGuiNavLayer           NavLayerCurrent;        // Current layer, 0..31 (we currently only use 0..1)
    short                   NavLayersActiveMask;    // Which layers have been written to (result from previous frame)
    short                   NavLayersActiveMaskNext;// Which layers have been written to (accumulator for current frame)
    bool                    NavIsScrollPushableX;   // Set when current work location may be scrolled horizontally when moving left / right. This is generally always true UNLESS within a column.
    bool                    NavHideHighlightOneFrame;
    bool                    NavWindowHasScrollY;    // Set per window when scrolling can be used (== ScrollMax.y > 0.0f)

    // Miscellaneous
    bool                    MenuBarAppending;       // FIXME: Remove this
    HvkVec2                  MenuBarOffset;          // MenuBarOffset.x is sort of equivalent of a per-layer CursorPos.x, saved/restored as we switch to the menu bar. The only situation when MenuBarOffset.y is > 0 if when (SafeAreaPadding.y > FramePadding.y), often used on TVs.
    HvkGuiMenuColumns        MenuColumns;            // Simplified columns storage for menu items measurement
    int                     TreeDepth;              // Current tree depth.
    HvkU32                   TreeHasStackDataDepthMask;      // Store whether given depth has HvkGuiTreeNodeStackData data. Could be turned into a HvkU64 if necessary.
    HvkU32                   TreeRecordsClippedNodesY2Mask;  // Store whether we should keep recording Y2. Cleared when passing clip max. Equivalent TreeHasStackDataDepthMask value should always be set.
    HvkVector<HvkGuiWindow*>  ChildWindows;
    HvkGuiStorage*           StateStorage;           // Current persistent per-window storage (store e.g. tree node open/close state)
    HvkGuiOldColumns*        CurrentColumns;         // Current columns set
    int                     CurrentTableIdx;        // Current table index (into g.Tables)
    HvkGuiLayoutType         LayoutType;
    HvkGuiLayoutType         ParentLayoutType;       // Layout type of parent window at the time of Begin()
    HvkU32                   ModalDimBgColor;
    HvkGuiItemStatusFlags    WindowItemStatusFlags;
    HvkGuiItemStatusFlags    ChildItemStatusFlags;

    // Local parameters stacks
    // We store the current settings outside of the vectors to increase memory locality (reduce cache misses). The vectors are rarely modified. Also it allows us to not heap allocate for short-lived windows which are not using those settings.
    float                   ItemWidth;              // Current item width (>0.0: width in pixels, <0.0: align xx pixels to the right of window).
    float                   TextWrapPos;            // Current text wrap pos.
    HvkVector<float>         ItemWidthStack;         // Store item widths to restore (attention: .back() is not == ItemWidth)
    HvkVector<float>         TextWrapPosStack;       // Store text wrap pos to restore (attention: .back() is not == TextWrapPos)
};

// Storage for one window
struct HvkGui_API HvkGuiWindow
{
    HvkGuiContext*           Ctx;                                // Parent UI context (needs to be set explicitly by parent).
    char*                   Name;                               // Window name, owned by the window.
    HvkGuiID                 ID;                                 // == HvkHashStr(Name)
    HvkGuiWindowFlags        Flags;                              // See enum HvkGuiWindowFlags_
    HvkGuiChildFlags         ChildFlags;                         // Set when window is a child window. See enum HvkGuiChildFlags_
    HvkGuiViewportP*         Viewport;                           // Always set in Begin(). Inactive windows may have a NULL value here if their viewport was discarded.
    HvkVec2                  Pos;                                // Position (always rounded-up to nearest pixel)
    HvkVec2                  Size;                               // Current size (==SizeFull or collapsed title bar size)
    HvkVec2                  SizeFull;                           // Size when non collapsed
    HvkVec2                  ContentSize;                        // Size of contents/scrollable client area (calculated from the extents reach of the cursor) from previous frame. Does not include window decoration or window padding.
    HvkVec2                  ContentSizeIdeal;
    HvkVec2                  ContentSizeExplicit;                // Size of contents/scrollable client area explicitly request by the user via SetNextWindowContentSize().
    HvkVec2                  WindowPadding;                      // Window padding at the time of Begin().
    float                   WindowRounding;                     // Window rounding at the time of Begin(). May be clamped lower to avoid rendering artifacts with title bar, menu bar etc.
    float                   WindowBorderSize;                   // Window border size at the time of Begin().
    float                   TitleBarHeight, MenuBarHeight;      // Note that those used to be function before 2024/05/28. If you have old code calling TitleBarHeight() you can change it to TitleBarHeight.
    float                   DecoOuterSizeX1, DecoOuterSizeY1;   // Left/Up offsets. Sum of non-scrolling outer decorations (X1 generally == 0.0f. Y1 generally = TitleBarHeight + MenuBarHeight). Locked during Begin().
    float                   DecoOuterSizeX2, DecoOuterSizeY2;   // Right/Down offsets (X2 generally == ScrollbarSize.x, Y2 == ScrollbarSizes.y).
    float                   DecoInnerSizeX1, DecoInnerSizeY1;   // Applied AFTER/OVER InnerRect. Specialized for Tables as they use specialized form of clipping and frozen rows/columns are inside InnerRect (and not part of regular decoration sizes).
    int                     NameBufLen;                         // Size of buffer storing Name. May be larger than strlen(Name)!
    HvkGuiID                 MoveId;                             // == window->GetID("#MOVE")
    HvkGuiID                 ChildId;                            // ID of corresponding item in parent window (for navigation to return from child window to parent window)
    HvkGuiID                 PopupId;                            // ID in the popup stack when this window is used as a popup/menu (because we use generic Name/ID for recycling)
    HvkVec2                  Scroll;
    HvkVec2                  ScrollMax;
    HvkVec2                  ScrollTarget;                       // target scroll position. stored as cursor position with scrolling canceled out, so the highest point is always 0.0f. (FLT_MAX for no change)
    HvkVec2                  ScrollTargetCenterRatio;            // 0.0f = scroll so that target position is at top, 0.5f = scroll so that target position is centered
    HvkVec2                  ScrollTargetEdgeSnapDist;           // 0.0f = no snapping, >0.0f snapping threshold
    HvkVec2                  ScrollbarSizes;                     // Size taken by each scrollbars on their smaller axis. Pay attention! ScrollbarSizes.x == width of the vertical scrollbar, ScrollbarSizes.y = height of the horizontal scrollbar.
    bool                    ScrollbarX, ScrollbarY;             // Are scrollbars visible?
    bool                    ScrollbarXStabilizeEnabled;         // Was ScrollbarX previously auto-stabilized?
    HvkU8                    ScrollbarXStabilizeToggledHistory;  // Used to stabilize scrollbar visibility in case of feedback loops
    bool                    Active;                             // Set to true on Begin(), unless Collapsed
    bool                    WasActive;
    bool                    WriteAccessed;                      // Set to true when any widget access the current window
    bool                    Collapsed;                          // Set when collapsing window to become only title-bar
    bool                    WantCollapseToggle;
    bool                    SkipItems;                          // Set when items can safely be all clipped (e.g. window not visible or collapsed)
    bool                    SkipRefresh;                        // [EXPERIMENTAL] Reuse previous frame drawn contents, Begin() returns false.
    bool                    Appearing;                          // Set during the frame where the window is appearing (or re-appearing)
    bool                    Hidden;                             // Do not display (== HiddenFrames*** > 0)
    bool                    IsFallbackWindow;                   // Set on the "Debug##Default" window.
    bool                    IsExplicitChild;                    // Set when passed _ChildWindow, left to false by BeginDocked()
    bool                    HasCloseButton;                     // Set when the window has a close button (p_open != NULL)
    signed char             ResizeBorderHovered;                // Current border being hovered for resize (-1: none, otherwise 0-3)
    signed char             ResizeBorderHeld;                   // Current border being held for resize (-1: none, otherwise 0-3)
    short                   BeginCount;                         // Number of Begin() during the current frame (generally 0 or 1, 1+ if appending via multiple Begin/End pairs)
    short                   BeginCountPreviousFrame;            // Number of Begin() during the previous frame
    short                   BeginOrderWithinParent;             // Begin() order within Hvkmediate parent window, if we are a child window. Otherwise 0.
    short                   BeginOrderWithinContext;            // Begin() order within entire HvkGui context. This is mostly used for debugging submission order related issues.
    short                   FocusOrder;                         // Order within WindowsFocusOrder[], altered when windows are focused.
    HvkGuiDir                AutoPosLastDirection;
    HvkS8                    AutoFitFramesX, AutoFitFramesY;
    bool                    AutoFitOnlyGrows;
    HvkS8                    HiddenFramesCanSkipItems;           // Hide the window for N frames
    HvkS8                    HiddenFramesCannotSkipItems;        // Hide the window for N frames while allowing items to be submitted so we can measure their size
    HvkS8                    HiddenFramesForRenderOnly;          // Hide the window until frame N at Render() time only
    HvkS8                    DisableInputsFrames;                // Disable window interactions for N frames
    HvkGuiWindowBgClickFlags BgClickFlags : 8;                   // Configure behavior of click+dragging on window bg/void or over items. Default sets by io.ConfigWindowsMoveFromTitleBarOnly. If you use this please report in #3379.
    HvkGuiCond               SetWindowPosAllowFlags : 8;         // store acceptable condition flags for SetNextWindowPos() use.
    HvkGuiCond               SetWindowSizeAllowFlags : 8;        // store acceptable condition flags for SetNextWindowSize() use.
    HvkGuiCond               SetWindowCollapsedAllowFlags : 8;   // store acceptable condition flags for SetNextWindowCollapsed() use.
    HvkVec2                  SetWindowPosVal;                    // store window position when using a non-zero Pivot (position set needs to be processed when we know the window size)
    HvkVec2                  SetWindowPosPivot;                  // store window pivot for positioning. HvkVec2(0, 0) when positioning from top-left corner; HvkVec2(0.5f, 0.5f) for centering; HvkVec2(1, 1) for bottom right.

    HvkVector<HvkGuiID>       IDStack;                            // ID stack. ID are hashes seeded with the value at the top of the stack. (In theory this should be in the TempData structure)
    HvkGuiWindowTempData     DC;                                 // Temporary per-window data, reset at the beginning of the frame. This used to be called HvkGuiDrawContext, hence the "DC" variable name.

    // The best way to understand what those rectangles are is to use the 'Metrics->Tools->Show Windows Rectangles' viewer.
    // The main 'OuterRect', omitted as a field, is window->Rect().
    HvkRect                  OuterRectClipped;                   // == Window->Rect() just after setup in Begin(). == window->Rect() for root window.
    HvkRect                  InnerRect;                          // Inner rectangle (omit title bar, menu bar, scroll bar)
    HvkRect                  InnerClipRect;                      // == InnerRect shrunk by WindowPadding*0.5f on each side, clipped within viewport or parent clip rect.
    HvkRect                  WorkRect;                           // Initially covers the whole scrolling region. Reduced by containers e.g columns/tables when active. Shrunk by WindowPadding*1.0f on each side. This is meant to replace ContentRegionRect over time (from 1.71+ onward).
    HvkRect                  ParentWorkRect;                     // Backup of WorkRect before entering a container such as columns/tables. Used by e.g. SpanAllColumns functions to easily access. Stacked containers are responsible for maintaining this. // FIXME-WORKRECT: Could be a stack?
    HvkRect                  ClipRect;                           // Current clipping/scissoring rectangle, evolve as we are using PushClipRect(), etc. == DrawList->clip_rect_stack.back().
    HvkRect                  ContentRegionRect;                  // FIXME: This is currently confusing/misleading. It is essentially WorkRect but not handling of scrolling. We currently rely on it as right/bottom aligned sizing operation need some size to rely on.
    HvkVec2ih                HitTestHoleSize;                    // Define an optional rectangular hole where mouse will pass-through the window.
    HvkVec2ih                HitTestHoleOffset;

    int                     LastFrameActive;                    // Last frame number the window was Active.
    float                   LastTimeActive;                     // Last timestamp the window was Active (using float as we don't need high precision there)
    float                   ItemWidthDefault;
    HvkGuiStorage            StateStorage;
    HvkVector<HvkGuiOldColumns> ColumnsStorage;
    float                   FontWindowScale;                    // User scale multiplier per-window, via SetWindowFontScale()
    float                   FontWindowScaleParents;
    float                   FontRefSize;                        // This is a copy of window->CalcFontSize() at the time of Begin(), trying to phase out CalcFontSize() especially as it may be called on non-current window.
    int                     SettingsOffset;                     // Offset into SettingsWindows[] (offsets are always valid as we only grow the array from the back)

    HvkDrawList*             DrawList;                           // == &DrawListInst (for backward compatibility reason with code using HvkGui_internal.h we keep this a pointer)
    HvkDrawList              DrawListInst;
    HvkGuiWindow*            ParentWindow;                       // If we are a child _or_ popup _or_ docked window, this is pointing to our parent. Otherwise NULL.
    HvkGuiWindow*            ParentWindowInBeginStack;
    HvkGuiWindow*            RootWindow;                         // Point to ourself or first ancestor that is not a child window. Doesn't cross through popups/dock nodes.
    HvkGuiWindow*            RootWindowPopupTree;                // Point to ourself or first ancestor that is not a child window. Cross through popups parent<>child.
    HvkGuiWindow*            RootWindowForTitleBarHighlight;     // Point to ourself or first ancestor which will display TitleBgActive color when this window is active.
    HvkGuiWindow*            RootWindowForNav;                   // Point to ourself or first ancestor which doesn't have the NavFlattened flag.
    HvkGuiWindow*            ParentWindowForFocusRoute;          // Set to manual link a window to its logical parent so that Shortcut() chain are honored (e.g. Tool linked to Document)

    HvkGuiWindow*            NavLastChildNavWindow;              // When going to the menu bar, we remember the child window we came from. (This could probably be made Hvkplicit if we kept g.Windows sorted by last focused including child window.)
    HvkGuiID                 NavLastIds[HvkGuiNavLayer_COUNT];    // Last known NavId for this window, per layer (0/1)
    HvkRect                  NavRectRel[HvkGuiNavLayer_COUNT];    // Reference rectangle, in window relative space
    HvkVec2                  NavPreferredScoringPosRel[HvkGuiNavLayer_COUNT]; // Preferred X/Y position updated when moving on a given axis, reset to FLT_MAX.
    HvkGuiID                 NavRootFocusScopeId;                // Focus Scope ID at the time of Begin()

    int                     MemoryDrawListIdxCapacity;          // Backup of last idx/vtx count, so when waking up the window we can preallocate and avoid iterative alloc/copy
    int                     MemoryDrawListVtxCapacity;
    bool                    MemoryCompacted;                    // Set when window extraneous data have been garbage collected

public:
    HvkGuiWindow(HvkGuiContext* context, const char* name);
    ~HvkGuiWindow();

    HvkGuiID     GetID(const char* str, const char* str_end = NULL);
    HvkGuiID     GetID(const void* ptr);
    HvkGuiID     GetID(int n);
    HvkGuiID     GetIDFromPos(const HvkVec2& p_abs);
    HvkGuiID     GetIDFromRectangle(const HvkRect& r_abs);

    // We don't use g.FontSize because the window may be != g.CurrentWindow.
    HvkRect      Rect() const            { return HvkRect(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y); }
    HvkRect      TitleBarRect() const    { return HvkRect(Pos, HvkVec2(Pos.x + SizeFull.x, Pos.y + TitleBarHeight)); }
    HvkRect      MenuBarRect() const     { float y1 = Pos.y + TitleBarHeight; return HvkRect(Pos.x, y1, Pos.x + SizeFull.x, y1 + MenuBarHeight); }

    // [OBSOLETE] HvkGuiWindow::CalcFontSize() was removed in 1.92.0 because error-prone/misleading. You can use window->FontRefSize for a copy of g.FontSize at the time of the last Begin() call for this window.
    //float     CalcFontSize() const    { HvkGuiContext& g = *Ctx; return g.FontSizeBase * FontWindowScale * FontWindowScaleParents;
};

//-----------------------------------------------------------------------------
// [SECTION] Tab bar, Tab item support
//-----------------------------------------------------------------------------

// Extend HvkGuiTabBarFlags_
enum HvkGuiTabBarFlagsPrivate_
{
    HvkGuiTabBarFlags_DockNode                   = 1 << 20,  // Part of a dock node [we don't use this in the master branch but it facilitate branch syncing to keep this around]
    HvkGuiTabBarFlags_IsFocused                  = 1 << 21,
    HvkGuiTabBarFlags_SaveSettings               = 1 << 22,  // FIXME: Settings are handled by the docking system, this only request the tab bar to mark settings dirty when reordering tabs
};

// Extend HvkGuiTabItemFlags_
enum HvkGuiTabItemFlagsPrivate_
{
    HvkGuiTabItemFlags_SectionMask_              = HvkGuiTabItemFlags_Leading | HvkGuiTabItemFlags_Trailing,
    HvkGuiTabItemFlags_NoCloseButton             = 1 << 20,  // Track whether p_open was set or not (we'll need this info on the next frame to recompute ContentWidth during layout)
    HvkGuiTabItemFlags_Button                    = 1 << 21,  // Used by TabItemButton, change the tab item behavior to mimic a button
    HvkGuiTabItemFlags_Invisible                 = 1 << 22,  // To reserve space e.g. with HvkGuiTabItemFlags_Leading
    //HvkGuiTabItemFlags_Unsorted                = 1 << 23,  // [Docking] Trailing tabs with the _Unsorted flag will be sorted based on the DockOrder of their Window.
};

// Storage for one active tab item (sizeof() 40 bytes)
struct HvkGuiTabItem
{
    HvkGuiID             ID;
    HvkGuiTabItemFlags   Flags;
    int                 LastFrameVisible;
    int                 LastFrameSelected;      // This allows us to infer an ordered list of the last activated tabs with little maintenance
    float               Offset;                 // Position relative to beginning of tab
    float               Width;                  // Width currently displayed
    float               ContentWidth;           // Width of label + padding, stored during BeginTabItem() call (misnamed as "Content" would normally Hvkply width of label only)
    float               RequestedWidth;         // Width optionally requested by caller, -1.0f is unused
    HvkS32               NameOffset;             // When Window==NULL, offset to name within parent HvkGuiTabBar::TabsNames
    HvkS16               BeginOrder;             // BeginTabItem() order, used to re-order tabs after toggling HvkGuiTabBarFlags_Reorderable
    HvkS16               IndexDuringLayout;      // Index only used during TabBarLayout(). Tabs gets reordered so 'Tabs[n].IndexDuringLayout == n' but may mismatch during additions.
    bool                WantClose;              // Marked as closed by SetTabItemClosed()

    HvkGuiTabItem()      { memset(this, 0, sizeof(*this)); LastFrameVisible = LastFrameSelected = -1; RequestedWidth = -1.0f; NameOffset = -1; BeginOrder = IndexDuringLayout = -1; }
};

// Storage for a tab bar (sizeof() 160 bytes)
struct HvkGui_API HvkGuiTabBar
{
    HvkGuiWindow*        Window;
    HvkVector<HvkGuiTabItem> Tabs;
    HvkGuiTabBarFlags    Flags;
    HvkGuiID             ID;                     // Zero for tab-bars used by docking
    HvkGuiID             SelectedTabId;          // Selected tab/window
    HvkGuiID             NextSelectedTabId;      // Next selected tab/window. Will also trigger a scrolling animation
    HvkGuiID             VisibleTabId;           // Can occasionally be != SelectedTabId (e.g. when previewing contents for Ctrl+Tab preview)
    int                 CurrFrameVisible;
    int                 PrevFrameVisible;
    HvkRect              BarRect;
    float               BarRectPrevWidth;       // Backup of previous width. When width change we enforce keep horizontal scroll on focused tab.
    float               CurrTabsContentsHeight;
    float               PrevTabsContentsHeight; // Record the height of contents submitted below the tab bar
    float               WidthAllTabs;           // Actual width of all tabs (locked during layout)
    float               WidthAllTabsIdeal;      // Ideal width if all tabs were visible and not clipped
    float               ScrollingAnim;
    float               ScrollingTarget;
    float               ScrollingTargetDistToVisibility;
    float               ScrollingSpeed;
    float               ScrollingRectMinX;
    float               ScrollingRectMaxX;
    float               SeparatorMinX;
    float               SeparatorMaxX;
    HvkGuiID             ReorderRequestTabId;
    HvkS16               ReorderRequestOffset;
    HvkS8                BeginCount;
    bool                WantLayout;
    bool                VisibleTabWasSubmitted;
    bool                TabsAddedNew;           // Set to true when a new tab item or button has been added to the tab bar during last frame
    bool                ScrollButtonEnabled;
    HvkS16               TabsActiveCount;        // Number of tabs submitted this frame.
    HvkS16               LastTabItemIdx;         // Index of last BeginTabItem() tab for use by EndTabItem()
    float               ItemSpacingY;
    HvkVec2              FramePadding;           // style.FramePadding locked at the time of BeginTabBar()
    HvkVec2              BackupCursorPos;
    HvkGuiTextBuffer     TabsNames;              // For non-docking tab bar we re-append names in a contiguous buffer.

    HvkGuiTabBar();
};

//-----------------------------------------------------------------------------
// [SECTION] Table support
//-----------------------------------------------------------------------------

#define Hvk_COL32_DISABLE                Hvk_COL32(0,0,0,1)   // Special sentinel code which cannot be used as a regular color.
#define HvkGui_TABLE_MAX_COLUMNS         512                 // Arbitrary "safety" maximum, may be lifted in the future if needed. Must fit in HvkGuiTableColumnIdx/HvkGuiTableDrawChannelIdx.

// [Internal] sizeof() ~ 112
// We use the terminology "Enabled" to refer to a column that is not Hidden by user/api.
// We use the terminology "Clipped" to refer to a column that is out of sight because of scrolling/clipping.
// This is in contrast with some user-facing api such as IsItemVisible() / IsRectVisible() which use "Visible" to mean "not clipped".
struct HvkGuiTableColumn
{
    HvkGuiTableColumnFlags   Flags;                          // Flags after some patching (not directly same as provided by user). See HvkGuiTableColumnFlags_
    float                   WidthGiven;                     // Final/actual width visible == (MaxX - MinX), locked in TableUpdateLayout(). May be > WidthRequest to honor minimum width, may be < WidthRequest to honor shrinking columns down in tight space.
    float                   MinX;                           // Absolute positions
    float                   MaxX;
    float                   WidthRequest;                   // Master width absolute value when !(Flags & _WidthStretch). When Stretch this is derived every frame from StretchWeight in TableUpdateLayout()
    float                   WidthAuto;                      // Automatic width
    float                   WidthMax;                       // Maximum width (FIXME: overwritten by each instance)
    float                   StretchWeight;                  // Master width weight when (Flags & _WidthStretch). Often around ~1.0f initially.
    float                   InitStretchWeightOrWidth;       // Value passed to TableSetupColumn(). For Width it is a content width (_without padding_).
    HvkRect                  ClipRect;                       // Clipping rectangle for the column
    HvkGuiID                 UserID;                         // Optional, value passed to TableSetupColumn()
    float                   WorkMinX;                       // Contents region min ~(MinX + CellPaddingX + CellSpacingX1) == cursor start position when entering column
    float                   WorkMaxX;                       // Contents region max ~(MaxX - CellPaddingX - CellSpacingX2)
    float                   ItemWidth;                      // Current item width for the column, preserved across rows
    float                   ContentMaxXFrozen;              // Contents maximum position for frozen rows (apart from headers), from which we can infer content width.
    float                   ContentMaxXUnfrozen;
    float                   ContentMaxXHeadersUsed;         // Contents maximum position for headers rows (regardless of freezing). TableHeader() automatically softclip itself + report ideal desired size, to avoid creating extraneous draw calls
    float                   ContentMaxXHeadersIdeal;
    HvkS16                   NameOffset;                     // Offset into parent ColumnsNames[]
    HvkGuiTableColumnIdx     DisplayOrder;                   // Index within Table's IndexToDisplayOrder[] (column may be reordered by users)
    HvkGuiTableColumnIdx     IndexWithinEnabledSet;          // Index within enabled/visible set (<= IndexToDisplayOrder)
    HvkGuiTableColumnIdx     PrevEnabledColumn;              // Index of prev enabled/visible column within Columns[], -1 if first enabled/visible column
    HvkGuiTableColumnIdx     NextEnabledColumn;              // Index of next enabled/visible column within Columns[], -1 if last enabled/visible column
    HvkGuiTableColumnIdx     SortOrder;                      // Index of this column within sort specs, -1 if not sorting on this column, 0 for single-sort, may be >0 on multi-sort
    HvkGuiTableDrawChannelIdx DrawChannelCurrent;            // Index within DrawSplitter.Channels[]
    HvkGuiTableDrawChannelIdx DrawChannelFrozen;             // Draw channels for frozen rows (often headers)
    HvkGuiTableDrawChannelIdx DrawChannelUnfrozen;           // Draw channels for unfrozen rows
    bool                    IsEnabled;                      // IsUserEnabled && (Flags & HvkGuiTableColumnFlags_Disabled) == 0
    bool                    IsUserEnabled;                  // Is the column not marked Hidden by the user? (unrelated to being off view, e.g. clipped by scrolling).
    bool                    IsUserEnabledNextFrame;
    bool                    IsVisibleX;                     // Is actually in view (e.g. overlapping the host window clipping rectangle, not scrolled).
    bool                    IsVisibleY;
    bool                    IsRequestOutput;                // Return value for TableSetColumnIndex() / TableNextColumn(): whether we request user to output contents or not.
    bool                    IsSkipItems;                    // Do we want item submissions to this column to be completely ignored (no layout will happen).
    bool                    IsPreserveWidthAuto;
    HvkS8                    NavLayerCurrent;                // HvkGuiNavLayer in 1 byte
    HvkU8                    AutoFitQueue;                   // Queue of 8 values for the next 8 frames to request auto-fit
    HvkU8                    CannotSkipItemsQueue;           // Queue of 8 values for the next 8 frames to disable Clipped/SkipItem
    HvkU8                    SortDirection : 2;              // HvkGuiSortDirection_Ascending or HvkGuiSortDirection_Descending
    HvkU8                    SortDirectionsAvailCount : 2;   // Number of available sort directions (0 to 3)
    HvkU8                    SortDirectionsAvailMask : 4;    // Mask of available sort directions (1-bit each)
    HvkU8                    SortDirectionsAvailList;        // Ordered list of available sort directions (2-bits each, total 8-bits)

    HvkGuiTableColumn()
    {
        memset(this, 0, sizeof(*this));
        StretchWeight = WidthRequest = -1.0f;
        NameOffset = -1;
        DisplayOrder = IndexWithinEnabledSet = -1;
        PrevEnabledColumn = NextEnabledColumn = -1;
        SortOrder = -1;
        SortDirection = HvkGuiSortDirection_None;
        DrawChannelCurrent = DrawChannelFrozen = DrawChannelUnfrozen = (HvkU8)-1;
    }
};

// Transient cell data stored per row.
// sizeof() ~ 6 bytes
struct HvkGuiTableCellData
{
    HvkU32                       BgColor;    // Actual color
    HvkGuiTableColumnIdx         Column;     // Column number
};

// Parameters for TableAngledHeadersRowEx()
// This may end up being refactored for more general purpose.
// sizeof() ~ 12 bytes
struct HvkGuiTableHeaderData
{
    HvkGuiTableColumnIdx         Index;      // Column index
    HvkU32                       TextColor;
    HvkU32                       BgColor0;
    HvkU32                       BgColor1;
};

// Per-instance data that needs preserving across frames (seemingly most others do not need to be preserved aside from debug needs. Does that means they could be moved to HvkGuiTableTempData?)
// sizeof() ~ 24 bytes
struct HvkGuiTableInstanceData
{
    HvkGuiID                     TableInstanceID;
    float                       LastOuterHeight;            // Outer height from last frame
    float                       LastTopHeadersRowHeight;    // Height of first consecutive header rows from last frame (FIXME: this is used assuming consecutive headers are in same frozen set)
    float                       LastFrozenHeight;           // Height of frozen section from last frame
    int                         HoveredRowLast;             // Index of row which was hovered last frame.
    int                         HoveredRowNext;             // Index of row hovered this frame, set after encountering it.

    HvkGuiTableInstanceData()    { TableInstanceID = 0; LastOuterHeight = LastTopHeadersRowHeight = LastFrozenHeight = 0.0f; HoveredRowLast = HoveredRowNext = -1; }
};

// sizeof() ~ 592 bytes + heap allocs described in TableBeginInitMemory()
struct HvkGui_API HvkGuiTable
{
    HvkGuiID                     ID;
    HvkGuiTableFlags             Flags;
    void*                       RawData;                    // Single allocation to hold Columns[], DisplayOrderToIndex[], and RowCellData[]
    HvkGuiTableTempData*         TempData;                   // Transient data while table is active. Point within g.CurrentTableStack[]
    HvkSpan<HvkGuiTableColumn>    Columns;                    // Point within RawData[]
    HvkSpan<HvkGuiTableColumnIdx> DisplayOrderToIndex;        // Point within RawData[]. Store display order of columns (when not reordered, the values are 0...Count-1)
    HvkSpan<HvkGuiTableCellData>  RowCellData;                // Point within RawData[]. Store cells background requests for current row.
    HvkBitArrayPtr               EnabledMaskByDisplayOrder;  // Column DisplayOrder -> IsEnabled map
    HvkBitArrayPtr               EnabledMaskByIndex;         // Column Index -> IsEnabled map (== not hidden by user/api) in a format adequate for iterating column without touching cold data
    HvkBitArrayPtr               VisibleMaskByIndex;         // Column Index -> IsVisibleX|IsVisibleY map (== not hidden by user/api && not hidden by scrolling/cliprect)
    HvkGuiTableFlags             SettingsLoadedFlags;        // Which data were loaded from the .ini file (e.g. when order is not altered we won't save order)
    int                         SettingsOffset;             // Offset in g.SettingsTables
    int                         LastFrameActive;
    int                         ColumnsCount;               // Number of columns declared in BeginTable()
    int                         CurrentRow;
    int                         CurrentColumn;
    HvkS16                       InstanceCurrent;            // Count of BeginTable() calls with same ID in the same frame (generally 0). This is a little bit similar to BeginCount for a window, but multiple tables with the same ID are multiple tables, they are just synced.
    HvkS16                       InstanceInteracted;         // Mark which instance (generally 0) of the same ID is being interacted with
    float                       RowPosY1;
    float                       RowPosY2;
    float                       RowMinHeight;               // Height submitted to TableNextRow()
    float                       RowCellPaddingY;            // Top and bottom padding. Reloaded during row change.
    float                       RowTextBaseline;
    float                       RowIndentOffsetX;
    HvkGuiTableRowFlags          RowFlags : 16;              // Current row flags, see HvkGuiTableRowFlags_
    HvkGuiTableRowFlags          LastRowFlags : 16;
    int                         RowBgColorCounter;          // Counter for alternating background colors (can be fast-forwarded by e.g clipper), not same as CurrentRow because header rows typically don't increase this.
    HvkU32                       RowBgColor[2];              // Background color override for current row.
    HvkU32                       BorderColorStrong;
    HvkU32                       BorderColorLight;
    float                       BorderX1;
    float                       BorderX2;
    float                       HostIndentX;
    float                       MinColumnWidth;
    float                       OuterPaddingX;
    float                       CellPaddingX;               // Padding from each borders. Locked in BeginTable()/Layout.
    float                       CellSpacingX1;              // Spacing between non-bordered cells. Locked in BeginTable()/Layout.
    float                       CellSpacingX2;
    float                       InnerWidth;                 // User value passed to BeginTable(), see comments at the top of BeginTable() for details.
    float                       ColumnsGivenWidth;          // Sum of current column width
    float                       ColumnsAutoFitWidth;        // Sum of ideal column width in order nothing to be clipped, used for auto-fitting and content width submission in outer window
    float                       ColumnsStretchSumWeights;   // Sum of weight of all enabled stretching columns
    float                       ResizedColumnNextWidth;
    float                       ResizeLockMinContentsX2;    // Lock minimum contents width while resizing down in order to not create feedback loops. But we allow growing the table.
    float                       RefScale;                   // Reference scale to be able to rescale columns on font/dpi changes.
    float                       AngledHeadersHeight;        // Set by TableAngledHeadersRow(), used in TableUpdateLayout()
    float                       AngledHeadersSlope;         // Set by TableAngledHeadersRow(), used in TableUpdateLayout()
    HvkRect                      OuterRect;                  // Note: for non-scrolling table, OuterRect.Max.y is often FLT_MAX until EndTable(), unless a height has been specified in BeginTable().
    HvkRect                      InnerRect;                  // InnerRect but without decoration. As with OuterRect, for non-scrolling tables, InnerRect.Max.y is "
    HvkRect                      WorkRect;
    HvkRect                      InnerClipRect;
    HvkRect                      BgClipRect;                 // We use this to cpu-clip cell background color fill, evolve during the frame as we cross frozen rows boundaries
    HvkRect                      Bg0ClipRectForDrawCmd;      // Actual HvkDrawCmd clip rect for BG0/1 channel. This tends to be == OuterWindow->ClipRect at BeginTable() because output in BG0/BG1 is cpu-clipped
    HvkRect                      Bg2ClipRectForDrawCmd;      // Actual HvkDrawCmd clip rect for BG2 channel. This tends to be a correct, tight-fit, because output to BG2 are done by widgets relying on regular ClipRect.
    HvkRect                      HostClipRect;               // This is used to check if we can eventually merge our columns draw calls into the current draw call of the current window.
    HvkRect                      HostBackupInnerClipRect;    // Backup of InnerWindow->ClipRect during PushTableBackground()/PopTableBackground()
    HvkGuiWindow*                OuterWindow;                // Parent window for the table
    HvkGuiWindow*                InnerWindow;                // Window holding the table data (== OuterWindow or a child window)
    HvkGuiTextBuffer             ColumnsNames;               // Contiguous buffer holding columns names
    HvkDrawListSplitter*         DrawSplitter;               // Shortcut to TempData->DrawSplitter while in table. Isolate draw commands per columns to avoid switching clip rect constantly
    HvkGuiTableInstanceData      InstanceDataFirst;
    HvkVector<HvkGuiTableInstanceData>    InstanceDataExtra;  // FIXME-OPT: Using a small-vector pattern would be good.
    HvkGuiTableColumnSortSpecs   SortSpecsSingle;
    HvkVector<HvkGuiTableColumnSortSpecs> SortSpecsMulti;     // FIXME-OPT: Using a small-vector pattern would be good.
    HvkGuiTableSortSpecs         SortSpecs;                  // Public facing sorts specs, this is what we return in TableGetSortSpecs()
    HvkGuiTableColumnIdx         SortSpecsCount;
    HvkGuiTableColumnIdx         ColumnsEnabledCount;        // Number of enabled columns (<= ColumnsCount)
    HvkGuiTableColumnIdx         ColumnsEnabledFixedCount;   // Number of enabled columns using fixed width (<= ColumnsCount)
    HvkGuiTableColumnIdx         DeclColumnsCount;           // Count calls to TableSetupColumn()
    HvkGuiTableColumnIdx         AngledHeadersCount;         // Count columns with angled headers
    HvkGuiTableColumnIdx         HoveredColumnBody;          // Index of column whose visible region is being hovered. Hvkportant: == ColumnsCount when hovering empty region after the right-most column!
    HvkGuiTableColumnIdx         HoveredColumnBorder;        // Index of column whose right-border is being hovered (for resizing).
    HvkGuiTableColumnIdx         HighlightColumnHeader;      // Index of column which should be highlighted.
    HvkGuiTableColumnIdx         AutoFitSingleColumn;        // Index of single column requesting auto-fit.
    HvkGuiTableColumnIdx         ResizedColumn;              // Index of column being resized. Reset when InstanceCurrent==0.
    HvkGuiTableColumnIdx         LastResizedColumn;          // Index of column being resized from previous frame.
    HvkGuiTableColumnIdx         HeldHeaderColumn;           // Index of column header being held.
    HvkGuiTableColumnIdx         ReorderColumn;              // Index of column being reordered. (not cleared)
    HvkGuiTableColumnIdx         ReorderColumnDir;           // -1 or +1
    HvkGuiTableColumnIdx         LeftMostEnabledColumn;      // Index of left-most non-hidden column.
    HvkGuiTableColumnIdx         RightMostEnabledColumn;     // Index of right-most non-hidden column.
    HvkGuiTableColumnIdx         LeftMostStretchedColumn;    // Index of left-most stretched column.
    HvkGuiTableColumnIdx         RightMostStretchedColumn;   // Index of right-most stretched column.
    HvkGuiTableColumnIdx         ContextPopupColumn;         // Column right-clicked on, of -1 if opening context menu from a neutral/empty spot
    HvkGuiTableColumnIdx         FreezeRowsRequest;          // Requested frozen rows count
    HvkGuiTableColumnIdx         FreezeRowsCount;            // Actual frozen row count (== FreezeRowsRequest, or == 0 when no scrolling offset)
    HvkGuiTableColumnIdx         FreezeColumnsRequest;       // Requested frozen columns count
    HvkGuiTableColumnIdx         FreezeColumnsCount;         // Actual frozen columns count (== FreezeColumnsRequest, or == 0 when no scrolling offset)
    HvkGuiTableColumnIdx         RowCellDataCurrent;         // Index of current RowCellData[] entry in current row
    HvkGuiTableDrawChannelIdx    DummyDrawChannel;           // Redirect non-visible columns here.
    HvkGuiTableDrawChannelIdx    Bg2DrawChannelCurrent;      // For Selectable() and other widgets drawing across columns after the freezing line. Index within DrawSplitter.Channels[]
    HvkGuiTableDrawChannelIdx    Bg2DrawChannelUnfrozen;
    HvkS8                        NavLayer;                   // HvkGuiNavLayer at the time of BeginTable().
    bool                        IsLayoutLocked;             // Set by TableUpdateLayout() which is called when beginning the first row.
    bool                        IsInsideRow;                // Set when inside TableBeginRow()/TableEndRow().
    bool                        IsInitializing;
    bool                        IsSortSpecsDirty;
    bool                        IsUsingHeaders;             // Set when the first row had the HvkGuiTableRowFlags_Headers flag.
    bool                        IsContextPopupOpen;         // Set when default context menu is open (also see: ContextPopupColumn, InstanceInteracted).
    bool                        DisableDefaultContextMenu;  // Disable default context menu. You may submit your own using TableBeginContextMenuPopup()/EndPopup()
    bool                        IsSettingsRequestLoad;
    bool                        IsSettingsDirty;            // Set when table settings have changed and needs to be reported into HvkGuiTableSettings data.
    bool                        IsDefaultDisplayOrder;      // Set when display order is unchanged from default (DisplayOrder contains 0...Count-1)
    bool                        IsResetAllRequest;
    bool                        IsResetDisplayOrderRequest;
    bool                        IsUnfrozenRows;             // Set when we got past the frozen row.
    bool                        IsDefaultSizingPolicy;      // Set if user didn't explicitly set a sizing policy in BeginTable()
    bool                        IsActiveIdAliveBeforeTable;
    bool                        IsActiveIdInTable;
    bool                        HasScrollbarYCurr;          // Whether ANY instance of this table had a vertical scrollbar during the current frame.
    bool                        HasScrollbarYPrev;          // Whether ANY instance of this table had a vertical scrollbar during the previous.
    bool                        MemoryCompacted;
    bool                        HostSkipItems;              // Backup of InnerWindow->SkipItem at the end of BeginTable(), because we will overwrite InnerWindow->SkipItem on a per-column basis

    HvkGuiTable()                { memset(this, 0, sizeof(*this)); LastFrameActive = -1; }
    ~HvkGuiTable()               { Hvk_FREE(RawData); }
};

// Transient data that are only needed between BeginTable() and EndTable(), those buffers are shared (1 per level of stacked table).
// - Accessing those requires chasing an extra pointer so for very frequently used data we leave them in the main table structure.
// - We also leave out of this structure data that tend to be particularly useful for debugging/metrics.
// FIXME-TABLE: more transient data could be stored in a stacked HvkGuiTableTempData: e.g. SortSpecs.
// sizeof() ~ 136 bytes.
struct HvkGui_API HvkGuiTableTempData
{
    HvkGuiID                     WindowID;                   // Shortcut to g.Tables[TableIndex]->OuterWindow->ID.
    int                         TableIndex;                 // Index in g.Tables.Buf[] pool
    float                       LastTimeActive;             // Last timestamp this structure was used
    float                       AngledHeadersExtraWidth;    // Used in EndTable()
    HvkVector<HvkGuiTableHeaderData> AngledHeadersRequests;   // Used in TableAngledHeadersRow()

    HvkVec2                      UserOuterSize;              // outer_size.x passed to BeginTable()
    HvkDrawListSplitter          DrawSplitter;

    HvkRect                      HostBackupWorkRect;         // Backup of InnerWindow->WorkRect at the end of BeginTable()
    HvkRect                      HostBackupParentWorkRect;   // Backup of InnerWindow->ParentWorkRect at the end of BeginTable()
    HvkVec2                      HostBackupPrevLineSize;     // Backup of InnerWindow->DC.PrevLineSize at the end of BeginTable()
    HvkVec2                      HostBackupCurrLineSize;     // Backup of InnerWindow->DC.CurrLineSize at the end of BeginTable()
    HvkVec2                      HostBackupCursorMaxPos;     // Backup of InnerWindow->DC.CursorMaxPos at the end of BeginTable()
    HvkVec1                      HostBackupColumnsOffset;    // Backup of OuterWindow->DC.ColumnsOffset at the end of BeginTable()
    float                       HostBackupItemWidth;        // Backup of OuterWindow->DC.ItemWidth at the end of BeginTable()
    int                         HostBackupItemWidthStackSize;//Backup of OuterWindow->DC.ItemWidthStack.Size at the end of BeginTable()

    HvkGuiTableTempData()        { memset(this, 0, sizeof(*this)); LastTimeActive = -1.0f; }
};

// sizeof() ~ 16
struct HvkGuiTableColumnSettings
{
    float                   WidthOrWeight;
    HvkGuiID                 UserID;
    HvkGuiTableColumnIdx     Index;
    HvkGuiTableColumnIdx     DisplayOrder;
    HvkGuiTableColumnIdx     SortOrder;
    HvkU8                    SortDirection : 2;
    HvkS8                    IsEnabled : 2; // "Visible" in ini file
    HvkU8                    IsStretch : 1;

    HvkGuiTableColumnSettings()
    {
        WidthOrWeight = 0.0f;
        UserID = 0;
        Index = -1;
        DisplayOrder = SortOrder = -1;
        SortDirection = HvkGuiSortDirection_None;
        IsEnabled = -1;
        IsStretch = 0;
    }
};

// This is designed to be stored in a single HvkChunkStream (1 header followed by N HvkGuiTableColumnSettings, etc.)
struct HvkGuiTableSettings
{
    HvkGuiID                     ID;                     // Set to 0 to invalidate/delete the setting
    HvkGuiTableFlags             SaveFlags;              // Indicate data we want to save using the Resizable/Reorderable/Sortable/Hideable flags (could be using its own flags..)
    float                       RefScale;               // Reference scale to be able to rescale columns on font/dpi changes.
    HvkGuiTableColumnIdx         ColumnsCount;
    HvkGuiTableColumnIdx         ColumnsCountMax;        // Maximum number of columns this settings instance can store, we can recycle a settings instance with lower number of columns but not higher
    bool                        WantApply;              // Set when loaded from .ini data (to enable merging/loading .ini data into an already running context)

    HvkGuiTableSettings()        { memset(this, 0, sizeof(*this)); }
    HvkGuiTableColumnSettings*   GetColumnSettings()     { return (HvkGuiTableColumnSettings*)(this + 1); }
};

//-----------------------------------------------------------------------------
// [SECTION] HvkGui internal API
// No guarantee of forward compatibility here!
//-----------------------------------------------------------------------------

namespace HvkGui
{
    // Windows
    // We should always have a CurrentWindow in the stack (there is an Hvkplicit "Debug" window)
    // If this ever crashes because g.CurrentWindow is NULL, it means that either:
    // - HvkGui::NewFrame() has never been called, which is illegal.
    // - You are calling HvkGui functions after HvkGui::EndFrame()/HvkGui::Render() and before the next HvkGui::NewFrame(), which is also illegal.
    HvkGui_API HvkGuiIO&         GetIO(HvkGuiContext* ctx);
    HvkGui_API HvkGuiPlatformIO& GetPlatformIO(HvkGuiContext* ctx);
    inline    HvkGuiWindow*  GetCurrentWindowRead()      { HvkGuiContext& g = *GHvkGui; return g.CurrentWindow; }
    inline    HvkGuiWindow*  GetCurrentWindow()          { HvkGuiContext& g = *GHvkGui; g.CurrentWindow->WriteAccessed = true; return g.CurrentWindow; }
    HvkGui_API HvkGuiWindow*  FindWindowByID(HvkGuiID id);
    HvkGui_API HvkGuiWindow*  FindWindowByName(const char* name);
    HvkGui_API void          UpdateWindowParentAndRootLinks(HvkGuiWindow* window, HvkGuiWindowFlags flags, HvkGuiWindow* parent_window);
    HvkGui_API void          UpdateWindowSkipRefresh(HvkGuiWindow* window);
    HvkGui_API HvkVec2        CalcWindowNextAutoFitSize(HvkGuiWindow* window);
    HvkGui_API bool          IsWindowChildOf(HvkGuiWindow* window, HvkGuiWindow* potential_parent, bool popup_hierarchy);
    HvkGui_API bool          IsWindowInBeginStack(HvkGuiWindow* window);
    HvkGui_API bool          IsWindowWithinBeginStackOf(HvkGuiWindow* window, HvkGuiWindow* potential_parent);
    HvkGui_API bool          IsWindowAbove(HvkGuiWindow* potential_above, HvkGuiWindow* potential_below);
    HvkGui_API bool          IsWindowNavFocusable(HvkGuiWindow* window);
    HvkGui_API void          SetWindowPos(HvkGuiWindow* window, const HvkVec2& pos, HvkGuiCond cond = 0);
    HvkGui_API void          SetWindowSize(HvkGuiWindow* window, const HvkVec2& size, HvkGuiCond cond = 0);
    HvkGui_API void          SetWindowCollapsed(HvkGuiWindow* window, bool collapsed, HvkGuiCond cond = 0);
    HvkGui_API void          SetWindowHitTestHole(HvkGuiWindow* window, const HvkVec2& pos, const HvkVec2& size);
    HvkGui_API void          SetWindowHiddenAndSkipItemsForCurrentFrame(HvkGuiWindow* window);
    inline void             SetWindowParentWindowForFocusRoute(HvkGuiWindow* window, HvkGuiWindow* parent_window) { window->ParentWindowForFocusRoute = parent_window; }
    inline HvkRect           WindowRectAbsToRel(HvkGuiWindow* window, const HvkRect& r) { HvkVec2 off = window->DC.CursorStartPos; return HvkRect(r.Min.x - off.x, r.Min.y - off.y, r.Max.x - off.x, r.Max.y - off.y); }
    inline HvkRect           WindowRectRelToAbs(HvkGuiWindow* window, const HvkRect& r) { HvkVec2 off = window->DC.CursorStartPos; return HvkRect(r.Min.x + off.x, r.Min.y + off.y, r.Max.x + off.x, r.Max.y + off.y); }
    inline HvkVec2           WindowPosAbsToRel(HvkGuiWindow* window, const HvkVec2& p)  { HvkVec2 off = window->DC.CursorStartPos; return HvkVec2(p.x - off.x, p.y - off.y); }
    inline HvkVec2           WindowPosRelToAbs(HvkGuiWindow* window, const HvkVec2& p)  { HvkVec2 off = window->DC.CursorStartPos; return HvkVec2(p.x + off.x, p.y + off.y); }

    // Windows: Display Order and Focus Order
    HvkGui_API void          FocusWindow(HvkGuiWindow* window, HvkGuiFocusRequestFlags flags = 0);
    HvkGui_API void          FocusTopMostWindowUnderOne(HvkGuiWindow* under_this_window, HvkGuiWindow* ignore_window, HvkGuiViewport* filter_viewport, HvkGuiFocusRequestFlags flags);
    HvkGui_API void          BringWindowToFocusFront(HvkGuiWindow* window);
    HvkGui_API void          BringWindowToDisplayFront(HvkGuiWindow* window);
    HvkGui_API void          BringWindowToDisplayBack(HvkGuiWindow* window);
    HvkGui_API void          BringWindowToDisplayBehind(HvkGuiWindow* window, HvkGuiWindow* above_window);
    HvkGui_API int           FindWindowDisplayIndex(HvkGuiWindow* window);
    HvkGui_API HvkGuiWindow*  FindBottomMostVisibleWindowWithinBeginStack(HvkGuiWindow* window);

    // Windows: Idle, Refresh Policies [EXPERIMENTAL]
    HvkGui_API void          SetNextWindowRefreshPolicy(HvkGuiWindowRefreshFlags flags);

    // Fonts, drawing
    HvkGui_API void          RegisterUserTexture(HvkTextureData* tex); // Register external texture. EXPERIMENTAL: DO NOT USE YET.
    HvkGui_API void          UnregisterUserTexture(HvkTextureData* tex);
    HvkGui_API void          RegisterFontAtlas(HvkFontAtlas* atlas);
    HvkGui_API void          UnregisterFontAtlas(HvkFontAtlas* atlas);
    HvkGui_API void          SetCurrentFont(HvkFont* font, float font_size_before_scaling, float font_size_after_scaling);
    HvkGui_API void          UpdateCurrentFontSize(float restore_font_size_after_scaling);
    HvkGui_API void          SetFontRasterizerDensity(float rasterizer_density);
    inline float            GetFontRasterizerDensity() { return GHvkGui->FontRasterizerDensity; }
    inline float            GetRoundedFontSize(float size) { return Hvk_ROUND(size); }
    HvkGui_API HvkFont*       GetDefaultFont();
    HvkGui_API void          PushPasswordFont();
    HvkGui_API void          PopPasswordFont();
    inline HvkDrawList*      GetForegroundDrawList(HvkGuiWindow* window) { Hvk_UNUSED(window); return GetForegroundDrawList(); } // This seemingly unnecessary wrapper simplifies compatibility between the 'master' and 'docking' branches.
    HvkGui_API HvkDrawList*   GetBackgroundDrawList(HvkGuiViewport* viewport);                     // get background draw list for the given viewport. this draw list will be the first rendering one. Useful to quickly draw shapes/text behind dear HvkGui contents.
    HvkGui_API HvkDrawList*   GetForegroundDrawList(HvkGuiViewport* viewport);                     // get foreground draw list for the given viewport. this draw list will be the last rendered one. Useful to quickly draw shapes/text over dear HvkGui contents.
    HvkGui_API void          AddDrawListToDrawDataEx(HvkDrawData* draw_data, HvkVector<HvkDrawList*>* out_list, HvkDrawList* draw_list);

    // Init
    HvkGui_API void          Initialize();
    HvkGui_API void          Shutdown();    // Since 1.60 this is a _private_ function. You can call DestroyContext() to destroy the context created by CreateContext().

    // Context name & generic context hooks
    HvkGui_API void          SetContextName(HvkGuiContext* ctx, const char* name);
    HvkGui_API HvkGuiID       AddContextHook(HvkGuiContext* ctx, const HvkGuiContextHook* hook);
    HvkGui_API void          RemoveContextHook(HvkGuiContext* ctx, HvkGuiID hook_to_remove);
    HvkGui_API void          CallContextHooks(HvkGuiContext* ctx, HvkGuiContextHookType type);

    // NewFrame
    HvkGui_API void          UpdateInputEvents(bool trickle_fast_inputs);
    HvkGui_API void          UpdateHoveredWindowAndCaptureFlags(const HvkVec2& mouse_pos);
    HvkGui_API void          FindHoveredWindowEx(const HvkVec2& pos, bool find_first_and_in_any_viewport, HvkGuiWindow** out_hovered_window, HvkGuiWindow** out_hovered_window_under_moving_window);
    HvkGui_API void          StartMouseMovingWindow(HvkGuiWindow* window);
    HvkGui_API void          StopMouseMovingWindow();
    HvkGui_API void          UpdateMouseMovingWindowNewFrame();
    HvkGui_API void          UpdateMouseMovingWindowEndFrame();

    // Viewports
    HvkGui_API void          ScaleWindowsInViewport(HvkGuiViewportP* viewport, float scale);
    HvkGui_API void          SetWindowViewport(HvkGuiWindow* window, HvkGuiViewportP* viewport);

    // Settings
    HvkGui_API void                  MarkIniSettingsDirty();
    HvkGui_API void                  MarkIniSettingsDirty(HvkGuiWindow* window);
    HvkGui_API void                  ClearIniSettings();
    HvkGui_API void                  AddSettingsHandler(const HvkGuiSettingsHandler* handler);
    HvkGui_API void                  RemoveSettingsHandler(const char* type_name);
    HvkGui_API HvkGuiSettingsHandler* FindSettingsHandler(const char* type_name);

    // Settings - Windows
    HvkGui_API HvkGuiWindowSettings*  CreateNewWindowSettings(const char* name);
    HvkGui_API HvkGuiWindowSettings*  FindWindowSettingsByID(HvkGuiID id);
    HvkGui_API HvkGuiWindowSettings*  FindWindowSettingsByWindow(HvkGuiWindow* window);
    HvkGui_API void                  ClearWindowSettings(const char* name);

    // Localization
    HvkGui_API void          LocalizeRegisterEntries(const HvkGuiLocEntry* entries, int count);
    inline const char*      LocalizeGetMsg(HvkGuiLocKey key) { HvkGuiContext& g = *GHvkGui; const char* msg = g.LocalizationTable[key]; return msg ? msg : "*Missing Text*"; }

    // Scrolling
    HvkGui_API void          SetScrollX(HvkGuiWindow* window, float scroll_x);
    HvkGui_API void          SetScrollY(HvkGuiWindow* window, float scroll_y);
    HvkGui_API void          SetScrollFromPosX(HvkGuiWindow* window, float local_x, float center_x_ratio);
    HvkGui_API void          SetScrollFromPosY(HvkGuiWindow* window, float local_y, float center_y_ratio);

    // Early work-in-progress API (ScrollToItem() will become public)
    HvkGui_API void          ScrollToItem(HvkGuiScrollFlags flags = 0);
    HvkGui_API void          ScrollToRect(HvkGuiWindow* window, const HvkRect& rect, HvkGuiScrollFlags flags = 0);
    HvkGui_API HvkVec2        ScrollToRectEx(HvkGuiWindow* window, const HvkRect& rect, HvkGuiScrollFlags flags = 0);
//#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    inline void             ScrollToBringRectIntoView(HvkGuiWindow* window, const HvkRect& rect) { ScrollToRect(window, rect, HvkGuiScrollFlags_KeepVisibleEdgeY); }
//#endif

    // Basic Accessors
    inline HvkGuiItemStatusFlags GetItemStatusFlags() { HvkGuiContext& g = *GHvkGui; return g.LastItemData.StatusFlags; }
    inline HvkGuiItemFlags   GetItemFlags()  { HvkGuiContext& g = *GHvkGui; return g.LastItemData.ItemFlags; }
    inline HvkGuiID          GetActiveID()   { HvkGuiContext& g = *GHvkGui; return g.ActiveId; }
    inline HvkGuiID          GetFocusID()    { HvkGuiContext& g = *GHvkGui; return g.NavId; }
    HvkGui_API void          SetActiveID(HvkGuiID id, HvkGuiWindow* window);
    HvkGui_API void          SetFocusID(HvkGuiID id, HvkGuiWindow* window);
    HvkGui_API void          ClearActiveID();
    HvkGui_API HvkGuiID       GetHoveredID();
    HvkGui_API void          SetHoveredID(HvkGuiID id);
    HvkGui_API void          KeepAliveID(HvkGuiID id);
    HvkGui_API void          MarkItemEdited(HvkGuiID id);     // Mark data associated to given item as "edited", used by IsItemDeactivatedAfterEdit() function.
    HvkGui_API void          PushOverrideID(HvkGuiID id);     // Push given value as-is at the top of the ID stack (whereas PushID combines old and new hashes)
    HvkGui_API HvkGuiID       GetIDWithSeed(const char* str_id_begin, const char* str_id_end, HvkGuiID seed);
    HvkGui_API HvkGuiID       GetIDWithSeed(int n, HvkGuiID seed);

    // Basic Helpers for widget code
    HvkGui_API void          ItemSize(const HvkVec2& size, float text_baseline_y = -1.0f);
    inline void             ItemSize(const HvkRect& bb, float text_baseline_y = -1.0f) { ItemSize(bb.GetSize(), text_baseline_y); } // FIXME: This is a misleading API since we expect CursorPos to be bb.Min.
    HvkGui_API bool          ItemAdd(const HvkRect& bb, HvkGuiID id, const HvkRect* nav_bb = NULL, HvkGuiItemFlags extra_flags = 0);
    HvkGui_API bool          ItemHoverable(const HvkRect& bb, HvkGuiID id, HvkGuiItemFlags item_flags);
    HvkGui_API bool          IsWindowContentHoverable(HvkGuiWindow* window, HvkGuiHoveredFlags flags = 0);
    HvkGui_API bool          IsClippedEx(const HvkRect& bb, HvkGuiID id);
    HvkGui_API void          SetLastItemData(HvkGuiID item_id, HvkGuiItemFlags item_flags, HvkGuiItemStatusFlags status_flags, const HvkRect& item_rect);
    HvkGui_API HvkVec2        CalcItemSize(HvkVec2 size, float default_w, float default_h);
    HvkGui_API float         CalcWrapWidthForPos(const HvkVec2& pos, float wrap_pos_x);
    HvkGui_API void          PushMultiItemsWidths(int components, float width_full);
    HvkGui_API void          ShrinkWidths(HvkGuiShrinkWidthItem* items, int count, float width_excess, float width_min);
    HvkGui_API void          CalcClipRectVisibleItemsY(const HvkRect& clip_rect, const HvkVec2& pos, float items_height, int* out_visible_start, int* out_visible_end);

    // Parameter stacks (shared)
    HvkGui_API const HvkGuiStyleVarInfo* GetStyleVarInfo(HvkGuiStyleVar idx);
    HvkGui_API void          BeginDisabledOverrideReenable();
    HvkGui_API void          EndDisabledOverrideReenable();

    // Logging/Capture
    HvkGui_API void          LogBegin(HvkGuiLogFlags flags, int auto_open_depth);         // -> BeginCapture() when we design v2 api, for now stay under the radar by using the old name.
    HvkGui_API void          LogToBuffer(int auto_open_depth = -1);                      // Start logging/capturing to internal buffer
    HvkGui_API void          LogRenderedText(const HvkVec2* ref_pos, const char* text, const char* text_end = NULL);
    HvkGui_API void          LogSetNextTextDecoration(const char* prefix, const char* suffix);

    // Childs
    HvkGui_API bool          BeginChildEx(const char* name, HvkGuiID id, const HvkVec2& size_arg, HvkGuiChildFlags child_flags, HvkGuiWindowFlags window_flags);

    // Popups, Modals
    HvkGui_API bool          BeginPopupEx(HvkGuiID id, HvkGuiWindowFlags extra_window_flags);
    HvkGui_API bool          BeginPopupMenuEx(HvkGuiID id, const char* label, HvkGuiWindowFlags extra_window_flags);
    HvkGui_API void          OpenPopupEx(HvkGuiID id, HvkGuiPopupFlags popup_flags = HvkGuiPopupFlags_None);
    HvkGui_API void          ClosePopupToLevel(int remaining, bool restore_focus_to_window_under_popup);
    HvkGui_API void          ClosePopupsOverWindow(HvkGuiWindow* ref_window, bool restore_focus_to_window_under_popup);
    HvkGui_API void          ClosePopupsExceptModals();
    HvkGui_API bool          IsPopupOpen(HvkGuiID id, HvkGuiPopupFlags popup_flags);
    HvkGui_API HvkRect        GetPopupAllowedExtentRect(HvkGuiWindow* window);
    HvkGui_API HvkGuiWindow*  GetTopMostPopupModal();
    HvkGui_API HvkGuiWindow*  GetTopMostAndVisiblePopupModal();
    HvkGui_API HvkGuiWindow*  FindBlockingModal(HvkGuiWindow* window);
    HvkGui_API HvkVec2        FindBestWindowPosForPopup(HvkGuiWindow* window);
    HvkGui_API HvkVec2        FindBestWindowPosForPopupEx(const HvkVec2& ref_pos, const HvkVec2& size, HvkGuiDir* last_dir, const HvkRect& r_outer, const HvkRect& r_avoid, HvkGuiPopupPositionPolicy policy);

    // Tooltips
    HvkGui_API bool          BeginTooltipEx(HvkGuiTooltipFlags tooltip_flags, HvkGuiWindowFlags extra_window_flags);
    HvkGui_API bool          BeginTooltipHidden();

    // Menus
    HvkGui_API bool          BeginViewportSideBar(const char* name, HvkGuiViewport* viewport, HvkGuiDir dir, float size, HvkGuiWindowFlags window_flags);
    HvkGui_API bool          BeginMenuEx(const char* label, const char* icon, bool enabled = true);
    HvkGui_API bool          MenuItemEx(const char* label, const char* icon, const char* shortcut = NULL, bool selected = false, bool enabled = true);

    // Combos
    HvkGui_API bool          BeginComboPopup(HvkGuiID popup_id, const HvkRect& bb, HvkGuiComboFlags flags);
    HvkGui_API bool          BeginComboPreview();
    HvkGui_API void          EndComboPreview();

    // Keyboard/Gamepad Navigation
    HvkGui_API void          NavInitWindow(HvkGuiWindow* window, bool force_reinit);
    HvkGui_API void          NavInitRequestApplyResult();
    HvkGui_API bool          NavMoveRequestButNoResultYet();
    HvkGui_API void          NavMoveRequestSubmit(HvkGuiDir move_dir, HvkGuiDir clip_dir, HvkGuiNavMoveFlags move_flags, HvkGuiScrollFlags scroll_flags);
    HvkGui_API void          NavMoveRequestForward(HvkGuiDir move_dir, HvkGuiDir clip_dir, HvkGuiNavMoveFlags move_flags, HvkGuiScrollFlags scroll_flags);
    HvkGui_API void          NavMoveRequestResolveWithLastItem(HvkGuiNavItemData* result);
    HvkGui_API void          NavMoveRequestResolveWithPastTreeNode(HvkGuiNavItemData* result, const HvkGuiTreeNodeStackData* tree_node_data);
    HvkGui_API void          NavMoveRequestCancel();
    HvkGui_API void          NavMoveRequestApplyResult();
    HvkGui_API void          NavMoveRequestTryWrapping(HvkGuiWindow* window, HvkGuiNavMoveFlags move_flags);
    HvkGui_API void          NavHighlightActivated(HvkGuiID id);
    HvkGui_API void          NavClearPreferredPosForAxis(HvkGuiAxis axis);
    HvkGui_API void          SetNavCursorVisibleAfterMove();
    HvkGui_API void          NavUpdateCurrentWindowIsScrollPushableX();
    HvkGui_API void          SetNavWindow(HvkGuiWindow* window);
    HvkGui_API void          SetNavID(HvkGuiID id, HvkGuiNavLayer nav_layer, HvkGuiID focus_scope_id, const HvkRect& rect_rel);
    HvkGui_API void          SetNavFocusScope(HvkGuiID focus_scope_id);

    // Focus/Activation
    // This should be part of a larger set of API: FocusItem(offset = -1), FocusItemByID(id), ActivateItem(offset = -1), ActivateItemByID(id) etc. which are
    // much harder to design and Hvkplement than expected. I have a couple of private branches on this matter but it's not simple. For now Hvkplementing the easy ones.
    HvkGui_API void          FocusItem();                    // Focus last item (no selection/activation).
    HvkGui_API void          ActivateItemByID(HvkGuiID id);   // Activate an item by ID (button, checkbox, tree node etc.). Activation is queued and processed on the next frame when the item is encountered again. Was called 'ActivateItem()' before 1.89.7.

    // Inputs
    // FIXME: Eventually we should aim to move e.g. IsActiveIdUsingKey() into IsKeyXXX functions.
    inline bool             IsNamedKey(HvkGuiKey key)                    { return key >= HvkGuiKey_NamedKey_BEGIN && key < HvkGuiKey_NamedKey_END; }
    inline bool             IsNamedKeyOrMod(HvkGuiKey key)               { return (key >= HvkGuiKey_NamedKey_BEGIN && key < HvkGuiKey_NamedKey_END) || key == HvkGuiMod_Ctrl || key == HvkGuiMod_Shift || key == HvkGuiMod_Alt || key == HvkGuiMod_Super; }
    inline bool             IsLegacyKey(HvkGuiKey key)                   { return key >= HvkGuiKey_LegacyNativeKey_BEGIN && key < HvkGuiKey_LegacyNativeKey_END; }
    inline bool             IsKeyboardKey(HvkGuiKey key)                 { return key >= HvkGuiKey_Keyboard_BEGIN && key < HvkGuiKey_Keyboard_END; }
    inline bool             IsGamepadKey(HvkGuiKey key)                  { return key >= HvkGuiKey_Gamepad_BEGIN && key < HvkGuiKey_Gamepad_END; }
    inline bool             IsMouseKey(HvkGuiKey key)                    { return key >= HvkGuiKey_Mouse_BEGIN && key < HvkGuiKey_Mouse_END; }
    inline bool             IsAliasKey(HvkGuiKey key)                    { return key >= HvkGuiKey_Aliases_BEGIN && key < HvkGuiKey_Aliases_END; }
    inline bool             IsLRModKey(HvkGuiKey key)                    { return key >= HvkGuiKey_LeftCtrl && key <= HvkGuiKey_RightSuper; }
    HvkGuiKeyChord           FixupKeyChord(HvkGuiKeyChord key_chord);
    inline HvkGuiKey         ConvertSingleModFlagToKey(HvkGuiKey key)
    {
        if (key == HvkGuiMod_Ctrl) return HvkGuiKey_ReservedForModCtrl;
        if (key == HvkGuiMod_Shift) return HvkGuiKey_ReservedForModShift;
        if (key == HvkGuiMod_Alt) return HvkGuiKey_ReservedForModAlt;
        if (key == HvkGuiMod_Super) return HvkGuiKey_ReservedForModSuper;
        return key;
    }

    HvkGui_API HvkGuiKeyData* GetKeyData(HvkGuiContext* ctx, HvkGuiKey key);
    inline HvkGuiKeyData*    GetKeyData(HvkGuiKey key)                                    { HvkGuiContext& g = *GHvkGui; return GetKeyData(&g, key); }
    HvkGui_API const char*   GetKeyChordName(HvkGuiKeyChord key_chord);
    inline HvkGuiKey         MouseButtonToKey(HvkGuiMouseButton button)                   { Hvk_ASSERT(button >= 0 && button < HvkGuiMouseButton_COUNT); return (HvkGuiKey)(HvkGuiKey_MouseLeft + button); }
    HvkGui_API bool          IsMouseDragPastThreshold(HvkGuiMouseButton button, float lock_threshold = -1.0f);
    HvkGui_API HvkVec2        GetKeyMagnitude2d(HvkGuiKey key_left, HvkGuiKey key_right, HvkGuiKey key_up, HvkGuiKey key_down);
    HvkGui_API float         GetNavTweakPressedAmount(HvkGuiAxis axis);
    HvkGui_API int           CalcTypematicRepeatAmount(float t0, float t1, float repeat_delay, float repeat_rate);
    HvkGui_API void          GetTypematicRepeatRate(HvkGuiInputFlags flags, float* repeat_delay, float* repeat_rate);
    HvkGui_API void          TeleportMousePos(const HvkVec2& pos);
    HvkGui_API void          SetActiveIdUsingAllKeyboardKeys();
    inline bool             IsActiveIdUsingNavDir(HvkGuiDir dir)                         { HvkGuiContext& g = *GHvkGui; return (g.ActiveIdUsingNavDirMask & (1 << dir)) != 0; }

    // [EXPERIMENTAL] Low-Level: Key/Input Ownership
    // - The idea is that instead of "eating" a given input, we can link to an owner id.
    // - Ownership is most often claimed as a result of reacting to a press/down event (but occasionally may be claimed ahead).
    // - Input queries can then read input by specifying HvkGuiKeyOwner_Any (== 0), HvkGuiKeyOwner_NoOwner (== -1) or a custom ID.
    // - Legacy input queries (without specifying an owner or _Any or _None) are equivalent to using HvkGuiKeyOwner_Any (== 0).
    // - Input ownership is automatically released on the frame after a key is released. Therefore:
    //   - for ownership registration happening as a result of a down/press event, the SetKeyOwner() call may be done once (common case).
    //   - for ownership registration happening ahead of a down/press event, the SetKeyOwner() call needs to be made every frame (happens if e.g. claiming ownership on hover).
    // - SetItemKeyOwner() is a shortcut for common simple case. A custom widget will probably want to call SetKeyOwner() multiple times directly based on its interaction state.
    // - This is marked experimental because not all widgets are fully honoring the Set/Test idioms. We will need to move forward step by step.
    //   Please open a GitHub Issue to submit your usage scenario or if there's a use case you need solved.
    HvkGui_API HvkGuiID       GetKeyOwner(HvkGuiKey key);
    HvkGui_API void          SetKeyOwner(HvkGuiKey key, HvkGuiID owner_id, HvkGuiInputFlags flags = 0);
    HvkGui_API void          SetKeyOwnersForKeyChord(HvkGuiKeyChord key, HvkGuiID owner_id, HvkGuiInputFlags flags = 0);
    HvkGui_API void          SetItemKeyOwner(HvkGuiKey key, HvkGuiInputFlags flags);       // Set key owner to last item if it is hovered or active. Equivalent to 'if (IsItemHovered() || IsItemActive()) { SetKeyOwner(key, GetItemID());'.
    HvkGui_API bool          TestKeyOwner(HvkGuiKey key, HvkGuiID owner_id);               // Test that key is either not owned, either owned by 'owner_id'
    inline HvkGuiKeyOwnerData* GetKeyOwnerData(HvkGuiContext* ctx, HvkGuiKey key)          { if (key & HvkGuiMod_Mask_) key = ConvertSingleModFlagToKey(key); Hvk_ASSERT(IsNamedKey(key)); return &ctx->KeysOwnerData[key - HvkGuiKey_NamedKey_BEGIN]; }

    // [EXPERIMENTAL] High-Level: Input Access functions w/ support for Key/Input Ownership
    // - Hvkportant: legacy IsKeyPressed(HvkGuiKey, bool repeat=true) _DEFAULTS_ to repeat, new IsKeyPressed() requires _EXPLICIT_ HvkGuiInputFlags_Repeat flag.
    // - Expected to be later promoted to public API, the prototypes are designed to replace existing ones (since owner_id can default to Any == 0)
    // - Specifying a value for 'HvkGuiID owner' will test that EITHER the key is NOT owned (UNLESS locked), EITHER the key is owned by 'owner'.
    //   Legacy functions use HvkGuiKeyOwner_Any meaning that they typically ignore ownership, unless a call to SetKeyOwner() explicitly used HvkGuiInputFlags_LockThisFrame or HvkGuiInputFlags_LockUntilRelease.
    // - Binding generators may want to ignore those for now, or suffix them with Ex() until we decide if this gets moved into public API.
    HvkGui_API bool          IsKeyDown(HvkGuiKey key, HvkGuiID owner_id);
    HvkGui_API bool          IsKeyPressed(HvkGuiKey key, HvkGuiInputFlags flags, HvkGuiID owner_id = 0);    // Hvkportant: when transitioning from old to new IsKeyPressed(): old API has "bool repeat = true", so would default to repeat. New API requires explicit HvkGuiInputFlags_Repeat.
    HvkGui_API bool          IsKeyReleased(HvkGuiKey key, HvkGuiID owner_id);
    HvkGui_API bool          IsKeyChordPressed(HvkGuiKeyChord key_chord, HvkGuiInputFlags flags, HvkGuiID owner_id = 0);
    HvkGui_API bool          IsMouseDown(HvkGuiMouseButton button, HvkGuiID owner_id);
    HvkGui_API bool          IsMouseClicked(HvkGuiMouseButton button, HvkGuiInputFlags flags, HvkGuiID owner_id = 0);
    HvkGui_API bool          IsMouseReleased(HvkGuiMouseButton button, HvkGuiID owner_id);
    HvkGui_API bool          IsMouseDoubleClicked(HvkGuiMouseButton button, HvkGuiID owner_id);

    // Shortcut Testing & Routing
    // - Set Shortcut() and SetNextItemShortcut() in HvkGui.h
    // - When a policy (except for HvkGuiInputFlags_RouteAlways *) is set, Shortcut() will register itself with SetShortcutRouting(),
    //   allowing the system to decide where to route the input among other route-aware calls.
    //   (* using HvkGuiInputFlags_RouteAlways is roughly equivalent to calling IsKeyChordPressed(key) and bypassing route registration and check)
    // - When using one of the routing option:
    //   - The default route is HvkGuiInputFlags_RouteFocused (accept inputs if window is in focus stack. Deep-most focused window takes inputs. ActiveId takes inputs over deep-most focused window.)
    //   - Routes are requested given a chord (key + modifiers) and a routing policy.
    //   - Routes are resolved during NewFrame(): if keyboard modifiers are matching current ones: SetKeyOwner() is called + route is granted for the frame.
    //   - Each route may be granted to a single owner. When multiple requests are made we have policies to select the winning route (e.g. deep most window).
    //   - Multiple read sites may use the same owner id can all access the granted route.
    //   - When owner_id is 0 we use the current Focus Scope ID as a owner ID in order to identify our location.
    // - You can chain two unrelated windows in the focus stack using SetWindowParentWindowForFocusRoute()
    //   e.g. if you have a tool window associated to a document, and you want document shortcuts to run when the tool is focused.
    HvkGui_API bool          Shortcut(HvkGuiKeyChord key_chord, HvkGuiInputFlags flags, HvkGuiID owner_id);
    HvkGui_API bool          SetShortcutRouting(HvkGuiKeyChord key_chord, HvkGuiInputFlags flags, HvkGuiID owner_id); // owner_id needs to be explicit and cannot be 0
    HvkGui_API bool          TestShortcutRouting(HvkGuiKeyChord key_chord, HvkGuiID owner_id);
    HvkGui_API HvkGuiKeyRoutingData* GetShortcutRoutingData(HvkGuiKeyChord key_chord);

    // [EXPERIMENTAL] Focus Scope
    // This is generally used to identify a unique input location (for e.g. a selection set)
    // There is one per window (automatically set in Begin), but:
    // - Selection patterns generally need to react (e.g. clear a selection) when landing on one item of the set.
    //   So in order to identify a set multiple lists in same window may each need a focus scope.
    //   If you Hvkagine an hypothetical BeginSelectionGroup()/EndSelectionGroup() api, it would likely call PushFocusScope()/EndFocusScope()
    // - Shortcut routing also use focus scope as a default location identifier if an owner is not provided.
    // We don't use the ID Stack for this as it is common to want them separate.
    HvkGui_API void          PushFocusScope(HvkGuiID id);
    HvkGui_API void          PopFocusScope();
    inline HvkGuiID          GetCurrentFocusScope() { HvkGuiContext& g = *GHvkGui; return g.CurrentFocusScopeId; }   // Focus scope we are outputting into, set by PushFocusScope()

    // Drag and Drop
    HvkGui_API bool          IsDragDropActive();
    HvkGui_API bool          BeginDragDropTargetCustom(const HvkRect& bb, HvkGuiID id);
    HvkGui_API bool          BeginDragDropTargetViewport(HvkGuiViewport* viewport, const HvkRect* p_bb = NULL);
    HvkGui_API void          ClearDragDrop();
    HvkGui_API bool          IsDragDropPayloadBeingAccepted();
    HvkGui_API void          RenderDragDropTargetRectForItem(const HvkRect& bb);
    HvkGui_API void          RenderDragDropTargetRectEx(HvkDrawList* draw_list, const HvkRect& bb);

    // Typing-Select API
    // (provide Windows Explorer style "select items by typing partial name" + "cycle through items by typing same letter" feature)
    // (this is currently not documented nor used by main library, but should work. See "widgets_typingselect" in HvkGui_test_suite for usage code. Please let us know if you use this!)
    HvkGui_API HvkGuiTypingSelectRequest* GetTypingSelectRequest(HvkGuiTypingSelectFlags flags = HvkGuiTypingSelectFlags_None);
    HvkGui_API int           TypingSelectFindMatch(HvkGuiTypingSelectRequest* req, int items_count, const char* (*get_item_name_func)(void*, int), void* user_data, int nav_item_idx);
    HvkGui_API int           TypingSelectFindNextSingleCharMatch(HvkGuiTypingSelectRequest* req, int items_count, const char* (*get_item_name_func)(void*, int), void* user_data, int nav_item_idx);
    HvkGui_API int           TypingSelectFindBestLeadingMatch(HvkGuiTypingSelectRequest* req, int items_count, const char* (*get_item_name_func)(void*, int), void* user_data);

    // Box-Select API
    HvkGui_API bool          BeginBoxSelect(const HvkRect& scope_rect, HvkGuiWindow* window, HvkGuiID box_select_id, HvkGuiMultiSelectFlags ms_flags);
    HvkGui_API void          EndBoxSelect(const HvkRect& scope_rect, HvkGuiMultiSelectFlags ms_flags);

    // Multi-Select API
    HvkGui_API void          MultiSelectItemHeader(HvkGuiID id, bool* p_selected, HvkGuiButtonFlags* p_button_flags);
    HvkGui_API void          MultiSelectItemFooter(HvkGuiID id, bool* p_selected, bool* p_pressed);
    HvkGui_API void          MultiSelectAddSetAll(HvkGuiMultiSelectTempData* ms, bool selected);
    HvkGui_API void          MultiSelectAddSetRange(HvkGuiMultiSelectTempData* ms, bool selected, int range_dir, HvkGuiSelectionUserData first_item, HvkGuiSelectionUserData last_item);
    inline HvkGuiBoxSelectState*     GetBoxSelectState(HvkGuiID id)   { HvkGuiContext& g = *GHvkGui; return (id != 0 && g.BoxSelectState.ID == id && g.BoxSelectState.IsActive) ? &g.BoxSelectState : NULL; }
    inline HvkGuiMultiSelectState*   GetMultiSelectState(HvkGuiID id) { HvkGuiContext& g = *GHvkGui; return g.MultiSelectStorage.GetByKey(id); }

    // Internal Columns API (this is not exposed because we will encourage transitioning to the Tables API)
    HvkGui_API void          SetWindowClipRectBeforeSetChannel(HvkGuiWindow* window, const HvkRect& clip_rect);
    HvkGui_API void          BeginColumns(const char* str_id, int count, HvkGuiOldColumnFlags flags = 0); // setup number of columns. use an identifier to distinguish multiple column sets. close with EndColumns().
    HvkGui_API void          EndColumns();                                                               // close columns
    HvkGui_API void          PushColumnClipRect(int column_index);
    HvkGui_API void          PushColumnsBackground();
    HvkGui_API void          PopColumnsBackground();
    HvkGui_API HvkGuiID       GetColumnsID(const char* str_id, int count);
    HvkGui_API HvkGuiOldColumns* FindOrCreateColumns(HvkGuiWindow* window, HvkGuiID id);
    HvkGui_API float         GetColumnOffsetFromNorm(const HvkGuiOldColumns* columns, float offset_norm);
    HvkGui_API float         GetColumnNormFromOffset(const HvkGuiOldColumns* columns, float offset);

    // Tables: Candidates for public API
    HvkGui_API void          TableOpenContextMenu(int column_n = -1);
    HvkGui_API void          TableSetColumnWidth(int column_n, float width);
    HvkGui_API void          TableSetColumnSortDirection(int column_n, HvkGuiSortDirection sort_direction, bool append_to_sort_specs);
    HvkGui_API int           TableGetHoveredRow();       // Retrieve *PREVIOUS FRAME* hovered row. This difference with TableGetHoveredColumn() is the reason why this is not public yet.
    HvkGui_API float         TableGetHeaderRowHeight();
    HvkGui_API float         TableGetHeaderAngledMaxLabelWidth();
    HvkGui_API void          TablePushBackgroundChannel();
    HvkGui_API void          TablePopBackgroundChannel();
    HvkGui_API void          TablePushColumnChannel(int column_n);
    HvkGui_API void          TablePopColumnChannel();
    HvkGui_API void          TableAngledHeadersRowEx(HvkGuiID row_id, float angle, float max_label_width, const HvkGuiTableHeaderData* data, int data_count);

    // Tables: Internals
    inline    HvkGuiTable*   GetCurrentTable() { HvkGuiContext& g = *GHvkGui; return g.CurrentTable; }
    HvkGui_API HvkGuiTable*   TableFindByID(HvkGuiID id);
    HvkGui_API bool          BeginTableEx(const char* name, HvkGuiID id, int columns_count, HvkGuiTableFlags flags = 0, const HvkVec2& outer_size = HvkVec2(0, 0), float inner_width = 0.0f);
    HvkGui_API void          TableBeginInitMemory(HvkGuiTable* table, int columns_count);
    HvkGui_API void          TableBeginApplyRequests(HvkGuiTable* table);
    HvkGui_API void          TableSetupDrawChannels(HvkGuiTable* table);
    HvkGui_API void          TableUpdateLayout(HvkGuiTable* table);
    HvkGui_API void          TableUpdateBorders(HvkGuiTable* table);
    HvkGui_API void          TableUpdateColumnsWeightFromWidth(HvkGuiTable* table);
    HvkGui_API void          TableDrawBorders(HvkGuiTable* table);
    HvkGui_API void          TableDrawDefaultContextMenu(HvkGuiTable* table, HvkGuiTableFlags flags_for_section_to_display);
    HvkGui_API bool          TableBeginContextMenuPopup(HvkGuiTable* table);
    HvkGui_API void          TableMergeDrawChannels(HvkGuiTable* table);
    inline HvkGuiTableInstanceData*  TableGetInstanceData(HvkGuiTable* table, int instance_no) { if (instance_no == 0) return &table->InstanceDataFirst; return &table->InstanceDataExtra[instance_no - 1]; }
    inline HvkGuiID                  TableGetInstanceID(HvkGuiTable* table, int instance_no)   { return TableGetInstanceData(table, instance_no)->TableInstanceID; }
    HvkGui_API void          TableFixDisplayOrder(HvkGuiTable* table);
    HvkGui_API void          TableSortSpecsSanitize(HvkGuiTable* table);
    HvkGui_API void          TableSortSpecsBuild(HvkGuiTable* table);
    HvkGui_API HvkGuiSortDirection TableGetColumnNextSortDirection(HvkGuiTableColumn* column);
    HvkGui_API void          TableFixColumnSortDirection(HvkGuiTable* table, HvkGuiTableColumn* column);
    HvkGui_API float         TableGetColumnWidthAuto(HvkGuiTable* table, HvkGuiTableColumn* column);
    HvkGui_API void          TableBeginRow(HvkGuiTable* table);
    HvkGui_API void          TableEndRow(HvkGuiTable* table);
    HvkGui_API void          TableBeginCell(HvkGuiTable* table, int column_n);
    HvkGui_API void          TableEndCell(HvkGuiTable* table);
    HvkGui_API HvkRect        TableGetCellBgRect(const HvkGuiTable* table, int column_n);
    HvkGui_API const char*   TableGetColumnName(const HvkGuiTable* table, int column_n);
    HvkGui_API HvkGuiID       TableGetColumnResizeID(HvkGuiTable* table, int column_n, int instance_no = 0);
    HvkGui_API float         TableCalcMaxColumnWidth(const HvkGuiTable* table, int column_n);
    HvkGui_API void          TableSetColumnWidthAutoSingle(HvkGuiTable* table, int column_n);
    HvkGui_API void          TableSetColumnWidthAutoAll(HvkGuiTable* table);
    HvkGui_API void          TableRemove(HvkGuiTable* table);
    HvkGui_API void          TableGcCompactTransientBuffers(HvkGuiTable* table);
    HvkGui_API void          TableGcCompactTransientBuffers(HvkGuiTableTempData* table);
    HvkGui_API void          TableGcCompactSettings();

    // Tables: Settings
    HvkGui_API void                  TableLoadSettings(HvkGuiTable* table);
    HvkGui_API void                  TableSaveSettings(HvkGuiTable* table);
    HvkGui_API void                  TableResetSettings(HvkGuiTable* table);
    HvkGui_API HvkGuiTableSettings*   TableGetBoundSettings(HvkGuiTable* table);
    HvkGui_API void                  TableSettingsAddSettingsHandler();
    HvkGui_API HvkGuiTableSettings*   TableSettingsCreate(HvkGuiID id, int columns_count);
    HvkGui_API HvkGuiTableSettings*   TableSettingsFindByID(HvkGuiID id);

    // Tab Bars
    inline    HvkGuiTabBar*  GetCurrentTabBar() { HvkGuiContext& g = *GHvkGui; return g.CurrentTabBar; }
    HvkGui_API HvkGuiTabBar*  TabBarFindByID(HvkGuiID id);
    HvkGui_API void          TabBarRemove(HvkGuiTabBar* tab_bar);
    HvkGui_API bool          BeginTabBarEx(HvkGuiTabBar* tab_bar, const HvkRect& bb, HvkGuiTabBarFlags flags);
    HvkGui_API HvkGuiTabItem* TabBarFindTabByID(HvkGuiTabBar* tab_bar, HvkGuiID tab_id);
    HvkGui_API HvkGuiTabItem* TabBarFindTabByOrder(HvkGuiTabBar* tab_bar, int order);
    HvkGui_API HvkGuiTabItem* TabBarGetCurrentTab(HvkGuiTabBar* tab_bar);
    inline int              TabBarGetTabOrder(HvkGuiTabBar* tab_bar, HvkGuiTabItem* tab) { return tab_bar->Tabs.index_from_ptr(tab); }
    HvkGui_API const char*   TabBarGetTabName(HvkGuiTabBar* tab_bar, HvkGuiTabItem* tab);
    HvkGui_API void          TabBarRemoveTab(HvkGuiTabBar* tab_bar, HvkGuiID tab_id);
    HvkGui_API void          TabBarCloseTab(HvkGuiTabBar* tab_bar, HvkGuiTabItem* tab);
    HvkGui_API void          TabBarQueueFocus(HvkGuiTabBar* tab_bar, HvkGuiTabItem* tab);
    HvkGui_API void          TabBarQueueFocus(HvkGuiTabBar* tab_bar, const char* tab_name);
    HvkGui_API void          TabBarQueueReorder(HvkGuiTabBar* tab_bar, HvkGuiTabItem* tab, int offset);
    HvkGui_API void          TabBarQueueReorderFromMousePos(HvkGuiTabBar* tab_bar, HvkGuiTabItem* tab, HvkVec2 mouse_pos);
    HvkGui_API bool          TabBarProcessReorder(HvkGuiTabBar* tab_bar);
    HvkGui_API bool          TabItemEx(HvkGuiTabBar* tab_bar, const char* label, bool* p_open, HvkGuiTabItemFlags flags, HvkGuiWindow* docked_window);
    HvkGui_API void          TabItemSpacing(const char* str_id, HvkGuiTabItemFlags flags, float width);
    HvkGui_API HvkVec2        TabItemCalcSize(const char* label, bool has_close_button_or_unsaved_marker);
    HvkGui_API HvkVec2        TabItemCalcSize(HvkGuiWindow* window);
    HvkGui_API void          TabItemBackground(HvkDrawList* draw_list, const HvkRect& bb, HvkGuiTabItemFlags flags, HvkU32 col);
    HvkGui_API void          TabItemLabelAndCloseButton(HvkDrawList* draw_list, const HvkRect& bb, HvkGuiTabItemFlags flags, HvkVec2 frame_padding, const char* label, HvkGuiID tab_id, HvkGuiID close_button_id, bool is_contents_visible, bool* out_just_closed, bool* out_text_clipped);

    // Render helpers
    // AVOID USING OUTSIDE OF HvkGui.CPP! NOT FOR PUBLIC CONSUMPTION. THOSE FUNCTIONS ARE A MESS. THEIR SIGNATURE AND BEHAVIOR WILL CHANGE, THEY NEED TO BE REFACTORED INTO SOMETHING DECENT.
    // NB: All position are in absolute pixels coordinates (we are never using window coordinates internally)
    HvkGui_API void          RenderText(HvkVec2 pos, const char* text, const char* text_end = NULL, bool hide_text_after_hash = true);
    HvkGui_API void          RenderTextWrapped(HvkVec2 pos, const char* text, const char* text_end, float wrap_width);
    HvkGui_API void          RenderTextClipped(const HvkVec2& pos_min, const HvkVec2& pos_max, const char* text, const char* text_end, const HvkVec2* text_size_if_known, const HvkVec2& align = HvkVec2(0, 0), const HvkRect* clip_rect = NULL);
    HvkGui_API void          RenderTextClippedEx(HvkDrawList* draw_list, const HvkVec2& pos_min, const HvkVec2& pos_max, const char* text, const char* text_end, const HvkVec2* text_size_if_known, const HvkVec2& align = HvkVec2(0, 0), const HvkRect* clip_rect = NULL);
    HvkGui_API void          RenderTextEllipsis(HvkDrawList* draw_list, const HvkVec2& pos_min, const HvkVec2& pos_max, float ellipsis_max_x, const char* text, const char* text_end, const HvkVec2* text_size_if_known);
    HvkGui_API void          RenderFrame(HvkVec2 p_min, HvkVec2 p_max, HvkU32 fill_col, bool borders = true, float rounding = 0.0f);
    HvkGui_API void          RenderFrameBorder(HvkVec2 p_min, HvkVec2 p_max, float rounding = 0.0f);
    HvkGui_API void          RenderColorComponentMarker(const HvkRect& bb, HvkU32 col, float rounding);
    HvkGui_API void          RenderColorRectWithAlphaCheckerboard(HvkDrawList* draw_list, HvkVec2 p_min, HvkVec2 p_max, HvkU32 fill_col, float grid_step, HvkVec2 grid_off, float rounding = 0.0f, HvkDrawFlags flags = 0);
    HvkGui_API void          RenderNavCursor(const HvkRect& bb, HvkGuiID id, HvkGuiNavRenderCursorFlags flags = HvkGuiNavRenderCursorFlags_None); // Navigation highlight
#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    inline    void          RenderNavHighlight(const HvkRect& bb, HvkGuiID id, HvkGuiNavRenderCursorFlags flags = HvkGuiNavRenderCursorFlags_None) { RenderNavCursor(bb, id, flags); } // Renamed in 1.91.4
#endif
    HvkGui_API const char*   FindRenderedTextEnd(const char* text, const char* text_end = NULL); // Find the optional ## from which we stop displaying text.
    HvkGui_API void          RenderMouseCursor(HvkVec2 pos, float scale, HvkGuiMouseCursor mouse_cursor, HvkU32 col_fill, HvkU32 col_border, HvkU32 col_shadow);

    // Render helpers (those functions don't access any HvkGui state!)
    HvkGui_API void          RenderArrow(HvkDrawList* draw_list, HvkVec2 pos, HvkU32 col, HvkGuiDir dir, float scale = 1.0f);
    HvkGui_API void          RenderBullet(HvkDrawList* draw_list, HvkVec2 pos, HvkU32 col);
    HvkGui_API void          RenderCheckMark(HvkDrawList* draw_list, HvkVec2 pos, HvkU32 col, float sz);
    HvkGui_API void          RenderArrowPointingAt(HvkDrawList* draw_list, HvkVec2 pos, HvkVec2 half_sz, HvkGuiDir direction, HvkU32 col);
    HvkGui_API void          RenderRectFilledInRangeH(HvkDrawList* draw_list, const HvkRect& rect, HvkU32 col, float fill_x0, float fill_x1, float rounding);
    HvkGui_API void          RenderRectFilledWithHole(HvkDrawList* draw_list, const HvkRect& outer, const HvkRect& inner, HvkU32 col, float rounding);

    // Widgets: Text
    HvkGui_API void          TextEx(const char* text, const char* text_end = NULL, HvkGuiTextFlags flags = 0);
    HvkGui_API void          TextAligned(float align_x, float size_x, const char* fmt, ...);               // FIXME-WIP: Works but API is likely to be reworked. This is designed for 1 item on the line. (#7024)
    HvkGui_API void          TextAlignedV(float align_x, float size_x, const char* fmt, va_list args);

    // Widgets
    HvkGui_API bool          ButtonEx(const char* label, const HvkVec2& size_arg = HvkVec2(0, 0), HvkGuiButtonFlags flags = 0);
    HvkGui_API bool          ArrowButtonEx(const char* str_id, HvkGuiDir dir, HvkVec2 size_arg, HvkGuiButtonFlags flags = 0);
    HvkGui_API bool          HvkageButtonEx(HvkGuiID id, HvkTextureRef tex_ref, const HvkVec2& Hvkage_size, const HvkVec2& uv0, const HvkVec2& uv1, const HvkVec4& bg_col, const HvkVec4& tint_col, HvkGuiButtonFlags flags = 0);
    HvkGui_API void          SeparatorEx(HvkGuiSeparatorFlags flags, float thickness = 1.0f);
    HvkGui_API void          SeparatorTextEx(HvkGuiID id, const char* label, const char* label_end, float extra_width);
    HvkGui_API bool          CheckboxFlags(const char* label, HvkS64* flags, HvkS64 flags_value);
    HvkGui_API bool          CheckboxFlags(const char* label, HvkU64* flags, HvkU64 flags_value);

    // Widgets: Window Decorations
    HvkGui_API bool          CloseButton(HvkGuiID id, const HvkVec2& pos);
    HvkGui_API bool          CollapseButton(HvkGuiID id, const HvkVec2& pos);
    HvkGui_API void          Scrollbar(HvkGuiAxis axis);
    HvkGui_API bool          ScrollbarEx(const HvkRect& bb, HvkGuiID id, HvkGuiAxis axis, HvkS64* p_scroll_v, HvkS64 avail_v, HvkS64 contents_v, HvkDrawFlags draw_rounding_flags = 0);
    HvkGui_API HvkRect        GetWindowScrollbarRect(HvkGuiWindow* window, HvkGuiAxis axis);
    HvkGui_API HvkGuiID       GetWindowScrollbarID(HvkGuiWindow* window, HvkGuiAxis axis);
    HvkGui_API HvkGuiID       GetWindowResizeCornerID(HvkGuiWindow* window, int n); // 0..3: corners
    HvkGui_API HvkGuiID       GetWindowResizeBorderID(HvkGuiWindow* window, HvkGuiDir dir);

    // Widgets low-level behaviors
    HvkGui_API bool          ButtonBehavior(const HvkRect& bb, HvkGuiID id, bool* out_hovered, bool* out_held, HvkGuiButtonFlags flags = 0);
    HvkGui_API bool          DragBehavior(HvkGuiID id, HvkGuiDataType data_type, void* p_v, float v_speed, const void* p_min, const void* p_max, const char* format, HvkGuiSliderFlags flags);
    HvkGui_API bool          SliderBehavior(const HvkRect& bb, HvkGuiID id, HvkGuiDataType data_type, void* p_v, const void* p_min, const void* p_max, const char* format, HvkGuiSliderFlags flags, HvkRect* out_grab_bb);
    HvkGui_API bool          SplitterBehavior(const HvkRect& bb, HvkGuiID id, HvkGuiAxis axis, float* size1, float* size2, float min_size1, float min_size2, float hover_extend = 0.0f, float hover_visibility_delay = 0.0f, HvkU32 bg_col = 0);

    // Widgets: Tree Nodes
    HvkGui_API bool          TreeNodeBehavior(HvkGuiID id, HvkGuiTreeNodeFlags flags, const char* label, const char* label_end = NULL);
    HvkGui_API void          TreeNodeDrawLineToChildNode(const HvkVec2& target_pos);
    HvkGui_API void          TreeNodeDrawLineToTreePop(const HvkGuiTreeNodeStackData* data);
    HvkGui_API void          TreePushOverrideID(HvkGuiID id);
    HvkGui_API bool          TreeNodeGetOpen(HvkGuiID storage_id);
    HvkGui_API void          TreeNodeSetOpen(HvkGuiID storage_id, bool open);
    HvkGui_API bool          TreeNodeUpdateNextOpen(HvkGuiID storage_id, HvkGuiTreeNodeFlags flags);   // Return open state. Consume previous SetNextItemOpen() data, if any. May return true when logging.

    // Template functions are instantiated in HvkGui_widgets.cpp for a finite number of types.
    // To use them externally (for custom widget) you may need an "extern template" statement in your code in order to link to existing instances and silence Clang warnings (see #2036).
    // e.g. " extern template HvkGui_API float RoundScalarWithFormatT<float, float>(const char* format, HvkGuiDataType data_type, float v); "
    template<typename T, typename SIGNED_T, typename FLOAT_T>   HvkGui_API float ScaleRatioFromValueT(HvkGuiDataType data_type, T v, T v_min, T v_max, bool is_logarithmic, float logarithmic_zero_epsilon, float zero_deadzone_size);
    template<typename T, typename SIGNED_T, typename FLOAT_T>   HvkGui_API T     ScaleValueFromRatioT(HvkGuiDataType data_type, float t, T v_min, T v_max, bool is_logarithmic, float logarithmic_zero_epsilon, float zero_deadzone_size);
    template<typename T, typename SIGNED_T, typename FLOAT_T>   HvkGui_API bool  DragBehaviorT(HvkGuiDataType data_type, T* v, float v_speed, T v_min, T v_max, const char* format, HvkGuiSliderFlags flags);
    template<typename T, typename SIGNED_T, typename FLOAT_T>   HvkGui_API bool  SliderBehaviorT(const HvkRect& bb, HvkGuiID id, HvkGuiDataType data_type, T* v, T v_min, T v_max, const char* format, HvkGuiSliderFlags flags, HvkRect* out_grab_bb);
    template<typename T>                                        HvkGui_API T     RoundScalarWithFormatT(const char* format, HvkGuiDataType data_type, T v);
    template<typename T>                                        HvkGui_API bool  CheckboxFlagsT(const char* label, T* flags, T flags_value);

    // Data type helpers
    HvkGui_API const HvkGuiDataTypeInfo*  DataTypeGetInfo(HvkGuiDataType data_type);
    HvkGui_API int           DataTypeFormatString(char* buf, int buf_size, HvkGuiDataType data_type, const void* p_data, const char* format);
    HvkGui_API void          DataTypeApplyOp(HvkGuiDataType data_type, int op, void* output, const void* arg_1, const void* arg_2);
    HvkGui_API bool          DataTypeApplyFromText(const char* buf, HvkGuiDataType data_type, void* p_data, const char* format, void* p_data_when_empty = NULL);
    HvkGui_API int           DataTypeCompare(HvkGuiDataType data_type, const void* arg_1, const void* arg_2);
    HvkGui_API bool          DataTypeClamp(HvkGuiDataType data_type, void* p_data, const void* p_min, const void* p_max);
    HvkGui_API bool          DataTypeIsZero(HvkGuiDataType data_type, const void* p_data);

    // InputText
    HvkGui_API bool          InputTextEx(const char* label, const char* hint, char* buf, int buf_size, const HvkVec2& size_arg, HvkGuiInputTextFlags flags, HvkGuiInputTextCallback callback = NULL, void* user_data = NULL);
    HvkGui_API void          InputTextDeactivateHook(HvkGuiID id);
    HvkGui_API bool          TempInputText(const HvkRect& bb, HvkGuiID id, const char* label, char* buf, int buf_size, HvkGuiInputTextFlags flags);
    HvkGui_API bool          TempInputScalar(const HvkRect& bb, HvkGuiID id, const char* label, HvkGuiDataType data_type, void* p_data, const char* format, const void* p_clamp_min = NULL, const void* p_clamp_max = NULL);
    inline bool             TempInputIsActive(HvkGuiID id)       { HvkGuiContext& g = *GHvkGui; return g.ActiveId == id && g.TempInputId == id; }
    inline HvkGuiInputTextState* GetInputTextState(HvkGuiID id)   { HvkGuiContext& g = *GHvkGui; return (id != 0 && g.InputTextState.ID == id) ? &g.InputTextState : NULL; } // Get input text state if active
    HvkGui_API void          SetNextItemRefVal(HvkGuiDataType data_type, void* p_data);
    inline bool             IsItemActiveAsInputText() { HvkGuiContext& g = *GHvkGui; return g.ActiveId != 0 && g.ActiveId == g.LastItemData.ID && g.InputTextState.ID == g.LastItemData.ID; } // This may be useful to apply workaround that a based on distinguish whenever an item is active as a text input field.

    // Color
    HvkGui_API void          ColorTooltip(const char* text, const float* col, HvkGuiColorEditFlags flags);
    HvkGui_API void          ColorEditOptionsPopup(const float* col, HvkGuiColorEditFlags flags);
    HvkGui_API void          ColorPickerOptionsPopup(const float* ref_col, HvkGuiColorEditFlags flags);
    inline void             SetNextItemColorMarker(HvkU32 col) { HvkGuiContext& g = *GHvkGui; g.NextItemData.HasFlags |= HvkGuiNextItemDataFlags_HasColorMarker; g.NextItemData.ColorMarker = col; }

    // Plot
    HvkGui_API int           PlotEx(HvkGuiPlotType plot_type, const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, const HvkVec2& size_arg);

    // Shade functions (write over already created vertices)
    HvkGui_API void          ShadeVertsLinearColorGradientKeepAlpha(HvkDrawList* draw_list, int vert_start_idx, int vert_end_idx, HvkVec2 gradient_p0, HvkVec2 gradient_p1, HvkU32 col0, HvkU32 col1);
    HvkGui_API void          ShadeVertsLinearUV(HvkDrawList* draw_list, int vert_start_idx, int vert_end_idx, const HvkVec2& a, const HvkVec2& b, const HvkVec2& uv_a, const HvkVec2& uv_b, bool clamp);
    HvkGui_API void          ShadeVertsTransformPos(HvkDrawList* draw_list, int vert_start_idx, int vert_end_idx, const HvkVec2& pivot_in, float cos_a, float sin_a, const HvkVec2& pivot_out);

    // Garbage collection
    HvkGui_API void          GcCompactTransientMiscBuffers();
    HvkGui_API void          GcCompactTransientWindowBuffers(HvkGuiWindow* window);
    HvkGui_API void          GcAwakeTransientWindowBuffers(HvkGuiWindow* window);

    // Error handling, State Recovery
    HvkGui_API bool          ErrorLog(const char* msg);
    HvkGui_API void          ErrorRecoveryStoreState(HvkGuiErrorRecoveryState* state_out);
    HvkGui_API void          ErrorRecoveryTryToRecoverState(const HvkGuiErrorRecoveryState* state_in);
    HvkGui_API void          ErrorRecoveryTryToRecoverWindowState(const HvkGuiErrorRecoveryState* state_in);
    HvkGui_API void          ErrorCheckUsingSetCursorPosToExtendParentBoundaries();
    HvkGui_API void          ErrorCheckEndFrameFinalizeErrorTooltip();
    HvkGui_API bool          BeginErrorTooltip();
    HvkGui_API void          EndErrorTooltip();

    // Debug Tools
    HvkGui_API void          DebugAllocHook(HvkGuiDebugAllocInfo* info, int frame_count, void* ptr, size_t size); // size >= 0 : alloc, size = -1 : free
    HvkGui_API void          DebugDrawCursorPos(HvkU32 col = Hvk_COL32(255, 0, 0, 255));
    HvkGui_API void          DebugDrawLineExtents(HvkU32 col = Hvk_COL32(255, 0, 0, 255));
    HvkGui_API void          DebugDrawItemRect(HvkU32 col = Hvk_COL32(255, 0, 0, 255));
    HvkGui_API void          DebugTextUnformattedWithLocateItem(const char* line_begin, const char* line_end);
    HvkGui_API void          DebugLocateItem(HvkGuiID target_id);                     // Call sparingly: only 1 at the same time!
    HvkGui_API void          DebugLocateItemOnHover(HvkGuiID target_id);              // Only call on reaction to a mouse Hover: because only 1 at the same time!
    HvkGui_API void          DebugLocateItemResolveWithLastItem();
    HvkGui_API void          DebugBreakClearData();
    HvkGui_API bool          DebugBreakButton(const char* label, const char* description_of_location);
    HvkGui_API void          DebugBreakButtonTooltip(bool keyboard_only, const char* description_of_location);
    HvkGui_API void          ShowFontAtlas(HvkFontAtlas* atlas);
    HvkGui_API HvkU64         DebugTextureIDToU64(HvkTextureID tex_id);
    HvkGui_API void          DebugHookIdInfo(HvkGuiID id, HvkGuiDataType data_type, const void* data_id, const void* data_id_end);
    HvkGui_API void          DebugNodeColumns(HvkGuiOldColumns* columns);
    HvkGui_API void          DebugNodeDrawList(HvkGuiWindow* window, HvkGuiViewportP* viewport, const HvkDrawList* draw_list, const char* label);
    HvkGui_API void          DebugNodeDrawCmdShowMeshAndBoundingBox(HvkDrawList* out_draw_list, const HvkDrawList* draw_list, const HvkDrawCmd* draw_cmd, bool show_mesh, bool show_aabb);
    HvkGui_API void          DebugNodeFont(HvkFont* font);
    HvkGui_API void          DebugNodeFontGlyphesForSrcMask(HvkFont* font, HvkFontBaked* baked, int src_mask);
    HvkGui_API void          DebugNodeFontGlyph(HvkFont* font, const HvkFontGlyph* glyph);
    HvkGui_API void          DebugNodeTexture(HvkTextureData* tex, int int_id, const HvkFontAtlasRect* highlight_rect = NULL); // ID used to facilitate persisting the "current" texture.
    HvkGui_API void          DebugNodeStorage(HvkGuiStorage* storage, const char* label);
    HvkGui_API void          DebugNodeTabBar(HvkGuiTabBar* tab_bar, const char* label);
    HvkGui_API void          DebugNodeTable(HvkGuiTable* table);
    HvkGui_API void          DebugNodeTableSettings(HvkGuiTableSettings* settings);
    HvkGui_API void          DebugNodeInputTextState(HvkGuiInputTextState* state);
    HvkGui_API void          DebugNodeTypingSelectState(HvkGuiTypingSelectState* state);
    HvkGui_API void          DebugNodeMultiSelectState(HvkGuiMultiSelectState* state);
    HvkGui_API void          DebugNodeWindow(HvkGuiWindow* window, const char* label);
    HvkGui_API void          DebugNodeWindowSettings(HvkGuiWindowSettings* settings);
    HvkGui_API void          DebugNodeWindowsList(HvkVector<HvkGuiWindow*>* windows, const char* label);
    HvkGui_API void          DebugNodeWindowsListByBeginStackParent(HvkGuiWindow** windows, int windows_size, HvkGuiWindow* parent_in_begin_stack);
    HvkGui_API void          DebugNodeViewport(HvkGuiViewportP* viewport);
    HvkGui_API void          DebugRenderKeyboardPreview(HvkDrawList* draw_list);
    HvkGui_API void          DebugRenderViewportThumbnail(HvkDrawList* draw_list, HvkGuiViewportP* viewport, const HvkRect& bb);

    // Obsolete functions
#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    //inline void   SetItemUsingMouseWheel()                                            { SetItemKeyOwner(HvkGuiKey_MouseWheelY); }      // Changed in 1.89
    //inline bool   TreeNodeBehaviorIsOpen(HvkGuiID id, HvkGuiTreeNodeFlags flags = 0)    { return TreeNodeUpdateNextOpen(id, flags); }   // Renamed in 1.89
    //inline bool   IsKeyPressedMap(HvkGuiKey key, bool repeat = true)                   { Hvk_ASSERT(IsNamedKey(key)); return IsKeyPressed(key, repeat); } // Removed in 1.87: Mapping from named key is always identity!

    // Refactored focus/nav/tabbing system in 1.82 and 1.84. If you have old/custom copy-and-pasted widgets which used FocusableItemRegister():
    //  (Old) HvkGui_VERSION_NUM  < 18209: using 'ItemAdd(....)'                              and 'bool tab_focused = FocusableItemRegister(...)'
    //  (Old) HvkGui_VERSION_NUM >= 18209: using 'ItemAdd(..., HvkGuiItemAddFlags_Focusable)'  and 'bool tab_focused = (g.LastItemData.StatusFlags & HvkGuiItemStatusFlags_Focused) != 0'
    //  (New) HvkGui_VERSION_NUM >= 18413: using 'ItemAdd(..., HvkGuiItemFlags_Inputable)'     and 'bool tab_focused = (g.NavActivateId == id && (g.NavActivateFlags & HvkGuiActivateFlags_PreferInput))'
    //inline bool   FocusableItemRegister(HvkGuiWindow* window, HvkGuiID id)              // -> pass HvkGuiItemAddFlags_Inputable flag to ItemAdd()
    //inline void   FocusableItemUnregister(HvkGuiWindow* window)                        // -> unnecessary: TempInputText() uses HvkGuiInputTextFlags_MergedItem
#endif

} // namespace HvkGui


//-----------------------------------------------------------------------------
// [SECTION] HvkFontLoader
//-----------------------------------------------------------------------------

// Hooks and storage for a given font backend.
// This structure is likely to evolve as we add support for incremental atlas updates.
// Conceptually this could be public, but API is still going to be evolve.
struct HvkFontLoader
{
    const char*     Name;
    bool            (*LoaderInit)(HvkFontAtlas* atlas);
    void            (*LoaderShutdown)(HvkFontAtlas* atlas);
    bool            (*FontSrcInit)(HvkFontAtlas* atlas, HvkFontConfig* src);
    void            (*FontSrcDestroy)(HvkFontAtlas* atlas, HvkFontConfig* src);
    bool            (*FontSrcContainsGlyph)(HvkFontAtlas* atlas, HvkFontConfig* src, HvkWchar codepoint);
    bool            (*FontBakedInit)(HvkFontAtlas* atlas, HvkFontConfig* src, HvkFontBaked* baked, void* loader_data_for_baked_src);
    void            (*FontBakedDestroy)(HvkFontAtlas* atlas, HvkFontConfig* src, HvkFontBaked* baked, void* loader_data_for_baked_src);
    bool            (*FontBakedLoadGlyph)(HvkFontAtlas* atlas, HvkFontConfig* src, HvkFontBaked* baked, void* loader_data_for_baked_src, HvkWchar codepoint, HvkFontGlyph* out_glyph, float* out_advance_x);

    // Size of backend data, Per Baked * Per Source. Buffers are managed by core to avoid excessive allocations.
    // FIXME: At this point the two other types of buffers may be managed by core to be consistent?
    size_t          FontBakedSrcLoaderDataSize;

    HvkFontLoader()  { memset(this, 0, sizeof(*this)); }
};

#ifdef HvkGui_ENABLE_STB_TRUETYPE
HvkGui_API const HvkFontLoader* HvkFontAtlasGetFontLoaderForStbTruetype();
#endif
#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
typedef HvkFontLoader HvkFontBuilderIO; // [renamed/changed in 1.92] The types are not actually compatible but we provide this as a compile-time error report helper.
#endif

//-----------------------------------------------------------------------------
// [SECTION] HvkFontAtlas internal API
//-----------------------------------------------------------------------------

#define HvkGui_FONT_SIZE_MAX                                     (512.0f)
#define HvkGui_FONT_SIZE_THRESHOLD_FOR_LOADADVANCEXONLYMODE      (128.0f)

// Helpers: HvkTextureRef ==/!= operators provided as convenience
// (note that _TexID and _TexData are never set simultaneously)
inline bool operator==(const HvkTextureRef& lhs, const HvkTextureRef& rhs)    { return lhs._TexID == rhs._TexID && lhs._TexData == rhs._TexData; }
inline bool operator!=(const HvkTextureRef& lhs, const HvkTextureRef& rhs)    { return lhs._TexID != rhs._TexID || lhs._TexData != rhs._TexData; }

// Refer to HvkFontAtlasPackGetRect() to better understand how this works.
#define HvkFontAtlasRectId_IndexMask_        (0x0007FFFF)    // 20-bits signed: index to access builder->RectsIndex[].
#define HvkFontAtlasRectId_GenerationMask_   (0x3FF00000)    // 10-bits: entry generation, so each ID is unique and get can safely detected old identifiers.
#define HvkFontAtlasRectId_GenerationShift_  (20)
inline int               HvkFontAtlasRectId_GetIndex(HvkFontAtlasRectId id)       { return (id & HvkFontAtlasRectId_IndexMask_); }
inline unsigned int      HvkFontAtlasRectId_GetGeneration(HvkFontAtlasRectId id)  { return (unsigned int)(id & HvkFontAtlasRectId_GenerationMask_) >> HvkFontAtlasRectId_GenerationShift_; }
inline HvkFontAtlasRectId HvkFontAtlasRectId_Make(int index_idx, int gen_idx)     { Hvk_ASSERT(index_idx >= 0 && index_idx <= HvkFontAtlasRectId_IndexMask_ && gen_idx <= (HvkFontAtlasRectId_GenerationMask_ >> HvkFontAtlasRectId_GenerationShift_)); return (HvkFontAtlasRectId)(index_idx | (gen_idx << HvkFontAtlasRectId_GenerationShift_)); }

// Packed rectangle lookup entry (we need an indirection to allow removing/reordering rectangles)
// User are returned HvkFontAtlasRectId values which are meant to be persistent.
// We handle this with an indirection. While Rects[] may be in theory shuffled, compacted etc., RectsIndex[] cannot it is keyed by HvkFontAtlasRectId.
// RectsIndex[] is used both as an index into Rects[] and an index into itself. This is basically a free-list. See HvkFontAtlasBuildAllocRectIndexEntry() code.
// Having this also makes it easier to e.g. sort rectangles during repack.
struct HvkFontAtlasRectEntry
{
    int                 TargetIndex : 20;   // When Used: HvkFontAtlasRectId -> into Rects[]. When unused: index to next unused RectsIndex[] slot to consume free-list.
    unsigned int        Generation : 10;    // Increased each time the entry is reused for a new rectangle.
    unsigned int        IsUsed : 1;
};

// Data available to potential texture post-processing functions
struct HvkFontAtlasPostProcessData
{
    HvkFontAtlas*        FontAtlas;
    HvkFont*             Font;
    HvkFontConfig*       FontSrc;
    HvkFontBaked*        FontBaked;
    HvkFontGlyph*        Glyph;

    // Pixel data
    void*               Pixels;
    HvkTextureFormat     Format;
    int                 Pitch;
    int                 Width;
    int                 Height;
};

// We avoid dragging Hvkstb_rectpack.h into public header (partly because binding generators are having issues with it)
#ifdef HvkGui_STB_NAMESPACE
namespace HvkGui_STB_NAMESPACE { struct stbrp_node; }
typedef HvkGui_STB_NAMESPACE::stbrp_node stbrp_node_im;
#else
struct stbrp_node;
typedef stbrp_node stbrp_node_im;
#endif
struct stbrp_context_opaque { char data[80]; };

// Internal storage for incrementally packing and building a HvkFontAtlas
struct HvkFontAtlasBuilder
{
    stbrp_context_opaque        PackContext;            // Actually 'stbrp_context' but we don't want to define this in the header file.
    HvkVector<stbrp_node_im>     PackNodes;
    HvkVector<HvkTextureRect>     Rects;
    HvkVector<HvkFontAtlasRectEntry> RectsIndex;          // HvkFontAtlasRectId -> index into Rects[]
    HvkVector<unsigned char>     TempBuffer;             // Misc scratch buffer
    int                         RectsIndexFreeListStart;// First unused entry
    int                         RectsPackedCount;       // Number of packed rectangles.
    int                         RectsPackedSurface;     // Number of packed pixels. Used when compacting to heuristically find the ideal texture size.
    int                         RectsDiscardedCount;
    int                         RectsDiscardedSurface;
    int                         FrameCount;             // Current frame count
    HvkVec2i                     MaxRectSize;            // Largest rectangle to pack (de-facto used as a "minimum texture size")
    HvkVec2i                     MaxRectBounds;          // Bottom-right most used pixels
    bool                        LockDisableResize;      // Disable resizing texture
    bool                        PreloadedAllGlyphsRanges; // Set when missing HvkGuiBackendFlags_RendererHasTextures features forces atlas to preload everything.

    // Cache of all HvkFontBaked
    HvkStableVector<HvkFontBaked,32> BakedPool;
    HvkGuiStorage                BakedMap;               // BakedId --> HvkFontBaked*
    int                         BakedDiscardedCount;

    // Custom rectangle identifiers
    HvkFontAtlasRectId           PackIdMouseCursors;     // White pixel + mouse cursors. Also happen to be fallback in case of packing failure.
    HvkFontAtlasRectId           PackIdLinesTexData;

    HvkFontAtlasBuilder()        { memset(this, 0, sizeof(*this)); FrameCount = -1; RectsIndexFreeListStart = -1; PackIdMouseCursors = PackIdLinesTexData = -1; }
};

HvkGui_API void              HvkFontAtlasBuildInit(HvkFontAtlas* atlas);
HvkGui_API void              HvkFontAtlasBuildDestroy(HvkFontAtlas* atlas);
HvkGui_API void              HvkFontAtlasBuildMain(HvkFontAtlas* atlas);
HvkGui_API void              HvkFontAtlasBuildSetupFontLoader(HvkFontAtlas* atlas, const HvkFontLoader* font_loader);
HvkGui_API void              HvkFontAtlasBuildNotifySetFont(HvkFontAtlas* atlas, HvkFont* old_font, HvkFont* new_font);
HvkGui_API void              HvkFontAtlasBuildUpdatePointers(HvkFontAtlas* atlas);
HvkGui_API void              HvkFontAtlasBuildRenderBitmapFromString(HvkFontAtlas* atlas, int x, int y, int w, int h, const char* in_str, char in_marker_char);
HvkGui_API void              HvkFontAtlasBuildClear(HvkFontAtlas* atlas); // Clear output and custom rects

HvkGui_API HvkTextureData*    HvkFontAtlasTextureAdd(HvkFontAtlas* atlas, int w, int h);
HvkGui_API void              HvkFontAtlasTextureMakeSpace(HvkFontAtlas* atlas);
HvkGui_API void              HvkFontAtlasTextureRepack(HvkFontAtlas* atlas, int w, int h);
HvkGui_API void              HvkFontAtlasTextureGrow(HvkFontAtlas* atlas, int old_w = -1, int old_h = -1);
HvkGui_API void              HvkFontAtlasTextureCompact(HvkFontAtlas* atlas);
HvkGui_API HvkVec2i           HvkFontAtlasTextureGetSizeEstimate(HvkFontAtlas* atlas);

HvkGui_API void              HvkFontAtlasBuildSetupFontSpecialGlyphs(HvkFontAtlas* atlas, HvkFont* font, HvkFontConfig* src);
HvkGui_API void              HvkFontAtlasBuildLegacyPreloadAllGlyphRanges(HvkFontAtlas* atlas); // Legacy
HvkGui_API void              HvkFontAtlasBuildGetOversampleFactors(HvkFontConfig* src, HvkFontBaked* baked, int* out_oversample_h, int* out_oversample_v);
HvkGui_API void              HvkFontAtlasBuildDiscardBakes(HvkFontAtlas* atlas, int unused_frames);

HvkGui_API bool              HvkFontAtlasFontSourceInit(HvkFontAtlas* atlas, HvkFontConfig* src);
HvkGui_API void              HvkFontAtlasFontSourceAddToFont(HvkFontAtlas* atlas, HvkFont* font, HvkFontConfig* src);
HvkGui_API void              HvkFontAtlasFontDestroySourceData(HvkFontAtlas* atlas, HvkFontConfig* src);
HvkGui_API bool              HvkFontAtlasFontInitOutput(HvkFontAtlas* atlas, HvkFont* font); // Using FontDestroyOutput/FontInitOutput sequence useful notably if font loader params have changed
HvkGui_API void              HvkFontAtlasFontDestroyOutput(HvkFontAtlas* atlas, HvkFont* font);
HvkGui_API void              HvkFontAtlasFontDiscardBakes(HvkFontAtlas* atlas, HvkFont* font, int unused_frames);

HvkGui_API HvkGuiID           HvkFontAtlasBakedGetId(HvkGuiID font_id, float baked_size, float rasterizer_density);
HvkGui_API HvkFontBaked*      HvkFontAtlasBakedGetOrAdd(HvkFontAtlas* atlas, HvkFont* font, float font_size, float font_rasterizer_density);
HvkGui_API HvkFontBaked*      HvkFontAtlasBakedGetClosestMatch(HvkFontAtlas* atlas, HvkFont* font, float font_size, float font_rasterizer_density);
HvkGui_API HvkFontBaked*      HvkFontAtlasBakedAdd(HvkFontAtlas* atlas, HvkFont* font, float font_size, float font_rasterizer_density, HvkGuiID baked_id);
HvkGui_API void              HvkFontAtlasBakedDiscard(HvkFontAtlas* atlas, HvkFont* font, HvkFontBaked* baked);
HvkGui_API HvkFontGlyph*      HvkFontAtlasBakedAddFontGlyph(HvkFontAtlas* atlas, HvkFontBaked* baked, HvkFontConfig* src, const HvkFontGlyph* in_glyph);
HvkGui_API void              HvkFontAtlasBakedAddFontGlyphAdvancedX(HvkFontAtlas* atlas, HvkFontBaked* baked, HvkFontConfig* src, HvkWchar codepoint, float advance_x);
HvkGui_API void              HvkFontAtlasBakedDiscardFontGlyph(HvkFontAtlas* atlas, HvkFont* font, HvkFontBaked* baked, HvkFontGlyph* glyph);
HvkGui_API void              HvkFontAtlasBakedSetFontGlyphBitmap(HvkFontAtlas* atlas, HvkFontBaked* baked, HvkFontConfig* src, HvkFontGlyph* glyph, HvkTextureRect* r, const unsigned char* src_pixels, HvkTextureFormat src_fmt, int src_pitch);

HvkGui_API void              HvkFontAtlasPackInit(HvkFontAtlas* atlas);
HvkGui_API HvkFontAtlasRectId HvkFontAtlasPackAddRect(HvkFontAtlas* atlas, int w, int h, HvkFontAtlasRectEntry* overwrite_entry = NULL);
HvkGui_API HvkTextureRect*    HvkFontAtlasPackGetRect(HvkFontAtlas* atlas, HvkFontAtlasRectId id);
HvkGui_API HvkTextureRect*    HvkFontAtlasPackGetRectSafe(HvkFontAtlas* atlas, HvkFontAtlasRectId id);
HvkGui_API void              HvkFontAtlasPackDiscardRect(HvkFontAtlas* atlas, HvkFontAtlasRectId id);

HvkGui_API void              HvkFontAtlasUpdateNewFrame(HvkFontAtlas* atlas, int frame_count, bool renderer_has_textures);
HvkGui_API void              HvkFontAtlasAddDrawListSharedData(HvkFontAtlas* atlas, HvkDrawListSharedData* data);
HvkGui_API void              HvkFontAtlasRemoveDrawListSharedData(HvkFontAtlas* atlas, HvkDrawListSharedData* data);
HvkGui_API void              HvkFontAtlasUpdateDrawListsTextures(HvkFontAtlas* atlas, HvkTextureRef old_tex, HvkTextureRef new_tex);
HvkGui_API void              HvkFontAtlasUpdateDrawListsSharedData(HvkFontAtlas* atlas);

HvkGui_API void              HvkFontAtlasTextureBlockConvert(const unsigned char* src_pixels, HvkTextureFormat src_fmt, int src_pitch, unsigned char* dst_pixels, HvkTextureFormat dst_fmt, int dst_pitch, int w, int h);
HvkGui_API void              HvkFontAtlasTextureBlockPostProcess(HvkFontAtlasPostProcessData* data);
HvkGui_API void              HvkFontAtlasTextureBlockPostProcessMultiply(HvkFontAtlasPostProcessData* data, float multiply_factor);
HvkGui_API void              HvkFontAtlasTextureBlockFill(HvkTextureData* dst_tex, int dst_x, int dst_y, int w, int h, HvkU32 col);
HvkGui_API void              HvkFontAtlasTextureBlockCopy(HvkTextureData* src_tex, int src_x, int src_y, HvkTextureData* dst_tex, int dst_x, int dst_y, int w, int h);
HvkGui_API void              HvkFontAtlasTextureBlockQueueUpload(HvkFontAtlas* atlas, HvkTextureData* tex, int x, int y, int w, int h);

HvkGui_API int               HvkTextureDataGetFormatBytesPerPixel(HvkTextureFormat format);
HvkGui_API const char*       HvkTextureDataGetStatusName(HvkTextureStatus status);
HvkGui_API const char*       HvkTextureDataGetFormatName(HvkTextureFormat format);

#ifndef HvkGui_DISABLE_DEBUG_TOOLS
HvkGui_API void              HvkFontAtlasDebugLogTextureRequests(HvkFontAtlas* atlas);
#endif

HvkGui_API bool      HvkFontAtlasGetMouseCursorTexData(HvkFontAtlas* atlas, HvkGuiMouseCursor cursor_type, HvkVec2* out_offset, HvkVec2* out_size, HvkVec2 out_uv_border[2], HvkVec2 out_uv_fill[2]);

//-----------------------------------------------------------------------------
// [SECTION] Test Engine specific hooks (HvkGui_test_engine)
//-----------------------------------------------------------------------------

#ifdef HvkGui_ENABLE_TEST_ENGINE
extern void         HvkGuiTestEngineHook_ItemAdd(HvkGuiContext* ctx, HvkGuiID id, const HvkRect& bb, const HvkGuiLastItemData* item_data);           // item_data may be NULL
extern void         HvkGuiTestEngineHook_ItemInfo(HvkGuiContext* ctx, HvkGuiID id, const char* label, HvkGuiItemStatusFlags flags);
extern void         HvkGuiTestEngineHook_Log(HvkGuiContext* ctx, const char* fmt, ...);
extern const char*  HvkGuiTestEngine_FindItemDebugLabel(HvkGuiContext* ctx, HvkGuiID id);

// In HvkGui_VERSION_NUM >= 18934: changed HvkGui_TEST_ENGINE_ITEM_ADD(bb,id) to HvkGui_TEST_ENGINE_ITEM_ADD(id,bb,item_data);
#define HvkGui_TEST_ENGINE_ITEM_ADD(_ID,_BB,_ITEM_DATA)      if (g.TestEngineHookItems) HvkGuiTestEngineHook_ItemAdd(&g, _ID, _BB, _ITEM_DATA)    // Register item bounding box
#define HvkGui_TEST_ENGINE_ITEM_INFO(_ID,_LABEL,_FLAGS)      if (g.TestEngineHookItems) HvkGuiTestEngineHook_ItemInfo(&g, _ID, _LABEL, _FLAGS)    // Register item label and status flags (optional)
#define HvkGui_TEST_ENGINE_LOG(_FMT,...)                     HvkGuiTestEngineHook_Log(&g, _FMT, __VA_ARGS__)                                      // Custom log entry from user land into test log
#else
#define HvkGui_TEST_ENGINE_ITEM_ADD(_ID,_BB,_ITEM_DATA)      ((void)0)
#define HvkGui_TEST_ENGINE_ITEM_INFO(_ID,_LABEL,_FLAGS)      ((void)g)
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

#endif // #ifndef HvkGui_DISABLE
