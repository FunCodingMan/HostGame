#pragma once
#include <SFML/Graphics.hpp>

class Parasite
{
    public:
        Parasite(sf::Vector2f position);
        void update(float dt);
        void draw(sf::RenderWindow& window);
    private:
        void updateHorizontal(float dt);
        void updateVertical(float dt);
        void checkXBoundaries();
        void resolveCollisions();
        void resolveFriction(float dt);
        void checkMaxSpeed();
        bool checkMoveKeys(float dt);

        sf::RectangleShape shape;
        sf::Vector2f velocity;

        float speed;
        float jumpForce;
        float gravity;
        float accel;
        float friction;
        bool isOnGround;
};