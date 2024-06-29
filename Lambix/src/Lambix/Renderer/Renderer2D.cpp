#include "Renderer2D.h"
#include "Lambix/Core/Core.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "glm/gtc/matrix_transform.hpp"

#include "platform/OpenGL/OpenGLShader.h"

namespace Lambix
{
    struct Renderer2DStorage
    {
        Ref<VertexArray> QuadVertexArray;
        Ref<Shader> Texture2DShader;
        Ref<Texture2D> WhiteTexture;
    };

    static Renderer2DStorage* s_Data;

    void Renderer2D::Init()
    {
        s_Data = new Renderer2DStorage();
        s_Data->QuadVertexArray = VertexArray::Create();

        float vertices[] = {
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f };
        Ref<VertexBuffer> QuadVertexBuffer;
        QuadVertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
        BufferLayout layout = {
            {"aPos", ShaderDataType::Float3},
            {"aTexCrood", ShaderDataType::Float2}
        };
        QuadVertexBuffer->SetLayout(layout);
        s_Data->QuadVertexArray->AddVertexBuffer(QuadVertexBuffer);

        uint32_t indices[] = { 0, 1, 2, 1, 2, 3 };
        Ref<IndexBuffer> m_IndexBuffer;
        m_IndexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
        s_Data->QuadVertexArray->SetIndexBuffer(m_IndexBuffer);

        s_Data->Texture2DShader = OpenGLShader::Create("assets/shaders/Texture2D.glsl");
        s_Data->Texture2DShader->Bind();
        s_Data->Texture2DShader->SetInt("u_Texture", 0);

        s_Data->WhiteTexture = Texture2D::Create(1, 1);
        uint32_t WhiteTextureData = 0xffffffff;
        s_Data->WhiteTexture->SetData(&WhiteTextureData, sizeof(uint32_t));
    }

    void Renderer2D::Shutdown()
    {
        delete s_Data;
    }

    void Renderer2D::BeginScene(const OrthoCamera& camera)
    {
        s_Data->Texture2DShader->Bind();
        s_Data->Texture2DShader->SetMat4("aViewProjection", camera.GetViewProjectionMatrix());
    }

    void Renderer2D::EndScene()
    {}

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, rotation, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
    {
        s_Data->Texture2DShader->Bind();
        s_Data->Texture2DShader->SetFloat4("u_Color", color);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
        transform *= glm::rotate(glm::mat4(1.0f), rotation, { 0.0f,0.0f,1.0f });
        transform *= glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_Data->Texture2DShader->SetMat4("aTransform", transform);

        s_Data->WhiteTexture->Bind();

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        s_Data->Texture2DShader->Bind();
        s_Data->Texture2DShader->SetFloat4("u_Color", tintColor);
        s_Data->Texture2DShader->SetFloat("u_tilingFactor", tilingFactor);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
        transform *= glm::rotate(glm::mat4(1.0f), rotation, { 0.0f,0.0f,1.0f });
        transform *= glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_Data->Texture2DShader->SetMat4("aTransform", transform);

        texture->Bind();

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }
}
