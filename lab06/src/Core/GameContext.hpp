#pragma once
#include <SFML/Graphics.hpp>
#include "Core/Map/Map.hpp"
#include "Core/Sound/SoundManager/SoundManager.hpp"
#include "Entities/Bullet/BulletManager/BulletManager.hpp"

class LivingEntity;

struct GameContext
{
    float dt;
    sf::Vector2f mousePos;
    Map* levelMap;
    SoundManager* audio;
    BulletManager* bullets = nullptr;

    LivingEntity* playerAvatar = nullptr;
    bool avatarIsParasite = true;
};