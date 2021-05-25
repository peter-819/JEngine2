#pragma once

namespace JEngine2 {
	struct RHIRenderTarget;

	struct RHIColor {
		float r, g, b, a;
		RHIColor() :r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}
		RHIColor(float _r, float _g, float _b) :r(_r), g(_g), b(_b), a(0.0f) {}
	};

	enum class RHICommandListType {
		Direct = 0,
		Copy
	};

	struct RHIRenderPassDesc {
		RHIColor color;
		std::shared_ptr<RHIRenderTarget> rt;
	};
}