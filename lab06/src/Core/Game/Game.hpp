#pragma once

#include <SFML/Graphics.hpp>
#include "Entities/Parasite/Parasite.hpp"
#include "Entities/Enemy/Enemy.hpp"

class Game {
public:
    Game();
    void Run();

private:
    void ProcessEvents();
    void Update(float dt);
    void ProcessKeyboardEvents(sf::Event event);
    void Render();

    sf::RenderWindow window;
    sf::Clock clock;
    Parasite parasite;
    Enemy enemy;
};