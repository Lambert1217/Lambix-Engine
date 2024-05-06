#pragma once

#include <string>
#include "Lambix/Core.h"

namespace Lambix
{
    class Shader
    {
    public:
        Shader() = default;
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static Ref<Shader> Create(const std::string& vertexSource, const std::string& fragmentSource);

    private:
        uint32_t m_RendererID;
    };
}