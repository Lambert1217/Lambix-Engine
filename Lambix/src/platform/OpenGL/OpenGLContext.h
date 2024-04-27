#pragma once

#include "Lambix/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Lambix
{

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow *windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow *m_WindowHandle;
	};

}