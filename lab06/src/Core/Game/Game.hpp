#pragma once

#include <SFML/Graphics.hpp>
#include "Entities/Parasite/Parasite.hpp"
#include "Entities/Enemy/Enemy.hpp"
#include "Core/Map/Map.hpp"
#include "Core/Camera/Camera.hpp"

class Game {
public:
    Game();
    void Run();

private:
    void ProcessEvents();
    void Update(float dt);
    void UpdateCamera(float dt);
    void ProcessKeyboardEvents(sf::Event event);
    void Render();

    sf::RenderWindow m_window;
    sf::Clock m_clock;
    Parasite m_parasite;
    Enemy m_enemy;
    Map m_map;
    Camera m_camera;
};