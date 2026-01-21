// Dear HvkGui: standalone example application for Windows API + DirectX 12

// Learn about Dear HvkGui:
// - FAQ                  https://dearHvkGui.com/faq
// - Getting Started      https://dearHvkGui.com/getting-started
// - Documentation        https://dearHvkGui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of HvkGui.cpp
#define _CRT_SECURE_NO_WARNINGS

#include "hvkgui.h"
#include "hvkgui_internal.h"
#include "../imgui/backends/hvkgui_impl_win32.h"
#include "../imgui/backends/hvkgui_impl_dx11.h"
#include "../imgui/backends/hvkgui_impl_dx12.h"
#include <d3d12.h>
#include <dxgi1_5.h>
#include <tchar.h>
#include <wincodec.h>
#include <d3dcompiler.h>
#include <cstdarg>
#include <cstdio>

#include <d3d11.h>

#include "../util/util.h"

#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#define DX12_ENABLE_DEBUG_LAYER
#endif

#ifdef DX12_ENABLE_DEBUG_LAYER
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif

// Config for example app
static const int APP_NUM_FRAMES_IN_FLIGHT = 2;
static const int APP_NUM_BACK_BUFFERS = 2;
static const int APP_SRV_HEAP_SIZE = 64;
static const int BLOOM_MIP_COUNT = 4;

#ifdef _DEV
static RenderBackend g_render_backend = RenderBackend::DX11;
#else
static RenderBackend g_render_backend = RenderBackend::DX12;
#endif

enum EmissiveRenderPath
{
	EmissiveRenderPath_MRT = 0,
	EmissiveRenderPath_TwoPass
};

#ifdef _DEBUG
static EmissiveRenderPath g_emissiveRenderPath = EmissiveRenderPath_TwoPass;
#else
static EmissiveRenderPath g_emissiveRenderPath = EmissiveRenderPath_MRT;
#endif

struct FrameContext
{
	ID3D12CommandAllocator *CommandAllocator;
	UINT64 FenceValue;
};

// Simple free list based allocator
struct ExampleDescriptorHeapAllocator
{
	ID3D12DescriptorHeap *Heap = nullptr;
	D3D12_DESCRIPTOR_HEAP_TYPE HeapType = D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES;
	D3D12_CPU_DESCRIPTOR_HANDLE HeapStartCpu;
	D3D12_GPU_DESCRIPTOR_HANDLE HeapStartGpu;
	UINT HeapHandleIncrement;
	HvkVector<int> FreeIndices;

	void Create(ID3D12Device *device, ID3D12DescriptorHeap *heap)
	{
		Hvk_ASSERT(Heap == nullptr && FreeIndices.empty());
		Heap = heap;
		D3D12_DESCRIPTOR_HEAP_DESC desc = heap->GetDesc();
		HeapType = desc.Type;
		HeapStartCpu = Heap->GetCPUDescriptorHandleForHeapStart();
		HeapStartGpu = Heap->GetGPUDescriptorHandleForHeapStart();
		HeapHandleIncrement = device->GetDescriptorHandleIncrementSize(HeapType);
		FreeIndices.reserve((int)desc.NumDescriptors);
		for (int n = desc.NumDescriptors; n > 0; n--)
			FreeIndices.push_back(n - 1);
	}
	void Destroy()
	{
		Heap = nullptr;
		FreeIndices.clear();
	}
	void Alloc(D3D12_CPU_DESCRIPTOR_HANDLE *out_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE *out_gpu_desc_handle)
	{
		Hvk_ASSERT(FreeIndices.Size > 0);
		int idx = FreeIndices.back();
		FreeIndices.pop_back();
		out_cpu_desc_handle->ptr = HeapStartCpu.ptr + (idx * HeapHandleIncrement);
		out_gpu_desc_handle->ptr = HeapStartGpu.ptr + (idx * HeapHandleIncrement);
	}
	void Free(D3D12_CPU_DESCRIPTOR_HANDLE out_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE out_gpu_desc_handle)
	{
		int cpu_idx = (int)((out_cpu_desc_handle.ptr - HeapStartCpu.ptr) / HeapHandleIncrement);
		int gpu_idx = (int)((out_gpu_desc_handle.ptr - HeapStartGpu.ptr) / HeapHandleIncrement);
		Hvk_ASSERT(cpu_idx == gpu_idx);
		FreeIndices.push_back(cpu_idx);
	}
};

// Data
static FrameContext g_frameContext[APP_NUM_FRAMES_IN_FLIGHT] = {};
static UINT g_frameIndex = 0;

static ID3D12Device *g_pd3dDevice = nullptr;
static ID3D12DescriptorHeap *g_pd3dRtvDescHeap = nullptr;
static ID3D12DescriptorHeap *g_pd3dSrvDescHeap = nullptr;
static ID3D12DescriptorHeap *g_pd3dPostSrvDescHeap = nullptr;
static ExampleDescriptorHeapAllocator g_pd3dSrvDescHeapAlloc;
static ID3D12CommandQueue *g_pd3dCommandQueue = nullptr;
static ID3D12GraphicsCommandList *g_pd3dCommandList = nullptr;
static ID3D12Fence *g_fence = nullptr;
static HANDLE g_fenceEvent = nullptr;
static UINT64 g_fenceLastSignaledValue = 0;
static IDXGISwapChain3 *g_pSwapChain = nullptr;
static bool g_SwapChainTearingSupport = false;
static bool g_SwapChainOccluded = false;
static HANDLE g_hSwapChainWaitableObject = nullptr;
static ID3D12Resource *g_mainRenderTargetResource[APP_NUM_BACK_BUFFERS] = {};
static D3D12_CPU_DESCRIPTOR_HANDLE g_mainRenderTargetDescriptor[APP_NUM_BACK_BUFFERS] = {};
static ID3D12Resource *g_hdrRenderTargetResource = nullptr;
static D3D12_CPU_DESCRIPTOR_HANDLE g_hdrRenderTargetDescriptor = {};
static D3D12_CPU_DESCRIPTOR_HANDLE g_hdrSrvCpuHandle = {};
static D3D12_GPU_DESCRIPTOR_HANDLE g_hdrSrvGpuHandle = {};
static ID3D12Resource *g_hdrEmissiveRenderTargetResource = nullptr;
static D3D12_CPU_DESCRIPTOR_HANDLE g_hdrEmissiveRenderTargetDescriptor = {};
static D3D12_CPU_DESCRIPTOR_HANDLE g_hdrEmissiveSrvCpuHandle = {};
static D3D12_GPU_DESCRIPTOR_HANDLE g_hdrEmissiveSrvGpuHandle = {};
static D3D12_CPU_DESCRIPTOR_HANDLE g_postSrvHeapStartCpu = {};
static D3D12_GPU_DESCRIPTOR_HANDLE g_postSrvHeapStartGpu = {};
static UINT g_postSrvDescriptorSize = 0;
static ID3D12RootSignature *g_tonemapRootSignature = nullptr;
static ID3D12PipelineState *g_tonemapPipelineState = nullptr;
static ID3D12PipelineState *g_emissiveAddPipelineState = nullptr;
static ID3D12PipelineState *g_bloomPrefilterPSO = nullptr;
static ID3D12PipelineState *g_bloomDownsamplePSO = nullptr;
static ID3D12PipelineState *g_bloomBlurPSO = nullptr;
static ID3D12PipelineState *g_bloomUpsamplePSO = nullptr;
static ID3D12Resource *g_bloomDownTex12[BLOOM_MIP_COUNT] = {};
static ID3D12Resource *g_bloomBlurTex12[BLOOM_MIP_COUNT] = {};
static D3D12_CPU_DESCRIPTOR_HANDLE g_bloomDownRtv12[BLOOM_MIP_COUNT] = {};
static D3D12_CPU_DESCRIPTOR_HANDLE g_bloomBlurRtv12[BLOOM_MIP_COUNT] = {};
static D3D12_CPU_DESCRIPTOR_HANDLE g_bloomDownSrvCpu12[BLOOM_MIP_COUNT] = {};
static D3D12_GPU_DESCRIPTOR_HANDLE g_bloomDownSrvGpu12[BLOOM_MIP_COUNT] = {};
static D3D12_CPU_DESCRIPTOR_HANDLE g_bloomBlurSrvCpu12[BLOOM_MIP_COUNT] = {};
static D3D12_GPU_DESCRIPTOR_HANDLE g_bloomBlurSrvGpu12[BLOOM_MIP_COUNT] = {};
static UINT g_bloomWidth12[BLOOM_MIP_COUNT] = {};
static UINT g_bloomHeight12[BLOOM_MIP_COUNT] = {};

// ---------------- DX11 data ----------------
static ID3D11Device *g_pd3dDevice11 = nullptr;
static ID3D11DeviceContext *g_pd3dDeviceContext11 = nullptr;
static IDXGISwapChain *g_pSwapChain11 = nullptr;
static ID3D11RenderTargetView *g_mainRenderTargetView11 = nullptr;
static ID3D11Texture2D *g_hdrTexture11 = nullptr;
static ID3D11RenderTargetView *g_hdrRTV11 = nullptr;
static ID3D11ShaderResourceView *g_hdrSRV11 = nullptr;
static ID3D11Texture2D *g_hdrEmissiveTexture11 = nullptr;
static ID3D11RenderTargetView *g_hdrEmissiveRTV11 = nullptr;
static ID3D11ShaderResourceView *g_hdrEmissiveSRV11 = nullptr;
static ID3D11VertexShader *g_tonemapVS11 = nullptr;
static ID3D11PixelShader *g_tonemapPS11 = nullptr;
static ID3D11SamplerState *g_tonemapSampler11 = nullptr;
static ID3D11BlendState *g_tonemapBlend11 = nullptr;
static ID3D11BlendState *g_tonemapAddBlend11 = nullptr;
static ID3D11RasterizerState *g_tonemapRasterizer11 = nullptr;
static ID3D11DepthStencilState *g_tonemapDepth11 = nullptr;
static ID3D11Texture2D *g_bloomDownTex11[BLOOM_MIP_COUNT] = {};
static ID3D11RenderTargetView *g_bloomDownRTV11[BLOOM_MIP_COUNT] = {};
static ID3D11ShaderResourceView *g_bloomDownSRV11[BLOOM_MIP_COUNT] = {};
static ID3D11Texture2D *g_bloomBlurTex11[BLOOM_MIP_COUNT] = {};
static ID3D11RenderTargetView *g_bloomBlurRTV11[BLOOM_MIP_COUNT] = {};
static ID3D11ShaderResourceView *g_bloomBlurSRV11[BLOOM_MIP_COUNT] = {};
static ID3D11VertexShader *g_bloomVS11 = nullptr;
static ID3D11PixelShader *g_bloomPrefilterPS11 = nullptr;
static ID3D11PixelShader *g_bloomDownsamplePS11 = nullptr;
static ID3D11PixelShader *g_bloomBlurPS11 = nullptr;
static ID3D11PixelShader *g_bloomUpsamplePS11 = nullptr;
static ID3D11SamplerState *g_bloomSampler11 = nullptr;
static ID3D11BlendState *g_bloomAdditiveBlend11 = nullptr;
static ID3D11Buffer *g_bloomConstants11 = nullptr;
static float g_bloomThreshold = 1.0f;
static float g_bloomKnee = 0.5f;
static float g_bloomIntensity = 1.0f;

static HvkTextureData *g_waterTexture = nullptr;
static HvkTextureRef g_waterTextureRef;
static HvkTextureData *g_waterEmissiveTexture = nullptr;
static HvkTextureRef g_waterEmissiveTextureRef;
static float g_waterScrollSpeed = 0.15f;
static float g_waterScrollOffset = 0.0f;
static HvkTextureData *g_borderTexture = nullptr;
static HvkTextureRef g_borderTextureRef;
static bool g_showEmissiveTextWindow = true;
static float g_bgEmissiveStrength = 2.5f;
static HvkVec4 g_bgEmissiveTint = HvkVec4(0.25f, 0.75f, 1.0f, 1.0f);
static float g_textEmissiveStrength = 2.0f;
static HvkVec4 g_textEmissiveTint = HvkVec4(1.0f, 0.85f, 0.25f, 1.0f);
#ifdef _DEBUG
static bool g_debugShowEmissive = false;
static int g_debugUiVtxCount = 0;
static int g_debugUiEmissiveVtx = 0;
static float g_debugUiEmissiveMax = 0.0f;
static int g_debugBgVtxCount = 0;
static int g_debugBgEmissiveVtx = 0;
static float g_debugBgEmissiveMax = 0.0f;
static HRESULT g_debugWicHrWater = S_OK;
static HRESULT g_debugWicHrWaterE = S_OK;
static HRESULT g_debugWicHrBorder = S_OK;
static HRESULT g_debugWicHrLast = S_OK;
#endif

static void DrawUvScrollX(HvkDrawList *draw_list, HvkTextureRef tex_ref, const HvkVec2 &p_min, const HvkVec2 &p_max, float scroll_speed, double time, float scroll_offset, const HvkVec2 &uv0, const HvkVec2 &uv1, HvkU32 col)
{
	if (time < 0.0)
		time = HvkGui::GetTime();

	const float uv_range_x = uv1.x - uv0.x;
	const float width = p_max.x - p_min.x;
	if (uv_range_x <= 0.0f || width <= 0.0f)
		return;

	float offset = scroll_offset + scroll_speed * (float)time;
	offset = HvkFmod(offset, 1.0f);
	if (offset < 0.0f)
		offset += 1.0f;

	if (offset <= 0.0001f)
	{
		draw_list->AddImage(tex_ref, p_min, p_max, uv0, uv1, col);
		return;
	}

	const float uv_split = uv0.x + uv_range_x * offset;
	const float split_ratio = (uv1.x - uv_split) / uv_range_x;
	const float split_x = p_min.x + width * split_ratio;

	if (split_x > p_min.x)
		draw_list->AddImage(tex_ref, HvkVec2(p_min.x, p_min.y), HvkVec2(split_x, p_max.y), HvkVec2(uv_split, uv0.y), HvkVec2(uv1.x, uv1.y), col);
	if (split_x < p_max.x)
		draw_list->AddImage(tex_ref, HvkVec2(split_x, p_min.y), HvkVec2(p_max.x, p_max.y), HvkVec2(uv0.x, uv0.y), HvkVec2(uv_split, uv1.y), col);
}

static void DrawWaterBackground()
{
	if (!g_waterTexture || !g_waterEmissiveTexture)
		return;
	if (g_waterTexture->GetTexID() == HvkTextureID_Invalid || g_waterEmissiveTexture->GetTexID() == HvkTextureID_Invalid)
		return;

	HvkGuiViewport *viewport = HvkGui::GetMainViewport();
	HvkDrawList *draw_list = HvkGui::GetBackgroundDrawList();
	const HvkVec2 p_min = viewport->Pos;
	const HvkVec2 p_max(viewport->Pos.x + 1920.0f, viewport->Pos.y + 1080.0f);
	const HvkVec2 uv0(0.0f, 0.0f);
	const HvkVec2 uv1(1.0f, 1.0f);

	draw_list->PushEmissiveTexture(g_waterEmissiveTextureRef);
	draw_list->PushEmissive(g_bgEmissiveStrength, HvkGui::ColorConvertFloat4ToU32(g_bgEmissiveTint));
	DrawUvScrollX(draw_list, g_waterTextureRef, p_min, p_max, g_waterScrollSpeed, -1.0, g_waterScrollOffset, uv0, uv1, Hvk_COL32_WHITE);
	draw_list->PopEmissive();
	draw_list->PopEmissiveTexture();
}

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
bool InitDX12(HWND hwnd);
bool InitDX11(HWND hwnd);
void CleanupDeviceD3D11();
void CreateRenderTargetDX11();
void CreateHDRResourcesDX11();
void CleanupHDRResourcesDX11();
void RenderTonemapDX11();
void CreateBloomResourcesDX11(UINT width, UINT height);
void CleanupBloomResourcesDX11();
void RenderBloomDX11();
void WaitForPendingOperations();
FrameContext *WaitForNextFrameContext();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool CreateTonemapPipelineDX12();
void CreateHDRResourcesDX12(UINT width, UINT height);
void CleanupHDRResourcesDX12();
bool CreateBloomPipelinesDX12();
void CreateBloomResourcesDX12(UINT width, UINT height);
void CleanupBloomResourcesDX12();
void RenderBloomDX12(ID3D12GraphicsCommandList *cmd);

