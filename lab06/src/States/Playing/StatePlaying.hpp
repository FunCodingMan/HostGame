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
#include "Entities/Host/Host.hpp"
#include "UI/EntityHUD/EntityHUD.hpp"
#include "UI/ScreenHUD/ScreenHUD.hpp"
#include "Entities/Items/Syringe/Syringe.hpp"
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
    void CleanDeadEnemies();
    void UpdateHUD();

    void CheckEndGameConditions();

    void HandleItems(); 

    GameDataRef m_data;
    
    Map m_map;
    Camera m_camera;
    EntityHUD m_floatingHud;
    ScreenHUD m_screenHud;

    Parasite m_parasite;
    std::vector<std::unique_ptr<Host>>   m_hosts;


    LivingEntity* m_playerAvatar;
    Host* m_currentHost = nullptr;

    float m_timeInCurrentHost;

    std::vector<std::unique_ptr<Syringe>> m_syringes;
};