#pragma once
#include <SFML/Graphics.hpp>

namespace Config 
{
    inline constexpr int TEXTURE_TILE_SIZE = 32;
    inline constexpr int WORLD_TILE_SIZE = 64;

    enum class EntityType {
        Parasite,
        Human,
        Bullet
    };

    enum class BlockNames {
        Empty,
        Wall,
        Floor,
        BlackWall,
        Elevator,
        Ventilation
    };

    enum class CollisionType {
        Empty,
        Solid,
        Ventilation
    };

    struct TileData {
        CollisionType collision;
        sf::IntRect textureRect;
    };
    
    inline constexpr float WINDOW_WIDTH = 1080.f;
    inline constexpr float WINDOW_HEIGHT = 720.f;
    inline constexpr float FLOOR_Y = 600.f;
    inline const sf::Color BG_COLOR = sf::Color(40, 40, 40);

    inline constexpr float ENEMY_INITIAL_X = 960.f;
    inline constexpr float ENEMY_INITIAL_Y = 540.f;
    inline constexpr float ENEMY_SIZE = 40.f;
    inline constexpr float ENEMY_START_VELOCITY_X = 200.f;
    inline constexpr float ENEMY_START_VELOCITY_Y = 150.f;
    inline constexpr float ENEMY_FLASH_TIME = 0.5f;
    inline const sf::Color ENEMY_COLOR = sf::Color::Green;
    inline const sf::Color ENEMY_FLASH_COLOR = sf::Color::Red;

    inline constexpr float PARASITE_SCALE = 2.0f;
    inline constexpr float PARASITE_INITIAL_X = 200.f;
    inline constexpr float PARASITE_INITIAL_Y = 200.f;
    inline constexpr float PARASITE_HITBOX_WIDTH = 32.f;
    inline constexpr float PARASITE_HITBOX_HEIGHT = 32.f;
    inline constexpr float PARASITE_MAX_SPEED = 300.f;
    inline constexpr float PARASITE_ACCEL = 2000.f;
    inline constexpr float PARASITE_FRICTION = 2000.f;
    inline constexpr float PARASITE_GRAVITY = 2500.f;
    inline constexpr float PARASITE_JUMP_FORCE = 1000.f;
    inline constexpr float MAX_FALL_SPEED = 1000.f;
    

    inline constexpr float PARASITE_DASH_FORCE = 1500.f;
    inline constexpr float DASH_COOLDOWN_TIME = 2.f;
    inline constexpr float DASH_DURATION = 0.15f;

    inline constexpr float CAMERA_LERP_FACTOR = 5.0f;
    inline constexpr float CAMERA_ZOOM = 1.5f;
    inline constexpr float CAMERA_LOOK_STRENGTH = 0.4f;
    inline constexpr float CAMERA_DEAD_ZONE_WIDTH = 80.f;
    inline constexpr float CAMERA_DEAD_ZONE_HEIGHT = 50.f;

    inline const sf::Color PARASITE_COLOR = sf::Color(200, 50, 100);
    inline constexpr float EYE_SIZE = 12.f;
    inline constexpr float PUPIL_SIZE = 6.f;
    inline constexpr float EYE_OFFSET = 10.f;  
    inline constexpr float PUPIL_OFFSET = 3.f;

    inline constexpr float EPSILON = 0.1f;
}