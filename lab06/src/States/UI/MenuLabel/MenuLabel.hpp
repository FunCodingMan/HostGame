#pragma once
#include <SFML/Graphics.hpp>

class MenuLabel
{
public:
    void Init(const sf::String& text, const sf::Font& font, int size, float yPos, float windowWidth);
    void Draw(sf::RenderWindow& window);

private:
    sf::Text m_text;
};