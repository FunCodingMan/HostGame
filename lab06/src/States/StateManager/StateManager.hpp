#pragma once
#include <memory>
#include <stack>
#include "States/State.hpp"

typedef std::unique_ptr<State> StateRef;

class StateManager
{
public:
    void AddState(StateRef newState, bool isReplacing = true);
    void RemoveState();
    void ProcessStateChanges();

    StateRef& GetActiveState();
    bool isEmpty() const { return m_states.empty(); }
private:
    void ProcessRemoveState();
    void ProcessAddState();

    std::stack<StateRef> m_states;
    StateRef m_newState;

    bool m_isRemoving = false;
    bool m_isAdding = false;
    bool m_isReplacing = false;
};
