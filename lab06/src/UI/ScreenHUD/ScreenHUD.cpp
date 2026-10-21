#include "ScreenHUD.hpp"

void ScreenHUD::Init(const sf::Font& font, float windowWidth)
{
    m_hintText.setFont(font);
    m_hintText.setCharacterSize(24);
    m_hintText.setFillColor(sf::Color::White);
    m_hintText.setOutlineColor(sf::Color::Black);
    m_hintText.setOutlineThickness(2.f);
    
    m_hintText.setPosition(20.f, 20.f); 
}

void ScreenHUD::SetHintText(const std::string& text)
{
    m_hintText.setString(text);
}

void ScreenHUD::Draw(sf::RenderWindow& window)
{
    window.draw(m_hintText);
}