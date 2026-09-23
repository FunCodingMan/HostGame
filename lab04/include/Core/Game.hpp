#pragma once

#include <SFML/Graphics.hpp>
#include "Entities/Parasite.hpp"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

    sf::RenderWindow window;
    sf::Clock clock;
    Parasite parasite;
};