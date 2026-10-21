#pragma once
#include "Entities/Entity/Entity.hpp"

class LivingEntity : public Entity
{
public:
    LivingEntity(float maxLife) 
        : m_maxLife(maxLife), m_life(maxLife), m_isDead(false), m_isPlayerControlled(false) {}

    virtual ~LivingEntity() = default;

    float GetLife() const { return m_life; }
    float GetMaxLife() const { return m_maxLife; }
    bool IsDead() const { return m_isDead; }

    void SetPlayerControlled(bool isControlled) { m_isPlayerControlled = isControlled; }
    bool IsPlayerControlled() const { return m_isPlayerControlled; }

    virtual float GetDecayTimer() const { return 0.f; }
    virtual float GetMaxDecayTimer() const { return 1.f; }

    virtual void TakeDamage(float amount)
    {
        if (m_isDead) return;
        m_life -= amount;
        if (m_life <= 0.f)
        {
            m_life = 0.f;
            Die();
        }
    }

protected:
    virtual void Die() 
    {
        m_isDead = true;
    }

    float m_life;
    float m_maxLife;
    bool m_isDead;
    bool m_isPlayerControlled;
};