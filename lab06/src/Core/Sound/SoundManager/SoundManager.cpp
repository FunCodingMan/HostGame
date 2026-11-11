#include "Core/Sound/SoundManager/SoundManager.hpp"
#include <iostream>


bool SoundManager::LoadSound(const std::string& id, const std::string& filepath, float volume)
{
    auto buffer = std::make_unique<sf::SoundBuffer>();
    if (!buffer->loadFromFile(filepath))
    {
        std::cerr << "Audio Error: Could not load sound '" << filepath << "'\n";
        return false;
    }

    sf::Sound sound;
    sound.setBuffer(*buffer);
    sound.setVolume(volume);

    m_soundBuffers[id] = std::move(buffer);
    m_sounds[id] = sound;
    
    return true;
}

void SoundManager::PlaySound(const std::string& id)
{
    auto it = m_sounds.find(id);
    if (it != m_sounds.end())
    {
        it->second.play();
    }
    else
    {
        std::cerr << "Audio Warning: Try to play unknown sound '" << id << "'\n";
    }
}

void SoundManager::SetSoundVolume(const std::string& id, float volume)
{
    auto it = m_sounds.find(id);
    if (it != m_sounds.end())
    {
        it->second.setVolume(volume);
    }
}

bool SoundManager::LoadMusic(const std::string& id, const std::string& filepath, float volume)
{
    auto music = std::make_unique<sf::Music>();
    if (!music->openFromFile(filepath))
    {
        std::cerr << "Audio Error: Could not load music '" << filepath << "'\n";
        return false;
    }

    music->setVolume(volume);
    m_musics[id] = std::move(music);

    return true;
}

void SoundManager::PlayMusic(const std::string& id, bool loop)
{
    auto it = m_musics.find(id);
    if (it != m_musics.end())
    {
        it->second->setLoop(loop);
        
        if (it->second->getStatus() != sf::Music::Playing) 
        {
            it->second->play();
        }
    }
}

void SoundManager::StopMusic(const std::string& id)
{
    auto it = m_musics.find(id);
    if (it != m_musics.end())
    {
        it->second->stop();
    }
}

void SoundManager::SetMusicVolume(const std::string& id, float volume)
{
    auto it = m_musics.find(id);
    if (it != m_musics.end())
    {
        it->second->setVolume(volume);
    }
}