#include <Lambix.h>
#include "imgui.h"

using namespace Lambix;

class ExampleLayer : public Lambix::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_Camera(-1.0f, 1.0f, 1.0f, -1.0f)
	{
		m_VertexArray.reset(VertexArray::Create());

		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.8f, 0.8f, 1.0f,
			-0.5f, 0.5f, 0.0f, 1.0f, 0.8f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.8f, 0.8f, 1.0f,
			0.5f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f };
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{"aPos", ShaderDataType::Float3},
			{"aColor", ShaderDataType::Float4} };
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[] = { 0, 1, 2, 1, 2, 3 };
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		// shader
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 aPos;
			layout(location = 1) in vec4 aColor;

			uniform mat4 aViewProjection;
			out vec4 color;
			void main()
			{
				gl_Position = aViewProjection * vec4(aPos,1.0f);
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

	void OnUpdate() override
	{
		if (Input::IsKeyPressed(LB_KEY_LEFT))
			Position.x += CameraMoveSpeed;
		else if (Input::IsKeyPressed(LB_KEY_RIGHT))
			Position.x -= CameraMoveSpeed;
		if (Input::IsKeyPressed(LB_KEY_UP))
			Position.y += CameraMoveSpeed;
		else if (Input::IsKeyPressed(LB_KEY_DOWN))
			Position.y -= CameraMoveSpeed;

		if (Input::IsKeyPressed(LB_KEY_D))
			Rotation -= CameraRotateSpeed;
		else if (Input::IsKeyPressed(LB_KEY_A))
			Rotation += CameraRotateSpeed;

		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		RenderCommand::Clear();

		m_Camera.SetPosition(Position);
		m_Camera.SetRotation(Rotation);

		Renderer::BeginScene(m_Camera);

		Renderer::Submit(m_Shader, m_VertexArray);

		Renderer::EndScene();
	}

	void OnEvent(Lambix::Event& event) override
	{}

	void OnImGuiRender() override
	{}
private:
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<VertexArray> m_VertexArray;

	OrthoCamera m_Camera;
	glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
	float Rotation = 0.0f;

	float CameraMoveSpeed = 0.01f;
	float CameraRotateSpeed = 1.0f;
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