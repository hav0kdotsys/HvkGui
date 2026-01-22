// FreeType font loader for HvkGui (used as a replacement for the stb_truetype loader)

#include "hvkgui.h"
#ifndef HvkGui_DISABLE
#include "hvkgui_internal.h"
#include "HvkGui_freetype.h"

#include <stdint.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_SYNTHESIS_H

namespace
{
    // Default memory allocators
    static void* HvkGuiFreeTypeDefaultAllocFunc(size_t size, void* user_data) { Hvk_UNUSED(user_data); return Hvk_ALLOC(size); }
    static void  HvkGuiFreeTypeDefaultFreeFunc(void* ptr, void* user_data) { Hvk_UNUSED(user_data); Hvk_FREE(ptr); }

    // Current memory allocators
    static void* (*GFreeTypeAllocFunc)(size_t sz, void* user_data) = HvkGuiFreeTypeDefaultAllocFunc;
    static void  (*GFreeTypeFreeFunc)(void* ptr, void* user_data) = HvkGuiFreeTypeDefaultFreeFunc;
    static void* GFreeTypeUserData = NULL;

    struct HvkGuiFreeTypeFontInfo
    {
        float Ascender;
        float Descender;
    };

    struct HvkGuiFreeTypeFontSrcData
    {
        FT_Face         Face;
        unsigned int    UserFlags;
        FT_Int32        LoadFlags;
        FT_Render_Mode  RenderMode;
    };

    struct HvkGuiFreeTypeGlyphInfo
    {
        int     Width;
        int     Height;
        int     OffsetX;
        int     OffsetY;
        float   AdvanceX;
        bool    IsColored;
    };

    #define FT_CEIL(X)  (((X + 63) & -64) / 64)

    static bool HvkGuiFreeType_SetPixelHeight(FT_Face face, unsigned int user_flags, float pixel_height, float rasterizer_density, HvkGuiFreeTypeFontInfo* out_info)
    {
        if (face == NULL)
            return false;

        FT_Size_RequestRec req = {};
        req.type = (user_flags & HvkGuiFreeTypeLoaderFlags_Bitmap) ? FT_SIZE_REQUEST_TYPE_NOMINAL : FT_SIZE_REQUEST_TYPE_REAL_DIM;
        req.width = 0;
        req.height = (uint32_t)(pixel_height * 64.0f * rasterizer_density);
        req.horiResolution = 0;
        req.vertResolution = 0;
        if (FT_Request_Size(face, &req) != 0)
            return false;

        if (out_info)
        {
            const float inv_density = (rasterizer_density > 0.0f) ? (1.0f / rasterizer_density) : 1.0f;
            FT_Size_Metrics metrics = face->size->metrics;
            out_info->Ascender = (float)FT_CEIL(metrics.ascender) * inv_density;
            out_info->Descender = (float)FT_CEIL(metrics.descender) * inv_density;
        }
        return true;
    }

    static bool HvkGuiFreeType_LoadGlyph(FT_Face face, unsigned int user_flags, FT_Int32 load_flags, HvkWchar codepoint)
    {
        if (face == NULL)
            return false;
        const uint32_t glyph_index = FT_Get_Char_Index(face, (uint32_t)codepoint);
        if (glyph_index == 0)
            return false;
        FT_Error error = FT_Load_Glyph(face, glyph_index, load_flags);
        if (error)
            return false;

        FT_GlyphSlot slot = face->glyph;
        if (user_flags & HvkGuiFreeTypeLoaderFlags_Bold)
            FT_GlyphSlot_Embolden(slot);
        if (user_flags & HvkGuiFreeTypeLoaderFlags_Oblique)
            FT_GlyphSlot_Oblique(slot);
        return true;
    }

