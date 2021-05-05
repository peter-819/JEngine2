#pragma once
#include "Platform/PlatformMarcos.h"
#include <string>

namespace JEngine2 {
	struct WindowConfig {
		int Height;
		int Width;
		PlatformInstanceHandle Instance;
		std::wstring WindowName;
	};

	class Window {
	public:
		Window(const WindowConfig& config);
		virtual void Show() const = 0;
	protected:
		int mWidth;
		int mHeight;
		WindowConfig mConfig;
	};
}