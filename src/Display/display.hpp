#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "../LinkedList/doublyLinkedList.hpp"
class display{

public:

    display();

    void displayPlaylist(DoublyLinkedList& playlist, int selected);
    void addSong(DoublyLinkedList& playlist);
    void start(DoublyLinkedList& playlist);
};

#endif

