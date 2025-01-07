#include "doublyLinkedList.hpp"
#include <string>
#include <iostream>

DoublyLinkedList::DoublyLinkedList(){
    head = nullptr;
    tail = nullptr;
}

bool DoublyLinkedList::addSong(Song* song){
    Song* newSong = new Song(song->title_,
                             song->artist_,
                             song->youtubeLink_,
                             song->duration_);

    if(!head){
        head = newSong;
        tail = newSong;
        std::cout<<"Added song: "<<newSong->title_<<", By: "<<newSong->artist_<<std::endl;
        return true;
    }

    if(head != nullptr){
        Song* temp;

        while(temp->next_){
            temp = temp->next_;
        }

        temp->next_ = newSong;
        tail = newSong;
        newSong->next_ = nullptr;
        newSong->previous_ = temp;
        std::cout<<"Added song: "<<newSong->title_<<", By: "<<newSong->artist_<<std::endl;

        return true;
    }
    return false;
}

bool DoublyLinkedList::addSong(Song* song, const int& position){
 
}

