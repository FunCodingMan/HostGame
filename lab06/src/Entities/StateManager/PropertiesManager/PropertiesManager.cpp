#include "Entities/StateManager/PropertiesManager/PropertiesManager.hpp"

void PropertiesManager::SetBool(const std::string& name, bool val) {
    m_bools[name] = val;
}
bool PropertiesManager::GetBool(const std::string& name, bool defaultVal) const {
    auto it = m_bools.find(name);
    return (it != m_bools.end()) ? it->second : defaultVal;
}

void PropertiesManager::SetInt(const std::string& name, int val) {
    m_ints[name] = val;
}
int PropertiesManager::GetInt(const std::string& name, int defaultVal) const {
    auto it = m_ints.find(name);
    return (it != m_ints.end()) ? it->second : defaultVal;
}

void PropertiesManager::SetFloat(const std::string& name, float val) {
    m_floats[name] = val;
}
float PropertiesManager::GetFloat(const std::string& name, float defaultVal) const {
    auto it = m_floats.find(name);
    return (it != m_floats.end()) ? it->second : defaultVal;
}

void PropertiesManager::SetString(const std::string& name, const std::string& val) {
    m_strings[name] = val;
}
std::string PropertiesManager::GetString(const std::string& name, const std::string& defaultVal) const {
    auto it = m_strings.find(name);
    return (it != m_strings.end()) ? it->second : defaultVal;
}