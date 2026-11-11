#include "UIMenu.hpp"
#include "Core/GameKeyboard/GameKeyboard.hpp"
#include "Core/Config.hpp"

using namespace Config;

UIMenu::UIMenu() : m_selectedOption(0) {}

void UIMenu::AddButton(const sf::String& text, const sf::Font& font, float yPos, float windowWidth, std::function<void()> onClick)
{
    m_buttons.emplace_back(text, font, yPos, windowWidth, onClick);
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

void UIMenu::HandleInput(const sf::Event& event, SoundManager& sound)
{
    if (GameKeyboard::IsEventUp(event)) {
        MenuUp();
        sound.PlaySound(SOUND_ID_BUTTON);
    }
    else if (GameKeyboard::IsEventDown(event)) {
        MenuDown();
        sound.PlaySound(SOUND_ID_BUTTON);
    } 
    else if (GameKeyboard::IsEventOpen(event)) {
        if (!m_buttons.empty()) {
            sound.PlaySound(SOUND_ID_BUTTON);
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