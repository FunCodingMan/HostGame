#pragma once
#include <SFML/Graphics.hpp>

class GameKeyboard
{
public:
    static bool IsEventUp(const sf::Event& event);
    static bool IsEventDown(const sf::Event& event);
    static bool IsEventLeft(const sf::Event& event);
    static bool IsEventRight(const sf::Event& event);
    static bool IsEventOpen(const sf::Event& event);
    static bool IsEventEscape(const sf::Event& event);
    static bool IsJumpKeyPressed();
    static bool IsLeftKeyPressed();
    static bool IsRightKeyPressed();
    static bool IsDashKeyPressed();
    static bool IsHitboxesPressed(const sf::Event& event);
private:

};