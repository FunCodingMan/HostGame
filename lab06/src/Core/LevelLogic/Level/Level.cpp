#include "Core/LevelLogic/Level/Level.hpp"
#include "Core/Config.hpp"
#include "Core/LevelLoader/LevelLoader.hpp"
#include "Core/GameKeyboard/GameKeyboard.hpp"
#include "Core/Sound/SoundManager/SoundManager.hpp"
#include <algorithm>

using namespace Config;

Level::Level()
    : m_camera(sf::Vector2f(WINDOW_WIDTH / CAMERA_ZOOM,
                            WINDOW_HEIGHT / CAMERA_ZOOM)),
      m_parasite(sf::Vector2f(PARASITE_INITIAL_X, PARASITE_INITIAL_Y))
{
}

void Level::Load(const std::string& name, SoundManager& audio)
{
    m_audio = &audio;
    m_stats.Reset();
    m_result = LevelResult::None;

    m_map.LoadTileConfig("../assets/map/tileset.txt");
    m_map.LoadFromFile("../assets/map/" + name + ".txt");

    m_hosts.clear();
    m_syringes.clear();
    m_bullets.Clear();

    LevelLoader::LoadEnemies("../assets/map/enemies_" + name + ".txt", m_hosts, m_map);
    LevelLoader::LoadSyringes("../assets/map/syringes_" + name + ".txt", m_syringes, m_map);

    m_parasite.SetPlayerControlled(true);
    m_playerAvatar = &m_parasite;
    m_currentHost = nullptr;
    m_timeInCurrentHost = 0.f;
}

void Level::UpdateCamera(float dt, const LevelInput& input)
{
    sf::Vector2f targetPos = m_playerAvatar->GetPosition();
    sf::Vector2i mousePixelPos = input.mousePixelPos;
    sf::Vector2u windowSize = input.windowSize;

    float normX = (mousePixelPos.x - (windowSize.x / 2.f)) / (windowSize.x / 2.f);
    float normY = (mousePixelPos.y - (windowSize.y / 2.f)) / (windowSize.y / 2.f);

    normX = std::clamp(normX, -1.f, 1.f);
    normY = std::clamp(normY, -1.f, 1.f);

    sf::Vector2f viewSize = m_camera.GetView().getSize();
    float maxOffsetX = (viewSize.x / 2.f) * Config::CAMERA_LOOK_STRENGTH;
    float maxOffsetY = (viewSize.y / 2.f) * Config::CAMERA_LOOK_STRENGTH;

    sf::Vector2f mouseOffset(normX * maxOffsetX, normY * maxOffsetY);

    m_camera.Update(targetPos, mouseOffset, dt,
                    m_map.GetWidthInPixels(), m_map.GetHeightInPixels());
}

sf::Vector2f Level::MapMouseToWorld(const LevelInput& input) const
{
    const sf::View& view = m_camera.GetView();
    float w = static_cast<float>(input.windowSize.x);
    float h = static_cast<float>(input.windowSize.y);

    return sf::Vector2f(
        view.getCenter().x - view.getSize().x / 2.f + (input.mousePixelPos.x / w) * view.getSize().x,
        view.getCenter().y - view.getSize().y / 2.f + (input.mousePixelPos.y / h) * view.getSize().y);
}

void Level::Update(float dt, const LevelInput& input)
{
    m_stats.timeSpent += dt;

    UpdateCamera(dt, input);

    GameContext ctx;
    ctx.dt = dt;
    ctx.mousePos = MapMouseToWorld(input);
    ctx.levelMap = &m_map;
    ctx.audio = m_audio;
    ctx.playerAvatar = m_playerAvatar;
    ctx.avatarIsParasite = (m_playerAvatar == &m_parasite);
    ctx.bullets = &m_bullets;

    m_parasite.Update(ctx);

    for (auto& enemy : m_hosts)
    {
        enemy->Update(ctx);
    }
    for (auto& syringe : m_syringes)
    {
        syringe->Update(ctx);
    }
    
    m_bullets.Update(dt, m_map);

    HandleBullets();
    HandlePossession();
    HandleEjection(ctx);
    HandleItems(ctx);

    CleanDeadEnemies();

    m_syringes.erase(std::remove_if(m_syringes.begin(), m_syringes.end(),
                                    [](const auto& s) { return s->IsCollected(); }),
                     m_syringes.end());

    CheckEndConditions();
}

void Level::HandleBullets()
{
    for (Bullet& bullet : m_bullets.GetBullets())
    {
        if (!bullet.active)
        {
            continue;
        }

        sf::FloatRect bulletBounds = BulletManager::GetBulletBounds(bullet);

        if (!bullet.fromPlayer)
        {
            if (m_playerAvatar != nullptr && !m_playerAvatar->IsDead() &&
                bulletBounds.intersects(m_playerAvatar->GetBounds()))
            {
                m_playerAvatar->TakeDamage(bullet.damage);
                bullet.active = false;
            }
            continue;
        }

        for (auto& enemy : m_hosts)
        {
            if (enemy->IsDead() || enemy.get() == m_currentHost)
            {
                continue;
            }

            if (bulletBounds.intersects(enemy->GetBounds()))
            {
                bool wasDead = enemy->IsDead();
                enemy->TakeDamage(bullet.damage);
                bullet.active = false;

                if (!wasDead && enemy->IsDead())
                {
                    const char* sound = enemy->GetDeathSound();
                    if (m_audio->HasSound(sound))
                    {
                        m_audio->PlaySound(sound);
                    }
                }
                break;
            }
        }
    }
}

