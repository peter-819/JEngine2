#include "pch.h"
#include "D3D12Device.h"

namespace JEngine2 {

	D3D12Device::D3D12Device(const D3D12DeviceConfig& config)
	{
		if (config.bEnableDebugLayer) {
			EnableDebugLayer();
		}
	}

	void D3D12Device::EnableDebugLayer()
	{
		if constexpr (_DEBUG) {
			ComPtr<ID3D12Debug> debugInterface;
			DXCALL(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
			debugInterface->EnableDebugLayer();
		}
	}
}