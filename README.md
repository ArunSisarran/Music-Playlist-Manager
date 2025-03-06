# TermiTune

TermiTune is a terminal-based music player that imports playlists from YouTube and Spotify using their respective APIs. The program stores the imported playlists in a doubly linked list and provides a terminal GUI built with `ncurses`. Music playback happens directly in the terminal without requiring a browser, thanks to `librespot`.

## Features

- Import playlists from YouTube and Spotify APIs
- Store songs in a doubly linked list
- Terminal-based GUI with `ncurses`
- Play music directly in the terminal using `librespot`
- No external browser required for playback

## Tech Stack

- C++
- ncurses
- librespot
- YouTube API
- Spotify API

## Installation

### Prerequisites

- C++ Compiler (g++)
- ncurses library
- librespot
- Spotify API credentials

### Setup

1. Clone the repository:

   ```bash
   git clone https://github.com/ArunSisarran/TermiTune.git
   cd TermiTune
   ```

2. Install dependencies:

   #### Linux (Debian-based)

   ```bash
   sudo apt-get install libncurses5-dev libncursesw5-dev
   ```

3. Create a `.env` file in the root directory and add the following credentials:

   ```env
   SPOTIFY_USERNAME=your_spotify_username
   SPOTIFY_PASSWORD=your_spotify_password
   CLIENT_ID=your_spotify_client_id
   CLIENT_SECRET=your_spotify_client_secret
   ```

   You can obtain your Spotify client ID and secret from the [Spotify Developer Dashboard](https://developer.spotify.com/dashboard/).

4. Build the project:

   ```bash
   make
   ```

5. Run the program:

   ```bash
   make run
   ```

## Usage

- Navigate through the terminal interface using arrow keys.
- Press `p` to select a song and start or stop playback.
- Press `n` to play the next song.
- Press `b` to play the previous song.
- Use `q` to quit the application.
- Playlist songs are dynamically imported and managed in a doubly linked list.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request.

## License

This project is licensed under the Apache-2.0 license.

## Contact

For any inquiries, please contact arunsisarran@gmail.com.

