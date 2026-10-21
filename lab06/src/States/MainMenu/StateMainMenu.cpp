#include "States/MainMenu/StateMainMenu.hpp"
#include "States/Playing/StatePlaying.hpp"
#include "States/Tutorial/StateTutorial.hpp"

StateMainMenu::StateMainMenu(GameDataRef data) : m_data(data) {}

void StateMainMenu::Init()
{
    auto windowSize = m_data->window.getSize();

    m_title.Init("THE HOST", m_data->headerFont, 100, 150.f, windowSize.x);

    m_menu.AddButton("Play", m_data->headerFont, 400.f, windowSize.x, [this]() {
    m_data->manager.RemoveState();
    m_data->manager.AddState(StateRef(new StateTutorial(m_data)), true);
});

    m_menu.AddButton("Exit", m_data->headerFont, 500.f, windowSize.x, [this]() {
        m_data->window.close();
    });
}

void StateMainMenu::HandleInput(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed) {
        m_menu.HandleInput(event);
    }
}

void StateMainMenu::Update(float dt)
{
    m_menu.Update();
}

void StateMainMenu::Draw()
{
    m_title.Draw(m_data->window);
    m_menu.Draw(m_data->window);
}