static bool LoadImageRGBA(const wchar_t *path, HvkVector<unsigned char> &out_pixels, int *out_width, int *out_height)
{
	if (!path || !out_width || !out_height)
		return false;

	UINT width = 0;
	UINT height = 0;
	HRESULT hr = S_OK;
#ifdef _DEBUG
	g_debugWicHrLast = S_OK;
#endif

	*out_width = 0;
	*out_height = 0;

	IWICImagingFactory *factory = nullptr;
	IWICBitmapDecoder *decoder = nullptr;
	IWICBitmapFrameDecode *frame = nullptr;
	IWICFormatConverter *converter = nullptr;
	bool ok = false;

	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
	if (FAILED(hr))
	{
		hr = CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
	}
	if (FAILED(hr))
		goto cleanup;

	hr = factory->CreateDecoderFromFilename(path, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
	if (FAILED(hr))
		goto cleanup;

	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr))
		goto cleanup;

	hr = frame->GetSize(&width, &height);
	if (FAILED(hr) || width == 0 || height == 0)
		goto cleanup;

	hr = factory->CreateFormatConverter(&converter);
	if (FAILED(hr))
		goto cleanup;

	hr = converter->Initialize(frame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);
	if (FAILED(hr))
		goto cleanup;

	out_pixels.resize((int)(width * height * 4));
	hr = converter->CopyPixels(nullptr, width * 4, out_pixels.Size, out_pixels.Data);
	if (FAILED(hr))
		goto cleanup;

	*out_width = (int)width;
	*out_height = (int)height;
	ok = true;

cleanup:
#ifdef _DEBUG
	if (!ok)
		g_debugWicHrLast = hr;
#endif
	if (converter)
		converter->Release();
	if (frame)
		frame->Release();
	if (decoder)
		decoder->Release();
	if (factory)
		factory->Release();
	return ok;
}

// Main code
static FILE *g_logFile = nullptr;
static bool g_logFirstFrame = false;

static void LogMessage(const char *fmt, ...)
{
	if (!g_logFile)
		return;
	va_list args;
	va_start(args, fmt);
	vfprintf(g_logFile, fmt, args);
	fprintf(g_logFile, "\n");
	fflush(g_logFile);
	va_end(args);
}

#define LOGF(...) LogMessage(__VA_ARGS__)

static void CloseLogFile()
{
	if (!g_logFile)
		return;
	LOGF("=== HvkGui end ===");
	fclose(g_logFile);
	g_logFile = nullptr;
}

static LONG WINAPI HvkGuiUnhandledExceptionFilter(EXCEPTION_POINTERS *info)
{
	if (info && info->ExceptionRecord)
		LOGF("Unhandled exception code=0x%08X addr=0x%p", (unsigned int)info->ExceptionRecord->ExceptionCode, info->ExceptionRecord->ExceptionAddress);
	CloseLogFile();
	return EXCEPTION_EXECUTE_HANDLER;
}

