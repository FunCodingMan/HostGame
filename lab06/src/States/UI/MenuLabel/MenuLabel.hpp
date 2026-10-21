#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class MenuLabel
{
public:
    MenuLabel() = default;

    void Init(const std::string& textStr, const sf::Font& font, unsigned int charSize, float yPos, float windowWidth);
    void Draw(sf::RenderWindow& window);

private:
    sf::Text m_text;
};