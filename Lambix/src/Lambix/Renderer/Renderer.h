#pragma once

#include "RenderCommand.h"
#include "Shader.h"
#include "OrthoCamera.h"

namespace Lambix
{
    class Renderer
    {
    public:
        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

        static void BeginScene(OrthoCamera &camera);
        static void EndScene();

        static void Submit(const std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray> &vertexArray);

    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static SceneData *m_SceneData;
    };
}