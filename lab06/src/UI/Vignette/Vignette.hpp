#pragma once
#include <SFML/Graphics.hpp>

class Vignette
{
public:
    void Init(sf::Vector2u windowSize);
    void Update(float dt, bool active);
    void Draw(sf::RenderWindow& window);

private:
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    float m_time = 0.f;
    float m_alpha = 0.f;
};
