#define DOUBLY_LINKED_LIST_
#include <string>


struct Song{
    std::string title_;
    std::string artist_;
    int duration_;
    std::string youtubeLink_;
    Song* previous_;
    Song* next_;

    Song(std::string title, std::string artist, std::string youtubeLink, int duration){
        title_ = title;
        artist_ = artist;
        youtubeLink_ = youtubeLink;
        duration_ = duration;
        previous_ = nullptr;
        next_ = nullptr;
    }
};

class DoublyLinkedList{
private:
    Song* head;
    Song* tail;
    int size;

public:
    DoublyLinkedList();

    bool addSong(Song* song);
    bool addSong(Song* song, const int& position);

    bool removeSong(const std::string& title);
    bool removeSong(const int& position);

};


