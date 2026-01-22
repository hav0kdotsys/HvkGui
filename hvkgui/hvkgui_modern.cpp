// Modern theme helpers and widgets inspired by ImGuiMenu-s/22

#include "hvkgui.h"
#ifndef HvkGui_DISABLE
#include "hvkgui_internal.h"

namespace
{
    static HvkFont* g_modern_font_title = NULL;
    static HvkFont* g_modern_font_body = NULL;
    static HvkFont* g_modern_font_icon = NULL;

    static HvkVec2 AddV(const HvkVec2& a, const HvkVec2& b)
    {
        return HvkVec2(a.x + b.x, a.y + b.y);
    }

    static HvkVec2 SubV(const HvkVec2& a, const HvkVec2& b)
    {
        return HvkVec2(a.x - b.x, a.y - b.y);
    }

    static float ModernAnimFloat(HvkGuiID id, float target, float speed)
    {
        HvkGuiContext& g = *GHvkGui;
        HvkGuiStorage* storage = HvkGui::GetStateStorage();
        float* value = storage->GetFloatRef(id, target);
        const float t = 1.0f - HvkPow(0.0001f, g.IO.DeltaTime * speed);
        *value = HvkLerp(*value, target, t);
        return *value;
    }

    static bool ModernSliderBehavior(const HvkRect& bb, HvkGuiID id, float* v, float v_min, float v_max)
    {
        HvkGuiContext& g = *GHvkGui;
        HvkGuiWindow* window = HvkGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;
        if (!HvkGui::ItemAdd(bb, id))
            return false;
        bool hovered, held;
        bool pressed = HvkGui::ButtonBehavior(bb, id, &hovered, &held);
        if (held)
        {
            float t = (g.IO.MousePos.x - bb.Min.x) / bb.GetWidth();
            t = HvkSaturate(t);
            *v = v_min + (v_max - v_min) * t;
        }
        return pressed || held;
    }

    static float GetFontSizeFor(const HvkFont* font)
    {
        if (font && font->LegacySize > 0.0f)
            return font->LegacySize;
        return HvkGui::GetFontSize();
    }

    static HvkVec2 CalcTextSizeForFont(HvkFont* font, const char* text)
    {
        if (font == NULL)
            return HvkGui::CalcTextSize(text, NULL, true);
        const float size = GetFontSizeFor(font);
        return font->CalcTextSizeA(size, FLT_MAX, 0.0f, text, NULL, NULL);
    }

    static void AddTextWithFont(HvkDrawList* draw_list, HvkFont* font, const HvkVec2& pos, HvkU32 col, const char* text)
    {
        if (font == NULL)
        {
            draw_list->AddText(pos, col, text);
            return;
        }
        draw_list->AddText(font, GetFontSizeFor(font), pos, col, text);
    }

    static void DrawCenteredText(HvkDrawList* draw_list, HvkFont* font, const HvkRect& bb, HvkU32 col, const char* text)
    {
        HvkVec2 size = CalcTextSizeForFont(font, text);
        HvkVec2 pos = HvkVec2(bb.Min.x + (bb.GetWidth() - size.x) * 0.5f, bb.Min.y + (bb.GetHeight() - size.y) * 0.5f);
        AddTextWithFont(draw_list, font, pos, col, text);
    }

    static HvkVec4 HSVToRGB(float h, float s, float v)
    {
        float r, g, b;
        HvkGui::ColorConvertHSVtoRGB(h, s, v, r, g, b);
        return HvkVec4(r, g, b, 1.0f);
    }
}

