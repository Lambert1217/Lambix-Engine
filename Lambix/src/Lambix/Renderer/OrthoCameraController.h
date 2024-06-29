#pragma once 

#include "Lambix/Core/Timestep.h"
#include "OrthoCamera.h"
#include "Lambix/Events/MouseEvent.h"
#include "Lambix/Events/ApplicationEvent.h"

namespace Lambix
{
    class OrthoCameraController
    {
    public:
        OrthoCameraController(float aspectRatio, bool rotation = false);

        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);

        OrthoCamera& GetCamera() { return m_Camera; };
        const OrthoCamera& GetCamera() const { return m_Camera; };
    private:
        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);

    private:
        float m_AspectRatio;
        float m_ZoomLevel = 1.0f;
        OrthoCamera m_Camera;

        bool m_Rotation;

        glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
        float m_CameraRotation = 0.0f;
        float m_CameraTranslationSpeed = 1.0f, m_CameraRotationSpeed = 90.0f;
    };
}