    static const FT_Bitmap* HvkGuiFreeType_RenderGlyphAndGetInfo(FT_Face face, FT_Render_Mode render_mode, HvkGuiFreeTypeGlyphInfo* out_info)
    {
        if (face == NULL)
            return NULL;
        FT_GlyphSlot slot = face->glyph;
        if (FT_Render_Glyph(slot, render_mode) != 0)
            return NULL;

        FT_Bitmap* ft_bitmap = &slot->bitmap;
        if (out_info)
        {
            out_info->Width = (int)ft_bitmap->width;
            out_info->Height = (int)ft_bitmap->rows;
            out_info->OffsetX = slot->bitmap_left;
            out_info->OffsetY = -slot->bitmap_top;
            out_info->AdvanceX = (float)FT_CEIL(slot->advance.x);
            out_info->IsColored = (ft_bitmap->pixel_mode == FT_PIXEL_MODE_BGRA);
        }
        return ft_bitmap;
    }

    static bool HvkGuiFreeType_FontSrcInit(HvkFontAtlas* atlas, HvkFontConfig* src)
    {
        if (atlas == NULL || src == NULL)
            return false;
        FT_Library ft_library = (FT_Library)atlas->FontLoaderData;
        if (ft_library == NULL)
            return false;

        HvkGuiFreeTypeFontSrcData* data = (HvkGuiFreeTypeFontSrcData*)GFreeTypeAllocFunc(sizeof(HvkGuiFreeTypeFontSrcData), GFreeTypeUserData);
        if (data == NULL)
            return false;
        memset(data, 0, sizeof(*data));

        if (FT_New_Memory_Face(ft_library, (uint8_t*)src->FontData, (uint32_t)src->FontDataSize, (uint32_t)src->FontNo, &data->Face) != 0)
        {
            GFreeTypeFreeFunc(data, GFreeTypeUserData);
            return false;
        }
        if (FT_Select_Charmap(data->Face, FT_ENCODING_UNICODE) != 0)
        {
            FT_Done_Face(data->Face);
            GFreeTypeFreeFunc(data, GFreeTypeUserData);
            return false;
        }

        data->UserFlags = src->FontLoaderFlags | atlas->FontLoaderFlags;
        data->LoadFlags = 0;
        if ((data->UserFlags & HvkGuiFreeTypeLoaderFlags_Bitmap) == 0)
            data->LoadFlags |= FT_LOAD_NO_BITMAP;
        if (data->UserFlags & HvkGuiFreeTypeLoaderFlags_NoHinting)
            data->LoadFlags |= FT_LOAD_NO_HINTING;
        if (data->UserFlags & HvkGuiFreeTypeLoaderFlags_NoAutoHint)
            data->LoadFlags |= FT_LOAD_NO_AUTOHINT;
        if (data->UserFlags & HvkGuiFreeTypeLoaderFlags_ForceAutoHint)
            data->LoadFlags |= FT_LOAD_FORCE_AUTOHINT;
        if (data->UserFlags & HvkGuiFreeTypeLoaderFlags_LightHinting)
            data->LoadFlags |= FT_LOAD_TARGET_LIGHT;
        else if (data->UserFlags & HvkGuiFreeTypeLoaderFlags_MonoHinting)
            data->LoadFlags |= FT_LOAD_TARGET_MONO;
        else
            data->LoadFlags |= FT_LOAD_TARGET_NORMAL;
        if (data->UserFlags & HvkGuiFreeTypeLoaderFlags_LoadColor)
            data->LoadFlags |= FT_LOAD_COLOR;

        data->RenderMode = (data->UserFlags & HvkGuiFreeTypeLoaderFlags_Monochrome) ? FT_RENDER_MODE_MONO : FT_RENDER_MODE_NORMAL;
        src->FontLoaderData = data;
        return true;
    }

    static void HvkGuiFreeType_FontSrcDestroy(HvkFontAtlas*, HvkFontConfig* src)
    {
        if (src == NULL || src->FontLoaderData == NULL)
            return;
        HvkGuiFreeTypeFontSrcData* data = (HvkGuiFreeTypeFontSrcData*)src->FontLoaderData;
        if (data->Face)
            FT_Done_Face(data->Face);
        data->Face = NULL;
        GFreeTypeFreeFunc(data, GFreeTypeUserData);
        src->FontLoaderData = NULL;
    }

