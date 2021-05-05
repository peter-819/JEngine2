#pragma once

#include "D3D12Include.h"

namespace JEngine2 {
	struct D3D12DeviceConfig {
		bool bEnableDebugLayer;
	};


	class D3D12Device {
	public:
		D3D12Device(const D3D12DeviceConfig& config);
	private:
		void EnableDebugLayer();
	};
}