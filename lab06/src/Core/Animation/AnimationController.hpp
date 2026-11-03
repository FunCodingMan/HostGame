#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <string>

class AnimationController
{
public:
    struct Clip
    {
        std::vector<sf::IntRect> frames;
        float frameTime = 0.1f;
        bool  loop      = true;
    };

    void Setup(const sf::Texture& tex, int frameW, int frameH);

    void AddClip(const std::string& name, int frameCount, float frameTime);
    void AddClip(const std::string& name, int frameCount, float frameTime,
                 int row, int startCol = 0);

    void AddClip(const std::string& name,
                 const std::vector<sf::IntRect>& frames,
                 float frameTime, bool loop = true);

    void Play(const std::string& name);
    void Update(float dt);
    void Apply(sf::Sprite& sprite) const;

    const std::string& Current() const { return m_current; }

private:
    void AddClipRow(const std::string& name, int row, int startCol,
                    int count, float frameTime);

    const sf::Texture* m_texture = nullptr;
    int m_frameW  = 0;
    int m_frameH  = 0;
    int m_nextRow = 0;

    std::unordered_map<std::string, Clip> m_clips;
    std::string m_current;
    int   m_frame   = 0;
    float m_elapsed = 0.f;
};