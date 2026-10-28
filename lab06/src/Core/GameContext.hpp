#pragma once
#include <SFML/Graphics.hpp>
#include "Core/Map/Map.hpp"

struct GameContext
{
    float dt;
    sf::Vector2f mousePos;
    Map* levelMap;
};