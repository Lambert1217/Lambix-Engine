#include "lbpch.h"

#include "Application.h"
#include "Log.h"
#include "Input.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Lambix
{

	Application *Application::m_Instance = nullptr;

	Application::Application()
	{
		LB_CORE_ASSERT(!m_Instance, "Application already exists!");
		m_Instance = this;

		// 窗口类创建 设置回调
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(LB_BIND_EVENT_FN(Application::OnEvent));

		// ImGui 图层创建 附加
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		//---------------------------
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		float vertices[3 * 3] = {
			0.0f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		uint32_t indices[3] = {0, 1, 2};
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		// shader
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 aPos;
			out vec3 color;
			void main()
			{
				gl_Position = vec4(aPos,1.0f);
				color = aPos;
			} 
		)";
		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 FragColor;
			in vec3 color;
			void main()
			{
				//FragColor = vec4(1.0f,0.8f,0.8f,1.0f);
				FragColor = vec4(color,1.0f);
			} 
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
			m_Shader->Unbind();

			// 遍历各层级 执行更新
			for (Layer *layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

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

		// LB_CORE_TRACE("{0}", e);

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
}