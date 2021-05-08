#pragma once

#include "Platform/Platform.h"

namespace JEngine2 {

	struct RHIDeviceConfig {
		bool bEnableDebugLayer;
		PlatformWindowHandle windowHandle;
		uint32_t windowWidth;
		uint32_t windowHeight;
	};

	class RHIDevice {
	public:
		RHIDevice() = default;
		RHIDevice(const RHIDevice&) = delete;
		virtual ~RHIDevice(){}
		
	public:
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f) = 0;
	};
}