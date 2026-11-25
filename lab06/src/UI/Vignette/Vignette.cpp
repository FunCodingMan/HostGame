#include "UI/Vignette/Vignette.hpp"
#include "Core/Config.hpp"
#include <algorithm>
#include <cmath>

namespace
{
    constexpr unsigned TEXTURE_SIZE = 256;
    constexpr float INNER_RADIUS = 0.55f;
    constexpr float OUTER_RADIUS = 1.0f;
}

void Vignette::Init(sf::Vector2u windowSize)
{
    sf::Image image;
    image.create(TEXTURE_SIZE, TEXTURE_SIZE, sf::Color::Transparent);

    const float center = TEXTURE_SIZE / 2.f;

    for (unsigned y = 0; y < TEXTURE_SIZE; ++y)
    {
        for (unsigned x = 0; x < TEXTURE_SIZE; ++x)
        {
            float dx = (x + 0.5f - center) / center;
            float dy = (y + 0.5f - center) / center;
            float distance = std::sqrt(dx * dx + dy * dy);

            float t = std::clamp((distance - INNER_RADIUS) / (OUTER_RADIUS - INNER_RADIUS),
                                 0.f, 1.f);
            t = t * t * (3.f - 2.f * t);

            auto alpha = static_cast<sf::Uint8>(t * 255.f);
            image.setPixel(x, y, sf::Color(255, 255, 255, alpha));
        }
    }

    m_texture.loadFromImage(image);
    m_texture.setSmooth(true);

    m_sprite.setTexture(m_texture, true);
    m_sprite.setPosition(0.f, 0.f);
    m_sprite.setScale(windowSize.x / static_cast<float>(TEXTURE_SIZE),
                      windowSize.y / static_cast<float>(TEXTURE_SIZE));
    m_sprite.setColor(sf::Color(Config::VIGNETTE_COLOR.r,
                                Config::VIGNETTE_COLOR.g,
                                Config::VIGNETTE_COLOR.b,
                                0));
}

void Vignette::Update(float dt, bool active)
{
    if (active)
    {
        m_time += dt * Config::HOST_DANGER_PULSE_SPEED;
    }
    else
    {
        m_time = 0.f;
    }

    float pulse = active ? 0.5f + 0.5f * std::sin(m_time) : 0.f;
    float target = active ? Config::VIGNETTE_BASE_ALPHA +
                            Config::VIGNETTE_PULSE_ALPHA * pulse
                          : 0.f;

    m_alpha += (target - m_alpha) * std::min(1.f, dt * 6.f);

    sf::Color color = Config::VIGNETTE_COLOR;
    color.a = static_cast<sf::Uint8>(std::clamp(m_alpha, 0.f, 255.f));
    m_sprite.setColor(color);
}

void Vignette::Draw(sf::RenderWindow& window)
{
    if (m_alpha > 0.f)
    {
        window.draw(m_sprite);
    }
}
