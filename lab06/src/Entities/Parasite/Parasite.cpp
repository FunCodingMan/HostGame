#include "Entities/Parasite/Parasite.hpp"

#include <iostream>

#include "Core/Config.hpp"

using namespace Config;

Parasite::Parasite(sf::Vector2f position)
    : m_accel(PARASITE_ACCEL), m_friction(PARASITE_FRICTION),
      m_jumpForce(PARASITE_JUMP_FORCE), m_gravity(PARASITE_GRAVITY), m_isOnGround(false),
      m_dashCooldown(0.f), m_dashForce(PARASITE_DASH_FORCE), m_hasDash(true), m_dashTimer(0.f)
{
    m_hitbox.setSize(sf::Vector2f(PARASITE_HITBOX_WIDTH, PARASITE_HITBOX_HEIGHT));
    m_hitbox.setOrigin(PARASITE_HITBOX_WIDTH / 2.f, PARASITE_HITBOX_HEIGHT / 2.f);
    m_hitbox.setPosition(position);

    m_hitbox.setFillColor(sf::Color::Transparent);
    m_hitbox.setOutlineColor(sf::Color::Red);
    m_hitbox.setOutlineThickness(1.f);

    if (!m_texture.loadFromFile("../assets/images/parasite.png"))
    {
        std::cerr << "Ошибка: Не удалось загрузить assets/parasite.png!" << std::endl;
    }

    m_texture.setSmooth(false);
    m_sprite.setTexture(m_texture);
    m_sprite.setScale(Config::PARASITE_SCALE, Config::PARASITE_SCALE);

    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

    m_sprite.setPosition(position);
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
        m_velocity.x -= m_accel * dt;
        return true;
    }

    if (right)
    {
        m_velocity.x += m_accel * dt;
        return true;
    }

    return false;
}

void Parasite::ResolveFriction(float dt)
{
    float f = m_isOnGround ? m_friction : m_friction * 0.3f;
    if (m_velocity.x > 0.f)
    {
        m_velocity.x -= f * dt;
        if (m_velocity.x < 0)
            m_velocity.x = 0.f;
    }
    else
    {
        m_velocity.x += f * dt;
        if (m_velocity.x > 0.f)
            m_velocity.x = 0.f;
    }
}

void Parasite::CheckMaxSpeed()
{
    if (m_velocity.x > PARASITE_MAX_SPEED)
    {
        m_velocity.x = PARASITE_MAX_SPEED;
    }
    if (m_velocity.x < -PARASITE_MAX_SPEED)
    {
        m_velocity.x = -PARASITE_MAX_SPEED;
    }
}

