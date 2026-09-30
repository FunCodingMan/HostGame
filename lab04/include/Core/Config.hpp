#pragma once
#include <SFML/Graphics.hpp>

namespace Config 
{
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

    inline constexpr float PARASITE_INITIAL_X = 100.f;
    inline constexpr float PARASITE_INITIAL_Y = 100.f;
    inline constexpr float PARASITE_WIDTH = 50.f;
    inline constexpr float PARASITE_HEIGHT = 60.f;
    inline constexpr float PARASITE_MAX_SPEED = 400.f;
    inline constexpr float PARASITE_ACCEL = 2000.f;
    inline constexpr float PARASITE_FRICTION = 1000.f;
    inline constexpr float PARASITE_GRAVITY = 1500.f;
    inline constexpr float PARASITE_JUMP_FORCE = 600.f;
    inline constexpr float MAX_FALL_SPEED = 1000.f;
    

    inline constexpr float PARASITE_DASH_FORCE = 1500.f;
    inline constexpr float DASH_COOLDOWN_TIME = 5.0f;
    inline constexpr float DASH_DURATION = 0.20f;


    inline const sf::Color PARASITE_COLOR = sf::Color(200, 50, 100);
    inline constexpr float EYE_SIZE = 12.f;
    inline constexpr float PUPIL_SIZE = 6.f;
    inline constexpr float EYE_OFFSET = 10.f;  
    inline constexpr float PUPIL_OFFSET = 3.f;
}