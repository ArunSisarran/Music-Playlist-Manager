#include "../src/LinkedList/doublyLinkedList.hpp"
#include <iostream>
#include <memory>
#include "../src/Display/display.hpp"

int main(){

    DoublyLinkedList playlist = DoublyLinkedList();
    display screen = display();
    auto song1 = std::make_shared<Song>("title 2", "artist 2", "link 2", 40);
//    auto song2 = std::make_shared<Song>("title 2", "artist 2", "link 2", 40);
//    auto song3 = std::make_shared<Song>("title 3", "artist 3", "link 3", 40);
//    auto song4 = std::make_shared<Song>("title 4", "artist 4", "link 4", 40);
//    auto song5 = std::make_shared<Song>("title 5", "artist 5", "link 5", 40);
//    auto song6 = std::make_shared<Song>("title 6", "artist 6", "link 6", 40);
//    auto song7 = std::make_shared<Song>("title 6", "artist 6", "link 6", 40);
//    auto song8 = std::make_shared<Song>("title 6", "artist 6", "link 6", 40);
//    auto song9 = std::make_shared<Song>("title 6", "artist 6", "link 6", 40);
//    auto song0 = std::make_shared<Song>("title 6", "artist 6", "link 6", 40);
//
    playlist.addSong(song1);
//    playlist.addSong(song2);
//    playlist.addSong(song3);
//    playlist.addSong(song4);
//    playlist.addSong(song5);
//    playlist.addSong(song6);
//
//    playlist.addSong(song7);
//    playlist.addSong(song8);
//    playlist.addSong(song9);
//    playlist.addSong(song0);


    screen.start(playlist);
    return 0;
}
