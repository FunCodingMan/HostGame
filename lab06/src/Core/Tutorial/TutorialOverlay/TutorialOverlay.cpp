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
    m_text.setCharacterSize(22);
    m_text.setFillColor(sf::Color::Yellow);
    m_text.setOutlineColor(sf::Color::Black);
    m_text.setOutlineThickness(2.f);
    m_bg.setFillColor(sf::Color(0, 0, 0, 180));

    GameKeyboard::SetJumpEnabled(false);
    GameKeyboard::SetDashEnabled(false);
    GameKeyboard::SetEjectEnabled(false);

    m_step = TutorialStep::Movement;
    SetText(ToSfStringFromUtf8("ТУТОРИАЛ\nИспользуйте A и D для движения"));
}

void TutorialOverlay::SetText(const sf::String &str)
{
    m_text.setString(str);
    sf::FloatRect bounds = m_text.getLocalBounds();
    m_text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    m_text.setPosition(m_windowWidth / 2.f, 120.f);

    m_bg.setSize(sf::Vector2f(bounds.width + 40.f, bounds.height + 30.f));
    m_bg.setOrigin(m_bg.getSize().x / 2.f, m_bg.getSize().y / 2.f);
    m_bg.setPosition(m_windowWidth / 2.f, 120.f);
}

void TutorialOverlay::Update(float dt, const Parasite& parasite, const Host* currentHost, bool syringeCollected)
{
    switch (m_step)
    {
    case TutorialStep::Movement:
        if (GameKeyboard::IsLeftKeyPressed() || GameKeyboard::IsRightKeyPressed())
        {
            m_timer += dt;
            if (m_timer > 0.5f)
            {
                m_step = TutorialStep::Dash;
                m_timer = 0.f;
                SetText(ToSfStringFromUtf8("ЛКМ - рывок к курсору\nПопробуйте пробить стекло!"));
                GameKeyboard::SetDashEnabled(true);
            }
        }
        break;
    case TutorialStep::Dash:
        if (parasite.GetBrokenTilesCount() > 0) 
        {
            m_step = Config::TutorialStep::Jump;
            SetText(ToSfStringFromUtf8("ПРОБЕЛ - прыжок"));
            GameKeyboard::SetJumpEnabled(true);
        }
        break;
    case TutorialStep::Jump:
        if (GameKeyboard::IsJumpKeyPressed())
        {
            m_timer += dt;
            if (m_timer > 0.2f)
            {
                m_step = TutorialStep::Possess;
                m_timer = 0.f;
                SetText(ToSfStringFromUtf8(
                "Найдите Учёного и сделайте рывок (ЛКМ)\nпрямо в него, чтобы захватить тело!"));
                
            }
        }
        break;
    case Config::TutorialStep::Possess:
        if (currentHost != nullptr)
        {
            m_step = Config::TutorialStep::HostInfo;
            SetText(ToSfStringFromUtf8("Тело учёного открывает двери лифта.\nНо оно быстро разлагается (полоска сверху)!"));
        }
        break;
    case Config::TutorialStep::HostInfo:
        m_timer += dt;
        if (m_timer > 4.0f)
        {
            m_step = Config::TutorialStep::Heal;
            SetText(ToSfStringFromUtf8("Пройдите вправо и подберите шприц,\nчтобы восстановить время жизни тела!"));
        }
        break;
    case Config::TutorialStep::Heal:
        if (syringeCollected)
        {
            m_step = Config::TutorialStep::Eject;
            m_timer = 0.f;
            SetText(ToSfStringFromUtf8("Отлично! А теперь нажмите ПКМ,\nчтобы экстренно покинуть тело."));
            
            GameKeyboard::SetEjectEnabled(true);
        }
        break;
    case Config::TutorialStep::Eject:
        if (currentHost == nullptr)
        {
            m_step = Config::TutorialStep::Exit;
            SetText(ToSfStringFromUtf8("Вы разорвали носителя! Но впереди есть ещё один.\nЗахватите его и заходите в лифт!"));
        }
        break;
    case Config::TutorialStep::Exit:
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