void HvkGui::StyleColorsModern22(HvkGuiStyle* dst)
{
    HvkGuiStyle* style = dst ? dst : &HvkGui::GetStyle();
    HvkVec4* colors = style->Colors;

    const HvkVec4 layout = HvkVec4(25.0f / 255.0f, 25.0f / 255.0f, 28.0f / 255.0f, 1.0f);
    const HvkVec4 child = HvkVec4(28.0f / 255.0f, 28.0f / 255.0f, 33.0f / 255.0f, 1.0f);
    const HvkVec4 widget = HvkVec4(33.0f / 255.0f, 33.0f / 255.0f, 40.0f / 255.0f, 1.0f);
    const HvkVec4 border = HvkVec4(35.0f / 255.0f, 35.0f / 255.0f, 44.0f / 255.0f, 1.0f);
    const HvkVec4 accent = HvkVec4(176.0f / 255.0f, 180.0f / 255.0f, 255.0f / 255.0f, 1.0f);
    const HvkVec4 text = HvkVec4(110.0f / 255.0f, 110.0f / 255.0f, 129.0f / 255.0f, 1.0f);

    colors[HvkGuiCol_Text]                 = HvkVec4(1.0f, 1.0f, 1.0f, 1.0f);
    colors[HvkGuiCol_TextDisabled]         = text;
    colors[HvkGuiCol_WindowBg]             = layout;
    colors[HvkGuiCol_ChildBg]              = child;
    colors[HvkGuiCol_PopupBg]              = child;
    colors[HvkGuiCol_Border]               = border;
    colors[HvkGuiCol_BorderShadow]         = HvkVec4(0.08f, 0.08f, 0.08f, 0.35f);
    colors[HvkGuiCol_FrameBg]              = widget;
    colors[HvkGuiCol_FrameBgHovered]       = HvkLerp(widget, HvkVec4(1.0f, 1.0f, 1.0f, 1.0f), 0.08f);
    colors[HvkGuiCol_FrameBgActive]        = HvkLerp(widget, accent, 0.25f);
    colors[HvkGuiCol_TitleBg]              = layout;
    colors[HvkGuiCol_TitleBgActive]        = layout;
    colors[HvkGuiCol_TitleBgCollapsed]     = layout;
    colors[HvkGuiCol_MenuBarBg]            = layout;
    colors[HvkGuiCol_ScrollbarBg]          = layout;
    colors[HvkGuiCol_ScrollbarGrab]        = HvkLerp(widget, HvkVec4(1.0f, 1.0f, 1.0f, 1.0f), 0.12f);
    colors[HvkGuiCol_ScrollbarGrabHovered] = HvkLerp(widget, HvkVec4(1.0f, 1.0f, 1.0f, 1.0f), 0.20f);
    colors[HvkGuiCol_ScrollbarGrabActive]  = HvkLerp(widget, HvkVec4(1.0f, 1.0f, 1.0f, 1.0f), 0.26f);
    colors[HvkGuiCol_CheckMark]            = accent;
    colors[HvkGuiCol_SliderGrab]           = accent;
    colors[HvkGuiCol_SliderGrabActive]     = accent;
    colors[HvkGuiCol_Button]               = accent;
    colors[HvkGuiCol_ButtonHovered]        = HvkLerp(accent, HvkVec4(1.0f, 1.0f, 1.0f, 1.0f), 0.08f);
    colors[HvkGuiCol_ButtonActive]         = HvkLerp(accent, HvkVec4(1.0f, 1.0f, 1.0f, 1.0f), 0.12f);
    colors[HvkGuiCol_Header]               = HvkLerp(accent, layout, 0.55f);
    colors[HvkGuiCol_HeaderHovered]        = HvkLerp(accent, layout, 0.40f);
    colors[HvkGuiCol_HeaderActive]         = HvkLerp(accent, layout, 0.30f);
    colors[HvkGuiCol_Separator]            = border;
    colors[HvkGuiCol_SeparatorHovered]     = accent;
    colors[HvkGuiCol_SeparatorActive]      = accent;
    colors[HvkGuiCol_ResizeGrip]           = HvkVec4(0, 0, 0, 0);
    colors[HvkGuiCol_ResizeGripHovered]    = accent;
    colors[HvkGuiCol_ResizeGripActive]     = accent;
    colors[HvkGuiCol_InputTextCursor]      = accent;
    colors[HvkGuiCol_Tab]                  = child;
    colors[HvkGuiCol_TabHovered]           = HvkLerp(child, accent, 0.25f);
    colors[HvkGuiCol_TabSelected]          = HvkLerp(child, accent, 0.35f);
    colors[HvkGuiCol_TabSelectedOverline]  = accent;
    colors[HvkGuiCol_TabDimmed]            = child;
    colors[HvkGuiCol_TabDimmedSelected]    = child;
    colors[HvkGuiCol_TabDimmedSelectedOverline] = HvkVec4(0, 0, 0, 0);
    colors[HvkGuiCol_TextSelectedBg]       = HvkLerp(accent, layout, 0.7f);
    colors[HvkGuiCol_TableHeaderBg]        = child;
    colors[HvkGuiCol_TableBorderStrong]    = border;
    colors[HvkGuiCol_TableBorderLight]     = border;
    colors[HvkGuiCol_TableRowBg]           = HvkVec4(0, 0, 0, 0);
    colors[HvkGuiCol_TableRowBgAlt]        = HvkVec4(1.0f, 1.0f, 1.0f, 0.03f);
    colors[HvkGuiCol_TextLink]             = accent;
    colors[HvkGuiCol_TreeLines]            = border;
    colors[HvkGuiCol_DragDropTarget]       = accent;
    colors[HvkGuiCol_DragDropTargetBg]     = HvkVec4(0, 0, 0, 0);
    colors[HvkGuiCol_UnsavedMarker]        = accent;
    colors[HvkGuiCol_NavCursor]            = accent;
    colors[HvkGuiCol_NavWindowingHighlight]= HvkVec4(1, 1, 1, 0.2f);
    colors[HvkGuiCol_NavWindowingDimBg]    = HvkVec4(0, 0, 0, 0.35f);
    colors[HvkGuiCol_ModalWindowDimBg]     = HvkVec4(0, 0, 0, 0.45f);
}

