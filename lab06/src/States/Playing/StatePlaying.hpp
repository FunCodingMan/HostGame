#pragma once
#include <SFML/Graphics.hpp>
#include "States/State.hpp"
#include "Core/GameData.hpp"
#include "Core/LevelLogic/Level/Level.hpp"
#include "UI/EntityHUD/EntityHUD.hpp"
#include "UI/ScreenHUD/ScreenHUD.hpp"
#include "Core/Tutorial/TutorialOverlay/TutorialOverlay.hpp"

class StatePlaying : public State
{
public:
    StatePlaying(GameDataRef data);

    void Init() override;
    void HandleInput(const sf::Event& event) override;
    void Update(float dt) override;
    void Draw() override;

private:
    void UpdateHUD();
    void HandleLevelResult();

    GameDataRef m_data;

    Level m_level;
    EntityHUD m_floatingHud;
    ScreenHUD m_screenHud;
    TutorialOverlay m_tutorial;

    bool m_endStatePushed = false;
};