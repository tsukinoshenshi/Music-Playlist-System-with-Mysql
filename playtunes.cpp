#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

using namespace std;
using namespace sql;

void addSong(Connection *con) {
    string title, artist, genre, duration;
    cout << "Enter Song Title: ";
    getline(cin >> ws, title);
    cout << "Enter Artist Name: ";
    getline(cin >> ws, artist);
    cout << "Enter Genre: ";
    getline(cin >> ws, genre);
    cout << "Enter Duration (mm:ss): ";
    getline(cin >> ws, duration);

    PreparedStatement *pstmt = con->prepareStatement(
        "INSERT INTO songs(title, artist, genre, duration) VALUES (?, ?, ?, ?)");
    pstmt->setString(1, title);
    pstmt->setString(2, artist);
    pstmt->setString(3, genre);
    pstmt->setString(4, duration);
    pstmt->execute();
    cout << "✅ Song added successfully!" << endl;
    delete pstmt;
}

void viewSongs(Connection *con) {
    Statement *stmt = con->createStatement();
    ResultSet *res = stmt->executeQuery("SELECT * FROM songs");

    cout << "\n--- All Songs ---\n";
    while (res->next()) {
        cout << res->getInt("song_id") << " | "
             << res->getString("title") << " | "
             << res->getString("artist") << " | "
             << res->getString("genre") << " | "
             << res->getString("duration") << endl;
    }
    delete res;
    delete stmt;
}

int main() {
    sql::Connection *con = nullptr;

    try {
        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "rahul", "Agario@786");

        // Test connection
        if (con->isValid()) {
            cout << "✅ Connected to database successfully!\n";
        } else {
            cerr << "❌ Connection failed!\n";
            return 1;
        }

        con->setSchema("musicdb");

        int choice;
        do {
            cout << "\n=== Music Playlist System ===\n";
            cout << "1. Add Song\n2. View Songs\n3. Exit\nEnter choice: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1: addSong(con); break;
                case 2: viewSongs(con); break;
                case 3: cout << "Goodbye!\n"; break;
                default: cout << "Invalid choice!\n";
            }
        } while (choice != 3);

    } catch (sql::SQLException &e) {
        cerr << "MySQL Error [" << e.getErrorCode() << "]: " << e.what() << endl;
    } catch (std::exception &e) {
        cerr << "Error: " << e.what() << endl;
    }

    if (con) delete con;

    cout << "\nPress Enter to exit...";
    cin.get(); // Wait for user input before closing
    return 0;
}
