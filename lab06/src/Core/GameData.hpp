#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include "States/StateManager/StateManager.hpp"
#include "Core/Sound/SoundManager/SoundManager.hpp"
#include "Core/LevelLogic/LevelStats.hpp"

struct GameData
{
    sf::RenderWindow window;
    StateManager manager;
    sf::Font headerFont;
    sf::Font hudFont;
    SoundManager sound;
    std::vector<std::string> levels;
    int currentLevelIndex = 0;

    LevelStats stats;

    std::string GetCurrentLevelName() const {
        if (currentLevelIndex >= 0 && currentLevelIndex < levels.size()) {
            return levels[currentLevelIndex];
        }
        return "";
    }
};

typedef std::shared_ptr<GameData> GameDataRef;