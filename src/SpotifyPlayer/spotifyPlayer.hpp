#ifndef SPOTIFY_PLAYER_HPP
#define SPOTIFY_PLAYER_HPP

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <ctime>
#include <memory>
#include <mutex>
#include <vector>
#include "../LinkedList/doublyLinkedList.hpp"

class SpotifyPlayer {
private:
    std::atomic<bool> is_playing{false};
    std::atomic<bool> is_paused{false};
    std::mutex mutex;
    std::condition_variable cv;

    std::vector<std::shared_ptr<Song>> current_playlist;
    size_t current_song_index{0};  // Initialize to 0

    pid_t librespot_pid = -1;

public:
    SpotifyPlayer() = default;

    void loadPlaylist(const std::vector<std::shared_ptr<Song>>& playlist);
    void play();
    void pause();
    void resume();
    void nextTrack();
    void previousTrack();
    void stopPlayback();
    bool isPlaying() const;
    bool isPaused() const;
    ~SpotifyPlayer();
};

#endif // !SPOTIFY_PLAYER_HPP
