#include "Sandbox2D.h"
#include "imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
    : Lambix::Layer("Sandbox2D"), m_OrthoCameraController(1280.0f / 720.0f, true)
    , m_ShaderLibrary(std::make_shared<Lambix::ShaderLibrary>())
{}

void Sandbox2D::OnAttach()
{
    m_VertexArray = Lambix::VertexArray::Create();
    float vertices[] = {
        -0.7f, -0.7f, 0.0f,
        -0.7f, 0.7f, 0.0f,
        0.7f, -0.7f, 0.0f,
        0.7f, 0.7f, 0.0f };
    Lambix::Ref<Lambix::VertexBuffer> m_VertexBuffer;
    m_VertexBuffer = Lambix::VertexBuffer::Create(vertices, sizeof(vertices));
    Lambix::BufferLayout layout = {
        {"aPos", Lambix::ShaderDataType::Float3}
    };
    m_VertexBuffer->SetLayout(layout);
    m_VertexArray->AddVertexBuffer(m_VertexBuffer);

    uint32_t indices[] = { 0, 1, 2, 1, 2, 3 };
    Lambix::Ref<Lambix::IndexBuffer> m_IndexBuffer;
    m_IndexBuffer = Lambix::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);

    m_ShaderLibrary->Load("assets/shaders/Texture2D.glsl");
}

void Sandbox2D::OnDetach()
{}

void Sandbox2D::OnUpdate(Lambix::Timestep ts)
{
    Lambix::RenderCommand::SetClearColor(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
    Lambix::RenderCommand::Clear();

    Lambix::Renderer::BeginScene(m_OrthoCameraController.GetCamera());

    m_OrthoCameraController.OnUpdate(ts);

    Lambix::Ref<Lambix::Shader> Texture2D = m_ShaderLibrary->Get("Texture2D");
    std::dynamic_pointer_cast<Lambix::OpenGLShader>(Texture2D)->Bind();
    std::dynamic_pointer_cast<Lambix::OpenGLShader>(Texture2D)->UploadUniformFloat4("u_Color", m_Color);

    Lambix::Renderer::Submit(Texture2D, m_VertexArray);

    Lambix::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Setting");
    ImGui::ColorEdit4("Color Setting", glm::value_ptr(m_Color));
    ImGui::End();
}

void Sandbox2D::OnEvent(Lambix::Event& event)
{
    m_OrthoCameraController.OnEvent(event);
}
