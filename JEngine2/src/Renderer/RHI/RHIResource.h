#pragma once
#include <memory>

namespace JEngine2 {
	struct RHIRenderTarget {
		virtual ~RHIRenderTarget() {}
	};
	using RHIRenderTargetPtr = std::shared_ptr<RHIRenderTarget>;

	//struct RHIResource {
	//	virtual ~RHIResource() {}
	//};
	//using RHIResourcePtr = std::shared_ptr<RHIResource>;
}