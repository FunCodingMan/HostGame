#include "States/LevelCompleted/StateLevelCompleted.hpp"
#include "States/MainMenu/StateMainMenu.hpp"
#include "States/Playing/StatePlaying.hpp"
#include "Core/Utils/StringUtils/StringUtils.hpp"
#include <sstream>
#include <iomanip>

using namespace StringUtils;

StateLevelCompleted::StateLevelCompleted(GameDataRef data) : m_data(data) {}

void StateLevelCompleted::Init()
{
    auto windowSize = m_data->window.getSize();

    m_backgroundTexture.create(windowSize.x, windowSize.y);
    m_backgroundTexture.update(m_data->window);
    m_backgroundSprite.setTexture(m_backgroundTexture);

    m_darkOverlay.setSize(sf::Vector2f(windowSize.x, windowSize.y));
    m_darkOverlay.setFillColor(sf::Color(0, 50, 0, 180)); 

    m_title.Init(ToSfStringFromUtf8("УРОВЕНЬ ПРОЙДЕН!"), m_data->headerFont, 70, 150.f, windowSize.x);
    m_data->sound.StopMusic(Config::MUSIC_ID_BGM);

    m_statsText.setFont(m_data->headerFont);
    m_statsText.setCharacterSize(22);
    m_statsText.setFillColor(sf::Color::White);
    m_statsText.setLineSpacing(1.5f);

    std::stringstream ss;
    ss << "ВРЕМЯ НА УРОВНЕ: " << std::fixed << std::setprecision(1) << m_data->stats.timeSpent << " сек\n"
       << "РЫВКОВ СДЕЛАНО: " << m_data->stats.dashesUsed << "\n"
       << "РАЗБИТО СТЁКОЛ: " << m_data->stats.tilesBroken << "\n"
       << "ЗАХВАЧЕНО ТЕЛ: " << m_data->stats.hostsPossessed << "\n"
       << "СОБРАНО ШПРИЦЕВ: " << m_data->stats.syringesCollected;

    m_statsText.setString(ToSfStringFromUtf8(ss.str()));

    sf::FloatRect textBounds = m_statsText.getLocalBounds();
    m_statsText.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top);
    m_statsText.setPosition(windowSize.x / 2.f, 300.f);


    m_menu.AddButton(ToSfStringFromUtf8("Следующий уровень"), m_data->headerFont, 480.f, windowSize.x,
                     [this]()
                     {
                         m_data->currentLevelIndex++; 
                         m_data->manager.RemoveState();
                         m_data->manager.AddState(StateRef(new StatePlaying(m_data)), true);
                     });

    m_menu.AddButton(ToSfStringFromUtf8("Рестарт"), m_data->headerFont, 560.f, windowSize.x,
                     [this]()
                     {
                         m_data->manager.RemoveState();
                         m_data->manager.AddState(StateRef(new StatePlaying(m_data)), true);
                     });

    m_menu.AddButton(ToSfStringFromUtf8("В главное меню"), m_data->headerFont, 640.f, windowSize.x,
                     [this]()
                     {
                         m_data->manager.RemoveState();
                         m_data->manager.AddState(StateRef(new StateMainMenu(m_data)), true);
                     });
}
void StateLevelCompleted::HandleInput(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        m_menu.HandleInput(event, m_data->sound);
    }
}

void StateLevelCompleted::Update(float dt)
{
    m_menu.Update();
}

void StateLevelCompleted::Draw()
{
    m_data->window.setView(m_data->window.getDefaultView());
    
    m_data->window.draw(m_backgroundSprite);
    m_data->window.draw(m_darkOverlay);

    m_title.Draw(m_data->window);
    m_data->window.draw(m_statsText);
    m_menu.Draw(m_data->window);
}