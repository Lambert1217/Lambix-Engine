#include "OpenGLShader.h"
#include "Lambix/Log.h"
#include "Lambix/Core.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include <fstream>

namespace Lambix
{
    static GLenum ShaderTypeFromString(const std::string& type)
    {
        if (type == "vertex")
            return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel")
            return GL_FRAGMENT_SHADER;

        LB_CORE_ASSERT(false, "Unknown shader type!");
        return 0;
    }
    OpenGLShader::OpenGLShader(const std::string& filepath)
    {
        std::string SourceCode = ReadFile(filepath);
        auto shaderSource = PreProcess(SourceCode);
        Compile(shaderSource);
    }

    OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource)
    {
        std::unordered_map<GLenum, std::string> shaderSource;
        shaderSource[GL_VERTEX_SHADER] = vertexSource;
        shaderSource[GL_FRAGMENT_SHADER] = fragmentSource;
        Compile(shaderSource);
    }
    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }
    void OpenGLShader::Bind() const
    {
        glUseProgram(m_RendererID);
    }
    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }
    void OpenGLShader::UploadUniformInt(const std::string& name, const int value)
    {
        GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1i(location, value);
    }
    void OpenGLShader::UploadUniformFloat(const std::string& name, const float value)
    {
        GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(location, value);
    }
    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
    {
        GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2f(location, value.x, value.y);
    }
    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
    {
        GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }
    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
    {
        GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }
    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
    {
        GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
    {
        GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    void OpenGLShader::Compile(std::unordered_map<GLenum, std::string> shaderSource)
    {
        m_RendererID = glCreateProgram();
        std::vector<GLuint> glShaderIDs;
        glShaderIDs.reserve((shaderSource.size()));
        for (auto& e : shaderSource)
        {
            GLenum type = e.first;
            const std::string& source = e.second;

            GLuint shader = glCreateShader(type);

            const GLchar* sourceCstr = (const GLchar*)source.c_str();
            glShaderSource(shader, 1, &sourceCstr, 0);

            // Compile the shader
            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                // The maxLength includes the NULL character
                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                // We don't need the shader anymore.
                glDeleteShader(shader);

                LB_CORE_ERROR("{0}", infoLog.data());
                LB_CORE_ASSERT(false, "Shader Compilation failure!");
                return;
            }
            glAttachShader(m_RendererID, shader);
            glShaderIDs.push_back(shader);
        }

        // Link our program
        glLinkProgram(m_RendererID);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(m_RendererID);
            // Don't leak shaders either.
            for (auto id : glShaderIDs)
            {
                glDeleteShader(id);
            }

            LB_CORE_ERROR("{0}", infoLog.data());
            LB_CORE_ASSERT(false, "Shader Link failure!");
            return;
        }

        for (auto id : glShaderIDs)
        {
            glDetachShader(m_RendererID, id);
        }
    }
    std::string OpenGLShader::ReadFile(const std::string& filepath)
    {
        std::string result;
        std::ifstream in(filepath, std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        }
        else
        {
            LB_CORE_ERROR("Unable to read file: {0}", filepath);
        }
        return result;
    }
    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& SourceCode)
    {
        std::unordered_map<GLenum, std::string> result;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = SourceCode.find(typeToken, 0);
        while (pos != std::string::npos)
        {
            size_t eol = SourceCode.find_first_of("\r\n", pos);
            LB_CORE_ASSERT(eol != std::string::npos, "Syntax error");
            size_t begin = pos + typeTokenLength + 1;
            std::string type = SourceCode.substr(begin, eol - begin);
            LB_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Unknown shader type!");

            size_t nextLinePos = SourceCode.find_first_not_of("\r\n", eol);
            pos = SourceCode.find(typeToken, nextLinePos);
            result[ShaderTypeFromString(type)] = SourceCode.substr(nextLinePos,
                pos - (nextLinePos == std::string::npos ? SourceCode.size() - 1 : nextLinePos));
        }

        return result;
    }
}
