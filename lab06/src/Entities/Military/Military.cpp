#include "Entities/Military/Military.hpp"
#include "Core/Config.hpp"
#include "Core/GameKeyboard/GameKeyboard.hpp"
#include "Core/Sound/SoundManager/SoundManager.hpp"
#include "Core/Utils/RandomUtils/RandomUtils.hpp"
#include "Core/Utils/Raycast/Raycast.hpp"
#include "Core/Utils/StringUtils/StringUtils.hpp"
#include <cmath>
#include <fstream>
#include <iostream>

using namespace Config;
using namespace StringUtils;

Military::Military(sf::Vector2f position)
    : Host(Config::MILITARY_MAX_HEALTH, position, Config::MILITARY_DECAY_TIME)
{
    m_accel     = Config::MILITARY_ACCEL;
    m_friction  = Config::MILITARY_FRICTION;
    m_jumpForce = Config::MILITARY_JUMP_FORCE;
    m_gravity   = Config::MILITARY_GRAVITY;
    m_maxSpeed  = Config::MILITARY_RUN_SPEED;
    m_stepSoundInterval = Config::HOST_STEP_SOUND_INTERVAL;

    bool hasTexture = std::ifstream("../assets/animations/military.png").good();

    if (hasTexture)
    {
        hasTexture = InitSprite("../assets/animations/military.png",
                                Config::MILITARY_FRAME_W,
                                Config::MILITARY_FRAME_H,
                                Config::MILITARY_SCALE);
    }

    if (!hasTexture)
    {
        InitSprite("../assets/animations/scientist.png",
                   Config::SCIENTIST_FRAME_W,
                   Config::SCIENTIST_FRAME_H,
                   Config::MILITARY_SCALE);
        m_sprite.setColor(sf::Color(170, 220, 170));
    }

    InitHitboxFromCenter(Config::MILITARY_HITBOX_LEFT,
                         Config::MILITARY_HITBOX_RIGHT,
                         Config::MILITARY_HITBOX_TOP,
                         Config::MILITARY_HITBOX_BOTTOM,
                         position);

    m_sprite.setPosition(position);

    m_anim.Setup(m_texture,
                 Config::MILITARY_FRAME_W,
                 Config::MILITARY_FRAME_H);

    m_anim.AddClip("idle", 8, Config::MILITARY_IDLE_FRAME_TIME);
    m_anim.AddClip("walk", 5, Config::MILITARY_WALK_FRAME_TIME);
    m_anim.AddClip("attack", 2, Config::MILITARY_SHOOT_FRAME_TIME, 2, 0, true);
    m_anim.AddClip("reload", 2, 0.15f, 3, 0, true);
    m_anim.AddClip("dead", 6, Config::MILITARY_DEATH_FRAME_TIME, 4, 0, false);

    m_properties.SetSfString("name", ToSfStringFromUtf8("Военный"));
    m_properties.SetInt("access_level", 0);

    if (!m_weaponTexture.loadFromFile("../assets/animations/gun.png"))
    {
        std::cerr << "Ошибка: Не удалось загрузить ../assets/animations/gun.png" << std::endl;
    }
    m_weaponTexture.setSmooth(false);

    m_weapon.setTexture(m_weaponTexture);
    m_weapon.setScale(Config::MILITARY_WEAPON_SCALE, Config::MILITARY_WEAPON_SCALE);
    m_weapon.setOrigin(Config::MILITARY_WEAPON_GRIP_X,
                       Config::MILITARY_WEAPON_FRAME_H / 2.f);

    m_weaponAnim.Setup(m_weaponTexture,
                       Config::MILITARY_WEAPON_FRAME_W,
                       Config::MILITARY_WEAPON_FRAME_H);
    m_weaponAnim.AddClip("idle", 1, 0.f);
    m_weaponAnim.AddClip("shoot", 2, Config::MILITARY_WEAPON_SHOOT_FRAME_TIME, 1, 0, false);
    m_weaponAnim.Play("idle");

    m_stateManager.SetState(EntityState::Idle);
}

