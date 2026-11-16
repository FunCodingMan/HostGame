#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Entities/Character/Character.hpp"

class Parasite : public Character
{
public:
    Parasite(sf::Vector2f position);

    void Update(const GameContext& ctx) override;
    bool IsDashing() const;
    void ForceDash(const GameContext& ctx);

    int GetJumpCount()        const { return m_jumpCount; }
    int GetDashCount()        const { return m_dashCount; }
    int GetBrokenTilesCount() const { return m_brokenTiles; }

protected:
    Config::EntityType GetEntityType() const override { return Config::EntityType::Parasite; }
    void               OnLanding() override { m_hasDash = true; }
    void               OnJump()    override { ++m_jumpCount; }
    bool               TryBreakTile(int x, int y, const GameContext& ctx) override;

    void UpdateHorizontal(const GameContext& ctx) override;
    void UpdateMove(const GameContext& ctx) override;
    const char* GetJumpSound() const override { return Config::SOUND_ID_JUMP; }
    const char* GetLandSound() const override { return Config::SOUND_ID_SMASH; }
    const char* GetWalkSound() const override { return Config::SOUND_ID_WALK; }

private:
    void HorizontalDash(float dt);
    void UpdateDash(const GameContext& ctx);
    void UpdateDashTimer(float dt);

    float m_dashCooldown = 0.f;
    float m_dashForce    = Config::PARASITE_DASH_FORCE;
    bool  m_hasDash      = true;

    int m_jumpCount   = 0;
    int m_dashCount   = 0;
    int m_brokenTiles = 0;
};