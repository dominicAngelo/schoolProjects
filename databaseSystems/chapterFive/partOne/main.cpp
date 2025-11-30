/* Program name: main.cpp
   Author: Dominic Angelo
   Date last updated: 11/28/25
   Purpose: Open and read from chinook database
*/

#include <iostream>
#include <string>
#include <sqlite3.h>    

int currentAlbumID = -1;

int albumCallback(void *data, int argc, char** argv, char** columnNames) {

    int albumID = atoi(argv[0]);
    std::string albumTitle;
    std::string artistName;
    std::string trackName;

    if (argv[1] != NULL && argv[2] != NULL && argv[3] != NULL) {
        albumTitle = argv[1];
        artistName = argv[2];
        trackName = argv[3];
    }

    if (currentAlbumID != albumID) {
        currentAlbumID = albumID;
        std::cout << std::endl << "AlbumId " << albumID << std::endl;
        std::cout << "Album Name: " << albumTitle << std::endl;
        std::cout << "Artist Name: " << artistName << std::endl; 
        std::cout << "Tracks: " << std::endl;
    }
     
    std::cout << "-" << trackName << std::endl;
    
    return 0;
}

int main () {
    sqlite3 *db;
    int runQuery = sqlite3_open("chinook.db", &db);

    if (runQuery != SQLITE_OK)   {
        std::cerr << "Error with chinook.db: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    } else {
        std::cout << "Database opened successfully" << std::endl;
    }

    const char *sql = 
        "SELECT "
        "albums.AlbumId, "
        "albums.Title, "
        "artists.Name, "
        "tracks.Name "
        "FROM albums "
        "JOIN artists ON albums.ArtistId = artists.ArtistId "
        "JOIN tracks ON tracks.AlbumId = albums.AlbumId "
        "ORDER BY albums.AlbumId, tracks.TrackId;";

    char *errorMsg = nullptr;

    runQuery = sqlite3_exec(db, sql, albumCallback, nullptr, &errorMsg);

    sqlite3_close(db);
    return 0;
}