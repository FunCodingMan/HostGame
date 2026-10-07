#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Entities/Entity/Entity.hpp"

class Parasite : public Entity
{
    public:
        Parasite(sf::Vector2f position);
        void Update(const GameContext& ctx) override;
    private:
        void UpdateHorizontal(float dt);
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
        void UpdateSpriteDirection();

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