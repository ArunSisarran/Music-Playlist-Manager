#include "doublyLinkedList.hpp"
#include <iostream>
#include <memory>
#include "display.hpp"

int main(){

    DoublyLinkedList playlist = DoublyLinkedList();
    auto song1 = std::make_shared<Song>("title 1", "artist 1", "link 1", 40);
    auto song2 = std::make_shared<Song>("title 2", "artist 2", "link 2", 40);
    auto song3 = std::make_shared<Song>("title 3", "artist 3", "link 3", 40);
    auto song4 = std::make_shared<Song>("title 4", "artist 4", "link 4", 40);
    auto song5 = std::make_shared<Song>("title 5", "artist 5", "link 5", 40);
    auto song6 = std::make_shared<Song>("title 6", "artist 6", "link 6", 40);

    playlist.addSong(song1);
    playlist.addSong(song2);
    playlist.addSong(song3);
    playlist.addSong(song4);
    playlist.addSong(song5);
    playlist.addSong(song6, 3);

    playlist.display();

    playlist.removeSong("title 1");
    playlist.removeSong("title 5");
    playlist.removeSong("title 6");

    playlist.display();

    playlist.searchForSong("title 4");

    display screen = display();
    screen.displayPlaylist(playlist);

    return 0;
}
