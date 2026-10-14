#include "Core/Game/Game.hpp"
#include "Entities/Parasite/Parasite.hpp"
#include "Entities/Enemy/Enemy.hpp"
#include <iostream>
#include "Core/Config.hpp"


Game::Game() 
    : m_window(sf::VideoMode::getDesktopMode(), "The Host", sf::Style::Fullscreen),
      m_parasite(sf::Vector2f(Config::PARASITE_INITIAL_X, Config::PARASITE_INITIAL_Y)),
      m_enemy(sf::Vector2f(Config::ENEMY_INITIAL_X, Config::ENEMY_INITIAL_Y)),
      m_camera(sf::Vector2f(Config::WINDOW_WIDTH / Config::CAMERA_ZOOM, 
                          Config::WINDOW_HEIGHT / Config::CAMERA_ZOOM))
{
    m_map.LoadTileConfig("../assets/map/tileset.txt");
    m_map.LoadFromFile("../assets/map/level1.txt");
}

void Game::Run() {
    while (m_window.isOpen()) {
        float dt = m_clock.restart().asSeconds();
        ProcessEvents();
        Update(dt);
        Render();
    }
}

void Game::ProcessKeyboardEvents(sf::Event event)
{
    if (event.key.code == sf::Keyboard::Escape)
    {
        m_window.close();
    }

    if (event.key.code == sf::Keyboard::B && sf::Keyboard::isKeyPressed(sf::Keyboard::F3))
    {
        Entity::ToggleHitboxes();
    }
}


void Game::ProcessEvents()
{
    sf::Event event;

    while (m_window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                m_window.close();
                break;
            case sf::Event::KeyPressed:
                ProcessKeyboardEvents(event);
                break;
        }
    }
}

void Game::UpdateCamera(float dt)
{
    sf::Vector2f targetPos = m_parasite.GetPosition();

    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(m_window);
    sf::Vector2u windowSize = m_window.getSize();

    float normX = (mousePixelPos.x - (windowSize.x / 2.f)) / (windowSize.x / 2.f);
    float normY = (mousePixelPos.y - (windowSize.y / 2.f)) / (windowSize.y / 2.f);

    normX = std::clamp(normX, -1.f, 1.f);
    normY = std::clamp(normY, -1.f, 1.f);

    sf::Vector2f viewSize = m_camera.GetView().getSize();
    float maxOffsetX = (viewSize.x / 2.f) * Config::CAMERA_LOOK_STRENGTH;
    float maxOffsetY = (viewSize.y / 2.f) * Config::CAMERA_LOOK_STRENGTH;

    sf::Vector2f mouseOffset(normX * maxOffsetX, normY * maxOffsetY);

    m_camera.Update(targetPos, mouseOffset, dt, m_map.GetWidthInPixels(), m_map.GetHeightInPixels());
}

void Game::Update(float dt)
{
    UpdateCamera(dt);

    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(m_window);
    sf::Vector2f mousePos = m_window.mapPixelToCoords(mousePixelPos, m_camera.GetView());

    GameContext ctx;
    ctx.dt = dt;
    ctx.mousePos = mousePos;
    ctx.levelMap = &m_map;
    

    m_parasite.Update(ctx);
    m_enemy.Update(dt);

    if (m_parasite.GetBounds().intersects(m_enemy.GetBounds()))
    {
        m_enemy.OnCollision();
    }
}

void Game::Render()
{
    m_window.clear(Config::BG_COLOR);
    m_window.setView(m_camera.GetView());
    m_map.Draw(m_window);
    m_parasite.Draw(m_window);
    m_window.display();
}