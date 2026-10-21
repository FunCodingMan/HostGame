#pragma once
#include <SFML/Graphics.hpp>
#include "States/UI/UIMenu/UIMenu.hpp"
#include "States/UI/MenuLabel/MenuLabel.hpp"
#include "States/State.hpp"
#include "Core/GameData.hpp"

class StatePause : public State
{
public:
    StatePause(GameDataRef data);

    void Init() override;
    void HandleInput(const sf::Event& event) override;
    void Update(float dt) override;
    void Draw() override;

private:
    GameDataRef m_data;

    sf::Texture m_backgroundTexture;
    sf::Sprite m_backgroundSprite;
    sf::RectangleShape m_darkOverlay;

    MenuLabel m_title;
    UIMenu m_menu;
};