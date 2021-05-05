#pragma once
#include "Platform/PlatformMarcos.h"

namespace JEngine2 {
	class Application {
	public:
		Application(PlatformInstanceHandle instance);
		virtual void Run();
	private:
		PlatformInstanceHandle mInstance;
	};
}