#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include "States/UI/MenuButton/MenuButton.hpp"
#include "Core/Sound/SoundManager/SoundManager.hpp"

class UIMenu
{
public:
    UIMenu();

    void AddButton(const sf::String& text, const sf::Font& font, float yPos, float windowWidth, std::function<void()> onClick);    
    void HandleInput(const sf::Event& event, SoundManager& sound);
    void Update();
    void Draw(sf::RenderWindow& window);

private:
    void MenuUp();
    void MenuDown();

    std::vector<MenuButton> m_buttons;
    int m_selectedOption;
};