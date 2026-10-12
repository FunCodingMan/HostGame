#include "Core/Camera/Camera.hpp"
#include "Core/Config.hpp"

using namespace Config;

Camera::Camera(sf::Vector2f size) : lerpSpeed(CAMERA_LERP_FACTOR)
{
    view.setSize(size);
    view.setCenter(size.x / 2.f, size.y / 2.f);
    deadZone = sf::Vector2f(100.f, 80.f);
}

void Camera::Update(sf::Vector2f targetPos, float dt, float mapWidthPx, float mapHeightPx)
{
    sf::Vector2f currentPos = view.getCenter();
    sf::Vector2f desiredPos = currentPos;
    
    if (targetPos.x > currentPos.x + deadZone.x) {
        desiredPos.x = targetPos.x - deadZone.x;
    } else if (targetPos.x < currentPos.x - deadZone.x) {
        desiredPos.x = targetPos.x + deadZone.x;
    }

    if (targetPos.y > currentPos.y + deadZone.y) {
        desiredPos.y = targetPos.y - deadZone.y;
    } else if (targetPos.y < currentPos.y - deadZone.y) {
        desiredPos.y = targetPos.y + deadZone.y;
    }
    
    sf::Vector2f newPos = currentPos + (desiredPos - currentPos) * lerpSpeed * dt;

    float halfWidth = view.getSize().x / 2.f;
    float halfHeight = view.getSize().y / 2.f;

    newPos.x = std::clamp(newPos.x, halfWidth, mapWidthPx - halfWidth);
    newPos.y = std::clamp(newPos.y, halfHeight, mapHeightPx - halfHeight);
    
    view.setCenter(newPos);
}

const sf::View& Camera::GetView() const
{
    return view;
}