#include "PlatformMarcos.h"
#include "Window.h"
#include <memory>

#if _WIN32
#include "Win/WinWindow.h"
#else
static_assert(false, "Only Support Windows !!!");
#endif

namespace JEngine2 {
	std::shared_ptr<Window> CreatePlatformWindow(const WindowConfig& config) {
		if constexpr (_WIN32) {
			return std::make_shared<WinWindow>(config);
		}
		else {
			Assert(false&&"Only Support Windows !!!");
			return nullptr;
		}
	}
}
