#include "Texture.h"
#include "Renderer.h"
#include "Lambix/Core/Log.h"

#include "platform/OpenGL/OpenGLTexture.h"

namespace Lambix
{
    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            LB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLTexture2D>(width, height);
        default:
            LB_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }

    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            LB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLTexture2D>(path);
        default:
            LB_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }
}