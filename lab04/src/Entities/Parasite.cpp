#include "Entities/Parasite.hpp"
const float PARASITE_SPEED = 300.f;
const float PARASITE_WIDTH = 50.f;
const float PARASITE_HEIGHT = 60.f;
const float PARASITE_GRAVITY = 1500.f;
const float PARASITE_JUMP_FORCE = 600.f;
const float FLOOR_Y = 600.f;
const sf::Color PARASITE_COLOR = sf::Color(200, 50, 100);

Parasite::Parasite(sf::Vector2f position)
: shape(sf::Vector2f(PARASITE_WIDTH, PARASITE_HEIGHT)),
  speed(PARASITE_SPEED),
  velocity(sf::Vector2f(0.f, 0.f)),
  jumpForce(PARASITE_JUMP_FORCE),
  gravity(PARASITE_GRAVITY),
  isOnGround(false)
{
    shape.setFillColor(PARASITE_COLOR);
    shape.setPosition(position);
}

void Parasite::update(float dt)
{
    velocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        velocity.x = -speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        velocity.x = speed;
    }

    velocity.y += PARASITE_GRAVITY * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isOnGround)
    {
        velocity.y = -PARASITE_JUMP_FORCE;
        isOnGround = false;
    }

    shape.move(velocity.x * dt, velocity.y * dt);

    float bottomY = shape.getPosition().y + PARASITE_HEIGHT;
    if (bottomY >= FLOOR_Y)
    {
        shape.setPosition(shape.getPosition().x, FLOOR_Y - PARASITE_HEIGHT);
        velocity.y = 0.f;
        isOnGround = true;
    }
}
void Parasite::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}