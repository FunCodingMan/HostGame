#pragma once
#include <unordered_map>
#include <string>

class PropertiesManager
{
public:
    PropertiesManager() = default;

    void SetBool(const std::string& name, bool val);
    bool GetBool(const std::string& name, bool defaultVal = false) const;

    void SetInt(const std::string& name, int val);
    int GetInt(const std::string& name, int defaultVal = 0) const;

    void SetFloat(const std::string& name, float val);
    float GetFloat(const std::string& name, float defaultVal = 0.0f) const;

    void SetString(const std::string& name, const std::string& val);
    std::string GetString(const std::string& name, const std::string& defaultVal = "") const;

private:
    std::unordered_map<std::string, bool> m_bools;
    std::unordered_map<std::string, int> m_ints;
    std::unordered_map<std::string, float> m_floats;
    std::unordered_map<std::string, std::string> m_strings;
};