#ifndef LINK_HPP
#define LINK_HPP

#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include "../LinkedList/doublyLinkedList.hpp"

class Link{

public:

    //YouTube Integration
    static size_t WriteCallBack(void* contents, size_t size, size_t nmemb, void* userp);
    static int convertDuration(const std::string& duration);
    static std::string fetchLinkData(const std::string link);  
    static std::string extractVideoID(const std::string& link);
    static std::shared_ptr<Song>parseLinkData(const std::string& data);

    //Spotify Integration
    static std::string base64_encode(const std::string& input);
    static std::string extractSpotifyID(const std::string& link);
    static std::string getSpotifyToken();
    static std::string fetchSpotifyPlaylist(const std::string& playlistID);
    static std::vector<std::shared_ptr<Song>> parseSpotifyData(const std::string& jsonResponse);
    static DoublyLinkedList importPlaylist(const std::string& playlistLink);
};
#endif // !LINK_HPP
