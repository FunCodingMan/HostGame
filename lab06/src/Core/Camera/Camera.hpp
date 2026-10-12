#pragma once
#include <SFML/Graphics.hpp>
#include "Core/Config.hpp"

class Camera
{
public:
    Camera(sf::Vector2f size);

    void Update(sf::Vector2f targetPos, float dt, float mapWidthPx, float mapHeightPx);
    
    const sf::View& GetView() const;

private:
    sf::View view;
    float lerpSpeed;
    sf::Vector2f deadZone;
};