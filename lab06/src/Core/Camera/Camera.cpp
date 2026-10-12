#include "Core/Camera/Camera.hpp"
#include "Core/Config.hpp"

using namespace Config;

Camera::Camera(sf::Vector2f size) : lerpSpeed(CAMERA_LERP_FACTOR)
{
    view.setSize(size);
    view.setCenter(size.x / 2.f, size.y / 2.f);
    deadZone = sf::Vector2f(100.f, 80.f);

    currentFocus = view.getCenter();
    currentMouseOffset = sf::Vector2f(0.f, 0.f);
}

void Camera::Update(sf::Vector2f targetPos, sf::Vector2f targetMouseOffset, float dt, float mapWidthPx, float mapHeightPx)
{
    sf::Vector2f desiredFocus = currentFocus;
    
    if (targetPos.x > currentFocus.x + deadZone.x) {
        desiredFocus.x = targetPos.x - deadZone.x;
    } else if (targetPos.x < currentFocus.x - deadZone.x) {
        desiredFocus.x = targetPos.x + deadZone.x;
    }

    if (targetPos.y > currentFocus.y + deadZone.y) {
        desiredFocus.y = targetPos.y - deadZone.y;
    } else if (targetPos.y < currentFocus.y - deadZone.y) {
        desiredFocus.y = targetPos.y + deadZone.y;
    }

    currentFocus += (desiredFocus - currentFocus) * lerpSpeed * dt;

    currentMouseOffset += (targetMouseOffset - currentMouseOffset) * lerpSpeed * dt;

    sf::Vector2f finalPos = currentFocus + currentMouseOffset;
    
    float halfWidth = view.getSize().x / 2.f;
    float halfHeight = view.getSize().y / 2.f;

    finalPos.x = std::clamp(finalPos.x, halfWidth, mapWidthPx - halfWidth);
    finalPos.y = std::clamp(finalPos.y, halfHeight, mapHeightPx - halfHeight);

    view.setCenter(finalPos);
}

const sf::View& Camera::GetView() const
{
    return view;
}

void Camera::SetSize(sf::Vector2f newSize)
{
    view.setSize(newSize);
}

void Camera::Zoom(float factor)
{
    view.zoom(factor);
}