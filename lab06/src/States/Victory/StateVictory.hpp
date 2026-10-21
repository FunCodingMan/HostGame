#pragma once
#include <SFML/Graphics.hpp>
#include "States/UI/UIMenu/UIMenu.hpp"
#include "States/UI/MenuLabel/MenuLabel.hpp"
#include "States/State.hpp"
#include "Core/GameData.hpp"

class StateVictory : public State
{
public:
    StateVictory(GameDataRef data);

    void Init() override;
    void HandleInput(const sf::Event& event) override;
    void Update(float dt) override;
    void Draw() override;

private:
    GameDataRef m_data;

    sf::RectangleShape m_darkOverlay;
    MenuLabel m_title;
    UIMenu m_menu;
};