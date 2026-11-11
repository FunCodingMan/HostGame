#pragma once
#include <SFML/Graphics.hpp>
#include "Core/Map/Map.hpp"
#include "Core/Sound/SoundManager/SoundManager.hpp"

struct GameContext
{
    float dt;
    sf::Vector2f mousePos;
    Map* levelMap;
    SoundManager* audio;
};