#include "Shader.h"
#include "Renderer.h"
#include "Lambix/Core.h"
#include "Lambix/Log.h"

#include "platform/OpenGL/OpenGLShader.h"

namespace Lambix
{
    Ref<Shader> Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            LB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLShader>(vertexSource, fragmentSource);
        default:
            LB_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }
}
