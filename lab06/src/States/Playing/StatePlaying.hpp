#pragma once
#include <SFML/Graphics.hpp>
#include "States/State.hpp"
#include "Core/GameData.hpp"
#include "Core/Config.hpp"
#include "Core/Map/Map.hpp"
#include "Core/Camera/Camera.hpp"
#include "Entities/Parasite/Parasite.hpp"
#include "Entities/Enemy/Enemy.hpp"

class StatePlaying : public State
{
public:
    StatePlaying(GameDataRef data);

    void Init() override;
    void HandleInput(const sf::Event& event) override;
    void Update(float dt) override;
    void Draw() override;

private:
    void UpdateCamera(float dt);

    GameDataRef m_data;
    
    Parasite m_parasite;
    Enemy m_enemy;
    Map m_map;
    Camera m_camera;
};