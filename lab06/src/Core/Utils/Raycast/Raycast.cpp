// Path: ./src/Core/Utils/Raycast/Raycast.cpp
#include "Core/Utils/Raycast/Raycast.hpp"

#include <cmath>

#include "Core/Config.hpp"
#include "Core/Debug/DebugOverlay/DebugOverlay.hpp"
#include "Entities/Entity/Entity.hpp"

bool Raycast::IsWithinFov(sf::Vector2f dirToTarget, sf::Vector2f faceDir, float fovCosine)
{
    float faceLen = std::hypot(faceDir.x, faceDir.y);
    if (faceLen == 0.f)
    {
        return true;
    }

    sf::Vector2f unitFace = faceDir / faceLen;
    float dot = dirToTarget.x * unitFace.x + dirToTarget.y * unitFace.y;
    return dot >= fovCosine;
}

void Raycast::DrawDebugRay(sf::Vector2f from, sf::Vector2f to, bool canSee)
{
    if (!Entity::AreHitboxesVisible())
    {
        return;
    }

    sf::Color color = canSee ? sf::Color::Green : sf::Color::Red;
    DebugOverlay::AddLine(from, to, color);
}

bool Raycast::CanSee(const Map& map,
                     sf::Vector2f eye,
                     sf::Vector2f target,
                     float maxRange,
                     float fovCosine,
                     sf::Vector2f faceDir)
{
    sf::Vector2f toTarget = target - eye;
    float distance = std::hypot(toTarget.x, toTarget.y);

    if (distance > maxRange)
    {
        return false;
    }

    if (distance == 0.f)
    {
        return true;
    }

    sf::Vector2f dir = toTarget / distance;
    if (!IsWithinFov(dir, faceDir, fovCosine))
    {
        return false;
    }

    float tileSize = static_cast<float>(map.GetTileSize());

    for (float walked = 0.f; walked < distance; walked += Config::LOS_STEP_SIZE)
    {
        sf::Vector2f point = eye + dir * walked;
        int tileX = static_cast<int>(std::floor(point.x / tileSize));
        int tileY = static_cast<int>(std::floor(point.y / tileSize));

        if (map.IsSolid(tileX, tileY, Config::EntityType::Bullet))
        {
            DrawDebugRay(eye, point, false);
            return false;
        }
    }

    DrawDebugRay(eye, target, true);
    return true;
}