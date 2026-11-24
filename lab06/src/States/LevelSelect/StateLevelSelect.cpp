#include "States/LevelSelect/StateLevelSelect.hpp"
#include "Core/GameKeyboard/GameKeyboard.hpp"
#include "Core/Utils/StringUtils/StringUtils.hpp"
#include "States/MainMenu/StateMainMenu.hpp"
#include "States/Playing/StatePlaying.hpp"
#include <algorithm>

using namespace StringUtils;

StateLevelSelect::StateLevelSelect(GameDataRef data) : m_data(data) {}

void StateLevelSelect::Init()
{
    auto windowSize = m_data->window.getSize();
    m_title.Init(L"ВЫБОР УРОВНЯ", m_data->headerFont, 80, 150.f, windowSize.x);

    const float spacing = 70.f;
    const int levelCount = static_cast<int>(m_data->levels.size());
    const float startY = std::max(windowSize.y * 0.4f, 300.f);

    for (int i = 0; i < levelCount; ++i)
    {
        sf::String levelName = ToSfStringFromUtf8(m_data->levels[i]);

        m_menu.AddButton(levelName, m_data->headerFont, startY + i * spacing, windowSize.x,
                         [this, i]()
                         {
                             m_data->currentLevelIndex = i;
                             m_data->manager.AddState(StateRef(new StatePlaying(m_data)), true);
                         });
    }

    m_menu.AddButton(L"Назад", m_data->headerFont, startY + levelCount * spacing, windowSize.x,
                     [this]()
                     {
                         m_data->manager.AddState(StateRef(new StateMainMenu(m_data)), true);
                     });
}

void StateLevelSelect::HandleInput(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (GameKeyboard::IsEventEscape(event))
        {
            m_data->manager.AddState(StateRef(new StateMainMenu(m_data)), true);
            return;
        }

        m_menu.HandleInput(event, m_data->sound);
    }
}

void StateLevelSelect::Update(float dt)
{
    m_menu.Update();
}

void StateLevelSelect::Draw()
{
    m_title.Draw(m_data->window);
    m_menu.Draw(m_data->window);
}
