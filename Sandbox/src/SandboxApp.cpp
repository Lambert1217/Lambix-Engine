#include <Lambix.h>
#include "imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Lambix::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_Camera(-1.0f, 1.0f, 1.0f, -1.0f), m_ShaderLibrary(std::make_shared<Lambix::ShaderLibrary>())
	{
		m_VertexArray = Lambix::VertexArray::Create();

		float vertices[] = {
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f };
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
		m_PlayerTexture = Lambix::Texture2D::Create("assets/textures/peashooter_run_1.png");
	}

	void OnUpdate(Lambix::Timestep ts) override
	{
		// camera
		if (Lambix::Input::IsKeyPressed(LB_KEY_A))
			C_Position.x += MoveSpeed * ts;
		else if (Lambix::Input::IsKeyPressed(LB_KEY_D))
			C_Position.x -= MoveSpeed * ts;
		if (Lambix::Input::IsKeyPressed(LB_KEY_W))
			C_Position.y += MoveSpeed * ts;
		else if (Lambix::Input::IsKeyPressed(LB_KEY_S))
			C_Position.y -= MoveSpeed * ts;

		if (Lambix::Input::IsKeyPressed(LB_KEY_Q))
			C_Rotation -= RotateSpeed * ts;
		else if (Lambix::Input::IsKeyPressed(LB_KEY_E))
			C_Rotation += RotateSpeed * ts;

		if (Lambix::Input::IsKeyPressed(LB_KEY_SPACE))
		{
			C_Position = { 0.0f, 0.0f, 0.0f };
			C_Rotation = 0.0f;
		}
		// square
		if (Lambix::Input::IsKeyPressed(LB_KEY_LEFT))
			S_Position.x -= MoveSpeed * ts;
		else if (Lambix::Input::IsKeyPressed(LB_KEY_RIGHT))
			S_Position.x += MoveSpeed * ts;
		if (Lambix::Input::IsKeyPressed(LB_KEY_UP))
			S_Position.y -= MoveSpeed * ts;
		else if (Lambix::Input::IsKeyPressed(LB_KEY_DOWN))
			S_Position.y += MoveSpeed * ts;

		Lambix::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Lambix::RenderCommand::Clear();

		m_Camera.SetPosition(C_Position);
		m_Camera.SetRotation(C_Rotation);

		Lambix::Renderer::BeginScene(m_Camera);

		Lambix::Ref<Lambix::Shader> Texture2D = m_ShaderLibrary->Get("Texture2D");
		std::dynamic_pointer_cast<Lambix::OpenGLShader>(Texture2D)->Bind();
		std::dynamic_pointer_cast<Lambix::OpenGLShader>(Texture2D)->UploadUniformInt("u_Texture", 0);

		m_BackgroundTexture->Bind();
		Lambix::Renderer::Submit(Texture2D, m_VertexArray);

		m_PlayerTexture->Bind();
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f / (1.28f / 0.72f), 0.3f, 0.3f));
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), S_Position) * scale;
		Lambix::Renderer::Submit(Texture2D, m_VertexArray, transform);

		Lambix::Renderer::EndScene();
	}

	void OnEvent(Lambix::Event& event) override
	{}

	void OnImGuiRender() override
	{
		// ImGui::Begin("Setting");
		// ImGui::ColorEdit3("Color Setting", glm::value_ptr(uColor));
		// ImGui::End();
	}
private:
	Lambix::Ref<Lambix::ShaderLibrary> m_ShaderLibrary;
	Lambix::Ref<Lambix::VertexArray> m_VertexArray;

	Lambix::Ref<Lambix::Texture2D> m_BackgroundTexture, m_PlayerTexture;

	Lambix::OrthoCamera m_Camera;

	glm::vec3 S_Position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 C_Position = { 0.0f, 0.0f, 0.0f };
	float C_Rotation = 0.0f;

	float MoveSpeed = 1.0f;
	float RotateSpeed = 90.0f;
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