#include "DebugOverlay.hpp"
#include "Core/Config.hpp"
#include "Core/Utils/StringUtils/StringUtils.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>

void DebugOverlay::Init(const sf::Font& font, sf::Vector2u windowSize)
{
    s_text.setFont(font);
    s_text.setCharacterSize(18);
    s_text.setFillColor(sf::Color::Yellow);
    s_text.setOutlineColor(sf::Color::Black);
    s_text.setOutlineThickness(2.f);
    s_text.setPosition(20.f, static_cast<float>(windowSize.y) - 35.f);
}

void DebugOverlay::ToggleMouseCoords() { s_show = !s_show; }
bool DebugOverlay::IsMouseCoordsEnabled() { return s_show; }

void DebugOverlay::Draw(sf::RenderWindow& window, const sf::View& worldView)
{
    if (!s_show) return;

    sf::Vector2i pixel = sf::Mouse::getPosition(window);
    sf::Vector2f world = window.mapPixelToCoords(pixel, worldView);

    int tileX = static_cast<int>(std::floor(world.x / Config::WORLD_TILE_SIZE));
    int tileY = static_cast<int>(std::floor(world.y / Config::WORLD_TILE_SIZE));

    std::ostringstream ss;
    ss << "Px: (" << pixel.x << ", " << pixel.y << ")   "
       << "World: (" << std::fixed << std::setprecision(1)
       << world.x << ", " << world.y << ")   "
       << "Tile: (" << tileX << ", " << tileY << ")";

    s_text.setString(StringUtils::ToSfStringFromUtf8(ss.str()));
    window.draw(s_text);
}