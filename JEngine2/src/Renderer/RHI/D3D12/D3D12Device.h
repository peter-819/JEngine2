#pragma once

#include "D3D12Include.h"

#include "D3D12SwapChain.h"
#include "Renderer/RHI/RHIDevice.h"

#include <queue>

namespace JEngine2 {

	class D3D12Infrastructure {
	public:
		static void EnableDebugLayer();
		static ComPtr<IDXGIAdapter4> GetAdaptor(bool useWarp = false);
		static ComPtr<ID3D12Device2> CreateDevice(ComPtr<IDXGIAdapter4>& adaptor);
		static ComPtr<ID3D12CommandQueue> CreateCommandQueue(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type);
		static ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(ComPtr<ID3D12Device2> device,
			D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors);
		static ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type);
		static ComPtr<ID3D12GraphicsCommandList2> CreateCommandList(ComPtr<ID3D12Device2> device,
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

	class D3D12CommandQueue {
	public:
		D3D12CommandQueue(const ComPtr<ID3D12Device2>& device,D3D12_COMMAND_LIST_TYPE type);
		RHICommandListPtr GetCommandList();
		uint64_t ExecuteCommandList(RHICommandListPtr rhiCommandList);

	public:
		ComPtr<ID3D12CommandQueue> GetD3D12CommandQueue() const { return mCommandQueue; }

	private:
		bool IsFenceComplete(uint64_t fenceValue) const;
		void WaitForFenceValue(uint64_t fenceValue);
		uint64_t Signal();

	private:
		ComPtr<ID3D12CommandQueue> mCommandQueue;
		struct AF {
			ComPtr<ID3D12CommandAllocator> allocator;
			uint64_t fenceValue;
		};
		std::queue<AF> mCommandAllocatorQueue;
		std::queue<ComPtr<ID3D12GraphicsCommandList2>> mCommandListQueue;
		ComPtr<ID3D12Device2> mDevice;
		ComPtr<ID3D12Fence> mFence;
		HANDLE mEventHandle;
		uint64_t mFenceValue;

		D3D12_COMMAND_LIST_TYPE mCommandListType;
		RHICommandListType mRHICommandListType;
	};

	class D3D12Device : public RHIDevice {
	public:
		virtual ~D3D12Device() override;
		D3D12Device(const RHIDeviceConfig& config);

		virtual RHICommandListPtr GetCommandList(RHICommandListType type) override;
		virtual void ExecuteCommandList(RHICommandListPtr commandList) override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;
		virtual RHIRenderTargetPtr GetCurrentSwapChainRT() const override;
		
	private:
		std::unique_ptr<D3D12SwapChain> mSwapChain;
		std::unique_ptr<D3D12CommandQueue> mDirectQueue;
		std::unique_ptr<D3D12CommandQueue> mCopyQueue;
	private:
		static const uint32_t mNumFrames = 3;
		static const bool mUseWarp = false;

		// DirectX 12 Objects
		ComPtr<IDXGIAdapter4> mAdaptor;
		ComPtr<ID3D12Device2> mDevice;

		ComPtr<ID3D12Fence> mFence;
		HANDLE mEventHandle;
		UINT mCurrentBackBufferIndex;

		uint64_t mFrameFenceValues[mNumFrames];
		uint64_t mFenceValue;

	};
}