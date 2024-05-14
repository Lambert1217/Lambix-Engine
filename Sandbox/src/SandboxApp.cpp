#include <Lambix.h>
#include "imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Lambix::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_OrthoCameraController(1280.0f / 720.0f, true),
		m_ShaderLibrary(std::make_shared<Lambix::ShaderLibrary>())
	{
		m_VertexArray = Lambix::VertexArray::Create();

		float vertices[] = {
			-1.28f, -0.72f, 0.0f, 0.0f, 0.0f,
			-1.28f, 0.72f, 0.0f, 0.0f, 1.0f,
			1.28f, -0.72f, 0.0f, 1.0f, 0.0f,
			1.28f, 0.72f, 0.0f, 1.0f, 1.0f };
		Lambix::Ref<Lambix::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer = Lambix::VertexBuffer::Create(vertices, sizeof(vertices));
		Lambix::BufferLayout layout = {
			{"aPos", Lambix::ShaderDataType::Float3},
			{"aTexCoord", Lambix::ShaderDataType::Float2}
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[] = { 0, 1, 2, 1, 2, 3 };
		Lambix::Ref<Lambix::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer = Lambix::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		m_ShaderLibrary->Load("assets/shaders/Texture2D.glsl");
		m_BackgroundTexture = Lambix::Texture2D::Create("assets/textures/test.jpg");
	}

	void OnUpdate(Lambix::Timestep ts) override
	{
		Lambix::RenderCommand::SetClearColor(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
		Lambix::RenderCommand::Clear();

		Lambix::Renderer::BeginScene(m_OrthoCameraController.GetCamera());

		m_OrthoCameraController.OnUpdate(ts);

		Lambix::Ref<Lambix::Shader> Texture2D = m_ShaderLibrary->Get("Texture2D");
		std::dynamic_pointer_cast<Lambix::OpenGLShader>(Texture2D)->Bind();
		std::dynamic_pointer_cast<Lambix::OpenGLShader>(Texture2D)->UploadUniformInt("u_Texture", 0);

		m_BackgroundTexture->Bind();
		Lambix::Renderer::Submit(Texture2D, m_VertexArray);

		Lambix::Renderer::EndScene();
	}

	void OnEvent(Lambix::Event& event) override
	{
		m_OrthoCameraController.OnEvent(event);
	}

	void OnImGuiRender() override
	{
		// ImGui::Begin("Setting");
		// ImGui::ColorEdit3("Color Setting", glm::value_ptr(uColor));
		// ImGui::End();
	}
private:
	Lambix::Ref<Lambix::ShaderLibrary> m_ShaderLibrary;
	Lambix::Ref<Lambix::VertexArray> m_VertexArray;
	Lambix::Ref<Lambix::Texture2D> m_BackgroundTexture;

	Lambix::OrthoCameraController m_OrthoCameraController;
};

class Sandbox : public Lambix::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{}
};

Lambix::Application* Lambix::CreateApplication()
{
	return new Sandbox();
}