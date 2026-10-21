#include "MenuLabel.hpp"

void MenuLabel::Init(const sf::String& text, const sf::Font& font, int size, float yPos, float windowWidth){
    m_text.setFont(font);
    m_text.setString(text);
    m_text.setCharacterSize(size);
    
    sf::FloatRect bounds = m_text.getLocalBounds();
    m_text.setPosition(windowWidth / 2.f - bounds.width / 2.f, yPos);
}

void MenuLabel::Draw(sf::RenderWindow& window)
{
    window.draw(m_text);
}