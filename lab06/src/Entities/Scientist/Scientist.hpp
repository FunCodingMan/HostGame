#pragma once
#include <SFML/Graphics.hpp>
#include "Entities/Character/Character.hpp"

class Scientist : public Character
{
public:
    Scientist(sf::Vector2f position);

    void Update(const GameContext& ctx) override;

    void  ResetDecayTimer()                 { m_decayTimer = m_maxDecayTimer; }
    float GetDecayTimer()    const override { return m_decayTimer; }
    float GetMaxDecayTimer() const override { return m_maxDecayTimer; }

protected:
    Config::EntityType GetEntityType() const override { return Config::EntityType::Human; }
    const char*        GetAnimationName() const override;

private:
    float m_decayTimer    = 0.f;
    float m_maxDecayTimer = 0.f;
};