#pragma once
#include <SFML/Graphics.hpp>
#include "Core/GameContext.hpp"

class Entity
{
protected:
    sf::RectangleShape hitbox;
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f velocity;
public:
    static void ToggleHitboxes()
    {
        showHitboxes = !showHitboxes;
    }

    virtual ~Entity() = default;

    virtual void Update(const GameContext& ctx) = 0;

    virtual void Draw(sf::RenderWindow& window)
    {
        window.draw(sprite);

        if (showHitboxes)
        {
            window.draw(hitbox);
        }
    }

    sf::FloatRect GetBounds() const
    {
        return hitbox.getGlobalBounds();
    }
private:
    inline static bool showHitboxes = false;
};