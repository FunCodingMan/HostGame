#include "Core/Map/Map.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

Map::Map() : m_width(0), m_height(0)
{
    if (!m_tileset.loadFromFile("../assets/images/tileset.png")) {
        std::cerr << "Ошибка: Не удалось загрузить tileset.png" << std::endl;
    }

    m_Vertices.setPrimitiveType(sf::Quads);
    
    InitStringMappings();
}

void Map::InitStringMappings()
{
    m_stringToBlock[".."] = Config::BlockNames::Empty;
    m_stringToBlock["wa"] = Config::BlockNames::Wall;
    m_stringToBlock["fl"] = Config::BlockNames::Floor;
    m_stringToBlock["bw"] = Config::BlockNames::BlackWall;
    m_stringToBlock["el"] = Config::BlockNames::Elevator;
    m_stringToBlock["ve"] = Config::BlockNames::Ventilation;
}

Config::CollisionType Map::StringToColisionType(const string& s)
{
    if (s == "Empty")
    {
        return Config::CollisionType::Empty;
    }
    if (s == "Solid")
    {
        return Config::CollisionType::Solid;
    }
    if (s == "Ventilation")
    {
        return Config::CollisionType::Ventilation;
    }
    return Config::CollisionType::Solid;
} 

void Map::ParseTileConfig(const string& code, const string collisionStr, const int col, const int row)
{
    Config::BlockNames blockName = m_stringToBlock[code];

    Config::CollisionType type = StringToColisionType(collisionStr);

    m_tileDictionary[blockName] = {type, sf::IntRect(col * Config::TEXTURE_TILE_SIZE, 
                                                     row * Config::TEXTURE_TILE_SIZE, 
                                                     Config::TEXTURE_TILE_SIZE, 
                                                     Config::TEXTURE_TILE_SIZE)};
}

bool Map::LoadTileConfig(const string& filename)
{
    ifstream file(filename);
    if(!file.is_open()) return false;

    string line;

    while (getline(file, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }
        stringstream ss(line);
        string code, collisionStr;

        int col, row;

        if (ss >> code >> collisionStr >> col >> row)
        {
            if (m_stringToBlock.find(code) == m_stringToBlock.end()) {
                std::cerr << "Внимание: Найден неизвестный код '" << code << "' в " << filename << std::endl;
                continue;
            }
            ParseTileConfig(code, collisionStr, col, row);
        }
    }
    return true;
}

void Map::LoadLeveFromLine(const string& line)
{
    stringstream ss(line);
    string tileCode;
    int curLineWidth = 0;

    while (ss >> tileCode)
    {
        if (m_stringToBlock.find(tileCode) == m_stringToBlock.end())
        {
            std::cerr << "Предупреждение: Неизвестный тайл '" << tileCode << "'" << std::endl;
            m_grid.push_back(Config::BlockNames::Empty);
        }
        else
        {
            m_grid.push_back(m_stringToBlock[tileCode]);
        }
        curLineWidth++;
    }
    if (m_width == 0) 
    {
        m_width = curLineWidth;
    }
    m_height++;
}

bool Map::LoadFromFile(const string& filename)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Ошибка: Не удалось загрузить карту " << filename << std::endl;
        return false;
    }

    m_grid.clear();

    string line;
    m_height = 0;
    m_width = 0;

    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        LoadLeveFromLine(line);
    }

    BuildVertexArray();

    return true;
}

Config::BlockNames Map::GetTile(int x, int y) const
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
    {
        return Config::BlockNames::BlackWall;
    }

    return m_grid[y * m_width + x];
}

const Config::TileData* Map::GetTileData(Config::BlockNames blockName) const
{
    auto it = m_tileDictionary.find(blockName);
    if (it != m_tileDictionary.end()) {
        return &it->second;
    }
    return nullptr;
}

bool Map::IsSolid(int x, int y, Config::EntityType entityType) const 
{
    Config::BlockNames block = GetTile(x, y);
    const Config::TileData* data = GetTileData(block);
    
    if (!data) return true; 

    Config::CollisionType type = data->collision;

    if (type == Config::CollisionType::Solid) return true;
    if (type == Config::CollisionType::Empty) return false;
    
    if (type == Config::CollisionType::Ventilation) 
    {
        return entityType != Config::EntityType::Parasite; 
    }

    return false;
}

void Map::BuildVertexElement(int xTile, int yTile)
{
    Config::BlockNames blockName = GetTile(xTile, yTile);

    const Config::TileData* tileData = GetTileData(blockName);
    if (!tileData)
    {
        return;
    }

    int vertexElIdx = (xTile + yTile * m_width) * 4;

    sf::Vertex* quad = &m_Vertices[vertexElIdx];
    float wSize = Config::WORLD_TILE_SIZE;
    float x = xTile * wSize;
    float y = yTile * wSize;

    quad[0].position = sf::Vector2f(x, y);
    quad[1].position = sf::Vector2f(x + wSize, y);
    quad[2].position = sf::Vector2f(x + wSize, y + wSize);
    quad[3].position = sf::Vector2f(x, y + wSize);

    sf::IntRect texRect = tileData->textureRect;
    float tx = texRect.left;
    float ty = texRect.top;
    float tSize = texRect.width;

    quad[0].texCoords = sf::Vector2f(tx, ty);
    quad[1].texCoords = sf::Vector2f(tx + tSize, ty);
    quad[2].texCoords = sf::Vector2f(tx + tSize, ty + tSize);
    quad[3].texCoords = sf::Vector2f(tx, ty + tSize);
}

void Map::BuildVertexArray()
{
    m_Vertices.resize(m_width * m_height * 4);

    for (int i = 0; i < m_width; ++i)
    {
        for (int j = 0; j < m_height; ++j)
        {
            BuildVertexElement(i, j);
        }
    }
}

void Map::Draw(sf::RenderWindow& window) const
{
    window.draw(m_Vertices, &m_tileset);
}
