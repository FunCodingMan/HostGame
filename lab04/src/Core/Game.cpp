#include "Core/Game.hpp"
#include "Entities/Parasite.hpp"
#include "Entities/Enemy.hpp"
#include <iostream>

const int GAME_HEIGHT = 1920;
const int GAME_WIDTH = 1080;

Game::Game() 
    : window(sf::VideoMode(1920, 1080), "The Host"),
      parasite(sf::Vector2f(100.f, 100.f)),
      enemy(sf::Vector2f(960.f, 540.f))
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
        std::cout << "COLLISION DETECTED!" << std::endl;
    }
}

void Game::render()
{
    window.clear(sf::Color(40, 40, 40));
    enemy.draw(window); 
    parasite.draw(window);
    window.display();
}