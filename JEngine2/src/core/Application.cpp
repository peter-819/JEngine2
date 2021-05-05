#include "pch.h"
#include "Application.h"

#include "Platform/Platform.h"

JEngine2::Application::Application(PlatformInstanceHandle instance) : mInstance(instance)
{

}

void JEngine2::Application::Run()
{
	JE_LOG_INFO("Hello JEngine 2 !");
	WindowConfig config{};
	config.Height = 540;
	config.Width = 720;
	config.Instance = mInstance;
	config.WindowName = L"JEngine2";
	
	auto window = CreatePlatformWindow(config);
	window->Show();

	while (1) {

	};
}
