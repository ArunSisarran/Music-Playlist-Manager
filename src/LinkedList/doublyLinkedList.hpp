#ifndef DOUBLY_LINKED_LIST_HPP
#define DOUBLY_LINKED_LIST_HPP
#include <memory>
#include <string>
#include <vector>


struct Song{
    std::string title_;
    std::string artist_;
    std::string link_;
    int duration_;
    std::shared_ptr<Song> previous_;
    std::shared_ptr<Song> next_;

    Song(std::string title,std::string artist, std::string link, int duration){
        title_ = title;
        duration_ = duration;
        artist_ = artist;
        link_ = link;
        previous_ = nullptr;
        next_ = nullptr;
    }
};

class DoublyLinkedList{
private:
    std::shared_ptr<Song> head;
    std::shared_ptr<Song> tail;
    int size;

public:
//Default Constructor
    DoublyLinkedList();
//Adds a song pointer to the linked list
    bool addSong(std::shared_ptr<Song> song);
//Adds a song pointer to the linked list at the specified position
    bool addSong(std::shared_ptr<Song> song, const int& position);
//Removes a song from the linked list based on the specified title of the song
    bool removeSong(const std::string& title);
//Searches and returns the song pointer of the specified title 
    std::shared_ptr<Song> searchForSong(const std::string& title);
//Displays current linked list
    void display() const;
//Saves the playlist to a json file
    void savePlaylist() const;
//Loads a playist that has been previously saved
    void loadPlaylist();
//Converts the linked list into a vector
    std::vector<std::string> getPlaylist();
//Getter method for the head of the list
    std::shared_ptr<Song>getHead() const;
//Getter method for the tail of the list
    std::shared_ptr<Song>getTail() const;
    int getSize() const;
};

#endif
