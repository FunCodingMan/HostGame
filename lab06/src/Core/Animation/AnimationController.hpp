#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include "Core/Config.hpp"

class AnimationController
{
public:
    struct Clip
    {
        std::vector<sf::IntRect> frames;
        float frameTime = Config::TIME_FOR_ONE_FRAME;
        bool loop = true;
    };

    void AddClip(const std::string&, const std::vector<sf::IntRect>& frames, float frameTime, bool loop = true);
    void Play(const std::string& name);
    void Update(float dt);
    void Apply(sf::Sprite& sprite) const; 
private:
    std::unordered_map<std::string, Clip> m_clips;
    std::string m_current;
    int   m_frame   = 0;
    float m_elapsed = 0.f;
};