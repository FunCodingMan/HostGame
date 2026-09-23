#include "Core/Game.hpp"

Game::Game() : window(sf::VideoMode(1920, 1080), "The Host")
{
    window.setFramerateLimit(60);
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
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

void Game::update()
{

}

void Game::render()
{
    window.clear(sf::Color(40, 40, 40));

    window.display();
}