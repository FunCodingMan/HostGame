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

    m_soundBuffers[id] = std::move(buffer);
    m_sounds[id].clear();
    m_soundVolumes[id] = volume;

    return true;
}

void SoundManager::PlaySound(const std::string& id, bool loop)
{
    auto bufferIt = m_soundBuffers.find(id);
    if (bufferIt == m_soundBuffers.end())
    {
        std::cerr << "Audio Warning: Try to play unknown sound '" << id << "'\n";
        return;
    }

    auto& sounds = m_sounds[id];
    float volume = m_soundVolumes[id];
    sf::Sound* playSound = nullptr;

    if (loop)
    {
        if (!sounds.empty() && sounds.front().getStatus() == sf::Sound::Playing)
        {
            return;
        }

        if (sounds.empty())
        {
            sounds.emplace_back(*bufferIt->second);
            sounds.back().setVolume(volume);
        }

        playSound = &sounds.front();
    }
    else
    {
        for (auto& sound : sounds)
        {
            if (sound.getStatus() != sf::Sound::Playing)
            {
                playSound = &sound;
                break;
            }
        }

        if (playSound == nullptr)
        {
            sounds.emplace_back(*bufferIt->second);
            sounds.back().setVolume(volume);
            playSound = &sounds.back();
        }
    }

    playSound->setLoop(loop);
    playSound->play();
}

void SoundManager::StopSound(const std::string& id)
{
    auto it = m_sounds.find(id);
    if (it != m_sounds.end())
    {
        for (auto& sound : it->second)
        {
            sound.stop();
        }
    }
}

bool SoundManager::HasSound(const std::string& id) const
{
    return m_soundBuffers.find(id) != m_soundBuffers.end();
}

void SoundManager::SetSoundVolume(const std::string& id, float volume)
{
    m_soundVolumes[id] = volume;

    auto it = m_sounds.find(id);
    if (it != m_sounds.end())
    {
        for (auto& sound : it->second)
        {
            sound.setVolume(volume);
        }
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