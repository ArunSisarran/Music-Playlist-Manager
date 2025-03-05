#include "link.hpp"
#include <iostream>
#include "../LinkedList/doublyLinkedList.hpp"
#include <memory>
#include <string>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <curl/easy.h>
#include <regex>
#include <vector>
#include "../../DotEnv.cpp"

using json = nlohmann::json;


size_t Link::WriteCallBack(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


//int Link::convertDuration(const std::string& duration) {
//    std::regex regex(R"(PT(\d+M)?(\d+S)?)"); // Regex to match minutes and seconds
//    std::smatch match;
//
//    int minutes = 0, seconds = 0;
//
//    if (std::regex_search(duration, match, regex)) {
//        if (match[1].matched) { // If minutes are present
//            minutes = std::stoi(match[1].str().substr(0, match[1].str().size() - 1)); // Remove 'M' and convert to int
//        }
//        if (match[2].matched) { // If seconds are present
//            seconds = std::stoi(match[2].str().substr(0, match[2].str().size() - 1)); // Remove 'S' and convert to int
//        }
//    }
//
//    int result = (minutes*60)+seconds;
//    return result;
//}
//
//std::string Link::extractVideoID(const std::string& link) {
//    std::regex regex(R"(v=([a-zA-Z0-9_-]+))");  // Regex to match the video ID from URL
//    std::smatch match;
//
//    if (std::regex_search(link, match, regex)) {
//        return match[1].str();  // Return the first capture group (video ID)
//    }
//    return "";  // Return empty if no match
//}
//
//std::string Link::fetchLinkData(const std::string link) {
//    std::string url = "https://www.googleapis.com/youtube/v3/videos?id=" + link + 
//                      "&part=snippet,contentDetails&key=" + "AIzaSyAH5y7NaX5rdDAydnqa-npZXB4oP8mI1Kg";
//    
//    CURL* curl = curl_easy_init();
//    std::string response;
//
//    if (curl) {
//        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
//        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
//        curl_easy_perform(curl);
//        curl_easy_cleanup(curl);
//    }
//    return response;
//}
//
//std::shared_ptr<Song>Link::parseLinkData(const std::string& data) {
//    auto jsonData = json::parse(data);
//    if (jsonData.contains("items") && !jsonData["items"].empty()) {
//        auto snippet = jsonData["items"][0]["snippet"];
//        auto contentDetails = jsonData["items"][0]["contentDetails"];
//
//        std::string title = snippet["title"];
//        std::string duration = contentDetails["duration"];
//
//        std::cout << "Title: " << title << std::endl;
//        std::cout << "Duration: " << convertDuration(duration) << std::endl;
//
//        std::shared_ptr<Song> newSong = std::make_shared<Song>(title,convertDuration(duration));
//        return newSong;
//    }
//    return 0;
//}



std::string Link::base64_encode(const std::string& input) {
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

std::string Link::extractSpotifyID(const std::string& link) {
    std::regex regex(R"(playlist/([a-zA-Z0-9]+))");
    std::smatch match;

    if (std::regex_search(link, match, regex)) {
        return match[1].str();
    }
    return "";
}

std::string Link::getSpotifyToken() {
    
    static DotEnv env;

    std::string clientID = env.get("SPOTIFY_CLIENT_ID");
    std::string clientSecret = env.get("SPOTIFY_CLIENT_SECRET");

    // Validate credentials
    if (clientID.empty() || clientSecret.empty()) {
        std::cerr << "Error: Spotify credentials not found. "
                  << "Please check your .env file." << std::endl;
        return "";
    }

    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
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
//    std::cout << "Token response: " << response << std::endl;

    if (response.empty()) {
        return "";
    }

    auto jsonData = json::parse(response);
    return jsonData["access_token"];
}

std::string Link::fetchSpotifyPlaylist(const std::string& playlistID) {
    std::string token = getSpotifyToken();
    std::string url = "https://api.spotify.com/v1/playlists/" + playlistID;

    CURL* curl = curl_easy_init();

    if (!curl) {
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

 
        curl_easy_cleanup(curl);
    }

 //   std::cout << "Response from Spotify API: " << response << std::endl;

    return response;
}


std::vector<std::shared_ptr<Song>> Link::parseSpotifyData(const std::string& jsonResponse) {
    auto jsonData = json::parse(jsonResponse);
    std::vector<std::shared_ptr<Song>> songs;

    for (const auto& item : jsonData["tracks"]["items"]) {
        std::string title = item["track"]["name"];
        std::string artist = item["track"]["artists"][0]["name"];
        int duration = item["track"]["duration_ms"].get<int>() / 1000;  // Convert ms to seconds

        std::string url = item["track"]["external_urls"]["spotify"];

        std::shared_ptr<Song> newSong = std::make_shared<Song>(title, artist, url, duration);
        songs.push_back(newSong);

//        std::cout << "Song added: " << title << " by " << artist << std::endl;
    }
    return songs;
}

//testing purposes
DoublyLinkedList Link::importPlaylist(const std::string& playlistLink) {
        DoublyLinkedList playlist;
        
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


