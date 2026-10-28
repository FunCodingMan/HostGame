#include "Entities/Character/Character.hpp"
#include "Core/GameKeyboard/GameKeyboard.hpp"
#include <iostream>
#include <cmath>

Character::Character(float maxLife, sf::Vector2f /*position*/)
    : LivingEntity(maxLife)
{
}
void Character::InitHitbox(float width, float height, sf::Vector2f position)
{
    m_hitbox.setSize(sf::Vector2f(width, height));
    m_hitbox.setOrigin(width / 2.f, height / 2.f);
    m_hitbox.setPosition(position);
    m_hitbox.setFillColor(sf::Color::Transparent);
    m_hitbox.setOutlineColor(sf::Color::Red);
    m_hitbox.setOutlineThickness(1.f);
}

void Character::InitSprite(const std::string& texturePath,
                           int frameW, int frameH, float scale)
{
    if (!m_texture.loadFromFile(texturePath))
    {
        std::cerr << "Ошибка: Не удалось загрузить " << texturePath << std::endl;
    }
    m_texture.setSmooth(false);

    m_sprite.setTexture(m_texture);
    m_sprite.setScale(scale, scale);
    m_sprite.setOrigin(frameW / 2.f, frameH / 2.f);
    m_scale = scale;
}


bool Character::CheckMoveKeys(float dt)
{
    bool left  = GameKeyboard::IsLeftKeyPressed();
    bool right = GameKeyboard::IsRightKeyPressed();

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

void Character::ResolveFriction(float dt)
{
    float f = m_isOnGround ? m_friction : m_friction * 0.3f;

    if (m_velocity.x > 0.f)
    {
        m_velocity.x -= f * dt;

        if (m_velocity.x < 0.f)
        {
            m_velocity.x = 0.f;
        }
    }
    else
    {
        m_velocity.x += f * dt;

        if (m_velocity.x > 0.f)
        {
            m_velocity.x = 0.f;
        }
    }
}

void Character::CheckMaxSpeed()
{
    if (m_velocity.x > m_maxSpeed)
    {
        m_velocity.x = m_maxSpeed;
    }

    if (m_velocity.x < -m_maxSpeed)
    {
        m_velocity.x = -m_maxSpeed;
    }
}

void Character::UpdateHorizontal(float dt)
{
    bool isMoving = CheckMoveKeys(dt);

    if (!isMoving)
    {
        ResolveFriction(dt);
    }

    CheckMaxSpeed();
}

void Character::UpdateVertical(float dt, bool allowInput)
{
    float curGravity = m_gravity;

    if (allowInput && !GameKeyboard::IsJumpKeyPressed() && m_velocity.y < 0.f)
    {
        curGravity *= 2.f;
    }

    m_velocity.y += curGravity * dt;

    if (m_velocity.y > Config::MAX_FALL_SPEED)
    {
        m_velocity.y = Config::MAX_FALL_SPEED;
    }

    if (allowInput && GameKeyboard::IsJumpKeyPressed() && m_isOnGround)
    {
        m_velocity.y = -m_jumpForce;
        m_isOnGround = false;
    }
}

void Character::UpdateMove(float dt)
{
    UpdateHorizontal(dt);
    UpdateVertical(dt, m_isPlayerControlled);
}


void Character::ResolveCollisionsX(int x, const GameContext& ctx)
{
    int tileSize = ctx.levelMap->GetTileSize();
    sf::FloatRect bounds = m_hitbox.getGlobalBounds();

    if (m_velocity.x > 0.f)
    {
        m_hitbox.setPosition(x * tileSize - (bounds.width / 2.f) - Config::EPSILON,
                             m_hitbox.getPosition().y);
        m_velocity.x = 0.f;
    }
    else if (m_velocity.x < 0.f)
    {
        m_hitbox.setPosition((x + 1) * tileSize + (bounds.width / 2.f) + Config::EPSILON,
                             m_hitbox.getPosition().y);
        m_velocity.x = 0.f;
    }
}

void Character::ResolveCollisionsY(int y, const GameContext& ctx)
{
    int tileSize = ctx.levelMap->GetTileSize();
    sf::FloatRect bounds = m_hitbox.getGlobalBounds();

    if (m_velocity.y > 0.f)
    {
        m_hitbox.setPosition(m_hitbox.getPosition().x,
                             y * tileSize - (bounds.height / 2.f) - Config::EPSILON);
        m_velocity.y = 0.f;
        m_isOnGround = true;
        OnLanding();
    }
    else if (m_velocity.y < 0.f)
    {
        m_hitbox.setPosition(m_hitbox.getPosition().x,
                             (y + 1) * tileSize + (bounds.height / 2.f) + Config::EPSILON);
        m_velocity.y = 0.f;
    }
}

void Character::ResolveCollisions(const GameContext& ctx, Axis axis)
{
    sf::FloatRect bounds = m_hitbox.getGlobalBounds();
    int tileSize = ctx.levelMap->GetTileSize();

    int minX = (int)std::floor(bounds.left / tileSize);
    int maxX = (int)std::floor((bounds.left + bounds.width)  / tileSize);
    int minY = (int)std::floor(bounds.top  / tileSize);
    int maxY = (int)std::floor((bounds.top + bounds.height) / tileSize);

    if (axis == Axis::Vertical)
    {
        m_isOnGround = false;
    }

    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            if (!ctx.levelMap->IsSolid(x, y, GetEntityType()))
            {
                continue;
            }

            if (TryBreakTile(x, y, ctx))
            {
                continue;
            }

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


void Character::UpdateSpriteDirection()
{
    if (m_velocity.x > 0.f)
    {
        m_sprite.setScale(m_scale, m_scale);
    }
    else if (m_velocity.x < 0.f)
    {
        m_sprite.setScale(-m_scale, m_scale);
    }
}

void Character::UpdateAnimation(float dt)
{
    const char* name = GetAnimationName();

    if (name == nullptr)
    {
        return;
    }

    m_anim.Play(name);
    m_anim.Update(dt);
    m_anim.Apply(m_sprite);
}

void Character::ApplyMovement(const GameContext& ctx)
{
    float dt = ctx.dt;

    m_hitbox.move(m_velocity.x * dt, 0.f);
    ResolveCollisions(ctx, Axis::Horizontal);

    m_hitbox.move(0.f, m_velocity.y * dt);
    ResolveCollisions(ctx, Axis::Vertical);

    UpdateSpriteDirection();
    UpdateAnimation(dt);

    m_sprite.setPosition(m_hitbox.getPosition());
}