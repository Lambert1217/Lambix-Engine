#pragma once
#include <Lambix.h>
#include "GameControl.h"

class GameLayer : public Lambix::Layer
{
public:
    GameLayer(uint32_t width, uint32_t height);
    ~GameLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(Lambix::Timestep ts) override;
    virtual void OnEvent(Lambix::Event& event) override;

private:
    Lambix::OrthoCamera m_Camera;
    GameControl m_GameControl;
    Lambix::Ref<Lambix::Texture2D> m_Textures[11];
    Lambix::Ref<Lambix::Texture2D> m_DefeatTexture;
    Lambix::Ref<Lambix::Texture2D> m_VictoryTexture;


    const float INTERVAL{ 0.025f };
};