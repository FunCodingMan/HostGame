#include "Core/../Entities/StateManger/PropertiesManager.hpp"

PropertiesManager::PropertiesManager() {
    m_states = {};
}

void PropertiesManager::SetVal(const std::string& name, bool val)
{
    m_states[name] = val;
}

bool PropertiesManager::GetValOfState(const std::string& name) const
{
    auto it = m_states.find(name);
    return it != m_states.end() && it->second;
}