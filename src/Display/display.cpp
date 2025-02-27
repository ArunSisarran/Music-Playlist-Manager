#include <memory>
#include <ncurses.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <vector>
#include "display.hpp"
#include "../LinkedList/doublyLinkedList.hpp"

#define HELP_MSG "Arrow Keys: Navigate | q: Quit | a: Add Song"

display::display() {
    initscr();                  
    start_color();               
    keypad(stdscr, TRUE);        
    noecho();                    
    curs_set(0);                 
    cbreak();                    

    // Color pairs normal and highlighted
    init_pair(1, COLOR_WHITE, COLOR_BLACK);  
    init_pair(2, COLOR_BLACK, COLOR_GREEN); 
}


void display::start(DoublyLinkedList& playlist) {
    int height, width;
    getmaxyx(stdscr, height, width);

    displayPlaylist(playlist, 0);

    int key, selected = 0, offset = 0;
    while (true) {
        key = getch();

        switch (key) {
            case 'a':  // Add song
                addSong(playlist);
                displayPlaylist(playlist, selected); 
                break;

            case KEY_UP:  // Move up
                if (selected > 0) {
                    selected--;
                    if (selected < offset) offset--; 
                }
                displayPlaylist(playlist, selected);  
                break;

            case KEY_DOWN:  // Move down
                if (selected < playlist.getSize() - 1) {
                    selected++;
                    if (selected >= offset + height - 3) offset++;  
                }
                displayPlaylist(playlist, selected); 
                break;

            case 'q':  // Quit
            case 27:   // ESC to quit
                endwin();
                return;
        }
    }
}



void display::displayPlaylist(DoublyLinkedList& playlist, int selected) {
    int height, width;
    getmaxyx(stdscr, height, width);

    std::shared_ptr<Song> current = playlist.getHead();
    int visibleRows = height - 3, offset = 0, row = 0, displayRow = 0;

    clear();  
    if (current == nullptr) {
        mvprintw(height / 2, 1, "Playlist is empty!");  // Display empty message
        refresh();
        return;
    }

    // Move to offset position
    while (row < offset && current != nullptr) {
        current = current->next_;
        row++;
    }

    // Display songs with highlighting for selected song
    while (current != nullptr && displayRow < visibleRows) {
        if (displayRow == selected) {
            attron(A_REVERSE);  // Highlight the selected song
        }

        std::string songInfo = current->title_ + " - " + current->artist_;
        mvprintw(displayRow, 1, "%s", songInfo.c_str());

        if (displayRow == selected) {
            attroff(A_REVERSE);  // Turn off highlight
        }

        current = current->next_;
        displayRow++;
    }

    mvprintw(height - 2, (width - strlen(HELP_MSG)) / 2, HELP_MSG);
    refresh();
}


void display::addSong(DoublyLinkedList& playlist) {
    int height, width;
    getmaxyx(stdscr, height, width);

    WINDOW* addSongWin = newwin(10, width - 2, height / 4, 1); 
    box(addSongWin, 0, 0); 
    wrefresh(addSongWin);

    std::shared_ptr<Song> newSong = std::make_shared<Song>("", "", "", 0);

    char buffer[256];

    // Title input
    mvwprintw(addSongWin, 1, 1, "Enter Title: ");
    wrefresh(addSongWin);
    move(height / 4 + 1, 15);  
    echo();
    getstr(buffer);
    newSong->title_ = std::string(buffer);

    // Artist input
    mvwprintw(addSongWin, 3, 1, "Enter Artist: ");
    wrefresh(addSongWin);
    move(height / 4 + 3, 16);  
    getstr(buffer);
    newSong->artist_ = std::string(buffer);

    // Link input
    mvwprintw(addSongWin, 5, 1, "Enter Link: ");
    wrefresh(addSongWin);
    move(height / 4 + 5, 14);  
    getstr(buffer);
    newSong->youtubeLink_ = std::string(buffer);

    // Duration input
    mvwprintw(addSongWin, 7, 1, "Enter Duration (seconds): ");
    wrefresh(addSongWin);
    move(height / 4 + 7, 28);  
    getstr(buffer);
    newSong->duration_ = std::stoi(buffer);

    noecho();

    playlist.addSong(newSong);

    mvprintw(height - 2, 1, "Song added to playlist!"); 
    refresh();
    sleep(1);

    delwin(addSongWin);  
}

