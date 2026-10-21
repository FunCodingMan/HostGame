#include "Core/Game/Game.hpp"
#include "Core/Config.hpp"
#include "States/MainMenu/StateMainMenu.hpp"

Game::Game() 
{
    m_data->window.create(sf::VideoMode::getDesktopMode(), "The Host", sf::Style::Fullscreen);
    m_data->window.setKeyRepeatEnabled(false);
    m_data->window.setVerticalSyncEnabled(true);
    m_data->headerFont.loadFromFile("../assets/fonts/PressStart2P-Regular.ttf");
    m_data->hudFont.loadFromFile("../assets/fonts/Sligoil-Micro.ttf");
    m_data->manager.AddState(StateRef(new StateMainMenu(m_data)));
}

void Game::Run() 
{
    sf::Clock clock;
    float accumulator = 0.0f;

    while (m_data->window.isOpen()) 
    {
        m_data->manager.ProcessStateChanges();

        ProcessEvents();

        float frameTime = clock.restart().asSeconds();
        if (frameTime > 0.25f) frameTime = 0.25f;
        accumulator += frameTime;

        while (accumulator >= m_dt) 
        {
            Update();
            accumulator -= m_dt;
        }

        Render();
    }
}

void Game::ProcessEvents()
{
    sf::Event event;
    while (m_data->window.pollEvent(event)) 
    {
        if (event.type == sf::Event::Closed) {
            m_data->window.close();
        }
        
        if (!m_data->manager.isEmpty()) {
            m_data->manager.GetActiveState()->HandleInput(event);
        }
    }
}

void Game::Update()
{
    if (!m_data->manager.isEmpty()) {
        m_data->manager.GetActiveState()->Update(m_dt);
    }
}

void Game::Render()
{
    m_data->window.clear(Config::BG_COLOR);
    
    if (!m_data->manager.isEmpty()) {
        m_data->manager.GetActiveState()->Draw();
    }

    m_data->window.display();
}