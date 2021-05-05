#include "pch.h"
#include "Window.h"

namespace JEngine2 {

	Window::Window(const WindowConfig& config) 
		:mConfig(config),mWidth(config.Width),mHeight(config.Height)
	{
	}

};