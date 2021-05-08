#pragma once
#include "Platform/Platform.h"
#include "Renderer/Renderer.h"

namespace JEngine2 {
	class Application {
	public:
		Application(PlatformInstanceHandle instance);
		virtual void Run();
		virtual void Tick();
		~Application();
	private:
		PlatformInstanceHandle mInstance;
		std::shared_ptr<Window> mWindow;
		std::unique_ptr<Renderer> mRenderer;
	};
}