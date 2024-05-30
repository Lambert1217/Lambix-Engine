#pragma once
#include <Lambix.h>

class Sandbox2D : public Lambix::Layer
{
public:
    Sandbox2D();
    ~Sandbox2D() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(Lambix::Timestep ts) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Lambix::Event& event) override;

private:
    Lambix::OrthoCameraController m_OrthoCameraController;

    glm::vec4 m_Color = { 1.0f, 0.78f, 0.81f, 1.0f };
};