void HvkGui::ApplyStyleModern22(HvkGuiStyle* dst)
{
    HvkGuiStyle* style = dst ? dst : &HvkGui::GetStyle();
    StyleColorsModern22(style);

    style->WindowPadding = HvkVec2(14.0f, 12.0f);
    style->FramePadding = HvkVec2(10.0f, 6.0f);
    style->ItemSpacing = HvkVec2(10.0f, 8.0f);
    style->ItemInnerSpacing = HvkVec2(8.0f, 6.0f);
    style->ScrollbarSize = 8.0f;
    style->GrabMinSize = 8.0f;
    style->FrameBorderSize = 1.0f;
    style->WindowBorderSize = 1.0f;
}

void HvkGui::SetModernFonts(HvkFont* title, HvkFont* body, HvkFont* icon)
{
    g_modern_font_title = title;
    g_modern_font_body = body;
    g_modern_font_icon = icon;
}

bool HvkGui::ModernBeginChild(const char* label, const HvkVec2& size, bool border, HvkGuiWindowFlags flags)
{
    HvkGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    HvkGuiID id = window->GetID(label);
    HvkGui::BeginChild(id, size, border ? HvkGuiChildFlags_Borders : 0, flags);

    HvkGuiWindow* child = GetCurrentWindow();
    HvkRect rect = child->Rect();
    HvkDrawList* draw_list = child->DrawList;
    HvkGuiStyle& style = GetStyle();

    draw_list->AddRectFilled(rect.Min, rect.Max, GetColorU32(HvkGuiCol_ChildBg), style.ChildRounding);
    if (border)
        draw_list->AddRect(rect.Min, rect.Max, GetColorU32(HvkGuiCol_Border), style.ChildRounding);

    if (label && label[0] != '\0')
    {
        HvkVec2 text_pos = AddV(rect.Min, HvkVec2(12.0f, 10.0f));
        AddTextWithFont(draw_list, g_modern_font_title, text_pos, GetColorU32(HvkGuiCol_Text), label);
        SetCursorPos(HvkVec2(12.0f, 32.0f));
    }
    else
    {
        SetCursorPos(HvkVec2(12.0f, 12.0f));
    }

    return true;
}

void HvkGui::ModernEndChild()
{
    HvkGui::EndChild();
}

