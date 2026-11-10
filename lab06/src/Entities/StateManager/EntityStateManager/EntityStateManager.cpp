#include "Entities/StateManager/EntityStateManager/EntityStateManager.hpp"
#include "Core/Config.hpp"

using namespace Config;

EntityStateManager::EntityStateManager()
    : m_currentState(Config::EntityState::None), m_timeInState(0.f)
{
}

void EntityStateManager::SetState(EntityState newState)
{
    if (m_currentState == newState) return;

    m_currentState = newState;
    m_timeInState = 0.f;

    if (m_onStateChanged)
    {
        m_onStateChanged(m_currentState);
    }
}

void EntityStateManager::Update(float dt)
{
    m_timeInState += dt;
}

void EntityStateManager::SetOnStateChanged(std::function<void(EntityState)> callback)
{
    m_onStateChanged = callback;
}