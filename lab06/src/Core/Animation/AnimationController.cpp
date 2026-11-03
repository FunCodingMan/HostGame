#include "Core/Animation/AnimationController.hpp"
#include <algorithm>

void AnimationController::Setup(const sf::Texture& tex, int frameW, int frameH)
{
    m_texture = &tex;
    m_frameW  = frameW;
    m_frameH  = frameH;
    m_nextRow = 0;
}

void AnimationController::AddClip(const std::string& name, int frameCount, float frameTime)
{
    if (m_texture == nullptr || frameCount <= 0) return;

    int rows = (int)(m_texture->getSize().y / m_frameH);
    if (m_nextRow >= rows) return;

    int row = m_nextRow++;
    AddClipRow(name, row, 0, frameCount, frameTime);
}

void AnimationController::AddClip(const std::string& name, int frameCount, float frameTime,
                                  int row, int startCol)
{
    if (m_texture == nullptr || frameCount <= 0 || row < 0 || startCol < 0) return;

    int rows = (int)(m_texture->getSize().y / m_frameH);
    if (row >= rows) return;

    AddClipRow(name, row, startCol, frameCount, frameTime);

    if (row + 1 > m_nextRow) m_nextRow = row + 1;
}

void AnimationController::AddClip(const std::string& name,
                                  const std::vector<sf::IntRect>& frames,
                                  float frameTime, bool loop)
{
    m_clips[name] = Clip{ frames, frameTime, loop };
}

void AnimationController::AddClipRow(const std::string& name, int row, int startCol,
                                     int count, float frameTime)
{
    int cols = (int)(m_texture->getSize().x / m_frameW);
    int realCount = std::min(count, cols - startCol);
    if (realCount <= 0) return;

    std::vector<sf::IntRect> frames;
    frames.reserve(realCount);
    for (int i = 0; i < realCount; ++i)
    {
        frames.emplace_back((startCol + i) * m_frameW, row * m_frameH, m_frameW, m_frameH);
    }

    bool  loop = (realCount > 1);
    float ft   = (realCount > 1) ? frameTime : 1.f;

    m_clips[name] = Clip{ frames, ft, loop };
}

void AnimationController::Play(const std::string& name)
{
    if (m_current == name) return;
    if (!m_clips.count(name)) return;
    m_current = name;
    m_frame   = 0;
    m_elapsed = 0.f;
}

void AnimationController::Update(float dt)
{
    auto it = m_clips.find(m_current);
    if (it == m_clips.end() || it->second.frames.empty()) return;

    const Clip& c = it->second;
    m_elapsed += dt;
    while (m_elapsed >= c.frameTime)
    {
        m_elapsed -= c.frameTime;
        if (++m_frame >= (int)c.frames.size())
            m_frame = c.loop ? 0 : (int)c.frames.size() - 1;
    }
}

void AnimationController::Apply(sf::Sprite& sprite) const
{
    auto it = m_clips.find(m_current);
    if (it == m_clips.end() || it->second.frames.empty()) return;
    sprite.setTextureRect(it->second.frames[m_frame]);
}