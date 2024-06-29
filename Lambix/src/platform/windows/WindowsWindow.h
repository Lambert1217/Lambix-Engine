#pragma once

#include "Lambix/Core/Window.h"

#include <GLFW/glfw3.h>

#include "Lambix/Renderer/GraphicsContext.h"

/*
	Windows窗口类实现  使用glfw
*/

namespace Lambix
{

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps &props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFn &callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void *GetNativeWindow() const override { return m_Window; };

	private:
		virtual void Init(const WindowProps &props);
		virtual void Shutdown();

	private:
		GLFWwindow *m_Window;
		GraphicsContext *m_Context;

		// 内部结构体 记录窗口属性
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}