#pragma once

#include "Renderer/RHI/RHICommandList.h"
#include "D3D12Include.h"
#include "D3D12Device.h"

namespace JEngine2 {
	class D3D12CommandList : public RHICommandList {
		friend class D3D12CommandQueue;
	public:
		D3D12CommandList(const ComPtr<ID3D12GraphicsCommandList2>& commandList,RHICommandListType type);
		virtual void BeginPass(const RHIRenderPassDesc& desc) override;
		virtual void EndPass() override;

		virtual void BeginFrame(const RHIRenderTargetPtr& rt) override;
		virtual void EndFrame() override;

	private:
		ComPtr<ID3D12GraphicsCommandList2> mCommandList;
		RHIDevicePtr mDevice;
	};
}