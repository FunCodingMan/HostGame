#pragma once
#include "Entities/Host/Host.hpp"

class Scientist : public Host
{
public:
    Scientist(sf::Vector2f position);

    void Update(const GameContext& ctx) override;

protected:
    Config::EntityType GetEntityType() const override { return Config::EntityType::Human; }
    const char*        GetAnimationName() const override;
};