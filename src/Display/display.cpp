#include <memory>
#include "../LinksFunction/link.hpp"
#include <ncurses.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <vector>
#include "display.hpp"
#include "../LinkedList/doublyLinkedList.hpp"
#include "../SpotifyPlayer/spotifyPlayer.hpp"

#define HELP_MSG "Arrow Keys: Navigate | q: Quit | a: Add Song | p: Play/Pause | n: Next Song | b: Previous Song"

display::display() {
    initscr();                  
    start_color();               
    keypad(stdscr, TRUE);        
    noecho();                    
    curs_set(0);                 
    cbreak();                    

    // Color pairs normal and highlighted
    init_pair(1, COLOR_WHITE, COLOR_BLACK);  
    init_pair(2, COLOR_BLACK, COLOR_WHITE); 
}

void display::start(DoublyLinkedList& playlist) {
    int height, width;
    getmaxyx(stdscr, height, width);
    SpotifyPlayer player;

    std::vector<std::shared_ptr<Song>>songs = playlist.getPlaylist();

    player.loadPlaylist(songs);

    int key, selected = 0, offset = 0;
    while (true) {
        displayPlaylist(playlist, selected, offset);

        key = getch();

        switch (key) {
            case 'a':  // Add song
                addSong(playlist);
                break;

            case KEY_UP:  // Move up
                if (selected > 0) {
                    selected--;
                    if (selected < offset) offset--; 
                }
                break;

            case KEY_DOWN:  // Move down
                if (selected < playlist.getSize() - 1) {
                    selected++;
                    int visibleRows = height - 3;
                    if (selected >= offset + visibleRows) offset++;  
                }
                break;

            case KEY_PPAGE:  // Page Up
                if (offset > 0) {
                    int visibleRows = height - 3;
                    offset = std::max(0, offset - visibleRows);
                    selected = offset;
                }
                break;

            case KEY_NPAGE:  // Page Down
                if (selected < playlist.getSize() - 1) {
                    int visibleRows = height - 3;
                    offset = std::min(static_cast<int>(playlist.getSize() - visibleRows), offset + visibleRows);
                    selected = offset;
                }
                break;

            case 'p': //Play/Pause
                if(!player.isPlaying())
                    player.play();
                else if(player.isPaused())
                    player.resume();
                else
                    player.pause();
                break;

            case 'n': //Next Track
                player.nextTrack();
                break;

            case 'b':
                player.previousTrack();
                break;

            case 'q':  // Quit
            case 27:   // ESC to quit
                player.stopPlayback();
                endwin();
                return;
        }
    }
}

void display::displayPlaylist(DoublyLinkedList& playlist, int& selected, int& offset) {
    int height, width;
    getmaxyx(stdscr, height, width);

    std::shared_ptr<Song> current = playlist.getHead();
    int visibleRows = height - 3;

    // Move to offset position
    for (int i = 0; i < offset && current != nullptr; i++) {
        current = current->next_;
    }

    clear();  
    if (current == nullptr) {
        mvprintw(height / 2, 1, "Playlist is empty!");
        refresh();
        return;
    }

    // Display songs with highlighting for selected song
    for (int displayRow = 0; 
         displayRow < visibleRows && current != nullptr; 
         displayRow++, current = current->next_) {
        
        int globalIndex = displayRow + offset;
        
        if (globalIndex == selected) {
            attron(COLOR_PAIR(2) | A_BOLD);  // Highlighted row
        } else {
            attron(COLOR_PAIR(1));  // Normal row
        }

        // Truncate or pad song info to fit width
        std::string songInfo = current->title_;
        if (static_cast<size_t>(width - 2) < songInfo.length()) {
            songInfo = songInfo.substr(0, static_cast<size_t>(width - 5)) + "...";
        }
        
        // Print song title
        mvprintw(displayRow, 1, "%s", songInfo.c_str());

        // Reset attributes
        if (globalIndex == selected) {
            attroff(COLOR_PAIR(2) | A_BOLD);
        } else {
            attroff(COLOR_PAIR(1));
        }
    }

    // Display total playlist info
    mvprintw(height - 2, 1, "Total Songs: %d | Showing: %d-%d", 
             playlist.getSize(), 
             offset + 1, 
             std::min(offset + visibleRows, static_cast<int>(playlist.getSize())));

    // Display help message centered
    mvprintw(height - 1, (width - strlen(HELP_MSG)) / 2, HELP_MSG);

    refresh();
}


void display::addSong(DoublyLinkedList& playlist) {
    int height, width;
    getmaxyx(stdscr, height, width);

    WINDOW* addSongWin = newwin(10, width - 2, height / 4, 1); 
    box(addSongWin, 0, 0); 
    wrefresh(addSongWin);

    char buffer[256] = {0};  // Initialize buffer to zero

    // Link input
    mvwprintw(addSongWin, 2, 1, "Enter Spotify Playlist Link:");
    mvwprintw(addSongWin, 4, 1, "Example: https://open.spotify.com/playlist/...");
    mvwprintw(addSongWin, 6, 1, "Link: ");
    wrefresh(addSongWin);
    
    echo();  
    wmove(addSongWin, 6, 7); // Move cursor to input position
    wgetstr(addSongWin, buffer);  
    noecho(); 

    std::string link(buffer);

    // Attempt to fetch and parse Spotify playlist
    try {
        // Extract playlist ID
        std::string playlistID = Link::extractSpotifyID(link);
        
        if (playlistID.empty()) {
            mvwprintw(addSongWin, 8, 1, "Invalid Spotify playlist link!");
            wrefresh(addSongWin);
            sleep(2);
            delwin(addSongWin);
            return;
        }

        // Fetch playlist data
        std::string jsonResponse = Link::fetchSpotifyPlaylist(playlistID);
        
        if (jsonResponse.empty()) {
            mvwprintw(addSongWin, 8, 1, "Failed to fetch playlist data!");
            wrefresh(addSongWin);
            sleep(2);
            delwin(addSongWin);
            return;
        }

        // Parse songs
        std::vector<std::shared_ptr<Song>> songs = Link::parseSpotifyData(jsonResponse);
        
        if (songs.empty()) {
            mvwprintw(addSongWin, 8, 1, "No songs found in the playlist!");
            wrefresh(addSongWin);
            sleep(2);
            delwin(addSongWin);
            return;
        }

        // Add songs to playlist
        for (const auto& song : songs) {
            playlist.addSong(song);
        }

        // Display success message
        mvwprintw(addSongWin, 8, 1, "%zu songs added to playlist!", songs.size());
        wrefresh(addSongWin);
        sleep(2);
    }
    catch (const std::exception& e) {
        // Handle any unexpected errors
        mvwprintw(addSongWin, 8, 1, "Error: %s", e.what());
        wrefresh(addSongWin);
        sleep(2);
    }

    delwin(addSongWin);
}
