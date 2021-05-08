#include "pch.h"
#include "Application.h"

JEngine2::Application::Application(PlatformInstanceHandle instance) : mInstance(instance)
{
	JE_LOG_INFO("Hello JEngine 2 !");
	WindowConfig config{};
	config.Height = 540;
	config.Width = 720;
	config.Instance = mInstance;
	config.WindowName = L"JEngine2";
	config.UpdateFunc = std::bind(&Application::Tick, this);

	mWindow.reset(CreatePlatformWindow(config));
	mRenderer = std::make_unique<Renderer>(mWindow);
}

void JEngine2::Application::Run()
{
	mWindow->Show();
}

void JEngine2::Application::Tick()
{
	static uint32_t tickCount = 0;
	JE_LOG_INFO("Application Tick {0}", tickCount++);
	mRenderer->Render();
}

JEngine2::Application::~Application()
{
	mRenderer.reset();
	mWindow.reset();
}
