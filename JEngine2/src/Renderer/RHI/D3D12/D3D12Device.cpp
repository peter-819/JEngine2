#include "pch.h"
#include "D3D12Device.h"
#include "D3D12Resource.h"
#include "D3D12CommandList.h"

namespace JEngine2 {
	
	D3D12Device::D3D12Device(const RHIDeviceConfig& config)
	{
		if (config.bEnableDebugLayer) {
			D3D12Infrastructure::EnableDebugLayer();
		}
		mAdaptor = D3D12Infrastructure::GetAdaptor();
		mDevice = D3D12Infrastructure::CreateDevice(mAdaptor);
		mDirectQueue = std::make_unique<D3D12CommandQueue>(mDevice, D3D12_COMMAND_LIST_TYPE_DIRECT);
		mCopyQueue = std::make_unique<D3D12CommandQueue>(mDevice, D3D12_COMMAND_LIST_TYPE_COPY);

		mSwapChain = std::make_unique<D3D12SwapChain>(config.windowHandle, mDevice, mDirectQueue->GetD3D12CommandQueue(), 
			config.windowWidth, config.windowHeight, mNumFrames);
		mCurrentBackBufferIndex = mSwapChain->GetCurrentBackBufferIndex();

		mFence = D3D12Infrastructure::CreateFence(mDevice);
		mEventHandle = D3D12Infrastructure::CreateEventHandle();
		
		memset(mFrameFenceValues, 0, sizeof mFrameFenceValues);
	}

	RHICommandListPtr D3D12Device::GetCommandList(RHICommandListType type)
	{
		switch (type) {
		case RHICommandListType::Direct:
			return mDirectQueue->GetCommandList();
			break;
		case RHICommandListType::Copy:
			return mCopyQueue->GetCommandList();
			break;
		default:
			Assert(false && "Invalid Command List Type");
			break;
		}
		return nullptr;
	}

	void D3D12Device::ExecuteCommandList(RHICommandListPtr commandList)
	{
		switch (commandList->GetType())
		{
		case RHICommandListType::Direct:
			mDirectQueue->ExecuteCommandList(commandList);
			break;
		case RHICommandListType::Copy:
			mCopyQueue->ExecuteCommandList(commandList);
			break;
		default:
			break;
		}
	}

	D3D12Device::~D3D12Device()
	{
		D3D12Infrastructure::Flush(mDirectQueue->GetD3D12CommandQueue(), mFence, mFenceValue, mEventHandle);
		::CloseHandle(mEventHandle);
	}

	void D3D12Device::BeginFrame()
	{
	}

	void D3D12Device::EndFrame()
	{
		//mFrameFenceValues[mCurrentBackBufferIndex] = D3D12Infrastructure::Signal(mCommandQueue, mFence, mFenceValue);

		mSwapChain->Present();

		//mCurrentBackBufferIndex = mSwapChain->GetCurrentBackBufferIndex();
		//D3D12Infrastructure::WaitForFenceValue(mFence, mFrameFenceValues[mCurrentBackBufferIndex], mEventHandle);
	}

	RHIRenderTargetPtr D3D12Device::GetCurrentSwapChainRT() const
	{
		return std::make_shared<D3D12RenderTarget>(mSwapChain->GetCurrentRTV(),mSwapChain->GetCurrentBackBuffer());
	}

	D3D12CommandQueue::D3D12CommandQueue(const ComPtr<ID3D12Device2>& device, D3D12_COMMAND_LIST_TYPE type)
		:mDevice(device), mCommandListType(type)
	{
		mFence = D3D12Infrastructure::CreateFence(mDevice);
		mEventHandle = D3D12Infrastructure::CreateEventHandle();
		mCommandQueue = D3D12Infrastructure::CreateCommandQueue(device, type);
		switch (type)
		{
		case D3D12_COMMAND_LIST_TYPE_DIRECT:
			mRHICommandListType = RHICommandListType::Direct;
			break;
		case D3D12_COMMAND_LIST_TYPE_COPY:
			mRHICommandListType = RHICommandListType::Copy;
			break;
		default:
			Assert(false && "Invalid RHI Command List Type");
			break;
		}
	}

	RHICommandListPtr D3D12CommandQueue::GetCommandList()
	{
		ComPtr<ID3D12CommandAllocator> allocator;
		
		if (!mCommandAllocatorQueue.empty() && IsFenceComplete(mCommandAllocatorQueue.front().fenceValue)) {
			allocator = mCommandAllocatorQueue.front().allocator;
			mCommandAllocatorQueue.pop();
		}
		else {
			allocator = D3D12Infrastructure::CreateCommandAllocator(mDevice, mCommandListType);
		}
		DXCALL(allocator->Reset());
		
		ComPtr<ID3D12GraphicsCommandList2> commandList;
		if (mCommandListQueue.empty()) {
			commandList = D3D12Infrastructure::CreateCommandList(mDevice, allocator, mCommandListType);

			commandList->Reset(allocator.Get(), nullptr);
		}
		else {
			commandList = mCommandListQueue.front();
			mCommandListQueue.pop();
			DXCALL(commandList->Reset(allocator.Get(), nullptr));
		} 
		// Associate the command allocator with the command list so that it can be
		// retrieved when the command list is executed.
		DXCALL(commandList->SetPrivateDataInterface(__uuidof(ID3D12CommandAllocator), allocator.Get()));
		return std::make_shared<D3D12CommandList>(commandList,mRHICommandListType);
	}

	uint64_t D3D12CommandQueue::ExecuteCommandList(RHICommandListPtr rhiCommandList)
	{
		auto commandList = dynamic_cast<D3D12CommandList&>(*rhiCommandList).mCommandList;

		commandList->Close();

		ID3D12CommandAllocator* commandAllocator;
		UINT dataSize = sizeof(commandAllocator);
		ThrowIfFailed(commandList->GetPrivateData(__uuidof(ID3D12CommandAllocator), &dataSize, &commandAllocator));

		ID3D12CommandList* const ppCommandLists[] = {
			commandList.Get()
		};

		mCommandQueue->ExecuteCommandLists(1, ppCommandLists);
		uint64_t fenceValue = Signal();

		mCommandAllocatorQueue.emplace(AF{ commandAllocator, fenceValue});
		mCommandListQueue.push(commandList);

		// The ownership of the command allocator has been transferred to the ComPtr
		// in the command allocator queue. It is safe to release the reference 
		// in this temporary COM pointer here.
		commandAllocator->Release();

		return fenceValue;
	}

	bool D3D12CommandQueue::IsFenceComplete(uint64_t fenceValue) const
	{
		return mFence->GetCompletedValue() >= fenceValue;
	}

	void D3D12CommandQueue::WaitForFenceValue(uint64_t fenceValue)
	{
		if (!IsFenceComplete(fenceValue)) {
			mFence->SetEventOnCompletion(fenceValue, mEventHandle);
			::WaitForSingleObject(mEventHandle,DWORD_MAX);
		}
	}

	uint64_t D3D12CommandQueue::Signal()
	{
		mCommandQueue->Signal(mFence.Get(), ++mFenceValue);
		return mFenceValue;
	}

}
