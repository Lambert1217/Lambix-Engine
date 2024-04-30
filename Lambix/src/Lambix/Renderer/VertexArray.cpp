#include "VertexArray.h"
#include "Renderer.h"
#include "Lambix/core.h"
#include "Lambix/Log.h"

#include "platform/OpenGL/OpenGLVertexArray.h"

namespace Lambix
{
    VertexArray *VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            LB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLVertexArray();
        default:
            LB_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }
}
