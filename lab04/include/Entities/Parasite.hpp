#pragma once
#include <SFML/Graphics.hpp>

class Parasite
{
    public:
        Parasite(sf::Vector2f position);
        void update(float dt);
        void draw(sf::RenderWindow& window);
    private:
        sf::RectangleShape shape;
        sf::Vector2f velocity;

        float speed;
        float jumpForce;
        float gravity;
        float accel;
        float friction;
        bool isOnGround;
};