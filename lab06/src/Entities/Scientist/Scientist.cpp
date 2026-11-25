#include "Entities/Scientist/Scientist.hpp"
#include "Core/Config.hpp"
#include <cmath>
#include "Core/Utils/RandomUtils/RandomUtils.hpp"
#include "Core/Utils/StringUtils/StringUtils.hpp"

using namespace Config;
using namespace StringUtils;

Scientist::Scientist(sf::Vector2f position)
    : Host(Config::SCIENTIST_MAX_HEALTH, position, Config::SCIENTIST_DECAY_TIME)
{
    m_accel     = Config::SCIENTIST_ACCEL;
    m_friction  = Config::SCIENTIST_FRICTION;
    m_jumpForce = Config::SCIENTIST_JUMP_FORCE;
    m_gravity   = Config::SCIENTIST_GRAVITY;
    m_maxSpeed  = Config::SCIENTIST_MAX_SPEED;
    m_stepSoundInterval = Config::HOST_STEP_SOUND_INTERVAL;

    InitSprite("../assets/animations/scientist.png",
        Config::SCIENTIST_FRAME_W,
        Config::SCIENTIST_FRAME_H,
        Config::SCIENTIST_SCALE);
        
    InitHitboxFromCenter(Config::SCIENTIST_HITBOX_LEFT,
                        Config::SCIENTIST_HITBOX_RIGHT,
                        Config::SCIENTIST_HITBOX_TOP,
                        Config::SCIENTIST_HITBOX_BOTTOM,
                        position);
        
    m_sprite.setPosition(position);

    m_anim.Setup(m_texture,
                 Config::SCIENTIST_FRAME_W,
                 Config::SCIENTIST_FRAME_H);

    m_anim.AddClip("idle", 3, Config::SCIENTIST_IDLE_FRAME_TIME);
    m_anim.AddClip("walk", 3, Config::SCIENTIST_WALK_FRAME_TIME);

    m_properties.SetSfString("name", ToSfStringFromUtf8("Учёный"));
    m_properties.SetInt("access_level", 1);

    m_idleTimer = RandomUtils::Float(Config::SCIENTIST_IDLE_TIME_MIN, Config::SCIENTIST_IDLE_TIME_MAX);

    m_stateManager.SetState(EntityState::Idle);
}

void Scientist::UpdateAI(const GameContext& ctx)
{
    UpdateVertical(ctx, false);

    if (m_isWalking)
    {
        if (IsBlockedAhead(ctx, m_moveDir))
        {
            m_velocity.x = 0.f;
            m_isWalking = false;
            m_idleTimer = RandomUtils::Float(Config::SCIENTIST_IDLE_TIME_MIN, Config::SCIENTIST_IDLE_TIME_MAX);
            return;
        }

        m_velocity.x = static_cast<float>(m_moveDir) * Config::SCIENTIST_PATROL_SPEED;
        SetLookDir(m_moveDir);

        m_walkTimer -= ctx.dt;
        if (m_walkTimer <= 0.f)
        {
            m_velocity.x = 0.f;
            m_isWalking = false;
            m_idleTimer = RandomUtils::Float(Config::SCIENTIST_IDLE_TIME_MIN, Config::SCIENTIST_IDLE_TIME_MAX);
        }
        return;
    }

    m_velocity.x = 0.f;
    m_idleTimer -= ctx.dt;

    if (m_idleTimer <= 0.f)
    {
        StartWalking(ctx);
    }
}

void Scientist::StartWalking(const GameContext& ctx)
{
    int dir = RandomUtils::Int(0, 1) == 0 ? -1 : 1;

    if (IsBlockedAhead(ctx, dir))
    {
        dir = -dir;
    }

    if (IsBlockedAhead(ctx, dir))
    {
        m_idleTimer = RandomUtils::Float(Config::SCIENTIST_IDLE_TIME_MIN, Config::SCIENTIST_IDLE_TIME_MAX);
        return;
    }

    m_moveDir = dir;
    m_isWalking = true;
    m_walkTimer = RandomUtils::Float(Config::SCIENTIST_WALK_TIME_MIN, Config::SCIENTIST_WALK_TIME_MAX);
}

bool Scientist::IsBlockedAhead(const GameContext& ctx, int dir) const
{
    const float tileSize = static_cast<float>(ctx.levelMap->GetTileSize());
    sf::FloatRect bounds = GetBounds();

    auto isSolidAt = [&](float x, float y)
    {
        int tx = static_cast<int>(std::floor(x / tileSize));
        int ty = static_cast<int>(std::floor(y / tileSize));
        return ctx.levelMap->IsSolid(tx, ty, Config::EntityType::Human);
    };

    float aheadX = dir > 0 ? bounds.left + bounds.width + 4.f
                           : bounds.left - 4.f;
    bool wallAhead   = isSolidAt(aheadX, bounds.top + bounds.height * 0.5f);
    bool groundAhead = isSolidAt(aheadX, bounds.top + bounds.height + 4.f);

    return wallAhead || !groundAhead;
}