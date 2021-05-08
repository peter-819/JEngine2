#pragma once

#include "Renderer/RHI/D3D12/D3D12Device.h"
#include "Platform/Window.h"

namespace JEngine2 {
	class Renderer {
	public:
		Renderer(const Renderer&) = delete;
		Renderer(const std::shared_ptr<Window> window);
		void Render();

	private:
		std::unique_ptr<RHIDevice> mDevice;
	};
}