int main(int, char **)
{
	g_logFile = fopen("hvkgui_debug.log", "w");
	LOGF("=== HvkGui start ===");
	SetUnhandledExceptionFilter(HvkGuiUnhandledExceptionFilter);

	HRESULT com_hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	const bool com_inited = SUCCEEDED(com_hr);
	LOGF("CoInitializeEx hr=0x%08X", (unsigned int)com_hr);

	// Make process DPI aware and obtain main monitor scale
	HvkGui_ImplWin32_EnableDpiAwareness();
	float main_scale = HvkGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{0, 0}, MONITOR_DEFAULTTOPRIMARY));

	// Create application window
	WNDCLASSEXW wc = {sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"HvkGui Example", nullptr};
	::RegisterClassExW(&wc);
	HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear HvkGui DirectX12 Example", WS_OVERLAPPEDWINDOW, 100, 100, (int)(1280 * main_scale), (int)(800 * main_scale), nullptr, nullptr, wc.hInstance, nullptr);

	// Initialize Direct3D
	bool rendererOk = false;
	LOGF("Render backend preference=%s", g_render_backend == RenderBackend::DX12 ? "DX12" : "DX11");

	// Try DX12 first if preferred
	if (g_render_backend == RenderBackend::DX12)
	{
		LOGF("InitDX12 begin");
		rendererOk = InitDX12(hwnd);
		LOGF("InitDX12 end ok=%d", rendererOk ? 1 : 0);
		if (!rendererOk)
			g_render_backend = RenderBackend::DX11; // fallback
	}

	// If DX12 failed or we prefer DX11
	if (!rendererOk && g_render_backend == RenderBackend::DX11)
	{
		LOGF("InitDX11 begin");
		rendererOk = InitDX11(hwnd);
		LOGF("InitDX11 end ok=%d", rendererOk ? 1 : 0);
	}

	if (!rendererOk)
	{
		LOGF("Renderer init failed");
		MessageBoxA(
			nullptr,
			"Failed to initialize a D3D11/D3D12 renderer on this system.",
			"Fatal Error",
			MB_ICONERROR);

		CleanupDeviceD3D11();
		CleanupDeviceD3D();
		::UnregisterClassW(wc.lpszClassName, wc.hInstance);
		if (com_inited)
			CoUninitialize();
		CloseLogFile();
		return 1;
	}

	// Show the window
	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);

	// Setup Dear HvkGui context
	HvkGui_CHECKVERSION();
	HvkGui::CreateContext();
	HvkGuiIO &io = HvkGui::GetIO();
	(void)io;
	io.ConfigFlags |= HvkGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= HvkGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

	// Setup Dear HvkGui style
	HvkGui::StyleColorsDark();
	// HvkGui::StyleColorsLight();

	// Setup scaling
	HvkGuiStyle &style = HvkGui::GetStyle();
	style.ScaleAllSizes(main_scale); // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
	style.FontScaleDpi = main_scale; // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

	// Setup Platform/Renderer backends
	LOGF("HvkGui_ImplWin32_Init");
	HvkGui_ImplWin32_Init(hwnd);

	if (g_render_backend == RenderBackend::DX12)
	{
		HvkGui_ImplDX12_InitInfo init_info = {};
		init_info.Device = g_pd3dDevice;
		init_info.CommandQueue = g_pd3dCommandQueue;
		init_info.NumFramesInFlight = APP_NUM_FRAMES_IN_FLIGHT;
		init_info.RTVFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;
		init_info.DSVFormat = DXGI_FORMAT_UNKNOWN;
		// Allocating SRV descriptors (for textures) is up to the application, so we provide callbacks.
		// (current version of the backend will only allocate one descriptor, future versions will need to allocate more)
		init_info.SrvDescriptorHeap = g_pd3dSrvDescHeap;
		init_info.SrvDescriptorAllocFn = [](HvkGui_ImplDX12_InitInfo *, D3D12_CPU_DESCRIPTOR_HANDLE *out_cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE *out_gpu_handle)
		{ return g_pd3dSrvDescHeapAlloc.Alloc(out_cpu_handle, out_gpu_handle); };
		init_info.SrvDescriptorFreeFn = [](HvkGui_ImplDX12_InitInfo *, D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle)
		{ return g_pd3dSrvDescHeapAlloc.Free(cpu_handle, gpu_handle); };
		LOGF("HvkGui_ImplDX12_Init");
		HvkGui_ImplDX12_Init(&init_info);
		HvkGui_ImplDX12_SetLdrFormat(DXGI_FORMAT_R8G8B8A8_UNORM);
		LOGF("HvkGui_ImplDX12_CreateDeviceObjects begin");
		if (!HvkGui_ImplDX12_CreateDeviceObjects())
		{
			LOGF("HvkGui_ImplDX12_CreateDeviceObjects FAILED");
			MessageBoxA(nullptr, "DX12 CreateDeviceObjects failed.", "Fatal Error", MB_ICONERROR);
			CleanupDeviceD3D11();
			CleanupDeviceD3D();
			::UnregisterClassW(wc.lpszClassName, wc.hInstance);
			if (com_inited)
				CoUninitialize();
			CloseLogFile();
			return 1;
		}
		LOGF("HvkGui_ImplDX12_CreateDeviceObjects end");
	}
	else
	{
		LOGF("HvkGui_ImplDX11_Init");
		HvkGui_ImplDX11_Init(g_pd3dDevice11, g_pd3dDeviceContext11);
		LOGF("HvkGui_ImplDX11_CreateDeviceObjects begin");
		if (!HvkGui_ImplDX11_CreateDeviceObjects())
		{
			LOGF("HvkGui_ImplDX11_CreateDeviceObjects FAILED");
			MessageBoxA(nullptr, "DX11 CreateDeviceObjects failed.", "Fatal Error", MB_ICONERROR);
			CleanupDeviceD3D11();
			CleanupDeviceD3D();
			::UnregisterClassW(wc.lpszClassName, wc.hInstance);
			if (com_inited)
				CoUninitialize();
			CloseLogFile();
			return 1;
		}
		LOGF("HvkGui_ImplDX11_CreateDeviceObjects end");
	}

	// Before 1.91.6: our signature was using a single descriptor. From 1.92, specifying SrvDescriptorAllocFn/SrvDescriptorFreeFn will be required to benefit from new features.
	// HvkGui_ImplDX12_Init(g_pd3dDevice, APP_NUM_FRAMES_IN_FLIGHT, DXGI_FORMAT_R8G8B8A8_UNORM, g_pd3dSrvDescHeap, g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart(), g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart());

	// Load Fonts
	// - If no fonts are loaded, dear HvkGui will use the default font. You can also load multiple fonts and use HvkGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the HvkFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - Use '#define HvkGui_ENABLE_FREETYPE' in your Hvkconfig file to use Freetype for higher quality font rendering.
	// - Read 'docs/FONTS.md' for more instructions and details. If you like the default font but want it to scale better, consider using the 'ProggyVector' from the same author!
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	// style.FontSizeBase = 20.0f;
	// io.Fonts->AddFontDefault();
	// io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf");
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
	// HvkFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
	// Hvk_ASSERT(font != nullptr);

	// Ensure font texture is built and uploaded.
	unsigned char *font_pixels = nullptr;
	int font_w = 0;
	int font_h = 0;
	io.Fonts->GetTexDataAsRGBA32(&font_pixels, &font_w, &font_h);
	LOGF("Font atlas built w=%d h=%d", font_w, font_h);
	if (io.Fonts->TexRef._TexData)
	{
		LOGF("Font tex update begin");
		if (g_render_backend == RenderBackend::DX12)
			HvkGui_ImplDX12_UpdateTexture(io.Fonts->TexRef._TexData);
		else
			HvkGui_ImplDX11_UpdateTexture(io.Fonts->TexRef._TexData);
		LOGF("Font tex update end");
	}

	// Our state
	HvkVec4 clear_color = HvkVec4(0.45f, 0.55f, 0.60f, 1.00f);
	{
		const wchar_t *water_path = L"C:\\Users\\HavoK\\Documents\\Network\\Laptop Shared\\Projects\\HvkGui\\assets\\water.png";
		const wchar_t *water_emissive_path = L"C:\\Users\\HavoK\\Documents\\Network\\Laptop Shared\\Projects\\HvkGui\\assets\\water_E.png";
		const wchar_t *border_path = L"C:\\Users\\HavoK\\Documents\\Network\\Laptop Shared\\Projects\\HvkGui\\assets\\Border.png";

		HvkVector<unsigned char> pixels;
		int tex_width = 0;
		int tex_height = 0;
		pixels.clear();
		tex_width = 0;
		tex_height = 0;
		LOGF("LoadImageRGBA water begin: %ls", water_path);
		if (LoadImageRGBA(water_path, pixels, &tex_width, &tex_height))
		{
			LOGF("LoadImageRGBA water ok w=%d h=%d bytes=%d", tex_width, tex_height, pixels.Size);
			g_waterTexture = new HvkTextureData();
			g_waterTexture->Create(HvkTextureFormat_RGBA32, tex_width, tex_height);
			memcpy(g_waterTexture->GetPixels(), pixels.Data, pixels.Size);
			g_waterTexture->UseColors = true;
			HvkGui::RegisterUserTexture(g_waterTexture);
			g_waterTextureRef = g_waterTexture->GetTexRef();
			if (g_render_backend == RenderBackend::DX12)
				HvkGui_ImplDX12_UpdateTexture(g_waterTexture);
			else
				HvkGui_ImplDX11_UpdateTexture(g_waterTexture);
			LOGF("Water tex updated, texid=0x%llx", (unsigned long long)g_waterTexture->GetTexID());
		}
		else
		{
#ifdef _DEBUG
			LOGF("LoadImageRGBA water FAILED hr=0x%08X", (unsigned int)g_debugWicHrLast);
#else
			LOGF("LoadImageRGBA water FAILED");
#endif
		}
#ifdef _DEBUG
		g_debugWicHrWater = g_debugWicHrLast;
#endif

		pixels.clear();
		tex_width = 0;
		tex_height = 0;
		LOGF("LoadImageRGBA water emissive begin: %ls", water_emissive_path);
		if (LoadImageRGBA(water_emissive_path, pixels, &tex_width, &tex_height))
		{
			LOGF("LoadImageRGBA water emissive ok w=%d h=%d bytes=%d", tex_width, tex_height, pixels.Size);
			g_waterEmissiveTexture = new HvkTextureData();
			g_waterEmissiveTexture->Create(HvkTextureFormat_RGBA32, tex_width, tex_height);
			memcpy(g_waterEmissiveTexture->GetPixels(), pixels.Data, pixels.Size);
			g_waterEmissiveTexture->UseColors = true;
			HvkGui::RegisterUserTexture(g_waterEmissiveTexture);
			g_waterEmissiveTextureRef = g_waterEmissiveTexture->GetTexRef();
			if (g_render_backend == RenderBackend::DX12)
				HvkGui_ImplDX12_UpdateTexture(g_waterEmissiveTexture);
			else
				HvkGui_ImplDX11_UpdateTexture(g_waterEmissiveTexture);
			LOGF("Water emissive tex updated, texid=0x%llx", (unsigned long long)g_waterEmissiveTexture->GetTexID());
		}
		else
		{
#ifdef _DEBUG
			LOGF("LoadImageRGBA water emissive FAILED hr=0x%08X", (unsigned int)g_debugWicHrLast);
#else
			LOGF("LoadImageRGBA water emissive FAILED");
#endif
		}
#ifdef _DEBUG
		g_debugWicHrWaterE = g_debugWicHrLast;
#endif

		LOGF("LoadImageRGBA border begin: %ls", border_path);
		if (LoadImageRGBA(border_path, pixels, &tex_width, &tex_height))
		{
			LOGF("LoadImageRGBA border ok w=%d h=%d bytes=%d", tex_width, tex_height, pixels.Size);
			g_borderTexture = new HvkTextureData();
			g_borderTexture->Create(HvkTextureFormat_RGBA32, tex_width, tex_height);
			memcpy(g_borderTexture->GetPixels(), pixels.Data, pixels.Size);
			g_borderTexture->UseColors = true;
			HvkGui::RegisterUserTexture(g_borderTexture);
			g_borderTextureRef = g_borderTexture->GetTexRef();
			if (g_render_backend == RenderBackend::DX12)
				HvkGui_ImplDX12_UpdateTexture(g_borderTexture);
			else
				HvkGui_ImplDX11_UpdateTexture(g_borderTexture);
			LOGF("Border tex updated, texid=0x%llx", (unsigned long long)g_borderTexture->GetTexID());
		}
		else
		{
#ifdef _DEBUG
			LOGF("LoadImageRGBA border FAILED hr=0x%08X", (unsigned int)g_debugWicHrLast);
#else
			LOGF("LoadImageRGBA border FAILED");
#endif
		}
#ifdef _DEBUG
		g_debugWicHrBorder = g_debugWicHrLast;
#endif
	}

	// Main loop
	bool done = false;
	LOGF("Entering main loop");
	auto RunMainLoop = [&]()
	{
		while (!done)
		{
			if (!g_logFirstFrame)
			{
				LOGF("Frame0 begin");
				g_logFirstFrame = true;
			}
		// Poll and handle messages (inputs, window resize, etc.)
		// See the WndProc() function below for our to dispatch events to the Win32 backend.
		MSG msg;
		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				done = true;
		}
		if (done)
			break;

		// Handle window screen locked
		if ((g_render_backend == RenderBackend::DX12 && g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED) || ::IsIconic(hwnd))
		{
			::Sleep(10);
			continue;
		}
		g_SwapChainOccluded = false;

		// Start the Dear HvkGui frame
		if (g_logFirstFrame)
			LOGF("Frame0 NewFrame begin");
		if (g_render_backend == RenderBackend::DX12)
			HvkGui_ImplDX12_NewFrame();
		else
			HvkGui_ImplDX11_NewFrame();
		HvkGui_ImplWin32_NewFrame();
		HvkGui::NewFrame();
		if (g_logFirstFrame)
			LOGF("Frame0 NewFrame end");

		DrawWaterBackground();

		const bool water_ready = g_waterTexture && g_waterEmissiveTexture && g_waterTexture->GetTexID() != HvkTextureID_Invalid && g_waterEmissiveTexture->GetTexID() != HvkTextureID_Invalid;
		HvkGui::SetNextWindowPos(HvkVec2(10.0f, 10.0f), HvkGuiCond_Once);
		HvkGui::SetNextWindowSize(HvkVec2(300.0f, 800.0f), HvkGuiCond_Once);
		HvkGui::Begin("Emission Controls");
		HvkGui::Text("Background Emission");
		HvkGui::Text("Water textures: %s", water_ready ? "OK" : "NOT LOADED");
		HvkGui::SliderFloat("BG Strength", &g_bgEmissiveStrength, 0.0f, 8.0f);
		HvkGui::ColorEdit4("BG Tint", &g_bgEmissiveTint.x);
		HvkGui::SliderFloat("Scroll Speed (UV/s)", &g_waterScrollSpeed, -1.0f, 1.0f);
		HvkGui::SliderFloat("Scroll Offset", &g_waterScrollOffset, -1.0f, 1.0f);
		HvkGui::Separator();
		HvkGui::Text("Emissive Text");
		HvkGui::SliderFloat("Text Strength", &g_textEmissiveStrength, 0.0f, 8.0f);
		HvkGui::ColorEdit4("Text Tint", &g_textEmissiveTint.x);
		HvkGui::Checkbox("Show Emissive Text Window", &g_showEmissiveTextWindow);
#ifdef _DEBUG
		HvkGui::Separator();
		HvkGui::Checkbox("Debug: Show Emissive RT", &g_debugShowEmissive);
		HvkGui::Text("UI Vtx: %d, Emissive: %d, Max: %.3f", g_debugUiVtxCount, g_debugUiEmissiveVtx, g_debugUiEmissiveMax);
		HvkGui::Text("BG Vtx: %d, Emissive: %d, Max: %.3f", g_debugBgVtxCount, g_debugBgEmissiveVtx, g_debugBgEmissiveMax);
		HvkTextureID dbg_font_id = HvkGui::GetIO().Fonts ? HvkGui::GetIO().Fonts->TexRef.GetTexID() : HvkTextureID_Invalid;
		HvkTextureID dbg_water_id = g_waterTexture ? g_waterTexture->GetTexID() : HvkTextureID_Invalid;
		HvkTextureID dbg_water_e_id = g_waterEmissiveTexture ? g_waterEmissiveTexture->GetTexID() : HvkTextureID_Invalid;
		HvkGui::Text("TexID Font: 0x%llx", (unsigned long long)dbg_font_id);
		HvkGui::Text("TexID Water: 0x%llx", (unsigned long long)dbg_water_id);
		HvkGui::Text("TexID WaterE: 0x%llx", (unsigned long long)dbg_water_e_id);
		HvkGui::Text("WIC HR Water: 0x%08x", (unsigned int)g_debugWicHrWater);
		HvkGui::Text("WIC HR WaterE: 0x%08x", (unsigned int)g_debugWicHrWaterE);
		HvkGui::Text("WIC HR Border: 0x%08x", (unsigned int)g_debugWicHrBorder);
#endif
		HvkGui::End();

		if (g_showEmissiveTextWindow && g_borderTexture && g_borderTexture->GetTexID() != HvkTextureID_Invalid)
		{
			HvkGui::Begin("Emissive Text Test", &g_showEmissiveTextWindow);
			HvkGui::Text("Text using emissive strength/tint.");
			HvkGui::PushEmissiveTexture(g_borderTextureRef);
			HvkGui::PushEmissive(g_textEmissiveStrength, g_textEmissiveTint);
			HvkGui::Text("HvkGui emissive text test");
			HvkGui::PopEmissive();
			HvkGui::PopEmissiveTexture();
			HvkGui::End();
		}
		

		// Rendering
		if (g_logFirstFrame)
			LOGF("Frame0 Render begin");
		HvkGui::Render();
		if (g_logFirstFrame)
			LOGF("Frame0 Render end");

		HvkDrawData *draw_data = HvkGui::GetDrawData();
		HvkDrawList *bg_list = HvkGui::GetBackgroundDrawList();
		HvkDrawData bg_draw_data;
		HvkDrawData ui_draw_data;
		HvkDrawData *bg_data = nullptr;
		HvkDrawData *ui_data = nullptr;
		if (draw_data && draw_data->Valid)
		{
			bg_draw_data.Clear();
			bg_draw_data.Valid = true;
			bg_draw_data.DisplayPos = draw_data->DisplayPos;
			bg_draw_data.DisplaySize = draw_data->DisplaySize;
			bg_draw_data.FramebufferScale = draw_data->FramebufferScale;
			bg_draw_data.OwnerViewport = draw_data->OwnerViewport;
			bg_draw_data.Textures = draw_data->Textures;
			if (bg_list)
				bg_draw_data.AddDrawList(bg_list);
			if (bg_draw_data.CmdListsCount > 0)
				bg_data = &bg_draw_data;

			ui_draw_data.Clear();
			ui_draw_data.Valid = true;
			ui_draw_data.DisplayPos = draw_data->DisplayPos;
			ui_draw_data.DisplaySize = draw_data->DisplaySize;
			ui_draw_data.FramebufferScale = draw_data->FramebufferScale;
			ui_draw_data.OwnerViewport = draw_data->OwnerViewport;
			ui_draw_data.Textures = draw_data->Textures;
			for (HvkDrawList *list : draw_data->CmdLists)
				if (list != bg_list)
					ui_draw_data.AddDrawList(list);
			if (ui_draw_data.CmdListsCount > 0)
				ui_data = &ui_draw_data;
		}

#ifdef _DEBUG
		g_debugUiVtxCount = 0;
		g_debugUiEmissiveVtx = 0;
		g_debugUiEmissiveMax = 0.0f;
		g_debugBgVtxCount = 0;
		g_debugBgEmissiveVtx = 0;
		g_debugBgEmissiveMax = 0.0f;
		auto accumulate_emissive_stats = [](HvkDrawData *data, int &out_vtx, int &out_emissive_vtx, float &out_max)
		{
			if (!data)
				return;
			for (HvkDrawList *list : data->CmdLists)
			{
				out_vtx += list->VtxBuffer.Size;
				for (int i = 0; i < list->VtxBuffer.Size; i++)
				{
					const float e = list->VtxBuffer[i].emissive;
					if (e > 0.0001f)
					{
						out_emissive_vtx++;
						if (e > out_max)
							out_max = e;
					}
				}
			}
		};
		accumulate_emissive_stats(ui_data, g_debugUiVtxCount, g_debugUiEmissiveVtx, g_debugUiEmissiveMax);
		accumulate_emissive_stats(bg_data, g_debugBgVtxCount, g_debugBgEmissiveVtx, g_debugBgEmissiveMax);
#endif

		const float clear_color_with_alpha[4] = {clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w};
		if (g_render_backend == RenderBackend::DX12)
		{
			if (g_logFirstFrame)
				LOGF("Frame0 DX12 submit begin");
			if (g_logFirstFrame)
				LOGF("Frame0 WaitForNextFrameContext begin");
			FrameContext *frameCtx = WaitForNextFrameContext();
			if (g_logFirstFrame)
				LOGF("Frame0 WaitForNextFrameContext end");
			UINT backBufferIdx = g_pSwapChain->GetCurrentBackBufferIndex();
			HRESULT reset_alloc_hr = frameCtx->CommandAllocator->Reset();
			if (g_logFirstFrame)
				LOGF("Frame0 CommandAllocator Reset hr=0x%08X", (unsigned int)reset_alloc_hr);

			HRESULT reset_list_hr = g_pd3dCommandList->Reset(frameCtx->CommandAllocator, nullptr);
			if (g_logFirstFrame)
				LOGF("Frame0 CommandList Reset hr=0x%08X", (unsigned int)reset_list_hr);

			D3D12_RESOURCE_BARRIER barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

			if (g_logFirstFrame)
				LOGF("Frame0 HDR resources hdr=0x%p emissive=0x%p hdrRTV=0x%llx emissiveRTV=0x%llx rtvHeap=0x%p postHeap=0x%p",
					g_hdrRenderTargetResource,
					g_hdrEmissiveRenderTargetResource,
					(unsigned long long)g_hdrRenderTargetDescriptor.ptr,
					(unsigned long long)g_hdrEmissiveRenderTargetDescriptor.ptr,
					g_pd3dRtvDescHeap,
					g_pd3dPostSrvDescHeap);
			if (g_hdrRenderTargetDescriptor.ptr == 0 || g_hdrEmissiveRenderTargetDescriptor.ptr == 0 || g_pd3dPostSrvDescHeap == nullptr)
			{
				LOGF("Frame0 invalid HDR descriptors; skipping frame");
				continue;
			}
			if (g_hdrRenderTargetResource && g_hdrEmissiveRenderTargetResource)
			{
				if (g_logFirstFrame)
					LOGF("Frame0 HDR barrier begin");
				D3D12_RESOURCE_BARRIER hdr_barriers[2] = {};
				hdr_barriers[0].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				hdr_barriers[0].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
				hdr_barriers[0].Transition.pResource = g_hdrRenderTargetResource;
				hdr_barriers[0].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				hdr_barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
				hdr_barriers[0].Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
				hdr_barriers[1] = hdr_barriers[0];
				hdr_barriers[1].Transition.pResource = g_hdrEmissiveRenderTargetResource;
				g_pd3dCommandList->ResourceBarrier(2, hdr_barriers);
				if (g_logFirstFrame)
					LOGF("Frame0 HDR barrier end");

				if (g_logFirstFrame)
					LOGF("Frame0 Clear HDR begin");
				g_pd3dCommandList->ClearRenderTargetView(g_hdrRenderTargetDescriptor, clear_color_with_alpha, 0, nullptr);
				const float emissive_clear[4] = {0.0f, 0.0f, 0.0f, 0.0f};
				g_pd3dCommandList->ClearRenderTargetView(g_hdrEmissiveRenderTargetDescriptor, emissive_clear, 0, nullptr);
				if (g_logFirstFrame)
					LOGF("Frame0 Clear HDR end");
				if (g_logFirstFrame)
					LOGF("Frame0 SetDescriptorHeaps begin srvHeap=0x%p", g_pd3dSrvDescHeap);
				g_pd3dCommandList->SetDescriptorHeaps(1, &g_pd3dSrvDescHeap);
				if (g_logFirstFrame)
					LOGF("Frame0 SetDescriptorHeaps end");

				if (bg_data && bg_data->CmdListsCount > 0)
				{
					if (g_logFirstFrame)
						LOGF("Frame0 BG draw begin");
					if (g_emissiveRenderPath == EmissiveRenderPath_TwoPass)
					{
						HvkGui_ImplDX12_SetOutputMode(HvkGui_ImplDX12_OutputMode_BaseOnly);
						g_pd3dCommandList->OMSetRenderTargets(1, &g_hdrRenderTargetDescriptor, FALSE, nullptr);
						HvkGui_ImplDX12_RenderDrawData(bg_data, g_pd3dCommandList);

						HvkGui_ImplDX12_SetOutputMode(HvkGui_ImplDX12_OutputMode_EmissiveOnly);
						g_pd3dCommandList->OMSetRenderTargets(1, &g_hdrEmissiveRenderTargetDescriptor, FALSE, nullptr);
						HvkGui_ImplDX12_RenderDrawData(bg_data, g_pd3dCommandList);
					}
					else
					{
						HvkGui_ImplDX12_SetOutputMode(HvkGui_ImplDX12_OutputMode_MRT);
						D3D12_CPU_DESCRIPTOR_HANDLE hdr_rtvs[2] = {g_hdrRenderTargetDescriptor, g_hdrEmissiveRenderTargetDescriptor};
						g_pd3dCommandList->OMSetRenderTargets(2, hdr_rtvs, FALSE, nullptr);
						HvkGui_ImplDX12_RenderDrawData(bg_data, g_pd3dCommandList);
					}
					if (g_logFirstFrame)
						LOGF("Frame0 BG draw end");
				}

				// UI emissive is composited later to avoid being covered by the UI background.

				hdr_barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
				hdr_barriers[0].Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
				hdr_barriers[1].Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
				hdr_barriers[1].Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
				g_pd3dCommandList->ResourceBarrier(2, hdr_barriers);
			}

			// Bloom uses post-process SRVs (g_hdrSrv/g_bloom SRVs).
			g_pd3dCommandList->SetDescriptorHeaps(1, &g_pd3dPostSrvDescHeap);
			RenderBloomDX12(g_pd3dCommandList);

			barrier.Transition.pResource = g_mainRenderTargetResource[backBufferIdx];
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
			g_pd3dCommandList->ResourceBarrier(1, &barrier);

			g_pd3dCommandList->OMSetRenderTargets(1, &g_mainRenderTargetDescriptor[backBufferIdx], FALSE, nullptr);
			g_pd3dCommandList->SetDescriptorHeaps(1, &g_pd3dPostSrvDescHeap);

			if (g_hdrRenderTargetResource && g_tonemapPipelineState && g_tonemapRootSignature)
			{
				D3D12_VIEWPORT vp = {};
				vp.Width = HvkGui::GetIO().DisplaySize.x * HvkGui::GetIO().DisplayFramebufferScale.x;
				vp.Height = HvkGui::GetIO().DisplaySize.y * HvkGui::GetIO().DisplayFramebufferScale.y;
				vp.MinDepth = 0.0f;
				vp.MaxDepth = 1.0f;
				D3D12_RECT scissor = {0, 0, (LONG)vp.Width, (LONG)vp.Height};
				g_pd3dCommandList->RSSetViewports(1, &vp);
				g_pd3dCommandList->RSSetScissorRects(1, &scissor);
				g_pd3dCommandList->SetGraphicsRootSignature(g_tonemapRootSignature);
				g_pd3dCommandList->SetPipelineState(g_tonemapPipelineState);
				float bloom_intensity = g_bloomIntensity;
				D3D12_GPU_DESCRIPTOR_HANDLE base_srv = g_hdrSrvGpuHandle;
#ifdef _DEBUG
				if (g_debugShowEmissive)
				{
					base_srv = g_hdrEmissiveSrvGpuHandle;
					bloom_intensity = 0.0f;
				}
#endif
				const float params[8] = {1.0f, bloom_intensity, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
				g_pd3dCommandList->SetGraphicsRoot32BitConstants(2, 8, params, 0);
				g_pd3dCommandList->SetGraphicsRootDescriptorTable(0, base_srv);
				g_pd3dCommandList->SetGraphicsRootDescriptorTable(1, g_bloomBlurSrvGpu12[0]);
				g_pd3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				g_pd3dCommandList->DrawInstanced(3, 1, 0, 0);
			}

			if (ui_data && ui_data->CmdListsCount > 0)
			{
				HvkGui_ImplDX12_SetOutputMode(HvkGui_ImplDX12_OutputMode_LDR);
				g_pd3dCommandList->SetDescriptorHeaps(1, &g_pd3dSrvDescHeap);
				g_pd3dCommandList->OMSetRenderTargets(1, &g_mainRenderTargetDescriptor[backBufferIdx], FALSE, nullptr);
				HvkGui_ImplDX12_RenderDrawData(ui_data, g_pd3dCommandList);
			}

			if (ui_data && ui_data->CmdListsCount > 0 && g_hdrEmissiveRenderTargetResource && g_emissiveAddPipelineState)
			{
				D3D12_RESOURCE_BARRIER ui_emissive_barrier = {};
				ui_emissive_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				ui_emissive_barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
				ui_emissive_barrier.Transition.pResource = g_hdrEmissiveRenderTargetResource;
				ui_emissive_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				ui_emissive_barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
				ui_emissive_barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
				g_pd3dCommandList->ResourceBarrier(1, &ui_emissive_barrier);

				const float emissive_clear_ui[4] = {0.0f, 0.0f, 0.0f, 0.0f};
				g_pd3dCommandList->ClearRenderTargetView(g_hdrEmissiveRenderTargetDescriptor, emissive_clear_ui, 0, nullptr);
				HvkGui_ImplDX12_SetOutputMode(HvkGui_ImplDX12_OutputMode_EmissiveOnly);
				g_pd3dCommandList->OMSetRenderTargets(1, &g_hdrEmissiveRenderTargetDescriptor, FALSE, nullptr);
				HvkGui_ImplDX12_RenderDrawData(ui_data, g_pd3dCommandList);

				ui_emissive_barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
				ui_emissive_barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
				g_pd3dCommandList->ResourceBarrier(1, &ui_emissive_barrier);

				// Bloom uses post-process SRVs (g_hdrEmissive/g_bloom SRVs).
				g_pd3dCommandList->SetDescriptorHeaps(1, &g_pd3dPostSrvDescHeap);
				RenderBloomDX12(g_pd3dCommandList);

				bool do_ui_emissive_add = true;
#ifdef _DEBUG
				if (g_debugShowEmissive)
					do_ui_emissive_add = false;
#endif
				if (do_ui_emissive_add)
				{
					g_pd3dCommandList->OMSetRenderTargets(1, &g_mainRenderTargetDescriptor[backBufferIdx], FALSE, nullptr);
					g_pd3dCommandList->SetDescriptorHeaps(1, &g_pd3dPostSrvDescHeap);
					D3D12_VIEWPORT vp = {};
					vp.Width = HvkGui::GetIO().DisplaySize.x * HvkGui::GetIO().DisplayFramebufferScale.x;
					vp.Height = HvkGui::GetIO().DisplaySize.y * HvkGui::GetIO().DisplayFramebufferScale.y;
					vp.MinDepth = 0.0f;
					vp.MaxDepth = 1.0f;
					D3D12_RECT scissor = {0, 0, (LONG)vp.Width, (LONG)vp.Height};
					g_pd3dCommandList->RSSetViewports(1, &vp);
					g_pd3dCommandList->RSSetScissorRects(1, &scissor);
					g_pd3dCommandList->SetGraphicsRootSignature(g_tonemapRootSignature);
					g_pd3dCommandList->SetPipelineState(g_emissiveAddPipelineState);
					const float params[8] = {1.0f, g_bloomIntensity, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
					g_pd3dCommandList->SetGraphicsRoot32BitConstants(2, 8, params, 0);
					g_pd3dCommandList->SetGraphicsRootDescriptorTable(0, g_hdrEmissiveSrvGpuHandle);
					g_pd3dCommandList->SetGraphicsRootDescriptorTable(1, g_bloomBlurSrvGpu12[0]);
					g_pd3dCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
					g_pd3dCommandList->DrawInstanced(3, 1, 0, 0);
				}
			}

			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
			g_pd3dCommandList->ResourceBarrier(1, &barrier);
			HRESULT close_hr = g_pd3dCommandList->Close();
			if (g_logFirstFrame)
				LOGF("Frame0 CommandList Close hr=0x%08X", (unsigned int)close_hr);

			g_pd3dCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList *const *)&g_pd3dCommandList);
			if (g_logFirstFrame)
				LOGF("Frame0 ExecuteCommandLists ok");
			HRESULT signal_hr = g_pd3dCommandQueue->Signal(g_fence, ++g_fenceLastSignaledValue);
			if (g_logFirstFrame)
				LOGF("Frame0 Signal hr=0x%08X fence=%llu", (unsigned int)signal_hr, (unsigned long long)g_fenceLastSignaledValue);
			frameCtx->FenceValue = g_fenceLastSignaledValue;

			HRESULT hr = g_pSwapChain->Present(1, 0);
			g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
			if (g_logFirstFrame)
				LOGF("Frame0 Present hr=0x%08X occluded=%d", (unsigned int)hr, g_SwapChainOccluded ? 1 : 0);
		}
		else
		{
			if (g_logFirstFrame)
				LOGF("Frame0 DX11 submit begin");
			if (g_hdrRTV11 && g_hdrEmissiveRTV11)
			{
				g_pd3dDeviceContext11->ClearRenderTargetView(g_hdrRTV11, clear_color_with_alpha);
				const float emissive_clear[4] = {0.0f, 0.0f, 0.0f, 0.0f};
				g_pd3dDeviceContext11->ClearRenderTargetView(g_hdrEmissiveRTV11, emissive_clear);

				if (bg_data && bg_data->CmdListsCount > 0)
				{
					if (g_emissiveRenderPath == EmissiveRenderPath_TwoPass)
					{
						HvkGui_ImplDX11_SetOutputMode(HvkGui_ImplDX11_OutputMode_BaseOnly);
						g_pd3dDeviceContext11->OMSetRenderTargets(1, &g_hdrRTV11, nullptr);
						HvkGui_ImplDX11_RenderDrawData(bg_data);

						HvkGui_ImplDX11_SetOutputMode(HvkGui_ImplDX11_OutputMode_EmissiveOnly);
						g_pd3dDeviceContext11->OMSetRenderTargets(1, &g_hdrEmissiveRTV11, nullptr);
						HvkGui_ImplDX11_RenderDrawData(bg_data);
					}
					else
					{
						HvkGui_ImplDX11_SetOutputMode(HvkGui_ImplDX11_OutputMode_MRT);
						ID3D11RenderTargetView *rtvs[2] = {g_hdrRTV11, g_hdrEmissiveRTV11};
						g_pd3dDeviceContext11->OMSetRenderTargets(2, rtvs, nullptr);
						HvkGui_ImplDX11_RenderDrawData(bg_data);
					}
				}
				// UI emissive is composited later to avoid being covered by the UI background.
			}
			RenderBloomDX11();
			RenderTonemapDX11();
			if (ui_data && ui_data->CmdListsCount > 0 && g_mainRenderTargetView11)
			{
				HvkGui_ImplDX11_SetOutputMode(HvkGui_ImplDX11_OutputMode_LDR);
				g_pd3dDeviceContext11->OMSetRenderTargets(1, &g_mainRenderTargetView11, nullptr);
				HvkGui_ImplDX11_RenderDrawData(ui_data);
			}
			if (ui_data && ui_data->CmdListsCount > 0 && g_hdrEmissiveRTV11 && g_hdrEmissiveSRV11 && g_tonemapAddBlend11)
			{
				const float emissive_clear_ui[4] = {0.0f, 0.0f, 0.0f, 0.0f};
				g_pd3dDeviceContext11->ClearRenderTargetView(g_hdrEmissiveRTV11, emissive_clear_ui);
				HvkGui_ImplDX11_SetOutputMode(HvkGui_ImplDX11_OutputMode_EmissiveOnly);
				g_pd3dDeviceContext11->OMSetRenderTargets(1, &g_hdrEmissiveRTV11, nullptr);
				HvkGui_ImplDX11_RenderDrawData(ui_data);

				RenderBloomDX11();

				bool do_ui_emissive_add = true;
#ifdef _DEBUG
				if (g_debugShowEmissive)
					do_ui_emissive_add = false;
#endif
				if (do_ui_emissive_add)
				{
					ID3D11Texture2D *backBuffer = nullptr;
					g_pSwapChain11->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
					if (backBuffer)
					{
						D3D11_TEXTURE2D_DESC bbDesc = {};
						backBuffer->GetDesc(&bbDesc);
						backBuffer->Release();
						D3D11_VIEWPORT vp = {};
						vp.Width = (float)bbDesc.Width;
						vp.Height = (float)bbDesc.Height;
						vp.MinDepth = 0.0f;
						vp.MaxDepth = 1.0f;
						g_pd3dDeviceContext11->RSSetViewports(1, &vp);
					}
					ID3D11RenderTargetView *rtv = g_mainRenderTargetView11;
					g_pd3dDeviceContext11->OMSetRenderTargets(1, &rtv, nullptr);
					g_pd3dDeviceContext11->RSSetState(g_tonemapRasterizer11);
					g_pd3dDeviceContext11->OMSetBlendState(g_tonemapAddBlend11, nullptr, 0xffffffff);
					g_pd3dDeviceContext11->OMSetDepthStencilState(g_tonemapDepth11, 0);
					g_pd3dDeviceContext11->IASetInputLayout(nullptr);
					g_pd3dDeviceContext11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
					g_pd3dDeviceContext11->VSSetShader(g_tonemapVS11, nullptr, 0);
					g_pd3dDeviceContext11->PSSetShader(g_tonemapPS11, nullptr, 0);
					g_pd3dDeviceContext11->PSSetSamplers(0, 1, &g_tonemapSampler11);

					ID3D11ShaderResourceView *srvs_add[2] = {g_hdrEmissiveSRV11, g_bloomBlurSRV11[0]};
					g_pd3dDeviceContext11->PSSetShaderResources(0, 2, srvs_add);
					struct TonemapParams
					{
						float Exposure;
						float BloomIntensity;
						float pad[2];
					};
					TonemapParams params = {};
					params.Exposure = 1.0f;
					params.BloomIntensity = g_bloomIntensity;
					ID3D11Buffer *cb = nullptr;
					D3D11_BUFFER_DESC cbDesc = {};
					cbDesc.ByteWidth = sizeof(TonemapParams);
					cbDesc.Usage = D3D11_USAGE_DYNAMIC;
					cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
					cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
					if (SUCCEEDED(g_pd3dDevice11->CreateBuffer(&cbDesc, nullptr, &cb)))
					{
						D3D11_MAPPED_SUBRESOURCE mapped = {};
						if (SUCCEEDED(g_pd3dDeviceContext11->Map(cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
						{
							memcpy(mapped.pData, &params, sizeof(params));
							g_pd3dDeviceContext11->Unmap(cb, 0);
						}
						g_pd3dDeviceContext11->PSSetConstantBuffers(0, 1, &cb);
					}
					g_pd3dDeviceContext11->Draw(3, 0);
					if (cb)
						cb->Release();
					ID3D11ShaderResourceView *null_srvs[2] = {};
					g_pd3dDeviceContext11->PSSetShaderResources(0, 2, null_srvs);
				}
			}
			g_pSwapChain11->Present(1, 0);
			if (g_logFirstFrame)
				LOGF("Frame0 DX11 submit end");
		}
		g_frameIndex++;
		}
	};
	RunMainLoop();

	WaitForPendingOperations();

	// Cleanup
	if (g_waterTexture)
	{
		HvkGui::UnregisterUserTexture(g_waterTexture);
		delete g_waterTexture;
		g_waterTexture = nullptr;
	}
	if (g_waterEmissiveTexture)
	{
		HvkGui::UnregisterUserTexture(g_waterEmissiveTexture);
		delete g_waterEmissiveTexture;
		g_waterEmissiveTexture = nullptr;
	}
	if (g_borderTexture)
	{
		HvkGui::UnregisterUserTexture(g_borderTexture);
		delete g_borderTexture;
		g_borderTexture = nullptr;
	}

	if (g_render_backend == RenderBackend::DX12)
		HvkGui_ImplDX12_Shutdown();
	else
		HvkGui_ImplDX11_Shutdown();

	HvkGui_ImplWin32_Shutdown();
	HvkGui::DestroyContext();

	if (g_render_backend == RenderBackend::DX12)
		CleanupDeviceD3D();
	else
		CleanupDeviceD3D11();

	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	::UnregisterClassW(wc.lpszClassName, wc.hInstance);
	if (com_inited)
		CoUninitialize();
	CloseLogFile();

	return 0;
}

// Helper functions
bool InitDX12(HWND hwnd)
{
	return CreateDeviceD3D(hwnd);
}

bool InitDX11(HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferCount = 2;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_0};

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createDeviceFlags,
		featureLevels,
		1,
		D3D11_SDK_VERSION,
		&sd,
		&g_pSwapChain11,
		&g_pd3dDevice11,
		&featureLevel,
		&g_pd3dDeviceContext11);

	if (FAILED(hr))
		return false;

	CreateRenderTargetDX11();
	return true;
}

