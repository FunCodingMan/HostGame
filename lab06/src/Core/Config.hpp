#pragma once
#include <SFML/Graphics.hpp>

namespace Config 
{
    inline constexpr int TEXTURE_TILE_SIZE = 32;
    inline constexpr int WORLD_TILE_SIZE = 64;

    enum class EntityState
    {
        None,
        Idle,
        Walk,
        Jump,
        Fall,
        Dash,
        Dead
    };

    enum class GameState {
        MainMenu,
        Tutorial,
        Playing,
        Paused,
        GameOver,
        Victory
    };

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
        Ventilation,
        Glass
    };

    enum class CollisionType {
        Empty,
        Solid,
        Ventilation,
        Breakable
    };

    struct TileData {
        CollisionType collision;
        sf::IntRect textureRect;
    };
    
    inline constexpr float WINDOW_WIDTH = 1080.f;
    inline constexpr float WINDOW_HEIGHT = 720.f;
    inline constexpr float FLOOR_Y = 600.f;
    inline const sf::Color BG_COLOR = sf::Color(40, 40, 40);

    inline constexpr float ENEMY_INITIAL_X = 400.f;
    inline constexpr float ENEMY_INITIAL_Y = 400.f;
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
    inline constexpr float PARASITE_MAX_HEALTH = 5.f; 
    inline constexpr float MAX_FALL_SPEED = 1000.f;
    inline constexpr int PARASITE_FRAME_W = 16;
    inline constexpr int PARASITE_FRAME_H = 16;

    inline constexpr float PARASITE_IDLE_FRAME_TIME = 0.25f;
    

    inline constexpr float PARASITE_DASH_FORCE = 1500.f;
    inline constexpr float SCIENTIST_SCALE          = 2.0f;
    inline constexpr float SCIENTIST_INITIAL_X      = 400.f;
    inline constexpr float SCIENTIST_INITIAL_Y      = 400.f;
    inline constexpr float SCIENTIST_HITBOX_LEFT   = 11.f;
    inline constexpr float SCIENTIST_HITBOX_RIGHT  = 8.f;
    inline constexpr float SCIENTIST_HITBOX_TOP    = 26.f;
    inline constexpr float SCIENTIST_HITBOX_BOTTOM = 32.f;

    inline constexpr float SCIENTIST_MAX_SPEED   = 300.f;
    inline constexpr float SCIENTIST_ACCEL       = 1500.f;
    inline constexpr float SCIENTIST_FRICTION    = 2000.f;
    inline constexpr float SCIENTIST_GRAVITY     = 3500.f;
    inline constexpr float SCIENTIST_JUMP_FORCE  = 1000.f;
    inline constexpr float SCIENTIST_MAX_HEALTH  = 50.f;

    inline constexpr int   SCIENTIST_FRAME_W = 16;
    inline constexpr int   SCIENTIST_FRAME_H = 32;

    inline constexpr float SCIENTIST_IDLE_FRAME_TIME = 0.40f;
    inline constexpr float SCIENTIST_WALK_FRAME_TIME = 0.10f;
    inline constexpr float PARASITE_DASH_FRAME_TIME = 0.04f;
    inline constexpr float SCIENTIST_WALK_THRESHOLD  = 15.f;

    inline constexpr float SCIENTIST_DECAY_TIME = 10.f;  

    inline constexpr float DASH_COOLDOWN_TIME = 2.f;
    inline constexpr float DASH_DURATION = 0.15f;

    inline constexpr float SYRINGE_HITBOX_SIZE = 24.f;
    inline constexpr float SYRINGE_DECAY_RESTORE = 5.f;     
    inline constexpr float SYRINGE_FLOAT_SPEED = 4.f;        
    inline constexpr float SYRINGE_FLOAT_AMPLITUDE = 5.f;

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

    inline constexpr float PHYSICS_FPS = 60.f;

    inline constexpr float TIME_FOR_ONE_FRAME = 1.0f / PHYSICS_FPS;

    inline constexpr float MENU_BUTTON_TEXT_SIZE = 50.f;

    inline constexpr float PLAYER_HUD_WIDTH = 50.f;

    inline constexpr float COOLDOWN_IN_HOST = 1.f;

    inline constexpr const char* SOUND_ID_BREAK_GLASS = "break_glass";
    inline constexpr const char* SOUND_ID_BUTTON = "button";
    inline constexpr const char* SOUND_ID_COLLECT = "collect";
    inline constexpr const char* SOUND_ID_DEATH = "death";
    inline constexpr const char* SOUND_ID_HOST = "host";
    inline constexpr const char* SOUND_ID_JUMP = "jump";
    inline constexpr const char* SOUND_ID_LOSE = "lose";
    inline constexpr const char* SOUND_ID_DASH = "dash";
    inline constexpr const char* SOUND_ID_SMASH = "smash";
    inline constexpr const char* SOUND_ID_WALK = "walk";
    inline constexpr const char* SOUND_ID_WIN = "win";
    inline constexpr const char* MUSIC_ID_BGM = "bgm_level1";

    inline constexpr const char* SOUND_PATH_BREAK_GLASS = "../assets/sounds/break_glass.wav";
    inline constexpr const char* SOUND_PATH_BUTTON      = "../assets/sounds/button_sound.wav";
    inline constexpr const char* SOUND_PATH_COLLECT     = "../assets/sounds/collect_syringe.wav";
    inline constexpr const char* SOUND_PATH_DEATH       = "../assets/sounds/death.wav";
    inline constexpr const char* SOUND_PATH_HOST        = "../assets/sounds/host.wav";
    inline constexpr const char* SOUND_PATH_JUMP        = "../assets/sounds/jump.wav";
    inline constexpr const char* SOUND_PATH_LOSE        = "../assets/sounds/loose.wav";
    inline constexpr const char* SOUND_PATH_DASH        = "../assets/sounds/dash.wav";
    inline constexpr const char* SOUND_PATH_SMASH       = "../assets/sounds/smash_floor.wav";
    inline constexpr const char* SOUND_PATH_WALK        = "../assets/sounds/walk.wav";
    inline constexpr const char* SOUND_PATH_WIN         = "../assets/sounds/win.wav";
    inline constexpr const char* MUSIC_PATH_BGM         = "../assets/sounds/music.ogg";

    inline constexpr float VOL_BREAK_GLASS = 70.f;
    inline constexpr float VOL_BUTTON = 70.f;
    inline constexpr float VOL_COLLECT = 90.f;
    inline constexpr float VOL_DEATH = 100.f;
    inline constexpr float VOL_HOST = 90.f;
    inline constexpr float VOL_JUMP = 40.f;
    inline constexpr float VOL_LOSE = 90.f;
    inline constexpr float VOL_DASH = 30.f;
    inline constexpr float VOL_SMASH = 10.f;
    inline constexpr float VOL_WALK = 40.f;
    inline constexpr float VOL_WIN = 90.f;
    inline constexpr float VOL_BGM = 5.f;

    inline constexpr float STEP_SOUND_INTERVAL = 0.35f;
    inline constexpr float WALK_SOUND_VELOCITY = 15.f;
    inline constexpr float SMASH_FALL_SPEED_RATIO = 0.6f;
    inline constexpr float EJECT_DAMAGE = 9999.f;
}