    static bool HvkGuiFreeType_FontSrcContainsGlyph(HvkFontAtlas*, HvkFontConfig* src, HvkWchar codepoint)
    {
        if (src == NULL || src->FontLoaderData == NULL)
            return false;
        HvkGuiFreeTypeFontSrcData* data = (HvkGuiFreeTypeFontSrcData*)src->FontLoaderData;
        return FT_Get_Char_Index(data->Face, (uint32_t)codepoint) != 0;
    }

    static bool HvkGuiFreeType_FontBakedInit(HvkFontAtlas*, HvkFontConfig* src, HvkFontBaked* baked, void*)
    {
        if (src == NULL || baked == NULL || src->FontLoaderData == NULL)
            return false;
        if (src->MergeMode)
            return true;

        HvkGuiFreeTypeFontSrcData* data = (HvkGuiFreeTypeFontSrcData*)src->FontLoaderData;
        const float rasterizer_density = src->RasterizerDensity * baked->RasterizerDensity;
        HvkGuiFreeTypeFontInfo info = {};
        if (!HvkGuiFreeType_SetPixelHeight(data->Face, data->UserFlags, baked->Size, rasterizer_density, &info))
            return false;
        baked->Ascent = HvkCeil(info.Ascender);
        baked->Descent = HvkFloor(info.Descender);
        return true;
    }

