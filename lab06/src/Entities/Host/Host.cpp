#include "Entities/Host/Host.hpp"

Host::Host(float maxLife, sf::Vector2f position, float decayTime)
    : Character(maxLife, position)
{
    m_maxDecayTimer = decayTime;
    m_decayTimer    = decayTime;
}

void Host::AddDecayTime(float time)
{
        m_decayTimer += time;
        if (m_decayTimer > m_maxDecayTimer) 
        {
            m_decayTimer = m_maxDecayTimer;
        }
}