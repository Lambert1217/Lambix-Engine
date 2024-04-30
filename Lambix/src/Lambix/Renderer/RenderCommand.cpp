#include "RenderCommand.h"

#include "platform/OpenGL/OpenGLRendererAPI.h"

namespace Lambix
{
    RendererAPI *RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}