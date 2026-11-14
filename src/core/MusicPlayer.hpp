#pragma once

#include <raylib.h>
#include <string>

class MusicPlayer {
public:
    MusicPlayer() = default;
    ~MusicPlayer();

    MusicPlayer(const MusicPlayer&) = delete;
    MusicPlayer& operator=(const MusicPlayer&) = delete;

    void start_song(const std::string& path);
    void stop();
    void update();
    void set_pitch(float pitch);
    void set_volume(float volume);

private:
    Music music_{};
    bool playing_ = false;
    std::string current_path_;
};
