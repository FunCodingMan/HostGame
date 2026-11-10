#pragma once
#include <functional>
#include "Core/Config.hpp"
class EntityStateManager
{
public:
    EntityStateManager();

    void SetState(Config::EntityState newState);
    
    Config::EntityState GetState() const { return m_currentState; }

    float GetTimeInState() const { return m_timeInState; }

    void Update(float dt);

    void SetOnStateChanged(std::function<void(Config::EntityState)> callback);

private:
    Config::EntityState m_currentState;
    float m_timeInState;
    std::function<void(Config::EntityState)> m_onStateChanged;
};