#include "GameLayer.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

GameLayer::GameLayer(uint32_t width, uint32_t height)
    : Lambix::Layer("GameLayer"), m_Camera(-1.0f, 1.0f, -1.0f, 1.0f)
{}

void GameLayer::OnAttach()
{
    for (int i = 0;i < 11;++i)
    {
        std::string path = "assets/textures/block_" + std::to_string((int)pow(2, i + 1)) + ".png";
        m_Textures[i] = Lambix::Texture2D::Create(path);
    }
    m_DefeatTexture = Lambix::Texture2D::Create("assets/textures/defeat.png");
    m_VictoryTexture = Lambix::Texture2D::Create("assets/textures/victory.png");
}

void GameLayer::OnDetach() {}

void GameLayer::OnUpdate(Lambix::Timestep ts)
{
    {
        Lambix::RenderCommand::SetClearColor(glm::vec4(1.0f, 0.8f, 0.78f, 1.0f));
        Lambix::RenderCommand::Clear();
    }

    {
        Lambix::Renderer2D::BeginScene(m_Camera);
        // 棋盘
        Lambix::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.2 }, { 1 - INTERVAL, 1 - INTERVAL }, glm::radians(0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.3f));
        // 棋盘格子
        float size = (2 - INTERVAL * (4 + 5)) / 4;
        for (int x = 0;x < 4;++x)
        {
            for (int y = 0;y < 4;++y)
            {
                // 格子位置
                glm::vec2 position((INTERVAL + size) * (x - 1.5f), (INTERVAL + size) * (y - 1.5f));

                // 格子背景
                Lambix::Renderer2D::DrawQuad({ position.x, position.y, -0.1f }, { size / 2, size / 2 }, glm::radians(0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 0.4f));

                // 数字
                int value = m_GameControl.GetData(x, y);
                if (value != 0)
                {
                    Lambix::Renderer2D::DrawQuad({ position.x, position.y }, { (size - INTERVAL) / 2, (size - INTERVAL) / 2 }, glm::radians(0.0f), m_Textures[(int)std::log2(value) - 1]);
                }
            }
        }
        if (m_GameControl.GetGameStatus() == GameControl::Defeat)
        {
            Lambix::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.1f }, { 1.0f, 1.0f }, glm::radians(0.0f),
                m_DefeatTexture, 1.0f, glm::vec4(1.0f, 0.0f, 0.0f, 0.8f));
        }
        if (m_GameControl.GetGameStatus() == GameControl::Victory)
        {
            Lambix::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.1f }, { 1.0f, 1.0f }, glm::radians(0.0f),
                m_VictoryTexture, 1.0f, glm::vec4(1.0f, 1.0f, 0.0f, 0.8f));
        }

        Lambix::Renderer2D::EndScene();
    }
}

void GameLayer::OnEvent(Lambix::Event& event)
{
    m_GameControl.OnEvent(event);
}
