#pragma once

#include <string>
#include <unordered_map>
#include "Lambix/Renderer/Shader.h"
#include "glm/glm.hpp"

//TODO: 删除
typedef unsigned int GLenum;

namespace Lambix
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
        virtual ~OpenGLShader();

        virtual const std::string& GetName() const { return m_Name; }

        void SetInt(const std::string& name, const int value) override;
        void SetFloat(const std::string& name, const float value) override;
        void SetFloat3(const std::string& name, const glm::vec3& value) override;
        void SetFloat4(const std::string& name, const glm::vec4& value) override;
        void SetMat4(const std::string& name, const glm::mat4& matrix) override;

        void Bind() const;
        void Unbind() const;

        void UploadUniformInt(const std::string& name, const int value);

        void UploadUniformFloat(const std::string& name, const float value);
        void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
        void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
        void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

        void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
    private:
        void Compile(std::unordered_map<GLenum, std::string> shaderSource);
        std::string ReadFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& SourceCode);
    private:
        uint32_t m_RendererID;
        std::string m_Name;
    };
}