#include "Entities/Parasite.hpp"
#include "Core/Config.hpp"
#include <iostream>

using namespace Config;

Parasite::Parasite(sf::Vector2f position)
: accel(PARASITE_ACCEL),
  friction(PARASITE_FRICTION),
  velocity(sf::Vector2f(0.f, 0.f)),
  jumpForce(PARASITE_JUMP_FORCE),
  gravity(PARASITE_GRAVITY),
  isOnGround(false),
  dashCooldown(0.f),
  dashForce(PARASITE_DASH_FORCE),
  hasDash(true),
  dashTimer(0.f)
{
    if (!texture.loadFromFile("../assets/parasite.png"))
    {
        std::cerr << "Ошибка: Не удалось загрузить assets/parasite.png!" << std::endl;
    }

    sprite.setTexture(texture);

    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

    sprite.setPosition(position);
}

bool Parasite::isJumpKeyPressed()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || 
           sf::Keyboard::isKeyPressed(sf::Keyboard::W)     ||
           sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
}
bool Parasite::isLeftKeyPressed()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
}
bool Parasite::isRightKeyPressed()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
}

bool Parasite::isDashKeyPressed()
{
    return sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
}

bool Parasite::checkMoveKeys(float dt)
{
    bool left = isLeftKeyPressed();
    bool right = isRightKeyPressed();

    if (left && right)
    {
        return false;
    }

    if (left)
    {
        velocity.x -= accel * dt;
        return true;
    }

    if (right)
    {
        velocity.x += accel * dt;
        return true;
    }
    
    return false;
}

void Parasite::resolveFriction(float dt)
{
    float f = isOnGround ? friction : friction * 0.3f;
    if (velocity.x > 0.f)
    {
        velocity.x -= f * dt;
        if (velocity.x < 0) velocity.x = 0.f;
    }
    else
    {
        velocity.x += f * dt;
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

void Parasite::horizontalDash(float dt)
{
    float drag = friction * 3.0f;

    if (velocity.x > 0.f)
    {
        velocity.x -= drag * dt;
        velocity.x = std::max(velocity.x, PARASITE_MAX_SPEED);
    }
    else
    {
        velocity.x += drag * dt;
        velocity.x = std::min(velocity.x, -PARASITE_MAX_SPEED);
    }
}

void Parasite::horizontalCommon(float dt)
{
    bool isMoving = checkMoveKeys(dt);

    if (!isMoving)
    {
        resolveFriction(dt);
    }

    checkMaxSpeed();
}

void Parasite::updateHorizontal(float dt)
{
    if (std::abs(velocity.x) > PARASITE_MAX_SPEED)
    {
        horizontalDash(dt);
    }
    else
    {
        horizontalCommon(dt);
    }
}

void Parasite::updateVertical(float dt)
{
    float curGravity = gravity;

    if (!isJumpKeyPressed() && velocity.y < 0.f)
    {
        curGravity *= 2.0f;
    }

    velocity.y += curGravity * dt;

    if (velocity.y > MAX_FALL_SPEED) {
       velocity.y = MAX_FALL_SPEED;
    }

    if (isJumpKeyPressed() && isOnGround)
    {
        velocity.y = -PARASITE_JUMP_FORCE;
        isOnGround = false;
    }
}

void Parasite::checkXBoundaries()
{
    if (sprite.getPosition().x < 0)
    {
        sprite.setPosition(0, sprite.getPosition().y);
        velocity.x = 0;
    }
    
    float rightX = sprite.getPosition().x + PARASITE_WIDTH;
    if (rightX >= WINDOW_WIDTH)
    {
        sprite.setPosition(WINDOW_WIDTH - PARASITE_WIDTH, sprite.getPosition().y);
        velocity.x = 0;
    }
}

void Parasite::resolveCollisions()
{
    isOnGround = false;
    
    float bottomY = sprite.getPosition().y + PARASITE_HEIGHT;
    if (bottomY >= FLOOR_Y)
    {
        sprite.setPosition(sprite.getPosition().x, FLOOR_Y - PARASITE_HEIGHT);
        velocity.y = 0.f;
        isOnGround = true;
        hasDash = true;
    }

    checkXBoundaries();
}

void Parasite::updateDash(float dt, sf::Vector2f mousePos)
{
    if (dashCooldown > 0.f)
    {
        dashCooldown -= dt;
    }

    if (isDashKeyPressed() && dashCooldown <= 0.f && hasDash)
    {
        sf::Vector2f center = sprite.getPosition() + sf::Vector2f(PARASITE_WIDTH / 2.f, PARASITE_HEIGHT / 2.f);

        float dx = mousePos.x - center.x;
        float dy = mousePos.y - center.y;

        float length = std::sqrt(dx * dx + dy * dy);

        if (length != 0)
        {
            velocity.x = (dx / length) * dashForce;
            velocity.y = (dy / length) * dashForce;

            dashCooldown = DASH_COOLDOWN_TIME;
            dashTimer = DASH_DURATION;
            hasDash = false;
            isOnGround = false;
        }
    }
}

void Parasite::updateDashTimer(float dt)
{
    if (dashTimer > 0.f)
    {
        dashTimer -= dt;

        if (dashTimer <= 0.f)
        {
            velocity.x *= 0.5f;
            velocity.y *= 0.5f;
        }
    }
}

void Parasite::updateMove(float dt)
{

    updateDashTimer(dt);
    if (dashTimer <= 0.f)
    {
        updateHorizontal(dt);
        updateVertical(dt);
    }
}

sf::FloatRect Parasite::getBounds() const
{
    return sprite.getGlobalBounds(); 
}

void Parasite::update(float dt, sf::Vector2f mousePos)
{
    updateDash(dt, mousePos);
    
    updateMove(dt);

    sprite.move(velocity.x * dt, velocity.y * dt);

    resolveCollisions();


    
}

void Parasite::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}