    static bool HvkGuiFreeType_FontBakedLoadGlyph(HvkFontAtlas* atlas, HvkFontConfig* src, HvkFontBaked* baked, void*, HvkWchar codepoint, HvkFontGlyph* out_glyph, float* out_advance_x)
    {
        if (src == NULL || baked == NULL || atlas == NULL || src->FontLoaderData == NULL)
            return false;
        HvkGuiFreeTypeFontSrcData* data = (HvkGuiFreeTypeFontSrcData*)src->FontLoaderData;

        const float rasterizer_density = src->RasterizerDensity * baked->RasterizerDensity;
        if (!HvkGuiFreeType_SetPixelHeight(data->Face, data->UserFlags, baked->Size, rasterizer_density, NULL))
            return false;
        if (!HvkGuiFreeType_LoadGlyph(data->Face, data->UserFlags, data->LoadFlags, codepoint))
            return false;

        const float inv_density = (rasterizer_density > 0.0f) ? (1.0f / rasterizer_density) : 1.0f;

        if (out_advance_x != NULL)
        {
            Hvk_ASSERT(out_glyph == NULL);
            FT_GlyphSlot slot = data->Face->glyph;
            *out_advance_x = (float)FT_CEIL(slot->advance.x) * inv_density;
            return true;
        }

        HvkGuiFreeTypeGlyphInfo info = {};
        const FT_Bitmap* ft_bitmap = HvkGuiFreeType_RenderGlyphAndGetInfo(data->Face, data->RenderMode, &info);
        if (ft_bitmap == NULL)
            return false;

        out_glyph->Codepoint = codepoint;
        out_glyph->AdvanceX = info.AdvanceX * inv_density;

        const bool is_visible = (info.Width > 0 && info.Height > 0);
        if (is_visible)
        {
            HvkFontAtlasRectId pack_id = HvkFontAtlasPackAddRect(atlas, info.Width, info.Height);
            if (pack_id == HvkFontAtlasRectId_Invalid)
            {
                Hvk_ASSERT(pack_id != HvkFontAtlasRectId_Invalid && "Out of texture memory.");
                return false;
            }
            HvkTextureRect* r = HvkFontAtlasPackGetRect(atlas, pack_id);

            const float ref_size = baked->OwnerFont->Sources[0]->SizePixels;
            const float offsets_scale = (ref_size != 0.0f) ? (baked->Size / ref_size) : 1.0f;
            float font_off_x = src->GlyphOffset.x * offsets_scale;
            float font_off_y = src->GlyphOffset.y * offsets_scale;
            if (src->PixelSnapH)
                font_off_x = Hvk_ROUND(font_off_x);
            if (src->PixelSnapV)
                font_off_y = Hvk_ROUND(font_off_y);
            font_off_y += Hvk_ROUND(baked->Ascent);

            out_glyph->X0 = info.OffsetX * inv_density + font_off_x;
            out_glyph->Y0 = info.OffsetY * inv_density + font_off_y;
            out_glyph->X1 = out_glyph->X0 + info.Width * inv_density;
            out_glyph->Y1 = out_glyph->Y0 + info.Height * inv_density;
            out_glyph->Visible = true;
            out_glyph->PackId = pack_id;
            out_glyph->Colored = info.IsColored;

            const unsigned char* src_pixels = ft_bitmap->buffer;
            int src_pitch = (int)ft_bitmap->pitch;
            HvkTextureFormat src_fmt = HvkTextureFormat_Alpha8;
            HvkFontAtlasBuilder* builder = atlas->Builder;

            if (ft_bitmap->pixel_mode == FT_PIXEL_MODE_GRAY)
            {
                src_fmt = HvkTextureFormat_Alpha8;
            }
            else if (ft_bitmap->pixel_mode == FT_PIXEL_MODE_MONO)
            {
                builder->TempBuffer.resize(info.Width * info.Height);
                unsigned char* temp = builder->TempBuffer.Data;
                const unsigned char* src_row = src_pixels;
                const int row_pitch = (src_pitch >= 0) ? src_pitch : -src_pitch;
                if (src_pitch < 0)
                    src_row = src_pixels + (info.Height - 1) * row_pitch;

                for (int y = 0; y < info.Height; y++)
                {
                    const unsigned char* src = src_row;
                    unsigned char* dst = temp + y * info.Width;
                    for (int x = 0; x < info.Width; x++)
                    {
                        const unsigned char bits = src[x >> 3];
                        dst[x] = (bits & (0x80 >> (x & 7))) ? 255 : 0;
                    }
                    src_row += (src_pitch >= 0) ? row_pitch : -row_pitch;
                }
                src_pixels = builder->TempBuffer.Data;
                src_pitch = info.Width;
                src_fmt = HvkTextureFormat_Alpha8;
            }
            else if (ft_bitmap->pixel_mode == FT_PIXEL_MODE_BGRA)
            {
                builder->TempBuffer.resize(info.Width * info.Height * 4);
                unsigned char* temp = builder->TempBuffer.Data;
                const unsigned char* src_row = src_pixels;
                const int row_pitch = (src_pitch >= 0) ? src_pitch : -src_pitch;
                if (src_pitch < 0)
                    src_row = src_pixels + (info.Height - 1) * row_pitch;

                for (int y = 0; y < info.Height; y++)
                {
                    const unsigned char* src = src_row;
                    unsigned char* dst = temp + y * info.Width * 4;
                    for (int x = 0; x < info.Width; x++)
                    {
                        const unsigned char b = src[x * 4 + 0];
                        const unsigned char g = src[x * 4 + 1];
                        const unsigned char r = src[x * 4 + 2];
                        const unsigned char a = src[x * 4 + 3];
                        if (a == 0)
                        {
                            dst[x * 4 + 0] = 0;
                            dst[x * 4 + 1] = 0;
                            dst[x * 4 + 2] = 0;
                            dst[x * 4 + 3] = 0;
                        }
                        else
                        {
                            dst[x * 4 + 0] = (unsigned char)((int)r * 255 / a);
                            dst[x * 4 + 1] = (unsigned char)((int)g * 255 / a);
                            dst[x * 4 + 2] = (unsigned char)((int)b * 255 / a);
                            dst[x * 4 + 3] = a;
                        }
                    }
                    src_row += (src_pitch >= 0) ? row_pitch : -row_pitch;
                }
                src_pixels = builder->TempBuffer.Data;
                src_pitch = info.Width * 4;
                src_fmt = HvkTextureFormat_RGBA32;
            }
            else
            {
                return false;
            }

            HvkFontAtlasBakedSetFontGlyphBitmap(atlas, baked, src, out_glyph, r, src_pixels, src_fmt, src_pitch);
        }

        return true;
    }

