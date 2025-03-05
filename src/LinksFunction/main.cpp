#include <iostream>
#include <vector>
#include <memory>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <regex>

using json = nlohmann::json;

// Song class definition
class Song {
public:
    std::string title;
    std::string artist;
    std::string url;
    int duration;

    Song(const std::string& t, const std::string& a, const std::string& u, int d)
        : title(t), artist(a), url(u), duration(d) {}
};

// Linked List Node
class SongNode {
public:
    std::shared_ptr<Song> song;
    SongNode* next;

    SongNode(std::shared_ptr<Song> s) : song(s), next(nullptr) {}
};

class PlaylistLinkedList {
private:
    SongNode* head;

public:
    PlaylistLinkedList() : head(nullptr) {}

    // Add a song to the end of the list
    void addSong(std::shared_ptr<Song> song) {
        SongNode* newNode = new SongNode(song);
        
        if (!head) {
            head = newNode;
            return;
        }

        SongNode* current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }

    // Print all songs in the playlist
    void printPlaylist() {
        SongNode* current = head;
        int index = 1;
        while (current) {
            std::cout << index++ << ". " 
                      << current->song->title << " by " 
                      << current->song->artist << std::endl;
            current = current->next;
        }
    }

    // Destructor to free memory
    ~PlaylistLinkedList() {
        while (head) {
            SongNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

class Link {
public:
    // Your existing Spotify-related methods
    static size_t WriteCallBack(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    static std::string base64_encode(const std::string& input) {
        static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string encoded;
    unsigned char const* bytes = reinterpret_cast<unsigned char const*>(input.data());
    int input_len = input.size();
    int i = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (input_len--) {
        char_array_3[i++] = *(bytes++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                encoded += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (int j = i; j < 3; j++) char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (int j = 0; (j < i + 1); j++)
            encoded += base64_chars[char_array_4[j]];

        while ((encoded.size() % 4) != 0)
            encoded += '=';
    }

    return encoded;

    }

    static std::string extractSpotifyID(const std::string& link) {
        std::regex regex(R"(playlist/([a-zA-Z0-9]+))");
    std::smatch match;

    if (std::regex_search(link, match, regex)) {
        return match[1].str();
    }
    return "";
    }

    static std::string getSpotifyToken() {
        CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        std::string clientID = "a9d05a8354a14563adb3054e7d7c0e80";
        std::string clientSecret = "a6ee0877be3142a4bfb8f9ed738cd97a";
        std::string auth = clientID + ":" + clientSecret;

        std::string authHeader = "Authorization: Basic " + 
                                 Link::base64_encode(auth);

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, authHeader.c_str());
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

        curl_easy_setopt(curl, CURLOPT_URL, "https://accounts.spotify.com/api/token");
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "grant_type=client_credentials");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    // Print the response for debugging
    std::cout << "Token response: " << response << std::endl;

    if (response.empty()) {
        return "";
    }

    auto jsonData = json::parse(response);
    return jsonData["access_token"];

    }

    static std::string fetchSpotifyPlaylist(const std::string& playlistID) {
        std::string token = getSpotifyToken();
    std::string url = "https://api.spotify.com/v1/playlists/" + playlistID;

    CURL* curl = curl_easy_init();

    if (!curl) {
        std::cerr << "CURL initialization failed!" << std::endl;
        return "";
    }
    std::string response;

    if (curl) {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + token).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "CURL request failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    std::cout << "Response from Spotify API: " << response << std::endl;

    return response;
    }

    static std::vector<std::shared_ptr<Song>> parseSpotifyData(const std::string& jsonResponse) {
        auto jsonData = json::parse(jsonResponse);
    std::vector<std::shared_ptr<Song>> songs;

    for (const auto& item : jsonData["tracks"]["items"]) {
        std::string title = item["track"]["name"];
        std::string artist = item["track"]["artists"][0]["name"];
        int duration = item["track"]["duration_ms"].get<int>() / 1000;  // Convert ms to seconds

        std::string url = item["track"]["external_urls"]["spotify"];

        std::shared_ptr<Song> newSong = std::make_shared<Song>(title, artist, url, duration);
        songs.push_back(newSong);

        std::cout << "Song added: " << title << " by " << artist << std::endl;
    }
    return songs;
    }

    // New method to import playlist directly into Linked List
    static PlaylistLinkedList importPlaylist(const std::string& playlistLink) {
        PlaylistLinkedList playlist;
        
        // Extract playlist ID
        std::string playlistID = extractSpotifyID(playlistLink);
        
        if (playlistID.empty()) {
            std::cerr << "Invalid Spotify playlist link!" << std::endl;
            return playlist;
        }

        // Fetch playlist data
        std::string jsonResponse = fetchSpotifyPlaylist(playlistID);
        
        if (jsonResponse.empty()) {
            std::cerr << "Failed to fetch playlist data!" << std::endl;
            return playlist;
        }

        // Parse songs and add to linked list
        std::vector<std::shared_ptr<Song>> songs = parseSpotifyData(jsonResponse);
        
        for (const auto& song : songs) {
            playlist.addSong(song);
        }

        return playlist;
    }
};

int main() {
    try {
        std::string playlistLink;
        std::cout << "Enter Spotify Playlist Link: ";
        std::getline(std::cin, playlistLink);

        PlaylistLinkedList myPlaylist = Link::importPlaylist(playlistLink);
        
        std::cout << "\nImported Playlist:" << std::endl;
        myPlaylist.printPlaylist();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
