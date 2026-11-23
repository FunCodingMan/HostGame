// Path: ./src/Core/Debug/DebugOverlay/DebugOverlay.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class DebugOverlay
{
public:
    static void Init(const sf::Font& font, sf::Vector2u windowSize);
    static void ToggleMouseCoords();
    static bool IsMouseCoordsEnabled();
    static void Draw(sf::RenderWindow& window, const sf::View& worldView);

    static void AddLine(sf::Vector2f p1, sf::Vector2f p2, sf::Color color);

private:
    struct DebugLine {
        sf::Vector2f p1, p2;
        sf::Color color;
    };
    static inline std::vector<DebugLine> s_lines;

    static inline bool s_show = false;
    static inline sf::Text s_text;
    static inline const sf::Font* s_currentFont = nullptr; 
};