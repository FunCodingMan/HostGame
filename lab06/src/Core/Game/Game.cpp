#include "Core/Game/Game.hpp"
#include "Core/Config.hpp"
#include "States/MainMenu/StateMainMenu.hpp"
#include <fstream>
#include <iostream>

using namespace Config;

Game::Game() 
{
    m_data->window.create(sf::VideoMode::getDesktopMode(), "The Host", sf::Style::Fullscreen);
    m_data->window.setKeyRepeatEnabled(false);
    m_data->window.setVerticalSyncEnabled(true);
    m_data->headerFont.loadFromFile("../assets/fonts/PressStart2P-Regular.ttf");
    m_data->hudFont.loadFromFile("../assets/fonts/Sligoil-Micro.ttf");

    m_data->sound.LoadSound(SOUND_ID_BREAK_GLASS, SOUND_PATH_BREAK_GLASS, VOL_BREAK_GLASS);
    m_data->sound.LoadSound(SOUND_ID_BUTTON,      SOUND_PATH_BUTTON,      VOL_BUTTON);
    m_data->sound.LoadSound(SOUND_ID_COLLECT,     SOUND_PATH_COLLECT,     VOL_COLLECT);
    m_data->sound.LoadSound(SOUND_ID_DEATH,       SOUND_PATH_DEATH,       VOL_DEATH);
    m_data->sound.LoadSound(SOUND_ID_HOST,        SOUND_PATH_HOST,        VOL_HOST);
    m_data->sound.LoadSound(SOUND_ID_JUMP,        SOUND_PATH_JUMP,        VOL_JUMP);
    m_data->sound.LoadSound(SOUND_ID_LOSE,        SOUND_PATH_LOSE,        VOL_LOSE);
    m_data->sound.LoadSound(SOUND_ID_SMASH,       SOUND_PATH_SMASH,       VOL_SMASH);
    m_data->sound.LoadSound(SOUND_ID_WALK,        SOUND_PATH_WALK,        VOL_WALK);
    m_data->sound.LoadSound(SOUND_ID_WIN,         SOUND_PATH_WIN,         VOL_WIN);
    m_data->sound.LoadSound(SOUND_ID_DASH,        SOUND_PATH_DASH,        VOL_DASH);

    m_data->sound.LoadSound(SOUND_ID_BULLETS_DROP, SOUND_PATH_BULLETS_DROP, VOL_BULLETS_DROP);
    m_data->sound.LoadSound(SOUND_ID_RELOAD, SOUND_PATH_RELOAD, VOL_RELOAD);
    m_data->sound.LoadSound(SOUND_ID_ALERT, SOUND_PATH_ALERT, VOL_ALERT);
    m_data->sound.LoadSound(SOUND_ID_REPORT, SOUND_PATH_REPORT, VOL_REPORT);
    m_data->sound.LoadSound(SOUND_ID_MILITARY_DEATH, SOUND_PATH_MILITARY_DEATH, VOL_MILITARY_DEATH);
    m_data->sound.LoadSound(SOUND_ID_HOST_WALK, SOUND_PATH_HOST_WALK, VOL_HOST_WALK);
    m_data->sound.LoadSound(SOUND_ID_DOOR_OPEN, SOUND_PATH_DOOR_OPEN, VOL_DOOR_OPEN);

    m_data->sound.LoadSound(SOUND_ID_SHOT_1, SOUND_PATH_SHOT_1, VOL_SHOT);
    m_data->sound.LoadSound(SOUND_ID_SHOT_2, SOUND_PATH_SHOT_2, VOL_SHOT);
    m_data->sound.LoadSound(SOUND_ID_SHOT_3, SOUND_PATH_SHOT_3, VOL_SHOT);
    m_data->sound.LoadSound(SOUND_ID_MILITARY_DAMAGE, SOUND_PATH_MILITARY_DAMAGE, VOL_MILITARY_DAMAGE);
    m_data->sound.LoadSound(SOUND_ID_SCIENTIST_DAMAGE, SOUND_PATH_SCIENTIST_DAMAGE, VOL_SCIENTIST_DAMAGE);
    m_data->sound.LoadSound(SOUND_ID_HEARTBEAT, SOUND_PATH_HEARTBEAT, VOL_HEARTBEAT);

    m_data->sound.LoadMusic(MUSIC_ID_BGM, MUSIC_PATH_BGM, VOL_BGM);

    LoadLevelsConfig();

    m_data->manager.AddState(StateRef(new StateMainMenu(m_data)));
}

void Game::LoadLevelsConfig()
{
    std::ifstream levelsFile("../assets/map/levels.txt");
    if (levelsFile.is_open())
    {
        std::string levelName;
        while (levelsFile >> levelName)
        {
            if (levelName[0] == '#')
            {
                std::string temp;
                std::getline(levelsFile, temp);
                continue;
            }
            m_data->levels.push_back(levelName);
        }
    }
    else
    {
        std::cerr << "Внимание: Не удалось открыть ../assets/map/levels.txt. Используем уровни по умолчанию.\n";
        m_data->levels = {"tutorial", "level1"};
    }
}

void Game::Run() 
{
    sf::Clock clock;
    float accumulator = 0.0f;

    while (m_data->window.isOpen()) 
    {
        m_data->manager.ProcessStateChanges();

        ProcessEvents();

        float frameTime = clock.restart().asSeconds();
        if (frameTime > 0.25f) frameTime = 0.25f;
        accumulator += frameTime;

        while (accumulator >= m_dt) 
        {
            Update();
            accumulator -= m_dt;
        }

        Render();
    }
}

void Game::ProcessEvents()
{
    sf::Event event;
    while (m_data->window.pollEvent(event)) 
    {
        if (event.type == sf::Event::Closed) {
            m_data->window.close();
        }
        
        if (!m_data->manager.isEmpty()) {
            m_data->manager.GetActiveState()->HandleInput(event);
        }
    }
}

void Game::Update()
{
    if (!m_data->manager.isEmpty()) {
        m_data->manager.GetActiveState()->Update(m_dt);
    }
}

void Game::Render()
{
    m_data->window.clear(Config::BG_COLOR);
    
    if (!m_data->manager.isEmpty()) {
        m_data->manager.GetActiveState()->Draw();
    }

    m_data->window.display();
}