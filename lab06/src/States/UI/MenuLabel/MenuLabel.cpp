#include "MenuLabel.hpp"

void MenuLabel::Init(const std::string& textStr, const sf::Font& font, unsigned int charSize, float yPos, float windowWidth)
{
    m_text.setFont(font);
    m_text.setString(textStr);
    m_text.setCharacterSize(charSize);
    
    sf::FloatRect bounds = m_text.getLocalBounds();
    m_text.setPosition(windowWidth / 2.f - bounds.width / 2.f, yPos);
}

void MenuLabel::Draw(sf::RenderWindow& window)
{
    window.draw(m_text);
}