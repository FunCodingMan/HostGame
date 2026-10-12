#include "Core/Map/Map.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

Map::Map() : width(0), height(0)
{
    if (!tileset.loadFromFile("../assets/tileset.png")) {
        std::cerr << "Ошибка: Не удалось загрузить tileset.png" << std::endl;
    }

    mVertices.setPrimitiveType(sf::Quads);
    
    InitStringMappings();
}

void Map::InitStringMappings()
{
    stringToBlock[".."] = Config::BlockNames::Empty;
    stringToBlock["wa"] = Config::BlockNames::Wall;
    stringToBlock["fl"] = Config::BlockNames::Floor;
    stringToBlock["bw"] = Config::BlockNames::BlackWall;
    stringToBlock["el"] = Config::BlockNames::Elevator;
    stringToBlock["ve"] = Config::BlockNames::Ventilation;
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
    Config::BlockNames blockName = stringToBlock[code];

    Config::CollisionType type = StringToColisionType(collisionStr);

    tileDictionary[blockName] = {type, sf::IntRect(col * Config::TEXTURE_TILE_SIZE, 
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
            if (stringToBlock.find(code) == stringToBlock.end()) {
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
        if (stringToBlock.find(tileCode) == stringToBlock.end())
        {
            std::cerr << "Предупреждение: Неизвестный тайл '" << tileCode << "'" << std::endl;
            grid.push_back(Config::BlockNames::Empty);
        }
        else
        {
            grid.push_back(stringToBlock[tileCode]);
        }
        curLineWidth++;
    }
    if (width == 0) 
    {
        width = curLineWidth;
    }
    height++;
}

bool Map::LoadFromFile(const string& filename)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Ошибка: Не удалось загрузить карту " << filename << std::endl;
        return false;
    }

    grid.clear();

    string line;
    height = 0;
    width = 0;

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
    if (x < 0 || x >= width || y < 0 || y >= height)
    {
        return Config::BlockNames::BlackWall;
    }

    return grid[y * width + x];
}

const Config::TileData* Map::GetTileData(Config::BlockNames blockName) const
{
    auto it = tileDictionary.find(blockName);
    if (it != tileDictionary.end()) {
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

    int vertexElIdx = (xTile + yTile * width) * 4;

    sf::Vertex* quad = &mVertices[vertexElIdx];
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
    mVertices.resize(width * height * 4);

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            BuildVertexElement(i, j);
        }
    }
}

void Map::Draw(sf::RenderWindow& window) const
{
    window.draw(mVertices, &tileset);
}
