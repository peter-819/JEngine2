#pragma once

#include <Windows.h>
#include "Platform/Window.h"

namespace JEngine2 {
	class WinWindow : public Window {
	public:
		WinWindow(const WindowConfig& config);
		virtual void Show() const override;
	private:
		void PrivateRegisterClass();
		void PrivateCreateWindow();
	private:
		HINSTANCE mInstance;
		HWND mHwnd;
	};
};