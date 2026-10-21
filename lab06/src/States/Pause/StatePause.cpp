#include "States/Pause/StatePause.hpp"
#include "States/MainMenu/StateMainMenu.hpp"
#include "Core/GameKeyboard/GameKeyboard.hpp"

StatePause::StatePause(GameDataRef data) : m_data(data) {}

void StatePause::Init()
{
    auto windowSize = m_data->window.getSize();

    m_backgroundTexture.create(windowSize.x, windowSize.y);
    m_backgroundTexture.update(m_data->window);
    m_backgroundSprite.setTexture(m_backgroundTexture);

    m_darkOverlay.setSize(sf::Vector2f(windowSize.x, windowSize.y));
    m_darkOverlay.setFillColor(sf::Color(0, 0, 0, 150));

    m_title.Init("PAUSED", m_data->headerFont, 100, 200.f, windowSize.x);

    m_menu.AddButton("Resume", m_data->headerFont, 400.f, windowSize.x, [this]() {
        m_data->manager.RemoveState(); 
    });

    m_menu.AddButton("Main Menu", m_data->headerFont, 500.f, windowSize.x, [this]() {
        m_data->manager.RemoveState(); 
        m_data->manager.AddState(StateRef(new StateMainMenu(m_data)), true);
    });
}

void StatePause::HandleInput(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (GameKeyboard::IsEventEscape(event)) {
            m_data->manager.RemoveState();
        } 
        else {
            m_menu.HandleInput(event);
        }
    }
}

void StatePause::Update(float dt)
{
    m_menu.Update();
}

void StatePause::Draw()
{
    m_data->window.draw(m_backgroundSprite);
    m_data->window.draw(m_darkOverlay);
    
    m_title.Draw(m_data->window);
    m_menu.Draw(m_data->window);
}