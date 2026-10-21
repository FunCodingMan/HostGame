#pragma once
#include <SFML/Graphics.hpp>
#include "States/UI/UIMenu/UIMenu.hpp"
#include "States/UI/MenuLabel/MenuLabel.hpp"
#include "States/State.hpp"
#include "Core/GameData.hpp"

class StateMainMenu : public State
{
public:
    StateMainMenu(GameDataRef data);

    void Init() override;
    void HandleInput(const sf::Event& event) override;
    void Update(float dt) override;
    void Draw() override;

private:
    GameDataRef m_data;
    
    MenuLabel m_title;
    
    UIMenu m_menu;
};