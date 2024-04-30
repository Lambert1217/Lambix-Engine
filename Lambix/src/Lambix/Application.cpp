#include "lbpch.h"

#include "Application.h"
#include "Log.h"
#include "Input.h"

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
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(LB_BIND_EVENT_FN(Application::OnEvent));

		// ImGui 图层创建 附加
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		//---------------------------
		m_VertexArray.reset(VertexArray::Create());

		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.8f, 0.8f, 1.0f,
			-0.5f, 0.5f, 0.0f, 1.0f, 0.8f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.8f, 0.8f, 1.0f,
			0.5f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f};
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{"aPos", ShaderDataType::Float3},
			{"aColor", ShaderDataType::Float4}};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[] = {0, 1, 2, 1, 2, 3};
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		// shader
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 aPos;
			layout(location = 1) in vec4 aColor;
			out vec4 color;
			void main()
			{
				gl_Position = vec4(aPos,1.0f);
				color = aColor;
			} 
		)";
		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 FragColor;
			in vec4 color;
			void main()
			{
				//FragColor = vec4(1.0f,0.8f,0.8f,1.0f);
				FragColor = color,1.0f;
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
			RenderCommand::SetClearColor({0.2f, 0.2f, 0.2f, 1.0f});
			RenderCommand::Clear();

			Renderer::BeginScene();

			m_Shader->Bind();
			Renderer::Submit(m_VertexArray);
			m_Shader->Unbind();

			Renderer::EndScene();

			// 遍历各层级 执行更新
			for (Layer *layer : m_LayerStack)
			{
				layer->OnUpdate();
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