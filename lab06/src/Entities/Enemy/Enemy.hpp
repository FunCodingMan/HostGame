#pragma once
#include <SFML/Graphics.hpp>

class Enemy
{
public:
    Enemy(sf::Vector2f startPos);
    void update(float dt);
    void draw(sf::RenderWindow& window);


    sf::FloatRect getBounds() const;
    
    void onCollision();

private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    
    float flashTimer;
};