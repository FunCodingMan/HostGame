#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <memory>

class SoundManager 
{
public:
    SoundManager() = default;
    ~SoundManager() = default;
;

    bool LoadSound(const std::string& id, const std::string& filepath, float volume = 100.f);
    void PlaySound(const std::string& id);
    void SetSoundVolume(const std::string& id, float volume);

    bool LoadMusic(const std::string& id, const std::string& filepath, float volume = 100.f);
    void PlayMusic(const std::string& id, bool loop = true);
    void StopMusic(const std::string& id);
    void SetMusicVolume(const std::string& id, float volume);

private:
    std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>> m_soundBuffers;
    std::unordered_map<std::string, sf::Sound> m_sounds;

    std::unordered_map<std::string, std::unique_ptr<sf::Music>> m_musics;
};