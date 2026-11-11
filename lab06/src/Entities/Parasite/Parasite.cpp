#include "Entities/Parasite/Parasite.hpp"
#include "Core/Config.hpp"
#include "Core/GameKeyboard/GameKeyboard.hpp"
#include "Core/Config.hpp"
#include "Core/Utils/StringUtils/StringUtils.hpp"

using namespace Config;
using namespace StringUtils;

Parasite::Parasite(sf::Vector2f position)
    : Character(Config::PARASITE_MAX_HEALTH, position)
{
    m_accel     = Config::PARASITE_ACCEL;
    m_friction  = Config::PARASITE_FRICTION;
    m_jumpForce = Config::PARASITE_JUMP_FORCE;
    m_gravity   = Config::PARASITE_GRAVITY;
    m_maxSpeed  = Config::PARASITE_MAX_SPEED;

    InitHitbox(Config::PARASITE_HITBOX_WIDTH,
               Config::PARASITE_HITBOX_HEIGHT,
               position);
    InitSprite("../assets/animations/parasite.png",
               Config::PARASITE_FRAME_W,
               Config::PARASITE_FRAME_H,
               Config::PARASITE_SCALE);

    m_sprite.setPosition(position);

    m_anim.Setup(m_texture,
                 Config::PARASITE_FRAME_W,
                 Config::PARASITE_FRAME_H);

    m_anim.AddClip("idle", 5, Config::PARASITE_IDLE_FRAME_TIME);
    m_anim.AddClip("dash", 3, Config::PARASITE_DASH_FRAME_TIME); 

    m_anim.Play("idle");
    m_anim.Apply(m_sprite);

    GetProperties().SetSfString("name", ToSfStringFromUtf8("Паразит"));
    m_properties.SetInt("access_level", 0);
}

bool Parasite::IsDashing() const
{
    return m_stateManager.GetState() == Config::EntityState::Dash;
}

void Parasite::ForceDash(const GameContext& ctx)
{
    ctx.audio->PlaySound(Config::SOUND_ID_DASH);
    sf::Vector2f targetPos = ctx.mousePos;

    sf::Vector2f center = m_hitbox.getPosition();

    float dx = targetPos.x - center.x;
    float dy = targetPos.y - center.y;
    float length = std::sqrt(dx * dx + dy * dy);

    if (length == 0.f)
    {
        return;
    }

    m_velocity.x = (dx / length) * m_dashForce;
    m_velocity.y = (dy / length) * m_dashForce;

    m_dashCooldown = Config::DASH_COOLDOWN_TIME;
    m_hasDash      = false;
    m_isOnGround   = false;

    m_stateManager.SetState(EntityState::Dash);
}

void Parasite::UpdateDash(const GameContext& ctx)
{
    if (m_dashCooldown > 0.f)
    {
        m_dashCooldown -= ctx.dt;
    }

    if (GameKeyboard::IsDashKeyPressed() && m_dashCooldown <= 0.f && m_hasDash)
    {
        ForceDash(ctx);
    }
}

void Parasite::UpdateDashTimer(float dt)
{
    if (m_stateManager.GetState() == EntityState::Dash)
    {
        if (m_stateManager.GetTimeInState() >= Config::DASH_DURATION)
        {
            m_velocity.x *= 0.5f;
            m_velocity.y *= 0.5f;
            
            m_stateManager.SetState(EntityState::Idle);
        }
    }
}

void Parasite::HorizontalDash(float dt)
{
    float drag = m_friction * 3.f;

    if (m_velocity.x > 0.f)
    {
        m_velocity.x -= drag * dt;
        m_velocity.x = std::max(m_velocity.x, Config::PARASITE_MAX_SPEED);
    }
    else
    {
        m_velocity.x += drag * dt;
        m_velocity.x = std::min(m_velocity.x, -Config::PARASITE_MAX_SPEED);
    }
}

void Parasite::UpdateHorizontal(const GameContext& ctx)
{
    if (std::abs(m_velocity.x) > m_maxSpeed) {
        HorizontalDash(ctx.dt);
    } else {
        Character::UpdateHorizontal(ctx);
    }
}
void Parasite::UpdateMove(const GameContext& ctx)
{
    m_stateManager.Update(ctx.dt);
    UpdateDashTimer(ctx.dt);

    if (m_stateManager.GetState() != EntityState::Dash) {
        UpdateHorizontal(ctx);
        UpdateVertical(ctx, true);
    }
}

bool Parasite::TryBreakTile(int x, int y, const GameContext& ctx)
{
    if (!IsDashing())
    {
        return false;
    }

    if (!ctx.levelMap->IsBreakable(x, y))
    {
        return false;
    }

    ctx.levelMap->BreakTile(x, y);
    ctx.audio->PlaySound(SOUND_ID_BREAK_GLASS);
    return true;
}

void Parasite::Update(const GameContext& ctx)
{
    if (!m_isPlayerControlled)
    {
        return;
    }

    UpdateDash(ctx);
    UpdateMove(ctx);
    ApplyMovement(ctx);
}