bool CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	// This is a basic setup. Optimally could handle fullscreen mode differently. See #8979 for suggestions.
	DXGI_SWAP_CHAIN_DESC1 sd;
	{
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = APP_NUM_BACK_BUFFERS;
		sd.Width = 0;
		sd.Height = 0;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		sd.Scaling = DXGI_SCALING_STRETCH;
		sd.Stereo = FALSE;
	}

	// [DEBUG] Enable debug interface
#ifdef DX12_ENABLE_DEBUG_LAYER
	ID3D12Debug *pdx12Debug = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&pdx12Debug))))
		pdx12Debug->EnableDebugLayer();
#endif

	// Create device
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	if (D3D12CreateDevice(nullptr, featureLevel, IID_PPV_ARGS(&g_pd3dDevice)) != S_OK)
		return false;

	// [DEBUG] Setup debug interface to break on any warnings/errors
#ifdef DX12_ENABLE_DEBUG_LAYER
	if (pdx12Debug != nullptr)
	{
		ID3D12InfoQueue *pInfoQueue = nullptr;
		g_pd3dDevice->QueryInterface(IID_PPV_ARGS(&pInfoQueue));
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, false);

		// Disable breaking on this warning because of a suspected bug in the D3D12 SDK layer, see #9084 for details.
		const int D3D12_MESSAGE_ID_FENCE_ZERO_WAIT_ = 1424; // not in all copies of d3d12sdklayers.h
		D3D12_MESSAGE_ID disabledMessages[] = {(D3D12_MESSAGE_ID)D3D12_MESSAGE_ID_FENCE_ZERO_WAIT_};
		D3D12_INFO_QUEUE_FILTER filter = {};
		filter.DenyList.NumIDs = 1;
		filter.DenyList.pIDList = disabledMessages;
		pInfoQueue->AddStorageFilterEntries(&filter);

		pInfoQueue->Release();
		pdx12Debug->Release();
	}
