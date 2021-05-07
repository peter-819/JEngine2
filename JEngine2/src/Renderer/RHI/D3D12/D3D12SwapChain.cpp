#include "pch.h"
#include "D3D12SwapChain.h"

namespace JEngine2 {

	D3D12SwapChain::D3D12SwapChain(HWND hwnd, ComPtr<ID3D12Device2> device, ComPtr<ID3D12CommandQueue> commandQueue, uint32_t width, uint32_t height, uint32_t bufferCount)
		:mHwnd(hwnd), 
		mDevice(device), 
		mCommandQueue(commandQueue),
		mWidth(width),
		mHeight(height),
		mBufferCount(bufferCount)
	{
		CreateDXGISwapChain();
		CreateRTVDescriptorHeap();
		CreateRTVs();
	}

	void D3D12SwapChain::Present() const
	{
		UINT presentFlags = CheckTearingSupport() && !mVSync ? DXGI_PRESENT_ALLOW_TEARING : 0;
		DXCALL(mDXGISwapChain->Present(static_cast<int>(mVSync), presentFlags));
	}

	CD3DX12_CPU_DESCRIPTOR_HANDLE D3D12SwapChain::GetCurrentRTV() const
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(mRTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			GetCurrentBackBufferIndex(), mRTVDescriptorSize);
		return rtv;
	}

	bool D3D12SwapChain::CheckTearingSupport() const
	{
		BOOL allowTearing = FALSE;

		// Rather than create the DXGI 1.5 factory interface directly, we create the
		// DXGI 1.4 interface and query for the 1.5 interface. This is to enable the 
		// graphics debugging tools which will not support the 1.5 factory interface 
		// until a future update.
		ComPtr<IDXGIFactory4> factory4;
		if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
		{
			ComPtr<IDXGIFactory5> factory5;
			if (SUCCEEDED(factory4.As(&factory5)))
			{
				if (FAILED(factory5->CheckFeatureSupport(
					DXGI_FEATURE_PRESENT_ALLOW_TEARING,
					&allowTearing, sizeof(allowTearing))))
				{
					allowTearing = FALSE;
				}
			}
		}

		return allowTearing == TRUE;
	}

	void D3D12SwapChain::CreateDXGISwapChain()
	{
		ComPtr<IDXGIFactory4> dxgiFactory4;
		UINT createFactoryFlags = 0;
		if constexpr (_DEBUG) {
			createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
		}

		DXCALL(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory4)));
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = mWidth;
		swapChainDesc.Height = mHeight;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.Stereo = FALSE;
		swapChainDesc.SampleDesc = { 1, 0 };
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = mBufferCount;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		// It is recommended to always allow tearing if tearing support is available.
		swapChainDesc.Flags = CheckTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
		ComPtr<IDXGISwapChain1> swapChain1;
		DXCALL(dxgiFactory4->CreateSwapChainForHwnd(
			mCommandQueue.Get(),
			mHwnd,
			&swapChainDesc,
			nullptr,
			nullptr,
			&swapChain1));

		// Disable the Alt+Enter full screen toggle feature. Switching to full screen
		// will be handled manually.
		DXCALL(dxgiFactory4->MakeWindowAssociation(mHwnd, DXGI_MWA_NO_ALT_ENTER));

		DXCALL(swapChain1.As(&mDXGISwapChain));
	}

	void D3D12SwapChain::CreateRTVDescriptorHeap()
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = mBufferCount;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

		DXCALL(mDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&mRTVDescriptorHeap)));

		mRTVDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	void D3D12SwapChain::CreateRTVs()
	{
		mBackBuffers.resize(mBufferCount);
		auto rtvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(mRTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

		for (uint32_t i = 0; i < mBufferCount; ++i)
		{
			ComPtr<ID3D12Resource> backBuffer;
			ThrowIfFailed(mDXGISwapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));

			mDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);

			mBackBuffers[i] = backBuffer;

			rtvHandle.Offset(rtvDescriptorSize);
		}
	}

}