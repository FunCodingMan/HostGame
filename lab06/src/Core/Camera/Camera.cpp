#include "Core/Camera/Camera.hpp"
#include "Core/Config.hpp"

using namespace Config;

Camera::Camera(sf::Vector2f size) : m_lerpSpeed(CAMERA_LERP_FACTOR)
{
    m_view.setSize(size);
    m_view.setCenter(size.x / 2.f, size.y / 2.f);
    m_deadZone = sf::Vector2f(CAMERA_DEAD_ZONE_WIDTH, CAMERA_DEAD_ZONE_HEIGHT);

    m_currentFocus = m_view.getCenter();
    m_currentMouseOffset = sf::Vector2f(0.f, 0.f);
}

void Camera::Update(sf::Vector2f targetPos, sf::Vector2f targetMouseOffset, float dt, float mapWidthPx, float mapHeightPx)
{
    sf::Vector2f desiredFocus = m_currentFocus;
    
    if (targetPos.x > m_currentFocus.x + m_deadZone.x) {
        desiredFocus.x = targetPos.x - m_deadZone.x;
    } else if (targetPos.x < m_currentFocus.x - m_deadZone.x) {
        desiredFocus.x = targetPos.x + m_deadZone.x;
    }

    if (targetPos.y > m_currentFocus.y + m_deadZone.y) {
        desiredFocus.y = targetPos.y - m_deadZone.y;
    } else if (targetPos.y < m_currentFocus.y - m_deadZone.y) {
        desiredFocus.y = targetPos.y + m_deadZone.y;
    }

    m_currentFocus += (desiredFocus - m_currentFocus) * m_lerpSpeed * dt;

    m_currentMouseOffset += (targetMouseOffset - m_currentMouseOffset) * m_lerpSpeed * dt;

    sf::Vector2f finalPos = m_currentFocus + m_currentMouseOffset;
    
    float halfWidth = m_view.getSize().x / 2.f;
    float halfHeight = m_view.getSize().y / 2.f;

    finalPos.x = std::clamp(finalPos.x, halfWidth, mapWidthPx - halfWidth);
    finalPos.y = std::clamp(finalPos.y, halfHeight, mapHeightPx - halfHeight);

    m_view.setCenter(finalPos);
}

const sf::View& Camera::GetView() const
{
    return m_view;
}

void Camera::SetSize(sf::Vector2f newSize)
{
    m_view.setSize(newSize);
}

void Camera::Zoom(float factor)
{
    m_view.zoom(factor);
}