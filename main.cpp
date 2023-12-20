#include <bits/stdc++.h>
#include <thread>

using namespace std;

const int boardSize = 8;
char board[boardSize][boardSize];
int shipHits = 0;
int totalShots = 0;

struct Player {
    string name;
    int shots;
};

bool isValidPlacement(int x, int y, int length, bool horizontal) {
    if (horizontal) {
        for (int i = 0; i < length; ++i) {
            if (y + i >= boardSize || board[x][y + i] == 'S') {
                return false;  // Invalid placement
            }
        }
    }
    else {
        for (int i = 0; i < length; ++i) {
            if (x + i >= boardSize || board[x + i][y] == 'S') {
                return false;  // Invalid placement
            }
        }
    }
    return true;  // Valid placement
}

void placeShip(int x, int y, int length, bool horizontal) {
    if (horizontal) {
        for (int i = 0; i < length; ++i) {
            board[x][y + i] = 'S';
        }
    }
    else {
        for (int i = 0; i < length; ++i) {
            board[x + i][y] = 'S';
        }
    }
}

void initializeBoard() {
    srand(time(0));
    for (int i = 0; i < boardSize; ++i)
        for (int j = 0; j < boardSize; ++j)
            board[i][j] = ' ';

    // Place a ship of length 3
    int x = rand() % (boardSize - 2);
    int y = rand() % boardSize;
    while (!isValidPlacement(x, y, 3, true)) {
        x = rand() % (boardSize - 2);
        y = rand() % boardSize;
    }
    placeShip(x, y, 3, true);

    // Place two ships of length 2
    for (int i = 0; i < 2; i++) {
        x = rand() % boardSize;
        y = rand() % (boardSize - 1);
        while (!isValidPlacement(x, y, 2, true)) {
            x = rand() % boardSize;
            y = rand() % (boardSize - 1);
        }
        placeShip(x, y, 2, true);
    }

    // Place four ships of length 1
    for (int i = 0; i < 4; i++) {
        x = rand() % boardSize;
        y = rand() % boardSize;
        while (!isValidPlacement(x, y, 1, false)) {
            x = rand() % boardSize;
            y = rand() % boardSize;
        }
        placeShip(x, y, 1, false);
    }
}

void printBoard() {
    // Print column numbers
    cout << "  ";
    for (int j = 0; j < boardSize; ++j)
        cout << j << ' ';
    cout << endl;

    for (int i = 0; i < boardSize; ++i) {
        // Print row number
        cout << i << ' ';

        // Print board cells
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] == 'X')
                cout << 'X';
            else if (board[i][j] == '*')
                cout << '*';
            else
                cout << '.';
            cout << ' ';
        }
        cout << endl;
    }
}

bool isValidMove(int row, int col) {
    // Check if the move is valid
    return row >= 0 && row < boardSize && col >= 0 && col < boardSize && board[row][col] != '*' && board[row][col] != 'X';
}

bool isGameOver() {
    // Check if the game is over
    return shipHits == 11;
}

void clearScreen() {
    cout << "\033[2J\033[H";
}

void playGame(const string& playerName) {
    initializeBoard();

    while (!isGameOver()) {
        int row, col;
        clearScreen();
        printBoard();

        cout << playerName << ", enter your guess (row, col): ";
        cin >> row >> col;
        totalShots++;

        while (!isValidMove(row, col)) {
            cout << "Invalid move!" << endl << "Please enter a new guess(row, col) : ";
            cin >> row >> col;
            totalShots++;
        }

        if (board[row][col] == 'S') {
            cout << "Hit!" << endl;
            board[row][col] = 'X';
            shipHits++;

            if (shipHits == 11) {
                clearScreen();
                printBoard();
                cout << "Sunk!" << endl;
                this_thread::sleep_for(chrono::milliseconds(400));
            }
            else {

                this_thread::sleep_for(chrono::milliseconds(400));
            }
        }
        else {
            cout << "Miss!" << endl;
            board[row][col] = '*';
            this_thread::sleep_for(chrono::milliseconds(400));
        }
    }

    cout << "Congratulations, " << playerName << "! You sank all the ships. Total shots: " << totalShots << endl;

    // Add a message and wait for user input before exiting
    cout << "Press Enter to exit...";
    cin.ignore();
    cin.get();
}


bool comparePlayers(const Player& player1, const Player& player2) {
    return player1.shots < player2.shots;
}

void displayPlayerList(const vector<Player>& players) {
    cout << "Player List (sorted by shots):" << endl;
    cout << "--------------------------------" << endl;
    cout << "Name\tShots" << endl;
    for (const Player& player : players) {
        cout << player.name << "\t" << player.shots << endl;
    }
    cout << "--------------------------------" << endl;
}

int main() {
    vector<Player> players;  // Store player information for sorting

    do {
        string playerName;
        cout << "Enter your name: ";
        cin >> playerName;

        playGame(playerName);

        // Store player information for sorting
        Player currentPlayer;
        currentPlayer.name = playerName;
        currentPlayer.shots = totalShots;
        players.push_back(currentPlayer);

        // Ask the player whether to start another game
        cout << "Do you want to start another game? (y/n): ";
        char playAgain;
        cin >> playAgain;

        if (playAgain != 'y' && playAgain != 'Y') {
            // Sort the player list based on shots
            sort(players.begin(), players.end(), comparePlayers);

            // Display a sorted list of all players
            displayPlayerList(players);

            // Add a message and wait for user input before exiting
            cout << "Press Enter to exit.";
            cin.ignore();
            cin.get();

            return 0;
        }

        // Reset the game variables for a new game
        shipHits = 0;
        totalShots = 0;

    } while (true);

    return 0;
}
