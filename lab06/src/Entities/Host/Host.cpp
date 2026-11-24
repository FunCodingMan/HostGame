#include "Entities/Host/Host.hpp"
#include "Core/Config.hpp"
#include <cmath>

using namespace Config;

Host::Host(float maxLife, sf::Vector2f position, float decayTime)
    : Character(maxLife, position)
{
    m_maxDecayTimer = decayTime;
    m_decayTimer    = decayTime;
}

void Host::Update(const GameContext& ctx)
{
    if (m_isDead)
    {
        m_corpseTimer += ctx.dt;
        m_velocity.x = 0.f;
        UpdateVertical(ctx, false);
        ApplyMovement(ctx);
        return;
    }

    m_stateManager.Update(ctx.dt);

    if (m_isPlayerControlled)
    {
        m_decayTimer -= ctx.dt;
        UpdateControlled(ctx);
    }
    else
    {
        UpdateAI(ctx);
    }

    UpdateMovementState();
    ApplyMovement(ctx);
}

void Host::UpdateControlled(const GameContext& ctx)
{
    UpdateMove(ctx);
}

void Host::UpdateAI(const GameContext& ctx)
{
    UpdateVertical(ctx, false);
}

void Host::UpdateMovementState()
{
    EntityState state = m_stateManager.GetState();

    if (state == EntityState::Dead ||
        state == EntityState::Alert ||
        state == EntityState::Attack ||
        state == EntityState::Reload ||
        state == EntityState::Report)
    {
        return;
    }

    if (m_isOnGround)
    {
        if (std::abs(m_velocity.x) > Config::HOST_WALK_THRESHOLD)
        {
            m_stateManager.SetState(EntityState::Walk);
        }
        else
        {
            m_stateManager.SetState(EntityState::Idle);
        }
    }
    else
    {
        m_stateManager.SetState(m_velocity.y < 0.f ? EntityState::Jump : EntityState::Fall);
    }
}

void Host::Die()
{
    LivingEntity::Die();
    m_isPlayerControlled = false;
    m_velocity = sf::Vector2f(0.f, 0.f);
    m_corpseTimer = 0.f;
    m_stateManager.SetState(EntityState::Dead);
}

void Host::OnPossessed()
{
    m_velocity = sf::Vector2f(0.f, 0.f);
    m_stateManager.SetState(EntityState::Idle);
}

void Host::OnEjected()
{
    if (m_isDead)
    {
        return;
    }

    m_velocity.x = 0.f;
    m_stateManager.SetState(EntityState::Idle);
}

void Host::Draw(sf::RenderWindow& window)
{
    if (!m_isDead)
    {
        Entity::Draw(window);
        return;
    }

    sf::Sprite corpse = m_sprite;
    corpse.setColor(sf::Color(130, 130, 130, 220));
    corpse.setRotation(90.f);

    sf::FloatRect corpseBounds = corpse.getGlobalBounds();
    sf::FloatRect bodyBounds   = m_hitbox.getGlobalBounds();
    corpse.move(0.f, (bodyBounds.top + bodyBounds.height) -
                     (corpseBounds.top + corpseBounds.height));

    window.draw(corpse);

    if (Entity::AreHitboxesVisible())
    {
        window.draw(m_hitbox);
    }
}

void Host::AddDecayTime(float time)
{
    m_decayTimer += time;
    if (m_decayTimer > m_maxDecayTimer)
    {
        m_decayTimer = m_maxDecayTimer;
    }
}
