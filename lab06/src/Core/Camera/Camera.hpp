#pragma once
#include <SFML/Graphics.hpp>
#include "Core/Config.hpp"

class Camera
{
public:
    Camera(sf::Vector2f size);

    void Update(sf::Vector2f targetPos, sf::Vector2f targetMouseOffset, float dt, float mapWidthPx,
                float mapHeightPx);

    void NewFunction(sf::Vector2f &targetPos, sf::Vector2f &desiredFocus);

    const sf::View& GetView() const;
    void SetSize(sf::Vector2f newSize);
    void Zoom(float factor);

private:
    sf::View m_view;
    float m_lerpSpeed;
    sf::Vector2f m_deadZone;
    sf::Vector2f m_currentFocus;
    sf::Vector2f m_currentMouseOffset;
};