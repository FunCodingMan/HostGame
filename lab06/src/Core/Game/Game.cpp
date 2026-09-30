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


void Game::ProcessEvents()
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
    }
}

void Game::Update(float dt)
{
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos(static_cast<float>(mousePixelPos.x), static_cast<float>(mousePixelPos.y));
    parasite.Update(dt, mousePos);
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