#include "Entities/Items/Syringe/Syringe.hpp"
#include "Core/Config.hpp"
#include <cmath>
#include <iostream>

Syringe::Syringe(sf::Vector2f position) : m_startPos(position)
{
    m_hitbox.setSize(sf::Vector2f(Config::SYRINGE_HITBOX_SIZE, Config::SYRINGE_HITBOX_SIZE));
    m_hitbox.setOrigin(Config::SYRINGE_HITBOX_SIZE / 2.f, Config::SYRINGE_HITBOX_SIZE / 2.f);
    m_hitbox.setPosition(position);
    
    m_hitbox.setFillColor(sf::Color::Transparent);
    m_hitbox.setOutlineColor(sf::Color::Cyan);
    m_hitbox.setOutlineThickness(1.f);

    if (!m_texture.loadFromFile("../assets/images/syringe.png"))
    {
        std::cerr << "Warning: Could not load syringe.png\n";
    }
    m_texture.setSmooth(false);
    m_sprite.setTexture(m_texture);
    
    m_sprite.setOrigin(m_texture.getSize().x / 2.f, m_texture.getSize().y / 2.f);
    m_sprite.setPosition(position);
}

void Syringe::Update(const GameContext& ctx)
{
    if (m_isCollected) return;

    m_floatTimer += ctx.dt * Config::SYRINGE_FLOAT_SPEED;
    float yOffset = std::sin(m_floatTimer) * Config::SYRINGE_FLOAT_AMPLITUDE;

    m_hitbox.setPosition(m_startPos.x, m_startPos.y + yOffset);
    m_sprite.setPosition(m_hitbox.getPosition());
}