bool HvkGui::ModernTabButton(const char* label, bool selected, const HvkVec2& size)
{
    HvkGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    HvkGuiStyle& style = GetStyle();
    HvkGuiID id = window->GetID(label);
    HvkVec2 label_size = CalcTextSizeForFont(g_modern_font_body, label);
    HvkVec2 item_size = size;
    if (item_size.x <= 0.0f)
        item_size.x = label_size.x + style.FramePadding.x * 2.0f + 8.0f;
    if (item_size.y <= 0.0f)
        item_size.y = label_size.y + style.FramePadding.y * 2.0f;

    HvkRect bb(window->DC.CursorPos, AddV(window->DC.CursorPos, item_size));
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    float target = selected ? 1.0f : (hovered ? 0.65f : 0.0f);
    float t = ModernAnimFloat(id + 1, target, 12.0f);

    HvkVec4 base = GetStyleColorVec4(HvkGuiCol_FrameBg);
    HvkVec4 accent = GetStyleColorVec4(HvkGuiCol_CheckMark);
    HvkVec4 bg = HvkLerp(base, accent, t);

    window->DrawList->AddRectFilled(bb.Min, bb.Max, GetColorU32(bg), style.FrameRounding);
    window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(HvkGuiCol_Border), style.FrameRounding);

    HvkU32 text_col = GetColorU32(selected ? HvkGuiCol_Text : HvkGuiCol_TextDisabled);
    DrawCenteredText(window->DrawList, g_modern_font_title, bb, text_col, label);

    return pressed;
}

bool HvkGui::ModernTopTabButton(const char* label, const char* icon, bool selected, const HvkVec2& size)
{
    HvkGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    HvkGuiStyle& style = GetStyle();
    HvkGuiID id = window->GetID(label);
    HvkVec2 label_size = CalcTextSizeForFont(g_modern_font_title, label);

    float target_w = selected ? (52.0f + label_size.x) : 40.0f;
    float width = ModernAnimFloat(id + 10, target_w, 24.0f);
    HvkVec2 item_size = size;
    if (item_size.x <= 0.0f)
        item_size.x = HvkFloor(width + 0.5f);
    if (item_size.y <= 0.0f)
        item_size.y = 40.0f;

    HvkRect bb(window->DC.CursorPos, AddV(window->DC.CursorPos, item_size));
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    float t = ModernAnimFloat(id + 11, selected ? 1.0f : 0.0f, 7.0f);
    HvkVec4 bg_off = GetStyleColorVec4(HvkGuiCol_ChildBg);
    HvkVec4 bg_on = GetStyleColorVec4(HvkGuiCol_FrameBg);
    HvkVec4 bg = HvkLerp(bg_off, bg_on, t);
    if (hovered)
        bg = HvkLerp(bg, HvkVec4(1.0f, 1.0f, 1.0f, 1.0f), 0.03f);

    HvkVec4 icon_off = GetStyleColorVec4(HvkGuiCol_TextDisabled);
    HvkVec4 icon_on = GetStyleColorVec4(HvkGuiCol_Text);
    HvkVec4 icon_col = HvkLerp(icon_off, icon_on, t);

    window->DrawList->AddRectFilled(bb.Min, bb.Max, GetColorU32(bg), style.FrameRounding);

    const HvkRect icon_bb(bb.Min, AddV(bb.Min, HvkVec2(40.0f, bb.GetHeight())));
    if (icon && icon[0] != '\0')
        DrawCenteredText(window->DrawList, g_modern_font_icon, icon_bb, GetColorU32(icon_col), icon);

    if (t > 0.01f)
    {
        HvkU32 text_col = GetColorU32(HvkVec4(icon_on.x, icon_on.y, icon_on.z, t));
        HvkVec2 text_min = AddV(bb.Min, HvkVec2(40.0f, 0.0f));
        HvkVec2 text_max = SubV(bb.Max, HvkVec2(12.0f, 0.0f));
        HvkVec4 clip(text_min.x, text_min.y, text_max.x, text_max.y);
        window->DrawList->AddText(g_modern_font_title, GetFontSizeFor(g_modern_font_title), text_min, text_col, label, NULL, 0.0f, &clip);
    }

    return pressed;
}