#endif

	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.NumDescriptors = APP_NUM_BACK_BUFFERS + 2 + (BLOOM_MIP_COUNT * 2);
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.NodeMask = 1;
		if (g_pd3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_pd3dRtvDescHeap)) != S_OK)
			return false;

		SIZE_T rtvDescriptorSize = g_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = g_pd3dRtvDescHeap->GetCPUDescriptorHandleForHeapStart();
		for (UINT i = 0; i < APP_NUM_BACK_BUFFERS; i++)
		{
			g_mainRenderTargetDescriptor[i] = rtvHandle;
			rtvHandle.ptr += rtvDescriptorSize;
		}
		g_hdrRenderTargetDescriptor = rtvHandle;
		rtvHandle.ptr += rtvDescriptorSize;
		g_hdrEmissiveRenderTargetDescriptor = rtvHandle;
		rtvHandle.ptr += rtvDescriptorSize;
		for (int i = 0; i < BLOOM_MIP_COUNT; i++)
		{
			g_bloomDownRtv12[i] = rtvHandle;
			rtvHandle.ptr += rtvDescriptorSize;
		}
		for (int i = 0; i < BLOOM_MIP_COUNT; i++)
		{
			g_bloomBlurRtv12[i] = rtvHandle;
			rtvHandle.ptr += rtvDescriptorSize;
		}
	}

	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		desc.NumDescriptors = APP_SRV_HEAP_SIZE;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		if (g_pd3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_pd3dSrvDescHeap)) != S_OK)
			return false;
		g_pd3dSrvDescHeapAlloc.Create(g_pd3dDevice, g_pd3dSrvDescHeap);
	}

	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		desc.NumDescriptors = 2 + (BLOOM_MIP_COUNT * 2);
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		if (g_pd3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_pd3dPostSrvDescHeap)) != S_OK)
			return false;
		g_postSrvHeapStartCpu = g_pd3dPostSrvDescHeap->GetCPUDescriptorHandleForHeapStart();
		g_postSrvHeapStartGpu = g_pd3dPostSrvDescHeap->GetGPUDescriptorHandleForHeapStart();
		g_postSrvDescriptorSize = g_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	{
		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 1;
		if (g_pd3dDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&g_pd3dCommandQueue)) != S_OK)
			return false;
	}

	for (UINT i = 0; i < APP_NUM_FRAMES_IN_FLIGHT; i++)
		if (g_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_frameContext[i].CommandAllocator)) != S_OK)
			return false;

	if (g_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_frameContext[0].CommandAllocator, nullptr, IID_PPV_ARGS(&g_pd3dCommandList)) != S_OK ||
		g_pd3dCommandList->Close() != S_OK)
		return false;

	if (g_pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_fence)) != S_OK)
		return false;

	g_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (g_fenceEvent == nullptr)
		return false;

	{
		IDXGIFactory5 *dxgiFactory = nullptr;
		IDXGISwapChain1 *swapChain1 = nullptr;
		if (CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)) != S_OK)
			return false;

		BOOL allow_tearing = FALSE;
		dxgiFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allow_tearing, sizeof(allow_tearing));
		g_SwapChainTearingSupport = (allow_tearing == TRUE);
		if (g_SwapChainTearingSupport)
			sd.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

		if (dxgiFactory->CreateSwapChainForHwnd(g_pd3dCommandQueue, hWnd, &sd, nullptr, nullptr, &swapChain1) != S_OK)
			return false;
		if (swapChain1->QueryInterface(IID_PPV_ARGS(&g_pSwapChain)) != S_OK)
			return false;
		if (g_SwapChainTearingSupport)
			dxgiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);

		swapChain1->Release();
		dxgiFactory->Release();
		g_pSwapChain->SetMaximumFrameLatency(APP_NUM_BACK_BUFFERS);
		g_hSwapChainWaitableObject = g_pSwapChain->GetFrameLatencyWaitableObject();
	}

	CreateRenderTarget();
	return true;
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain)
	{
		g_pSwapChain->SetFullscreenState(false, nullptr);
		g_pSwapChain->Release();
		g_pSwapChain = nullptr;
	}
	if (g_hSwapChainWaitableObject != nullptr)
	{
		CloseHandle(g_hSwapChainWaitableObject);
	}
	for (UINT i = 0; i < APP_NUM_FRAMES_IN_FLIGHT; i++)
		if (g_frameContext[i].CommandAllocator)
		{
			g_frameContext[i].CommandAllocator->Release();
			g_frameContext[i].CommandAllocator = nullptr;
		}
	if (g_pd3dCommandQueue)
	{
		g_pd3dCommandQueue->Release();
		g_pd3dCommandQueue = nullptr;
	}
	if (g_pd3dCommandList)
	{
		g_pd3dCommandList->Release();
		g_pd3dCommandList = nullptr;
	}
	if (g_pd3dRtvDescHeap)
	{
		g_pd3dRtvDescHeap->Release();
		g_pd3dRtvDescHeap = nullptr;
	}
	if (g_pd3dSrvDescHeap)
	{
		g_pd3dSrvDescHeap->Release();
		g_pd3dSrvDescHeap = nullptr;
	}
	if (g_pd3dPostSrvDescHeap)
	{
		g_pd3dPostSrvDescHeap->Release();
		g_pd3dPostSrvDescHeap = nullptr;
	}
	g_postSrvHeapStartCpu.ptr = 0;
	g_postSrvHeapStartGpu.ptr = 0;
	g_postSrvDescriptorSize = 0;
	if (g_fence)
	{
		g_fence->Release();
		g_fence = nullptr;
	}
	if (g_fenceEvent)
	{
		CloseHandle(g_fenceEvent);
		g_fenceEvent = nullptr;
	}
	if (g_pd3dDevice)
	{
		g_pd3dDevice->Release();
		g_pd3dDevice = nullptr;
	}

#ifdef DX12_ENABLE_DEBUG_LAYER
	IDXGIDebug1 *pDebug = nullptr;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&pDebug))))
	{
		pDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_SUMMARY);
		pDebug->Release();
	}
#endif
}

void CreateRenderTarget()
{
	for (UINT i = 0; i < APP_NUM_BACK_BUFFERS; i++)
	{
		ID3D12Resource *pBackBuffer = nullptr;
		g_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
		g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, g_mainRenderTargetDescriptor[i]);
		g_mainRenderTargetResource[i] = pBackBuffer;
	}
	if (g_mainRenderTargetResource[0])
	{
		D3D12_RESOURCE_DESC desc = g_mainRenderTargetResource[0]->GetDesc();
		CreateHDRResourcesDX12((UINT)desc.Width, desc.Height);
	}
}

void CleanupRenderTarget()
{
	WaitForPendingOperations();

	for (UINT i = 0; i < APP_NUM_BACK_BUFFERS; i++)
		if (g_mainRenderTargetResource[i])
		{
			g_mainRenderTargetResource[i]->Release();
			g_mainRenderTargetResource[i] = nullptr;
		}
	CleanupHDRResourcesDX12();
	if (g_tonemapPipelineState)
	{
		g_tonemapPipelineState->Release();
		g_tonemapPipelineState = nullptr;
	}
	if (g_emissiveAddPipelineState)
	{
		g_emissiveAddPipelineState->Release();
		g_emissiveAddPipelineState = nullptr;
	}
	if (g_bloomPrefilterPSO)
	{
		g_bloomPrefilterPSO->Release();
		g_bloomPrefilterPSO = nullptr;
	}
	if (g_bloomDownsamplePSO)
	{
		g_bloomDownsamplePSO->Release();
		g_bloomDownsamplePSO = nullptr;
	}
	if (g_bloomBlurPSO)
	{
		g_bloomBlurPSO->Release();
		g_bloomBlurPSO = nullptr;
	}
	if (g_bloomUpsamplePSO)
	{
		g_bloomUpsamplePSO->Release();
		g_bloomUpsamplePSO = nullptr;
	}
	if (g_tonemapRootSignature)
	{
		g_tonemapRootSignature->Release();
		g_tonemapRootSignature = nullptr;
	}
}

bool CreateTonemapPipelineDX12()
{
	if (g_tonemapPipelineState && g_emissiveAddPipelineState)
		return true;

	D3D12_DESCRIPTOR_RANGE descRange0 = {};
	descRange0.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descRange0.NumDescriptors = 1;
	descRange0.BaseShaderRegister = 0;
	descRange0.RegisterSpace = 0;
	descRange0.OffsetInDescriptorsFromTableStart = 0;

	D3D12_DESCRIPTOR_RANGE descRange1 = {};
	descRange1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descRange1.NumDescriptors = 1;
	descRange1.BaseShaderRegister = 1;
	descRange1.RegisterSpace = 0;
	descRange1.OffsetInDescriptorsFromTableStart = 0;

	D3D12_ROOT_PARAMETER param[3] = {};
	param[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	param[0].DescriptorTable.NumDescriptorRanges = 1;
	param[0].DescriptorTable.pDescriptorRanges = &descRange0;
	param[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	param[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	param[1].DescriptorTable.NumDescriptorRanges = 1;
	param[1].DescriptorTable.pDescriptorRanges = &descRange1;
	param[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	param[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	param[2].Constants.ShaderRegister = 0;
	param[2].Constants.RegisterSpace = 0;
	param[2].Constants.Num32BitValues = 8;
	param[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_STATIC_SAMPLER_DESC staticSampler = {};
	staticSampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSampler.MipLODBias = 0.f;
	staticSampler.MaxAnisotropy = 0;
	staticSampler.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	staticSampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	staticSampler.MinLOD = 0.f;
	staticSampler.MaxLOD = D3D12_FLOAT32_MAX;
	staticSampler.ShaderRegister = 0;
	staticSampler.RegisterSpace = 0;
	staticSampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_ROOT_SIGNATURE_DESC rsDesc = {};
	rsDesc.NumParameters = _countof(param);
	rsDesc.pParameters = param;
	rsDesc.NumStaticSamplers = 1;
	rsDesc.pStaticSamplers = &staticSampler;
	rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob *signatureBlob = nullptr;
	ID3DBlob *errorBlob = nullptr;
	if (D3D12SerializeRootSignature(&rsDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob) != S_OK)
	{
		if (errorBlob)
			errorBlob->Release();
		return false;
	}
	HRESULT hr = g_pd3dDevice->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&g_tonemapRootSignature));
	signatureBlob->Release();
	if (errorBlob)
		errorBlob->Release();
	if (FAILED(hr))
		return false;

	static const char *vsSrc =
		"struct VSOut { float4 pos : SV_POSITION; float2 uv : TEXCOORD0; };\
		 VSOut main(uint id : SV_VertexID)\
		 {\
		   float2 pos = float2((id == 2) ? 3.0f : -1.0f, (id == 1) ? 3.0f : -1.0f);\
		   VSOut o; o.pos = float4(pos, 0.0f, 1.0f);\
		   o.uv = float2((pos.x + 1.0f) * 0.5f, (1.0f - pos.y) * 0.5f);\
		   return o;\
		 }";

	static const char *psSrc =
		"Texture2D hdrTex : register(t0);\
		 Texture2D bloomTex : register(t1);\
		 SamplerState samp0 : register(s0);\
		 cbuffer Params : register(b0) { float Exposure; float BloomIntensity; float2 pad0; float4 pad1; };\
		 float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_Target\
		 {\
		   float3 baseColor = hdrTex.Sample(samp0, uv).rgb * Exposure;\
		   float3 bloom = bloomTex.Sample(samp0, uv).rgb * BloomIntensity;\
		   float3 color = baseColor + bloom;\
		   color = color / (1.0 + color);\
		   color = pow(color, 1.0 / 2.2);\
		   return float4(color, 1.0);\
		 }";

	ID3DBlob *vsBlob = nullptr;
	ID3DBlob *psBlob = nullptr;
	if (FAILED(D3DCompile(vsSrc, strlen(vsSrc), nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, &vsBlob, nullptr)))
		return false;
	if (FAILED(D3DCompile(psSrc, strlen(psSrc), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &psBlob, nullptr)))
	{
		vsBlob->Release();
		return false;
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = g_tonemapRootSignature;
	psoDesc.VS = {vsBlob->GetBufferPointer(), vsBlob->GetBufferSize()};
	psoDesc.PS = {psBlob->GetBufferPointer(), psBlob->GetBufferSize()};
	D3D12_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = false;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	psoDesc.BlendState = blendDesc;
	psoDesc.SampleMask = UINT_MAX;
	D3D12_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterDesc.CullMode = D3D12_CULL_MODE_NONE;
	rasterDesc.FrontCounterClockwise = FALSE;
	rasterDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasterDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.MultisampleEnable = FALSE;
	rasterDesc.AntialiasedLineEnable = FALSE;
	rasterDesc.ForcedSampleCount = 0;
	rasterDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	psoDesc.RasterizerState = rasterDesc;
	D3D12_DEPTH_STENCIL_DESC depthDesc = {};
	depthDesc.DepthEnable = false;
	depthDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	depthDesc.StencilEnable = false;
	depthDesc.FrontFace.StencilFailOp = depthDesc.FrontFace.StencilDepthFailOp = depthDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	depthDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	depthDesc.BackFace = depthDesc.FrontFace;
	psoDesc.DepthStencilState = depthDesc;
	psoDesc.InputLayout = {nullptr, 0};
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;

	hr = g_pd3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&g_tonemapPipelineState));
	if (FAILED(hr))
	{
		vsBlob->Release();
		psBlob->Release();
		return false;
	}

	D3D12_BLEND_DESC addBlend = blendDesc;
	addBlend.RenderTarget[0].BlendEnable = TRUE;
	addBlend.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	addBlend.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	addBlend.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	addBlend.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	addBlend.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
	addBlend.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	psoDesc.BlendState = addBlend;
	hr = g_pd3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&g_emissiveAddPipelineState));
	vsBlob->Release();
	psBlob->Release();
	return SUCCEEDED(hr);
}

void CreateHDRResourcesDX12(UINT width, UINT height)
{
	CleanupHDRResourcesDX12();
	if (!g_pd3dDevice)
		return;

	D3D12_HEAP_PROPERTIES heapProps = {};
	heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment = 0;
	desc.Width = width;
	desc.Height = height;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	clearValue.Color[0] = 0.0f;
	clearValue.Color[1] = 0.0f;
	clearValue.Color[2] = 0.0f;
	clearValue.Color[3] = 0.0f;

	if (FAILED(g_pd3dDevice->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue, IID_PPV_ARGS(&g_hdrRenderTargetResource))))
		return;

	g_pd3dDevice->CreateRenderTargetView(g_hdrRenderTargetResource, nullptr, g_hdrRenderTargetDescriptor);

	if (FAILED(g_pd3dDevice->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue, IID_PPV_ARGS(&g_hdrEmissiveRenderTargetResource))))
		return;

	g_pd3dDevice->CreateRenderTargetView(g_hdrEmissiveRenderTargetResource, nullptr, g_hdrEmissiveRenderTargetDescriptor);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	g_hdrSrvCpuHandle.ptr = g_postSrvHeapStartCpu.ptr + g_postSrvDescriptorSize * 0;
	g_hdrSrvGpuHandle.ptr = g_postSrvHeapStartGpu.ptr + g_postSrvDescriptorSize * 0;
	g_pd3dDevice->CreateShaderResourceView(g_hdrRenderTargetResource, &srvDesc, g_hdrSrvCpuHandle);

	g_hdrEmissiveSrvCpuHandle.ptr = g_postSrvHeapStartCpu.ptr + g_postSrvDescriptorSize * 1;
	g_hdrEmissiveSrvGpuHandle.ptr = g_postSrvHeapStartGpu.ptr + g_postSrvDescriptorSize * 1;
	g_pd3dDevice->CreateShaderResourceView(g_hdrEmissiveRenderTargetResource, &srvDesc, g_hdrEmissiveSrvCpuHandle);

	CreateTonemapPipelineDX12();
	CreateBloomResourcesDX12(width, height);
	CreateBloomPipelinesDX12();
}

