#pragma once
#include <SFML/Graphics.hpp>

class DebugOverlay
{
public:
    static void Init(const sf::Font& font, sf::Vector2u windowSize);

    static void ToggleMouseCoords();
    static bool IsMouseCoordsEnabled();

    static void Draw(sf::RenderWindow& window, const sf::View& worldView);

private:
    static inline bool     s_show = false;
    static inline sf::Text s_text; 
};