void Military::UpdateControlled(const GameContext& ctx)
{
    Character::UpdateMove(ctx);
    UpdateTimers(ctx);

    sf::Vector2f center = m_hitbox.getPosition();
    SetLookDir(ctx.mousePos.x >= center.x ? 1 : -1);
    UpdateWeaponAim(ctx.mousePos);

    if (m_ammo <= 0 && !m_isReloading)
    {
        StartReload(ctx);
    }
    else if (GameKeyboard::IsShootKeyPressed())
    {
        TryFire(ctx, true);
    }
}

void Military::UpdateAI(const GameContext& ctx)
{
    UpdateTimers(ctx);
    UpdateVertical(ctx, false);

    bool seesTarget = false;
    sf::Vector2f targetPos;

    if (ctx.playerAvatar != nullptr && !ctx.playerAvatar->IsDead() && ctx.avatarIsParasite)
    {
        sf::FloatRect bounds = ctx.playerAvatar->GetBounds();
        targetPos = sf::Vector2f(bounds.left + bounds.width / 2.f,
                                 bounds.top + bounds.height / 2.f);

        seesTarget = Raycast::CanSee(*ctx.levelMap,
                                     GetEyePosition(),
                                     targetPos,
                                     Config::MILITARY_VIEW_RANGE,
                                     Config::MILITARY_FOV_COSINE,
                                     sf::Vector2f(static_cast<float>(m_lookDir), 0.f));
    }

    if (seesTarget)
    {
        m_hasTarget = true;
        m_velocity.x = 0.f;

        SetLookDir(targetPos.x >= m_hitbox.getPosition().x ? 1 : -1);
        UpdateWeaponAim(targetPos);

        if (!m_wasSeen)
        {
            m_reactionTimer = Config::MILITARY_REACTION_TIME;
            m_alertTimer    = Config::MILITARY_ALERT_STATE_TIME;
            m_stateManager.SetState(EntityState::Alert);

            if (ctx.audio != nullptr)
            {
                if (ctx.audio->HasSound(Config::SOUND_ID_REPORT))
                {
                    ctx.audio->PlaySound(Config::SOUND_ID_REPORT);
                }
                if (ctx.audio->HasSound(Config::SOUND_ID_ALERT))
                {
                    ctx.audio->PlaySound(Config::SOUND_ID_ALERT);
                }
            }
        }

        if (!m_isReloading && m_reactionTimer <= 0.f)
        {
            if (m_ammo <= 0)
            {
                StartReload(ctx);
            }
            else
            {
                TryFire(ctx, false);
            }
        }
    }
    else
    {
        m_hasTarget = false;
        m_reactionTimer = 0.f;

        if (m_isReloading)
        {
            m_velocity.x = 0.f;
        }
        else
        {
            UpdatePatrol(ctx);
        }

        sf::Vector2f forward = GetEyePosition();
        forward.x += static_cast<float>(m_lookDir) * 100.f;
        UpdateWeaponAim(forward);

        if (m_ammo <= 0 && !m_isReloading)
        {
            StartReload(ctx);
        }
    }

    m_wasSeen = seesTarget;
}

void Military::UpdatePatrol(const GameContext& ctx)
{
    if (!m_isOnGround)
    {
        m_velocity.x = 0.f;
        return;
    }

    const float tileSize = static_cast<float>(ctx.levelMap->GetTileSize());
    sf::FloatRect bounds = GetBounds();

    auto isSolidAt = [&](float x, float y)
    {
        int tx = static_cast<int>(std::floor(x / tileSize));
        int ty = static_cast<int>(std::floor(y / tileSize));
        return ctx.levelMap->IsSolid(tx, ty, Config::EntityType::Military);
    };

    float aheadX = m_patrolDir > 0.f ? bounds.left + bounds.width + 4.f
                                    : bounds.left - 4.f;
    bool wallAhead   = isSolidAt(aheadX, bounds.top + bounds.height * 0.5f);
    bool groundAhead = isSolidAt(aheadX, bounds.top + bounds.height + 4.f);

    if (wallAhead || !groundAhead)
    {
        m_patrolDir = -m_patrolDir;
    }

    m_velocity.x = m_patrolDir * Config::MILITARY_PATROL_SPEED;
    SetLookDir(m_patrolDir > 0.f ? 1 : -1);
}

