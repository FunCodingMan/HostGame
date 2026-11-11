#pragma once
#include <SFML/Graphics.hpp>

class Enemy
{
public:
    Enemy(sf::Vector2f startPos);
    void Update(float dt);
    void Draw(sf::RenderWindow& window);


    sf::FloatRect GetBounds() const;
    
    void OnCollision();

private:
    sf::RectangleShape m_shape;
    sf::Vector2f m_velocity;
    
    float m_flashTimer;
};