#include "Entities/Enemy.hpp"

const float WINDOW_WIDTH = 1920.f;
const float WINDOW_HEIGHT = 1080.f;
const float ENEMY_SIZE = 40.f;

Enemy::Enemy(sf::Vector2f startPos)
{
    shape.setSize(sf::Vector2f(ENEMY_SIZE, ENEMY_SIZE));
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(startPos);
    
    velocity = sf::Vector2f(200.0f, 150.0f);
    flashTimer = 0.f;
}

void Enemy::update(float dt)
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
    } else if (pos.y + ENEMY_SIZE >= WINDOW_HEIGHT) {
        pos.y = WINDOW_HEIGHT - ENEMY_SIZE;
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

void Enemy::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::FloatRect Enemy::getBounds() const
{
    return shape.getGlobalBounds();
}

void Enemy::onCollision()
{
    shape.setFillColor(sf::Color::Red);
    flashTimer = 0.5f;
}