#pragma once

#include "D3D12Include.h"
#include "Platform/Platform.h"

#include "D3D12SwapChain.h"

namespace JEngine2 {
	struct D3D12DeviceConfig {
		bool bEnableDebugLayer;
		PlatformWindowHandle windowHandle;
		uint32_t windowWidth;
		uint32_t windowHeight;
	};

	class D3D12Infrastructure {
	public:
		static void EnableDebugLayer();
		static ComPtr<IDXGIAdapter4> GetAdaptor(bool useWarp = false);
		static ComPtr<ID3D12Device2> CreateDevice(ComPtr<IDXGIAdapter4>& adaptor);
		static ComPtr<ID3D12CommandQueue> CreateCommandQueue(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type);
		static ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(ComPtr<ID3D12Device2> device,
			D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors);
		static ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type);
		static ComPtr<ID3D12GraphicsCommandList> CreateCommandList(ComPtr<ID3D12Device2> device,
			ComPtr<ID3D12CommandAllocator> commandAllocator, D3D12_COMMAND_LIST_TYPE type);
		static ComPtr<ID3D12Fence> CreateFence(ComPtr<ID3D12Device2> device);
		static HANDLE CreateEventHandle();
		static uint64_t Signal(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence,
			uint64_t& fenceValue);
		static void WaitForFenceValue(ComPtr<ID3D12Fence> fence, uint64_t fenceValue, HANDLE fenceEvent,
			std::chrono::milliseconds duration = std::chrono::milliseconds::max());
		static void Flush(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence,
				uint64_t& fenceValue, HANDLE fenceEvent);
		static void Update();
	};

	class D3D12Device {
	public:
		D3D12Device() = delete;
		D3D12Device(const D3D12Device&) = delete;
		~D3D12Device();

		D3D12Device(const D3D12DeviceConfig& config);
		void Render();
		void BeginFrame();
		void EndFrame();
	private:
		std::unique_ptr<D3D12SwapChain> mSwapChain;
	private:
		static const uint32_t mNumFrames = 3;
		static const bool mUseWarp = false;

		// DirectX 12 Objects
		ComPtr<IDXGIAdapter4> mAdaptor;
		ComPtr<ID3D12Device2> mDevice;
		ComPtr<ID3D12CommandQueue> mCommandQueue;
		ComPtr<ID3D12GraphicsCommandList> mCommandList;
		ComPtr<ID3D12CommandAllocator> mCommandAllocators[mNumFrames];

		ComPtr<ID3D12Fence> mFence;
		HANDLE mEventHandle;
		UINT mCurrentBackBufferIndex;

		uint64_t mFrameFenceValues[mNumFrames];
		uint64_t mFenceValue;

	};
}