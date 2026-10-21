#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

class MenuButton
{
public:
    MenuButton(const sf::String& text, const sf::Font& font, float yPos, float windowWidth, std::function<void()> action);

    void OnClick();
    void SetSelected(bool isSelected);
    void Draw(sf::RenderWindow& window);
private:
    sf::Text m_text;
    std::function<void()> m_action;
};