#pragma once

#ifndef HvkGui_DISABLE

namespace HvkGui
{
    // Modern 22 theme helpers and widgets (style inspired by ImGuiMenu-s/22).
    HvkGui_API void StyleColorsModern22(HvkGuiStyle* dst = NULL);
    HvkGui_API void ApplyStyleModern22(HvkGuiStyle* dst = NULL);
    HvkGui_API void SetModernFonts(HvkFont* title, HvkFont* body, HvkFont* icon = NULL);

    HvkGui_API bool ModernBeginChild(const char* label, const HvkVec2& size = HvkVec2(0, 0), bool border = true, HvkGuiWindowFlags flags = 0);
    HvkGui_API void ModernEndChild();

    HvkGui_API bool ModernTabButton(const char* label, bool selected, const HvkVec2& size = HvkVec2(0, 0));
    HvkGui_API bool ModernTopTabButton(const char* label, const char* icon, bool selected, const HvkVec2& size = HvkVec2(0, 0));
    HvkGui_API bool ModernCategoryButton(const char* label, const char* description, bool* enabled);
    HvkGui_API bool ModernCheckbox(const char* label, const char* description, bool* v);
    HvkGui_API bool ModernSliderFloat(const char* label, const char* description, float* v, float v_min, float v_max, const char* format = "%.2f");
    HvkGui_API bool ModernCombo(const char* label, const char* description, int* current_item, const char* const items[], int items_count);
    HvkGui_API bool ModernSelectable(const char* label, bool selected, const HvkVec2& size = HvkVec2(0, 0));
    HvkGui_API bool ModernColorPicker(const char* label, HvkVec4* color);
}

#endif // HvkGui_DISABLE
