#include "lbpch.h"
#include "OpenGLContext.h"
#include "Lambix/Core/Log.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"


namespace Lambix {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		LB_CORE_ASSERT(windowHandle, "Window Handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		// 加载Glad
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		LB_CORE_ASSERT(status, "Failed to initialize Glad!");

		LB_CORE_INFO("OpenGL Info:");
		LB_CORE_INFO("  Vendor: {0}", (const char*)(glGetString(GL_VENDOR)));
		LB_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		LB_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}