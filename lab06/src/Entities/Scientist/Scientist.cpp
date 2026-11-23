#include "Entities/Scientist/Scientist.hpp"
#include "Core/Config.hpp"
#include <cmath>
#include "Core/Utils/StringUtils/StringUtils.hpp"

using namespace Config;
using namespace StringUtils;

Scientist::Scientist(sf::Vector2f position)
    : Host(Config::SCIENTIST_MAX_HEALTH, position, Config::SCIENTIST_DECAY_TIME)
{
    m_accel     = Config::SCIENTIST_ACCEL;
    m_friction  = Config::SCIENTIST_FRICTION;
    m_jumpForce = Config::SCIENTIST_JUMP_FORCE;
    m_gravity   = Config::SCIENTIST_GRAVITY;
    m_maxSpeed  = Config::SCIENTIST_MAX_SPEED;

    InitSprite("../assets/animations/scientist.png",
        Config::SCIENTIST_FRAME_W,
        Config::SCIENTIST_FRAME_H,
        Config::SCIENTIST_SCALE);
        
    InitHitboxFromCenter(Config::SCIENTIST_HITBOX_LEFT,
                        Config::SCIENTIST_HITBOX_RIGHT,
                        Config::SCIENTIST_HITBOX_TOP,
                        Config::SCIENTIST_HITBOX_BOTTOM,
                        position);
        
    m_sprite.setPosition(position);

    m_anim.Setup(m_texture,
                 Config::SCIENTIST_FRAME_W,
                 Config::SCIENTIST_FRAME_H);

    m_anim.AddClip("idle", 3, Config::SCIENTIST_IDLE_FRAME_TIME);
    m_anim.AddClip("walk", 3, Config::SCIENTIST_WALK_FRAME_TIME);

    m_properties.SetSfString("name", ToSfStringFromUtf8("Учёный"));
    m_properties.SetInt("access_level", 1);

    m_stateManager.SetState(EntityState::Idle);
}