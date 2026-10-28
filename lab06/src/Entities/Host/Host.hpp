#pragma once
#include <SFML/Graphics.hpp>
#include "Entities/Character/Character.hpp"

class Host : public Character
{
public:
    Host(float maxLife, sf::Vector2f position, float decayTime);

    void  ResetDecayTimer()                 { m_decayTimer = m_maxDecayTimer; }
    float GetDecayTimer()    const override { return m_decayTimer; }
    float GetMaxDecayTimer() const override { return m_maxDecayTimer; }

protected:
    float m_decayTimer    = 0.f;
    float m_maxDecayTimer = 0.f;
};