#include "pch.h"
#include "D3D12Device.h"

namespace JEngine2 {
	
	D3D12Device::D3D12Device(const D3D12DeviceConfig& config)
	{
		if (config.bEnableDebugLayer) {
			D3D12Infrastructure::EnableDebugLayer();
		}
		mTearingSupport = D3D12Infrastructure::CheckTearingSupport();
		mAdaptor = D3D12Infrastructure::GetAdaptor();
		mDevice = D3D12Infrastructure::CreateDevice(mAdaptor);
		mCommandQueue = D3D12Infrastructure::CreateCommandQueue(mDevice, D3D12_COMMAND_LIST_TYPE_DIRECT);
		mSwapChain = D3D12Infrastructure::CreateSwapChain(config.windowHandle, mCommandQueue, config.windowWidth, config.windowHeight, mNumFrames);
		mCurrentBackBufferIndex = mSwapChain->GetCurrentBackBufferIndex();

		mRTVDescriptorHeap = D3D12Infrastructure::CreateDescriptorHeap(mDevice, D3D12_DESCRIPTOR_HEAP_TYPE_RTV,mNumFrames);
		mRTVDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		D3D12Infrastructure::CreateSwapChainRTVs(mDevice, mSwapChain, mRTVDescriptorHeap, mNumFrames, mBackBuffers);

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

	void D3D12Device::Render()
	{
		auto commandAllocator = mCommandAllocators[mCurrentBackBufferIndex];
		auto backBuffer = mBackBuffers[mCurrentBackBufferIndex];
		commandAllocator->Reset();
		mCommandList->Reset(commandAllocator.Get(), nullptr);
		
		CD3DX12_RESOURCE_BARRIER PresentToRenderTargetBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			backBuffer.Get(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET);
		mCommandList->ResourceBarrier(1, &PresentToRenderTargetBarrier);
		
		float clearColor[] = { 0.4f,0.6f,0.9f,1.0f };
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(mRTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			mCurrentBackBufferIndex, mRTVDescriptorSize);
		mCommandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);

		CD3DX12_RESOURCE_BARRIER RenderTargetToPresentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			backBuffer.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT);
		mCommandList->ResourceBarrier(1, &RenderTargetToPresentBarrier);
		DXCALL(mCommandList->Close());
		ID3D12CommandList* const commandLists[] = { mCommandList.Get() };
		mCommandQueue->ExecuteCommandLists(1,commandLists);
		
		mFrameFenceValues[mCurrentBackBufferIndex] = D3D12Infrastructure::Signal(mCommandQueue, mFence, mFenceValue);
		
		
		UINT presentFlags = mTearingSupport && !mVSync ? DXGI_PRESENT_ALLOW_TEARING : 0;
		DXCALL(mSwapChain->Present(static_cast<int>(mVSync), presentFlags));
		
		mCurrentBackBufferIndex = mSwapChain->GetCurrentBackBufferIndex();
		D3D12Infrastructure::WaitForFenceValue(mFence, mFrameFenceValues[mCurrentBackBufferIndex], mEventHandle);
	}

}
