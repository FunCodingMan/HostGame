#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "Core/GameData.hpp"
#include "Core/Config.hpp"

class Game {
public:
    Game();
    void Run();

private:
    void ProcessEvents();
    void Update();
    void Render();

    GameDataRef m_data = std::make_shared<GameData>();
    const float m_dt = 1.0f / Config::PHYSICS_FPS;
};