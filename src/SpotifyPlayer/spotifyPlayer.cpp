#include "spotifyPlayer.hpp"
#include "../LinkedList/doublyLinkedList.hpp"
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include "../../DotEnv.cpp"

void SpotifyPlayer::loadPlaylist(const std::vector<std::shared_ptr<Song>>& playlist){
    current_playlist = playlist;
    current_song_index = 0;
}

void SpotifyPlayer::play(){
    stopPlayback();

    // Make sure we have a playlist with songs
    if(current_playlist.empty()){
        std::cerr << "No playlist loaded" << std::endl;
        return;
    }

    // Load environment variables from .env file
    DotEnv env;
    
    // Retrieve Spotify username and password from .env
    std::string spotify_username = env.get("SPOTIFY_USERNAME");
    std::string spotify_password = env.get("SPOTIFY_PASSWORD");

    // Check if credentials are available
    if (spotify_username.empty() || spotify_password.empty()) {
        std::cerr << "Spotify credentials not found in .env file" << std::endl;
        return;
    }

    // Gets the current song url
    std::string spotify_url = current_playlist[current_song_index]->link_;

    // Construct Librespot command
    std::string command = "librespot -u \"" + spotify_username + "\" -p \"" + spotify_password + "\" "
                          "-n TerminalPlayer "
                          "--initial-volume 50 "
                          "--spotify-uri \"" + spotify_url + "\" "
                          "> /dev/null 2>&1";  // Redirect output to /dev/null

    librespot_pid = fork();

    if (librespot_pid == 0) { 
        // Child process
        // Close standard output and error to prevent screen clutter
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        // Execute Librespot
        execlp("bash", "bash", "-c", command.c_str(), NULL);
        exit(1);  // If execlp fails
    } else if (librespot_pid > 0) {
        is_playing = true;
        is_paused = false;
        
        std::cout << "Now playing: " 
                  << current_playlist[current_song_index]->title_ 
                  << " by " 
                  << current_playlist[current_song_index]->artist_ 
                  << std::endl;
    } else {
        std::cerr << "Failed to start playback" << std::endl;
    }
}

// Rest of the implementation remains the same...

void SpotifyPlayer::pause(){
    if (is_playing && !is_paused) {
        // Send SIGSTOP to pause
        if (kill(librespot_pid, SIGSTOP) == 0) {
            is_paused = true;
            std::cout << "Playback paused" << std::endl;
        } else {
            std::cerr << "Failed to pause playback" << std::endl;
        }
    }
}

void SpotifyPlayer::resume() {
    if (is_playing && is_paused) {
        // Send SIGCONT to resume
        if (kill(librespot_pid, SIGCONT) == 0) {
            is_paused = false;
            std::cout << "Playback resumed" << std::endl;
        } else {
            std::cerr << "Failed to resume playback" << std::endl;
        }
    }
}
void SpotifyPlayer::stopPlayback(){
    if(librespot_pid > 0){

        //Kill the librespot process
        kill(librespot_pid, SIGTERM);

        int status;
        waitpid(librespot_pid, &status, 0);

        librespot_pid = -1;
        is_playing = false;
        is_paused = false;
    }
}

void SpotifyPlayer::nextTrack(){
    stopPlayback();

    current_song_index = (current_song_index + 1)%current_playlist.size();
    play();
}

void SpotifyPlayer::previousTrack(){
    stopPlayback();

    if(current_song_index > 0){
        current_song_index--;
    }
    else{
        current_song_index = current_playlist.size() - 1;
    }
    play();
}

bool SpotifyPlayer::isPlaying() const{
    return is_playing;
}

bool SpotifyPlayer::isPaused() const{
    return is_paused;
}

SpotifyPlayer::~SpotifyPlayer(){
    stopPlayback();
}
