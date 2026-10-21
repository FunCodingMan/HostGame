#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>
#include "States/UI/MenuButton/MenuButton.hpp"

class UIMenu
{
public:
    UIMenu();

    void AddButton(const std::string& textStr, const sf::Font& font, float yPos, float windowWidth, std::function<void()> action);
    
    void HandleInput(const sf::Event& event);
    void Update();
    void Draw(sf::RenderWindow& window);

private:
    void MenuUp();
    void MenuDown();

    std::vector<MenuButton> m_buttons;
    int m_selectedOption;
};