#pragma once
#include "D3D12Include.h"
#include "Renderer/RHI/RHIResource.h"

namespace JEngine2 {
	struct D3D12RenderTarget : public RHIRenderTarget {
		D3D12RenderTarget(CD3DX12_CPU_DESCRIPTOR_HANDLE _handle, ComPtr<ID3D12Resource> _resource) :handle(_handle),resource(_resource) {}
		virtual ~D3D12RenderTarget() override {}
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle;
		ComPtr<ID3D12Resource> resource;
	};

	//struct D3D12Resource : public RHIResource {
	//	D3D12Resource(ComPtr<ID3D12Resource> _resource) : resource(_resource) {}
	//	virtual ~D3D12Resource() override {}
	//	ComPtr<ID3D12Resource> resource;
	//};
}