bool HvkGui::ModernCategoryButton(const char* label, const char* description, bool* enabled)
{
    HvkGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    if (enabled == NULL)
        return false;

    HvkGuiStyle& style = GetStyle();
    HvkGuiID id = window->GetID(label);
    const float height = 64.0f;
    HvkRect bb(window->DC.CursorPos, AddV(window->DC.CursorPos, HvkVec2(GetContentRegionAvail().x, height)));

    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);
    if (pressed)
        *enabled = !*enabled;

    float t = ModernAnimFloat(id + 1, *enabled ? 1.0f : 0.0f, 10.0f);

    HvkVec4 base = GetStyleColorVec4(HvkGuiCol_FrameBg);
    HvkVec4 accent = GetStyleColorVec4(HvkGuiCol_CheckMark);
    HvkVec4 bg = HvkLerp(base, accent, t * 0.25f);
    if (hovered)
        bg = HvkLerp(bg, HvkVec4(1.0f, 1.0f, 1.0f, 1.0f), 0.04f);

    window->DrawList->AddRectFilled(bb.Min, bb.Max, GetColorU32(bg), style.FrameRounding);
    window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(HvkGuiCol_Border), style.FrameRounding);

    HvkVec2 title_pos = AddV(bb.Min, HvkVec2(14.0f, 12.0f));
    AddTextWithFont(window->DrawList, g_modern_font_title, title_pos, GetColorU32(HvkGuiCol_Text), label);
    if (description && description[0] != '\0')
        AddTextWithFont(window->DrawList, g_modern_font_body, AddV(title_pos, HvkVec2(0.0f, 20.0f)), GetColorU32(HvkGuiCol_TextDisabled), description);

    HvkRect toggle(SubV(bb.Max, HvkVec2(52.0f, 30.0f)), SubV(bb.Max, HvkVec2(14.0f, 12.0f)));
    float toggle_t = ModernAnimFloat(id + 2, *enabled ? 1.0f : 0.0f, 12.0f);
    HvkVec4 toggle_col = HvkLerp(base, accent, toggle_t);
    window->DrawList->AddRectFilled(toggle.Min, toggle.Max, GetColorU32(toggle_col), toggle.GetHeight() * 0.5f);
    HvkVec2 knob = HvkVec2(toggle.Min.x + toggle.GetHeight() * 0.5f + toggle_t * (toggle.GetWidth() - toggle.GetHeight()), toggle.GetCenter().y);
    window->DrawList->AddCircleFilled(knob, toggle.GetHeight() * 0.35f, GetColorU32(HvkGuiCol_Text));

    return pressed;
}

bool HvkGui::ModernCheckbox(const char* label, const char* description, bool* v)
{
    HvkGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    if (v == NULL)
        return false;

    HvkGuiStyle& style = GetStyle();
    HvkGuiID id = window->GetID(label);
    const float height = 56.0f;
    HvkRect bb(window->DC.CursorPos, AddV(window->DC.CursorPos, HvkVec2(GetContentRegionAvail().x, height)));

    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);
    if (pressed)
        *v = !*v;

    float t = ModernAnimFloat(id + 1, *v ? 1.0f : 0.0f, 10.0f);

    HvkVec4 base = GetStyleColorVec4(HvkGuiCol_FrameBg);
    HvkVec4 accent = GetStyleColorVec4(HvkGuiCol_CheckMark);
    HvkVec4 bg = HvkLerp(base, accent, t * 0.2f);
    if (hovered)
        bg = HvkLerp(bg, HvkVec4(1.0f, 1.0f, 1.0f, 1.0f), 0.04f);

    window->DrawList->AddRectFilled(bb.Min, bb.Max, GetColorU32(bg), style.FrameRounding);
    window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(HvkGuiCol_Border), style.FrameRounding);

    HvkVec2 title_pos = AddV(bb.Min, HvkVec2(14.0f, 10.0f));
    AddTextWithFont(window->DrawList, g_modern_font_title, title_pos, GetColorU32(HvkGuiCol_Text), label);
    if (description && description[0] != '\0')
        AddTextWithFont(window->DrawList, g_modern_font_body, AddV(title_pos, HvkVec2(0.0f, 20.0f)), GetColorU32(HvkGuiCol_TextDisabled), description);

    HvkRect toggle(SubV(bb.Max, HvkVec2(46.0f, 28.0f)), SubV(bb.Max, HvkVec2(14.0f, 12.0f)));
    HvkVec4 toggle_col = HvkLerp(base, accent, t);
    window->DrawList->AddRectFilled(toggle.Min, toggle.Max, GetColorU32(toggle_col), toggle.GetHeight() * 0.5f);
    HvkVec2 knob = HvkVec2(toggle.Min.x + toggle.GetHeight() * 0.5f + t * (toggle.GetWidth() - toggle.GetHeight()), toggle.GetCenter().y);
    window->DrawList->AddCircleFilled(knob, toggle.GetHeight() * 0.33f, GetColorU32(HvkGuiCol_Text));

    return pressed;
}

