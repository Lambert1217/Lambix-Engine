#include "Buffer.h"

#include "Renderer.h"
#include "Lambix/core.h"
#include "Lambix/Log.h"

#include "platform/OpenGL/OpenGLBuffer.h"

namespace Lambix
{
    VertexBuffer *VertexBuffer::Create(float *vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::None:
            LB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::OpenGL:
            return new OpenGLVertexBuffer(vertices, size);
        default:
            LB_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }
    IndexBuffer *IndexBuffer::Create(uint32_t *indices, uint32_t count)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::None:
            LB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::OpenGL:
            return new OpenGLIndexBuffer(indices, count);
        default:
            LB_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }
}