void Military::UpdateTimers(const GameContext& ctx)
{
    m_weaponAnim.Update(ctx.dt);

    if (m_weaponAnim.Current() == "shoot" && m_weaponAnim.IsFinished())
    {
        m_weaponAnim.Play("idle");
    }

    if (m_fireTimer > 0.f)
    {
        m_fireTimer -= ctx.dt;
    }

    if (m_reactionTimer > 0.f)
    {
        m_reactionTimer -= ctx.dt;
    }

    if (m_attackTimer > 0.f)
    {
        m_attackTimer -= ctx.dt;
        if (m_attackTimer <= 0.f && m_stateManager.GetState() == EntityState::Attack)
        {
            m_stateManager.SetState(EntityState::Idle);
        }
    }

    if (m_alertTimer > 0.f)
    {
        m_alertTimer -= ctx.dt;
        if (m_alertTimer <= 0.f && m_stateManager.GetState() == EntityState::Alert)
        {
            m_stateManager.SetState(EntityState::Idle);
        }
    }

    if (m_isReloading)
    {
        m_reloadTimer -= ctx.dt;
        if (m_reloadTimer <= 0.f)
        {
            m_isReloading = false;
            m_ammo = Config::MILITARY_AMMO;

            if (m_stateManager.GetState() == EntityState::Reload)
            {
                m_stateManager.SetState(EntityState::Idle);
            }
        }
    }
}

void Military::UpdateWeaponAim(sf::Vector2f point)
{
    sf::Vector2f shoulder = GetShoulderPosition();
    sf::Vector2f direction = point - shoulder;

    if (std::hypot(direction.x, direction.y) > 0.001f)
    {
        m_aimAngle = std::atan2(direction.y, direction.x) * Config::RAD_TO_DEG;
    }
}

void Military::TryFire(const GameContext& ctx, bool fromPlayer)
{
    if (m_isReloading || m_fireTimer > 0.f || m_ammo <= 0 || ctx.bullets == nullptr)
    {
        return;
    }

    --m_ammo;
    m_fireTimer   = Config::MILITARY_FIRE_INTERVAL;
    m_attackTimer = Config::MILITARY_ATTACK_STATE_TIME;
    m_stateManager.SetState(EntityState::Attack);
    m_weaponAnim.Play("shoot", true);

    float radians = m_aimAngle / Config::RAD_TO_DEG;
    sf::Vector2f direction(std::cos(radians), std::sin(radians));

    ctx.bullets->Spawn(GetMuzzlePosition(), direction,
                       Config::MILITARY_SHOT_DAMAGE, fromPlayer);

    if (ctx.audio != nullptr)
    {
        const char* shots[Config::SOUND_SHOT_VARIANTS] = {
            Config::SOUND_ID_SHOT_1,
            Config::SOUND_ID_SHOT_2,
            Config::SOUND_ID_SHOT_3
        };

        const char* id = shots[RandomUtils::Int(0, Config::SOUND_SHOT_VARIANTS - 1)];
        if (ctx.audio->HasSound(id))
        {
            ctx.audio->PlaySound(id);
        }
    }
}

void Military::StartReload(const GameContext& ctx)
{
    if (m_isReloading || m_ammo >= Config::MILITARY_AMMO)
    {
        return;
    }

    m_isReloading = true;
    m_reloadTimer = Config::MILITARY_RELOAD_TIME;
    m_stateManager.SetState(EntityState::Reload);

    if (ctx.audio != nullptr && ctx.audio->HasSound(Config::SOUND_ID_RELOAD))
    {
        ctx.audio->PlaySound(Config::SOUND_ID_RELOAD);
    }
}

