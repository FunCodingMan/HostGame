#include "Entities/Enemy/Enemy.hpp"
#include "Core/Config.hpp"

Enemy::Enemy(sf::Vector2f position) : LivingEntity(50.f)
{
    m_hitbox.setSize(sf::Vector2f(40.f, 40.f));
    m_hitbox.setOrigin(20.f, 20.f);
    m_hitbox.setPosition(position);
    m_hitbox.setFillColor(sf::Color::Yellow);
    m_hitbox.setOutlineColor(sf::Color::Red);
    m_hitbox.setOutlineThickness(1.f);
}

void Enemy::Draw(sf::RenderWindow& window)
{
    window.draw(m_hitbox); 
}

void Enemy::Update(const GameContext& ctx)
{
    if (m_isDead) return;

    if (m_isPlayerControlled)
    {
        m_decayTimer -= ctx.dt;

        float speed = 150.f;
        m_velocity.x = 0.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) m_velocity.x = -speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) m_velocity.x = speed;

        m_hitbox.move(m_velocity.x * ctx.dt, 0.f);
        m_sprite.setPosition(m_hitbox.getPosition());
    }
}