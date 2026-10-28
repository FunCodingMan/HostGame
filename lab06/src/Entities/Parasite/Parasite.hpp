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
    void ForceDash(sf::Vector2f targetPos);

protected:
    Config::EntityType GetEntityType() const override { return Config::EntityType::Parasite; }
    const char*        GetAnimationName() const override { return "idle"; }
    void               OnLanding() override { m_hasDash = true; }
    bool               TryBreakTile(int x, int y, const GameContext& ctx) override;

    void UpdateHorizontal(float dt) override;
    void UpdateMove(float dt) override;

private:
    void HorizontalDash(float dt);
    void UpdateDash(float dt, sf::Vector2f mousePos);
    void UpdateDashTimer(float dt);

    float m_dashCooldown = 0.f;
    float m_dashForce    = Config::PARASITE_DASH_FORCE;
    bool  m_hasDash      = true;
    float m_dashTimer    = 0.f;
};