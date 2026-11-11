#include "Core/LevelLoader/LevelLoader.hpp"
#include "Core/Map/Map.hpp"
#include "Core/Config.hpp"
#include "Entities/Host/Host.hpp"
#include "Entities/Scientist/Scientist.hpp"
#include "Entities/Items/Syringe/Syringe.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

bool LevelLoader::IsBoundsBlocked(const Map& map, const sf::FloatRect& bounds)
{
    const float ts = static_cast<float>(Config::WORLD_TILE_SIZE);

    int minX = static_cast<int>(std::floor(bounds.left / ts));
    int maxX = static_cast<int>(std::floor((bounds.left + bounds.width)  / ts));
    int minY = static_cast<int>(std::floor(bounds.top / ts));
    int maxY = static_cast<int>(std::floor((bounds.top  + bounds.height) / ts));

    for (int ty = minY; ty <= maxY; ++ty)
        for (int tx = minX; tx <= maxX; ++tx)
            if (map.IsSolid(tx, ty, Config::EntityType::Parasite))
                return true;

    return false;
}

void LevelLoader::LoadEnemies(const std::string& path,
                              std::vector<std::unique_ptr<Host>>& out,
                              const Map& map)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[LevelLoader] Cannot open enemies file '" << path << "'\n";
        return;
    }

    std::string line;
    int lineNum = 0;
    while (std::getline(file, line)) {
        ++lineNum;
        if (line.empty() || line[0] == '#') continue;

        std::istringstream ss(line);
        std::string type;
        float x, y;
        if (!(ss >> type >> x >> y)) {
            std::cerr << "[LevelLoader] " << path << ":" << lineNum
                      << " invalid format (expected 'Type x y [decay]')\n";
            continue;
        }

        sf::Vector2f pos(x, y);
        std::unique_ptr<Host> enemy;
        if (type == "Scientist") {
            enemy = std::make_unique<Scientist>(pos);
        } else {
            std::cerr << "[LevelLoader] " << path << ":" << lineNum
                      << " unknown enemy type '" << type << "'\n";
            continue;
        }

        float customDecay = 0.f;
        if (ss >> customDecay) enemy->SetMaxDecayTime(customDecay);

        if (IsBoundsBlocked(map, enemy->GetBounds())) {
            std::cerr << "[LevelLoader] " << path << ":" << lineNum
                      << " cannot spawn " << type << " at (" << x << "," << y
                      << "): hitbox overlaps a solid tile\n";
            continue;
        }

        out.push_back(std::move(enemy));
    }
}

void LevelLoader::LoadSyringes(const std::string& path,
                               std::vector<std::unique_ptr<Syringe>>& out,
                               const Map& map)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[LevelLoader] Cannot open syringes file '" << path << "'\n";
        return;
    }

    const float half = Config::SYRINGE_HITBOX_SIZE / 2.f;
    std::string line;
    int lineNum = 0;
    while (std::getline(file, line)) {
        ++lineNum;
        if (line.empty() || line[0] == '#') continue;

        std::istringstream ss(line);
        float x, y;
        if (!(ss >> x >> y)) {
            std::cerr << "[LevelLoader] " << path << ":" << lineNum
                      << " invalid format (expected 'x y')\n";
            continue;
        }

        sf::FloatRect bounds(x - half, y - half,
                             Config::SYRINGE_HITBOX_SIZE,
                             Config::SYRINGE_HITBOX_SIZE);
        if (IsBoundsBlocked(map, bounds)) {
            std::cerr << "[LevelLoader] " << path << ":" << lineNum
                      << " cannot spawn syringe at (" << x << "," << y
                      << "): hitbox overlaps a solid tile\n";
            continue;
        }

        out.push_back(std::make_unique<Syringe>(sf::Vector2f(x, y)));
    }
}