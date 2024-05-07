#include "Renderer.h"

#include "platform/OpenGL/OpenGLShader.h"

namespace Lambix
{
    Renderer::SceneData *Renderer::m_SceneData = new Renderer::SceneData();

    void Renderer::Init()
    {
        RenderCommand::Init();
    }

    void Renderer::BeginScene(OrthoCamera& camera)
    {
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const Ref<Shader> shader, const Ref<VertexArray>& vertexArray
        , const glm::mat4& transform)
    {
        shader->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("aViewProjection", m_SceneData->ViewProjectionMatrix);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("aTransform", transform);
        RenderCommand::DrawIndexed(vertexArray);
        shader->Unbind();
    }
}