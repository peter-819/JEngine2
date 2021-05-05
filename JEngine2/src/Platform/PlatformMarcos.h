#if _WIN32
	#include "Windows.h"
	using PlatformWindowHandle = HWND;
	using PlatformInstanceHandle = HINSTANCE;
#else
	using PlatformWindowHandle = int;
	using PlatformInstanceHandle = int;
	static_assert(false, "Only support Windows !!!");
#endif