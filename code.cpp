#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

class Maze {
private:
    vector<vector<int>> grid;
    int size;

public:
    Maze(int mazeSize) : size(mazeSize) {
        grid.resize(size, vector<int>(size, 0)); 
        
        for (int i = 0; i < size; i++) {
            grid[0][i] = 1;
            grid[size-1][i] = 1;
            grid[i][0] = 1;
            grid[i][size-1] = 1;
        }
        
        int wallCount = size * size / 4;
        for (int i = 0; i < wallCount; i++) {
            int r = rand() % (size-2) + 1;
            int c = rand() % (size-2) + 1;
            
            if ((r != 1 || c != 1) && (r != size-2 || c != size-2)) {
                grid[r][c] = 1;
            }
        }
    }
    
    int getCell(int row, int col) const {
        if (row >= 0 && row < size && col >= 0 && col < size) {
            return grid[row][col];
        }
        return -1;
    }
    
    void setCell(int row, int col, int value) {
        if (row >= 0 && row < size && col >= 0 && col < size) {
            grid[row][col] = value;
        }
    }
    
    int getSize() const {
        return size;
    }
    
    void createPath(int startRow, int startCol, int exitRow, int exitCol) {
        for (int i = startRow; i <= exitRow; i++) {
            grid[i][startCol] = 0;
        }
        
        for (int j = startCol; j <= exitCol; j++) {
            grid[exitRow][j] = 0;
        }
    }
};

class Player {
private:
    int row;
    int col;

public:
    Player(int startRow, int startCol) : row(startRow), col(startCol) {}
    
    int getRow() const { return row; }
    int getCol() const { return col; }
    
    void moveTo(int newRow, int newCol) {
        row = newRow;
        col = newCol;
    }
};

class GameRenderer {
public:
    static void displayMaze(const Maze& maze, const Player& player, int exitRow, int exitCol) {
        cout << "\n";
        int size = maze.getSize();
        
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (i == player.getRow() && j == player.getCol()) {
                    cout << "P ";
                } else if (i == exitRow && j == exitCol) {
                    cout << "E ";
                } else {
                    int cell = maze.getCell(i, j);
                    switch (cell) {
                        case 0: cout << "  "; break;
                        case 1: cout << "██"; break;
                        default: cout << "  "; break;
                    }
                }
            }
            cout << "\n";
        }
        cout << "\nP = Player, E = Exit, ██ = Wall\n";
    }
    
    static void displayWelcomeMessage() {
        cout << "======= MAZE ESCAPE GAME =======\n";
        cout << "Find your way to the exit (E)!\n";
        cout << "Controls: w (up), s (down), a (left), d (right), q (quit)\n";
    }
    
    static void displayVictoryMessage() {
        cout << "Congratulations! You escaped the maze!\n";
    }
    
    static void displayInvalidMoveMessage() {
        cout << "Invalid move! Try again.\n";
    }
    
    static void displayQuitMessage() {
        cout << "Quitting game. Thanks for playing!\n";
    }
    
    static void displayEndMessage() {
        cout << "Thanks for playing!\n";
    }
};

class MazeGame {
private:
    Maze maze;
    Player player;
    int exitRow, exitCol;
    bool gameOver;
    
    bool isValidMove(int row, int col) const {
        if (row < 0 || row >= maze.getSize() || col < 0 || col >= maze.getSize()) {
            return false;
        }
        
        if (maze.getCell(row, col) == 1) {
            return false;
        }
        
        return true;
    }

public:
    MazeGame(int mazeSize = 10) : 
        maze(mazeSize),
        player(1, 1),
        exitRow(mazeSize - 2),
        exitCol(mazeSize - 2),
        gameOver(false)
    {
        srand(static_cast<unsigned int>(time(nullptr)));
        
        maze.createPath(1, 1, exitRow, exitCol);
    }
    
    void display() {
        GameRenderer::displayMaze(maze, player, exitRow, exitCol);
    }
    
    bool isGameOver() const {
        return gameOver;
    }
    
    bool processMove(char direction) {
        int newRow = player.getRow();
        int newCol = player.getCol();
        
        switch (direction) {
            case 'w': newRow--; break;
            case 's': newRow++; break;
            case 'a': newCol--; break;
            case 'd': newCol++; break;
            default: return false;
        }
        
        if (!isValidMove(newRow, newCol)) {
            return false;
        }
        
        if (newRow == exitRow && newCol == exitCol) {
            gameOver = true;
            player.moveTo(newRow, newCol);
            GameRenderer::displayVictoryMessage();
            return true;
        }
        
        player.moveTo(newRow, newCol);
        
        return true;
    }
};

int main() {
    GameRenderer::displayWelcomeMessage();
    
    int mazeSize;
    cout << "Enter maze size (8-20): ";
    cin >> mazeSize;
    
    if (mazeSize < 8) mazeSize = 8;
    if (mazeSize > 20) mazeSize = 20;
    
    MazeGame game(mazeSize);
    char move;
    
    while (!game.isGameOver()) {
        game.display();
        cout << "Enter your move: ";
        cin >> move;
        
        if (move == 'q') {
            GameRenderer::displayQuitMessage();
            break;
        }
        
        bool validMove = game.processMove(move);
        if (!validMove) {
            GameRenderer::displayInvalidMoveMessage();
        }
    }
    
    if (game.isGameOver()) {
        game.display();
        GameRenderer::displayEndMessage();
    }
    
    return 0;
}