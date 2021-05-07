#pragma once
#include "D3D12Include.h"
#include "core/facilities.h"

namespace JEngine2 {
	class D3D12SwapChain {
	public:
		D3D12SwapChain() = delete;
		D3D12SwapChain(const D3D12SwapChain&) = delete;
		D3D12SwapChain(HWND hwnd, ComPtr<ID3D12Device2> device, ComPtr<ID3D12CommandQueue> commandQueue,
			uint32_t width, uint32_t height, uint32_t bufferCount);
		FORCEINLINE uint32_t GetCurrentBackBufferIndex() const { return mDXGISwapChain->GetCurrentBackBufferIndex(); }
		void Present() const;
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetCurrentRTV() const;
		
	public: // For Debug
		FORCEINLINE ComPtr<ID3D12Resource> GetCurrentBackBuffer() const { return mBackBuffers[GetCurrentBackBufferIndex()]; }
	private:
		bool CheckTearingSupport() const;
		void CreateDXGISwapChain();
		void CreateRTVDescriptorHeap();
		void CreateRTVs();
	
	private:
		static const bool mVSync = false;
	private:
		uint32_t mBufferCount;
		uint32_t mWidth;
		uint32_t mHeight;
		HWND mHwnd;
		ComPtr<ID3D12CommandQueue> mCommandQueue;
		ComPtr<ID3D12Device2> mDevice;

	private:
		ComPtr<IDXGISwapChain4> mDXGISwapChain;
		ComPtr<ID3D12DescriptorHeap> mRTVDescriptorHeap;
		std::vector<ComPtr<ID3D12Resource>> mBackBuffers;
		UINT mRTVDescriptorSize;
	};
}