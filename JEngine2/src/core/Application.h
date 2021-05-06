#pragma once
#include "Platform/Platform.h"
#include "Renderer/RHI/D3D12/D3D12Device.h"

namespace JEngine2 {
	class Application {
	public:
		Application(PlatformInstanceHandle instance);
		virtual void Run();
		virtual void Tick();
		~Application();
	private:
		PlatformInstanceHandle mInstance;
		std::unique_ptr<Window> mWindow;
		std::unique_ptr<D3D12Device> mRenderDevice;

	};
}