void Military::OnPossessed()
{
    Host::OnPossessed();

    m_hasTarget     = false;
    m_wasSeen       = false;
    m_reactionTimer = 0.f;
    m_attackTimer   = 0.f;
    m_alertTimer    = 0.f;
    m_fireTimer     = 0.15f;
}

void Military::OnEjected()
{
    Host::OnEjected();

    m_hasTarget     = false;
    m_wasSeen       = false;
    m_reactionTimer = 0.f;
    m_attackTimer   = 0.f;
    m_alertTimer    = 0.f;
    m_fireTimer     = 0.3f;
}

sf::Vector2f Military::GetEyePosition() const
{
    return m_hitbox.getPosition() + sf::Vector2f(0.f, -10.f);
}

sf::Vector2f Military::GetShoulderPosition() const
{
    return m_hitbox.getPosition() +
           sf::Vector2f(static_cast<float>(m_lookDir) * Config::MILITARY_WEAPON_SHOULDER_X,
                        Config::MILITARY_WEAPON_SHOULDER_Y);
}

sf::Vector2f Military::GetMuzzlePosition() const
{
    float radians = m_aimAngle / Config::RAD_TO_DEG;
    sf::Vector2f direction(std::cos(radians), std::sin(radians));

    float muzzleDistance = (Config::MILITARY_WEAPON_FRAME_W - Config::MILITARY_WEAPON_GRIP_X) *
                           Config::MILITARY_WEAPON_SCALE;

    return GetShoulderPosition() + direction * muzzleDistance;
}

void Military::Draw(sf::RenderWindow& window)
{
    Host::Draw(window);

    if (m_isDead)
    {
        return;
    }

    float radians = m_aimAngle / Config::RAD_TO_DEG;
    bool aimingLeft = std::cos(radians) < 0.f;

    m_weapon.setScale(Config::MILITARY_WEAPON_SCALE,
                      aimingLeft ? -Config::MILITARY_WEAPON_SCALE : Config::MILITARY_WEAPON_SCALE);
    m_weapon.setPosition(GetShoulderPosition());
    m_weapon.setRotation(m_aimAngle);
    m_weaponAnim.Apply(m_weapon);
    window.draw(m_weapon);

    if (m_hasTarget || m_alertTimer > 0.f)
    {
        DrawAlertMark(window);
    }
}

void Military::DrawAlertMark(sf::RenderWindow& window)
{
    sf::FloatRect bounds = GetBounds();
    float x = bounds.left + bounds.width / 2.f;
    float barBottom = bounds.top - Config::ALERT_MARK_BOTTOM_OFFSET;

    sf::RectangleShape bar(sf::Vector2f(Config::ALERT_MARK_WIDTH, Config::ALERT_MARK_HEIGHT));
    bar.setOrigin(Config::ALERT_MARK_WIDTH / 2.f, Config::ALERT_MARK_HEIGHT);
    bar.setPosition(x, barBottom);
    bar.setFillColor(Config::ALERT_MARK_COLOR);
    bar.setOutlineThickness(1.f);
    bar.setOutlineColor(sf::Color::Black);
    window.draw(bar);

    sf::CircleShape dot(Config::ALERT_MARK_DOT_RADIUS);
    dot.setOrigin(Config::ALERT_MARK_DOT_RADIUS, Config::ALERT_MARK_DOT_RADIUS);
    dot.setPosition(x, barBottom + Config::ALERT_MARK_DOT_OFFSET);
    dot.setFillColor(Config::ALERT_MARK_COLOR);
    dot.setOutlineThickness(1.f);
    dot.setOutlineColor(sf::Color::Black);
    window.draw(dot);
}
