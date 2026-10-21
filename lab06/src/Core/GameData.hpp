#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "States/StateManager/StateManager.hpp"

struct GameData
{
    sf::RenderWindow window;
    StateManager manager;
    sf::Font headerFont;
    sf::Font hudFont;
};

typedef std::shared_ptr<GameData> GameDataRef;