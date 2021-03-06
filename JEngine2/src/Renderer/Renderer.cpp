#include "pch.h"
#include "Renderer.h"

namespace JEngine2 {
	
	Renderer::Renderer(const std::shared_ptr<Window> window)
	{
		RHIDeviceConfig config;
		config.bEnableDebugLayer = true;
		config.windowHandle = window->GetPlatformWindowHandle();
		auto [x, y] = window->GetWindowRect();
		config.windowWidth = x;
		config.windowHeight = y;

		mDevice = std::make_unique<D3D12Device>(config);
	}

	void Renderer::Render()
	{
		mDevice->BeginFrame();
		auto commandList = mDevice->GetCommandList(RHICommandListType::Direct);
		commandList->BeginFrame(mDevice->GetCurrentSwapChainRT());
		RHIRenderPassDesc renderPassDesc;
		renderPassDesc.rt = mDevice->GetCurrentSwapChainRT();
		renderPassDesc.color = RHIColor(0.3f, 0.5f, 0.3f);
		commandList->BeginPass(renderPassDesc);
		commandList->EndPass();
		commandList->EndFrame();
		mDevice->ExecuteCommandList(commandList);
		mDevice->EndFrame();
	}

}