#include "States/Playing/StatePlaying.hpp"
#include "Core/GameKeyboard/GameKeyboard.hpp"
#include "States/Pause/StatePause.hpp"

StatePlaying::StatePlaying(GameDataRef data) 
    : m_data(data),
      m_parasite(sf::Vector2f(Config::PARASITE_INITIAL_X, Config::PARASITE_INITIAL_Y)),
      m_enemy(sf::Vector2f(Config::ENEMY_INITIAL_X, Config::ENEMY_INITIAL_Y)),
      m_camera(sf::Vector2f(Config::WINDOW_WIDTH / Config::CAMERA_ZOOM, Config::WINDOW_HEIGHT / Config::CAMERA_ZOOM))
{
}

void StatePlaying::Init()
{
    m_map.LoadTileConfig("../assets/map/tileset.txt");
    m_map.LoadFromFile("../assets/map/level1.txt");
}

void StatePlaying::HandleInput(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (GameKeyboard::IsHitboxesPressed(event)) {
            Entity::ToggleHitboxes();
        }
        if (GameKeyboard::IsEventEscape(event)) {
            m_data->manager.AddState(StateRef(new StatePause(m_data)), false);
        }
    }
}

void StatePlaying::UpdateCamera(float dt)
{
    sf::Vector2f targetPos = m_parasite.GetPosition();
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(m_data->window);
    sf::Vector2u windowSize = m_data->window.getSize();

    float normX = (mousePixelPos.x - (windowSize.x / 2.f)) / (windowSize.x / 2.f);
    float normY = (mousePixelPos.y - (windowSize.y / 2.f)) / (windowSize.y / 2.f);

    normX = std::clamp(normX, -1.f, 1.f);
    normY = std::clamp(normY, -1.f, 1.f);

    sf::Vector2f viewSize = m_camera.GetView().getSize();
    float maxOffsetX = (viewSize.x / 2.f) * Config::CAMERA_LOOK_STRENGTH;
    float maxOffsetY = (viewSize.y / 2.f) * Config::CAMERA_LOOK_STRENGTH;

    sf::Vector2f mouseOffset(normX * maxOffsetX, normY * maxOffsetY);

    m_camera.Update(targetPos, mouseOffset, dt, m_map.GetWidthInPixels(), m_map.GetHeightInPixels());
}

void StatePlaying::Update(float dt)
{
    UpdateCamera(dt);

    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(m_data->window);
    sf::Vector2f mousePos = m_data->window.mapPixelToCoords(mousePixelPos, m_camera.GetView());

    GameContext ctx;
    ctx.dt = dt;
    ctx.mousePos = mousePos;
    ctx.levelMap = &m_map;
    
    m_parasite.Update(ctx);
    m_enemy.Update(dt);

    if (m_parasite.GetBounds().intersects(m_enemy.GetBounds())) {
        m_enemy.OnCollision();
    }
}

void StatePlaying::Draw()
{
    m_data->window.setView(m_camera.GetView());
    m_map.Draw(m_data->window);
    m_parasite.Draw(m_data->window);

    m_data->window.setView(m_data->window.getDefaultView());
}