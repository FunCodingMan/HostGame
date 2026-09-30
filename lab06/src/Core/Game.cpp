#include "Core/Game.hpp"
#include "Entities/Parasite.hpp"
#include "Entities/Enemy.hpp"
#include <iostream>
#include "Core/Config.hpp"


Game::Game() 
    : window(sf::VideoMode(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT), "The Host"),
      parasite(sf::Vector2f(Config::PARASITE_INITIAL_X, Config::PARASITE_INITIAL_Y)),
      enemy(sf::Vector2f(Config::ENEMY_INITIAL_X, Config::ENEMY_INITIAL_Y))
{
}

void Game::run() {
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}


void Game::processEvents()
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

void Game::update(float dt)
{
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos(static_cast<float>(mousePixelPos.x), static_cast<float>(mousePixelPos.y));
    parasite.update(dt, mousePos);
    enemy.update(dt);

    if (parasite.getBounds().intersects(enemy.getBounds()))
    {
        enemy.onCollision();
    }
}

void Game::render()
{
    window.clear(Config::BG_COLOR);
    enemy.draw(window); 
    parasite.draw(window);
    window.display();
}