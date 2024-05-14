#include "lbpch.h"

#include "Application.h"
#include "Log.h"
#include "Input.h"
#include "Lambix/Core/Timestep.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Lambix/Renderer/Renderer.h"

namespace Lambix
{
	Application *Application::m_Instance = nullptr;

	Application::Application()
	{
		LB_CORE_ASSERT(!m_Instance, "Application already exists!");
		m_Instance = this;

		// 窗口类创建 设置回调
		m_Window = Window::Create();
		m_Window->SetEventCallback(LB_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		// ImGui 图层创建 附加
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float CurrentTime = (float)glfwGetTime();
			Timestep timestep = CurrentTime - LastFrameTime;
			LastFrameTime = CurrentTime;

			if (!m_Minsize)
			{
				// 遍历各层级 执行更新
				for (Layer* layer : m_LayerStack)
				{
					layer->OnUpdate(timestep);
				}
			}

			// imgui 绘制
			m_ImGuiLayer->Begin();
			for (Layer *layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			// 窗口类更新
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event &e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(LB_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(LB_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.GetHandled())
				break;
		}
	}

	void Application::PushLayer(Layer *layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer *overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent &e)
	{
		m_Running = false;
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minsize = true;
			return false;
		}
		m_Minsize = false;
		RenderCommand::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
		return false;
	}
}