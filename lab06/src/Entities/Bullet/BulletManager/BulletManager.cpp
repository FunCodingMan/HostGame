#include "Entities/Bullet/BulletManager/BulletManager.hpp"

#include <cmath>

#include "Entities/Entity/Entity.hpp"

BulletManager::BulletManager()
{
    m_shape.setSize(sf::Vector2f(Config::BULLET_LENGTH, Config::BULLET_THICKNESS));
    m_shape.setOrigin(Config::BULLET_LENGTH / 2.f, Config::BULLET_THICKNESS / 2.f);

    m_debugBox.setSize(sf::Vector2f(Config::BULLET_HITBOX_SIZE, Config::BULLET_HITBOX_SIZE));
    m_debugBox.setOrigin(Config::BULLET_HITBOX_SIZE / 2.f, Config::BULLET_HITBOX_SIZE / 2.f);
    m_debugBox.setFillColor(sf::Color::Transparent);
    m_debugBox.setOutlineColor(Config::BULLET_DEBUG_COLOR);
    m_debugBox.setOutlineThickness(1.f);
}

void BulletManager::Spawn(sf::Vector2f origin, sf::Vector2f direction, float damage,
                          bool fromPlayer)
{
    float length = std::hypot(direction.x, direction.y);
    if (length == 0.f)
    {
        return;
    }

    sf::Vector2f unitDir = direction / length;

    for (Bullet &bullet : m_pool)
    {
        if (bullet.active)
        {
            continue;
        }

        bullet.active = true;
        bullet.fromPlayer = fromPlayer;
        bullet.position = origin;
        bullet.velocity = unitDir * Config::BULLET_SPEED;
        bullet.angleDeg = std::atan2(unitDir.y, unitDir.x) * Config::RAD_TO_DEG;
        bullet.damage = damage;
        bullet.lifeTimer = Config::BULLET_MAX_LIFETIME;
        return;
    }
}

bool BulletManager::IsOutOfMap(sf::Vector2f pos, const Map &map) const
{
    return pos.x < 0.f || pos.x > map.GetWidthInPixels() || pos.y < 0.f ||
           pos.y > map.GetHeightInPixels();
}

std::vector<sf::Vector2i> BulletManager::Update(float dt, Map &map)
{
    std::vector<sf::Vector2i> brokenTiles;
    float tileSize = static_cast<float>(map.GetTileSize());

    for (Bullet &bullet : m_pool)
    {
        if (!bullet.active)
        {
            continue;
        }

        bullet.position += bullet.velocity * dt;
        bullet.lifeTimer -= dt;

        if (bullet.lifeTimer <= 0.f || IsOutOfMap(bullet.position, map))
        {
            bullet.active = false;
            continue;
        }

        int tileX = static_cast<int>(std::floor(bullet.position.x / tileSize));
        int tileY = static_cast<int>(std::floor(bullet.position.y / tileSize));

        if (!map.IsSolid(tileX, tileY, Config::EntityType::Bullet))
        {
            continue;
        }

        if (map.IsBreakable(tileX, tileY))
        {
            map.BreakTile(tileX, tileY);
            brokenTiles.emplace_back(tileX, tileY);
        }

        bullet.active = false;
    }

    return brokenTiles;
}

void BulletManager::Draw(sf::RenderWindow &window)
{
    bool showHitboxes = Entity::AreHitboxesVisible();

    for (const Bullet &bullet : m_pool)
    {
        if (!bullet.active)
        {
            continue;
        }

        m_shape.setPosition(bullet.position);
        m_shape.setRotation(bullet.angleDeg);
        m_shape.setFillColor(bullet.fromPlayer ? Config::BULLET_PLAYER_COLOR
                                               : Config::BULLET_ENEMY_COLOR);
        window.draw(m_shape);

        if (showHitboxes)
        {
            m_debugBox.setPosition(bullet.position);
            window.draw(m_debugBox);
        }
    }
}

void BulletManager::Clear()
{
    for (Bullet &bullet : m_pool)
    {
        bullet.active = false;
    }
}

std::array<Bullet, Config::BULLET_POOL_SIZE> &BulletManager::GetBullets()
{
    return m_pool;
}

sf::FloatRect BulletManager::GetBulletBounds(const Bullet &bullet)
{
    float half = Config::BULLET_HITBOX_SIZE / 2.f;
    return sf::FloatRect(bullet.position.x - half, bullet.position.y - half,
                         Config::BULLET_HITBOX_SIZE, Config::BULLET_HITBOX_SIZE);
}