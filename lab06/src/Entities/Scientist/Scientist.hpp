#pragma once
#include "Entities/Host/Host.hpp"

class Scientist : public Host
{
public:
    Scientist(sf::Vector2f position);

protected:
    Config::EntityType GetEntityType() const override { return Config::EntityType::Human; }
    const char* GetWalkSound() const override { return Config::SOUND_ID_HOST_WALK; }
    const char* GetDamageSound() const override { return Config::SOUND_ID_SCIENTIST_DAMAGE; }
    void UpdateAI(const GameContext& ctx) override;

private:
    void StartWalking(const GameContext& ctx);
    bool IsBlockedAhead(const GameContext& ctx, int dir) const;

    int m_moveDir = 1;
    float m_walkTimer = 0.f;
    float m_idleTimer = 0.f;
    bool m_isWalking = false;
};