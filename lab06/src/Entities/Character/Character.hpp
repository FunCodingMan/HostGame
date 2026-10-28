#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Entities/LivingEntity/LivingEntity.hpp"
#include "Core/Animation/AnimationController.hpp"
#include "Core/Map/Map.hpp"

class Character : public LivingEntity
{
public:
    Character(float maxLife, sf::Vector2f position);
    virtual ~Character() = default;

protected:
    enum class Axis { Horizontal, Vertical };

    void InitHitbox(float width, float height, sf::Vector2f position);
    void InitSprite(const std::string& texturePath, int frameW, int frameH, float scale);

    virtual void UpdateHorizontal(float dt);
    virtual void UpdateVertical(float dt, bool allowInput);
    virtual void UpdateMove(float dt);

    bool CheckMoveKeys(float dt);
    void ResolveFriction(float dt);
    void CheckMaxSpeed();

    void ResolveCollisions(const GameContext& ctx, Axis axis);
    void ResolveCollisionsX(int x, const GameContext& ctx);
    void ResolveCollisionsY(int y, const GameContext& ctx);

    void UpdateSpriteDirection();
    void UpdateAnimation(float dt);
    void ApplyMovement(const GameContext& ctx);

    virtual Config::EntityType GetEntityType() const = 0;
    virtual const char* GetAnimationName() const = 0;
    virtual void OnLanding() {}
    virtual bool TryBreakTile(int x, int y, const GameContext& ctx) { return false; }

    float m_accel     = 0.f;
    float m_friction  = 0.f;
    float m_jumpForce = 0.f;
    float m_gravity   = 0.f;
    float m_maxSpeed  = 0.f;
    float m_scale     = 1.f;
    bool  m_isOnGround = false;

    AnimationController m_anim;
};