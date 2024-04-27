#pragma once

#include "Lambix/core.h"

namespace Lambix
{

	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

}