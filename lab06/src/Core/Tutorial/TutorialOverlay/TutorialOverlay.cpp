// Path: ./src/Core/Tutorial/TutorialOverlay/TutorialOverlay.cpp
#include "TutorialOverlay.hpp"
#include "Core/Config.hpp"
#include "Core/GameKeyboard/GameKeyboard.hpp"
#include "Core/Utils/StringUtils/StringUtils.hpp"

using namespace StringUtils;
using namespace Config;

void TutorialOverlay::Init(const sf::Font &font, float windowWidth)
{
    m_windowWidth = windowWidth;
    m_text.setFont(font);
    m_text.setCharacterSize(24);
    m_text.setFillColor(sf::Color::Yellow);
    m_text.setOutlineColor(sf::Color::Black);
    m_text.setOutlineThickness(2.f);
    m_bg.setFillColor(sf::Color(0, 0, 0, 200));

    GameKeyboard::SetJumpEnabled(false);
    GameKeyboard::SetDashEnabled(false);
    GameKeyboard::SetEjectEnabled(false);

    m_step = TutorialStep::Movement;
    SetText(ToSfStringFromUtf8("ОБУЧЕНИЕ\nИспользуйте A/D или ←/→ для движения"));
}

void TutorialOverlay::SetText(const sf::String &str)
{
    m_text.setString(str);
    sf::FloatRect bounds = m_text.getLocalBounds();
    m_text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    m_text.setPosition(m_windowWidth / 2.f, 100.f);

    m_bg.setSize(sf::Vector2f(bounds.width + 60.f, bounds.height + 40.f));
    m_bg.setOrigin(m_bg.getSize().x / 2.f, m_bg.getSize().y / 2.f);
    m_bg.setPosition(m_windowWidth / 2.f, 100.f);
}

void TutorialOverlay::Update(float dt, const Parasite& parasite, const Host* currentHost, int syringesCollected)
{
    switch (m_step)
    {
    case TutorialStep::Movement:
        if (GameKeyboard::IsLeftKeyPressed() || GameKeyboard::IsRightKeyPressed())
        {
            m_timer += dt;
            if (m_timer > 0.8f)
            {
                m_step = TutorialStep::Dash;
                m_timer = 0.f;
                SetText(ToSfStringFromUtf8("РЫВОК\nНажмите ЛКМ, чтобы пробить стекло"));
                GameKeyboard::SetDashEnabled(true);
            }
        }
        break;
    case TutorialStep::Dash:
        if (parasite.GetBrokenTilesCount() > 0) 
        {
            m_step = TutorialStep::Jump;
            SetText(ToSfStringFromUtf8("ПРЫЖОК\nНажмите ПРОБЕЛ, чтобы перепрыгнуть препятствия"));
            GameKeyboard::SetJumpEnabled(true);
        }
        break;
    case TutorialStep::Jump:
        if (GameKeyboard::IsJumpKeyPressed())
        {
            m_timer += dt;
            if (m_timer > 0.5f)
            {
                m_step = TutorialStep::Possess;
                m_timer = 0.f;
                SetText(ToSfStringFromUtf8("ВСЕЛЕНИЕ\nСделайте рывок (ЛКМ) прямо в Учёного,\nчтобы захватить его тело!"));
            }
        }
        break;
    case TutorialStep::Possess:
        if (currentHost != nullptr)
        {
            m_step = TutorialStep::HostInfo;
            SetText(ToSfStringFromUtf8("УСПЕХ!\nТело учёного открывает двери.\nНо оно быстро разлагается (полоска сверху)!"));
        }
        break;
    case TutorialStep::HostInfo:
        m_timer += dt;
        if (m_timer > 3.5f)
        {
            m_step = TutorialStep::Heal;
            SetText(ToSfStringFromUtf8("ЛЕЧЕНИЕ\nПодберите шприц, чтобы восстановить\nвремя жизни тела!"));
        }
        break;
    case TutorialStep::Heal:
        if (syringesCollected > 0)
        {
            m_step = TutorialStep::Eject;
            m_timer = 0.f;
            SetText(ToSfStringFromUtf8("ОТЛИЧНО!\nНажмите ПКМ, чтобы экстренно\nпокинуть тело перед финишем."));
            GameKeyboard::SetEjectEnabled(true);
        }
        break;
    case TutorialStep::Eject:
        if (currentHost == nullptr)
        {
            m_step = TutorialStep::Exit;
            SetText(ToSfStringFromUtf8("ВХОД В ЛИФТ\nЗаходите в лифт (el), чтобы завершить обучение!"));
        }
        break;
    case TutorialStep::Exit:
        break;
    default:
        break;
    }
}

void TutorialOverlay::Draw(sf::RenderWindow &window)
{
    window.draw(m_bg);
    window.draw(m_text);
}