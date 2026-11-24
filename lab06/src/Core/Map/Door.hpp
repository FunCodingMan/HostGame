#pragma once
#include <SFML/Graphics.hpp>
#include "Core/Animation/AnimationController.hpp"

class Door
{
public:
    Door(int tileX, int tileY, const sf::Texture& texture);

    bool Open();
    void Update(float dt);
    void Draw(sf::RenderWindow& window) const;

    bool IsOpen() const { return m_state == State::Open; }
    int  GetTileX() const { return m_tileX; }
    int  GetTileY() const { return m_tileY; }

private:
    enum class State
    {
        Closed,
        Opening,
        Open
    };

    sf::Sprite m_sprite;
    AnimationController m_anim;
    int m_tileX;
    int m_tileY;
    State m_state = State::Closed;
};
