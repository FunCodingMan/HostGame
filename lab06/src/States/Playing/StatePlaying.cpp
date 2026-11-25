#include "StatePlaying.hpp"
#include "Core/Config.hpp"
#include "Core/Debug/DebugOverlay/DebugOverlay.hpp"
#include "Core/GameKeyboard/GameKeyboard.hpp"
#include "Core/Utils/StringUtils/StringUtils.hpp"
#include "Entities/Military/Military.hpp"
#include "States/GameOver/StateGameOver.hpp"
#include "States/Pause/StatePause.hpp"
#include "States/Victory/StateVictory.hpp"
#include "States/LevelCompleted/StateLevelCompleted.hpp"
#include <string>

using namespace StringUtils;

StatePlaying::StatePlaying(GameDataRef data) : m_data(data) {}

void StatePlaying::Init()
{
    m_endStatePushed = false;
    m_data->stats.Reset();

    GameKeyboard::EnableAll();

    std::string levelName = m_data->GetCurrentLevelName();
    m_level.Load(levelName, m_data->sound);

    m_floatingHud.Init();
    m_screenHud.Init(m_data->headerFont, m_data->window.getSize().x);
    DebugOverlay::Init(m_data->headerFont, m_data->window.getSize());

    m_data->sound.PlayMusic(Config::MUSIC_ID_BGM, true);

    if (levelName == "tutorial")
    {
        m_tutorial.Init(m_data->headerFont, m_data->window.getSize().x);
        m_screenHud.SetHintText(ToSfStringFromUtf8("Обучение"));
    }
    else
    {
        m_screenHud.SetHintText(ToSfStringFromUtf8("Уровень: ") + ToSfStringFromUtf8(levelName) +
                                ToSfStringFromUtf8("\nЛКМ: Рывок | ПКМ: Выход"));
    }
}

void StatePlaying::HandleInput(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (GameKeyboard::IsHitboxesPressed(event))
            Entity::ToggleHitboxes();
        if (GameKeyboard::IsCoordsTogglePressed(event))
            DebugOverlay::ToggleMouseCoords();
        if (GameKeyboard::IsEventEscape(event))
            m_data->manager.AddState(StateRef(new StatePause(m_data)), false);
    }
}

void StatePlaying::Update(float dt)
{
    LevelInput input;
    input.mousePixelPos = sf::Mouse::getPosition(m_data->window);
    input.windowSize = m_data->window.getSize();

    m_level.Update(dt, input);

    if (m_data->GetCurrentLevelName() == "tutorial")
    {
        m_tutorial.Update(dt, m_level.GetParasite(), m_level.GetCurrentHost(),
                          m_level.GetStats().syringesCollected);
    }

    UpdateHUD();
    HandleLevelResult();
}

void StatePlaying::UpdateHUD()
{
    LivingEntity* avatar = m_level.GetAvatar();
    m_floatingHud.SetPosition(avatar->GetPosition());

    sf::String avatarName =
        avatar->GetProperties().GetSfString("name", ToSfStringFromUtf8("Неизвестный"));
    bool isInElevator = m_level.IsAvatarInElevator();
    Host* host = m_level.GetCurrentHost();

    if (host != nullptr && avatar == host)
    {
        Military* military = dynamic_cast<Military*>(host);

        if (military != nullptr)
            m_floatingHud.UpdateAmmo(military->GetAmmo(), Config::MILITARY_AMMO);
        else
            m_floatingHud.HideAmmo();

        if (!isInElevator)
        {
            sf::String hint = ToSfStringFromUtf8("Носитель: ") + avatarName;

            if (military != nullptr)
            {
                if (military->IsReloading())
                {
                    hint += ToSfStringFromUtf8("\nПерезарядка...");
                }

                hint += ToSfStringFromUtf8("\nЛКМ: Огонь | ПКМ: Высвободиться");
            }
            else
            {
                hint += ToSfStringFromUtf8("\nПКМ: Высвободиться");
            }

            m_screenHud.SetHintText(hint);
        }

        m_floatingHud.Update(host->GetDecayTimer(), host->GetMaxDecayTimer(),
                             sf::Color(220, 200, 0));
    }
    else
    {
        if (!isInElevator)
            m_screenHud.SetHintText(ToSfStringFromUtf8("Форма: ") + avatarName +
                                    ToSfStringFromUtf8("\nЛКМ: Вселиться"));
        m_floatingHud.HideAmmo();
        const Parasite& p = m_level.GetParasite();
        m_floatingHud.Update(p.GetLife(), p.GetMaxLife(), sf::Color(200, 0, 0));
    }

    if (isInElevator && avatar->GetProperties().GetInt("access_level", 0) < 1)
    {
        m_screenHud.SetHintText(
            ToSfStringFromUtf8("ДОСТУП ЗАПРЕЩЁН!\nТребуется тело учёного!"));
    }

    m_screenHud.SetTimerText(m_level.GetStats().timeSpent, m_data->window.getSize().x);
}

void StatePlaying::HandleLevelResult()
{
    if (m_endStatePushed)
        return;

    switch (m_level.GetResult())
    {
    case LevelResult::Failed:
        m_endStatePushed = true;
        m_data->manager.AddState(StateRef(new StateGameOver(m_data)), false);
        break;

    case LevelResult::Completed:
        m_endStatePushed = true;
        m_data->stats = m_level.GetStats();   // фикс: статистика теперь и для Victory
        if (m_data->currentLevelIndex + 1 < m_data->levels.size())
            m_data->manager.AddState(StateRef(new StateLevelCompleted(m_data)), true);
        else
            m_data->manager.AddState(StateRef(new StateVictory(m_data)), false);
        break;

    default:
        break;
    }
}

void StatePlaying::Draw()
{
    m_data->window.setView(m_level.GetView());
    m_level.Draw(m_data->window);
    m_floatingHud.Draw(m_data->window);

    m_data->window.setView(m_data->window.getDefaultView());
    m_screenHud.Draw(m_data->window);
    DebugOverlay::Draw(m_data->window, m_level.GetView());

    if (m_data->GetCurrentLevelName() == "tutorial")
        m_tutorial.Draw(m_data->window);
}