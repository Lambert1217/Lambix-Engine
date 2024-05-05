#include <Lambix.h>
#include "imgui.h"

#include "glm/gtc/matrix_transform.hpp"

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
			uniform mat4 aTransform;
			out vec4 color;
			void main()
			{
				gl_Position = aViewProjection * aTransform * vec4(aPos,1.0f);
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

	void OnUpdate(Timestep ts) override
	{
		// camera
		if (Input::IsKeyPressed(LB_KEY_A))
			C_Position.x += MoveSpeed * ts;
		else if (Input::IsKeyPressed(LB_KEY_D))
			C_Position.x -= MoveSpeed * ts;
		if (Input::IsKeyPressed(LB_KEY_W))
			C_Position.y += MoveSpeed * ts;
		else if (Input::IsKeyPressed(LB_KEY_S))
			C_Position.y -= MoveSpeed * ts;

		if (Input::IsKeyPressed(LB_KEY_PAGE_UP))
			C_Rotation -= RotateSpeed * ts;
		else if (Input::IsKeyPressed(LB_KEY_PAGE_DOWN))
			C_Rotation += RotateSpeed * ts;

		if (Input::IsKeyPressed(LB_KEY_SPACE))
		{
			C_Position = { 0.0f, 0.0f, 0.0f };
			C_Rotation = 0.0f;
		}
		// square
		if (Input::IsKeyPressed(LB_KEY_LEFT))
			S_Position.x -= MoveSpeed * ts;
		else if (Input::IsKeyPressed(LB_KEY_RIGHT))
			S_Position.x += MoveSpeed * ts;
		if (Input::IsKeyPressed(LB_KEY_UP))
			S_Position.y -= MoveSpeed * ts;
		else if (Input::IsKeyPressed(LB_KEY_DOWN))
			S_Position.y += MoveSpeed * ts;

		if (Input::IsKeyPressed(LB_KEY_I))
			S_Rotation -= RotateSpeed * ts;
		else if (Input::IsKeyPressed(LB_KEY_O))
			S_Rotation += RotateSpeed * ts;

		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		RenderCommand::Clear();

		m_Camera.SetPosition(C_Position);
		m_Camera.SetRotation(C_Rotation);

		Renderer::BeginScene(m_Camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));

		for (int i = 0;i < 3;i++)
		{
			for (int j = 0;j < 3;j++)
			{
				glm::vec3 pos(S_Position.x + i * 0.21f, S_Position.y + j * 0.21f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos);
				transform = glm::rotate(transform, glm::radians(S_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
				transform *= scale;
				Renderer::Submit(m_Shader, m_VertexArray, transform);
			}
		}

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

	glm::vec3 S_Position = { 0.0f, 0.0f, 0.0f };
	float S_Rotation = 0.0f;

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