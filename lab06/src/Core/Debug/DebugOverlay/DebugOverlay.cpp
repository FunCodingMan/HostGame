// Path: ./src/Core/Debug/DebugOverlay/DebugOverlay.cpp
#include "DebugOverlay.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "Core/Config.hpp"
#include "Core/Utils/StringUtils/StringUtils.hpp"

void DebugOverlay::Init(const sf::Font &font, sf::Vector2u windowSize)
{
    s_currentFont = &font;

    s_text = sf::Text();

    s_text.setFont(font);
    s_text.setCharacterSize(18);
    s_text.setFillColor(sf::Color::Yellow);
    s_text.setOutlineColor(sf::Color::Black);
    s_text.setOutlineThickness(2.f);
    s_text.setPosition(20.f, static_cast<float>(windowSize.y) - 35.f);
}

void DebugOverlay::ToggleMouseCoords()
{
    s_show = !s_show;
}
bool DebugOverlay::IsMouseCoordsEnabled()
{
    return s_show;
}

void DebugOverlay::AddLine(sf::Vector2f p1, sf::Vector2f p2, sf::Color color)
{
    s_lines.push_back({p1, p2, color});
}

void DebugOverlay::Draw(sf::RenderWindow &window, const sf::View &worldView)
{
    if (!s_lines.empty())
    {
        sf::View oldView = window.getView();
        window.setView(worldView);

        std::vector<sf::Vertex> vertices;
        vertices.reserve(s_lines.size() * 2);
        for (const auto &line : s_lines)
        {
            vertices.push_back(sf::Vertex(line.p1, line.color));
            vertices.push_back(sf::Vertex(line.p2, line.color));
        }
        window.draw(vertices.data(), vertices.size(), sf::Lines);
        s_lines.clear(); // Сбрасываем линии после рендера текущего кадра

        window.setView(oldView);
    }

    if (!s_show)
        return;
    if (s_currentFont == nullptr)
        return;

    sf::Vector2i pixel = sf::Mouse::getPosition(window);
    sf::Vector2f world = window.mapPixelToCoords(pixel, worldView);

    int tileX = static_cast<int>(std::floor(world.x / Config::WORLD_TILE_SIZE));
    int tileY = static_cast<int>(std::floor(world.y / Config::WORLD_TILE_SIZE));

    std::ostringstream ss;
    ss << "Px: (" << pixel.x << ", " << pixel.y << ")   " << "World: (" << std::fixed
       << std::setprecision(1) << world.x << ", " << world.y << ")   " << "Tile: (" << tileX << ", "
       << tileY << ")";

    s_text.setString(StringUtils::ToSfStringFromUtf8(ss.str()));
    window.draw(s_text);
}