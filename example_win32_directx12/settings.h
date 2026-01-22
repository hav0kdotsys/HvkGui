#include "hvkgui.h"

class c_user_settings
{
    public:

    bool show_style_editor = false;

    struct {
        struct {
            /* Main Window */
            float r;
            float g;
            float b;
            float a;
        } color;
    } style;

};

inline c_user_settings* user_settings = new c_user_settings();

void hvkgui_apply_user_settings()
{
    HvkGuiStyle& style = HvkGui::GetStyle();
    style.Colors[HvkGuiCol_WindowBg].x = user_settings->style.color.r;
    style.Colors[HvkGuiCol_WindowBg].y = user_settings->style.color.g;
    style.Colors[HvkGuiCol_WindowBg].z = user_settings->style.color.b;
    style.Colors[HvkGuiCol_WindowBg].w = user_settings->style.color.a;
}