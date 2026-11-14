#include "MusicPlayer.hpp"

MusicPlayer::~MusicPlayer() {
    stop();
    for (Sound& s : active_sfx_) {
        StopSound(s);
        UnloadSound(s);
    }
    active_sfx_.clear();
}

void MusicPlayer::start_song(const std::string& path) {
    if (path.empty() || (playing_ && path == current_path_)) {
        return;
    }

    stop();

    music_ = LoadMusicStream(path.c_str());
    if (music_.frameCount > 0) {
        music_.looping = true;
        PlayMusicStream(music_);
        playing_ = true;
        current_path_ = path;
    }
}

void MusicPlayer::play_sfx(const std::string& path) {
    if (path.empty()) {
        return;
    }
    Sound s = LoadSound(path.c_str());
    if (s.frameCount > 0) {
        PlaySound(s);
        active_sfx_.push_back(s);
    }
}

void MusicPlayer::stop() {
    if (playing_) {
        StopMusicStream(music_);
        UnloadMusicStream(music_);
        music_ = Music{};
        playing_ = false;
        current_path_.clear();
    }
}

void MusicPlayer::update() {
    if (playing_) {
        UpdateMusicStream(music_);
    }
    for (auto it = active_sfx_.begin(); it != active_sfx_.end();) {
        if (!IsSoundPlaying(*it)) {
            UnloadSound(*it);
            it = active_sfx_.erase(it);
        } else {
            ++it;
        }
    }
}

void MusicPlayer::set_pitch(float pitch) {
    if (playing_) {
        SetMusicPitch(music_, pitch);
    }
}

void MusicPlayer::set_volume(float volume) {
    if (playing_) {
        SetMusicVolume(music_, volume);
    }
}
