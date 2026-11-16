#include "TutorialSystem.hpp"
#include "Core/Utils/StringUtils/StringUtils.hpp"
#include <fstream>
#include <iostream>

using namespace StringUtils;

void TutorialSystem::Init(const sf::Font& font)
{
    m_text.setFont(font);
    m_text.setCharacterSize(26);
    m_text.setFillColor(sf::Color::Yellow);
    m_text.setOutlineColor(sf::Color::Black);
    m_text.setOutlineThickness(2.f);
    m_text.setPosition(20.f, 110.f);
}

bool TutorialSystem::LoadFromFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "[TutorialSystem] Cannot open '" << path << "'\n";
        return false;
    }

    m_steps.clear();

    std::string line;
    int lineNum = 0;
    while (std::getline(file, line))
    {
        ++lineNum;

        if (lineNum == 1 && line.size() >= 3 &&
            (unsigned char)line[0] == 0xEF &&
            (unsigned char)line[1] == 0xBB &&
            (unsigned char)line[2] == 0xBF)
        {
            line = line.substr(3);
        }

        if (line.empty() || line[0] == '#') continue;

        auto sep = line.find('|');
        if (sep == std::string::npos)
        {
            std::cerr << "[TutorialSystem] " << path << ":" << lineNum
                      << " invalid format (expected 'flag | hint')\n";
            continue;
        }

        std::string condition = line.substr(0, sep);
        std::string hint      = line.substr(sep + 1);

        size_t s = condition.find_first_not_of(" \t");
        size_t e = condition.find_last_not_of(" \t");
        if (s == std::string::npos) continue;
        condition = condition.substr(s, e - s + 1);

        s = hint.find_first_not_of(" \t");
        if (s != std::string::npos) hint = hint.substr(s);

        std::string processed;
        processed.reserve(hint.size());
        for (size_t i = 0; i < hint.size(); ++i)
        {
            if (hint[i] == '\\' && i + 1 < hint.size() && hint[i + 1] == 'n')
            {
                processed += '\n';
                ++i;
            }
            else
            {
                processed += hint[i];
            }
        }

        m_steps.push_back({ condition, ToSfStringFromUtf8(processed) });
    }

    return !m_steps.empty();
}

void TutorialSystem::SetFlag(const std::string& name)
{
    m_flags[name] = true;
}

bool TutorialSystem::HasFlag(const std::string& name) const
{
    auto it = m_flags.find(name);
    return (it != m_flags.end()) && it->second;
}

void TutorialSystem::Update(float dt)
{
    if (!m_active || m_finished || m_steps.empty()) return;

    m_stepTimer += dt;
    if (m_stepTimer < MIN_STEP_TIME) return;

    if (HasFlag(m_steps[m_current].condition))
    {
        ++m_current;
        m_stepTimer = 0.f;
        if (m_current >= m_steps.size())
        {
            m_finished = true;
            m_active   = false;
        }
    }
}

void TutorialSystem::Draw(sf::RenderWindow& window) const
{
    if (!m_active || m_finished || m_steps.empty()) return;

    m_text.setString(m_steps[m_current].hint);
    window.draw(m_text);
}