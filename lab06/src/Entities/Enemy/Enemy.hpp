#pragma once
#include <SFML/Graphics.hpp>
#include "Entities/LivingEntity/LivingEntity.hpp"
#include "Core/GameContext.hpp"

class Enemy : public LivingEntity
{
public:
    Enemy(sf::Vector2f position);
    
    void Update(const GameContext& ctx) override;

    void Draw(sf::RenderWindow& window) override;

    float GetDecayTimer() const override { return m_decayTimer; }
    float GetMaxDecayTimer() const override { return m_maxDecayTimer; }
    void ResetDecayTimer() { m_decayTimer = m_maxDecayTimer; }

private:
    float m_maxDecayTimer = 15.f;
    float m_decayTimer = 15.f;
};