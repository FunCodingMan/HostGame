#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>

class Map;
class Host;
class Syringe;

class LevelLoader
{
public:
    static void LoadEnemies(const std::string& path,
                            std::vector<std::unique_ptr<Host>>& out,
                            const Map& map);

    static void LoadSyringes(const std::string& path,
                             std::vector<std::unique_ptr<Syringe>>& out,
                             const Map& map);

private:
    static bool IsBoundsBlocked(const Map& map, const sf::FloatRect& bounds);
};