#include "UIMenu.hpp"
#include "Core/GameKeyboard/GameKeyboard.hpp"

UIMenu::UIMenu() : m_selectedOption(0) {}

void UIMenu::AddButton(const std::string& textStr, const sf::Font& font, float yPos, float windowWidth, std::function<void()> action)
{
    m_buttons.emplace_back(textStr, font, yPos, windowWidth, action);
}

void UIMenu::MenuUp()
{
    if (m_buttons.empty()) return;
    if (m_selectedOption > 0) m_selectedOption--;
    else m_selectedOption = m_buttons.size() - 1;
}

void UIMenu::MenuDown()
{
    if (m_buttons.empty()) return;
    if (m_selectedOption < m_buttons.size() - 1) m_selectedOption++;
    else m_selectedOption = 0;
}

void UIMenu::HandleInput(const sf::Event& event)
{
    if (GameKeyboard::IsEventUp(event)) {
        MenuUp();
    }
    else if (GameKeyboard::IsEventDown(event)) {
        MenuDown();
    } 
    else if (GameKeyboard::IsEventOpen(event)) {
        if (!m_buttons.empty()) {
            m_buttons[m_selectedOption].OnClick();
        }
    }
}

void UIMenu::Update()
{
    for (size_t i = 0; i < m_buttons.size(); ++i) {
        m_buttons[i].SetSelected(i == m_selectedOption);
    }
}

void UIMenu::Draw(sf::RenderWindow& window)
{
    for (auto& btn : m_buttons) {
        btn.Draw(window);
    }
}