#include "OrthoCameraController.h"
#include "Lambix/Core/Input.h"
#include "Lambix/Core/KeyCodes.h"
#include "Lambix/Core/Core.h"

namespace Lambix
{
    OrthoCameraController::OrthoCameraController(float aspectRatio, bool rotation)
        : m_AspectRatio(aspectRatio),
        m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
        m_Rotation(rotation)
    {}

    void OrthoCameraController::OnUpdate(Timestep ts)
    {
        if (Lambix::Input::IsKeyPressed(LB_KEY_SPACE))
        {
            m_CameraPosition = { 0.0f, 0.0f, 0.0f };
            m_CameraRotation = 0.0f;
        }

        m_CameraTranslationSpeed = m_ZoomLevel;

        if (Lambix::Input::IsKeyPressed(LB_KEY_A))
            m_CameraPosition.x += m_CameraTranslationSpeed * ts;
        else if (Lambix::Input::IsKeyPressed(LB_KEY_D))
            m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
        if (Lambix::Input::IsKeyPressed(LB_KEY_W))
            m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
        else if (Lambix::Input::IsKeyPressed(LB_KEY_S))
            m_CameraPosition.y += m_CameraTranslationSpeed * ts;

        m_Camera.SetPosition(m_CameraPosition);

        if (m_Rotation)
        {
            if (Lambix::Input::IsKeyPressed(LB_KEY_Q))
                m_CameraRotation -= m_CameraRotationSpeed * ts;
            else if (Lambix::Input::IsKeyPressed(LB_KEY_E))
                m_CameraRotation += m_CameraRotationSpeed * ts;

            m_Camera.SetRotation(m_CameraRotation);
        }
    }

    void OrthoCameraController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(LB_BIND_EVENT_FN(OrthoCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(LB_BIND_EVENT_FN(OrthoCameraController::OnWindowResized));
    }

    bool OrthoCameraController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        m_ZoomLevel -= e.GetYOffset() * 0.1f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.4f);
        m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }

    bool OrthoCameraController::OnWindowResized(WindowResizeEvent& e)
    {
        m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
        m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }

}