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
        Dead,
        Alert,
        Attack,
        Reload,
        Report
    };

    enum class GameState
    {
        MainMenu,
        Tutorial,
        Playing,
        Paused,
        GameOver,
        Victory
    };

    enum class EntityType
    {
        Parasite,
        Human,
        Military
    };

    enum class BlockNames
    {
        Empty,
        Wall,
        Floor,
        BlackWall,
        Elevator,
        Ventilation,
        Glass,
        Door,
        DoorOpen
    };

    enum class CollisionType
    {
        Empty,
        Solid,
        Ventilation,
        Breakable,
        Door
    };

    struct TileData
    {
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
    inline constexpr float SCIENTIST_SCALE = 2.0f;
    inline constexpr float SCIENTIST_INITIAL_X = 400.f;
    inline constexpr float SCIENTIST_INITIAL_Y = 400.f;
    inline constexpr float SCIENTIST_HITBOX_LEFT = 11.f;
    inline constexpr float SCIENTIST_HITBOX_RIGHT = 8.f;
    inline constexpr float SCIENTIST_HITBOX_TOP = 26.f;
    inline constexpr float SCIENTIST_HITBOX_BOTTOM = 32.f;

    inline constexpr float SCIENTIST_MAX_SPEED = 300.f;
    inline constexpr float SCIENTIST_ACCEL = 1500.f;
    inline constexpr float SCIENTIST_FRICTION = 2000.f;
    inline constexpr float SCIENTIST_GRAVITY = 3500.f;
    inline constexpr float SCIENTIST_JUMP_FORCE = 1000.f;
    inline constexpr float SCIENTIST_MAX_HEALTH = 5.f;

    inline constexpr int SCIENTIST_FRAME_W = 16;
    inline constexpr int SCIENTIST_FRAME_H = 32;

    inline constexpr float SCIENTIST_IDLE_FRAME_TIME = 0.40f;
    inline constexpr float SCIENTIST_WALK_FRAME_TIME = 0.10f;
    inline constexpr float PARASITE_DASH_FRAME_TIME = 0.04f;
    inline constexpr float SCIENTIST_WALK_THRESHOLD = 15.f;

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

    inline constexpr const char *SOUND_ID_BREAK_GLASS = "break_glass";
    inline constexpr const char *SOUND_ID_BUTTON = "button";
    inline constexpr const char *SOUND_ID_COLLECT = "collect";
    inline constexpr const char *SOUND_ID_DEATH = "death";
    inline constexpr const char *SOUND_ID_HOST = "host";
    inline constexpr const char *SOUND_ID_JUMP = "jump";
    inline constexpr const char *SOUND_ID_LOSE = "lose";
    inline constexpr const char *SOUND_ID_DASH = "dash";
    inline constexpr const char *SOUND_ID_SMASH = "smash";
    inline constexpr const char *SOUND_ID_WALK = "walk";
    inline constexpr const char *SOUND_ID_WIN = "win";
    inline constexpr const char *MUSIC_ID_BGM = "bgm_level1";

    inline constexpr const char *SOUND_PATH_BREAK_GLASS = "../assets/sounds/break_glass.wav";
    inline constexpr const char *SOUND_PATH_BUTTON = "../assets/sounds/button_sound.wav";
    inline constexpr const char *SOUND_PATH_COLLECT = "../assets/sounds/collect_syringe.wav";
    inline constexpr const char *SOUND_PATH_DEATH = "../assets/sounds/death.wav";
    inline constexpr const char *SOUND_PATH_HOST = "../assets/sounds/host.wav";
    inline constexpr const char *SOUND_PATH_JUMP = "../assets/sounds/jump.wav";
    inline constexpr const char *SOUND_PATH_LOSE = "../assets/sounds/loose.wav";
    inline constexpr const char *SOUND_PATH_DASH = "../assets/sounds/dash.wav";
    inline constexpr const char *SOUND_PATH_SMASH = "../assets/sounds/smash_floor.wav";
    inline constexpr const char *SOUND_PATH_WALK = "../assets/sounds/walk.wav";
    inline constexpr const char *SOUND_PATH_WIN = "../assets/sounds/win.wav";
    inline constexpr const char *MUSIC_PATH_BGM = "../assets/sounds/music.ogg";

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

    inline constexpr float MILITARY_SCALE            = 2.0f;
    inline constexpr int   MILITARY_FRAME_W          = 16;
    inline constexpr int   MILITARY_FRAME_H          = 32;
    inline constexpr float MILITARY_HITBOX_LEFT      = 11.f;
    inline constexpr float MILITARY_HITBOX_RIGHT     = 8.f;
    inline constexpr float MILITARY_HITBOX_TOP       = 26.f;
    inline constexpr float MILITARY_HITBOX_BOTTOM    = 32.f;

    inline constexpr float MILITARY_PATROL_SPEED     = 110.f;
    inline constexpr float MILITARY_RUN_SPEED        = 190.f;
    inline constexpr float MILITARY_ACCEL            = 1500.f;
    inline constexpr float MILITARY_FRICTION         = 2000.f;
    inline constexpr float MILITARY_GRAVITY          = 3500.f;
    inline constexpr float MILITARY_JUMP_FORCE       = 1000.f;
    inline constexpr float MILITARY_MAX_HEALTH       = 3.f;
    inline constexpr float MILITARY_DECAY_TIME       = 12.f;

    inline constexpr float MILITARY_VIEW_RANGE       = 420.f;
    inline constexpr float MILITARY_REACTION_TIME    = 0.35f;
    inline constexpr float MILITARY_FIRE_INTERVAL    = 0.45f;
    inline constexpr int   MILITARY_AMMO             = 6;
    inline constexpr float MILITARY_RELOAD_TIME      = 1.6f;
    inline constexpr float MILITARY_SHOT_DAMAGE      = 1.f;
    inline constexpr float MILITARY_LOSE_TARGET_TIME = 2.5f;

    inline constexpr float MILITARY_REPORT_RADIUS    = 64.f;
    inline constexpr float MILITARY_REPORT_TIMEOUT   = 2.f;

    inline constexpr float MILITARY_IDLE_FRAME_TIME  = 0.40f;
    inline constexpr float MILITARY_WALK_FRAME_TIME  = 0.12f;
    inline constexpr float MILITARY_SHOOT_FRAME_TIME = 0.06f;
    inline constexpr float MILITARY_DEATH_FRAME_TIME = 0.08f;
    inline constexpr float MILITARY_WALK_THRESHOLD   = 15.f;

    inline constexpr float HOST_DANGER_THRESHOLD   = 3.f;
    inline constexpr float HOST_DANGER_PULSE_SPEED = 6.f;

    inline constexpr const char* SOUND_ID_SHOT_1        = "shot_1";
    inline constexpr const char* SOUND_ID_SHOT_2        = "shot_2";
    inline constexpr const char* SOUND_ID_SHOT_3        = "shot_3";
    inline constexpr int         SOUND_SHOT_VARIANTS    = 3;
    inline constexpr const char* SOUND_ID_RELOAD        = "reload";
    inline constexpr const char* SOUND_ID_ALERT         = "alert";
    inline constexpr const char* SOUND_ID_REPORT        = "report";
    inline constexpr const char* SOUND_ID_EXPLOSION     = "explosion";
    inline constexpr const char* SOUND_ID_MILITARY_DEATH = "military_death";
    inline constexpr const char* SOUND_ID_MILITARY_STEP  = "military_step";
    inline constexpr const char* SOUND_ID_DOOR_OPEN     = "door_open";
    inline constexpr const char* SOUND_ID_DOOR_LOCKED   = "door_locked";

    inline constexpr const char* SOUND_PATH_SHOT_1        = "../assets/sounds/shot_1.wav";
    inline constexpr const char* SOUND_PATH_SHOT_2        = "../assets/sounds/shot_2.wav";
    inline constexpr const char* SOUND_PATH_SHOT_3        = "../assets/sounds/shot_3.wav";
    inline constexpr const char* SOUND_PATH_RELOAD        = "../assets/sounds/reload.wav";
    inline constexpr const char* SOUND_PATH_ALERT         = "../assets/sounds/alert.wav";
    inline constexpr const char* SOUND_PATH_REPORT        = "../assets/sounds/report.wav";
    inline constexpr const char* SOUND_PATH_EXPLOSION     = "../assets/sounds/explosion.wav";
    inline constexpr const char* SOUND_PATH_MILITARY_DEATH = "../assets/sounds/military_death.wav";
    inline constexpr const char* SOUND_PATH_MILITARY_STEP  = "../assets/sounds/military_step.wav";
    inline constexpr const char* SOUND_PATH_DOOR_OPEN     = "../assets/sounds/door_open.wav";
    inline constexpr const char* SOUND_PATH_DOOR_LOCKED   = "../assets/sounds/door_locked.wav";

    inline constexpr float VOL_SHOT           = 60.f;
    inline constexpr float VOL_RELOAD         = 50.f;
    inline constexpr float VOL_ALERT          = 70.f;
    inline constexpr float VOL_REPORT         = 60.f;
    inline constexpr float VOL_EXPLOSION      = 90.f;
    inline constexpr float VOL_MILITARY_DEATH = 80.f;
    inline constexpr float VOL_MILITARY_STEP  = 35.f;
    inline constexpr float VOL_DOOR_OPEN      = 70.f;
    inline constexpr float VOL_DOOR_LOCKED    = 60.f;

    inline constexpr float RAD_TO_DEG = 57.2957795f;

    inline constexpr int   BULLET_POOL_SIZE    = 64;
    inline constexpr float BULLET_SPEED        = 800.f;
    inline constexpr float BULLET_LENGTH       = 14.f;
    inline constexpr float BULLET_THICKNESS    = 4.f;
    inline constexpr float BULLET_HITBOX_SIZE  = 6.f;
    inline constexpr float BULLET_MAX_LIFETIME = 2.0f;

    inline const sf::Color BULLET_PLAYER_COLOR = sf::Color(255, 240, 120);
    inline const sf::Color BULLET_ENEMY_COLOR  = sf::Color(255, 110, 60);
    inline const sf::Color BULLET_DEBUG_COLOR  = sf::Color::Yellow;

    inline constexpr float LOS_STEP_SIZE       = 8.f;

    enum class TutorialStep
    {
        Movement,
        Dash,
        Jump,
        Possess,
        HostInfo,
        Heal,
        Eject,
        Exit
    };
}