void Parasite::HorizontalDash(float dt)
{
    float drag = m_friction * 3.0f;

    if (m_velocity.x > 0.f)
    {
        m_velocity.x -= drag * dt;
        m_velocity.x = std::max(m_velocity.x, PARASITE_MAX_SPEED);
    }
    else
    {
        m_velocity.x += drag * dt;
        m_velocity.x = std::min(m_velocity.x, -PARASITE_MAX_SPEED);
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
    if (std::abs(m_velocity.x) > PARASITE_MAX_SPEED)
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
    float curGravity = m_gravity;

    if (!IsJumpKeyPressed() && m_velocity.y < 0.f)
    {
        curGravity *= 2.0f;
    }

    m_velocity.y += curGravity * dt;

    if (m_velocity.y > MAX_FALL_SPEED)
    {
        m_velocity.y = MAX_FALL_SPEED;
    }

    if (IsJumpKeyPressed() && m_isOnGround)
    {
        m_velocity.y = -PARASITE_JUMP_FORCE;
        m_isOnGround = false;
    }
}

void Parasite::CheckXBoundaries()
{
    sf::FloatRect bounds = m_hitbox.getGlobalBounds();
    float halfWidth = bounds.width / 2.f;

    if (bounds.left < 0)
    {
        m_hitbox.setPosition(halfWidth, m_hitbox.getPosition().y);
        m_velocity.x = 0;
    }

    if (bounds.left + bounds.width >= WINDOW_WIDTH)
    {
        m_hitbox.setPosition(WINDOW_WIDTH - halfWidth, m_hitbox.getPosition().y);
        m_velocity.x = 0;
    }
}

void Parasite::UpdateDash(float dt, sf::Vector2f mousePos)
{
    if (m_dashCooldown > 0.f)
    {
        m_dashCooldown -= dt;
    }

    if (IsDashKeyPressed() && m_dashCooldown <= 0.f && m_hasDash)
    {
        sf::Vector2f center = m_hitbox.getPosition();

        float dx = mousePos.x - center.x;
        float dy = mousePos.y - center.y;

        float length = std::sqrt(dx * dx + dy * dy);

        if (length != 0)
        {
            m_velocity.x = (dx / length) * m_dashForce;
            m_velocity.y = (dy / length) * m_dashForce;

            m_dashCooldown = DASH_COOLDOWN_TIME;
            m_dashTimer = DASH_DURATION;
            m_hasDash = false;
            m_isOnGround = false;
        }
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

void Parasite::UpdateMove(float dt)
{

    UpdateDashTimer(dt);
    if (m_dashTimer <= 0.f)
    {
        UpdateHorizontal(dt);
        UpdateVertical(dt);
    }
}

void Parasite::UpdateSpriteDirection()
{
    if (m_dashTimer > 0.f)
    {
        if (m_velocity.x > 0.f)
        {
            m_sprite.setScale(Config::PARASITE_SCALE, Config::PARASITE_SCALE);
        }
        else if (m_velocity.x < 0.f)
        {
            m_sprite.setScale(-Config::PARASITE_SCALE, Config::PARASITE_SCALE);
        }
    }
    else
    {
        bool left = IsLeftKeyPressed();
        bool right = IsRightKeyPressed();
    
        if (right && !left)
        {
            m_sprite.setScale(Config::PARASITE_SCALE, Config::PARASITE_SCALE);
        }
        else if (left && !right)
        {
            m_sprite.setScale(-Config::PARASITE_SCALE, Config::PARASITE_SCALE);
        }
    }
}

void Parasite::ResolveCollisionsX(int x, const GameContext& ctx)
{
    int tileSize = ctx.levelMap->GetTileSize();
    sf::FloatRect bounds = m_hitbox.getGlobalBounds();

    if (m_velocity.x > 0.f)
    {
        m_hitbox.setPosition(x * tileSize - (bounds.width / 2.f) - Config::EPSILON, m_hitbox.getPosition().y);
        m_velocity.x = 0.f;
    }
    else if (m_velocity.x < 0.f)
    {
        m_hitbox.setPosition((x + 1) * tileSize + (bounds.width / 2.f) + Config::EPSILON, m_hitbox.getPosition().y);
        m_velocity.x = 0.f;
    }
}

void Parasite::ResolveCollisionsY(int y, const GameContext& ctx)
{
    int tileSize = ctx.levelMap->GetTileSize();
    sf::FloatRect bounds = m_hitbox.getGlobalBounds();

    if (m_velocity.y > 0.f)
    {
        m_hitbox.setPosition(m_hitbox.getPosition().x, y * tileSize - (bounds.height / 2.f) - Config::EPSILON);
        m_velocity.y = 0.f;
        m_isOnGround = true;
        m_hasDash = true;
    }
    else if (m_velocity.y < 0.f) 
    {
        m_hitbox.setPosition(m_hitbox.getPosition().x, (y + 1) * tileSize + (bounds.height / 2.f) + Config::EPSILON);
        m_velocity.y = 0.f;
    }    
}

void Parasite::ResolveCollisions(const GameContext& ctx, Axis axis)
{
    sf::FloatRect bounds = m_hitbox.getGlobalBounds();
    int tileSize = ctx.levelMap->GetTileSize();

    int minX = std::floor(bounds.left / tileSize);
    int maxX = std::floor((bounds.left + bounds.width) / tileSize);
    int minY = std::floor(bounds.top / tileSize);
    int maxY = std::floor((bounds.top + bounds.height) / tileSize);

    float epsilon = 0.01f;

    if (axis == Axis::Vertical) 
    {
        m_isOnGround = false;
    }

    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            if (ctx.levelMap->IsSolid(x, y, Config::EntityType::Parasite))
            {
                if (axis == Axis::Horizontal)
                {
                    ResolveCollisionsX(x, ctx);
                }
                else
                {
                    ResolveCollisionsY(y, ctx);
                }
            }
        }
    }
}



void Parasite::Update(const GameContext& ctx)
{
    float dt = ctx.dt;
    sf::Vector2f mousePos = ctx.mousePos;

    UpdateDash(dt, mousePos);

    UpdateMove(dt);

    m_hitbox.move(m_velocity.x * dt, 0.f);
    ResolveCollisions(ctx, Axis::Horizontal);

    m_hitbox.move(0.f, m_velocity.y * dt);
    ResolveCollisions(ctx, Axis::Vertical);

    UpdateSpriteDirection();

    m_sprite.setPosition(m_hitbox.getPosition());
}