#include "Core/Game/Game.hpp"
#include "Entities/Parasite/Parasite.hpp"
#include "Entities/Enemy/Enemy.hpp"
#include <iostream>
#include "Core/Config.hpp"


Game::Game() 
    : window(sf::VideoMode(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT), "The Host"),
      parasite(sf::Vector2f(Config::PARASITE_INITIAL_X, Config::PARASITE_INITIAL_Y)),
      enemy(sf::Vector2f(Config::ENEMY_INITIAL_X, Config::ENEMY_INITIAL_Y))
{
}

void Game::Run() {
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        ProcessEvents();
        Update(dt);
        Render();
    }
}

void Game::ProcessKeyboardEvents(sf::Event event)
{
    if (event.key.code == sf::Keyboard::B && sf::Keyboard::isKeyPressed(sf::Keyboard::F3))
    {
        Entity::ToggleHitboxes();
    }
}


void Game::ProcessEvents()
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                ProcessKeyboardEvents(event);
                break;
        }
    }
}

void Game::Update(float dt)
{
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(mousePixelPos);

    GameContext ctx;
    ctx.dt = dt;
    ctx.mousePos = mousePos;

    parasite.Update(ctx);
    enemy.Update(dt);

    if (parasite.GetBounds().intersects(enemy.GetBounds()))
    {
        enemy.OnCollision();
    }
}

void Game::Render()
{
    window.clear(Config::BG_COLOR);
    enemy.Draw(window); 
    parasite.Draw(window);
    window.display();
}