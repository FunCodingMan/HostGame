#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "States/StateManager/StateManager.hpp"
#include "Core/Sound/SoundManager/SoundManager.hpp"

struct LevelStats {
    float timeSpent = 0.f;
    int dashesUsed = 0;
    int tilesBroken = 0;
    int hostsPossessed = 0;
    int syringesCollected = 0;

    void Reset() {
        timeSpent = 0.f;
        dashesUsed = 0;
        tilesBroken = 0;
        hostsPossessed = 0;
        syringesCollected = 0;
    }
};

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