#include <SFML/Graphics.hpp>
#include <functional>
#include "MenuButton.hpp"
#include "Core/Config.hpp"

MenuButton::MenuButton(const sf::String& text, const sf::Font& font, float yPos, float windowWidth, std::function<void()> action) : m_action(action)
{
    m_text.setFont(font);
    m_text.setString(text);
    m_text.setCharacterSize(Config::MENU_BUTTON_TEXT_SIZE);

    sf::FloatRect bounds = m_text.getLocalBounds();
    m_text.setPosition(windowWidth / 2.f - bounds.width / 2.f, yPos);
}

void MenuButton::OnClick()
{
    if (m_action)
    {
        m_action();
    }
}

void MenuButton::SetSelected(bool isSelected)
{
    m_text.setFillColor(isSelected ? sf::Color::Red : sf::Color::White);
}

void MenuButton::Draw(sf::RenderWindow& window)
{
    window.draw(m_text);
}