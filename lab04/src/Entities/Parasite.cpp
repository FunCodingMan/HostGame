#include "Entities/Parasite.hpp"
const float PARASITE_MAX_SPEED = 400.f;
const float PARASITE_ACCEL = 2000.f;
const float PARASITE_FRICTION = 1000.f;
const float PARASITE_WIDTH = 50.f;
const float PARASITE_HEIGHT = 60.f;
const float PARASITE_GRAVITY = 1500.f;
const float PARASITE_JUMP_FORCE = 600.f;
const float FLOOR_Y = 600.f;
const float GAME_WIDTH = 1920.f;
const float MAX_FALL_SPEED = 1000.f;
const sf::Color PARASITE_COLOR = sf::Color(200, 50, 100);

Parasite::Parasite(sf::Vector2f position)
: shape(sf::Vector2f(PARASITE_WIDTH, PARASITE_HEIGHT)),
  accel(PARASITE_ACCEL),
  friction(PARASITE_FRICTION),
  velocity(sf::Vector2f(0.f, 0.f)),
  jumpForce(PARASITE_JUMP_FORCE),
  gravity(PARASITE_GRAVITY),
  isOnGround(false)
{
    shape.setFillColor(PARASITE_COLOR);
    shape.setPosition(position);
}

bool Parasite::checkMoveKeys(float dt)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        velocity.x -= accel * dt;
        return true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        velocity.x += accel * dt;
        return true;
    }
    return false;
}

void Parasite::resolveFriction(float dt)
{
    if (velocity.x > 0.f)
    {
        velocity.x -= friction * dt;
        if (velocity.x < 0) velocity.x = 0.f;
    }
    else
    {
        velocity.x += friction * dt;
        if (velocity.x > 0.f) velocity.x = 0.f;
    }
}

void Parasite::checkMaxSpeed()
{
    if (velocity.x > PARASITE_MAX_SPEED)
    {
        velocity.x = PARASITE_MAX_SPEED;
    }
    if (velocity.x < -PARASITE_MAX_SPEED)
    {
        velocity.x = -PARASITE_MAX_SPEED;
    }
}

void Parasite::updateHorizontal(float dt)
{
    bool isMoving = checkMoveKeys(dt);

    if (!isMoving)
    {
        resolveFriction(dt);
    }

    checkMaxSpeed();
}

void Parasite::updateVertical(float dt)
{
    float curGravity = gravity;

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && velocity.y < 0.f)
    {
        curGravity *= 2.0f;
    }

    velocity.y += curGravity * dt;

    if (velocity.y > MAX_FALL_SPEED) {
       velocity.y = MAX_FALL_SPEED;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isOnGround)
    {
        velocity.y = -PARASITE_JUMP_FORCE;
        isOnGround = false;
    }
}

void Parasite::checkXBoundaries()
{
    if (shape.getPosition().x < 0)
    {
        shape.setPosition(0, shape.getPosition().y);
        velocity.x = 0;
    }
    
    float rightX = shape.getPosition().x + PARASITE_WIDTH;
    if (rightX >= GAME_WIDTH)
    {
        shape.setPosition(GAME_WIDTH - PARASITE_WIDTH, shape.getPosition().y);
        velocity.x = 0;
    }
}

void Parasite::resolveCollisions()
{
    float bottomY = shape.getPosition().y + PARASITE_HEIGHT;
    if (bottomY >= FLOOR_Y)
    {
        shape.setPosition(shape.getPosition().x, FLOOR_Y - PARASITE_HEIGHT);
        velocity.y = 0.f;
        isOnGround = true;
    }

    checkXBoundaries();
}

void Parasite::update(float dt)
{
    updateHorizontal(dt);

    updateVertical(dt);

    shape.move(velocity.x * dt, velocity.y * dt);

    resolveCollisions();
}

void Parasite::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}