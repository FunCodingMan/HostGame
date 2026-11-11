#include "Entities/Character/Character.hpp"
#include "Core/GameKeyboard/GameKeyboard.hpp"
#include "Core/Config.hpp"
#include <iostream>
#include <cmath>

using namespace Config;

Character::Character(float maxLife, sf::Vector2f /*position*/)
    : LivingEntity(maxLife)
{
    m_stateManager.SetOnStateChanged([this](Config::EntityState newState) {
        switch (newState)
        {
            case EntityState::Idle:
                m_anim.Play("idle"); 
                break;
            case EntityState::Walk: 
                m_anim.Play("walk"); 
                break;
            case EntityState::Dash: 
                m_anim.Play("dash"); 
                break;
            case EntityState::Dead:
                m_anim.Play("dead"); 
                break;
            default:
                break;
        }
    });
}

void Character::InitHitbox(float width, float height, sf::Vector2f position)
{
    m_hitbox.setSize(sf::Vector2f(width, height));
    m_hitbox.setOrigin(width / 2.f, height / 2.f);
    m_hitbox.setPosition(position);
    m_hitbox.setFillColor(sf::Color::Transparent);
    m_hitbox.setOutlineColor(sf::Color::Red);
    m_hitbox.setOutlineThickness(1.f);
    m_spriteOffset = sf::Vector2f(0.f, 0.f);
}

void Character::InitHitboxFromCenter(float left, float right, float top, float bottom, sf::Vector2f position)
{
    float w = left + right;
    float h = top + bottom;

    sf::Vector2f offset((right - left) / 2.f, (bottom - top) / 2.f);

    m_hitbox.setSize(sf::Vector2f(w, h));
    m_hitbox.setOrigin(w / 2.f, h / 2.f);
    m_hitbox.setPosition(position + offset);

    m_spriteOffset = -offset;

    m_hitbox.setFillColor(sf::Color::Transparent);
    m_hitbox.setOutlineColor(sf::Color::Red);
    m_hitbox.setOutlineThickness(1.f);
}

void Character::InitSprite(const std::string& texturePath, int frameW, int frameH, float scale)
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

void Character::UpdateHorizontal(const GameContext& ctx)
{
    bool isMoving = CheckMoveKeys(ctx.dt);

    if (!isMoving)
    {
        ResolveFriction(ctx.dt);
    }

    CheckMaxSpeed();
}

void Character::UpdateVertical(const GameContext& ctx, bool allowInput)
{
    float curGravity = m_gravity;

    if (allowInput && !GameKeyboard::IsJumpKeyPressed() && m_velocity.y < 0.f)
    {
        curGravity *= 2.f;
    }

    m_velocity.y += curGravity * ctx.dt;

    if (m_velocity.y > Config::MAX_FALL_SPEED)
    {
        m_velocity.y = Config::MAX_FALL_SPEED;
    }

    if (allowInput && GameKeyboard::IsJumpKeyPressed() && m_isOnGround)
    {
        m_velocity.y = -m_jumpForce;
        m_isOnGround = false;
        
        if (const char* sound = GetJumpSound()) {
            ctx.audio->PlaySound(sound);
        }
    }
}

void Character::UpdateMove(const GameContext& ctx)
{
    UpdateHorizontal(ctx);
    UpdateVertical(ctx, m_isPlayerControlled);
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
        float impactVelocity = m_velocity.y;
        m_hitbox.setPosition(m_hitbox.getPosition().x,
                             y * tileSize - (bounds.height / 2.f) - Config::EPSILON);
        m_velocity.y = 0.f;
        m_isOnGround = true;

        if (impactVelocity > Config::MAX_FALL_SPEED * Config::SMASH_FALL_SPEED_RATIO) 
        {
            if (const char* sound = GetLandSound()) {
                ctx.audio->PlaySound(sound); 
            }
        }

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
    m_anim.Update(dt);
    m_anim.Apply(m_sprite);
}

void Character::ApplyMovement(const GameContext& ctx)
{
    m_hitbox.move(m_velocity.x * ctx.dt, 0.f);
    ResolveCollisions(ctx, Axis::Horizontal);

    m_hitbox.move(0.f, m_velocity.y * ctx.dt);
    ResolveCollisions(ctx, Axis::Vertical);

    UpdateSpriteDirection();
    UpdateAnimation(ctx.dt);

    if (m_isOnGround && std::abs(m_velocity.x) > Config::WALK_SOUND_VELOCITY) 
    {
        m_stepTimer -= ctx.dt;
        if (m_stepTimer <= 0.f)
        {
            if (const char* sound = GetWalkSound()) {
                ctx.audio->PlaySound(sound); 
            }
            m_stepTimer = Config::STEP_SOUND_INTERVAL; 
        }
    }
    else
    {
        m_stepTimer = 0.f;
    }

    sf::Vector2f eff = m_spriteOffset;

    if (m_sprite.getScale().x < 0.f)
    {
        eff.x = -eff.x;
    }

    m_sprite.setPosition(m_hitbox.getPosition() + eff);
}