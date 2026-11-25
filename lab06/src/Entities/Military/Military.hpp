#pragma once
#include <SFML/Graphics.hpp>
#include "Entities/Host/Host.hpp"

class Military : public Host
{
public:
    Military(sf::Vector2f position);

    void Draw(sf::RenderWindow& window) override;

    int  GetAmmo()      const { return m_ammo; }
    bool IsReloading()  const { return m_isReloading; }
    bool HasTarget()    const { return m_hasTarget; }

protected:
    Config::EntityType GetEntityType() const override { return Config::EntityType::Military; }
    const char* GetJumpSound() const override { return Config::SOUND_ID_JUMP; }
    const char* GetLandSound() const override { return Config::SOUND_ID_SMASH; }
    const char* GetWalkSound() const override { return Config::SOUND_ID_HOST_WALK; }
    const char* GetDeathSound() const override { return Config::SOUND_ID_MILITARY_DEATH; }
    const char* GetDamageSound() const override { return Config::SOUND_ID_MILITARY_DAMAGE; }

    void UpdateControlled(const GameContext& ctx) override;
    void UpdateAI(const GameContext& ctx) override;
    void OnPossessed() override;
    void OnEjected() override;

private:
    void UpdateTimers(const GameContext& ctx);
    void UpdatePatrol(const GameContext& ctx);
    void UpdateWeaponAim(sf::Vector2f point);
    void TryFire(const GameContext& ctx, bool fromPlayer);
    void StartReload(const GameContext& ctx);

    sf::Vector2f GetEyePosition() const;
    sf::Vector2f GetShoulderPosition() const;
    sf::Vector2f GetMuzzlePosition() const;

    sf::Texture m_weaponTexture;
    sf::Sprite m_weapon;
    AnimationController m_weaponAnim;

    float m_aimAngle    = 0.f;
    float m_reactionTimer = 0.f;
    float m_fireTimer     = 0.f;
    float m_reloadTimer   = 0.f;
    float m_attackTimer   = 0.f;
    float m_alertTimer    = 0.f;
    float m_patrolDir     = 1.f;

    bool m_isReloading = false;
    bool m_hasTarget   = false;
    bool m_wasSeen     = false;

    int m_ammo = Config::MILITARY_AMMO;
};