bool HvkGui::ModernSliderFloat(const char* label, const char* description, float* v, float v_min, float v_max, const char* format)
{
    HvkGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    if (v == NULL)
        return false;

    HvkGuiStyle& style = GetStyle();
    HvkGuiID id = window->GetID(label);
    const float height = 64.0f;
    HvkRect bb(window->DC.CursorPos, AddV(window->DC.CursorPos, HvkVec2(GetContentRegionAvail().x, height)));

    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    HvkRect slider_bb(AddV(bb.Min, HvkVec2(14.0f, height - 18.0f)), SubV(bb.Max, HvkVec2(14.0f, 10.0f)));
    bool changed = ModernSliderBehavior(slider_bb, id, v, v_min, v_max);

    float t = HvkSaturate((*v - v_min) / (v_max - v_min));
    float anim = ModernAnimFloat(id + 1, t, 10.0f);

    window->DrawList->AddRectFilled(bb.Min, bb.Max, GetColorU32(HvkGuiCol_FrameBg), style.FrameRounding);
    window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(HvkGuiCol_Border), style.FrameRounding);

    HvkVec2 title_pos = AddV(bb.Min, HvkVec2(14.0f, 10.0f));
    AddTextWithFont(window->DrawList, g_modern_font_title, title_pos, GetColorU32(HvkGuiCol_Text), label);
    if (description && description[0] != '\0')
        AddTextWithFont(window->DrawList, g_modern_font_body, AddV(title_pos, HvkVec2(0.0f, 20.0f)), GetColorU32(HvkGuiCol_TextDisabled), description);

    char value_buf[64];
    HvkFormatString(value_buf, Hvk_ARRAYSIZE(value_buf), format, *v);
    HvkVec2 value_size = CalcTextSizeForFont(g_modern_font_title, value_buf);
    HvkVec2 value_pos = HvkVec2(bb.Max.x - value_size.x - 14.0f, bb.Min.y + 10.0f);
    AddTextWithFont(window->DrawList, g_modern_font_title, value_pos, GetColorU32(HvkGuiCol_Text), value_buf);

    window->DrawList->AddRectFilled(slider_bb.Min, slider_bb.Max, GetColorU32(HvkGuiCol_WindowBg), slider_bb.GetHeight() * 0.5f);
    HvkVec2 fill_max = HvkVec2(slider_bb.Min.x + slider_bb.GetWidth() * anim, slider_bb.Max.y);
    window->DrawList->AddRectFilled(slider_bb.Min, fill_max, GetColorU32(HvkGuiCol_CheckMark), slider_bb.GetHeight() * 0.5f);
    HvkVec2 knob = HvkVec2(fill_max.x, slider_bb.GetCenter().y);
    window->DrawList->AddCircleFilled(knob, slider_bb.GetHeight() * 0.75f, GetColorU32(HvkGuiCol_Text));

    return changed;
}