void Level::HandlePossession()
{
    if (m_playerAvatar != &m_parasite || !m_parasite.IsDashing())
        return;

    for (auto& enemy : m_hosts)
    {
        if (!enemy->IsDead() && m_parasite.GetBounds().intersects(enemy->GetBounds()))
        {
            m_parasite.SetPlayerControlled(false);
            m_parasite.ResetMovement();

            enemy->SetPlayerControlled(true);
            enemy->ResetDecayTimer();
            enemy->OnPossessed();

            m_playerAvatar = enemy.get();
            m_currentHost = enemy.get();
            m_timeInCurrentHost = 0.f;

            m_stats.hostsPossessed++;
            m_audio->PlaySound(SOUND_ID_HOST);
            break;
        }
    }
}

void Level::HandleEjection(const GameContext& ctx)
{
    if (m_currentHost == nullptr || m_playerAvatar != m_currentHost)
        return;

    m_timeInCurrentHost += ctx.dt;

    bool manualEject = GameKeyboard::IsEjectKeyPressed();
    bool timeOut = (m_currentHost->GetDecayTimer() <= 0.f);
    bool hostDead = m_currentHost->IsDead();

    if (manualEject && m_timeInCurrentHost < Config::COOLDOWN_IN_HOST)
        manualEject = false;

    if (manualEject || timeOut || hostDead)
    {
        sf::Vector2f ejectPosition = m_currentHost->GetPosition();
        bool hostDiedNow = !hostDead && (manualEject || timeOut);

        m_currentHost->SetPlayerControlled(false);
        m_currentHost->OnEjected();

        if (hostDiedNow)
        {
            m_currentHost->Kill();
        }

        if (hostDiedNow || hostDead)
        {
            const char* sound = m_currentHost->GetDeathSound();
            if (ctx.audio->HasSound(sound))
            {
                ctx.audio->PlaySound(sound);
            }
        }

        m_parasite.SetPosition(ejectPosition);
        m_parasite.ResetMovement();
        m_parasite.SetPlayerControlled(true);

        if (manualEject)
            m_parasite.ForceDash(ctx);

        m_playerAvatar = &m_parasite;
        m_currentHost = nullptr;
        m_timeInCurrentHost = 0.f;
    }
}

void Level::HandleItems(const GameContext& ctx)
{
    for (auto& syringe : m_syringes)
    {
        if (syringe->IsCollected())
            continue;

        if (m_playerAvatar->GetBounds().intersects(syringe->GetBounds()))
        {
            if (m_currentHost != nullptr && m_playerAvatar == m_currentHost)
            {
                syringe->Collect();
                m_stats.syringesCollected++;
                m_currentHost->AddDecayTime(Config::SYRINGE_DECAY_RESTORE);
                ctx.audio->PlaySound(SOUND_ID_COLLECT);
            }
        }
    }
}

void Level::CleanDeadEnemies()
{
    m_hosts.erase(std::remove_if(m_hosts.begin(), m_hosts.end(),
                                 [this](const std::unique_ptr<Host>& enemy)
                                 {
                                     return enemy->IsDead() &&
                                            enemy->GetCorpseTime() >= Config::CORPSE_LINGER_TIME &&
                                            enemy.get() != m_currentHost;
                                 }),
                  m_hosts.end());
}

void Level::CheckEndConditions()
{
    if (m_result != LevelResult::None)
        return;

    bool isParasiteDead = (m_parasite.GetLife() <= 0.f);
    bool fellIntoPit = (m_playerAvatar->GetPosition().y > m_map.GetHeightInPixels());

    if (isParasiteDead || fellIntoPit)
    {
        m_audio->PlaySound(SOUND_ID_LOSE);
        m_audio->PlaySound(SOUND_ID_DEATH);
        m_result = LevelResult::Failed;
        return;
    }

    if (m_map.IsIntersectingWith(m_playerAvatar->GetBounds(), BlockNames::Elevator))
    {
        int access = m_playerAvatar->GetProperties().GetInt("access_level", 0);
        if (access >= 1)
        {
            m_audio->PlaySound(SOUND_ID_WIN);
            m_result = LevelResult::Completed;
        }
    }
}

bool Level::IsAvatarInElevator() const
{
    return m_map.IsIntersectingWith(m_playerAvatar->GetBounds(), BlockNames::Elevator);
}

void Level::Draw(sf::RenderWindow& window)
{
    m_map.Draw(window);

    for (auto& enemy : m_hosts)
    {
        if (enemy->IsDead())
        {
            enemy->Draw(window);
        }
    }

    for (auto& enemy : m_hosts)
    {
        if (!enemy->IsDead())
        {
            enemy->Draw(window);
        }
    }

    for (auto& syringe : m_syringes)
    {
        syringe->Draw(window);
    }

    if (m_parasite.IsPlayerControlled())
    {
        m_parasite.Draw(window);
    }
    
    m_bullets.Draw(window);
}