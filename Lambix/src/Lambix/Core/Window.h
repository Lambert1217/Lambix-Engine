#pragma once

#include "lbpch.h"

#include "Lambix/Events/Event.h"

namespace Lambix
{

	// 窗口属性
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string &title = "Lambix Engine",
					unsigned int width = 1280, unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// 窗口接口类
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event &)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn &callback) = 0; // 事件回调
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void *GetNativeWindow() const = 0; // 获取活动窗口

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}