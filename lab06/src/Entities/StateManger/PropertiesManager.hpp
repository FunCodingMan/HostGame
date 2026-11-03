#pragma once
#include <map>
#include <string>

class PropertiesManager
{
public:
    PropertiesManager();
    bool GetValOfState(const std::string& name) const;
    void SetVal(const std::string& name, bool val);
private:
    std::map<std::string, bool> m_states;


};