#pragma once
#include "Platform/PlatformMarcos.h"
#include <string>

namespace JEngine2 {
	struct WindowConfig {
		int Height;
		int Width;
		PlatformInstanceHandle Instance;
		std::wstring WindowName;
		std::function<void()> UpdateFunc;
	};

	class Window {
	public:
		Window(const WindowConfig& config);
		virtual void Show() const = 0;
		virtual PlatformWindowHandle GetPlatformWindowHandle() const = 0;
		FORCEINLINE std::pair<uint32_t, uint32_t> GetWindowRect() const { return std::make_pair(mWidth,mHeight); }
	protected:
		int mWidth;
		int mHeight;
		WindowConfig mConfig;
	};
}