#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Core/Config.hpp"

class Map
{
public:
    Map();

    bool LoadTileConfig(const std::string& filename);
    bool LoadFromFile(const std::string& filename);

    Config::BlockNames GetTile(int x, int y) const;
    bool IsSolid(int x, int y, Config::EntityType entityType) const;

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    int GetTileSize() const { return tileSize; }

    const Config::TileData* GetTileData(Config::BlockNames blockName) const;

private:
    std::vector<Config::BlockNames> grid;
    int width;
    int height;
    int tileSize;

    std::unordered_map<Config::BlockNames, Config::TileData> tileDictionary;

    std::unordered_map<std::string, Config::BlockNames> stringToBlock;

    void ParseTileConfig(const std::string& code, const std::string collisionStr, const int col, const int row);

    void LoadLeveFromLine(const std::string& line);

    Config::CollisionType StringToColisionType(const std::string& s);

    void InitTileDictionary();

    void InitStringMappings();
};