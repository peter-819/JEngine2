#pragma once

#include "RHIDesc.h"
#include "RHIResource.h"

namespace JEngine2 {
	class RHICommandList {
	public:
		RHICommandList(RHICommandListType type):mType(type){}
		FORCEINLINE RHICommandListType GetType() const { return mType; }
	public:
		virtual void BeginPass(const RHIRenderPassDesc& desc) = 0;
		virtual void EndPass() = 0;

		virtual void BeginFrame(const RHIRenderTargetPtr& rt) = 0;
		virtual void EndFrame() = 0;
	protected:
		RHICommandListType mType;

		RHIRenderTargetPtr mCurrentRT;
		
	};
	using RHICommandListPtr = std::shared_ptr<RHICommandList>;
}