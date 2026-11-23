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
    static bool IsHitboxesPressed(const sf::Event& event);
    static bool IsCoordsTogglePressed(const sf::Event& event);

    static bool IsJumpKeyPressed();
    static bool IsLeftKeyPressed();
    static bool IsRightKeyPressed();
    static bool IsDashKeyPressed();
    static bool IsEjectKeyPressed();
    static bool IsShootKeyPressed();

    static void SetMovementEnabled(bool state) { s_movementEnabled = state; }
    static void SetJumpEnabled(bool state)     { s_jumpEnabled = state; }
    static void SetDashEnabled(bool state)     { s_dashEnabled = state; }
    static void SetEjectEnabled(bool state)    { s_ejectEnabled = state; }
    static void EnableAll() {
        s_movementEnabled = true;
        s_jumpEnabled = true;
        s_dashEnabled = true;
        s_ejectEnabled = true;
    }
private:
    static inline bool s_movementEnabled = true;
    static inline bool s_jumpEnabled = true;
    static inline bool s_dashEnabled = true;
    static inline bool s_ejectEnabled = true;
};