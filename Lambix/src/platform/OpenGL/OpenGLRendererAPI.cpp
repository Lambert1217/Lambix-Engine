#include "OpenGLRendererAPI.h"

#include "glad/glad.h"

#include "Lambix/Core/Log.h"

namespace Lambix
{
    void OpenGLRendererAPI::Init()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void OpenGLRendererAPI::SetClearColor(const glm::vec4 &color)
    {
        glClearColor(color.x, color.y, color.z, color.w);
    }
    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }
    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
    {
        vertexArray->Bind();
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
}