#pragma once
#include <SFML/Graphics.hpp>
#include "Core/GameContext.hpp"

class Entity
{
protected:
    sf::RectangleShape m_hitbox;
    sf::Sprite m_sprite;
    sf::Texture m_texture;
    sf::Vector2f m_velocity;
public:
    static void ToggleHitboxes()
    {
        m_showHitboxes = !m_showHitboxes;
    }

    virtual ~Entity() = default;

    virtual void Update(const GameContext& ctx) = 0;

    virtual void Draw(sf::RenderWindow& window)
    {
        window.draw(m_sprite);

        if (m_showHitboxes)
        {
            window.draw(m_hitbox);
        }
    }

    sf::FloatRect GetBounds() const
    {
        return m_hitbox.getGlobalBounds();
    }

    sf::Vector2f GetPosition() const {
        return m_hitbox.getPosition();
    }

    void SetPosition(sf::Vector2f newPos) {
        m_hitbox.setPosition(newPos);
        m_sprite.setPosition(newPos);
    }
private:
    inline static bool m_showHitboxes = false;
};