bool HvkGui::ModernCombo(const char* label, const char* description, int* current_item, const char* const items[], int items_count)
{
    HvkGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    if (current_item == NULL || items == NULL || items_count <= 0)
        return false;

    HvkGuiStyle& style = GetStyle();
    HvkGuiID id = window->GetID(label);
    const float height = 64.0f;
    HvkRect bb(window->DC.CursorPos, AddV(window->DC.CursorPos, HvkVec2(GetContentRegionAvail().x, height)));

    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    HvkRect combo_bb(AddV(bb.Min, HvkVec2(14.0f, height - 22.0f)), SubV(bb.Max, HvkVec2(14.0f, 12.0f)));
    const char* preview = (current_item && *current_item >= 0 && *current_item < items_count) ? items[*current_item] : "";

    HvkGuiID popup_id = window->GetID(label);
    PushID(label);
    SetCursorScreenPos(combo_bb.Min);
    bool pressed = InvisibleButton("##combo_btn", combo_bb.GetSize());
    PopID();
    if (pressed)
        OpenPopupEx(popup_id, HvkGuiPopupFlags_None);

    bool open = BeginPopupEx(popup_id, HvkGuiWindowFlags_AlwaysAutoResize);

    window->DrawList->AddRectFilled(bb.Min, bb.Max, GetColorU32(HvkGuiCol_FrameBg), style.FrameRounding);
    window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(HvkGuiCol_Border), style.FrameRounding);

    HvkVec2 title_pos = AddV(bb.Min, HvkVec2(14.0f, 10.0f));
    AddTextWithFont(window->DrawList, g_modern_font_title, title_pos, GetColorU32(HvkGuiCol_Text), label);
    if (description && description[0] != '\0')
        AddTextWithFont(window->DrawList, g_modern_font_body, AddV(title_pos, HvkVec2(0.0f, 20.0f)), GetColorU32(HvkGuiCol_TextDisabled), description);

    window->DrawList->AddRectFilled(combo_bb.Min, combo_bb.Max, GetColorU32(HvkGuiCol_WindowBg), combo_bb.GetHeight() * 0.5f);
    window->DrawList->AddRect(combo_bb.Min, combo_bb.Max, GetColorU32(HvkGuiCol_Border), combo_bb.GetHeight() * 0.5f);
    HvkVec2 preview_size = CalcTextSizeForFont(g_modern_font_body, preview);
    HvkVec2 text_pos = AddV(combo_bb.Min, HvkVec2(10.0f, (combo_bb.GetHeight() - preview_size.y) * 0.5f));
    AddTextWithFont(window->DrawList, g_modern_font_body, text_pos, GetColorU32(HvkGuiCol_Text), preview);

    if (open)
    {
        for (int n = 0; n < items_count; n++)
        {
            const bool is_selected = (current_item && *current_item == n);
            if (Selectable(items[n], is_selected))
            {
                if (current_item)
                    *current_item = n;
            }
        }
        EndPopup();
    }

    return open;
}

bool HvkGui::ModernSelectable(const char* label, bool selected, const HvkVec2& size)
{
    HvkGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    HvkGuiStyle& style = GetStyle();
    HvkGuiID id = window->GetID(label);
    HvkVec2 label_size = CalcTextSize(label, NULL, true);
    HvkVec2 item_size = size;
    if (item_size.x <= 0.0f)
        item_size.x = GetContentRegionAvail().x;
    if (item_size.y <= 0.0f)
        item_size.y = label_size.y + style.FramePadding.y * 2.0f;

    HvkRect bb(window->DC.CursorPos, AddV(window->DC.CursorPos, item_size));
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);
    if (pressed)
        selected = !selected;

    float target = selected ? 1.0f : (hovered ? 0.4f : 0.0f);
    float t = ModernAnimFloat(id + 1, target, 10.0f);
    HvkVec4 base = GetStyleColorVec4(HvkGuiCol_FrameBg);
    HvkVec4 accent = GetStyleColorVec4(HvkGuiCol_CheckMark);
    HvkVec4 bg = HvkLerp(base, accent, t * 0.2f);

    window->DrawList->AddRectFilled(bb.Min, bb.Max, GetColorU32(bg), style.FrameRounding);
    window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(HvkGuiCol_Border), style.FrameRounding);
    AddTextWithFont(window->DrawList, g_modern_font_body, AddV(bb.Min, HvkVec2(12.0f, (bb.GetHeight() - label_size.y) * 0.5f)), GetColorU32(HvkGuiCol_Text), label);

    return pressed;
}

