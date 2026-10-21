#pragma once
#include <SFML/Graphics.hpp>

class EntityHUD
{
public:
    EntityHUD();

    void Init(); 

    void SetPosition(sf::Vector2f entityPosition);

    void Update(float currentValue, float maxValue, sf::Color barColor);
    void Draw(sf::RenderWindow& window);

private:
    sf::RectangleShape m_bgBar; 
    sf::RectangleShape m_fgBar; 
    
    float m_maxWidth;
};