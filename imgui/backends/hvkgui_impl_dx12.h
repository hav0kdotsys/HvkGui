// dear HvkGui: Renderer Backend for DirectX12
// This needs to be used along with a Platform Backend (e.g. Win32)

// implemented features:
//  [X] Renderer: User texture binding. Use 'D3D12_GPU_DESCRIPTOR_HANDLE' as texture identifier. Read the FAQ about HvkTextureID/HvkTextureRef!
//  [X] Renderer: Large meshes support (64k+ vertices) even with 16-bit indices (HvkGuiBackendFlags_RendererHasVtxOffset).
//  [X] Renderer: Texture updates support for dynamic font atlas (HvkGuiBackendFlags_RendererHasTextures).
//  [X] Renderer: Expose selected render state for draw callbacks to use. Access in '(HvkGui_ImplXXXX_RenderState*)GetPlatformIO().Renderer_RenderState'.

// The aim of HvkGui_impl_dx12.h/.cpp is to be usable in your engine without any modification.
// IF YOU FEEL YOU NEED TO MAKE ANY CHANGE TO THIS CODE, please share them and your feedback at https://github.com/ocornut/HvkGui/

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
#include <dxgiformat.h> // DXGI_FORMAT
#include <d3d12.h>      // D3D12_CPU_DESCRIPTOR_HANDLE

enum HvkGui_ImplDX12_OutputMode
{
    HvkGui_ImplDX12_OutputMode_MRT = 0,
    HvkGui_ImplDX12_OutputMode_BaseOnly,
    HvkGui_ImplDX12_OutputMode_EmissiveOnly,
    HvkGui_ImplDX12_OutputMode_LDR
};

// Initialization data, for HvkGui_ImplDX12_Init()
struct HvkGui_ImplDX12_InitInfo
{
    ID3D12Device*               Device;
    ID3D12CommandQueue*         CommandQueue;       // Command queue used for queuing texture uploads.
    int                         NumFramesInFlight;
    DXGI_FORMAT                 RTVFormat;          // RenderTarget format.
    DXGI_FORMAT                 DSVFormat;          // DepthStencilView format.
    void*                       UserData;

    // Allocating SRV descriptors for textures is up to the application, so we provide callbacks.
    // (current version of the backend will only allocate one descriptor, from 1.92 the backend will need to allocate more)
    ID3D12DescriptorHeap*       SrvDescriptorHeap;
    void                        (*SrvDescriptorAllocFn)(HvkGui_ImplDX12_InitInfo* info, D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_desc_handle);
    void                        (*SrvDescriptorFreeFn)(HvkGui_ImplDX12_InitInfo* info, D3D12_CPU_DESCRIPTOR_HANDLE cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_desc_handle);
#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
    D3D12_CPU_DESCRIPTOR_HANDLE LegacySingleSrvCpuDescriptor; // To facilitate transition from single descriptor to allocator callback, you may use those.
    D3D12_GPU_DESCRIPTOR_HANDLE LegacySingleSrvGpuDescriptor;
#endif

    HvkGui_ImplDX12_InitInfo()   { memset(this, 0, sizeof(*this)); }
};

// Follow "Getting Started" link and check examples/ folder to learn about using backends!
HvkGui_IMPL_API bool     HvkGui_ImplDX12_Init(HvkGui_ImplDX12_InitInfo* info);
HvkGui_IMPL_API void     HvkGui_ImplDX12_Shutdown();
HvkGui_IMPL_API void     HvkGui_ImplDX12_NewFrame();
HvkGui_IMPL_API void     HvkGui_ImplDX12_RenderDrawData(HvkDrawData* draw_data, ID3D12GraphicsCommandList* graphics_command_list);
HvkGui_IMPL_API void     HvkGui_ImplDX12_SetOutputMode(HvkGui_ImplDX12_OutputMode mode);
HvkGui_IMPL_API void     HvkGui_ImplDX12_SetLdrFormat(DXGI_FORMAT format);

#ifndef HvkGui_DISABLE_OBSOLETE_FUNCTIONS
// Legacy initialization API Obsoleted in 1.91.5
// - font_srv_cpu_desc_handle and font_srv_gpu_desc_handle are handles to a single SRV descriptor to use for the internal font texture, they must be in 'srv_descriptor_heap'
// - When we introduced the HvkGui_ImplDX12_InitInfo struct we also added a 'ID3D12CommandQueue* CommandQueue' field.
HvkGui_IMPL_API bool     HvkGui_ImplDX12_Init(ID3D12Device* device, int num_frames_in_flight, DXGI_FORMAT rtv_format, ID3D12DescriptorHeap* srv_descriptor_heap, D3D12_CPU_DESCRIPTOR_HANDLE font_srv_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE font_srv_gpu_desc_handle);
#endif

// Use if you want to reset your rendering device without losing Dear HvkGui state.
HvkGui_IMPL_API bool     HvkGui_ImplDX12_CreateDeviceObjects();
HvkGui_IMPL_API void     HvkGui_ImplDX12_InvalidateDeviceObjects();

// (Advanced) Use e.g. if you need to precisely control the timing of texture updates (e.g. for staged rendering), by setting HvkDrawData::Textures = NULL to handle this manually.
HvkGui_IMPL_API void     HvkGui_ImplDX12_UpdateTexture(HvkTextureData* tex);

// [BETA] Selected render state data shared with callbacks.
// This is temporarily stored in GetPlatformIO().Renderer_RenderState during the HvkGui_ImplDX12_RenderDrawData() call.
// (Please open an issue if you feel you need access to more data)
struct HvkGui_ImplDX12_RenderState
{
    ID3D12Device*               Device;
    ID3D12GraphicsCommandList*  CommandList;
};

#endif // #ifndef HvkGui_DISABLE
