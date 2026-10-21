#include "States/Tutorial/StateTutorial.hpp"
#include "States/Playing/StatePlaying.hpp"
#include "States/MainMenu/StateMainMenu.hpp"
#include "Core/GameKeyboard/GameKeyboard.hpp"
#include "Core/Config.hpp"

StateTutorial::StateTutorial(GameDataRef data) : m_data(data) {}

void StateTutorial::Init()
{
    auto windowSize = m_data->window.getSize();

    m_title.Init(L"КАК ИГРАТЬ", m_data->headerFont, 70, 50.f, windowSize.x);

    m_instructions.setFont(m_data->headerFont);
    m_instructions.setCharacterSize(30);
    m_instructions.setFillColor(sf::Color::White);
    
    m_instructions.setString(
        L"ЦЕЛЬ:\nДобраться до лифта в конце уровня.\n\n"
        L"УПРАВЛЕНИЕ:\n"
        L"A / D - Движение\n"
        L"ПРОБЕЛ - Прыжок\n"
        L"ЛКМ (Левый клик) - Рывок / Вселение во врага\n"
        L"ПКМ (Правый клик) - Разорвать носителя и выпрыгнуть\n\n"
        L"ВНИМАНИЕ:\n"
        L"Падение в пропасть — мгновенная смерть.\n"
        L"Тело носителя разлагается за 15 секунд!"
    );

    sf::FloatRect textRect = m_instructions.getLocalBounds();
    m_instructions.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top);
    m_instructions.setPosition(windowSize.x / 2.0f, 160.f);

    m_menu.AddButton(L"Начать игру", m_data->headerFont, 550.f, windowSize.x, [this]() {
        m_data->manager.RemoveState();
        m_data->manager.AddState(StateRef(new StatePlaying(m_data)), true);
    });

    m_menu.AddButton(L"В главное меню", m_data->headerFont, 630.f, windowSize.x, [this]() {
        m_data->manager.RemoveState();
        m_data->manager.AddState(StateRef(new StateMainMenu(m_data)), true);
    });
}

void StateTutorial::HandleInput(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (GameKeyboard::IsEventEscape(event))
        {
            m_data->manager.RemoveState();
            m_data->manager.AddState(StateRef(new StateMainMenu(m_data)), true);
        }
        else
        {
            m_menu.HandleInput(event);
        }
    }
}

void StateTutorial::Update(float dt)
{
    m_menu.Update();
}

void StateTutorial::Draw()
{
    m_data->window.clear(Config::BG_COLOR);
    
    m_title.Draw(m_data->window);
    m_data->window.draw(m_instructions);
    m_menu.Draw(m_data->window);
}