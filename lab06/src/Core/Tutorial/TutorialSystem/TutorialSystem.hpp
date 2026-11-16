#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <unordered_map>

class TutorialSystem
{
public:
    void Init(const sf::Font& font);
    bool LoadFromFile(const std::string& path);

    void SetFlag(const std::string& name);
    bool HasFlag(const std::string& name) const;

    void Update(float dt);
    void Draw(sf::RenderWindow& window) const;

    bool IsFinished() const { return m_finished; }
    bool IsActive()   const { return m_active; }

private:
    struct Step
    {
        std::string condition;
        sf::String  hint;
    };

    std::vector<Step> m_steps;
    std::unordered_map<std::string, bool> m_flags;

    size_t m_current   = 0;
    float  m_stepTimer = 0.f;
    bool   m_finished  = false;
    bool   m_active    = true;

    sf::Text m_text;

    static constexpr float MIN_STEP_TIME = 0.7f;
};