void CleanupHDRResourcesDX12()
{
	if (g_hdrRenderTargetResource)
	{
		g_hdrRenderTargetResource->Release();
		g_hdrRenderTargetResource = nullptr;
	}
	if (g_hdrEmissiveRenderTargetResource)
	{
		g_hdrEmissiveRenderTargetResource->Release();
		g_hdrEmissiveRenderTargetResource = nullptr;
	}
	CleanupBloomResourcesDX12();
	g_hdrSrvCpuHandle.ptr = 0;
	g_hdrSrvGpuHandle.ptr = 0;
	g_hdrEmissiveSrvCpuHandle.ptr = 0;
	g_hdrEmissiveSrvGpuHandle.ptr = 0;
}

void CreateBloomResourcesDX12(UINT width, UINT height)
{
	CleanupBloomResourcesDX12();
	if (!g_pd3dDevice)
		return;

	UINT base_w = (width / 2) > 0 ? (width / 2) : 1;
	UINT base_h = (height / 2) > 0 ? (height / 2) : 1;

	D3D12_HEAP_PROPERTIES heapProps = {};
	heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	for (int i = 0; i < BLOOM_MIP_COUNT; i++)
	{
		const UINT w = (base_w >> i) > 0 ? (base_w >> i) : 1;
		const UINT h = (base_h >> i) > 0 ? (base_h >> i) : 1;
		g_bloomWidth12[i] = w;
		g_bloomHeight12[i] = h;

		D3D12_RESOURCE_DESC desc = {};
		desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Alignment = 0;
		desc.Width = w;
		desc.Height = h;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

		D3D12_CLEAR_VALUE clearValue = {};
		clearValue.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		clearValue.Color[0] = 0.0f;
		clearValue.Color[1] = 0.0f;
		clearValue.Color[2] = 0.0f;
		clearValue.Color[3] = 0.0f;

		if (SUCCEEDED(g_pd3dDevice->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue, IID_PPV_ARGS(&g_bloomDownTex12[i]))))
		{
			g_pd3dDevice->CreateRenderTargetView(g_bloomDownTex12[i], nullptr, g_bloomDownRtv12[i]);

			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			g_bloomDownSrvCpu12[i].ptr = g_postSrvHeapStartCpu.ptr + g_postSrvDescriptorSize * (2 + i);
			g_bloomDownSrvGpu12[i].ptr = g_postSrvHeapStartGpu.ptr + g_postSrvDescriptorSize * (2 + i);
			g_pd3dDevice->CreateShaderResourceView(g_bloomDownTex12[i], &srvDesc, g_bloomDownSrvCpu12[i]);
		}

		if (SUCCEEDED(g_pd3dDevice->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue, IID_PPV_ARGS(&g_bloomBlurTex12[i]))))
		{
			g_pd3dDevice->CreateRenderTargetView(g_bloomBlurTex12[i], nullptr, g_bloomBlurRtv12[i]);

			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			const UINT blur_index = 2 + BLOOM_MIP_COUNT + i;
			g_bloomBlurSrvCpu12[i].ptr = g_postSrvHeapStartCpu.ptr + g_postSrvDescriptorSize * blur_index;
			g_bloomBlurSrvGpu12[i].ptr = g_postSrvHeapStartGpu.ptr + g_postSrvDescriptorSize * blur_index;
			g_pd3dDevice->CreateShaderResourceView(g_bloomBlurTex12[i], &srvDesc, g_bloomBlurSrvCpu12[i]);
		}
	}
}

void CleanupBloomResourcesDX12()
{
	for (int i = 0; i < BLOOM_MIP_COUNT; i++)
	{
		if (g_bloomDownTex12[i]) { g_bloomDownTex12[i]->Release(); g_bloomDownTex12[i] = nullptr; }
		if (g_bloomBlurTex12[i]) { g_bloomBlurTex12[i]->Release(); g_bloomBlurTex12[i] = nullptr; }
		g_bloomDownSrvCpu12[i].ptr = 0;
		g_bloomDownSrvGpu12[i].ptr = 0;
		g_bloomBlurSrvCpu12[i].ptr = 0;
		g_bloomBlurSrvGpu12[i].ptr = 0;
	}
}

bool CreateBloomPipelinesDX12()
{
	if (g_bloomPrefilterPSO && g_bloomDownsamplePSO && g_bloomBlurPSO && g_bloomUpsamplePSO)
		return true;
	if (!g_tonemapRootSignature)
		return false;

	static const char *vsSrc =
		"struct VSOut { float4 pos : SV_POSITION; float2 uv : TEXCOORD0; };\
		 VSOut main(uint id : SV_VertexID)\
		 {\
		   float2 pos = float2((id == 2) ? 3.0f : -1.0f, (id == 1) ? 3.0f : -1.0f);\
		   VSOut o; o.pos = float4(pos, 0.0f, 1.0f);\
		   o.uv = float2((pos.x + 1.0f) * 0.5f, (1.0f - pos.y) * 0.5f);\
		   return o;\
		 }";

	static const char *prefilterSrc =
		"Texture2D srcTex : register(t0);\
		 SamplerState samp0 : register(s0);\
		 cbuffer Params : register(b0)\
		 {\
		   float2 TexelSize;\
		   float Threshold;\
		   float Knee;\
		   float Intensity;\
		   float3 pad;\
		 };\
		 float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_Target\
		 {\
		   float3 color = srcTex.Sample(samp0, uv).rgb;\
		   float brightness = max(max(color.r, color.g), color.b);\
		   float knee = max(Knee, 1e-5);\
		   float soft = saturate((brightness - Threshold + knee) / (2.0 * knee));\
		   float contrib = max(brightness - Threshold, 0.0) + soft * soft * knee;\
		   float scale = contrib / max(brightness, 1e-5);\
		   return float4(color * scale, 1.0);\
		 }";
	static const char *downsampleSrc =
		"Texture2D srcTex : register(t0);\
		 SamplerState samp0 : register(s0);\
		 cbuffer Params : register(b0)\
		 {\
		   float2 TexelSize;\
		   float Threshold;\
		   float Knee;\
		   float Intensity;\
		   float3 pad;\
		 };\
		 float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_Target\
		 {\
		   float2 o = TexelSize * 0.5;\
		   float3 sum = srcTex.Sample(samp0, uv + float2(-o.x, -o.y)).rgb;\
		   sum += srcTex.Sample(samp0, uv + float2( o.x, -o.y)).rgb;\
		   sum += srcTex.Sample(samp0, uv + float2(-o.x,  o.y)).rgb;\
		   sum += srcTex.Sample(samp0, uv + float2( o.x,  o.y)).rgb;\
		   return float4(sum * 0.25, 1.0);\
		 }";
	static const char *blurSrc =
		"Texture2D srcTex : register(t0);\
		 SamplerState samp0 : register(s0);\
		 cbuffer Params : register(b0)\
		 {\
		   float2 TexelSize;\
		   float Threshold;\
		   float Knee;\
		   float Intensity;\
		   float3 pad;\
		 };\
		 float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_Target\
		 {\
		   float2 t = TexelSize;\
		   float3 c = srcTex.Sample(samp0, uv).rgb * 0.4;\
		   c += srcTex.Sample(samp0, uv + float2( t.x, 0)).rgb * 0.1;\
		   c += srcTex.Sample(samp0, uv + float2(-t.x, 0)).rgb * 0.1;\
		   c += srcTex.Sample(samp0, uv + float2(0,  t.y)).rgb * 0.1;\
		   c += srcTex.Sample(samp0, uv + float2(0, -t.y)).rgb * 0.1;\
		   c += srcTex.Sample(samp0, uv + float2( t.x,  t.y)).rgb * 0.05;\
		   c += srcTex.Sample(samp0, uv + float2(-t.x,  t.y)).rgb * 0.05;\
		   c += srcTex.Sample(samp0, uv + float2( t.x, -t.y)).rgb * 0.05;\
		   c += srcTex.Sample(samp0, uv + float2(-t.x, -t.y)).rgb * 0.05;\
		   return float4(c, 1.0);\
		 }";
	static const char *upsampleSrc =
		"Texture2D srcTex : register(t0);\
		 SamplerState samp0 : register(s0);\
		 cbuffer Params : register(b0)\
		 {\
		   float2 TexelSize;\
		   float Threshold;\
		   float Knee;\
		   float Intensity;\
		   float3 pad;\
		 };\
		 float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_Target\
		 {\
		   float3 c = srcTex.Sample(samp0, uv).rgb * Intensity;\
		   return float4(c, 1.0);\
		 }";

	ID3DBlob *vsBlob = nullptr;
	ID3DBlob *psPrefilter = nullptr;
	ID3DBlob *psDownsample = nullptr;
	ID3DBlob *psBlur = nullptr;
	ID3DBlob *psUpsample = nullptr;

	if (FAILED(D3DCompile(vsSrc, strlen(vsSrc), nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, &vsBlob, nullptr)))
		return false;
	if (FAILED(D3DCompile(prefilterSrc, strlen(prefilterSrc), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &psPrefilter, nullptr)))
		return false;
	if (FAILED(D3DCompile(downsampleSrc, strlen(downsampleSrc), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &psDownsample, nullptr)))
		return false;
	if (FAILED(D3DCompile(blurSrc, strlen(blurSrc), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &psBlur, nullptr)))
		return false;
	if (FAILED(D3DCompile(upsampleSrc, strlen(upsampleSrc), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &psUpsample, nullptr)))
		return false;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.pRootSignature = g_tonemapRootSignature;
	psoDesc.VS = {vsBlob->GetBufferPointer(), vsBlob->GetBufferSize()};
	psoDesc.SampleMask = UINT_MAX;
	D3D12_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterDesc.CullMode = D3D12_CULL_MODE_NONE;
	rasterDesc.FrontCounterClockwise = FALSE;
	rasterDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasterDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.MultisampleEnable = FALSE;
	rasterDesc.AntialiasedLineEnable = FALSE;
	rasterDesc.ForcedSampleCount = 0;
	rasterDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	psoDesc.RasterizerState = rasterDesc;

	D3D12_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = false;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	psoDesc.BlendState = blendDesc;

	D3D12_DEPTH_STENCIL_DESC depthDesc = {};
	depthDesc.DepthEnable = false;
	depthDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	depthDesc.StencilEnable = false;
	depthDesc.FrontFace.StencilFailOp = depthDesc.FrontFace.StencilDepthFailOp = depthDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	depthDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	depthDesc.BackFace = depthDesc.FrontFace;
	psoDesc.DepthStencilState = depthDesc;
	psoDesc.InputLayout = {nullptr, 0};
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
	psoDesc.SampleDesc.Count = 1;

	psoDesc.PS = {psPrefilter->GetBufferPointer(), psPrefilter->GetBufferSize()};
	if (FAILED(g_pd3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&g_bloomPrefilterPSO))))
		return false;

	psoDesc.PS = {psDownsample->GetBufferPointer(), psDownsample->GetBufferSize()};
	if (FAILED(g_pd3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&g_bloomDownsamplePSO))))
		return false;

	psoDesc.PS = {psBlur->GetBufferPointer(), psBlur->GetBufferSize()};
	if (FAILED(g_pd3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&g_bloomBlurPSO))))
		return false;

	D3D12_BLEND_DESC addBlend = blendDesc;
	addBlend.RenderTarget[0].BlendEnable = TRUE;
	addBlend.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	addBlend.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	addBlend.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	addBlend.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	addBlend.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
	addBlend.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	psoDesc.BlendState = addBlend;
	psoDesc.PS = {psUpsample->GetBufferPointer(), psUpsample->GetBufferSize()};
	if (FAILED(g_pd3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&g_bloomUpsamplePSO))))
		return false;

	vsBlob->Release();
	psPrefilter->Release();
	psDownsample->Release();
	psBlur->Release();
	psUpsample->Release();
	return true;
}

