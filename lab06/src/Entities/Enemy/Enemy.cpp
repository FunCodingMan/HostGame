#include "Entities/Enemy/Enemy.hpp"
#include "Core/Config.hpp"

using namespace Config;

Enemy::Enemy(sf::Vector2f startPos)
{
    m_shape.setSize(sf::Vector2f(ENEMY_SIZE, ENEMY_SIZE));
    m_shape.setFillColor(sf::Color::Green);
    m_shape.setPosition(startPos);
    
    m_velocity = sf::Vector2f(200.0f, 150.0f);
    m_flashTimer = 0.f;
}

void Enemy::Update(float dt)
{
    m_shape.move(m_velocity.x * dt, m_velocity.y * dt);

    sf::Vector2f pos = m_shape.getPosition();

    if (pos.x <= 0.f) {
        pos.x = 0.f;
        m_velocity.x = -m_velocity.x;
    } else if (pos.x + ENEMY_SIZE >= WINDOW_WIDTH) {
        pos.x = WINDOW_WIDTH - ENEMY_SIZE;
        m_velocity.x = -m_velocity.x;
    }

    if (pos.y <= 0.f) {
        pos.y = 0.f;
        m_velocity.y = -m_velocity.y;
    } else if (pos.y + ENEMY_SIZE >= FLOOR_Y) {
        pos.y = FLOOR_Y - ENEMY_SIZE;
        m_velocity.y = -m_velocity.y;
    }

    m_shape.setPosition(pos);

    if (m_flashTimer > 0.f) {
        m_flashTimer -= dt;
        if (m_flashTimer <= 0.f) {
            m_shape.setFillColor(sf::Color::Green);
        }
    }
}

void Enemy::Draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
}

sf::FloatRect Enemy::GetBounds() const
{
    return m_shape.getGlobalBounds();
}

void Enemy::OnCollision()
{
    m_shape.setFillColor(sf::Color::Red);
    m_flashTimer = 0.5f;
}