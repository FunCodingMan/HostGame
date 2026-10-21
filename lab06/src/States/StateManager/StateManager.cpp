#include "States/StateManager/StateManager.hpp"

void StateManager::AddState(StateRef newState, bool isReplacing)
{
    this->m_isAdding = true;
    this->m_isReplacing = isReplacing;
    this->m_newState = std::move(newState);
}

void StateManager::RemoveState()
{
    this->m_isRemoving = true;
}

void StateManager::ProcessRemoveState()
{
    this->m_states.pop();
    if (!this->m_states.empty())
    {
        this->m_states.top()->Resume();
    }
    this->m_isRemoving = false;    
}

void StateManager::ProcessAddState()
{
    if (!this->m_states.empty())
    {
        if (this->m_isReplacing)
        {
            this->m_states.pop();
        }
        else
        {
            this->m_states.top()->Pause();
        }
    }
    this->m_states.push(std::move(m_newState));
    this->m_states.top()->Init();
    this->m_isAdding = false;    
}

void StateManager::ProcessStateChanges()
{
    if (this->m_isRemoving && !this->m_states.empty())
    {
        ProcessRemoveState();
    }

    if (this->m_isAdding)
    {
        ProcessAddState();
    }
}

StateRef& StateManager::GetActiveState()
{
    return this->m_states.top();
}