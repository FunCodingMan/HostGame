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
    m_entityPosition = entityPosition;

    sf::Vector2f hudPos = sf::Vector2f(entityPosition.x, entityPosition.y - 40.f);
    m_bgBar.setPosition(hudPos);
    m_fgBar.setPosition(hudPos);

    UpdateAmmoPositions();
}

void EntityHUD::Update(float currentValue, float maxValue, sf::Color barColor)
{
    if (maxValue <= 0) return;

    float percentage = std::clamp(currentValue / maxValue, 0.f, 1.f);

    m_fgBar.setSize(sf::Vector2f(m_maxWidth * percentage, 6.f));
    m_fgBar.setFillColor(barColor);
}

void EntityHUD::UpdateAmmo(int current, int maxAmmo)
{
    if (maxAmmo <= 0)
    {
        HideAmmo();
        return;
    }

    m_showAmmo = true;

    if (static_cast<int>(m_ammoDots.size()) != maxAmmo)
    {
        m_ammoDots.resize(maxAmmo);

        for (auto& dot : m_ammoDots)
        {
            dot.setRadius(Config::AMMO_DOT_RADIUS);
            dot.setOrigin(Config::AMMO_DOT_RADIUS, Config::AMMO_DOT_RADIUS);
            dot.setOutlineThickness(1.f);
            dot.setOutlineColor(sf::Color::Black);
        }

        UpdateAmmoPositions();
    }

    for (int i = 0; i < maxAmmo; ++i)
    {
        m_ammoDots[i].setFillColor(i < current ? Config::AMMO_DOT_FILL_COLOR
                                               : Config::AMMO_DOT_EMPTY_COLOR);
    }
}

void EntityHUD::HideAmmo()
{
    m_showAmmo = false;
}

void EntityHUD::UpdateAmmoPositions()
{
    if (m_ammoDots.empty())
        return;

    float totalWidth = (m_ammoDots.size() - 1) * Config::AMMO_DOT_SPACING;
    float startX = m_entityPosition.x - totalWidth / 2.f;
    float y = m_entityPosition.y - Config::AMMO_DOT_OFFSET_Y;

    for (size_t i = 0; i < m_ammoDots.size(); ++i)
    {
        m_ammoDots[i].setPosition(startX + i * Config::AMMO_DOT_SPACING, y);
    }
}

void EntityHUD::Draw(sf::RenderWindow& window)
{
    if (m_showAmmo)
    {
        for (const auto& dot : m_ammoDots)
        {
            window.draw(dot);
        }
    }

    window.draw(m_bgBar);
    window.draw(m_fgBar);
}