#include "Sandbox2D.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

#include "platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
    : Lambix::Layer("Sandbox2D"), m_OrthoCameraController(1280.0f / 720.0f, true)
{}

void Sandbox2D::OnAttach()
{}

void Sandbox2D::OnDetach()
{}

void Sandbox2D::OnUpdate(Lambix::Timestep ts)
{
    m_OrthoCameraController.OnUpdate(ts);

    Lambix::RenderCommand::SetClearColor(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
    Lambix::RenderCommand::Clear();

    Lambix::Renderer2D::BeginScene(m_OrthoCameraController.GetCamera());
    Lambix::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 1.2f, 1.2f }, m_Color);
    Lambix::Renderer2D::DrawQuad({ 0.5f, 0.6f }, { 1.0f, 1.0f }, m_Color);
    Lambix::Renderer2D::EndScene();
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
