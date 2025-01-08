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
        size++;
        return true;
    }

    if(head != nullptr){
        Song* temp = head;

        while(temp->next_){
            temp = temp->next_;
        }

        temp->next_ = newSong;
        tail = newSong;
        newSong->next_ = nullptr;
        newSong->previous_ = temp;
        std::cout<<"Added song: "<<newSong->title_<<", By: "<<newSong->artist_<<std::endl;
        size++;
        return true;
    }
    return false;
}

bool DoublyLinkedList::addSong(Song* song, const int& position){
    if(position <= 0 || position > size){
        std::cout<<"Invalid position '"<<position<<"'"<<std::endl;
        return false;
    } 

    Song* newSong = new Song(song->title_,
                             song->artist_,
                             song->youtubeLink_,
                             song->duration_);
    if(position == 1){
        newSong->next_ = head;
        head->previous_ = newSong;
        head = newSong;
        size++;
        return true;
    }
    else if(position <= size){
        Song* current = head;
        int index = 1;

        while(index < position - 1 && current->next_){
            current = current->next_;
            index++;
        }

        newSong->next_ = current->next_;
        newSong->previous_ = current;
        current->next_ = newSong;
        size++;
        return true;
    }
    else if(position == size){
        tail->next_ = newSong;
        newSong->previous_ = tail;
        newSong->next_ = nullptr;
        tail = newSong;
        size++;
        return true;
    }
    return false;
}

bool DoublyLinkedList::removeSong(const std::string& title){
    Song* current = head;

    if(!head){
        std::cout<<"Playlist is empty"<<std::endl;
        return false;
    }

    while(current && current->title_ != title){
        current = current->next_;
    }

    if(!current){
        std::cout<<"Name not found in playlist"<<std::endl;
        return false;
    }

    if(current == head && current == tail){
        head = nullptr;
        tail = nullptr;
    }
    else if(current == head){
        head = current->next_;
        head->previous_ = nullptr;
    }
    else if(current == tail){
        tail = current->previous_;
        tail->next_ = nullptr;
    }else{
        current->previous_->next_ = current->next_;
        current->next_->previous_ = current->previous_;
    }

    delete current;
    size--;
    std::cout<<"Removed Song: "<<title<<std::endl;
    return true;
}

bool DoublyLinkedList::removeSong(const int& position){
    if(position <= 0 || position > size){
        std::cout<<"Invalid position '"<<position<<"'"<<std::endl;
        return false;
    }


}

void DoublyLinkedList::display() const{
    if (!head) {
        std::cout << "Playlist is empty"<<std::endl;
        return;
    }

    Song* current = head;
    int index = 1;

    while (current) {
        std::cout << index++ << ". " << current->title_ << " by " << current->artist_ << " (" << current->duration_ << " seconds)"<<std::endl;
        current = current->next_;
    }
}
