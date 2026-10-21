#include "EntityHUD.hpp"
#include <algorithm>
#include "Core/Config.hpp"

EntityHUD::EntityHUD() : m_maxWidth(Config::PLAYER_HUD_WIDTH) {}

void EntityHUD::Init()
{
    m_bgBar.setSize(sf::Vector2f(m_maxWidth, 6.f));
    m_bgBar.setOrigin(m_maxWidth / 2.f, 3.f);
    m_bgBar.setFillColor(sf::Color(50, 50, 50, 200));

    m_fgBar.setSize(sf::Vector2f(m_maxWidth, 6.f));
    m_fgBar.setOrigin(m_maxWidth / 2.f, 3.f);
}

void EntityHUD::SetPosition(sf::Vector2f entityPosition)
{
    sf::Vector2f hudPos = sf::Vector2f(entityPosition.x, entityPosition.y - 40.f);
    m_bgBar.setPosition(hudPos);
    m_fgBar.setPosition(hudPos);
}

void EntityHUD::Update(float currentValue, float maxValue, sf::Color barColor)
{
    if (maxValue <= 0) return;

    float percentage = std::clamp(currentValue / maxValue, 0.f, 1.f);

    m_fgBar.setSize(sf::Vector2f(m_maxWidth * percentage, 6.f));
    m_fgBar.setFillColor(barColor);
}

void EntityHUD::Draw(sf::RenderWindow& window)
{
    window.draw(m_bgBar);
    window.draw(m_fgBar);
}