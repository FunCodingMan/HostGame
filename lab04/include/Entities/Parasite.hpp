#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class Parasite
{
    public:
        Parasite(sf::Vector2f position);
        void Update(float dt, sf::Vector2f mousePos);
        void Draw(sf::RenderWindow& window);
        sf::FloatRect GetBounds() const;
    private:
        void updateHorizontal(float dt);
        void UpdateVertical(float dt);
        void CheckXBoundaries();
        void ResolveCollisions();
        void ResolveFriction(float dt);
        void CheckMaxSpeed();   
        bool CheckMoveKeys(float dt);
        bool IsJumpKeyPressed();
        bool IsLeftKeyPressed();
        bool IsRightKeyPressed();
        bool IsDashKeyPressed();
        void UpdateDash(float dt, sf::Vector2f mousePos);
        void UpdateMove(float dt);
        void HorizontalDash(float dt);
        void HorizontalCommon(float dt);
        void UpdateDashTimer(float dt);
        void moveEye(sf::Vector2f mousePos);

        sf::RectangleShape body;
        sf::RectangleShape eye;
        sf::RectangleShape pupil;
        sf::Vector2f velocity;

        float speed;
        float m_jumpForce;
        float m_gravity;
        float m_accel;
        float m_friction;
        float m_dashCooldown;
        float m_dashForce;
        bool m_hasDash;
        float m_dashTimer;
        bool m_isOnGround;
};