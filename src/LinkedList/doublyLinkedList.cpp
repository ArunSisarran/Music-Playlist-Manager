#include "doublyLinkedList.hpp"
#include <memory>
#include <string>
#include <iostream>
#include <vector>

DoublyLinkedList::DoublyLinkedList(){
    head = nullptr;
    tail = nullptr;
    size = 0;
}

bool DoublyLinkedList::addSong(std::shared_ptr<Song> song){
    auto newSong = std::make_shared<Song>(song->title_,
                                          song->artist_,
                                          song->link_,
                                          song->duration_);
//if the list is empty
    if(!head){
        head = newSong;
        tail = newSong;
        std::cout<<"Added song: "<<newSong->title_<<std::endl;
        size++;
        return true;
    }

//if the list has items in it
    if(head != nullptr){
        std::shared_ptr<Song> temp = head;

        while(temp->next_){
            temp = temp->next_;
        }

        temp->next_ = newSong;
        tail = newSong;
        newSong->next_ = nullptr;
        newSong->previous_ = temp;
        size++;
        return true;
    }
    return false;
}

bool DoublyLinkedList::addSong(std::shared_ptr<Song> song, const int& position){
    if(position <= 0 || position > size){
        return false;
    } 

    auto newSong = std::make_shared<Song>(song->title_,
                                          song->artist_,
                                          song->link_,
                                          song->duration_);

    if(position == 1){
        newSong->next_ = head;
        head->previous_ = newSong;
        head = newSong;
        size++;
        return true;
    }
    else if(position <= size){
        std::shared_ptr<Song> current = head;
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

std::shared_ptr<Song> DoublyLinkedList::searchForSong(const std::string& title){
    std::shared_ptr<Song> current = head;

    if(!head){
        return nullptr;
    }

    while(current && current->title_ != title){
        current = current->next_;
    }

    if(!current){
        return nullptr;
    }

    return current;
}

void DoublyLinkedList::display() const{
    if (!head) {
        return;
    }

    std::shared_ptr<Song> current = head;
    int index = 1;

    while (current) {
        std::cout << index++ << ". " << current->title_ << " (" << current->duration_ << " seconds)"<<std::endl;
        current = current->next_;
    }
}

//TODO IMPLEMENT FUNCTION
bool DoublyLinkedList::removeSong(const std::string& title){
    std::shared_ptr<Song> current = searchForSong(title);

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

    size--;
    std::cout<<"Removed Song: "<<title<<std::endl;
    return true;
}

//testing purposes
std::vector<std::shared_ptr<Song>> DoublyLinkedList::getPlaylist(){
    std::vector<std::shared_ptr<Song>> result;
    auto current = getHead();
    while(current){
        result.push_back(current);
        current = current->next_;
    }
    return result;
}

std::shared_ptr<Song>DoublyLinkedList::getHead() const{
    return head;
}

std::shared_ptr<Song>DoublyLinkedList::getTail() const{
    return tail;
}

int DoublyLinkedList::getSize() const{
    return size;
}
