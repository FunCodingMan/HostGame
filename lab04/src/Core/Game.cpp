#include "Core/Game.hpp"
#include "Entities/Parasite.hpp"

const int GAME_HEIGHT = 1920;
const int GAME_WIDTH = 1080;

Game::Game() 
    : window(sf::VideoMode(1920, 1080), "The Host"),
      parasite(sf::Vector2f(100.f, 100.f))
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
    parasite.update(dt);
}

void Game::render()
{
    window.clear(sf::Color(40, 40, 40));
    parasite.draw(window);
    window.display();
}