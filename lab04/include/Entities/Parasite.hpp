#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class Parasite
{
    public:
        Parasite(sf::Vector2f position);
        void update(float dt, sf::Vector2f mousePos);
        void draw(sf::RenderWindow& window);
    private:
        void updateHorizontal(float dt);
        void updateVertical(float dt);
        void checkXBoundaries();
        void resolveCollisions();
        void resolveFriction(float dt);
        void checkMaxSpeed();
        bool checkMoveKeys(float dt);
        bool isJumpKeyPressed();
        bool isLeftKeyPressed();
        bool isRightKeyPressed();
        bool isDashKeyPressed();
        void updateDash(float dt, sf::Vector2f mousePos);
        void updateMove(float dt);
        void horizontalDash(float dt);
        void horizontalCommon(float dt);
        void updateDashTimer(float dt);

        sf::RectangleShape shape;
        sf::Vector2f velocity;

        float speed;
        float jumpForce;
        float gravity;
        float accel;
        float friction;
        float dashCooldown;
        float dashForce;
        bool hasDash;
        float dashTimer;
        bool isOnGround;
};