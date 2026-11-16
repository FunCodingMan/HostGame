#pragma once
#include <SFML/Graphics.hpp>
#include "Entities/Parasite/Parasite.hpp"
#include "Entities/Host/Host.hpp"
#include "Core/Config.hpp"


class TutorialOverlay {
public:
    void Init(const sf::Font& font, float windowWidth);
    void Update(float dt, const Parasite& parasite, const Host* currentHost, bool syringeCollected);
    void Draw(sf::RenderWindow& window);

private:
    void SetText(const sf::String& str);

    Config::TutorialStep m_step = Config::TutorialStep::Movement;
    sf::Text m_text;
    sf::RectangleShape m_bg;
    float m_windowWidth = 0.f;
    float m_timer = 0.f;
};