#include "OrthoCamera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Lambix
{
    OrthoCamera::OrthoCamera(float left, float right, float bottom, float top)
        : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
    {
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthoCamera::SetPosition(const glm::vec3 &position)
    {
        m_Position = position;
        RecalculateViewMatrix();
    }
    void OrthoCamera::SetRotation(float rotation)
    {
        m_Rotation = rotation;
        RecalculateViewMatrix();
    }
    void OrthoCamera::RecalculateViewMatrix()
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
        transform = glm::rotate(transform, glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}