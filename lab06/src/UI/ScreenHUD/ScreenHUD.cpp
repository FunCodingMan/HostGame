#include "ScreenHUD.hpp"
#include <sstream>
#include <iomanip>

void ScreenHUD::Init(const sf::Font& font, float windowWidth)
{
    m_hintText.setFont(font);
    m_hintText.setCharacterSize(24);
    m_hintText.setFillColor(sf::Color::White);
    m_hintText.setOutlineColor(sf::Color::Black);
    m_hintText.setOutlineThickness(2.f);
    m_hintText.setPosition(20.f, 20.f); 

    m_timerText.setFont(font);
    m_timerText.setCharacterSize(24);
    m_timerText.setFillColor(sf::Color::White);
    m_timerText.setOutlineColor(sf::Color::Black);
    m_timerText.setOutlineThickness(2.f);
}

void ScreenHUD::SetHintText(const sf::String& text)
{
    m_hintText.setString(text);
}

void ScreenHUD::SetTimerText(float timeInSeconds, float windowWidth)
{
    int minutes = static_cast<int>(timeInSeconds) / 60;
    float seconds = timeInSeconds - (minutes * 60);

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << minutes << ":" 
       << std::setfill('0') << std::setw(4) << std::fixed << std::setprecision(1) << seconds;

    m_timerText.setString(ss.str());

    sf::FloatRect bounds = m_timerText.getLocalBounds();
    m_timerText.setPosition(windowWidth - bounds.width - 20.f, 20.f);
}

void ScreenHUD::Draw(sf::RenderWindow& window)
{
    window.draw(m_hintText);
    window.draw(m_timerText);
}