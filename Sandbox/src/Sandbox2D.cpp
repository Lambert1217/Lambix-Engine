#include "Sandbox2D.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

#define PROFILE_SCOPE(name) Lambix::Timer timer##__LINE__(name, [&](ProfileResult result) { m_ProfileResults.push_back(result); })

Sandbox2D::Sandbox2D(uint32_t width, uint32_t height)
    : Lambix::Layer("Sandbox2D"), m_OrthoCameraController((float)width / (float)height)
{}

void Sandbox2D::OnAttach()
{
    m_BackgroundTexture = Lambix::Texture2D::Create("assets/textures/menu_background.png");
}

void Sandbox2D::OnDetach()
{}

void Sandbox2D::OnUpdate(Lambix::Timestep ts)
{
    PROFILE_SCOPE("Sandbox2D::OnUpdate");

    {
        PROFILE_SCOPE("m_OrthoCameraController::OnUpdate");
        m_OrthoCameraController.OnUpdate(ts);
    }

    {
        PROFILE_SCOPE("Renderer prep");
        Lambix::RenderCommand::SetClearColor(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
        Lambix::RenderCommand::Clear();
    }

    {
        PROFILE_SCOPE("Renderer draw");
        Lambix::Renderer2D::BeginScene(m_OrthoCameraController.GetCamera());
        Lambix::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.7f, 0.7f }, glm::radians(0.0f), m_Color);
        Lambix::Renderer2D::DrawQuad({ 0.5f, 0.2f }, { 0.4f, 0.4f }, glm::radians(45.0f), m_Color);
        Lambix::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 1920.0f / 1080.0f, 1.0f }, glm::radians(0.0f), m_BackgroundTexture);
        Lambix::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Setting");
    ImGui::ColorEdit4("Color Setting", glm::value_ptr(m_Color));
    ImGui::End();

    ImGui::Begin("ProfileResults");
    ImGui::SetWindowFontScale(1.3f);
    for (auto& result : m_ProfileResults)
    {
        ImGui::Text("%s: %.3fms", result.Name, result.Time);
    }
    m_ProfileResults.clear();
    ImGui::End();
}

void Sandbox2D::OnEvent(Lambix::Event& event)
{
    m_OrthoCameraController.OnEvent(event);
}
