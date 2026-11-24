#pragma once
#include "Entities/Host/Host.hpp"

class Scientist : public Host
{
public:
    Scientist(sf::Vector2f position);

protected:
    Config::EntityType GetEntityType() const override { return Config::EntityType::Human; }
    const char* GetDamageSound() const override { return Config::SOUND_ID_SCIENTIST_DAMAGE; }
};