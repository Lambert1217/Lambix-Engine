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
        Ref<Shader> FlatColorShader;
        Ref<Shader> Texture2DShader;
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

        s_Data->FlatColorShader = OpenGLShader::Create("assets/shaders/FlatColor.glsl");

        s_Data->Texture2DShader = OpenGLShader::Create("assets/shaders/Texture2D.glsl");
        s_Data->Texture2DShader->Bind();
        s_Data->Texture2DShader->SetInt("u_Texture", 0);
    }

    void Renderer2D::Shutdown()
    {
        delete s_Data;
    }

    void Renderer2D::BeginScene(const OrthoCamera& camera)
    {
        s_Data->FlatColorShader->Bind();
        s_Data->FlatColorShader->SetMat4("aViewProjection", camera.GetViewProjectionMatrix());

        s_Data->Texture2DShader->Bind();
        s_Data->Texture2DShader->SetMat4("aViewProjection", camera.GetViewProjectionMatrix());
    }

    void Renderer2D::EndScene()
    {}

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        s_Data->FlatColorShader->Bind();
        s_Data->FlatColorShader->SetFloat4("u_Color", color);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
        transform *= glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_Data->FlatColorShader->SetMat4("aTransform", transform);

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture)
    {
        s_Data->Texture2DShader->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
        transform *= glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_Data->FlatColorShader->SetMat4("aTransform", transform);

        texture->Bind();

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }
}
