#pragma once
#include <SFML/Graphics.hpp>

#include "Core/Map/Map.hpp"

class Raycast
{
  public:
    static bool CanSee(const Map &map, sf::Vector2f eye, sf::Vector2f target, float maxRange,
                       float fovCosine = -1.f, sf::Vector2f faceDir = sf::Vector2f(0.f, 0.f));

  private:
    static bool IsWithinFov(sf::Vector2f dirToTarget, sf::Vector2f faceDir, float fovCosine);
    static void DrawDebugRay(sf::Vector2f from, sf::Vector2f to, bool canSee);
};