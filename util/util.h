#pragma once

#include <directx/d3dx12.h>
#include <dxgi1_4.h>
#pragma comment(lib, "dxgi.lib")

inline bool SupportsDX12()
{
	IDXGIFactory4* factory = nullptr;
	if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
		return false;

	IDXGIAdapter1* adapter = nullptr;
	for (UINT i = 0;
		factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		DXGI_ADAPTER_DESC1 desc{};
		adapter->GetDesc1(&desc);

		// skip software adapters
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			adapter->Release();
			continue;
		}

		// test DX12 device creation
		HRESULT hr = D3D12CreateDevice(
			adapter,
			D3D_FEATURE_LEVEL_11_0,
			__uuidof(ID3D12Device),
			nullptr);

		adapter->Release();

		if (SUCCEEDED(hr))
		{
			factory->Release();
			return true;
		}
	}

	factory->Release();
	return false;
}


enum class RenderBackend
{
	DX12,
	DX11
};
