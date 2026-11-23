#pragma once
#include <SFML/Graphics.hpp>
#include "Entities/Character/Character.hpp"

class Host : public Character
{
public:
    Host(float maxLife, sf::Vector2f position, float decayTime);

    void Update(const GameContext& ctx) override;
    void Draw(sf::RenderWindow& window) override;

    void  ResetDecayTimer()                 { m_decayTimer = m_maxDecayTimer; }
    float GetDecayTimer()    const override { return m_decayTimer; }
    float GetMaxDecayTimer() const override { return m_maxDecayTimer; }
    void AddDecayTime(float time);
    void SetMaxDecayTime(float time) { m_maxDecayTimer = time; m_decayTimer = time; }

    virtual void OnPossessed();
    virtual void OnEjected();
    void Kill() { Die(); }

    float GetCorpseTime() const { return m_corpseTimer; }
    bool  IsCorpse()      const { return m_isDead; }

protected:
    void Die() override;

    virtual void UpdateControlled(const GameContext& ctx);
    virtual void UpdateAI(const GameContext& ctx);
    void UpdateMovementState();

    float m_decayTimer    = 0.f;
    float m_maxDecayTimer = 0.f;
    float m_corpseTimer   = 0.f;
};
