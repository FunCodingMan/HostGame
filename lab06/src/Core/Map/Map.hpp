#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Core/Config.hpp"
#include "Core/Map/Door.hpp"

class Map
{
public:
    Map();

    bool LoadTileConfig(const std::string& filename);
    bool LoadFromFile(const std::string& filename);

    void Update(float dt);

    Config::BlockNames GetTile(int x, int y) const;
    bool IsSolid(int x, int y, Config::EntityType entityType) const;
    bool IsSolidAt(sf::Vector2f worldPos) const;
    bool BlocksSight(int x, int y) const;
    bool IsBreakable(int x, int y) const;
    
    void BreakTile(int x, int y); 

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    int GetTileSize() const { return Config::WORLD_TILE_SIZE; }

    void BuildVertexArray();
    void Draw(sf::RenderWindow& window) const;

    const Config::TileData* GetTileData(Config::BlockNames blockName) const;

    bool IsIntersectingWith(const sf::FloatRect& bounds, Config::BlockNames targetBlock) const;

    float GetWidthInPixels() const { return m_width * Config::WORLD_TILE_SIZE; }
    float GetHeightInPixels() const { return m_height * Config::WORLD_TILE_SIZE; }
    
    bool IsDoor(int x, int y) const;
    bool OpenDoor(int x, int y);

private:
    std::vector<Config::BlockNames> m_grid;
    int m_width;
    int m_height;
    int m_tileSize;

    std::unordered_map<Config::BlockNames, Config::TileData> m_tileDictionary;

    std::unordered_map<std::string, Config::BlockNames> m_stringToBlock;

    void ParseTileConfig(const std::string& code, const std::string collisionStr, const int col, const int row);

    void LoadLeveFromLine(const std::string& line);

    Config::CollisionType StringToColisionType(const std::string& s);

    void InitStringMappings();

    void BuildVertexElement(int x, int y);

    Door*       FindDoor(int x, int y);
    const Door* FindDoor(int x, int y) const;

    sf::Texture m_tileset;
    sf::Texture m_doorTexture;
    std::vector<Door> m_doors;
    sf::VertexArray m_Vertices;
};