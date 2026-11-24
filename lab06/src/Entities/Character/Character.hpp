#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Entities/LivingEntity/LivingEntity.hpp"                             
#include "Core/Animation/AnimationController.hpp"
#include "Core/Config.hpp"
#include "Core/Map/Map.hpp"
#include "Entities/StateManager/EntityStateManager/EntityStateManager.hpp"

class Character : public LivingEntity
{
public:
    Character(float maxLife, sf::Vector2f position);
    virtual ~Character() = default;
    EntityStateManager& GetStateManager() { return m_stateManager; }
    const EntityStateManager& GetStateManager() const { return m_stateManager; }

    int  GetLookDir() const { return m_lookDir; }
    void SetLookDir(int dir) { m_lookDir = (dir < 0) ? -1 : 1; }

    virtual const char* GetDeathSound() const { return Config::SOUND_ID_DEATH; }

protected:
    virtual const char* GetJumpSound() const { return nullptr; }
    virtual const char* GetLandSound() const { return nullptr; }
    virtual const char* GetWalkSound() const { return nullptr; }
    enum class Axis { Horizontal, Vertical };

    void InitHitbox(float width, float height, sf::Vector2f position);
    void InitHitboxFromCenter(float left, float right, float top, float bottom, sf::Vector2f position);
    bool InitSprite(const std::string& texturePath, int frameW, int frameH, float scale);

    virtual void UpdateHorizontal(const GameContext& ctx);
    virtual void UpdateVertical(const GameContext& ctx, bool allowInput);
    virtual void UpdateMove(const GameContext& ctx);

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
    virtual void OnLanding() {}
    virtual void OnJump()    {}
    virtual bool TryBreakTile(int x, int y, const GameContext& ctx) { return false; }
    virtual bool TryOpenDoor(int x, int y, const GameContext& ctx);

    float m_accel     = 0.f;
    float m_friction  = 0.f;
    float m_jumpForce = 0.f;
    float m_gravity   = 0.f;
    float m_maxSpeed  = 0.f;
    float m_scale     = 1.f;
    bool  m_isOnGround = false;
    int   m_lookDir   = 1;
    sf::Vector2f m_spriteOffset{0.f, 0.f};

    float m_stepTimer = 0.f;
    float m_stepSoundInterval = Config::STEP_SOUND_INTERVAL;

    AnimationController m_anim;

    EntityStateManager m_stateManager;
};