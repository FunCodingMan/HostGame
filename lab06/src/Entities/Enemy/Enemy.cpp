#include "Entities/Enemy/Enemy.hpp"
#include "Core/Config.hpp"

using namespace Config;

Enemy::Enemy(sf::Vector2f startPos)
{
    shape.setSize(sf::Vector2f(ENEMY_SIZE, ENEMY_SIZE));
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(startPos);
    
    velocity = sf::Vector2f(200.0f, 150.0f);
    flashTimer = 0.f;
}

void Enemy::Update(float dt)
{
    shape.move(velocity.x * dt, velocity.y * dt);

    sf::Vector2f pos = shape.getPosition();

    if (pos.x <= 0.f) {
        pos.x = 0.f;
        velocity.x = -velocity.x;
    } else if (pos.x + ENEMY_SIZE >= WINDOW_WIDTH) {
        pos.x = WINDOW_WIDTH - ENEMY_SIZE;
        velocity.x = -velocity.x;
    }

    if (pos.y <= 0.f) {
        pos.y = 0.f;
        velocity.y = -velocity.y;
    } else if (pos.y + ENEMY_SIZE >= FLOOR_Y) {
        pos.y = FLOOR_Y - ENEMY_SIZE;
        velocity.y = -velocity.y;
    }

    shape.setPosition(pos);

    if (flashTimer > 0.f) {
        flashTimer -= dt;
        if (flashTimer <= 0.f) {
            shape.setFillColor(sf::Color::Green);
        }
    }
}

void Enemy::Draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::FloatRect Enemy::GetBounds() const
{
    return shape.getGlobalBounds();
}

void Enemy::OnCollision()
{
    shape.setFillColor(sf::Color::Red);
    flashTimer = 0.5f;
}