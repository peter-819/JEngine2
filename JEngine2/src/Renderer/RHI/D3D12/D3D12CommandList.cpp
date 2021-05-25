#include "pch.h"
#include "D3D12CommandList.h"
#include "D3D12Resource.h"

namespace JEngine2 {

	D3D12CommandList::D3D12CommandList(const ComPtr<ID3D12GraphicsCommandList2>& commandList,
		RHICommandListType type) : mCommandList(commandList), RHICommandList(type)
	{

	}

	void D3D12CommandList::BeginPass(const RHIRenderPassDesc& desc)
	{
		const float* clearColor = reinterpret_cast<const float*>(&desc.color);
		auto rtv = dynamic_cast<const D3D12RenderTarget&>(*desc.rt).handle;
		mCommandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
	}

	void D3D12CommandList::EndPass()
	{
	}

	void D3D12CommandList::BeginFrame(const RHIRenderTargetPtr& rt)
	{
		mCurrentRT = rt;
		auto backBuffer = dynamic_cast<const D3D12RenderTarget&>(*rt).resource;
		CD3DX12_RESOURCE_BARRIER PresentToRenderTargetBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			backBuffer.Get(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET);
		mCommandList->ResourceBarrier(1, &PresentToRenderTargetBarrier);

	}

	void D3D12CommandList::EndFrame()
	{
		auto backBuffer = dynamic_cast<D3D12RenderTarget&>(*mCurrentRT).resource;
		CD3DX12_RESOURCE_BARRIER RenderTargetToPresentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			backBuffer.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT);
		mCommandList->ResourceBarrier(1, &RenderTargetToPresentBarrier);
	}

}