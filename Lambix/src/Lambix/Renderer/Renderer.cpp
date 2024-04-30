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

    void Renderer::Submit(const std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray> &vertexArray)
    {
        shader->Bind();
        shader->UploadUniformMat4("aViewProjection", m_SceneData->ViewProjectionMatrix);
        RenderCommand::DrawIndexed(vertexArray);
        shader->Unbind();
    }
}