#pragma once
#include "hvkgui.h"
#ifndef HvkGui_DISABLE

// Hinting greatly impacts visuals (and glyph sizes).
// - By default, hinting is enabled and the font's native hinter is preferred over the auto-hinter.
// - When disabled, FreeType generates blurrier glyphs, more or less matches the stb_truetype.h
// - The Default hinting mode usually looks good, but may distort glyphs in an unusual way.
// - The Light hinting mode generates fuzzier glyphs but better matches Microsoft's rasterizer.
// You can set those flags globally in HvkFontAtlas::FontLoaderFlags
// You can set those flags on a per font basis in HvkFontConfig::FontLoaderFlags
enum HvkGuiFreeTypeLoaderFlags_
{
    HvkGuiFreeTypeLoaderFlags_NoHinting     = 1 << 0,   // Disable hinting. This generally generates 'blurrier' bitmap glyphs when the glyph are rendered in any of the anti-aliased modes.
    HvkGuiFreeTypeLoaderFlags_NoAutoHint    = 1 << 1,   // Disable auto-hinter.
    HvkGuiFreeTypeLoaderFlags_ForceAutoHint = 1 << 2,   // Indicates that the auto-hinter is preferred over the font's native hinter.
    HvkGuiFreeTypeLoaderFlags_LightHinting  = 1 << 3,   // A lighter hinting algorithm for gray-level modes.
    HvkGuiFreeTypeLoaderFlags_MonoHinting   = 1 << 4,   // Strong hinting algorithm that should only be used for monochrome output.
    HvkGuiFreeTypeLoaderFlags_Bold          = 1 << 5,   // Styling: Should we artificially embolden the font?
    HvkGuiFreeTypeLoaderFlags_Oblique       = 1 << 6,   // Styling: Should we slant the font, emulating italic style?
    HvkGuiFreeTypeLoaderFlags_Monochrome    = 1 << 7,   // Disable anti-aliasing. Combine this with MonoHinting for best results!
    HvkGuiFreeTypeLoaderFlags_LoadColor     = 1 << 8,   // Enable FreeType color-layered glyphs
    HvkGuiFreeTypeLoaderFlags_Bitmap        = 1 << 9    // Enable FreeType bitmap glyphs
};

namespace HvkGuiFreeType
{
    // This is automatically assigned when using '#define HvkGui_ENABLE_FREETYPE'.
    // If you need to dynamically select between multiple loaders:
    // - you can manually assign this loader with 'atlas->SetFontLoader(HvkGuiFreeType::GetFontLoader())'
    // - prefer deep-copying this into your own HvkFontLoader instance if you use hot-reloading that messes up static data.
    HvkGui_API const HvkFontLoader* GetFontLoader();

    // Override allocators. By default HvkGuiFreeType will use Hvk_ALLOC()/Hvk_FREE()
    HvkGui_API void SetAllocatorFunctions(void* (*alloc_func)(size_t sz, void* user_data), void (*free_func)(void* ptr, void* user_data), void* user_data = NULL);

    // Debug helper for editing font loader flags in the metrics UI.
    HvkGui_API bool DebugEditFontLoaderFlags(unsigned int* p_font_loader_flags);
}

#endif // #ifndef HvkGui_DISABLE
