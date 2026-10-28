#include "Entities/Scientist/Scientist.hpp"
#include "Core/Config.hpp"
#include <cmath>

Scientist::Scientist(sf::Vector2f position)
    : Character(Config::SCIENTIST_MAX_HEALTH, position)
{
    m_maxDecayTimer = Config::SCIENTIST_DECAY_TIME;
    m_decayTimer    = m_maxDecayTimer;

    m_accel     = Config::SCIENTIST_ACCEL;
    m_friction  = Config::SCIENTIST_FRICTION;
    m_jumpForce = Config::SCIENTIST_JUMP_FORCE;
    m_gravity   = Config::SCIENTIST_GRAVITY;
    m_maxSpeed  = Config::SCIENTIST_MAX_SPEED;

    InitHitbox(Config::SCIENTIST_HITBOX_WIDTH,
               Config::SCIENTIST_HITBOX_HEIGHT,
               position);
    InitSprite("../assets/animations/scientist.png",
               Config::SCIENTIST_FRAME_W,
               Config::SCIENTIST_FRAME_H,
               Config::SCIENTIST_SCALE);

    m_sprite.setPosition(position);

    constexpr int FW = Config::SCIENTIST_FRAME_W;
    constexpr int FH = Config::SCIENTIST_FRAME_H;

    sf::Vector2u texSize = m_texture.getSize();
    int cols = texSize.x / FW;
    int rows = texSize.y / FH;

    auto makeRow = [&](int r, int count)
    {
        std::vector<sf::IntRect> frames;
        for (int i = 0; i < count; ++i)
        {
            frames.push_back(sf::IntRect(i * FW, r * FH, FW, FH));
        }
        return frames;
    };

    int idleCols = std::min(cols, 4);
    m_anim.AddClip("idle", makeRow(0, idleCols),
                   idleCols > 1 ? Config::SCIENTIST_IDLE_FRAME_TIME : 1.f,
                   idleCols > 1);

    if (rows > 1)
    {
        int walkCols = std::min(cols, 4);
        m_anim.AddClip("walk", makeRow(1, walkCols),
                       walkCols > 1 ? Config::SCIENTIST_WALK_FRAME_TIME : 1.f,
                       walkCols > 1);
    }

    m_anim.Play("idle");
    m_anim.Apply(m_sprite);
}

const char* Scientist::GetAnimationName() const
{
    if (std::abs(m_velocity.x) > Config::SCIENTIST_WALK_THRESHOLD)
    {
        return "walk";
    }
    return "idle";
}

void Scientist::Update(const GameContext& ctx)
{
    if (m_isDead)
    {
        return;
    }

    if (m_isPlayerControlled)
    {
        m_decayTimer -= ctx.dt;
        UpdateMove(ctx.dt);
    }
    else
    {
        UpdateVertical(ctx.dt, false);
    }

    ApplyMovement(ctx);
}