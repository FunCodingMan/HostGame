#pragma once
#include <SFML/Graphics.hpp>

class State
{
public:
    virtual ~State() = default;

    virtual void Init() = 0;

    virtual void HandleInput(const sf::Event& event) = 0;

    virtual void Update(float dt) = 0;
    
    virtual void Draw() = 0;

    virtual void Pause() {}

    virtual void Resume() {}
};