bool HvkGui::ModernColorPicker(const char* label, HvkVec4* color)
{
    HvkGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    if (color == NULL)
        return false;

    HvkGuiID id = window->GetID(label);
    HvkGuiStorage* storage = GetStateStorage();
    float* h = storage->GetFloatRef(id + 1, 0.0f);
    float* s = storage->GetFloatRef(id + 2, 0.0f);
    float* v = storage->GetFloatRef(id + 3, 0.0f);
    ColorConvertRGBtoHSV(color->x, color->y, color->z, *h, *s, *v);

    HvkVec4 old = *color;

    const float height = 90.0f;
    HvkRect bb(window->DC.CursorPos, AddV(window->DC.CursorPos, HvkVec2(GetContentRegionAvail().x, height)));
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    window->DrawList->AddRectFilled(bb.Min, bb.Max, GetColorU32(HvkGuiCol_FrameBg), GetStyle().FrameRounding);
    window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(HvkGuiCol_Border), GetStyle().FrameRounding);
    AddTextWithFont(window->DrawList, g_modern_font_title, AddV(bb.Min, HvkVec2(14.0f, 10.0f)), GetColorU32(HvkGuiCol_Text), label);

    float slider_width = bb.GetWidth() - 28.0f;
    HvkVec2 slider_pos = AddV(bb.Min, HvkVec2(14.0f, 36.0f));
    HvkRect sat_bb(slider_pos, AddV(slider_pos, HvkVec2(slider_width, 8.0f)));
    HvkRect val_bb(AddV(slider_pos, HvkVec2(0.0f, 18.0f)), AddV(slider_pos, HvkVec2(slider_width, 26.0f)));
    HvkRect hue_bb(AddV(slider_pos, HvkVec2(0.0f, 36.0f)), AddV(slider_pos, HvkVec2(slider_width, 44.0f)));

    ModernSliderBehavior(sat_bb, id + 4, s, 0.0f, 1.0f);
    ModernSliderBehavior(val_bb, id + 5, v, 0.0f, 1.0f);
    ModernSliderBehavior(hue_bb, id + 6, h, 0.0f, 1.0f);

    HvkU32 sat_a = GetColorU32(HvkVec4(1, 1, 1, 1));
    HvkU32 sat_b = GetColorU32(HSVToRGB(*h, 1.0f, 1.0f));
    window->DrawList->AddRectFilledMultiColor(sat_bb.Min, sat_bb.Max, sat_a, sat_b, sat_b, sat_a);

    HvkU32 val_a = GetColorU32(HvkVec4(0, 0, 0, 1));
    HvkU32 val_b = GetColorU32(HvkVec4(1, 1, 1, 1));
    window->DrawList->AddRectFilledMultiColor(val_bb.Min, val_bb.Max, val_a, val_b, val_b, val_a);

    const int hue_steps = 6;
    for (int i = 0; i < hue_steps; ++i)
    {
        float t0 = i / float(hue_steps);
        float t1 = (i + 1) / float(hue_steps);
        HvkU32 c0 = GetColorU32(HSVToRGB(t0, 1.0f, 1.0f));
        HvkU32 c1 = GetColorU32(HSVToRGB(t1, 1.0f, 1.0f));
        HvkVec2 p0 = HvkVec2(HvkLerp(hue_bb.Min.x, hue_bb.Max.x, t0), hue_bb.Min.y);
        HvkVec2 p1 = HvkVec2(HvkLerp(hue_bb.Min.x, hue_bb.Max.x, t1), hue_bb.Max.y);
        window->DrawList->AddRectFilledMultiColor(p0, p1, c0, c1, c1, c0);
    }

    auto draw_knob = [&](const HvkRect& bb, float t)
    {
        HvkVec2 center = HvkVec2(bb.Min.x + bb.GetWidth() * HvkSaturate(t), bb.GetCenter().y);
        window->DrawList->AddCircleFilled(center, 5.0f, GetColorU32(HvkGuiCol_Text));
    };

    draw_knob(sat_bb, *s);
    draw_knob(val_bb, *v);
    draw_knob(hue_bb, *h);

    ColorConvertHSVtoRGB(*h, *s, *v, color->x, color->y, color->z);

    return (old.x != color->x || old.y != color->y || old.z != color->z);
}

#endif // HvkGui_DISABLE
