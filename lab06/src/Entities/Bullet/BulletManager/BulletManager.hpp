#pragma once
#include <array>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Core/Config.hpp"
#include "Core/Map/Map.hpp"

struct Bullet
{
    bool active = false;
    bool fromPlayer = false;
    sf::Vector2f position{0.f, 0.f};
    sf::Vector2f velocity{0.f, 0.f};
    float angleDeg = 0.f;
    float damage = 0.f;
    float lifeTimer = 0.f;
};

class BulletManager
{
  public:
    BulletManager();

    void Spawn(sf::Vector2f origin, sf::Vector2f direction, float damage, bool fromPlayer);
    std::vector<sf::Vector2i> Update(float dt, Map &map);
    void Draw(sf::RenderWindow &window);
    void Clear();

    std::array<Bullet, Config::BULLET_POOL_SIZE> &GetBullets();
    static sf::FloatRect GetBulletBounds(const Bullet &bullet);

  private:
    bool IsOutOfMap(sf::Vector2f pos, const Map &map) const;

    std::array<Bullet, Config::BULLET_POOL_SIZE> m_pool;
    sf::RectangleShape m_shape;
    sf::RectangleShape m_debugBox;
};