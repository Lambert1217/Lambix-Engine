#include "Renderer.h"

namespace Lambix
{
    void Renderer::BeginScene()
    {
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const std::shared_ptr<VertexArray> &vertexArray)
    {
        RenderCommand::DrawIndexed(vertexArray);
    }
}