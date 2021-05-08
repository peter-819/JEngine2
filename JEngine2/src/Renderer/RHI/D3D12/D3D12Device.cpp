#include "pch.h"
#include "D3D12Device.h"

namespace JEngine2 {
	
	D3D12Device::D3D12Device(const RHIDeviceConfig& config)
	{
		if (config.bEnableDebugLayer) {
			D3D12Infrastructure::EnableDebugLayer();
		}
		mAdaptor = D3D12Infrastructure::GetAdaptor();
		mDevice = D3D12Infrastructure::CreateDevice(mAdaptor);
		mCommandQueue = D3D12Infrastructure::CreateCommandQueue(mDevice, D3D12_COMMAND_LIST_TYPE_DIRECT);
		
		mSwapChain = std::make_unique<D3D12SwapChain>(config.windowHandle, mDevice, mCommandQueue, 
			config.windowWidth, config.windowHeight, mNumFrames);
		mCurrentBackBufferIndex = mSwapChain->GetCurrentBackBufferIndex();

		for (int i = 0; i < mNumFrames; i++) {
			mCommandAllocators[i] = D3D12Infrastructure::CreateCommandAllocator(mDevice, D3D12_COMMAND_LIST_TYPE_DIRECT);
		}
		mCommandList = D3D12Infrastructure::CreateCommandList(mDevice, mCommandAllocators[mCurrentBackBufferIndex], D3D12_COMMAND_LIST_TYPE_DIRECT);
		
		mFence = D3D12Infrastructure::CreateFence(mDevice);
		mEventHandle = D3D12Infrastructure::CreateEventHandle();
		
		memset(mFrameFenceValues, 0, sizeof mFrameFenceValues);
	}

	D3D12Device::~D3D12Device()
	{
		D3D12Infrastructure::Flush(mCommandQueue, mFence, mFenceValue, mEventHandle);
		::CloseHandle(mEventHandle);
	}

	void D3D12Device::Clear(float r,float g,float b,float a)
	{
		float clearColor[] = { r,g,b,a };
		auto rtv = mSwapChain->GetCurrentRTV();
		mCommandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
	}

	void D3D12Device::BeginFrame()
	{
		auto commandAllocator = mCommandAllocators[mCurrentBackBufferIndex];
		commandAllocator->Reset();
		mCommandList->Reset(commandAllocator.Get(), nullptr);

		auto backBuffer = mSwapChain->GetCurrentBackBuffer();
		CD3DX12_RESOURCE_BARRIER PresentToRenderTargetBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			backBuffer.Get(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET);
		mCommandList->ResourceBarrier(1, &PresentToRenderTargetBarrier);
	}

	void D3D12Device::EndFrame()
	{
		auto backBuffer = mSwapChain->GetCurrentBackBuffer();
		CD3DX12_RESOURCE_BARRIER RenderTargetToPresentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			backBuffer.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT);
		mCommandList->ResourceBarrier(1, &RenderTargetToPresentBarrier);
		DXCALL(mCommandList->Close());
		ID3D12CommandList* const commandLists[] = { mCommandList.Get() };
		mCommandQueue->ExecuteCommandLists(1, commandLists);

		mFrameFenceValues[mCurrentBackBufferIndex] = D3D12Infrastructure::Signal(mCommandQueue, mFence, mFenceValue);

		mSwapChain->Present();

		mCurrentBackBufferIndex = mSwapChain->GetCurrentBackBufferIndex();
		D3D12Infrastructure::WaitForFenceValue(mFence, mFrameFenceValues[mCurrentBackBufferIndex], mEventHandle);
	}

}
