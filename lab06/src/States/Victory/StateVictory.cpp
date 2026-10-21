#include "States/Victory/StateVictory.hpp"

#include "States/MainMenu/StateMainMenu.hpp"
#include "States/Playing/StatePlaying.hpp"

StateVictory::StateVictory(GameDataRef data) : m_data(data) {}

void StateVictory::Init()
{
    auto windowSize = m_data->window.getSize();

    m_darkOverlay.setSize(sf::Vector2f(windowSize.x, windowSize.y));
    m_darkOverlay.setFillColor(sf::Color(0, 100, 0, 150));

    m_title.Init("LEVEL CLEARED!", m_data->headerFont, 100, 200.f, windowSize.x);

    m_menu.AddButton("Play Again", m_data->headerFont, 400.f, windowSize.x,
                     [this]()
                     {
                         m_data->manager.RemoveState();
                         m_data->manager.AddState(StateRef(new StatePlaying(m_data)), true);
                     });

    m_menu.AddButton("Main Menu", m_data->headerFont, 500.f, windowSize.x,
                     [this]()
                     {
                         m_data->manager.RemoveState();
                         m_data->manager.AddState(StateRef(new StateMainMenu(m_data)), true);
                     });
}

void StateVictory::HandleInput(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        m_menu.HandleInput(event);
    }
}
void StateVictory::Update(float dt)
{
    m_menu.Update();
}

void StateVictory::Draw()
{
    m_data->window.setView(m_data->window.getDefaultView());
    m_data->window.draw(m_darkOverlay);
    m_title.Draw(m_data->window);
    m_menu.Draw(m_data->window);
}