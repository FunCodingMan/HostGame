#include <SFML/Graphics.hpp>
#include "GameKeyboard.hpp"

bool GameKeyboard::IsJumpKeyPressed()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
           sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
           sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
}
bool GameKeyboard::IsLeftKeyPressed()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
           sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
}
bool GameKeyboard::IsRightKeyPressed()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
           sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
}

bool GameKeyboard::IsDashKeyPressed()
{
    return sf::Mouse::isButtonPressed(sf::Mouse::Left) ||
           sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
}

bool GameKeyboard::IsHitboxesPressed(const sf::Event& event)
{
    return event.key.code == sf::Keyboard::B && sf::Keyboard::isKeyPressed(sf::Keyboard::F3);
}

bool GameKeyboard::IsEventUp(const sf::Event& event)
{
    return event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W;
}

bool GameKeyboard::IsEventDown(const sf::Event& event)
{
    return event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S;
}

bool GameKeyboard::IsEventLeft(const sf::Event& event)
{
    return event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A;
}

bool GameKeyboard::IsEventRight(const sf::Event& event)
{
    return event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D;
}

bool GameKeyboard::IsEventOpen(const sf::Event& event)
{
    return event.key.code == sf::Keyboard::Enter;
}

bool GameKeyboard::IsEventEscape(const sf::Event& event)
{
    return event.key.code == sf::Keyboard::Escape;
}