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
}

void JEngine2::Application::Run()
{
	D3D12DeviceConfig config;
	config.bEnableDebugLayer = true;
	config.windowHandle = mWindow->GetPlatformWindowHandle();
	auto[x, y] = mWindow->GetWindowRect();
	config.windowWidth = x;
	config.windowHeight = y;
	
	mRenderDevice = std::make_unique<D3D12Device>(config);
	mWindow->Show();
}

void JEngine2::Application::Tick()
{
	static uint32_t tickCount = 0;
	JE_LOG_INFO("Application Tick {0}", tickCount++);
	mRenderDevice->Render();
}

JEngine2::Application::~Application()
{
	mRenderDevice.reset();
	mWindow.reset();
}
