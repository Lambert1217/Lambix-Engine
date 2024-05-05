#include "Renderer.h"

namespace Lambix
{
    Renderer::SceneData *Renderer::m_SceneData = new Renderer::SceneData();

    void Renderer::BeginScene(OrthoCamera &camera)
    {
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray>& vertexArray
        , const glm::mat4& transform)
    {
        shader->Bind();
        shader->UploadUniformMat4("aViewProjection", m_SceneData->ViewProjectionMatrix);
        shader->UploadUniformMat4("aTransform", transform);
        RenderCommand::DrawIndexed(vertexArray);
        shader->Unbind();
    }
}