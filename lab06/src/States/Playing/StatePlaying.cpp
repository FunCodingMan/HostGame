#include "States/Playing/StatePlaying.hpp"

#include "Core/GameKeyboard/GameKeyboard.hpp"
#include "States/GameOver/StateGameOver.hpp"
#include "States/Pause/StatePause.hpp"
#include "States/Victory/StateVictory.hpp"

StatePlaying::StatePlaying(GameDataRef data)
    : m_data(data),
      m_parasite(sf::Vector2f(Config::PARASITE_INITIAL_X, Config::PARASITE_INITIAL_Y)),
      m_camera(sf::Vector2f(Config::WINDOW_WIDTH / Config::CAMERA_ZOOM,
                            Config::WINDOW_HEIGHT / Config::CAMERA_ZOOM)),
      m_currentHost(nullptr)

{
}

void StatePlaying::Init()
{
    m_map.LoadTileConfig("../assets/map/tileset.txt");
    m_map.LoadFromFile("../assets/map/level1.txt");

    m_floatingHud.Init();
    m_screenHud.Init(m_data->headerFont, m_data->window.getSize().x);
    m_screenHud.SetHintText("Level 1\nLMB: Dash | RMB: Eject");

    m_enemies.push_back(std::make_unique<Scientist>(sf::Vector2f(400.f, 400.f)));
    m_enemies.push_back(std::make_unique<Scientist>(sf::Vector2f(600.f, 400.f)));
    m_enemies.push_back(std::make_unique<Scientist>(sf::Vector2f(800.f, 400.f)));

    m_parasite.SetPlayerControlled(true);
    m_playerAvatar = &m_parasite;
    m_timeInCurrentHost = 0.f;
}

void StatePlaying::HandleInput(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (GameKeyboard::IsHitboxesPressed(event))
        {
            Entity::ToggleHitboxes();
        }
        if (GameKeyboard::IsEventEscape(event))
        {
            m_data->manager.AddState(StateRef(new StatePause(m_data)), false);
        }
    }
}

void StatePlaying::UpdateCamera(float dt)
{
    sf::Vector2f targetPos = m_playerAvatar->GetPosition();
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

    m_camera.Update(targetPos, mouseOffset, dt, m_map.GetWidthInPixels(),
                    m_map.GetHeightInPixels());
}

void StatePlaying::Update(float dt)
{
    UpdateCamera(dt);

    GameContext ctx;
    ctx.dt = dt;
    ctx.mousePos =
        m_data->window.mapPixelToCoords(sf::Mouse::getPosition(m_data->window), m_camera.GetView());
    ctx.levelMap = &m_map;

    m_parasite.Update(ctx);

    for (auto &enemy : m_enemies)
    {
        enemy->Update(ctx);
    }

    HandlePossession();
    HandleEjection(ctx);

    CleanDeadEnemies();

    UpdateHUD();

    CheckEndGameConditions();
}

void StatePlaying::HandlePossession()
{
    if (m_playerAvatar != &m_parasite || !m_parasite.IsDashing())
        return;

    for (auto &enemy : m_enemies)
    {
        if (!enemy->IsDead() && m_parasite.GetBounds().intersects(enemy->GetBounds()))
        {
            m_parasite.SetPlayerControlled(false);

            enemy->SetPlayerControlled(true);
            enemy->ResetDecayTimer();

            m_playerAvatar = enemy.get();
            m_currentHost = enemy.get();
            m_timeInCurrentHost = 0.f;
            break;
        }
    }
}

void StatePlaying::HandleEjection(const GameContext &ctx)
{
    if (m_currentHost == nullptr || m_playerAvatar != m_currentHost)
        return;

    m_timeInCurrentHost += ctx.dt;

    bool manualEject = sf::Mouse::isButtonPressed(sf::Mouse::Right);
    bool timeOut = (m_currentHost->GetDecayTimer() <= 0.f);

    if (manualEject && m_timeInCurrentHost < Config::COOLDOWN_IN_HOST)
    {
        manualEject = false;
    }

    if (manualEject || timeOut)
    {
        m_currentHost->SetPlayerControlled(false);
        m_currentHost->TakeDamage(9999.f);

        m_parasite.SetPosition(m_currentHost->GetPosition());
        m_parasite.SetPlayerControlled(true);

        if (manualEject)
        {
            m_parasite.ForceDash(ctx.mousePos);
        }

        m_playerAvatar = &m_parasite;
        m_currentHost = nullptr;
    }
}

void StatePlaying::UpdateHUD()
{
    m_floatingHud.SetPosition(m_playerAvatar->GetPosition());

    if (m_currentHost != nullptr && m_playerAvatar == m_currentHost)
    {
        m_floatingHud.Update(m_currentHost->GetDecayTimer(), m_currentHost->GetMaxDecayTimer(),
                             sf::Color(220, 200, 0));
    }
    else
    {
        m_floatingHud.Update(m_parasite.GetLife(), m_parasite.GetMaxLife(), sf::Color(200, 0, 0));
    }
}

void StatePlaying::CheckEndGameConditions()
{

    bool isParasiteDead = (m_parasite.GetLife() <= 0.f);

    bool fellIntoPit = (m_playerAvatar->GetPosition().y > m_map.GetHeightInPixels());

    if (isParasiteDead || fellIntoPit)
    {
        m_data->manager.AddState(StateRef(new StateGameOver(m_data)), false);
        return;
    }

    if (m_map.IsIntersectingWith(m_playerAvatar->GetBounds(), Config::BlockNames::Elevator))
    {
        m_data->manager.AddState(StateRef(new StateVictory(m_data)), false);
        return;
    }
}

void StatePlaying::CleanDeadEnemies()
{
    m_enemies.erase(std::remove_if(
        m_enemies.begin(), m_enemies.end(),
        [](const std::unique_ptr<Scientist> &enemy) { return enemy->IsDead(); }),
        m_enemies.end()
    );
}

void StatePlaying::Draw()
{
    m_data->window.setView(m_camera.GetView());
    m_map.Draw(m_data->window);

    for (auto &enemy : m_enemies)
    {
        if (!enemy->IsDead())
        {
            enemy->Draw(m_data->window);
        }
    }

    if (m_parasite.IsPlayerControlled())
    {
        m_parasite.Draw(m_data->window);
    }

    m_floatingHud.Draw(m_data->window);

    m_data->window.setView(m_data->window.getDefaultView());

    m_screenHud.Draw(m_data->window);
}