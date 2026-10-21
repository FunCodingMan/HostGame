#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Entities/LivingEntity/LivingEntity.hpp"
#include "Core/Map/Map.hpp"

class Parasite : public LivingEntity
{
    public:
        Parasite(sf::Vector2f position);
        void Update(const GameContext& ctx) override;
        bool IsDashing() const;
        void ForceDash(sf::Vector2f targetPos);
    private:
        void UpdateHorizontal(float dt);
        void UpdateVertical(float dt);
        void CheckXBoundaries();
        enum class Axis { Horizontal, Vertical };
        void ResolveCollisions(const GameContext& ctx, Axis axis);
        void ResolveCollisionsX(int x, const GameContext& ctx);
        void ResolveCollisionsY(int y, const GameContext& ctx);
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