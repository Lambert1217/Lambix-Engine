#include <Lambix.h>
#include "imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "platform/OpenGL/OpenGLShader.h"

using namespace Lambix;

class ExampleLayer : public Lambix::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_Camera(-1.28f, 1.28f, 0.72f, -0.72f)
	{
		m_VertexArray.reset(VertexArray::Create());

		float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f };
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{"aPos", ShaderDataType::Float3}
		};
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

			uniform mat4 aViewProjection;
			uniform mat4 aTransform;

			void main()
			{
				gl_Position = aViewProjection * aTransform * vec4(aPos,1.0f);
			} 
		)";
		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 FragColor;
			uniform vec3 uColor;

			void main()
			{
				FragColor = vec4(uColor,1.0f);
			} 
		)";

		m_Shader.reset(Shader::Create(vertexSrc, fragmentSrc));
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

		if (Input::IsKeyPressed(LB_KEY_Q))
			C_Rotation -= RotateSpeed * ts;
		else if (Input::IsKeyPressed(LB_KEY_E))
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

		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		RenderCommand::Clear();

		m_Camera.SetPosition(C_Position);
		m_Camera.SetRotation(C_Rotation);

		Renderer::BeginScene(m_Camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));

		std::dynamic_pointer_cast<OpenGLShader>(m_Shader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(m_Shader)->UploadUniformFloat3("uColor", uColor);

		for (int i = 0;i < 5;i++)
		{
			for (int j = 0;j < 5;j++)
			{
				glm::vec3 pos(S_Position.x + (i - 2) * 0.21f, S_Position.y + (j - 2) * 0.21f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Renderer::Submit(m_Shader, m_VertexArray, transform);
			}
		}

		Renderer::EndScene();
	}

	void OnEvent(Lambix::Event& event) override
	{}

	void OnImGuiRender() override
	{
		ImGui::Begin("Setting");
		ImGui::ColorEdit3("Color Setting", glm::value_ptr(uColor));
		ImGui::End();
	}
private:
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<VertexArray> m_VertexArray;

	OrthoCamera m_Camera;

	glm::vec3 S_Position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 C_Position = { 0.0f, 0.0f, 0.0f };
	float C_Rotation = 0.0f;

	float MoveSpeed = 1.0f;
	float RotateSpeed = 90.0f;

	glm::vec3 uColor = { 1.0f, 0.8f, 0.8f };
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