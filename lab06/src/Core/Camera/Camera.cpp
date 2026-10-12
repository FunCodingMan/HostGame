#include "Core/Camera/Camera.hpp"

Camera::Camera(sf::Vector2f size) : lerpSpeed(5.0f)
{
    view.setSize(size);
    view.setCenter(size.x / 2.f, size.y / 2.f);
}

void Camera::Update(sf::Vector2f targetPos, float dt)
{
    sf::Vector2f currentPos = view.getCenter();
    
    sf::Vector2f newPos = currentPos + (targetPos - currentPos) * lerpSpeed * dt;
    
    view.setCenter(newPos);
}

const sf::View& Camera::GetView() const
{
    return view;
}