#include <ncurses.h>
#include <string>
#include <unistd.h>
#include <vector>
#include "display.hpp"
#include "doublyLinkedList.hpp"

display::display(){

    initscr();
    start_color();
    noecho();
    curs_set(0);
}
void display::displayPlaylist(DoublyLinkedList& playlist){

    std::vector<std::string>playlistVector = playlist.getPlaylist();

    int row = 0;

    for(const auto& str : playlistVector){
        mvprintw(row, 0, str.c_str());
        row++;
    }

    refresh();
    getch();
    endwin();
}
