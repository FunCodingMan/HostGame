#include "Core/Map/Door.hpp"
#include "Core/Config.hpp"

Door::Door(int tileX, int tileY, const sf::Texture& texture)
    : m_tileX(tileX), m_tileY(tileY)
{
    const float scale = static_cast<float>(Config::WORLD_TILE_SIZE) / Config::TEXTURE_TILE_SIZE;

    m_sprite.setTexture(texture);
    m_sprite.setScale(scale, scale);
    m_sprite.setOrigin(Config::TEXTURE_TILE_SIZE / 2.f, static_cast<float>(Config::TEXTURE_TILE_SIZE));
    m_sprite.setPosition(tileX * Config::WORLD_TILE_SIZE + Config::WORLD_TILE_SIZE / 2.f,
                         (tileY + 1) * Config::WORLD_TILE_SIZE);

    m_anim.Setup(texture, Config::TEXTURE_TILE_SIZE, Config::TEXTURE_TILE_SIZE);
    m_anim.AddClip("opening", Config::DOOR_OPEN_FRAME_COUNT,
                   Config::DOOR_OPEN_FRAME_TIME, 0, 0, false);
    m_anim.Play("opening");
    m_anim.Apply(m_sprite);
}

bool Door::Open()
{
    if (m_state != State::Closed)
    {
        return false;
    }

    m_state = State::Opening;
    m_anim.Play("opening", true);
    m_anim.Apply(m_sprite);
    return true;
}

void Door::Update(float dt)
{
    if (m_state != State::Opening)
    {
        return;
    }

    m_anim.Update(dt);
    m_anim.Apply(m_sprite);

    if (m_anim.IsFinished())
    {
        m_state = State::Open;
    }
}

void Door::Draw(sf::RenderWindow& window) const
{
    if (m_state == State::Open)
    {
        return;
    }

    window.draw(m_sprite);
}
