#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "States/State.hpp"
#include "Core/GameData.hpp"
#include "Core/Config.hpp"
#include "Core/Map/Map.hpp"
#include "Core/Camera/Camera.hpp"
#include "Entities/Parasite/Parasite.hpp"
#include "Entities/Enemy/Enemy.hpp"
#include "UI/EntityHUD/EntityHUD.hpp"
#include "UI/ScreenHUD/ScreenHUD.hpp"
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

    void HandlePossession();
    void HandleEjection(const GameContext& ctx);
    void UpdateHUD();

    void CheckEndGameConditions();

    GameDataRef m_data;
    
    Map m_map;
    Camera m_camera;
    EntityHUD m_floatingHud;
    ScreenHUD m_screenHud;

    Parasite m_parasite;
    std::vector<std::unique_ptr<Enemy>> m_enemies;

    LivingEntity* m_playerAvatar;
    Enemy* m_currentHost;

    float m_timeInCurrentHost;
};