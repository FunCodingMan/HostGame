#include "Entities/Parasite/Parasite.hpp"
#include "Core/Config.hpp"
#include "Core/GameKeyboard/GameKeyboard.hpp"

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

    constexpr int FW = Config::PARASITE_FRAME_W;
    constexpr int FH = Config::PARASITE_FRAME_H;

    m_anim.AddClip("idle",
        {
            sf::IntRect(0,    0, FW, FH),
            sf::IntRect(FW,   0, FW, FH),
            sf::IntRect(2*FW, 0, FW, FH),
            sf::IntRect(3*FW, 0, FW, FH),
        },
        Config::PARASITE_IDLE_FRAME_TIME,
        true);

    m_anim.Play("idle");
    m_anim.Apply(m_sprite);
}

bool Parasite::IsDashing() const
{
    return m_dashTimer > 0.f;
}

void Parasite::ForceDash(sf::Vector2f targetPos)
{
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
    m_dashTimer    = Config::DASH_DURATION;
    m_hasDash      = false;
    m_isOnGround   = false;
}

void Parasite::UpdateDash(float dt, sf::Vector2f mousePos)
{
    if (m_dashCooldown > 0.f)
    {
        m_dashCooldown -= dt;
    }

    if (GameKeyboard::IsDashKeyPressed() && m_dashCooldown <= 0.f && m_hasDash)
    {
        ForceDash(mousePos);
    }
}

void Parasite::UpdateDashTimer(float dt)
{
    if (m_dashTimer > 0.f)
    {
        m_dashTimer -= dt;

        if (m_dashTimer <= 0.f)
        {
            m_velocity.x *= 0.5f;
            m_velocity.y *= 0.5f;
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

void Parasite::UpdateHorizontal(float dt)
{
    if (std::abs(m_velocity.x) > m_maxSpeed)
    {
        HorizontalDash(dt);
    }
    else
    {
        Character::UpdateHorizontal(dt);
    }
}

void Parasite::UpdateMove(float dt)
{
    UpdateDashTimer(dt);

    if (m_dashTimer <= 0.f)
    {
        UpdateHorizontal(dt);
        UpdateVertical(dt, true);
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
    return true;
}

void Parasite::Update(const GameContext& ctx)
{
    if (!m_isPlayerControlled)
    {
        return;
    }

    UpdateDash(ctx.dt, ctx.mousePos);
    UpdateMove(ctx.dt);
    ApplyMovement(ctx);
}