void RenderBloomDX12(ID3D12GraphicsCommandList *cmd)
{
	if (!g_hdrEmissiveRenderTargetResource || !g_bloomPrefilterPSO || !g_bloomDownTex12[0])
		return;

	auto transition = [&](ID3D12Resource *res, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
	{
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = res;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = before;
		barrier.Transition.StateAfter = after;
		cmd->ResourceBarrier(1, &barrier);
	};

	auto set_constants = [&](UINT w, UINT h, float threshold, float knee, float intensity)
	{
		float params[8] = {};
		params[0] = (w > 0) ? (1.0f / (float)w) : 0.0f;
		params[1] = (h > 0) ? (1.0f / (float)h) : 0.0f;
		params[2] = threshold;
		params[3] = knee;
		params[4] = intensity;
		cmd->SetGraphicsRoot32BitConstants(2, 8, params, 0);
	};

	cmd->SetGraphicsRootSignature(g_tonemapRootSignature);
	cmd->SetDescriptorHeaps(1, &g_pd3dPostSrvDescHeap);
	cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Prefilter
	{
		transition(g_bloomDownTex12[0], D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
		cmd->OMSetRenderTargets(1, &g_bloomDownRtv12[0], FALSE, nullptr);
		const float clear[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		cmd->ClearRenderTargetView(g_bloomDownRtv12[0], clear, 0, nullptr);
		D3D12_VIEWPORT vp = {0.0f, 0.0f, (float)g_bloomWidth12[0], (float)g_bloomHeight12[0], 0.0f, 1.0f};
		D3D12_RECT sc = {0, 0, (LONG)g_bloomWidth12[0], (LONG)g_bloomHeight12[0]};
		cmd->RSSetViewports(1, &vp);
		cmd->RSSetScissorRects(1, &sc);
		cmd->SetPipelineState(g_bloomPrefilterPSO);
		set_constants(g_bloomWidth12[0], g_bloomHeight12[0], g_bloomThreshold, g_bloomKnee, 1.0f);
		cmd->SetGraphicsRootDescriptorTable(0, g_hdrEmissiveSrvGpuHandle);
		cmd->SetGraphicsRootDescriptorTable(1, g_hdrEmissiveSrvGpuHandle);
		cmd->DrawInstanced(3, 1, 0, 0);
		transition(g_bloomDownTex12[0], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	}

	// Downsample chain
	for (int i = 1; i < BLOOM_MIP_COUNT; i++)
	{
		transition(g_bloomDownTex12[i], D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
		cmd->OMSetRenderTargets(1, &g_bloomDownRtv12[i], FALSE, nullptr);
		const float clear[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		cmd->ClearRenderTargetView(g_bloomDownRtv12[i], clear, 0, nullptr);
		D3D12_VIEWPORT vp = {0.0f, 0.0f, (float)g_bloomWidth12[i], (float)g_bloomHeight12[i], 0.0f, 1.0f};
		D3D12_RECT sc = {0, 0, (LONG)g_bloomWidth12[i], (LONG)g_bloomHeight12[i]};
		cmd->RSSetViewports(1, &vp);
		cmd->RSSetScissorRects(1, &sc);
		cmd->SetPipelineState(g_bloomDownsamplePSO);
		set_constants(g_bloomWidth12[i - 1], g_bloomHeight12[i - 1], g_bloomThreshold, g_bloomKnee, 1.0f);
		cmd->SetGraphicsRootDescriptorTable(0, g_bloomDownSrvGpu12[i - 1]);
		cmd->SetGraphicsRootDescriptorTable(1, g_bloomDownSrvGpu12[i - 1]);
		cmd->DrawInstanced(3, 1, 0, 0);
		transition(g_bloomDownTex12[i], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	}

	// Blur mips
	for (int i = 0; i < BLOOM_MIP_COUNT; i++)
	{
		transition(g_bloomBlurTex12[i], D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
		cmd->OMSetRenderTargets(1, &g_bloomBlurRtv12[i], FALSE, nullptr);
		const float clear[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		cmd->ClearRenderTargetView(g_bloomBlurRtv12[i], clear, 0, nullptr);
		D3D12_VIEWPORT vp = {0.0f, 0.0f, (float)g_bloomWidth12[i], (float)g_bloomHeight12[i], 0.0f, 1.0f};
		D3D12_RECT sc = {0, 0, (LONG)g_bloomWidth12[i], (LONG)g_bloomHeight12[i]};
		cmd->RSSetViewports(1, &vp);
		cmd->RSSetScissorRects(1, &sc);
		cmd->SetPipelineState(g_bloomBlurPSO);
		set_constants(g_bloomWidth12[i], g_bloomHeight12[i], g_bloomThreshold, g_bloomKnee, 1.0f);
		cmd->SetGraphicsRootDescriptorTable(0, g_bloomDownSrvGpu12[i]);
		cmd->SetGraphicsRootDescriptorTable(1, g_bloomDownSrvGpu12[i]);
		cmd->DrawInstanced(3, 1, 0, 0);
		transition(g_bloomBlurTex12[i], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	}

	// Upsample combine
	for (int i = BLOOM_MIP_COUNT - 1; i > 0; i--)
	{
		transition(g_bloomBlurTex12[i - 1], D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
		cmd->OMSetRenderTargets(1, &g_bloomBlurRtv12[i - 1], FALSE, nullptr);
		D3D12_VIEWPORT vp = {0.0f, 0.0f, (float)g_bloomWidth12[i - 1], (float)g_bloomHeight12[i - 1], 0.0f, 1.0f};
		D3D12_RECT sc = {0, 0, (LONG)g_bloomWidth12[i - 1], (LONG)g_bloomHeight12[i - 1]};
		cmd->RSSetViewports(1, &vp);
		cmd->RSSetScissorRects(1, &sc);
		cmd->SetPipelineState(g_bloomUpsamplePSO);
		set_constants(g_bloomWidth12[i - 1], g_bloomHeight12[i - 1], g_bloomThreshold, g_bloomKnee, 1.0f);
		cmd->SetGraphicsRootDescriptorTable(0, g_bloomBlurSrvGpu12[i]);
		cmd->SetGraphicsRootDescriptorTable(1, g_bloomBlurSrvGpu12[i]);
		cmd->DrawInstanced(3, 1, 0, 0);
		transition(g_bloomBlurTex12[i - 1], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	}
}

void CreateRenderTargetDX11()
{
	ID3D11Texture2D *pBackBuffer = nullptr;
	g_pSwapChain11->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (pBackBuffer)
	{
		g_pd3dDevice11->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView11);
		pBackBuffer->Release();
	}
	CreateHDRResourcesDX11();
}

void CleanupRenderTargetDX11()
{
	if (g_mainRenderTargetView11)
	{
		g_mainRenderTargetView11->Release();
		g_mainRenderTargetView11 = nullptr;
	}
	CleanupHDRResourcesDX11();
}

void CreateHDRResourcesDX11()
{
	CleanupHDRResourcesDX11();

	ID3D11Texture2D *backBuffer = nullptr;
	g_pSwapChain11->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (!backBuffer)
		return;

	D3D11_TEXTURE2D_DESC bbDesc = {};
	backBuffer->GetDesc(&bbDesc);
	backBuffer->Release();

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = bbDesc.Width;
	desc.Height = bbDesc.Height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	if (FAILED(g_pd3dDevice11->CreateTexture2D(&desc, nullptr, &g_hdrTexture11)))
		return;

	g_pd3dDevice11->CreateRenderTargetView(g_hdrTexture11, nullptr, &g_hdrRTV11);
	g_pd3dDevice11->CreateShaderResourceView(g_hdrTexture11, nullptr, &g_hdrSRV11);

	if (FAILED(g_pd3dDevice11->CreateTexture2D(&desc, nullptr, &g_hdrEmissiveTexture11)))
		return;

	g_pd3dDevice11->CreateRenderTargetView(g_hdrEmissiveTexture11, nullptr, &g_hdrEmissiveRTV11);
	g_pd3dDevice11->CreateShaderResourceView(g_hdrEmissiveTexture11, nullptr, &g_hdrEmissiveSRV11);

	CreateBloomResourcesDX11(bbDesc.Width, bbDesc.Height);

	if (!g_tonemapVS11 || !g_tonemapPS11)
	{
		static const char *vsSrc =
			"struct VSOut { float4 pos : SV_POSITION; float2 uv : TEXCOORD0; };\
			 VSOut main(uint id : SV_VertexID)\
			 {\
			   float2 pos = float2((id == 2) ? 3.0f : -1.0f, (id == 1) ? 3.0f : -1.0f);\
			   VSOut o; o.pos = float4(pos, 0.0f, 1.0f);\
			   o.uv = float2((pos.x + 1.0f) * 0.5f, (1.0f - pos.y) * 0.5f);\
			   return o;\
			 }";

		static const char *psSrc =
			"Texture2D hdrTex : register(t0);\
			 Texture2D bloomTex : register(t1);\
			 SamplerState samp0 : register(s0);\
			 cbuffer Params : register(b0) { float Exposure; float BloomIntensity; float2 pad; };\
			 float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_Target\
			 {\
			   float3 baseColor = hdrTex.Sample(samp0, uv).rgb * Exposure;\
			   float3 bloom = bloomTex.Sample(samp0, uv).rgb * BloomIntensity;\
			   float3 color = baseColor + bloom;\
			   color = color / (1.0 + color);\
			   color = pow(color, 1.0 / 2.2);\
			   return float4(color, 1.0);\
			 }";

		ID3DBlob *vsBlob = nullptr;
		ID3DBlob *psBlob = nullptr;
		if (SUCCEEDED(D3DCompile(vsSrc, strlen(vsSrc), nullptr, nullptr, nullptr, "main", "vs_4_0", 0, 0, &vsBlob, nullptr)) &&
			SUCCEEDED(D3DCompile(psSrc, strlen(psSrc), nullptr, nullptr, nullptr, "main", "ps_4_0", 0, 0, &psBlob, nullptr)))
		{
			g_pd3dDevice11->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &g_tonemapVS11);
			g_pd3dDevice11->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &g_tonemapPS11);
		}
		if (vsBlob) vsBlob->Release();
		if (psBlob) psBlob->Release();

		D3D11_SAMPLER_DESC sampDesc = {};
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		g_pd3dDevice11->CreateSamplerState(&sampDesc, &g_tonemapSampler11);

		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.RenderTarget[0].BlendEnable = FALSE;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		g_pd3dDevice11->CreateBlendState(&blendDesc, &g_tonemapBlend11);

		D3D11_BLEND_DESC blendAdd = blendDesc;
		blendAdd.RenderTarget[0].BlendEnable = TRUE;
		blendAdd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendAdd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendAdd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendAdd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendAdd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendAdd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		g_pd3dDevice11->CreateBlendState(&blendAdd, &g_tonemapAddBlend11);

		D3D11_RASTERIZER_DESC rastDesc = {};
		rastDesc.FillMode = D3D11_FILL_SOLID;
		rastDesc.CullMode = D3D11_CULL_NONE;
		rastDesc.ScissorEnable = FALSE;
		rastDesc.DepthClipEnable = TRUE;
		g_pd3dDevice11->CreateRasterizerState(&rastDesc, &g_tonemapRasterizer11);

		D3D11_DEPTH_STENCIL_DESC depthDesc = {};
		depthDesc.DepthEnable = FALSE;
		depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		depthDesc.StencilEnable = FALSE;
		g_pd3dDevice11->CreateDepthStencilState(&depthDesc, &g_tonemapDepth11);
	}
}

void CleanupHDRResourcesDX11()
{
	if (g_hdrSRV11) { g_hdrSRV11->Release(); g_hdrSRV11 = nullptr; }
	if (g_hdrRTV11) { g_hdrRTV11->Release(); g_hdrRTV11 = nullptr; }
	if (g_hdrTexture11) { g_hdrTexture11->Release(); g_hdrTexture11 = nullptr; }
	if (g_hdrEmissiveSRV11) { g_hdrEmissiveSRV11->Release(); g_hdrEmissiveSRV11 = nullptr; }
	if (g_hdrEmissiveRTV11) { g_hdrEmissiveRTV11->Release(); g_hdrEmissiveRTV11 = nullptr; }
	if (g_hdrEmissiveTexture11) { g_hdrEmissiveTexture11->Release(); g_hdrEmissiveTexture11 = nullptr; }
	CleanupBloomResourcesDX11();
}

void RenderTonemapDX11()
{
	if (!g_hdrSRV11 || !g_mainRenderTargetView11 || !g_tonemapVS11 || !g_tonemapPS11)
		return;

	ID3D11Texture2D *backBuffer = nullptr;
	g_pSwapChain11->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (!backBuffer)
		return;
	D3D11_TEXTURE2D_DESC bbDesc = {};
	backBuffer->GetDesc(&bbDesc);
	backBuffer->Release();

	D3D11_VIEWPORT vp = {};
	vp.Width = (float)bbDesc.Width;
	vp.Height = (float)bbDesc.Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	g_pd3dDeviceContext11->RSSetViewports(1, &vp);

	ID3D11RenderTargetView *rtv = g_mainRenderTargetView11;
	g_pd3dDeviceContext11->OMSetRenderTargets(1, &rtv, nullptr);
	g_pd3dDeviceContext11->RSSetState(g_tonemapRasterizer11);
	g_pd3dDeviceContext11->OMSetBlendState(g_tonemapBlend11, nullptr, 0xffffffff);
	g_pd3dDeviceContext11->OMSetDepthStencilState(g_tonemapDepth11, 0);
	g_pd3dDeviceContext11->IASetInputLayout(nullptr);
	g_pd3dDeviceContext11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pd3dDeviceContext11->VSSetShader(g_tonemapVS11, nullptr, 0);
	g_pd3dDeviceContext11->PSSetShader(g_tonemapPS11, nullptr, 0);
	g_pd3dDeviceContext11->PSSetSamplers(0, 1, &g_tonemapSampler11);
	ID3D11ShaderResourceView *base_srv = g_hdrSRV11;
#ifdef _DEBUG
	if (g_debugShowEmissive)
		base_srv = g_hdrEmissiveSRV11;
#endif
	ID3D11ShaderResourceView *srvs[2] = {base_srv, g_bloomBlurSRV11[0]};
	g_pd3dDeviceContext11->PSSetShaderResources(0, 2, srvs);
	struct TonemapParams
	{
		float Exposure;
		float BloomIntensity;
		float pad[2];
	};
	TonemapParams params = {};
	params.Exposure = 1.0f;
	params.BloomIntensity = g_bloomIntensity;
#ifdef _DEBUG
	if (g_debugShowEmissive)
		params.BloomIntensity = 0.0f;
#endif
	ID3D11Buffer *cb = nullptr;
	D3D11_BUFFER_DESC cbDesc = {};
	cbDesc.ByteWidth = sizeof(TonemapParams);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (SUCCEEDED(g_pd3dDevice11->CreateBuffer(&cbDesc, nullptr, &cb)))
	{
		D3D11_MAPPED_SUBRESOURCE mapped = {};
		if (SUCCEEDED(g_pd3dDeviceContext11->Map(cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
		{
			memcpy(mapped.pData, &params, sizeof(params));
			g_pd3dDeviceContext11->Unmap(cb, 0);
		}
		g_pd3dDeviceContext11->PSSetConstantBuffers(0, 1, &cb);
	}
	g_pd3dDeviceContext11->Draw(3, 0);

	if (cb)
		cb->Release();
	ID3D11ShaderResourceView *null_srvs[2] = {};
	g_pd3dDeviceContext11->PSSetShaderResources(0, 2, null_srvs);
}

void CreateBloomResourcesDX11(UINT width, UINT height)
{
	CleanupBloomResourcesDX11();
	if (!g_pd3dDevice11)
		return;

	UINT base_w = (width / 2) > 0 ? (width / 2) : 1;
	UINT base_h = (height / 2) > 0 ? (height / 2) : 1;

	for (int i = 0; i < BLOOM_MIP_COUNT; i++)
	{
		const UINT w = (base_w >> i) > 0 ? (base_w >> i) : 1;
		const UINT h = (base_h >> i) > 0 ? (base_h >> i) : 1;

		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = w;
		desc.Height = h;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		if (SUCCEEDED(g_pd3dDevice11->CreateTexture2D(&desc, nullptr, &g_bloomDownTex11[i])))
		{
			g_pd3dDevice11->CreateRenderTargetView(g_bloomDownTex11[i], nullptr, &g_bloomDownRTV11[i]);
			g_pd3dDevice11->CreateShaderResourceView(g_bloomDownTex11[i], nullptr, &g_bloomDownSRV11[i]);
		}

		if (SUCCEEDED(g_pd3dDevice11->CreateTexture2D(&desc, nullptr, &g_bloomBlurTex11[i])))
		{
			g_pd3dDevice11->CreateRenderTargetView(g_bloomBlurTex11[i], nullptr, &g_bloomBlurRTV11[i]);
			g_pd3dDevice11->CreateShaderResourceView(g_bloomBlurTex11[i], nullptr, &g_bloomBlurSRV11[i]);
		}
	}

	if (!g_bloomVS11)
	{
		static const char *vsSrc =
			"struct VSOut { float4 pos : SV_POSITION; float2 uv : TEXCOORD0; };\
			 VSOut main(uint id : SV_VertexID)\
			 {\
			   float2 pos = float2((id == 2) ? 3.0f : -1.0f, (id == 1) ? 3.0f : -1.0f);\
			   VSOut o; o.pos = float4(pos, 0.0f, 1.0f);\
			   o.uv = float2((pos.x + 1.0f) * 0.5f, (1.0f - pos.y) * 0.5f);\
			   return o;\
			 }";

		ID3DBlob *vsBlob = nullptr;
		if (SUCCEEDED(D3DCompile(vsSrc, strlen(vsSrc), nullptr, nullptr, nullptr, "main", "vs_4_0", 0, 0, &vsBlob, nullptr)))
		{
			g_pd3dDevice11->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &g_bloomVS11);
		}
		if (vsBlob)
			vsBlob->Release();
	}

	if (!g_bloomPrefilterPS11 || !g_bloomDownsamplePS11 || !g_bloomBlurPS11 || !g_bloomUpsamplePS11)
	{
		static const char *prefilterSrc =
			"Texture2D srcTex : register(t0);\
			 SamplerState samp0 : register(s0);\
			 cbuffer BloomParams : register(b0)\
			 {\
			   float2 TexelSize;\
			   float Threshold;\
			   float Knee;\
			   float Intensity;\
			   float3 pad;\
			 };\
			 float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_Target\
			 {\
			   float3 color = srcTex.Sample(samp0, uv).rgb;\
			   float brightness = max(max(color.r, color.g), color.b);\
			   float knee = max(Knee, 1e-5);\
			   float soft = saturate((brightness - Threshold + knee) / (2.0 * knee));\
			   float contrib = max(brightness - Threshold, 0.0) + soft * soft * knee;\
			   float scale = contrib / max(brightness, 1e-5);\
			   return float4(color * scale, 1.0);\
			 }";
		static const char *downsampleSrc =
			"Texture2D srcTex : register(t0);\
			 SamplerState samp0 : register(s0);\
			 cbuffer BloomParams : register(b0)\
			 {\
			   float2 TexelSize;\
			   float Threshold;\
			   float Knee;\
			   float Intensity;\
			   float3 pad;\
			 };\
			 float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_Target\
			 {\
			   float2 o = TexelSize * 0.5;\
			   float3 sum = srcTex.Sample(samp0, uv + float2(-o.x, -o.y)).rgb;\
			   sum += srcTex.Sample(samp0, uv + float2( o.x, -o.y)).rgb;\
			   sum += srcTex.Sample(samp0, uv + float2(-o.x,  o.y)).rgb;\
			   sum += srcTex.Sample(samp0, uv + float2( o.x,  o.y)).rgb;\
			   return float4(sum * 0.25, 1.0);\
			 }";
		static const char *blurSrc =
			"Texture2D srcTex : register(t0);\
			 SamplerState samp0 : register(s0);\
			 cbuffer BloomParams : register(b0)\
			 {\
			   float2 TexelSize;\
			   float Threshold;\
			   float Knee;\
			   float Intensity;\
			   float3 pad;\
			 };\
			 float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_Target\
			 {\
			   float2 t = TexelSize;\
			   float3 c = srcTex.Sample(samp0, uv).rgb * 0.4;\
			   c += srcTex.Sample(samp0, uv + float2( t.x, 0)).rgb * 0.1;\
			   c += srcTex.Sample(samp0, uv + float2(-t.x, 0)).rgb * 0.1;\
			   c += srcTex.Sample(samp0, uv + float2(0,  t.y)).rgb * 0.1;\
			   c += srcTex.Sample(samp0, uv + float2(0, -t.y)).rgb * 0.1;\
			   c += srcTex.Sample(samp0, uv + float2( t.x,  t.y)).rgb * 0.05;\
			   c += srcTex.Sample(samp0, uv + float2(-t.x,  t.y)).rgb * 0.05;\
			   c += srcTex.Sample(samp0, uv + float2( t.x, -t.y)).rgb * 0.05;\
			   c += srcTex.Sample(samp0, uv + float2(-t.x, -t.y)).rgb * 0.05;\
			   return float4(c, 1.0);\
			 }";
		static const char *upsampleSrc =
			"Texture2D srcTex : register(t0);\
			 SamplerState samp0 : register(s0);\
			 cbuffer BloomParams : register(b0)\
			 {\
			   float2 TexelSize;\
			   float Threshold;\
			   float Knee;\
			   float Intensity;\
			   float3 pad;\
			 };\
			 float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_Target\
			 {\
			   float3 c = srcTex.Sample(samp0, uv).rgb * Intensity;\
			   return float4(c, 1.0);\
			 }";

		auto compile_ps = [&](const char *src, ID3D11PixelShader **out_ps)
		{
			ID3DBlob *psBlob = nullptr;
			if (SUCCEEDED(D3DCompile(src, strlen(src), nullptr, nullptr, nullptr, "main", "ps_4_0", 0, 0, &psBlob, nullptr)))
			{
				g_pd3dDevice11->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, out_ps);
			}
			if (psBlob)
				psBlob->Release();
		};

		compile_ps(prefilterSrc, &g_bloomPrefilterPS11);
		compile_ps(downsampleSrc, &g_bloomDownsamplePS11);
		compile_ps(blurSrc, &g_bloomBlurPS11);
		compile_ps(upsampleSrc, &g_bloomUpsamplePS11);
	}

	if (!g_bloomSampler11)
	{
		D3D11_SAMPLER_DESC sampDesc = {};
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		g_pd3dDevice11->CreateSamplerState(&sampDesc, &g_bloomSampler11);
	}

	if (!g_bloomAdditiveBlend11)
	{
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		g_pd3dDevice11->CreateBlendState(&blendDesc, &g_bloomAdditiveBlend11);
	}

	if (!g_bloomConstants11)
	{
		D3D11_BUFFER_DESC cbDesc = {};
		cbDesc.ByteWidth = 32;
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		g_pd3dDevice11->CreateBuffer(&cbDesc, nullptr, &g_bloomConstants11);
	}
}

void CleanupBloomResourcesDX11()
{
	for (int i = 0; i < BLOOM_MIP_COUNT; i++)
	{
		if (g_bloomDownSRV11[i]) { g_bloomDownSRV11[i]->Release(); g_bloomDownSRV11[i] = nullptr; }
		if (g_bloomDownRTV11[i]) { g_bloomDownRTV11[i]->Release(); g_bloomDownRTV11[i] = nullptr; }
		if (g_bloomDownTex11[i]) { g_bloomDownTex11[i]->Release(); g_bloomDownTex11[i] = nullptr; }
		if (g_bloomBlurSRV11[i]) { g_bloomBlurSRV11[i]->Release(); g_bloomBlurSRV11[i] = nullptr; }
		if (g_bloomBlurRTV11[i]) { g_bloomBlurRTV11[i]->Release(); g_bloomBlurRTV11[i] = nullptr; }
		if (g_bloomBlurTex11[i]) { g_bloomBlurTex11[i]->Release(); g_bloomBlurTex11[i] = nullptr; }
	}

	if (g_bloomVS11) { g_bloomVS11->Release(); g_bloomVS11 = nullptr; }
	if (g_bloomPrefilterPS11) { g_bloomPrefilterPS11->Release(); g_bloomPrefilterPS11 = nullptr; }
	if (g_bloomDownsamplePS11) { g_bloomDownsamplePS11->Release(); g_bloomDownsamplePS11 = nullptr; }
	if (g_bloomBlurPS11) { g_bloomBlurPS11->Release(); g_bloomBlurPS11 = nullptr; }
	if (g_bloomUpsamplePS11) { g_bloomUpsamplePS11->Release(); g_bloomUpsamplePS11 = nullptr; }
	if (g_bloomSampler11) { g_bloomSampler11->Release(); g_bloomSampler11 = nullptr; }
	if (g_bloomAdditiveBlend11) { g_bloomAdditiveBlend11->Release(); g_bloomAdditiveBlend11 = nullptr; }
	if (g_bloomConstants11) { g_bloomConstants11->Release(); g_bloomConstants11 = nullptr; }
}

void RenderBloomDX11()
{
	if (!g_hdrEmissiveSRV11 || !g_bloomPrefilterPS11 || !g_bloomDownRTV11[0] || !g_bloomBlurSRV11[0])
		return;

	struct BloomParams
	{
		float texelSize[2];
		float threshold;
		float knee;
		float intensity;
		float pad[3];
	};

	auto set_constants = [&](UINT w, UINT h, float threshold, float knee, float intensity)
	{
		if (!g_bloomConstants11)
			return;
		D3D11_MAPPED_SUBRESOURCE mapped = {};
		if (SUCCEEDED(g_pd3dDeviceContext11->Map(g_bloomConstants11, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
		{
			BloomParams params = {};
			params.texelSize[0] = (w > 0) ? (1.0f / (float)w) : 0.0f;
			params.texelSize[1] = (h > 0) ? (1.0f / (float)h) : 0.0f;
			params.threshold = threshold;
			params.knee = knee;
			params.intensity = intensity;
			memcpy(mapped.pData, &params, sizeof(params));
			g_pd3dDeviceContext11->Unmap(g_bloomConstants11, 0);
		}
		g_pd3dDeviceContext11->PSSetConstantBuffers(0, 1, &g_bloomConstants11);
	};

	g_pd3dDeviceContext11->RSSetState(g_tonemapRasterizer11);
	g_pd3dDeviceContext11->OMSetDepthStencilState(g_tonemapDepth11, 0);
	g_pd3dDeviceContext11->IASetInputLayout(nullptr);
	g_pd3dDeviceContext11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pd3dDeviceContext11->VSSetShader(g_bloomVS11, nullptr, 0);
	g_pd3dDeviceContext11->PSSetSamplers(0, 1, &g_bloomSampler11);

	// Prefilter to mip 0
	{
		D3D11_TEXTURE2D_DESC desc = {};
		g_bloomDownTex11[0]->GetDesc(&desc);
		D3D11_VIEWPORT vp = {0.0f, 0.0f, (float)desc.Width, (float)desc.Height, 0.0f, 1.0f};
		g_pd3dDeviceContext11->RSSetViewports(1, &vp);
		g_pd3dDeviceContext11->OMSetRenderTargets(1, &g_bloomDownRTV11[0], nullptr);
		const float clear[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		g_pd3dDeviceContext11->ClearRenderTargetView(g_bloomDownRTV11[0], clear);
		g_pd3dDeviceContext11->PSSetShader(g_bloomPrefilterPS11, nullptr, 0);
		set_constants(desc.Width, desc.Height, g_bloomThreshold, g_bloomKnee, 1.0f);
		g_pd3dDeviceContext11->PSSetShaderResources(0, 1, &g_hdrEmissiveSRV11);
		g_pd3dDeviceContext11->OMSetBlendState(nullptr, nullptr, 0xffffffff);
		g_pd3dDeviceContext11->Draw(3, 0);
	}

	// Downsample chain
	for (int i = 1; i < BLOOM_MIP_COUNT; i++)
	{
		D3D11_TEXTURE2D_DESC desc = {};
		g_bloomDownTex11[i]->GetDesc(&desc);
		D3D11_VIEWPORT vp = {0.0f, 0.0f, (float)desc.Width, (float)desc.Height, 0.0f, 1.0f};
		g_pd3dDeviceContext11->RSSetViewports(1, &vp);
		g_pd3dDeviceContext11->OMSetRenderTargets(1, &g_bloomDownRTV11[i], nullptr);
		const float clear[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		g_pd3dDeviceContext11->ClearRenderTargetView(g_bloomDownRTV11[i], clear);
		g_pd3dDeviceContext11->PSSetShader(g_bloomDownsamplePS11, nullptr, 0);
		set_constants(desc.Width, desc.Height, g_bloomThreshold, g_bloomKnee, 1.0f);
		g_pd3dDeviceContext11->PSSetShaderResources(0, 1, &g_bloomDownSRV11[i - 1]);
		g_pd3dDeviceContext11->OMSetBlendState(nullptr, nullptr, 0xffffffff);
		g_pd3dDeviceContext11->Draw(3, 0);
	}

	// Blur each mip
	for (int i = 0; i < BLOOM_MIP_COUNT; i++)
	{
		D3D11_TEXTURE2D_DESC desc = {};
		g_bloomBlurTex11[i]->GetDesc(&desc);
		D3D11_VIEWPORT vp = {0.0f, 0.0f, (float)desc.Width, (float)desc.Height, 0.0f, 1.0f};
		g_pd3dDeviceContext11->RSSetViewports(1, &vp);
		g_pd3dDeviceContext11->OMSetRenderTargets(1, &g_bloomBlurRTV11[i], nullptr);
		const float clear[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		g_pd3dDeviceContext11->ClearRenderTargetView(g_bloomBlurRTV11[i], clear);
		g_pd3dDeviceContext11->PSSetShader(g_bloomBlurPS11, nullptr, 0);
		set_constants(desc.Width, desc.Height, g_bloomThreshold, g_bloomKnee, 1.0f);
		g_pd3dDeviceContext11->PSSetShaderResources(0, 1, &g_bloomDownSRV11[i]);
		g_pd3dDeviceContext11->OMSetBlendState(nullptr, nullptr, 0xffffffff);
		g_pd3dDeviceContext11->Draw(3, 0);
	}

	// Upsample and combine
	for (int i = BLOOM_MIP_COUNT - 1; i > 0; i--)
	{
		D3D11_TEXTURE2D_DESC desc = {};
		g_bloomBlurTex11[i - 1]->GetDesc(&desc);
		D3D11_VIEWPORT vp = {0.0f, 0.0f, (float)desc.Width, (float)desc.Height, 0.0f, 1.0f};
		g_pd3dDeviceContext11->RSSetViewports(1, &vp);
		g_pd3dDeviceContext11->OMSetRenderTargets(1, &g_bloomBlurRTV11[i - 1], nullptr);
		g_pd3dDeviceContext11->PSSetShader(g_bloomUpsamplePS11, nullptr, 0);
		set_constants(desc.Width, desc.Height, g_bloomThreshold, g_bloomKnee, 1.0f);
		g_pd3dDeviceContext11->PSSetShaderResources(0, 1, &g_bloomBlurSRV11[i]);
		g_pd3dDeviceContext11->OMSetBlendState(g_bloomAdditiveBlend11, nullptr, 0xffffffff);
		g_pd3dDeviceContext11->Draw(3, 0);
	}

	ID3D11ShaderResourceView *null_srv = nullptr;
	g_pd3dDeviceContext11->PSSetShaderResources(0, 1, &null_srv);
	g_pd3dDeviceContext11->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}

void CleanupDeviceD3D11()
{
	CleanupRenderTargetDX11();
	if (g_pSwapChain11)
	{
		g_pSwapChain11->Release();
		g_pSwapChain11 = nullptr;
	}
	if (g_pd3dDeviceContext11)
	{
		g_pd3dDeviceContext11->Release();
		g_pd3dDeviceContext11 = nullptr;
	}
	if (g_pd3dDevice11)
	{
		g_pd3dDevice11->Release();
		g_pd3dDevice11 = nullptr;
	}
	if (g_tonemapVS11) { g_tonemapVS11->Release(); g_tonemapVS11 = nullptr; }
	if (g_tonemapPS11) { g_tonemapPS11->Release(); g_tonemapPS11 = nullptr; }
	if (g_tonemapSampler11) { g_tonemapSampler11->Release(); g_tonemapSampler11 = nullptr; }
	if (g_tonemapBlend11) { g_tonemapBlend11->Release(); g_tonemapBlend11 = nullptr; }
	if (g_tonemapAddBlend11) { g_tonemapAddBlend11->Release(); g_tonemapAddBlend11 = nullptr; }
	if (g_tonemapRasterizer11) { g_tonemapRasterizer11->Release(); g_tonemapRasterizer11 = nullptr; }
	if (g_tonemapDepth11) { g_tonemapDepth11->Release(); g_tonemapDepth11 = nullptr; }
}

void WaitForPendingOperations()
{
	g_pd3dCommandQueue->Signal(g_fence, ++g_fenceLastSignaledValue);

	g_fence->SetEventOnCompletion(g_fenceLastSignaledValue, g_fenceEvent);
	::WaitForSingleObject(g_fenceEvent, INFINITE);
}

FrameContext *WaitForNextFrameContext()
{
	FrameContext *frame_context = &g_frameContext[g_frameIndex % APP_NUM_FRAMES_IN_FLIGHT];
	LOGF("WaitForNextFrameContext frame=%u fenceCompleted=%llu fenceTarget=%llu swapWaitable=0x%p fenceEvent=0x%p",
		g_frameIndex,
		(unsigned long long)g_fence->GetCompletedValue(),
		(unsigned long long)frame_context->FenceValue,
		g_hSwapChainWaitableObject,
		g_fenceEvent);
	if (g_fence->GetCompletedValue() < frame_context->FenceValue)
	{
		g_fence->SetEventOnCompletion(frame_context->FenceValue, g_fenceEvent);
		HANDLE waitableObjects[] = {g_hSwapChainWaitableObject, g_fenceEvent};
		DWORD wait_result = ::WaitForMultipleObjects(2, waitableObjects, TRUE, INFINITE);
		LOGF("WaitForMultipleObjects result=0x%08X lastError=0x%08X", (unsigned int)wait_result, (unsigned int)GetLastError());
	}
	else
	{
		DWORD wait_result = ::WaitForSingleObject(g_hSwapChainWaitableObject, INFINITE);
		LOGF("WaitForSingleObject result=0x%08X lastError=0x%08X", (unsigned int)wait_result, (unsigned int)GetLastError());
	}

	return frame_context;
}

// Forward declare message handler from HvkGui_impl_win32.cpp
extern HvkGui_IMPL_API LRESULT HvkGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear HvkGui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear HvkGui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (HvkGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != nullptr && wParam != SIZE_MINIMIZED)
		{
			CleanupRenderTarget();
			DXGI_SWAP_CHAIN_DESC1 desc = {};
			g_pSwapChain->GetDesc1(&desc);
			HRESULT result = g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), desc.Format, desc.Flags);
			Hvk_ASSERT(SUCCEEDED(result) && "Failed to resize swapchain.");
			CreateRenderTarget();
		}
		if (g_pd3dDevice11 != nullptr && wParam != SIZE_MINIMIZED)
		{
			if (g_pd3dDeviceContext11)
				g_pd3dDeviceContext11->OMSetRenderTargets(0, nullptr, nullptr);
			CleanupRenderTargetDX11();
			HRESULT result = g_pSwapChain11->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			Hvk_ASSERT(SUCCEEDED(result) && "Failed to resize DX11 swapchain.");
			CreateRenderTargetDX11();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
