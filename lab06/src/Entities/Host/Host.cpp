#include "Entities/Host/Host.hpp"

Host::Host(float maxLife, sf::Vector2f position, float decayTime)
    : Character(maxLife, position)
{
    m_maxDecayTimer = decayTime;
    m_decayTimer    = decayTime;
}