#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

vector<vector<char>> board(3, vector<char>(3, ' '));

enum Archetype { PALADIN, ALCHEMIST, NONE };
struct Player {
    char mark;
    Archetype archetype;
};

void displayBoard() {
    cout << "\n";
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cout << (j ? "|" : "") << (board[i][j] == ' ' ? ' ' : board[i][j]);
        }
        cout << "\n" << (i < 2 ? "-----\n" : "");
    }
    cout << "\n";
}

void resetBoard() {
    for (int i = 0; i < 3; ++i) {
        fill(board[i].begin(), board[i].end(), ' ');
    }
}

bool isEmpty(int row, int col) {
    return board[row][col] == ' ';
}

bool checkWin(char mark) {
    for (int i = 0; i < 3; ++i)
        if ((board[i][0] == mark && board[i][1] == mark && board[i][2] == mark) ||
            (board[0][i] == mark && board[1][i] == mark && board[2][i] == mark))
            return true;

    return (board[0][0] == mark && board[1][1] == mark && board[2][2] == mark) ||
           (board[0][2] == mark && board[1][1] == mark && board[2][0] == mark);
}

char promptMark() {
    char mark;
    do {
        cout << "Enter your mark (A-Z, a-z, ?, !, *, ~, $, %, #): ";
        cin >> mark;
    } while (!isalnum(mark) && mark != '?' && mark != '!' && mark != '*' && mark != '~' && mark != '$' && mark != '%' && mark != '#');
    return mark;
}

Archetype chooseArchetype() {
    int choice;
    do {
        cout << "Choose an archetype:\n1. Paladin\n2. Alchemist\nChoice: ";
        cin >> choice;
    } while (choice < 1 || choice > 2);
    return choice == 1 ? PALADIN : ALCHEMIST;
}

bool makeMove(int row, int col, char mark) {
    if (row < 0 || row > 2 || col < 0 || col > 2 || !isEmpty(row, col)) {
        cout << "Invalid move. Try again.\n";
        return false;
    }
    board[row][col] = mark;
    return true;
}

bool alchemistSwap() {
    int x1, y1, x2, y2;
    cout << "Enter the coordinates of the first mark to swap (row and column): ";
    cin >> x1 >> y1;
    cout << "Enter the coordinates of the second mark to swap (row and column): ";
    cin >> x2 >> y2;
    if (board[x1][y1] != ' ' && board[x2][y2] != ' ' && board[x1][y1] != board[x2][y2]) {
        swap(board[x1][y1], board[x2][y2]);
        return true;
    } else {
        cout << "Invalid swap. Try again.\n";
        return false;
    }
}

bool paladinShift(char playerMark) {
    int x, y, newX, newY;
    cout << "Enter the coordinates of the mark to shift: ";
    cin >> x >> y;
    cout << "Enter the coordinates to shift it to: ";
    cin >> newX >> newY;
    if (board[x][y] != ' ' && isEmpty(newX, newY) && abs(x - newX) <= 1 && abs(y - newY) <= 1) {
        board[newX][newY] = board[x][y];
        board[x][y] = ' ';
        return true;
    } else {
        cout << "Invalid shift. Try again.\n";
        return false;
    }
}

void playGame(Player player1, Player player2) {
    bool gameOver = false;
    int turns = 0;
    Player currentPlayer = player1;
    
    while (!gameOver && turns < 9) {
        displayBoard();
        cout << "Player " << (currentPlayer.mark == player1.mark ? "1" : "2") << " (" << currentPlayer.mark << ") - ";
        
        if (currentPlayer.archetype == ALCHEMIST) {
            cout << "Choose move: 1. Regular move 2. Alchemist swap: ";
        } else if (currentPlayer.archetype == PALADIN) {
            cout << "Choose move: 1. Regular move 2. Paladin shift: ";
        } else {
            cout << "Choose move: 1. Regular move: ";
        }

        int choice;
        cin >> choice;
        if (choice == 1) {
            int row, col;
            cout << "Enter row and column: ";
            cin >> row >> col;
            if (makeMove(row, col, currentPlayer.mark)) {
                turns++;
                if (checkWin(currentPlayer.mark)) {
                    displayBoard();
                    cout << "Player " << (currentPlayer.mark == player1.mark ? "1" : "2") << " wins!\n";
                    gameOver = true;
                }
            }
        } else if (choice == 2) {
            if ((currentPlayer.archetype == ALCHEMIST && alchemistSwap()) ||
                (currentPlayer.archetype == PALADIN && paladinShift(currentPlayer.mark))) {
                if (checkWin(currentPlayer.mark)) {
                    displayBoard();
                    cout << "Player " << (currentPlayer.mark == player1.mark ? "1" : "2") << " wins!\n";
                    gameOver = true;
                }
            }
        } else {
            cout << "Invalid choice. Try again.\n";
        }
        
        currentPlayer = (currentPlayer.mark == player1.mark) ? player2 : player1;
    }
    
    if (!gameOver) {
        displayBoard();
        cout << "It's a draw!\n";
    }
}

int main() {
    char playAgain;
    do {
        cout << "Welcome to Tic-Tac-Toe!\nChoose game type:\n1. Regular\n2. Battle\nChoice: ";
        int gameType;
        cin >> gameType;

        resetBoard();
        Player player1 = {promptMark(), gameType == 2 ? chooseArchetype() : NONE};
        Player player2 = {promptMark(), gameType == 2 ? chooseArchetype() : NONE};

        playGame(player1, player2);

        cout << "Play again? (y/n): ";
        cin >> playAgain;
    } while (playAgain == 'y' || playAgain == 'Y');

    cout << "Thanks for playing!\n";
    return 0;
}