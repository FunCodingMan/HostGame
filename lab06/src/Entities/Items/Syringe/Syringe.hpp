#pragma once
#include "Entities/Entity/Entity.hpp"

class Syringe : public Entity
{
public:
    Syringe(sf::Vector2f position);

    void Update(const GameContext& ctx) override;
    
    bool IsCollected() const { return m_isCollected; }
    void Collect() { m_isCollected = true; }
private:
    bool m_isCollected = false;
    float m_floatTimer = 0.f;
    sf::Vector2f m_startPos;
};