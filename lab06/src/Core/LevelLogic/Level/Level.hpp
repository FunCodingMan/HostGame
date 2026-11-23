#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <string>
#include "Core/LevelLogic/LevelStats.hpp"
#include "Core/Map/Map.hpp"
#include "Core/Camera/Camera.hpp"
#include "Entities/Parasite/Parasite.hpp"
#include "Entities/Host/Host.hpp"
#include "Entities/Items/Syringe/Syringe.hpp"
#include "Entities/Bullet/BulletManager/BulletManager.hpp"

class SoundManager;

enum class LevelResult { None, Completed, Failed };

struct LevelInput {
    sf::Vector2i mousePixelPos{0, 0};
    sf::Vector2u windowSize{0, 0};
};

class Level
{
public:
    Level();

    void Load(const std::string& name, SoundManager& audio);
    void Update(float dt, const LevelInput& input);
    void Draw(sf::RenderWindow& window);

    const sf::View& GetView() const { return m_camera.GetView(); }
    LivingEntity* GetAvatar() const { return m_playerAvatar; }
    Host* GetCurrentHost() const { return m_currentHost; }
    const Parasite& GetParasite() const { return m_parasite; }
    LevelResult GetResult() const { return m_result; }
    const LevelStats& GetStats() const { return m_stats; }
    bool IsAvatarInElevator() const;

private:
    void UpdateCamera(float dt, const LevelInput& input);
    sf::Vector2f MapMouseToWorld(const LevelInput& input) const;

    void HandlePossession();
    void HandleEjection(const GameContext& ctx);
    void HandleItems(const GameContext& ctx);
    void CleanDeadEnemies();
    void CheckEndConditions();
    void HandleBullets();

    BulletManager m_bullets;

    SoundManager* m_audio = nullptr;
    Map m_map;
    Camera m_camera;
    Parasite m_parasite;
    std::vector<std::unique_ptr<Host>>    m_hosts;
    std::vector<std::unique_ptr<Syringe>> m_syringes;

    LivingEntity* m_playerAvatar = nullptr;
    Host* m_currentHost = nullptr;
    float m_timeInCurrentHost = 0.f;

    LevelStats m_stats;
    LevelResult m_result = LevelResult::None;
};