    static bool HvkGuiFreeType_LoaderInit(HvkFontAtlas* atlas)
    {
        if (atlas == NULL)
            return false;
        FT_Library ft_library = NULL;
        if (FT_Init_FreeType(&ft_library) != 0)
            return false;
        atlas->FontLoaderData = ft_library;
        return true;
    }

    static void HvkGuiFreeType_LoaderShutdown(HvkFontAtlas* atlas)
    {
        if (atlas == NULL || atlas->FontLoaderData == NULL)
            return;
        FT_Done_FreeType((FT_Library)atlas->FontLoaderData);
        atlas->FontLoaderData = NULL;
    }
}

const HvkFontLoader* HvkGuiFreeType::GetFontLoader()
{
    static HvkFontLoader loader;
    loader.Name = "freetype";
    loader.LoaderInit = HvkGuiFreeType_LoaderInit;
    loader.LoaderShutdown = HvkGuiFreeType_LoaderShutdown;
    loader.FontSrcInit = HvkGuiFreeType_FontSrcInit;
    loader.FontSrcDestroy = HvkGuiFreeType_FontSrcDestroy;
    loader.FontSrcContainsGlyph = HvkGuiFreeType_FontSrcContainsGlyph;
    loader.FontBakedInit = HvkGuiFreeType_FontBakedInit;
    loader.FontBakedDestroy = NULL;
    loader.FontBakedLoadGlyph = HvkGuiFreeType_FontBakedLoadGlyph;
    loader.FontBakedSrcLoaderDataSize = 0;
    return &loader;
}

void HvkGuiFreeType::SetAllocatorFunctions(void* (*alloc_func)(size_t sz, void* user_data), void (*free_func)(void* ptr, void* user_data), void* user_data)
{
    GFreeTypeAllocFunc = alloc_func ? alloc_func : HvkGuiFreeTypeDefaultAllocFunc;
    GFreeTypeFreeFunc = free_func ? free_func : HvkGuiFreeTypeDefaultFreeFunc;
    GFreeTypeUserData = user_data;
}

bool HvkGuiFreeType::DebugEditFontLoaderFlags(unsigned int* p_font_loader_flags)
{
    if (p_font_loader_flags == NULL)
        return false;
    unsigned int flags = *p_font_loader_flags;
    bool changed = false;
    if (HvkGui::CheckboxFlags("NoHinting", &flags, HvkGuiFreeTypeLoaderFlags_NoHinting)) changed = true;
    if (HvkGui::CheckboxFlags("NoAutoHint", &flags, HvkGuiFreeTypeLoaderFlags_NoAutoHint)) changed = true;
    if (HvkGui::CheckboxFlags("ForceAutoHint", &flags, HvkGuiFreeTypeLoaderFlags_ForceAutoHint)) changed = true;
    if (HvkGui::CheckboxFlags("LightHinting", &flags, HvkGuiFreeTypeLoaderFlags_LightHinting)) changed = true;
    if (HvkGui::CheckboxFlags("MonoHinting", &flags, HvkGuiFreeTypeLoaderFlags_MonoHinting)) changed = true;
    if (HvkGui::CheckboxFlags("Bold", &flags, HvkGuiFreeTypeLoaderFlags_Bold)) changed = true;
    if (HvkGui::CheckboxFlags("Oblique", &flags, HvkGuiFreeTypeLoaderFlags_Oblique)) changed = true;
    if (HvkGui::CheckboxFlags("Monochrome", &flags, HvkGuiFreeTypeLoaderFlags_Monochrome)) changed = true;
    if (HvkGui::CheckboxFlags("LoadColor", &flags, HvkGuiFreeTypeLoaderFlags_LoadColor)) changed = true;
    if (HvkGui::CheckboxFlags("Bitmap", &flags, HvkGuiFreeTypeLoaderFlags_Bitmap)) changed = true;
    if (changed)
        *p_font_loader_flags = flags;
    return changed;
}

#endif // HvkGui_DISABLE
