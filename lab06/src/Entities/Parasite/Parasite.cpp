#include "Entities/Parasite/Parasite.hpp"

#include <iostream>

#include "Core/Config.hpp"

using namespace Config;

Parasite::Parasite(sf::Vector2f position)
    : accel(PARASITE_ACCEL), friction(PARASITE_FRICTION), velocity(sf::Vector2f(0.f, 0.f)),
      jumpForce(PARASITE_JUMP_FORCE), gravity(PARASITE_GRAVITY), isOnGround(false),
      dashCooldown(0.f), dashForce(PARASITE_DASH_FORCE), hasDash(true), dashTimer(0.f)
{
    hitbox.setSize(sf::Vector2f(PARASITE_HITBOX_WIDTH, PARASITE_HITBOX_HEIGHT));
    hitbox.setOrigin(PARASITE_HITBOX_WIDTH / 2.f, PARASITE_HITBOX_HEIGHT / 2.f);
    hitbox.setPosition(position);

    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(1.f);

    if (!texture.loadFromFile("../assets/parasite.png"))
    {
        std::cerr << "Ошибка: Не удалось загрузить assets/parasite.png!" << std::endl;
    }

    texture.setSmooth(false);
    sprite.setTexture(texture);
    sprite.setScale(Config::PARASITE_SCALE, Config::PARASITE_SCALE);

    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

    sprite.setPosition(position);
}

bool Parasite::IsJumpKeyPressed()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
           sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
           sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
}
bool Parasite::IsLeftKeyPressed()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
           sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
}
bool Parasite::IsRightKeyPressed()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
           sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
}

bool Parasite::IsDashKeyPressed()
{
    return sf::Mouse::isButtonPressed(sf::Mouse::Left) ||
           sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
}

bool Parasite::CheckMoveKeys(float dt)
{
    bool left = IsLeftKeyPressed();
    bool right = IsRightKeyPressed();

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

void Parasite::ResolveFriction(float dt)
{
    float f = isOnGround ? friction : friction * 0.3f;
    if (velocity.x > 0.f)
    {
        velocity.x -= f * dt;
        if (velocity.x < 0)
            velocity.x = 0.f;
    }
    else
    {
        velocity.x += f * dt;
        if (velocity.x > 0.f)
            velocity.x = 0.f;
    }
}

void Parasite::CheckMaxSpeed()
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

void Parasite::HorizontalDash(float dt)
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

void Parasite::HorizontalCommon(float dt)
{
    bool isMoving = CheckMoveKeys(dt);

    if (!isMoving)
    {
        ResolveFriction(dt);
    }

    CheckMaxSpeed();
}

void Parasite::UpdateHorizontal(float dt)
{
    if (std::abs(velocity.x) > PARASITE_MAX_SPEED)
    {
        HorizontalDash(dt);
    }
    else
    {
        HorizontalCommon(dt);
    }
}

void Parasite::UpdateVertical(float dt)
{
    float curGravity = gravity;

    if (!IsJumpKeyPressed() && velocity.y < 0.f)
    {
        curGravity *= 2.0f;
    }

    velocity.y += curGravity * dt;

    if (velocity.y > MAX_FALL_SPEED)
    {
        velocity.y = MAX_FALL_SPEED;
    }

    if (IsJumpKeyPressed() && isOnGround)
    {
        velocity.y = -PARASITE_JUMP_FORCE;
        isOnGround = false;
    }
}

void Parasite::CheckXBoundaries()
{
    sf::FloatRect bounds = hitbox.getGlobalBounds();
    float halfWidth = bounds.width / 2.f;

    if (bounds.left < 0)
    {
        hitbox.setPosition(halfWidth, hitbox.getPosition().y);
        velocity.x = 0;
    }

    if (bounds.left + bounds.width >= WINDOW_WIDTH)
    {
        hitbox.setPosition(WINDOW_WIDTH - halfWidth, hitbox.getPosition().y);
        velocity.x = 0;
    }
}

void Parasite::ResolveCollisions()
{
    isOnGround = false;

    sf::FloatRect bounds = hitbox.getGlobalBounds();
    float halfHeight = bounds.height / 2.f;

    float bottomY = bounds.top + bounds.height;

    if (bottomY >= FLOOR_Y)
    {
        hitbox.setPosition(hitbox.getPosition().x, FLOOR_Y - halfHeight);
        velocity.y = 0.f;
        isOnGround = true;
        hasDash = true;
    }

    CheckXBoundaries();
}

void Parasite::UpdateDash(float dt, sf::Vector2f mousePos)
{
    if (dashCooldown > 0.f)
    {
        dashCooldown -= dt;
    }

    if (IsDashKeyPressed() && dashCooldown <= 0.f && hasDash)
    {
        sf::Vector2f center = hitbox.getPosition();

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

void Parasite::UpdateDashTimer(float dt)
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

void Parasite::UpdateMove(float dt)
{

    UpdateDashTimer(dt);
    if (dashTimer <= 0.f)
    {
        UpdateHorizontal(dt);
        UpdateVertical(dt);
    }
}

sf::FloatRect Parasite::GetBounds() const
{
    return hitbox.getGlobalBounds();
}


void Parasite::UpdateSpriteDirection()
{
    if (dashTimer > 0.f)
    {
        if (velocity.x > 0.f)
        {
            sprite.setScale(Config::PARASITE_SCALE, Config::PARASITE_SCALE);
        }
        else if (velocity.x < 0.f)
        {
            sprite.setScale(-Config::PARASITE_SCALE, Config::PARASITE_SCALE);
        }
    }
    else
    {
        bool left = IsLeftKeyPressed();
        bool right = IsRightKeyPressed();
    
        if (right && !left)
        {
            sprite.setScale(Config::PARASITE_SCALE, Config::PARASITE_SCALE);
        }
        else if (left && !right)
        {
            sprite.setScale(-Config::PARASITE_SCALE, Config::PARASITE_SCALE);
        }
    }
}

void Parasite::Update(float dt, sf::Vector2f mousePos)
{
    UpdateDash(dt, mousePos);

    UpdateMove(dt);

    hitbox.move(velocity.x * dt, velocity.y * dt);

    ResolveCollisions();

    UpdateSpriteDirection();

    sprite.setPosition(hitbox.getPosition());
}

void Parasite::Draw(sf::RenderWindow &window)
{
    window.draw(sprite);
    window.draw(hitbox);
}