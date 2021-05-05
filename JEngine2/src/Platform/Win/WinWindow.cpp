#include "pch.h"
#include "WinWindow.h"

namespace {
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

namespace JEngine2 {

	WinWindow::WinWindow(const WindowConfig& config)
		:Window(config),mInstance(config.Instance)
	{
		PrivateRegisterClass();
		PrivateCreateWindow();
	}

	void WinWindow::Show() const
	{
		ShowWindow(mHwnd, 1);
	}

	void WinWindow::PrivateRegisterClass()
	{
		// Register a window class for creating our render window with.
		WNDCLASSEXW windowClass = {};

		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = &::WindowProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = mInstance;
		windowClass.hIcon = ::LoadIcon(mInstance, NULL);
		windowClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = mConfig.WindowName.c_str();
		windowClass.hIconSm = ::LoadIcon(mInstance, NULL);

		static ATOM atom = ::RegisterClassExW(&windowClass);
		Assert(atom > 0);
	}

	void WinWindow::PrivateCreateWindow()
	{
		int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

		RECT windowRect = { 0, 0, static_cast<LONG>(mWidth), static_cast<LONG>(mHeight) };
		::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		int windowWidth = windowRect.right - windowRect.left;
		int windowHeight = windowRect.bottom - windowRect.top;

		// Center the window within the screen. Clamp to 0, 0 for the top-left corner.
		int windowX = std::max<int>(0, (screenWidth - windowWidth) / 2);
		int windowY = std::max<int>(0, (screenHeight - windowHeight) / 2);

		mHwnd = ::CreateWindowExW(
			NULL,
			mConfig.WindowName.c_str(),
			mConfig.WindowName.c_str(),
			WS_OVERLAPPEDWINDOW,
			windowX,
			windowY,
			windowWidth,
			windowHeight,
			NULL,
			NULL,
			mInstance,
			nullptr
		);

		Assert(mHwnd && "Failed to create window");
	}

}