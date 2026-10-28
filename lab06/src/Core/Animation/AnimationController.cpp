#include "AnimationController.hpp"

void AnimationController::AddClip(const std::string& name, const std::vector<sf::IntRect>& frames, float frameTime, bool loop)
{
    m_clips[name] = Clip{ frames, frameTime, loop };
}

void AnimationController::Play(const std::string& name)
{
    if (m_current == name || !m_clips.count(name))
    {
        return;
    }

    m_current = name;
    m_frame   = 0;
    m_elapsed = 0.f;
}

void AnimationController::Update(float dt)
{
    auto it = m_clips.find(m_current);

    if (it == m_clips.end() || it->second.frames.empty())
    {
        return;
    }

    const Clip& c = it->second;
    m_elapsed += dt;
    while (m_elapsed >= c.frameTime)
    {
        m_elapsed -= c.frameTime;
        m_frame++;
        if (m_frame >= (int)c.frames.size())
            m_frame = c.loop ? 0 : (int)c.frames.size() - 1;
    }
}

void AnimationController::Apply(sf::Sprite& sprite) const
{
    auto it = m_clips.find(m_current);
    if (it == m_clips.end() || it->second.frames.empty()) return;
    sprite.setTextureRect(it->second.frames[m_frame]);
}