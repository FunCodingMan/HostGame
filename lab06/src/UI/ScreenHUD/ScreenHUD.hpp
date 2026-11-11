#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class ScreenHUD
{
public:
    void Init(const sf::Font& font, float windowWidth);
    
    void SetHintText(const sf::String& text);
    
    void Draw(sf::RenderWindow& window);

private:
    sf::Text m_hintText;
};