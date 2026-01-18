// dear HvkGui: Renderer Backend for DirectX11
// This needs to be used along with a Platform Backend (e.g. Win32)

// implemented features:
//  [X] Renderer: User texture binding. Use 'ID3D11ShaderResourceView*' as texture identifier. Read the FAQ about HvkTextureID/HvkTextureRef!
//  [X] Renderer: Large meshes support (64k+ vertices) even with 16-bit indices (HvkGuiBackendFlags_RendererHasVtxOffset).
//  [X] Renderer: Texture updates support for dynamic font atlas (HvkGuiBackendFlags_RendererHasTextures).
//  [X] Renderer: Expose selected render state for draw callbacks to use. Access in '(HvkGui_ImplXXXX_RenderState*)GetPlatformIO().Renderer_RenderState'.

// You can use unmodified HvkGui_impl_* files in your project. See examples/ folder for examples of using this.
// Prefer including the entire HvkGui/ repository into your project (either as a copy or as a submodule), and only build the backends you need.
// Learn about Dear HvkGui:
// - FAQ                  https://dearHvkGui.com/faq
// - Getting Started      https://dearHvkGui.com/getting-started
// - Documentation        https://dearHvkGui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of HvkGui.cpp

#pragma once
#include "hvkgui.h"      // HvkGui_IMPL_API
#ifndef HvkGui_DISABLE

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11SamplerState;
struct ID3D11Buffer;

enum HvkGui_ImplDX11_OutputMode
{
    HvkGui_ImplDX11_OutputMode_MRT = 0,
    HvkGui_ImplDX11_OutputMode_BaseOnly,
    HvkGui_ImplDX11_OutputMode_EmissiveOnly,
    HvkGui_ImplDX11_OutputMode_LDR
};

// Follow "Getting Started" link and check examples/ folder to learn about using backends!
HvkGui_IMPL_API bool     HvkGui_ImplDX11_Init(ID3D11Device* device, ID3D11DeviceContext* device_context);
HvkGui_IMPL_API void     HvkGui_ImplDX11_Shutdown();
HvkGui_IMPL_API void     HvkGui_ImplDX11_NewFrame();
HvkGui_IMPL_API void     HvkGui_ImplDX11_RenderDrawData(HvkDrawData* draw_data);
HvkGui_IMPL_API void     HvkGui_ImplDX11_SetOutputMode(HvkGui_ImplDX11_OutputMode mode);
HvkGui_IMPL_API void     HvkGui_ImplDX11_SetForceEmissiveFromBase(bool enable);

// Use if you want to reset your rendering device without losing Dear HvkGui state.
HvkGui_IMPL_API bool     HvkGui_ImplDX11_CreateDeviceObjects();
HvkGui_IMPL_API void     HvkGui_ImplDX11_InvalidateDeviceObjects();

// (Advanced) Use e.g. if you need to precisely control the timing of texture updates (e.g. for staged rendering), by setting HvkDrawData::Textures = NULL to handle this manually.
HvkGui_IMPL_API void     HvkGui_ImplDX11_UpdateTexture(HvkTextureData* tex);

// [BETA] Selected render state data shared with callbacks.
// This is temporarily stored in GetPlatformIO().Renderer_RenderState during the HvkGui_ImplDX11_RenderDrawData() call.
// (Please open an issue if you feel you need access to more data)
struct HvkGui_ImplDX11_RenderState
{
    ID3D11Device*           Device;
    ID3D11DeviceContext*    DeviceContext;
    ID3D11SamplerState*     SamplerDefault;
    ID3D11Buffer*           VertexConstantBuffer;
};

#endif // #ifndef HvkGui_DISABLE
